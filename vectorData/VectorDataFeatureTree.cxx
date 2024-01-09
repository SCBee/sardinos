/**
\file
Copyright (c) 2020 Lockheed Martin Corp. All rights reserved.
*/

#include <VectorDataFeatureTree.h>

#include <LmCdl/I_VectorDataLayer.h>
#include <LmCdl/I_VectorDataLayerRegistry.h>
#include <LmCdl/VectorDataFeature.h>
#include <LmCdl/VectorDataFeatureId.h>
#include <LmCdl/VectorDataLineString.h>
#include <LmCdl/VectorDataPoint.h>
#include <LmCdl/VectorDataPolygon.h>

#include <QTreeWidget>
#include <QTreeWidgetItem>

VectorDataFeatureTree::VectorDataFeatureTree(LmCdl::I_VectorDataLayerRegistry& layerRegistry, QTreeWidget& treeWidget)
    : layerRegistry_(layerRegistry)
    , treeWidget_(treeWidget)
{
    connect(&treeWidget_, &QTreeWidget::itemDoubleClicked, this, &VectorDataFeatureTree::onItemDoubleClicked);
}

VectorDataFeatureTree::~VectorDataFeatureTree() { }

void VectorDataFeatureTree::loadTree(QList<LmCdl::VectorDataLayerId> vectorLayers)
{
    treeWidget_.clear();

    foreach (LmCdl::VectorDataLayerId id, vectorLayers) {
        QTreeWidgetItem* topLevelItem = createTreeWidgetItemForLayer(layerRegistry_.layerById(id));
        treeWidget_.addTopLevelItem(topLevelItem);
    }
}

void VectorDataFeatureTree::onItemDoubleClicked(QTreeWidgetItem* item, int /*column*/)
{
    QVariant v = item->data(0, Qt::UserRole);
    if (v.canConvert<VectorLayerIdFeatureIdPair>()) {
        VectorLayerIdFeatureIdPair pair = v.value<VectorLayerIdFeatureIdPair>();
        emit featureDoubleClicked(pair);
    }
}

QTreeWidgetItem* VectorDataFeatureTree::createTreeWidgetItemForLayer(LmCdl::I_VectorDataLayer& layer)
{
    QTreeWidgetItem* layerItem = new QTreeWidgetItem();
    layerItem->setText(0, layer.layerName());

    QList<LmCdl::VectorDataFeatureId> points = layer.allPoints();
    foreach (LmCdl::VectorDataFeatureId pointId, points) {
        const LmCdl::VectorDataPoint* point = layer.pointById(pointId);
        if (point) {
            QTreeWidgetItem* childItem = new QTreeWidgetItem(layerItem);
            childItem->setText(0, featureNameOrUseDefault(point, "Point"));
            childItem->setText(1, point->location().toString());
            VectorLayerIdFeatureIdPair uniqueId(layer.layerId(), pointId);
            childItem->setData(0, Qt::UserRole, QVariant::fromValue<VectorLayerIdFeatureIdPair>(uniqueId));
            createTreeWidgetItemForFeature(childItem, *point);
        }
    }
    QList<LmCdl::VectorDataFeatureId> lines = layer.allLines();
    foreach (LmCdl::VectorDataFeatureId lineId, lines) {
        const LmCdl::VectorDataLineString* line = layer.lineById(lineId);
        if (line) {
            QTreeWidgetItem* childItem = new QTreeWidgetItem(layerItem);
            childItem->setText(0, featureNameOrUseDefault(line, "Line"));
            childItem->setText(1, line->locations().toString());
            VectorLayerIdFeatureIdPair uniqueId(layer.layerId(), lineId);
            childItem->setData(0, Qt::UserRole, QVariant::fromValue<VectorLayerIdFeatureIdPair>(uniqueId));
            createTreeWidgetItemForFeature(childItem, *line);
        }
    }
    QList<LmCdl::VectorDataFeatureId> polygons = layer.allPolygons();
    foreach (LmCdl::VectorDataFeatureId polygonId, polygons) {
        const LmCdl::VectorDataPolygon* polygon = layer.polygonById(polygonId);
        if (polygon) {
            QTreeWidgetItem* childItem = new QTreeWidgetItem(layerItem);
            childItem->setText(0, featureNameOrUseDefault(polygon, "Polygon"));
            childItem->setText(1, polygon->geometry().toString());
            VectorLayerIdFeatureIdPair uniqueId(layer.layerId(), polygonId);
            childItem->setData(0, Qt::UserRole, QVariant::fromValue<VectorLayerIdFeatureIdPair>(uniqueId));
            createTreeWidgetItemForFeature(childItem, *polygon);
        }
    }
    return layerItem;
}

void VectorDataFeatureTree::createTreeWidgetItemForFeature(
    QTreeWidgetItem* parentItem, const LmCdl::VectorDataFeature& feature)
{
    QStringList attributes = feature.attributeList();
    foreach (QString attribute, attributes) {
        QTreeWidgetItem* item = new QTreeWidgetItem(parentItem);
        item->setText(0, attribute);
        item->setText(1, feature.getAttribute(attribute));
    }
}

QString VectorDataFeatureTree::featureNameOrUseDefault(
    const LmCdl::VectorDataFeature* feature, const QString& defaultName)
{
    if (feature && feature->attributeList().contains("Name", Qt::CaseInsensitive)) {
        return feature->getAttribute("Name");
    }
    return defaultName;
}
