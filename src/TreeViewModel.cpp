#include "TreeViewModel.h"

#include <QSqlQuery>
#include <QFile>
#include <QIcon>

#include "DataStructures.h"

TreeViewModel::TreeViewModel(IDatabaseManager *database, const QString &connectionString, QObject *parent)
    : QAbstractItemModel{parent} {
    m_dbManager = database;
    if (m_dbManager->connect(connectionString)) {
        setupModelData();
    }
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
            if (QFile::exists(country.iconPath)) {
                return QIcon{country.iconPath};
            }
        }
    } else if (itemData.canConvert<OperatorInfo>()) {
        const OperatorInfo op{itemData.value<OperatorInfo>()};

        if (role == Qt::DisplayRole) {
            return QVariant::fromValue(op);
        }
        if (role == Qt::DecorationRole) {
            if (QFile::exists(op.iconPath)) {
                return QIcon{op.iconPath};
            }
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
