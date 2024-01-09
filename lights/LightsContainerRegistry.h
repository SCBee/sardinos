/**
\file
Copyright (c) 2018 Lockheed Martin Corp. All rights reserved.
*/

#pragma once

#include <LmCdl/UniqueIdentifier.h>
#include <QHash>
#include <QObject>

class LightsContainer;

namespace LmCdl {
class I_ActionApi;
class I_QmlApi;
class I_VcsiApplicationApi;
class I_VcsiVehicleExtensionApi;
class I_Vehicle;
class I_VehicleCollectionApi;
}

/*
 * Responsible for creating/removing the UI and business logic container for
 * each vehicle
 */
class LightsContainerRegistry : public QObject {
    Q_OBJECT
public:
    LightsContainerRegistry(LmCdl::I_VehicleCollectionApi& vehicleCollectionApi,
        LmCdl::I_VcsiApplicationApi& applicationApi, LmCdl::I_QmlApi& qmlApi, LmCdl::I_ActionApi& actionApi);
    virtual ~LightsContainerRegistry();

private slots:
    void onVehicleDiscovered(LmCdl::UniqueIdentifier vehicleId);
    void onVehicleAboutToBeRemoved(LmCdl::UniqueIdentifier vehicleId);

private:
    bool doesVehicleTypeNeedThisEnginePanel(LmCdl::I_Vehicle& vehicle);

    LmCdl::I_VehicleCollectionApi& vehicleCollectionApi_;
    LmCdl::I_VcsiApplicationApi& applicationApi_;
    LmCdl::I_QmlApi& qmlApi_;
    LmCdl::I_ActionApi& actionApi_;

    QHash<LmCdl::UniqueIdentifier, LightsContainer*> containers_;
};
