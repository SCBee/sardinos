/**
\file
Copyright (c) 2020 Lockheed Martin Corp. All rights reserved.
*/

#include <VectorLayerIdFeatureIdPair.h>

static const int vectorLayerIdFeatureIdMetaTypeId = qRegisterMetaType<VectorLayerIdFeatureIdPair>();
template int qMetaTypeId<VectorLayerIdFeatureIdPair>();

VectorLayerIdFeatureIdPair::VectorLayerIdFeatureIdPair(
    LmCdl::VectorDataLayerId layerId, LmCdl::VectorDataFeatureId featureId)
    : layerId_(layerId)
    , featureId_(featureId)
{
}

VectorLayerIdFeatureIdPair::~VectorLayerIdFeatureIdPair() { }
