/**
\file
Copyright (c) 2017 Lockheed Martin Corp. All rights reserved.
*/

#pragma once

#include <LmCdl/UniqueIdentifier.h>
#include <QHash>
#include <QObject>
class CloakingDevicePower;
class QLabel;
namespace LmCdl {
class I_DownloadedRouteCollection;
}

class DownloadedCloakingDevicePowerView : public QObject {
public:
    DownloadedCloakingDevicePowerView(
        const CloakingDevicePower& cloakingDevicePower, LmCdl::I_DownloadedRouteCollection& routes);
    virtual ~DownloadedCloakingDevicePowerView();

private:
    void updateCloakingDeviceDisplay(LmCdl::UniqueIdentifier waypointId);
    void createNewCloakingDeviceWidget(LmCdl::UniqueIdentifier waypointId);

private:
    Q_DISABLE_COPY(DownloadedCloakingDevicePowerView)

    const CloakingDevicePower& cloakingDevicePower_;
    LmCdl::I_DownloadedRouteCollection& routes_;

    QHash<LmCdl::UniqueIdentifier, QLabel*> cloakingDeviceLabels_;
};
