/**
\file
Copyright (c) 2020 Lockheed Martin Corp. All rights reserved.
*/

#pragma once

#include <ExampleTrackVisualization.h>
#include <QList>
#include <QObject>

class ExampleTrackDomain : public QObject {
    Q_OBJECT
public:
    ExampleTrackDomain();
    ~ExampleTrackDomain();

    QList<ExampleTrackVisualization*> tracks() const;

private:
    Q_DISABLE_COPY(ExampleTrackDomain)

    void setupTrackForHawaii(ExampleTrackVisualization& track, int id);

    QScopedPointer<ExampleTrackVisualization> track1_;
    QScopedPointer<ExampleTrackVisualization> track2_;
    QScopedPointer<ExampleTrackVisualization> track3_;

    QList<ExampleTrackVisualization*> allTracks_;
};
