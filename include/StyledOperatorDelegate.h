#pragma once

#include <QStyledItemDelegate>
#include <QApplication>
#include <QPainter>

class QStyledOperatorDelegate : public QStyledItemDelegate {
    Q_OBJECT

public:
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    // virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override {
    //     return QSize{50, 50};
    // }
};
