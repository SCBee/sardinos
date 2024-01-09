/**
\file
Copyright (c) 2020 Lockheed Martin Corp. All rights reserved.
*/

#pragma once

#include <I_AnimalTrackerApi.h>
#include <LmCdl/VcsiPointOfInterestId.h>
#include <QScopedPointer>
#include <QStringList>
#include <QTimer>

namespace LmCdl {
class I_PointOfInterestApi;
class VcsiPointOfInterestProperties;
}

class AnimalTrackerContent : public GdWild::I_AnimalTrackerApi {
    Q_OBJECT
public:
    explicit AnimalTrackerContent(LmCdl::I_PointOfInterestApi& pointOfInterestApi);
    virtual ~AnimalTrackerContent();

    QStringList animalList() const override;

private slots:
    void refreshAnimalList();

private:
    QTimer timer_;
    LmCdl::I_PointOfInterestApi& pointOfInterestApi_;
    QStringList animals_;
};
