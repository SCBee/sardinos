/**
\file
Copyright (c) 2020 Lockheed Martin Corp. All rights reserved.
*/

#pragma once

#include <LmCdl/I_VectorDataLoader.h>
#include <LmCdl/VectorDataFeatureId.h>
#include <QObject>
#include <QScopedPointer>
#include <VectorLayerIdFeatureIdPair.h>
#include <ui_vectorform.h>

namespace LmCdl {
class I_AggregateVectorDataLoader;
class I_CenterMapOnLocationApi;
class I_VectorDataLayerRegistry;
class I_VectorDataWriterRegistry;
class I_VcsiApplicationApi;
class I_VcsiStandardPaths;
}

class VectorDataDockWidget;
class VectorDataFeatureTree;

class VectorDataExampleContent : public QObject, public LmCdl::I_VectorDataLoader {
    Q_OBJECT
public:
    explicit VectorDataExampleContent(LmCdl::I_VectorDataLayerRegistry& layerRegistry,
        LmCdl::I_VectorDataWriterRegistry& vectorDataWriters, LmCdl::I_AggregateVectorDataLoader& dataLoader,
        LmCdl::I_CenterMapOnLocationApi& centerMapApi, LmCdl::I_VcsiApplicationApi& applicationApi,
        LmCdl::I_VcsiStandardPaths& standardPathsApi);
    virtual ~VectorDataExampleContent();

    QList<LmCdl::VectorDataLayerId> loadVectorData(const QString& uri) override;

private slots:
    void processNewVectorData(LmCdl::VectorDataLayerId id);
    void loadFile();
    void centerMapOnFeature(VectorLayerIdFeatureIdPair uniqueFeatureId);
    void promptUserToChooseFile();
    void exportAllVectorDataToGpx();

private:
    void connectToApiSignals();
    int extractData(const QString& uri);
    QList<LmCdl::VectorDataLayerId> injectDataIntoSystem(const QString& uri, int data);

private:
    LmCdl::I_VectorDataLayerRegistry& layerRegistry_;
    LmCdl::I_VectorDataWriterRegistry& vectorDataWriters_;
    LmCdl::I_AggregateVectorDataLoader& dataLoader_;
    LmCdl::I_CenterMapOnLocationApi& centerMapApi_;
    LmCdl::I_VcsiApplicationApi& applicationApi_;
    LmCdl::I_VcsiStandardPaths& standardPathsApi_;
    Ui::VectorDataDockForm ui_;
    VectorDataDockWidget* dockWidget_;
    VectorDataFeatureTree* featureTreeWidget_;
};
