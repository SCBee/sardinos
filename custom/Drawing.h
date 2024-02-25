#pragma once

#include <QObject>

#include <BoundingBox.h>
#include <Line.h>
#include <LmCdl/I_VcsiUserNotificationApi.h>
#include <LmCdl/I_VectorDataDrawing.h>
#include <LmCdl/I_VectorDataDrawingApi.h>
#include <LmCdl/I_VectorDataLineDrawing.h>
#include <LmCdl/I_VectorDataPointDrawing.h>
#include <LmCdl/I_VectorDataPolygonDrawing.h>
#include <MissionDomain.h>
#include <Notifications.h>
#include <Polygon.h>

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

    void addPolygon(Polygon* polygon);

    void addPolygons(const QList<Polygon*>& polygon);

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

    void draw(const QList<Polygon*>& polygons,
              const QList<Line*>& lines,
              LmCdl::I_VectorDataDrawingApi& drawApi_);

    [[nodiscard]] bool visible() const override;

private:
    QSet<LmCdl::I_VectorDataPointDrawing*> points_;
    QSet<LmCdl::I_VectorDataLineDrawing*> lines_;
    QSet<LmCdl::I_VectorDataPolygonDrawing*> polygons_;
};