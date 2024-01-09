/**
\file
Copyright (c) 2018 Lockheed Martin Corp. All rights reserved.
*/

#pragma once

#include <LmCdl/UniqueIdentifier.h>
#include <QObject>
#include <QtGlobal>
#include <memory>
#include <unordered_map>

namespace LmCdl {
class I_VehicleCollectionApi;
class UniqueIdentifier;
}

class GroundModeContent;

class GroundModeContentCreator : public QObject {
public:
    explicit GroundModeContentCreator(LmCdl::I_VehicleCollectionApi& api);
    virtual ~GroundModeContentCreator();

private:
    void addContentForVehicle(LmCdl::UniqueIdentifier vehicleId);
    void removeContentForVehicle(LmCdl::UniqueIdentifier vehicleId);

    Q_DISABLE_COPY(GroundModeContentCreator)

private:
    LmCdl::I_VehicleCollectionApi& api_;
    std::unordered_map<LmCdl::UniqueIdentifier, std::unique_ptr<GroundModeContent>> content_;
};
