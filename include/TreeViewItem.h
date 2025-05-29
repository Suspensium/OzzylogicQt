#pragma once

#include <QVariant>

class TreeViewItem {
public:
    explicit TreeViewItem(const QVariant &data, TreeViewItem *parentItem = nullptr)
        : m_itemData{data}, m_parentItem{parentItem} {
    }

    ~TreeViewItem() {
        qDeleteAll(m_childItems);
    }

    void appendChild(TreeViewItem *child) {
        m_childItems.append(child);
    }

    const QVector<TreeViewItem *> &children() const {
        return m_childItems;
    }

    QVector<TreeViewItem *> &children() {
        return m_childItems;
    }

    TreeViewItem *child(int row) const {
        return m_childItems.value(row);
    }

    int childCount() const {
        return m_childItems.count();
    }

    const QVariant &data() const {
        return m_itemData;
    }

    int row() const {
        return m_parentItem->m_childItems.indexOf(this);
    }

    TreeViewItem *parentItem() const {
        return m_parentItem;
    }

private:
    QVector<TreeViewItem *> m_childItems;
    QVariant m_itemData;
    TreeViewItem *m_parentItem;
};
