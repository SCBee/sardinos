/**
 \file
 Copyright (c) 2019 Lockheed Martin Corp. All rights reserved.
 */

#include <PointOfInterestCrudContent.h>

#include <LmCdl/I_PointOfInterestApi.h>
#include <LmCdl/VcsiIdentifiedPointOfInterest.h>
#include <LmCdl/VcsiMilStdCode.h>
#include <LmCdl/VcsiPointOfInterestProperties.h>
#include <QDateTime>
#include <QGeoCoordinate>
#include <QList>

namespace {
const QList<LmCdl::VcsiMilStdCode> exampleMilStdCodes = { LmCdl::VcsiMilStdCode("GDOCSBN--------"),
    LmCdl::VcsiMilStdCode("GDSXPI---------"), LmCdl::VcsiMilStdCode("GFMPOFS--------"),
    LmCdl::VcsiMilStdCode("ODPAA----------"), LmCdl::VcsiMilStdCode("OFPPC----------"),
    LmCdl::VcsiMilStdCode("IGUASRE---NL---"), LmCdl::VcsiMilStdCode("IKUFSRU---NS---"),
    LmCdl::VcsiMilStdCode("SGGAEW----MW---"), LmCdl::VcsiMilStdCode("SKGFUIR---H----"),
    LmCdl::VcsiMilStdCode("SLG-UCI----E---"), LmCdl::VcsiMilStdCode("SOGXIUE--------") };
}

namespace {
const QString CLONE_LABEL_SUFFIX = "Sample Plugin Clone";
}

PointOfInterestCrudContent::PointOfInterestCrudContent(LmCdl::I_PointOfInterestApi& pointOfInterestApi)
    : pointOfInterestApi_(pointOfInterestApi)
{
    connectToApiSignals();
}

PointOfInterestCrudContent::~PointOfInterestCrudContent() { }

void PointOfInterestCrudContent::connectToApiSignals()
{
    connect(&pointOfInterestApi_,
        SIGNAL(pointOfInterestAdded(LmCdl::VcsiPointOfInterestId, const LmCdl::VcsiPointOfInterestProperties&)), this,
        SLOT(makeAndPublishImperfectClone(LmCdl::VcsiPointOfInterestId, const LmCdl::VcsiPointOfInterestProperties&)));

    connect(&pointOfInterestApi_,
        SIGNAL(pointOfInterestUpdated(LmCdl::VcsiPointOfInterestId, const LmCdl::VcsiPointOfInterestProperties&)), this,
        SLOT(updateCloneIfNecessary(LmCdl::VcsiPointOfInterestId, const LmCdl::VcsiPointOfInterestProperties&)));

    connect(&pointOfInterestApi_, SIGNAL(pointOfInterestRemoved(LmCdl::VcsiPointOfInterestId)), this,
        SLOT(removeAndUnpublishClone(LmCdl::VcsiPointOfInterestId)));
}

void PointOfInterestCrudContent::makeAndPublishImperfectClone(
    LmCdl::VcsiPointOfInterestId sourceId, const LmCdl::VcsiPointOfInterestProperties& pointOfInterest)
{
    if (hasPointOfInterestAlreadyBeenCloned(pointOfInterest)) {
        return;
    }

    publishAndMapPointOfInterest(sourceId, makeImperfectClone(pointOfInterest));
}

void PointOfInterestCrudContent::updateCloneIfNecessary(
    LmCdl::VcsiPointOfInterestId id, const LmCdl::VcsiPointOfInterestProperties& pointOfInterest)
{
    if (isPointOfInterestASourceOfAClone(id)) {
        LmCdl::VcsiPointOfInterestId cloneId = sourceIdsToClonedIds_.value(id);
        LmCdl::VcsiPointOfInterestProperties newClone = makeImperfectClone(pointOfInterest);
        pointOfInterestApi_.updatePointOfInterest(cloneId, newClone);
    }
}

void PointOfInterestCrudContent::removeAndUnpublishClone(LmCdl::VcsiPointOfInterestId id)
{
    if (isPointOfInterestASourceOfAClone(id)) {
        unpublishAndUnmapClone(id);
    } else {
        unmapClone(id);
    }
}

bool PointOfInterestCrudContent::isPointOfInterestASourceOfAClone(LmCdl::VcsiPointOfInterestId id) const
{
    return sourceIdsToClonedIds_.contains(id);
}

bool PointOfInterestCrudContent::hasPointOfInterestAlreadyBeenCloned(
    const LmCdl::VcsiPointOfInterestProperties& pointOfInterest) const
{
    if (pointOfInterest.label().contains(CLONE_LABEL_SUFFIX)) {
        return true;
    } else {
        const QString cloneLabel = generateCloneLabel(pointOfInterest.label());

        foreach (LmCdl::VcsiIdentifiedPointOfInterest potentialClone, pointOfInterestApi_.pointsOfInterest()) {
            if (potentialClone.pointOfInterest().label().contains(cloneLabel)) {
                return true;
            }
        }
    }
    return false;
}

LmCdl::VcsiPointOfInterestProperties PointOfInterestCrudContent::makeImperfectClone(
    const LmCdl::VcsiPointOfInterestProperties& pointOfInterest) const
{
    const QString cloneLabel = generateCloneLabel(pointOfInterest.label());
    QGeoCoordinate cloneLocation = nudgeLocation(pointOfInterest.location());
    LmCdl::VcsiMilStdCode cloneMilStdCode = nudgeMilStdCode(pointOfInterest.milStdCode());
    LmCdl::VcsiPointOfInterestProperties clone
        = LmCdl::VcsiPointOfInterestProperties(cloneLabel, cloneLocation, cloneMilStdCode);

    return clone;
}

QGeoCoordinate PointOfInterestCrudContent::nudgeLocation(const QGeoCoordinate& originalLocation) const
{
    // Nudging the latitude so that the clone is visible -- i.e. to prevent
    // the two POIs from being co-located or their labels overlapping.
    const double nudgeMagnitude = 0.005;
    double originalLatitude = originalLocation.latitude();
    double nudgeDirection = originalLatitude > 0 ? -1 : 1;
    double nudgedLatitude = originalLatitude + (nudgeMagnitude * nudgeDirection);

    QGeoCoordinate nudgedLocation = QGeoCoordinate(originalLocation);
    nudgedLocation.setLatitude(nudgedLatitude);
    return nudgedLocation;
}

LmCdl::VcsiMilStdCode PointOfInterestCrudContent::nudgeMilStdCode(const LmCdl::VcsiMilStdCode& milStdCode) const
{
    // Randomly pick an example Code from the list
    return exampleMilStdCodes.at(QDateTime::currentSecsSinceEpoch() % exampleMilStdCodes.size());
}

void PointOfInterestCrudContent::publishAndMapPointOfInterest(
    LmCdl::VcsiPointOfInterestId sourceId, const LmCdl::VcsiPointOfInterestProperties& pointOfInterest)
{
    auto mapIds = [this, sourceId](const LmCdl::VcsiPointOfInterestId& cloneId) {
        if (cloneId.isValid()) {
            sourceIdsToClonedIds_[sourceId] = cloneId;
        }
    };

    pointOfInterestApi_.addPointOfInterest(pointOfInterest, mapIds);
}

void PointOfInterestCrudContent::unpublishAndUnmapClone(LmCdl::VcsiPointOfInterestId sourceId)
{
    LmCdl::VcsiPointOfInterestId cloneId = sourceIdsToClonedIds_.value(sourceId);
    pointOfInterestApi_.removePointOfInterest(cloneId);
    unmapClone(cloneId);
}

void PointOfInterestCrudContent::unmapClone(LmCdl::VcsiPointOfInterestId cloneId)
{
    LmCdl::VcsiPointOfInterestId sourceId = sourceIdsToClonedIds_.key(cloneId);
    sourceIdsToClonedIds_.remove(sourceId);
}

QString PointOfInterestCrudContent::generateCloneLabel(const QString& baseName) const
{
    return QString("%1 - %2").arg(baseName).arg(CLONE_LABEL_SUFFIX);
}
