/* ============================================================
* TabManager plugin for Falkon
* Copyright (C) 2013-2017  S. Razi Alavizadeh <s.r.alavizadeh@gmail.com>
* Copyright (C) 2018       David Rosca <nowrep@gmail.com>
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
#ifndef TABMANAGERWIDGETCONTROLLER_H
#define TABMANAGERWIDGETCONTROLLER_H

#include "sidebarinterface.h"
#include "tabmanagerwidget.h"

class WebPage;
class AbstractButtonInterface;

class TabManagerWidgetController : public SideBarInterface
{
    Q_OBJECT
public:
    explicit TabManagerWidgetController(QObject* parent = nullptr);
    ~TabManagerWidgetController() override;

    QString title() const override;
    QAction* createMenuAction() override;
    QWidget* createSideBarWidget(BrowserWindow* mainWindow) override;

    AbstractButtonInterface* createStatusBarIcon(BrowserWindow* mainWindow);

    TabManagerWidget::GroupType groupType();
    TabManagerWidget* createTabManagerWidget(BrowserWindow* mainClass, QWidget* parent = nullptr, bool defaultWidget = false);

    TabManagerWidget* defaultTabManager();

    void addStatusBarIcon(BrowserWindow* window);
    void removeStatusBarIcon(BrowserWindow* window);

public Q_SLOTS:
    void setGroupType(TabManagerWidget::GroupType type);
    void mainWindowDeleted(BrowserWindow* window);
    void raiseTabManager();
    void showSideBySide();
    void emitRefreshTree();

private:
    TabManagerWidget* m_defaultTabManager;
    TabManagerWidget::GroupType m_groupType;

    QHash<BrowserWindow*, AbstractButtonInterface*> m_statusBarIcons;
    QHash<BrowserWindow*, QAction*> m_actions;

Q_SIGNALS:
    void requestRefreshTree(WebPage* p = nullptr);
};

#endif // TABMANAGERWIDGETCONTROLLER_H
