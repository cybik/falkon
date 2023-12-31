/* ============================================================
* GreaseMonkey plugin for Falkon
* Copyright (C) 2012  David Rosca <nowrep@gmail.com>
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
#ifndef GM_SETTINGSSCRIPTINFO_H
#define GM_SETTINGSSCRIPTINFO_H

#include <QDialog>

namespace Ui
{
class GM_SettingsScriptInfo;
}

class GM_Script;

class GM_SettingsScriptInfo : public QDialog
{
    Q_OBJECT

public:
    explicit GM_SettingsScriptInfo(GM_Script* script, QWidget* parent = nullptr);
    ~GM_SettingsScriptInfo();

private Q_SLOTS:
    void editInTextEditor();
    void loadScript();

private:
    Ui::GM_SettingsScriptInfo* ui;

    GM_Script* m_script;
};

#endif // GM_SETTINGSSCRIPTINFO_H
