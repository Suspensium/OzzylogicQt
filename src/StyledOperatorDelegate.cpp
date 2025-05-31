#include "StyledOperatorDelegate.h"

#include <QTreeView>

#include "DataStructures.h"

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

    // Draw countries in bold
    if (!index.parent().isValid()) {
        QFont boldFont{option.font};
        boldFont.setBold(true);
        painter->setFont(boldFont);
    }

    const QString text{data.toString()};
    const QRect textRect{option.rect.adjusted(iconSize.width() + 10, 0, 0, 0)};
    painter->drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, text);
}

void QStyledOperatorDelegate::paintDecorationRole(QPainter *painter, const QStyleOptionViewItem &option,
                                                  const QModelIndex &index) {
    const QVariant iconVar{index.data(Qt::DecorationRole)}; // QString – icon path
    assert(iconVar.canConvert<QString>());

    const QString iconPath{qvariant_cast<QString>(iconVar)};

    // Find pixmap in cache
    QPixmap iconPixmap;
    if (!QPixmapCache::find(iconPath, &iconPixmap)) {
        if (!QFile::exists(iconPath)) return;

        const QIcon icon{iconPath};
        iconPixmap = icon.pixmap(iconSize);
        QPixmapCache::insert(iconPath, iconPixmap);
    }

    const QRect iconRect{option.rect.topLeft(), iconSize};
    painter->drawPixmap(iconRect, iconPixmap);
}
