#pragma once

#include <LmCdl/UniqueIdentifier.h>
#include <QObject>
#include <QtGlobal>
#include <memory>
#include <unordered_map>
#include <LmCdl/VcsiIdentifiedPointOfInterest.h>
#include <LmCdl/VcsiPointOfInterestProperties.h>
#include <LmCdl/I_ContextMenu.h>
#include <qgeocoordinate.h>
#include <LmCdl/VcsiMilStdCode.h>
#include <LmCdl/I_ContextMenuItem.h>
#include <LmCdl/I_VcsiMapExtensionApi.h>
#include <LmCdl/I_PointOfInterestApi.h>
#include <LmCdl/I_VcsiUserNotificationApi.h>
#include <LmCdl/I_UserNotification.h>


namespace LmCdl {
class I_VcsiMapExtensionApi;
class I_PointOfInterestApi;
class I_VcsiApplicationApi;
}

class MissionPlanningContentCreator : public QObject {
    Q_OBJECT
public:
    MissionPlanningContentCreator(LmCdl::I_VcsiMapExtensionApi& mapApi, LmCdl::I_PointOfInterestApi& poiApi, LmCdl::I_VcsiUserNotificationApi& notApi);
    virtual ~MissionPlanningContentCreator();
    void publishAndMapPointOfInterest(LmCdl::VcsiPointOfInterestId sourceId, const LmCdl::VcsiPointOfInterestProperties& pointOfInterest);
    void getPoiProperties(const LmCdl::ContextMenuEvent &event);
    void connectToApiSignals();
    void removeNotification();

private:
    Q_DISABLE_COPY(MissionPlanningContentCreator);

    LmCdl::I_ContextMenuItem& contextMenuItem_;
    QHash<LmCdl::VcsiPointOfInterestId, LmCdl::VcsiPointOfInterestId> sourceIdsToClonedIds_;
    LmCdl::I_PointOfInterestApi& poiApi_;
    LmCdl::I_VcsiUserNotificationApi& notApi_;
    LmCdl::I_UserNotification* notification_;
};
