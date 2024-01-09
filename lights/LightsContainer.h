/**
\file
Copyright (c) 2018 Lockheed Martin Corp. All rights reserved.
*/

#pragma once

#include <LightsServiceContainer.h>
#include <LightsViewContainer.h>
#include <LmCdl/UniqueIdentifier.h>
#include <QObject>

namespace LmCdl {
class I_ActionApi;
class I_QmlApi;
class I_Vehicle;
class I_VcsiApplicationApi;
}

class LightsContainer : public QObject {
public:
    explicit LightsContainer(LmCdl::I_Vehicle& vehicle, LmCdl::UniqueIdentifier vehicleId,
        LmCdl::I_VcsiApplicationApi& applicationApi, LmCdl::I_QmlApi& qmlApi, LmCdl::I_ActionApi& actionApi);
    virtual ~LightsContainer();

private:
    Q_DISABLE_COPY(LightsContainer);

    LightsServiceContainer serviceContainer_;
    LightsViewContainer viewContainer_;
};
