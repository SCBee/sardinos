/**
\file
Copyright (c) 2020 Lockheed Martin Corp. All rights reserved.
*/

#pragma once

#include <LmCdl/VectorDataFeatureId.h>
#include <LmCdl/VectorDataLayerId.h>
#include <QMetaType>

class VectorLayerIdFeatureIdPair {
public:
    explicit VectorLayerIdFeatureIdPair() { }
    explicit VectorLayerIdFeatureIdPair(LmCdl::VectorDataLayerId layerId, LmCdl::VectorDataFeatureId featureId);
    virtual ~VectorLayerIdFeatureIdPair();

    LmCdl::VectorDataLayerId layerId_;
    LmCdl::VectorDataFeatureId featureId_;
};

Q_DECLARE_METATYPE(VectorLayerIdFeatureIdPair)
extern template int qMetaTypeId<VectorLayerIdFeatureIdPair>();
