#include "SQLiteManager.h"

#include <QSqlError>
#include <QSqlQuery>

#include "DataStructures.h"

SQLiteManager::SQLiteManager(const QString &connectionString) {
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    connect(connectionString);
}

bool SQLiteManager::connect(const QString &connectionString) {
    if (isConnected()) return true;

    m_db.setDatabaseName(connectionString);
    if (!m_db.open()) {
        qWarning() << "Failed to open database:" << m_db.lastError().text();
        return false;
    }
    return true;
}

QSqlQuery SQLiteManager::executeQuery(const QString &query) {
    QSqlQuery sqlQuery{m_db};
    if (!sqlQuery.exec(query)) {
        qWarning() << "Query execution failed:" << sqlQuery.lastError().text();
    }
    return sqlQuery;
}

QList<CountryInfo> SQLiteManager::getCountryOperatorData() {
    QHash<QString, CountryInfo> countries;

    QSqlQuery countryQuery{executeQuery("SELECT mcc, code, name, mnc_length FROM countries")};
    while (countryQuery.next()) {
        QString mcc{countryQuery.value(0).toString()};
        QString code{countryQuery.value(1).toString()};
        QString name{countryQuery.value(2).toString()};
        int mncLength{countryQuery.value(3).toInt()};
        QList<OperatorInfo> operatorsList;

        QSqlQuery operatorQuery{m_db};
        operatorQuery.prepare("SELECT mnc, name FROM operators WHERE mcc = ?");
        operatorQuery.addBindValue(mcc);
        operatorQuery.exec();
        while (operatorQuery.next()) {
            QString opMnc{operatorQuery.value(0).toString()};
            QString opName{operatorQuery.value(1).toString()};
            operatorsList.emplaceBack(mcc, opMnc, opName);
        }

        // If country already exists, we should append new MCC and corresponding operators to it
        if (countries.contains(code)) {
            CountryInfo &ci = countries[code];
            ci.mcc.append(mcc);
            ci.operators.append(operatorsList);
        } else {
            const auto it{
                countries.emplace(code, mcc, code, name, mncLength, operatorsList)
            };
            it->addIconToCache(); // Cache country icons by default
        }
    }

    return countries.values();
}
