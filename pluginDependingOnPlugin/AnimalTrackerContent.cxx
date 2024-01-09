/**
\file
Copyright (c) 2020 Lockheed Martin Corp. All rights reserved.
*/

#include <AnimalTrackerContent.h>

#include <LmCdl/I_PointOfInterestApi.h>
#include <LmCdl/VcsiIdentifiedPointOfInterest.h>
#include <LmCdl/VcsiPointOfInterestProperties.h>

AnimalTrackerContent::AnimalTrackerContent(LmCdl::I_PointOfInterestApi& pointOfInterestApi)
    : timer_()
    , pointOfInterestApi_(pointOfInterestApi)
{
    timer_.setSingleShot(false);
    timer_.start(60000);

    QObject::connect(&timer_, &QTimer::timeout, this, &AnimalTrackerContent::refreshAnimalList);
}

AnimalTrackerContent::~AnimalTrackerContent() { }

void AnimalTrackerContent::refreshAnimalList()
{
    const QString animalPrefix = "animal:";

    QList<LmCdl::VcsiIdentifiedPointOfInterest> pointsOfInterest = pointOfInterestApi_.pointsOfInterest();

    animals_.clear();
    qDebug("Refreshing animal list...\n");
    for (int i = 0; i < pointsOfInterest.size(); i++) {
        LmCdl::VcsiPointOfInterestProperties properties = pointsOfInterest.at(i).pointOfInterest();
        if (properties.label().contains(animalPrefix, Qt::CaseInsensitive)) {
            QString animalName = properties.label().mid(animalPrefix.size());
            if (!animalName.isEmpty()) {
                qDebug("Added animal to list: %s\n", qPrintable(animalName));
                animals_.append(animalName);
            }
        }
    }
    emit animalListChanged();
}

QStringList AnimalTrackerContent::animalList() const { return animals_; }
