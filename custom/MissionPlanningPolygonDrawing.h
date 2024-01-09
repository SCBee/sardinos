#pragma once

#include <LmCdl/I_VectorDataPointDrawing.h>
#include <LmCdl/I_VectorDataLineDrawing.h>
#include <LmCdl/I_VectorDataPolygonDrawing.h>
#include <LmCdl/I_VectorDataDrawing.h>
#include <MissionPlanningPolygon.h>
#include <QObject>

class MissionPlanningPolygonDrawing : public LmCdl::I_VectorDataDrawing {
    Q_OBJECT

public:
    MissionPlanningPolygonDrawing();
    virtual ~MissionPlanningPolygonDrawing();

    const QSet<LmCdl::I_VectorDataPointDrawing*>& pointDrawings(const QGeoRectangle& area) const override;
    const QSet<LmCdl::I_VectorDataLineDrawing*>& lineDrawings(const QGeoRectangle& area) const override;
    const QSet<LmCdl::I_VectorDataPolygonDrawing*>& polygonDrawings(const QGeoRectangle& area) const override;
    const QSet<LmCdl::I_VectorDataPolygonDrawing*>& polygonDrawings();
    void addPolygon(MissionPlanningPolygon* polygon);
    void clear();
    void update();

    bool visible() const override;

signals:
    void visibilityChanged(bool visible);

    void drawingChanged();

private:
    QSet<LmCdl::I_VectorDataPointDrawing*> points_;
    QSet<LmCdl::I_VectorDataLineDrawing*> lines_;
    QSet<LmCdl::I_VectorDataPolygonDrawing*> polygons_;
};