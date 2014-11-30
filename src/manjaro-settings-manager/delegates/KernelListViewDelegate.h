/*
 *  Manjaro Settings Manager
 *  Ramon Buldó <ramon@manjaro.org>
 *
 *  Copyright (C) 2007 Free Software Foundation, Inc.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef KERNEL_LIST_VIEW_DELEGATE_H
#define KERNEL_LIST_VIEW_DELEGATE_H

#include "models/KernelModel.h"

#include <QtGui/QMouseEvent>
#include <QtGui/QPainter>
#include <QtWidgets/QStyledItemDelegate>
#include <QtWidgets/QStyleOptionButton>

class KernelListViewDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    KernelListViewDelegate(QObject *parent = 0);
    virtual ~KernelListViewDelegate();

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);

signals:
    void installButtonClicked(const QModelIndex &index);
    void infoButtonClicked(const QModelIndex &index);

private:
    QStyle::State stateInstallButton_;
    QStyle::State stateInfoButton_;
};

#endif // KERNEL_LIST_VIEW_DELEGATE_H