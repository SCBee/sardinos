/**
 \file
 Copyright (c) 2020 Lockheed Martin Corp. All rights reserved.
 */

#pragma once

#include <LmCdl/SpeedUnit.h>
#include <LmCdl/SpeedUnitPreference.h>
#include <LmCdl/UniqueIdentifier.h>
#include <QLabel>
#include <QObject>

namespace LmCdl {
class I_UnitPreferencesApi;
class I_VcsiVehicleExtensionApi;
class I_Vehicle;
}

class SingleVehicleStatusDisplay : public QObject {
    Q_OBJECT
public:
    SingleVehicleStatusDisplay(LmCdl::I_Vehicle& vehicle, LmCdl::I_VcsiVehicleExtensionApi& vehicleExtensionApi,
        LmCdl::UniqueIdentifier vehicleId, LmCdl::I_UnitPreferencesApi& unitPreferencesApi);

private:
    Q_DISABLE_COPY(SingleVehicleStatusDisplay)

    QWidget* createUi(LmCdl::I_VcsiVehicleExtensionApi& vehicleExtensionApi, LmCdl::UniqueIdentifier vehicleId);
    void registerUi(
        LmCdl::I_VcsiVehicleExtensionApi& vehicleExtensionApi, LmCdl::UniqueIdentifier vehicleId, QWidget* mainWidget);
    void initializeDisplayContents(LmCdl::UniqueIdentifier vehicleId);

    void updateLatitudeLongitude();
    void updateAltitude();
    void updateAirspeed();

    QString speedUnitSymbol(LmCdl::SpeedUnit unit) const;

    LmCdl::I_Vehicle& vehicle_;
    LmCdl::I_UnitPreferencesApi& unitPreferencesApi_;

    QLabel* idDisplay_;
    QLabel* latitudeLongitudeDisplay_;
    QLabel* altitudeDisplay_;
    QLabel* airspeedDisplay_;
};
