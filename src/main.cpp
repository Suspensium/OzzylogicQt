#include <QPixmapCache>
#include <QTreeView>
#include <QPixmapCache>

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

    TreeViewModel model{new SQLiteManager, "data/system.db"};
    QTreeView view;
    view.resize(QSize{400, 600});
    view.setHeaderHidden(true);
    view.setModel(new TreeViewModel(new SQLiteManager, "data/system.db"));
    view.setItemDelegate(new QStyledOperatorDelegate);
    view.setWindowTitle("Mobile Operators");
    view.show();

    return app.exec();
}
