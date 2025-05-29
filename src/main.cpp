#include <QTreeView>
#include <QProxyStyle>

#include "DataStructures.h"
#include "SQLiteManager.h"
#include "StyledOperatorDelegate.h"
#include "TreeViewModel.h"

int main(int argc, char *argv[]) {
    QApplication app{argc, argv};

    QPixmapCache::setCacheLimit(1024); // 1 Mb for icons cache

    qRegisterMetaType<OperatorInfo>("OperatorInfo");
    QMetaType::registerConverter<OperatorInfo, QString>(&OperatorInfo::toString);
    qRegisterMetaType<CountryInfo>("CountryInfo");
    QMetaType::registerConverter<CountryInfo, QString>(&CountryInfo::toString);

    QTreeView treeView;
    treeView.setRootIsDecorated(true);
    treeView.setItemsExpandable(true);
    treeView.setAllColumnsShowFocus(true);
    treeView.setUniformRowHeights(true);
    treeView.setAlternatingRowColors(false);
    treeView.setStyle(new QProxyStyle);
    treeView.resize(QSize{400, 600});
    treeView.setHeaderHidden(true);
    treeView.setModel(new TreeViewModel(new SQLiteManager, "data/system.db"));
    treeView.setItemDelegate(new QStyledOperatorDelegate);
    treeView.setWindowTitle("Mobile Operators");
    treeView.setSortingEnabled(true);
    treeView.sortByColumn(0, Qt::AscendingOrder);
    treeView.show();

    return app.exec();
}
