/* ============================================================
* TabManager plugin for Falkon
* Copyright (C) 2013-2017  S. Razi Alavizadeh <s.r.alavizadeh@gmail.com>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
* ============================================================ */
#include "tabmanagerplugin.h"
#include "tabmanagerwidget.h"
#include "browserwindow.h"
#include "pluginproxy.h"
#include "mainapplication.h"
#include "sidebar.h"
#include "tabwidget.h"
#include "tabbar.h"
#include "tabmanagersettings.h"
#include "../config.h"

#include <QInputDialog>
#include <QSettings>
#include <QAction>
#include <QTimer>
#include <QMenu>

QString TabManagerPlugin::s_settingsPath;

TabManagerPlugin::TabManagerPlugin()
    : QObject()
    , m_controller(nullptr)
    , m_tabManagerWidget(nullptr)
    , m_viewType(Undefined)
    , m_initState(false)
    , m_asTabBarReplacement(false)
{
}

void TabManagerPlugin::init(InitState state, const QString &settingsPath)
{
    Q_UNUSED(state)

    m_controller = new TabManagerWidgetController(this);
    connect(mApp->plugins(), SIGNAL(mainWindowCreated(BrowserWindow*)), this, SLOT(mainWindowCreated(BrowserWindow*)));
    connect(mApp->plugins(), SIGNAL(mainWindowDeleted(BrowserWindow*)), m_controller, SLOT(mainWindowDeleted(BrowserWindow*)));
    connect(mApp->plugins(), SIGNAL(webPageCreated(WebPage*)), m_controller, SIGNAL(requestRefreshTree()));
    connect(mApp->plugins(), SIGNAL(webPageDeleted(WebPage*)), m_controller, SIGNAL(requestRefreshTree(WebPage*)));

    s_settingsPath = settingsPath + QL1S("/TabManager");
    m_initState = true;

    // load settings
    QSettings settings(s_settingsPath + QL1S("/tabmanager.ini"), QSettings::IniFormat);
    settings.beginGroup("View");
    m_controller->setGroupType(TabManagerWidget::GroupType(settings.value("GroupType", TabManagerWidget::GroupByWindow).toInt()));
    m_viewType = ViewType(settings.value("ViewType", ShowAsWindow).toInt());
    m_asTabBarReplacement = settings.value("AsTabBarReplacement", false).toBool();
    settings.endGroup();

    setAsTabBarReplacement(m_asTabBarReplacement);
    insertManagerWidget();
}

void TabManagerPlugin::unload()
{
    saveSettings();

    setTabBarVisible(true);
    removeManagerWidget();

    delete m_controller;
}

bool TabManagerPlugin::testPlugin()
{
    return (QString::fromLatin1(Qz::VERSION) == QLatin1String(FALKON_VERSION));
}

void TabManagerPlugin::showSettings(QWidget* parent)
{
    auto* settings = new TabManagerSettings(this, parent);
    settings->exec();
}

void TabManagerPlugin::populateExtensionsMenu(QMenu* menu)
{
    if (viewType() == ShowAsWindow) {
        QAction* showAction = m_controller->createMenuAction();
        showAction->setCheckable(false);
        connect(showAction, SIGNAL(triggered()), m_controller, SLOT(raiseTabManager()));
        menu->addAction(showAction);
    }
}

void TabManagerPlugin::insertManagerWidget()
{
    if (viewType() == ShowAsSideBar) {
        SideBarManager::addSidebar(QStringLiteral("TabManager"), m_controller);
    }
    else if (viewType() == ShowAsWindow) {
        if (!m_tabManagerWidget) {
            m_tabManagerWidget = m_controller->createTabManagerWidget(mApp->getWindow(), nullptr, true);
            m_tabManagerWidget->setWindowFlags(Qt::Window);
        }
    }

    if (m_initState) {
        const auto windows = mApp->windows();
        for (BrowserWindow* window : windows) {
            mainWindowCreated(window, false);
        }
        m_initState = false;
    }
}

void TabManagerPlugin::mainWindowCreated(BrowserWindow* window, bool refresh)
{
    if (window) {
        window->tabWidget()->tabBar()->setForceHidden(m_asTabBarReplacement);

        if (m_viewType == ShowAsWindow) {
            m_controller->addStatusBarIcon(window);
        }

        connect(window->tabWidget(), SIGNAL(currentChanged(int)), m_controller, SIGNAL(requestRefreshTree()));
        connect(window->tabWidget(), SIGNAL(pinStateChanged(int,bool)), m_controller, SIGNAL(requestRefreshTree()));
    }

    if (refresh) {
        m_controller->emitRefreshTree();
    }
}

void TabManagerPlugin::setTabBarVisible(bool visible)
{
    const auto windows = mApp->windows();
    for (BrowserWindow* window : windows) {
        window->tabWidget()->tabBar()->setForceHidden(!visible);
    }
}

void TabManagerPlugin::removeManagerWidget()
{
    if (viewType() == ShowAsSideBar) {
        SideBarManager::removeSidebar(m_controller);
    }
    else if (viewType() == ShowAsWindow) {
        // remove statusbar icon
        const auto windows = mApp->windows();
        for (BrowserWindow* window : windows) {
            m_controller->removeStatusBarIcon(window);
        }

        m_tabManagerWidget->close();
        delete m_tabManagerWidget;
        m_tabManagerWidget = nullptr;
    }
}


TabManagerPlugin::ViewType TabManagerPlugin::viewType()
{
    return m_viewType;
}

void TabManagerPlugin::setViewType(ViewType type)
{
    if (m_viewType != type) {
        removeManagerWidget();
        m_viewType  = type;
        insertManagerWidget();

        if (!m_initState) {
            if (m_viewType == ShowAsSideBar) {
                mApp->getWindow()->sideBarManager()->showSideBar(QStringLiteral("TabManager"));
            }
            else if (m_viewType == ShowAsWindow) {
                // add statusbar icon
                const auto windows = mApp->windows();
                for (BrowserWindow* window : windows) {
                    m_controller->addStatusBarIcon(window);
                }
            }
        }
    }
}

QString TabManagerPlugin::settingsPath()
{
    return s_settingsPath;
}

void TabManagerPlugin::saveSettings()
{
    QSettings settings(s_settingsPath + QL1S("/tabmanager.ini"), QSettings::IniFormat);
    settings.beginGroup("View");
    settings.setValue("GroupType", m_controller->groupType());
    settings.setValue("ViewType", viewType());
    settings.setValue("AsTabBarReplacement", asTabBarReplacement());
    settings.endGroup();
}

bool TabManagerPlugin::asTabBarReplacement() const
{
    return m_asTabBarReplacement;
}

void TabManagerPlugin::setAsTabBarReplacement(bool yes)
{
    m_asTabBarReplacement = yes;
    setTabBarVisible(!m_asTabBarReplacement);
}

