/**
\file
Copyright (c) 2020 Lockheed Martin Corp. All rights reserved.
*/

#include <VectorDataExampleContent.h>

#include <LmCdl/I_AggregateVectorDataLoader.h>
#include <LmCdl/I_CenterMapOnLocationApi.h>
#include <LmCdl/I_VcsiApplicationApi.h>
#include <LmCdl/I_VcsiStandardPaths.h>
#include <LmCdl/I_VectorDataLayer.h>
#include <LmCdl/I_VectorDataLayerRegistry.h>
#include <LmCdl/I_VectorDataWriter.h>
#include <LmCdl/I_VectorDataWriterRegistry.h>
#include <LmCdl/VectorDataLineString.h>
#include <LmCdl/VectorDataPoint.h>
#include <LmCdl/VectorDataPolygon.h>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QWidget>
#include <VectorDataDockWidget.h>
#include <VectorDataFeatureTree.h>

VectorDataExampleContent::VectorDataExampleContent(LmCdl::I_VectorDataLayerRegistry& layerRegistry,
    LmCdl::I_VectorDataWriterRegistry& vectorDataWriters, LmCdl::I_AggregateVectorDataLoader& dataLoader,
    LmCdl::I_CenterMapOnLocationApi& centerMapApi, LmCdl::I_VcsiApplicationApi& applicationApi,
    LmCdl::I_VcsiStandardPaths& standardPathsApi)
    : layerRegistry_(layerRegistry)
    , vectorDataWriters_(vectorDataWriters)
    , dataLoader_(dataLoader)
    , centerMapApi_(centerMapApi)
    , applicationApi_(applicationApi)
    , standardPathsApi_(standardPathsApi)
    , dockWidget_(new VectorDataDockWidget(applicationApi))
    , featureTreeWidget_(nullptr)
{
    connectToApiSignals();

    QWidget* widget = new QWidget;
    ui_.setupUi(widget);
    connect(ui_.chooseFileButton_, SIGNAL(clicked()), this, SLOT(promptUserToChooseFile()));
    connect(ui_.loadFileButton_, SIGNAL(clicked()), this, SLOT(loadFile()));
    connect(ui_.exportButton_, SIGNAL(clicked()), this, SLOT(exportAllVectorDataToGpx()));
    if (ui_.treeWidget_) {
        featureTreeWidget_ = new VectorDataFeatureTree(layerRegistry, *ui_.treeWidget_);
        connect(featureTreeWidget_, &VectorDataFeatureTree::featureDoubleClicked, this,
            &VectorDataExampleContent::centerMapOnFeature);
    }
    dockWidget_->setWidget(widget);
}

VectorDataExampleContent::~VectorDataExampleContent() { }

void VectorDataExampleContent::connectToApiSignals()
{
    // passive receive notifications whenever a layer has been added to the system
    QObject::connect(&layerRegistry_, SIGNAL(layerAdded(LmCdl::VectorDataLayerId)), this,
        SLOT(processNewVectorData(LmCdl::VectorDataLayerId)), Qt::DirectConnection);
}

void VectorDataExampleContent::processNewVectorData(LmCdl::VectorDataLayerId id)
{
    QList<LmCdl::VectorDataLayerId> layerIds = layerRegistry_.allLayerIds();

    qDebug("New Layer detected with name %s, (NOTE that this layers content has not yet been parsed from the file), "
           "printing all known layers with content:\n\n",
        qPrintable(layerRegistry_.layerById(id).layerName()));

    for (int i = 0; i < layerIds.size(); i++) {
        LmCdl::I_VectorDataLayer& layer = layerRegistry_.layerById(layerIds.at(i));

        // examine vector data that has been inserted by a I_VectorDataLoader
        qDebug("Layer: %s contains %d points.", qPrintable(layer.layerName()), layer.allPoints().size());
        qDebug("Layer: %s contains %d lines.", qPrintable(layer.layerName()), layer.allLines().size());
        qDebug("Layer: %s contains %d polygons.", qPrintable(layer.layerName()), layer.allPolygons().size());
    }
}

void VectorDataExampleContent::loadFile()
{
    QFileInfo file(ui_.fileNameInput_->text());
    if (!file.exists()) {
        QMessageBox::warning(dockWidget_, "Load Error", QString("Cannot load: %1").arg(ui_.fileNameInput_->text()));
        return;
    }

    if (featureTreeWidget_) {
        featureTreeWidget_->loadTree(dataLoader_.loadVectorData(file.absoluteFilePath()));
    }
}

void VectorDataExampleContent::centerMapOnFeature(VectorLayerIdFeatureIdPair uniqueFeatureId)
{
    LmCdl::VectorDataPoint* point
        = layerRegistry_.layerById(uniqueFeatureId.layerId_).pointById(uniqueFeatureId.featureId_);
    if (point) {
        centerMapApi_.centerMapOnLocation(point->location());
        return;
    }

    LmCdl::VectorDataLineString* line
        = layerRegistry_.layerById(uniqueFeatureId.layerId_).lineById(uniqueFeatureId.featureId_);
    if (line) {
        if (line->locations().size() > 0) {
            centerMapApi_.centerMapOnLocation(line->locations().coordinateAt(0));
            return;
        }
    }

    LmCdl::VectorDataPolygon* polygon
        = layerRegistry_.layerById(uniqueFeatureId.layerId_).polygonById(uniqueFeatureId.featureId_);
    if (polygon) {
        if (polygon->geometry().size() > 0) {
            centerMapApi_.centerMapOnLocation(polygon->geometry().coordinateAt(0));
            return;
        }
    }
}

void VectorDataExampleContent::promptUserToChooseFile()
{
    QString fileName = QFileDialog::getOpenFileName(dockWidget_, tr("Choose Vector File"),
        QDir::homePath()); //, tr("Vector files (*.kml *.geojson *.gpx *.mvt)"));
    if (!fileName.isNull()) {
        ui_.fileNameInput_->setText(fileName);
    }
}

void VectorDataExampleContent::exportAllVectorDataToGpx()
{
    LmCdl::I_VectorDataWriter* gpxWriter = vectorDataWriters_.fileWriterForFormatDescriptionContaining("gpx");

    QList<LmCdl::I_VectorDataLayer*> layersToExport;
    QList<LmCdl::VectorDataLayerId> allLayerIds = layerRegistry_.allLayerIds();
    foreach (LmCdl::VectorDataLayerId layerId, allLayerIds) {
        layersToExport.append(&(layerRegistry_.layerById(layerId)));
    }

    if (gpxWriter) {
        QDir userDataPath
            = standardPathsApi_.locateDirectory(LmCdl::I_VcsiStandardPaths::Location::VcsiApplicationDataRoot);
        userDataPath.mkdir("exports");
        QString fileName = QFileDialog::getSaveFileName(
            dockWidget_, tr("Export Vector File (gpx)"), userDataPath.absoluteFilePath("exports"), "Gpx files (*.gpx)");
        gpxWriter->writeVectorDataToFile(layersToExport, fileName);
    }
}

QList<LmCdl::VectorDataLayerId> VectorDataExampleContent::loadVectorData(const QString& uri)
{
    return QList<LmCdl::VectorDataLayerId>();

    // only uncomment this if you DONT have another vector data loader plugin existing
    // otherwise they may conflict and only one will be invoked
    //    int data = extractData(uri);
    //    auto layerIds = injectDataIntoSystem(uri, data);
    //    return layerIds;
}

int VectorDataExampleContent::extractData(const QString& uri)
{
    // extract vector data from uri here
    return uri.size();
}

QList<LmCdl::VectorDataLayerId> VectorDataExampleContent::injectDataIntoSystem(const QString& uri, int /*data*/)
{
    LmCdl::VectorDataLayerId id = layerRegistry_.createLayer(uri, "foo layer");
    LmCdl::I_VectorDataLayer& layer = layerRegistry_.layerById(id);

    layer.createPoint().setFeatureLabel("point data");
    layer.createLine().setFeatureLabel("line string data");
    layer.createPolygon().setFeatureLabel("polygon data");

    // return IDs of created layers
    return QList<LmCdl::VectorDataLayerId> { id };
}