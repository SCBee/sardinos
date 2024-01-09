/**
\file
Copyright (c) 2020 Lockheed Martin Corp. All rights reserved.
*/

#include <StandardPathsExampleContent.h>

#include <LmCdl/I_VcsiStandardPaths.h>
#include <QDir>
#include <QTimer>

namespace {
QString stripTimestamp(const QString& line)
{
    static QString exampleTimestamp("[21:24:15.891]");
    return line.right(line.length() - exampleTimestamp.length());
}

WarningInfo parseWarnings(QTextStream& stream)
{
    WarningInfo info;
    QString line = stream.readLine();
    while (stream.readLineInto(&line)) {
        ++info.lines;
        if (line.contains(QString("[WARNING ]"))) {
            info.warnings.insert(stripTimestamp(line));
            ++info.count;
        }
    }

    return info;
}
}

StandardPathsExampleContent::StandardPathsExampleContent(LmCdl::I_VcsiStandardPaths& standardPaths)
    : standardPaths_(standardPaths)

    , timer_()
    , logFile_()
    , logStream_(&logFile_)
{
    connect(&timer_, &QTimer::timeout, this, &StandardPathsExampleContent::parseLogFile);

    timer_.setInterval(5000);
    timer_.start();

    QDir logDirectory(standardPaths_.locateDirectory(LmCdl::I_VcsiStandardPaths::Location::Logs));
    QString filenameWithPath = logDirectory.absoluteFilePath("vcsi.log");
    logFile_.setFileName(filenameWithPath);
    logFile_.open(QIODevice::ReadOnly | QIODevice::Text | QIODevice::Unbuffered);
}

StandardPathsExampleContent::~StandardPathsExampleContent() { }

QStringList StandardPathsExampleContent::warnings() { return warnings_.warnings.values(); }

DiffInfo StandardPathsExampleContent::diff(const QString& otherLog)
{
    QFile otherLogFile(otherLog);
    otherLogFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream otherStream(&otherLogFile);
    WarningInfo otherWarnings = parseWarnings(otherStream);

    DiffInfo diff;
    diff.diff = warnings_.warnings - otherWarnings.warnings;
    diff.currentWarningCount = warnings_.count;
    diff.otherWarningCount = otherWarnings.count;
    return diff;
}

void StandardPathsExampleContent::parseLogFile()
{
    if (!logFile_.isOpen()) {
        return;
    }

    WarningInfo update = parseWarnings(logStream_);
    warnings_.unite(update);
    emit warningUpdated(warnings_, update);
}
