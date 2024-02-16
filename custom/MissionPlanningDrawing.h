#pragma once

#include <QObject>

#include <LmCdl/I_VectorDataDrawing.h>
#include <LmCdl/I_VectorDataLineDrawing.h>
#include <LmCdl/I_VectorDataPointDrawing.h>
#include <LmCdl/I_VectorDataPolygonDrawing.h>
#include <MissionPlanningLine.h>
#include <MissionPlanningPolygon.h>

class MissionPlanningDrawing : public LmCdl::I_VectorDataDrawing
{
    Q_OBJECT

public:
    MissionPlanningDrawing();

    ~MissionPlanningDrawing() override;

    [[nodiscard]] const QSet<LmCdl::I_VectorDataPointDrawing*>& pointDrawings(
        const QGeoRectangle& area) const override;

    [[nodiscard]] const QSet<LmCdl::I_VectorDataLineDrawing*>& lineDrawings(
        const QGeoRectangle& area) const override;

    [[nodiscard]] const QSet<LmCdl::I_VectorDataPolygonDrawing*>&
    polygonDrawings(const QGeoRectangle& area) const override;

    void addPolygon(MissionPlanningPolygon* polygon);

    void addPolygons(const QList<MissionPlanningPolygon*>& polygon);

    void addLine(MissionPlanningLine* line);

    void addLines(const QList<MissionPlanningLine*>& lines);

    void clear();

    void update();

    [[nodiscard]] bool visible() const override;

private:
    QSet<LmCdl::I_VectorDataPointDrawing*> points_;
    QSet<LmCdl::I_VectorDataLineDrawing*> lines_;
    QSet<LmCdl::I_VectorDataPolygonDrawing*> polygons_;
};