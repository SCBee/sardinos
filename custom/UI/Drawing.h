#pragma once

#include <QObject>
#include <QGeoRectangle>
#include <QSet>

#include <LmCdl/I_VcsiUserNotificationApi.h>
#include <LmCdl/I_VectorDataDrawing.h>
#include <LmCdl/I_VectorDataDrawingApi.h>
#include <LmCdl/I_VectorDataLineDrawing.h>
#include <LmCdl/I_VectorDataPointDrawing.h>
#include <LmCdl/I_VectorDataPolygonDrawing.h>

#include "Polygon.h"
#include "Line.h"
#include "Notifications.h"
#include <Helpers/Sardinos.h>
#include <Helpers/BoundingBox.h>
#include <Waypoint/MissionDomain.h>

class Drawing : public LmCdl::I_VectorDataDrawing
{
    Q_OBJECT

public:
    Drawing();

    ~Drawing() override;

    [[nodiscard]] const QSet<LmCdl::I_VectorDataPointDrawing*>& pointDrawings(
        const QGeoRectangle& area) const override;

    [[nodiscard]] const QSet<LmCdl::I_VectorDataLineDrawing*>& lineDrawings(
        const QGeoRectangle& area) const override;

    [[nodiscard]] const QSet<LmCdl::I_VectorDataPolygonDrawing*>&
    polygonDrawings(const QGeoRectangle& area) const override;

    void addPolygon(class Polygon* polygon);

    void addPolygons(const QList<class Polygon*>& polygon);

    void addLine(Line* line);

    void addLines(const QList<Line*>& lines);

    void clear();

    void update();

    static void drawFlightPath(MissionDomain& mission_,
                               LmCdl::I_MissionDrawingApi& missionApi_);

    void drawMissionArea(std::vector<std::vector<QGeoCoordinate>> pois_,
                         BoundingBox& missionBounds_,
                         LmCdl::I_VectorDataDrawingApi& drawApi_);

    static void clearFlightPath(MissionDomain& mission_,
                                LmCdl::I_MissionDrawingApi& missionApi_);

    void clearMissionArea(LmCdl::I_VectorDataDrawingApi& drawApi_);

    void draw(const QList<class Polygon*>& polygons,
              const QList<Line*>& lines,
              LmCdl::I_VectorDataDrawingApi& drawApi_);

    [[nodiscard]] bool visible() const override;

private:
    QSet<LmCdl::I_VectorDataPointDrawing*> points_;
    QSet<LmCdl::I_VectorDataLineDrawing*> lines_;
    QSet<LmCdl::I_VectorDataPolygonDrawing*> polygons_;
};