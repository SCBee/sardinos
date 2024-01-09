/**
\file
Copyright (c) 2020 Lockheed Martin Corp. All rights reserved.
*/

#pragma once

#include <LmCdl/I_KeepOutRestrictionZoneCollection.h>
#include <LmCdl/KeepOutRestrictionZoneId.h>
#include <LmCdl/VectorDataLayerId.h>
#include <QFileDialog>
#include <QList>
#include <QScopedPointer>
class QString;
namespace LmCdl {
class I_AggregateVectorDataLoader;
class I_KeepOutRestrictionZoneCollection;
class I_VectorDataLayerRegistry;
}

class RestrictionZoneApiExampleContent : public QObject {
    Q_OBJECT

public:
    explicit RestrictionZoneApiExampleContent(
        LmCdl::I_KeepOutRestrictionZoneCollection& keepOutRestrictionZoneCollection,
        LmCdl::I_VectorDataLayerRegistry& layerRegistry, LmCdl::I_AggregateVectorDataLoader& dataLoader);
    virtual ~RestrictionZoneApiExampleContent();

    void loadVectorDataAndCreateRestrictionZones(const QString& filePath);
    void retrieveAndPrintRestrictionZonesCreatedByPlugin();
    void removeRestrictionZonesCreatedByPlugin();

signals:
    void statusUpdate(const QString& status);

private slots:
    void createKeepOutZonePerPolygon(const QList<LmCdl::VectorDataLayerId>& layers);
    void restrictedZoneIsAdded(LmCdl::KeepOutRestrictionZoneId id);
    void restrictedZoneAboutToBeDeleted(LmCdl::KeepOutRestrictionZoneId id);

    void createRestrictionZone(const QString& name);

private:
    LmCdl::I_KeepOutRestrictionZoneCollection& keepOutRestrictionZoneCollection_;
    LmCdl::I_VectorDataLayerRegistry& layerRegistry_;
    LmCdl::I_AggregateVectorDataLoader& dataLoader_;
    QList<LmCdl::KeepOutRestrictionZoneId> pluginZones_;
};
