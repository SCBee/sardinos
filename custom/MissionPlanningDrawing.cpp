#include <QGeoRectangle>
#include <QSet>

#include <MissionPlanningDrawing.h>
#include <MissionPlanningPolygon.h>

MissionPlanningDrawing::MissionPlanningDrawing() {}

MissionPlanningDrawing::~MissionPlanningDrawing() {}

const QSet<LmCdl::I_VectorDataPointDrawing*>&
MissionPlanningDrawing::pointDrawings(const QGeoRectangle& area) const
{
    return points_;
}

const QSet<LmCdl::I_VectorDataLineDrawing*>&
MissionPlanningDrawing::lineDrawings(const QGeoRectangle& area) const
{
    return lines_;
}

const QSet<LmCdl::I_VectorDataPolygonDrawing*>&
MissionPlanningDrawing::polygonDrawings(const QGeoRectangle& area) const
{
    return polygons_;
}

void MissionPlanningDrawing::addPolygon(MissionPlanningPolygon* polygon)
{
    polygons_.insert(polygon);
}

void MissionPlanningDrawing::addPolygons(
    QList<MissionPlanningPolygon*> polygons)
{
    for (auto polygon : polygons)
        addPolygon(polygon);
}

void MissionPlanningDrawing::addLine(MissionPlanningLine* line)
{
    lines_.insert(line);
}

void MissionPlanningDrawing::addLines(QList<MissionPlanningLine*> lines)
{
    for (auto line : lines)
        addLine(line);
}

void MissionPlanningDrawing::clear()
{
    polygons_.clear();
    lines_.clear();

    update();
}

void MissionPlanningDrawing::update()
{
    emit visibilityChanged(true);
    emit drawingChanged();
}

bool MissionPlanningDrawing::visible() const
{
    return true;
}
