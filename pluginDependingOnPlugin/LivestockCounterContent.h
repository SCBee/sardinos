/**
\file
Copyright (c) 2020 Lockheed Martin Corp. All rights reserved.
*/

#pragma once

#include <QObject>
#include <QScopedPointer>

namespace GdWild {
class I_AnimalTrackerApi;
}

class LivestockCounterContent : public QObject {
    Q_OBJECT

public:
    explicit LivestockCounterContent(GdWild::I_AnimalTrackerApi& animalTrackerApi);
    virtual ~LivestockCounterContent();

private slots:
    void logCurrentLivestockCount();

private:
    void connectToApiSignals();

private:
    GdWild::I_AnimalTrackerApi& animalTrackerApi_;
};
