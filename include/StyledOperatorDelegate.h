#pragma once

#include <QStyledItemDelegate>
#include <QApplication>
#include <QPainter>

class QStyledOperatorDelegate : public QStyledItemDelegate {
    Q_OBJECT

public:
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
    static void paintDisplayRole(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index);

    static void paintDecorationRole(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index);
};
