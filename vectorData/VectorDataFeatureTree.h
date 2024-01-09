/**
\file
Copyright (c) 2020 Lockheed Martin Corp. All rights reserved.
*/

#pragma once

#include <LmCdl/VectorDataLayerId.h>
#include <QObject>
#include <QString>
#include <VectorLayerIdFeatureIdPair.h>

namespace LmCdl {
class I_VectorDataLayer;
class I_VectorDataLayerRegistry;
class VectorDataFeature;
}

class QTreeWidget;
class QTreeWidgetItem;

class VectorDataFeatureTree : public QObject {
    Q_OBJECT
public:
    explicit VectorDataFeatureTree(LmCdl::I_VectorDataLayerRegistry& layerRegistry, QTreeWidget& treeWidget);
    virtual ~VectorDataFeatureTree();

public slots:
    void loadTree(QList<LmCdl::VectorDataLayerId> layers);

signals:
    void featureDoubleClicked(VectorLayerIdFeatureIdPair uniqueFeatureId);

private slots:
    void onItemDoubleClicked(QTreeWidgetItem* item, int column);

private:
    QTreeWidgetItem* createTreeWidgetItemForLayer(LmCdl::I_VectorDataLayer& layer);
    void createTreeWidgetItemForFeature(QTreeWidgetItem* parentItem, const LmCdl::VectorDataFeature& feature);
    QString featureNameOrUseDefault(const LmCdl::VectorDataFeature* feature, const QString& defaultName);

private:
    LmCdl::I_VectorDataLayerRegistry& layerRegistry_;
    QTreeWidget& treeWidget_;
};
