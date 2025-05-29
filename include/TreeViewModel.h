#pragma once

#include <QAbstractItemModel>
#include <QSqlDatabase>

#include "TreeViewItem.h"
#include "DatabaseManager.h"

class TreeViewModel : public QAbstractItemModel {
    Q_OBJECT

public:
    TreeViewModel(IDatabaseManager *database, QObject *parent = nullptr);

    virtual void sort(int column, Qt::SortOrder order = Qt::AscendingOrder) override;

    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;

    virtual QModelIndex parent(const QModelIndex &index) const override;

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override { return 1; }

    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void setupModelData();

private:
    std::unique_ptr<TreeViewItem> m_rootItem{std::make_unique<TreeViewItem>("Root")};
    IDatabaseManager *m_dbManager;
};
