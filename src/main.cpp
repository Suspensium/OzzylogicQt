#include <QApplication>
#include <QTreeView>

#include "SQLiteManager.h"
#include "TreeViewModel.h"

int main(int argc, char *argv[]) {
    QApplication app{argc, argv};

    qRegisterMetaType<OperatorInfo>("OperatorInfo");
    QMetaType::registerConverter<OperatorInfo, QString>(&OperatorInfo::toString);
    qRegisterMetaType<CountryInfo>("CountryInfo");
    QMetaType::registerConverter<CountryInfo, QString>(&CountryInfo::toString);

    TreeViewModel model{new SQLiteManager, "data/system.db"};
    QTreeView view;
    view.setHeaderHidden(true);
    view.setModel(&model);
    view.setWindowTitle("Mobile Operators");
    view.show();

    return app.exec();
}
