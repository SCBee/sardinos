#include <QGeoRectangle>
#include <QSet>

#include <MissionPlanningDrawing.h>
#include <MissionPlanningPolygon.h>
#include <Sardinos.h>

MissionPlanningDrawing::MissionPlanningDrawing() = default;

MissionPlanningDrawing::~MissionPlanningDrawing() = default;

const QSet<LmCdl::I_VectorDataPointDrawing*>&
MissionPlanningDrawing::pointDrawings(const QGeoRectangle&) const
{
    return points_;
}

const QSet<LmCdl::I_VectorDataLineDrawing*>&
MissionPlanningDrawing::lineDrawings(const QGeoRectangle&) const
{
    return lines_;
}

const QSet<LmCdl::I_VectorDataPolygonDrawing*>&
MissionPlanningDrawing::polygonDrawings(const QGeoRectangle&) const
{
    return polygons_;
}

void MissionPlanningDrawing::addPolygon(MissionPlanningPolygon* polygon)
{
    polygons_.insert(polygon);
}

void MissionPlanningDrawing::addPolygons(
    const QList<MissionPlanningPolygon*>& polygons)
{
    for (auto polygon : polygons)
        addPolygon(polygon);
}

void MissionPlanningDrawing::addLine(MissionPlanningLine* line)
{
    lines_.insert(line);
}

void MissionPlanningDrawing::addLines(const QList<MissionPlanningLine*>& lines)
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

void MissionPlanningDrawing::drawFlightPath(
    MissionDomain& mission_, LmCdl::I_MissionDrawingApi& missionApi_)
{
    foreach(MissionPlanningWaypoint* waypoint, mission_.waypoints()) {
        missionApi_.addDrawingForWaypoint(*waypoint);
    }
    foreach(LmCdl::I_SimpleWaypointConnector* waypointConnector,
            mission_.waypointConnectors())
    {
        missionApi_.addDrawingForWaypointConnector(*waypointConnector);
    }
}

void MissionPlanningDrawing::clearFlightPath(
    MissionDomain& mission_, LmCdl::I_MissionDrawingApi& missionApi_)
{
    foreach(MissionPlanningWaypoint* waypoint, mission_.waypoints()) {
        missionApi_.removeDrawingForWaypoint(*waypoint);
    }
    foreach(LmCdl::I_SimpleWaypointConnector* waypointConnector,
            mission_.waypointConnectors())
    {
        missionApi_.removeDrawingForWaypointConnector(*waypointConnector);
    }
}

void MissionPlanningDrawing::clearMissionArea(
    LmCdl::I_VectorDataDrawingApi& drawApi_)
{
    draw(QList<MissionPlanningPolygon*>(),
         QList<MissionPlanningLine*>(),
         drawApi_);
}

void MissionPlanningDrawing::draw(
    const QList<MissionPlanningPolygon*>& polygons,
    const QList<MissionPlanningLine*>& lines,
    LmCdl::I_VectorDataDrawingApi& drawApi_)
{
    clear();
    addPolygons(polygons);
    addLines(lines);
    update();

    drawApi_.removeDrawingForVectorData(*this);
    drawApi_.addDrawingForVectorData(
        *this,
        LmCdl::I_VectorDataDrawingApi::DrawingMode::
            OptimizedForFrequentChanges);
}

Q_SLOT void MissionPlanningDrawing::drawMissionArea(
    std::vector<std::vector<QGeoCoordinate>> pois_,
    BoundingBox& missionBounds_,
    LmCdl::I_VectorDataDrawingApi& drawApi_)
{
    auto polygon = QGeoPolygon(missionBounds_.list());

    auto polygons = QList<MissionPlanningPolygon*>();

    polygons.append(new MissionPlanningPolygon(polygon));

    auto lines = QList<MissionPlanningLine*>();

    sardinos::cvhull(pois_);

    for (auto i = 1; i < pois_.size(); i++) {
        lines.push_back(new MissionPlanningLine(pois_[i][0], pois_[i - 1][0]));
        if (i == pois_.size() - 1)
            lines.push_back(new MissionPlanningLine(pois_[i][0], pois_[0][0]));
    }

    draw(polygons, lines, drawApi_);
}
