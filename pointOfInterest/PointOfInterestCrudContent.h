/**
 \file
 Copyright (c) 2019 Lockheed Martin Corp. All rights reserved.
 */

#pragma once

#include <LmCdl/VcsiPointOfInterestId.h>
#include <QHash>
#include <QObject>
class QGeoCoordinate;

namespace LmCdl {
class I_PointOfInterestApi;
class VcsiMilStdCode;
class VcsiPointOfInterestProperties;
}

class PointOfInterestCrudContent : public QObject {
    Q_OBJECT

public:
    explicit PointOfInterestCrudContent(LmCdl::I_PointOfInterestApi& pointOfInterestApi);
    virtual ~PointOfInterestCrudContent();

private slots:
    void makeAndPublishImperfectClone(
        LmCdl::VcsiPointOfInterestId sourceId, const LmCdl::VcsiPointOfInterestProperties& pointOfInterest);
    void updateCloneIfNecessary(
        LmCdl::VcsiPointOfInterestId id, const LmCdl::VcsiPointOfInterestProperties& pointOfInterest);
    void removeAndUnpublishClone(LmCdl::VcsiPointOfInterestId id);

private:
    void connectToApiSignals();

    bool isPointOfInterestASourceOfAClone(LmCdl::VcsiPointOfInterestId id) const;
    bool hasPointOfInterestAlreadyBeenCloned(const LmCdl::VcsiPointOfInterestProperties& pointOfInterest) const;

    LmCdl::VcsiPointOfInterestProperties makeImperfectClone(
        const LmCdl::VcsiPointOfInterestProperties& pointOfInterest) const;
    QGeoCoordinate nudgeLocation(const QGeoCoordinate& originalLocation) const;
    LmCdl::VcsiMilStdCode nudgeMilStdCode(const LmCdl::VcsiMilStdCode& milStdCode) const;

    void publishAndMapPointOfInterest(
        LmCdl::VcsiPointOfInterestId sourceId, const LmCdl::VcsiPointOfInterestProperties& pointOfInterest);
    void unpublishAndUnmapClone(LmCdl::VcsiPointOfInterestId sourceId);
    void unmapClone(LmCdl::VcsiPointOfInterestId cloneId);

    QString generateCloneLabel(const QString& baseName) const;

private:
    LmCdl::I_PointOfInterestApi& pointOfInterestApi_;

    QHash<LmCdl::VcsiPointOfInterestId, LmCdl::VcsiPointOfInterestId> sourceIdsToClonedIds_;
};
