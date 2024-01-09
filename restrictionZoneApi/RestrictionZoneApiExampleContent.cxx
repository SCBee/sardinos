/**
\file
Copyright (c) 2020 Lockheed Martin Corp. All rights reserved.
*/

#include <RestrictionZoneApiExampleContent.h>

#include <LmCdl/I_AggregateVectorDataLoader.h>
#include <LmCdl/I_KeepOutRestrictionZone.h>
#include <LmCdl/I_KeepOutRestrictionZoneCollection.h>
#include <LmCdl/I_VectorDataLayer.h>
#include <LmCdl/I_VectorDataLayerRegistry.h>
#include <LmCdl/I_VectorDataLoaderRegistry.h>
#include <LmCdl/VectorDataFeature.h>
#include <QFileInfo>
#include <QString>
#include <QUrl>

RestrictionZoneApiExampleContent::RestrictionZoneApiExampleContent(
    LmCdl::I_KeepOutRestrictionZoneCollection& keepOutRestrictionZoneCollection,
    LmCdl::I_VectorDataLayerRegistry& layerRegistry, LmCdl::I_AggregateVectorDataLoader& dataLoader)
    : keepOutRestrictionZoneCollection_(keepOutRestrictionZoneCollection)
    , layerRegistry_(layerRegistry)
    , dataLoader_(dataLoader)
    , pluginZones_()
{
    connect(&dataLoader_, &LmCdl::I_AggregateVectorDataLoader::vectorDataLoaderRegistered, this, [this]() {
        emit statusUpdate("New vector data loader available. Try reloading any files that failed previously.");
    });

    connect(&keepOutRestrictionZoneCollection_, &LmCdl::I_KeepOutRestrictionZoneCollection::restrictionZoneAdded, this,
        &RestrictionZoneApiExampleContent::restrictedZoneIsAdded);

    connect(&keepOutRestrictionZoneCollection_,
        &LmCdl::I_KeepOutRestrictionZoneCollection::restrictionZoneAboutToBeRemoved, this,
        &RestrictionZoneApiExampleContent::restrictedZoneAboutToBeDeleted);
}

RestrictionZoneApiExampleContent::~RestrictionZoneApiExampleContent() { removeRestrictionZonesCreatedByPlugin(); }

void RestrictionZoneApiExampleContent::loadVectorDataAndCreateRestrictionZones(const QString& filePath)
{
    QList<LmCdl::VectorDataLayerId> layers = dataLoader_.loadVectorData(filePath);
    if (layers.empty()) {
        emit statusUpdate(QString("Failed to load vector data from file %1").arg(filePath));
    } else {
        createKeepOutZonePerPolygon(layers);
    }
}

void RestrictionZoneApiExampleContent::retrieveAndPrintRestrictionZonesCreatedByPlugin()
{
    foreach (LmCdl::KeepOutRestrictionZoneId id, pluginZones_) {
        LmCdl::I_KeepOutRestrictionZone* zone = keepOutRestrictionZoneCollection_.retrieveKeepOutZone(id);
        emit statusUpdate(QString("Zone with id %1 (0x%2) was loaded by this plugin")
                              .arg(id.value())
                              .arg(reinterpret_cast<quintptr>(zone), QT_POINTER_SIZE * 2, 16, QChar('0')));
    }
}

void RestrictionZoneApiExampleContent::removeRestrictionZonesCreatedByPlugin()
{
    foreach (LmCdl::KeepOutRestrictionZoneId id, pluginZones_) {
        keepOutRestrictionZoneCollection_.removeKeepOutZone(id);
    }
    pluginZones_.clear();
}

void RestrictionZoneApiExampleContent::createKeepOutZonePerPolygon(const QList<LmCdl::VectorDataLayerId>& layers)
{
    foreach (LmCdl::VectorDataLayerId id, layers) {
        LmCdl::I_VectorDataLayer& layer = layerRegistry_.layerById(id);
        foreach (LmCdl::VectorDataFeatureId featureId, layer.allPolygons()) {
            LmCdl::VectorDataFeature* feature = layer.featureById(featureId);
            emit statusUpdate(
                QString("Creating RZ for feature with attributes: %1").arg(feature->attributeList().join(", ")));

            QString featureLabel = feature ? feature->featureLabel() : QString("NULL feature");
            featureLabel = featureLabel.isEmpty() ? feature->getAttribute("Name") : featureLabel;
            featureLabel = featureLabel.isEmpty() ? QString("Unknown feature") : featureLabel;
            createRestrictionZone(QString("RestrictionZoneApiExampleContent - %1").arg(featureLabel));
        }
    }
}

void RestrictionZoneApiExampleContent::restrictedZoneIsAdded(LmCdl::KeepOutRestrictionZoneId id)
{
    emit statusUpdate(QString("New Restricted Zone Added id#: %1").arg(id.value()));
}

void RestrictionZoneApiExampleContent::restrictedZoneAboutToBeDeleted(LmCdl::KeepOutRestrictionZoneId id)
{
    emit statusUpdate(QString("Restricted Zone to be Deleted id#: %1").arg(id.value()));
}

void RestrictionZoneApiExampleContent::createRestrictionZone(const QString& name)
{
    LmCdl::KeepOutRestrictionZoneId zoneId = keepOutRestrictionZoneCollection_.createKeepOutZone(name);
    pluginZones_.append(zoneId);
}
