#pragma once

class QString;
class QSqlQuery;

class IDatabaseManager {
public:
    virtual ~IDatabaseManager() = default;

    virtual bool connect(const QString &connectionString) = 0;

    virtual void disconnect() = 0;

    virtual bool isConnected() const = 0;

    virtual QSqlQuery executeQuery(const QString &query) = 0;

    virtual QList<struct CountryInfo> getCountryOperatorData() = 0;
};
