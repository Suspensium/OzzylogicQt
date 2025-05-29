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
    assert((iconVar.canConvert<QPair<QString, QIcon>>()));

    const auto [key, icon]{qvariant_cast<QPair<QString, QIcon> >(iconVar)};

    QPixmap iconPixmap;
    if (!QPixmapCache::find(key, &iconPixmap)) {
        iconPixmap = icon.pixmap(iconSize);
        QPixmapCache::insert(key, iconPixmap);
    }

    const QRect iconRect{option.rect.topLeft(), iconSize};
    painter->drawPixmap(iconRect, iconPixmap);
}
