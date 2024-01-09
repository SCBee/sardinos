/**
\file
Copyright (c) 2018 Lockheed Martin Corp. All rights reserved.
*/

#include <LightsContainerRegistry.h>

#include <LightsContainer.h>
#include <LmCdl/I_VcsiApplicationApi.h>
#include <LmCdl/I_VcsiWidgetExtensionApi.h>
#include <LmCdl/I_Vehicle.h>
#include <LmCdl/I_VehicleCollectionApi.h>
#include <LmCdl/StanagVehicleType.h>

/*
 *  Connects to the Vehicle Collection API to detect when vehicles are
 * discovered or
 *  are about to be removed.
 */
LightsContainerRegistry::LightsContainerRegistry(LmCdl::I_VehicleCollectionApi& vehicleCollectionApi,
    LmCdl::I_VcsiApplicationApi& applicationApi, LmCdl::I_QmlApi& qmlApi, LmCdl::I_ActionApi& actionApi)
    : vehicleCollectionApi_(vehicleCollectionApi)
    , applicationApi_(applicationApi)
    , qmlApi_(qmlApi)
    , actionApi_(actionApi)
{
    connect(&vehicleCollectionApi_, &LmCdl::I_VehicleCollectionApi::vehicleAdded, this,
        &LightsContainerRegistry::onVehicleDiscovered);

    connect(&vehicleCollectionApi_, &LmCdl::I_VehicleCollectionApi::vehicleAboutToBeRemoved, this,
        &LightsContainerRegistry::onVehicleAboutToBeRemoved);
}

/*
 *  The plugin is being unloaded, de-allocate all containers.  No need to
 * disconnect signals,
 *  Qt handles this in QObject::~QObject.
 */
LightsContainerRegistry::~LightsContainerRegistry()
{
    qDeleteAll(containers_);
    containers_.clear();
}

/*
 *  Allocate new UI/business logic container when a specific type of vehicle is
 *discovered.
 *
 *  Note that the same vehicle can be discovered/removed numerous times
 */
void LightsContainerRegistry::onVehicleDiscovered(LmCdl::UniqueIdentifier vehicleId)
{
    LmCdl::I_Vehicle& vehicle = vehicleCollectionApi_.vehicle(vehicleId);

    if (!containers_.contains(vehicleId)) {
        containers_.insert(vehicleId, new LightsContainer(vehicle, vehicleId, applicationApi_, qmlApi_, actionApi_));
    } else {
        qWarning("Same vehicle %d discovered multiple times without prior "
                 "removal.  Should never get here...",
            vehicleId.value());
    }
}

/*
 * De-allocate UI/business logic for a particular vehicle.  Reset so that the
 * vehicle
 * can be re-discovered again.
 */
void LightsContainerRegistry::onVehicleAboutToBeRemoved(LmCdl::UniqueIdentifier vehicleId)
{
    if (containers_.contains(vehicleId)) {
        LightsContainer* doomedContainer = containers_.take(vehicleId);
        delete doomedContainer;
    }
}
