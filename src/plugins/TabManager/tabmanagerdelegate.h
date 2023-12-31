/* ============================================================
* Falkon - Qt web browser
* Copyright (C) 2016-2017  S. Razi Alavizadeh <s.r.alavizadeh@gmail.com>
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
#ifndef TABMANAGERDELEGATE_H
#define TABMANAGERDELEGATE_H

#include <QStyledItemDelegate>

class TabManagerDelegate : public QStyledItemDelegate
{
public:
    explicit TabManagerDelegate(QObject* parent = nullptr);

    void paint(QPainter* painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
    void viewItemDrawText(QPainter *p, const QStyleOptionViewItem *option, const QRect &rect,
                          const QString &text, const QColor &color,
                          const QString &searchText = QString()) const;

    QString m_filterText;

};

#endif // TABMANAGERDELEGATE_H
