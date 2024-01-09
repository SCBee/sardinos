/**
\file
Copyright (c) 2020 Lockheed Martin Corp. All rights reserved.
*/

#include <LivestockCounterContent.h>

#include <I_AnimalTrackerApi.h>
#include <QStringList>

LivestockCounterContent::LivestockCounterContent(GdWild::I_AnimalTrackerApi& animalTrackerApi)
    : animalTrackerApi_(animalTrackerApi)
{
    connectToApiSignals();
}

LivestockCounterContent::~LivestockCounterContent() { }

void LivestockCounterContent::connectToApiSignals()
{
    connect(&animalTrackerApi_, SIGNAL(animalListChanged()), this, SLOT(logCurrentLivestockCount()));
}

void LivestockCounterContent::logCurrentLivestockCount()
{
    QStringList animalList = animalTrackerApi_.animalList();
    int livestockCount = 0;
    for (int i = 0; i < animalList.size(); i++) {
        const QString animal = animalList.at(i);
        if (animal.contains("cow", Qt::CaseInsensitive) || animal.contains("sheep", Qt::CaseInsensitive)) {
            livestockCount++;
        }
    }
    qDebug("Number of livestock animals tracked: %d", livestockCount);
}
