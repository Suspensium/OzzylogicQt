#pragma once

struct OperatorInfo {
    OperatorInfo() = default;

    OperatorInfo(const QString &mcc_, const QString &mnc_, const QString &name_)
        : mcc{mcc_}, mnc{mnc_}, name{name_} {
        iconPath = QString("icons/operators/%1_%2.png").arg(mcc, mnc);
    }

    QString toString() const {
        return QString("%1 (%2-%3)").arg(name, mcc, mnc);
    }

    QString mcc{};
    QString mnc{};
    QString name{};
    QString iconPath{};
};

struct CountryInfo {
    CountryInfo() = default;

    CountryInfo(const QString &mcc_, const QString &code_, const QString &name_,
                int mncLength_, QList<OperatorInfo> operators_)
        : mcc{mcc_}, code{code_}, name{name_}, mncLength{mncLength_}, operators{std::move(operators_)} {
        iconPath = QString("icons/countries/%1.png").arg(code);
    }

    QString toString() const {
        return QString("%1 (%2)").arg(name, code);
    }

    QString mcc{};
    QString code{};
    QString name{};
    QString iconPath{};
    int mncLength{};
    QList<OperatorInfo> operators{};
};

Q_DECLARE_METATYPE(OperatorInfo)

Q_DECLARE_METATYPE(CountryInfo)

class QString;
class QSqlQuery;

class IDatabaseManager {
public:
    virtual ~IDatabaseManager() = default;

    virtual bool connect(const QString &connectionString) = 0;

    virtual void disconnect() = 0;

    virtual bool isConnected() const = 0;

    virtual QSqlQuery executeQuery(const QString &query) = 0;

    virtual QList<CountryInfo> getCountryOperatorData() = 0;
};
