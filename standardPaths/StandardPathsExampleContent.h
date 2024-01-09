/**
\file
Copyright (c) 2020 Lockheed Martin Corp. All rights reserved.
*/

#pragma once

#include <QFile>
#include <QObject>
#include <QScopedPointer>
#include <QSet>
#include <QString>
#include <QStringList>
#include <QTextStream>
#include <QTimer>

namespace LmCdl {
class I_VcsiStandardPaths;
};

struct WarningInfo {
    QSet<QString> warnings;
    int count;
    int lines;

    WarningInfo()
        : warnings()
        , count(0)
        , lines(0)
    {
    }

    WarningInfo& unite(const WarningInfo& other)
    {
        count += other.count;
        lines += other.lines;
        warnings.unite(other.warnings);
        return *this;
    }
};

struct DiffInfo {
    QSet<QString> diff;
    int currentWarningCount;
    int otherWarningCount;

    DiffInfo()
        : diff()
        , currentWarningCount(0)
        , otherWarningCount(0)
    {
    }
};

class StandardPathsExampleContent : public QObject {
    Q_OBJECT

public:
    explicit StandardPathsExampleContent(LmCdl::I_VcsiStandardPaths& standardPaths);
    virtual ~StandardPathsExampleContent();

    QStringList warnings();
    DiffInfo diff(const QString& otherLog);

signals:
    void warningUpdated(WarningInfo total, WarningInfo update);

private slots:
    void parseLogFile();

private:
    LmCdl::I_VcsiStandardPaths& standardPaths_;

    QTimer timer_;
    QFile logFile_;
    QTextStream logStream_;

    WarningInfo warnings_;
};
