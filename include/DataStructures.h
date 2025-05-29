#pragma once

#include <QFile>
#include <QIcon>
#include <QString>
#include <QList>
#include <QMetaType>
#include <QPixmapCache>

constexpr QSize iconSize{16, 16};

struct OperatorInfo {
    OperatorInfo() = default;

    OperatorInfo(const QString &mcc_, const QString &mnc_, const QString &name_)
        : mcc{mcc_}, mnc{mnc_}, name{name_} {
        iconPath = QString("icons/operators/%1_%2.png").arg(mcc, mnc);
    }

    QString toString() const {
        return QString("%1 (%2-%3)").arg(name, mcc, mnc);
    }

    bool addIconToCache() const {
        if (QFile::exists(iconPath)) {
            QPixmapCache::insert(iconPath, QIcon{iconPath}.pixmap(iconSize));
            return true;
        }
        return false;
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
        : code{code_}, name{name_}, mncLength{mncLength_}, operators{std::move(operators_)} {
        mcc.append(mcc_);
        iconPath = QString("icons/countries/%1.png").arg(code);
    }

    QString toString() const {
        return QString("%1 (%2)").arg(name, code);
    }

    bool addIconToCache() const {
        if (QFile::exists(iconPath)) {
            QPixmapCache::insert(iconPath, QIcon{iconPath}.pixmap(iconSize));
            return true;
        }
        return false;
    }

    QList<QString> mcc{};
    QString code{};
    QString name{};
    QString iconPath{};
    int mncLength{};
    QList<OperatorInfo> operators{};
};

Q_DECLARE_METATYPE(OperatorInfo)

Q_DECLARE_METATYPE(CountryInfo)
