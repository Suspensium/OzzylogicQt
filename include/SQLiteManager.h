#pragma once

#include <QSqlDatabase>

#include "DatabaseManager.h"

class SQLiteManager final : public IDatabaseManager {
public:
    explicit SQLiteManager(const QString &connectionString);

    ~SQLiteManager() override {
        disconnect();
    }

    bool connect(const QString &connectionString) override;

    void disconnect() override { if (isConnected()) m_db.close(); }

    bool isConnected() const override { return m_db.isOpen(); }

    QSqlQuery executeQuery(const QString &query) override;

    QList<struct CountryInfo> getCountryOperatorData() override;

private:
    QSqlDatabase m_db;
};
