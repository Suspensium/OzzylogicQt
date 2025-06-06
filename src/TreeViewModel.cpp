#include "TreeViewModel.h"

#include "DataStructures.h"

TreeViewModel::TreeViewModel(IDatabaseManager *database, QObject *parent)
    : QAbstractItemModel{parent} {
    m_dbManager = database;
    if (m_dbManager->isConnected()) {
        setupModelData();
    }
}

void TreeViewModel::sort(int column, Qt::SortOrder order) {
    emit layoutAboutToBeChanged();

    QVector<TreeViewItem *> &children{m_rootItem->children()};

    std::ranges::sort(children, [order](const TreeViewItem *a, const TreeViewItem *b) {
        const QString sa{a->data().toString()};
        const QString sb{b->data().toString()};

        return order == Qt::AscendingOrder ? sa < sb : sa > sb;
    });

    emit layoutChanged();
}

QModelIndex TreeViewModel::index(int row, int column, const QModelIndex &parent) const {
    if (!hasIndex(row, column, parent)) return {};

    const TreeViewItem *parentItem{
        parent.isValid() ? static_cast<TreeViewItem *>(parent.internalPointer()) : m_rootItem.get()
    };
    const TreeViewItem *childItem{parentItem->child(row)};
    return childItem ? createIndex(row, column, childItem) : QModelIndex();
}

QModelIndex TreeViewModel::parent(const QModelIndex &index) const {
    if (!index.isValid()) return {};

    const TreeViewItem *childItem{static_cast<TreeViewItem *>(index.internalPointer())};
    const TreeViewItem *parentItem{childItem->parentItem()};

    if (parentItem == m_rootItem.get()) return {};

    return createIndex(parentItem->row(), 0, parentItem);
}

int TreeViewModel::rowCount(const QModelIndex &parent) const {
    const TreeViewItem *parentItem{
        parent.isValid() ? static_cast<TreeViewItem *>(parent.internalPointer()) : m_rootItem.get()
    };
    return parentItem->childCount();
}

QVariant TreeViewModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) return {};

    const TreeViewItem *item{static_cast<TreeViewItem *>(index.internalPointer())};
    const QVariant &itemData{item->data()};

    if (itemData.canConvert<CountryInfo>()) {
        const CountryInfo country{itemData.value<CountryInfo>()};

        if (role == Qt::DisplayRole) {
            return QVariant::fromValue(country);
        }
        if (role == Qt::DecorationRole) {
            return QVariant::fromValue(country.iconPath);
        }
    } else if (itemData.canConvert<OperatorInfo>()) {
        const OperatorInfo op{itemData.value<OperatorInfo>()};

        if (role == Qt::DisplayRole) {
            return QVariant::fromValue(op);
        }
        if (role == Qt::DecorationRole) {
            return QVariant::fromValue(op.iconPath);
        }
    }
    return {};
}

void TreeViewModel::setupModelData() {
    QList<CountryInfo> countries{m_dbManager->getCountryOperatorData()};

    for (const CountryInfo &country: countries) {
        TreeViewItem *countryItem{new TreeViewItem(QVariant::fromValue(country), m_rootItem.get())};
        m_rootItem->appendChild(countryItem);

        for (const OperatorInfo &op: country.operators) {
            TreeViewItem *operatorItem{new TreeViewItem(QVariant::fromValue(op), countryItem)};
            countryItem->appendChild(operatorItem);
        }
    }
}
