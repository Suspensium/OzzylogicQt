#include "StyledOperatorDelegate.h"

#include <QTreeView>

#include "DataStructures.h"

extern const QSize iconSize;

void QStyledOperatorDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                                    const QModelIndex &index) const {
    painter->save();

    QStyleOptionViewItem opt{option};
    initStyleOption(&opt, index);

    option.widget->style()->drawPrimitive(QStyle::PE_PanelItemViewItem, &option, painter, option.widget);

    paintDisplayRole(painter, option, index);
    paintDecorationRole(painter, option, index);
    paintTreeLines(painter, option, index);

    painter->restore();
}

void QStyledOperatorDelegate::paintDisplayRole(QPainter *painter, const QStyleOptionViewItem &option,
                                               const QModelIndex &index) {
    const QVariant data{index.data(Qt::DisplayRole)}; // CountryInfo or OperatorInfo

    // Draw text
    const QString text{data.toString()};
    const QRect textRect{option.rect.adjusted(iconSize.width() + 10, 0, 0, 0)};
    painter->drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, text);
}

void QStyledOperatorDelegate::paintDecorationRole(QPainter *painter, const QStyleOptionViewItem &option,
                                                  const QModelIndex &index) {
    const QVariant iconVar{index.data(Qt::DecorationRole)}; // QPair<QString, QIcon>

    [[unlikely]] if (!iconVar.canConvert<QPair<QString, QIcon> >()) return;

    const auto [key, icon]{qvariant_cast<QPair<QString, QIcon> >(iconVar)};

    QPixmap iconPixmap;
    if (!QPixmapCache::find(key, &iconPixmap)) {
        iconPixmap = icon.pixmap(iconSize);
        QPixmapCache::insert(key, iconPixmap);
    }

    const QRect iconRect{option.rect.topLeft(), iconSize};
    painter->drawPixmap(iconRect, iconPixmap);
}

void QStyledOperatorDelegate::paintTreeLines(QPainter *painter, const QStyleOptionViewItem &option,
                                             const QModelIndex &index) {
    QPen pen{Qt::gray, 1, Qt::DotLine};
    painter->save();
    painter->setPen(pen);

    const QRect &r{option.rect};

    // Bottom dotted line under item
    painter->drawLine(r.left(), r.bottom(), r.right(), r.bottom());

    // Draw vertical connector on the left
    if (index.parent().isValid()) {
        if (const QTreeView *view{qobject_cast<const QTreeView *>(option.widget)}) {
            int depth{};
            QModelIndex temp{index.parent()};
            while (temp.isValid()) {
                ++depth;
                temp = temp.parent();
            }

            const int indent{view->indentation()};
            const int x{r.left() - indent / 2};
            const int y1{r.top()};
            const int y2{r.bottom()};

            // Vertical line connecting parent to this child
            painter->drawLine(x, y1, x, y2);

            // Horizontal line from vertical to entry
            painter->drawLine(x, r.center().y(), r.left(), r.center().y());
        }
    }
}
