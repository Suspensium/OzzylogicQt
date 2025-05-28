#include "StyledOperatorDelegate.h"

#include "DataStructures.h"

void QStyledOperatorDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                                    const QModelIndex &index) const {
    constexpr QSize iconSize{16, 16};

    painter->save();

    initStyleOption(const_cast<QStyleOptionViewItem *>(&option), index);

    option.widget->style()->drawPrimitive(QStyle::PE_PanelItemViewItem, &option, painter, option.widget);

    const QVariant data{index.data(Qt::DisplayRole)}; // CountryInfo or OperatorInfo
    const QVariant iconVar{index.data(Qt::DecorationRole)}; // QIcon

    // Draw icon
    [[likely]] if (iconVar.canConvert<QIcon>()) {
        const QIcon icon{qvariant_cast<QIcon>(iconVar)};
        const QRect iconRect{QRect(option.rect.topLeft(), iconSize)};
        icon.paint(painter, iconRect, Qt::AlignVCenter | Qt::AlignLeft);
    }

    // Draw text
    QString text;
    if (data.canConvert<CountryInfo>()) {
        text = data.value<CountryInfo>().toString();
    } else if (data.canConvert<OperatorInfo>()) {
        text = data.value<OperatorInfo>().toString();
    }

    QFont font{option.font};
    font.setPointSize(14);
    painter->setFont(font);
    const QRect textRect{option.rect.adjusted(iconSize.width() + 10, 0, 0, 0)};
    painter->drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, text);

    painter->restore();
}
