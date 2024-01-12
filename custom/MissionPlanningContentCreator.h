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
#include <MissionPlanningDrawing.h>
#include <MissionPlanningPolygon.h>
#include <MissionPlanningLine.h>
#include<QList>
#include <vector>

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

    void updateDrawing();

    void draw(QList<MissionPlanningPolygon*> polygons, QList<MissionPlanningLine*> lines);

    void delay(int ms);

    std::vector<double> sqPolar(QGeoCoordinate &point, QGeoCoordinate &com);
    
    void cvhull();

    LmCdl::I_ContextMenuItem &contextMenuItem_;
    std::vector<std::vector<QGeoCoordinate>> pois_;
    LmCdl::I_PointOfInterestApi &poiApi_;
    LmCdl::I_VcsiUserNotificationApi &notApi_;
    LmCdl::I_VectorDataDrawingApi &drawApi_;
    LmCdl::I_UserNotification *notification_;
    MissionPlanningDrawing *drawing_ = new MissionPlanningDrawing();
};
