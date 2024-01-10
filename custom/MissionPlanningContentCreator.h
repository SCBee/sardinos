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
#include <LmCdl/I_VectorDataDrawingApi.h>
#include <MissionPlanningPolygonDrawing.h>
#include <MissionPlanningPolygon.h>
#include<QList>

namespace LmCdl {
    class I_VcsiMapExtensionApi;

    class I_PointOfInterestApi;

    class I_VcsiApplicationApi;
}

class MissionPlanningContentCreator : public QObject {
Q_OBJECT
public:
    MissionPlanningContentCreator(LmCdl::I_VcsiMapExtensionApi &mapApi, LmCdl::I_PointOfInterestApi &poiApi,
                                  LmCdl::I_VcsiUserNotificationApi &notApi, LmCdl::I_VectorDataDrawingApi &drawApi);

    virtual ~MissionPlanningContentCreator();

private:
    Q_DISABLE_COPY(MissionPlanningContentCreator);

    void publishAndMapPointOfInterest(LmCdl::VcsiPointOfInterestId sourceId,
                                      const LmCdl::VcsiPointOfInterestProperties &pointOfInterest);

    void getPoiProperties(const LmCdl::ContextMenuEvent &event);

    void connectToApiSignals();

    void removeNotification();

    void updatePolygon();

    void clearDrawing();

    void removePoi(LmCdl::VcsiPointOfInterestId id);

    LmCdl::I_ContextMenuItem &contextMenuItem_;
    QHash<LmCdl::VcsiPointOfInterestId, LmCdl::VcsiPointOfInterestProperties> pois_;
    LmCdl::I_PointOfInterestApi &poiApi_;
    LmCdl::I_VcsiUserNotificationApi &notApi_;
    LmCdl::I_VectorDataDrawingApi &drawApi_;
    LmCdl::I_UserNotification *notification_;
    MissionPlanningPolygonDrawing *drawing_ = new MissionPlanningPolygonDrawing();
};
