#include <QGeoRectangle>
#include <QSet>

#include <Drawing.h>
#include <Polygon.h>
#include <Sardinos.h>

Drawing::Drawing() = default;

Drawing::~Drawing() = default;

const QSet<LmCdl::I_VectorDataPointDrawing*>& Drawing::pointDrawings(const QGeoRectangle&) const
{
    return points_;
}

const QSet<LmCdl::I_VectorDataLineDrawing*>& Drawing::lineDrawings(const QGeoRectangle&) const
{
    return lines_;
}

const QSet<LmCdl::I_VectorDataPolygonDrawing*>& Drawing::polygonDrawings(const QGeoRectangle&) const
{
    return polygons_;
}

void Drawing::addPolygon(Polygon* polygon)
{
    polygons_.insert(polygon);
}

void Drawing::addPolygons(
    const QList<Polygon*>& polygons)
{
    for (auto polygon : polygons)
        addPolygon(polygon);
}

void Drawing::addLine(Line* line)
{
    lines_.insert(line);
}

void Drawing::addLines(const QList<Line*>& lines)
{
    for (auto line : lines)
        addLine(line);
}

void Drawing::clear()
{
    polygons_.clear();
    lines_.clear();

    update();
}

void Drawing::update()
{
    emit visibilityChanged(true);
    emit drawingChanged();
}

bool Drawing::visible() const
{
    return true;
}

void Drawing::drawFlightPath(
    MissionDomain& mission_, LmCdl::I_MissionDrawingApi& missionApi_)
{
    foreach(Waypoint* waypoint, mission_.waypoints()) {
        missionApi_.addDrawingForWaypoint(*waypoint);
    }
    foreach(LmCdl::I_SimpleWaypointConnector* waypointConnector,
            mission_.waypointConnectors())
    {
        missionApi_.addDrawingForWaypointConnector(*waypointConnector);
    }
}

void Drawing::clearFlightPath(
    MissionDomain& mission_, LmCdl::I_MissionDrawingApi& missionApi_)
{
    foreach(Waypoint* waypoint, mission_.waypoints()) {
        missionApi_.removeDrawingForWaypoint(*waypoint);
    }
    foreach(LmCdl::I_SimpleWaypointConnector* waypointConnector,
            mission_.waypointConnectors())
    {
        missionApi_.removeDrawingForWaypointConnector(*waypointConnector);
    }
}

void Drawing::clearMissionArea(
    LmCdl::I_VectorDataDrawingApi& drawApi_)
{
    draw(QList<Polygon*>(),
         QList<Line*>(),
         drawApi_);
}

void Drawing::draw(
    const QList<Polygon*>& polygons,
    const QList<Line*>& lines,
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

Q_SLOT void Drawing::drawMissionArea(
    std::vector<std::vector<QGeoCoordinate>> pois_,
    BoundingBox& missionBounds_,
    LmCdl::I_VectorDataDrawingApi& drawApi_)
{
    auto polygon = QGeoPolygon(missionBounds_.list());

    auto polygons = QList<Polygon*>();

    polygons.append(new Polygon(polygon));

    auto lines = QList<Line*>();

    sardinos::cvhull(pois_);

    for (auto i = 1; i < pois_.size(); i++) {
        lines.push_back(new Line(pois_[i][0], pois_[i - 1][0]));
        if (i == pois_.size() - 1)
            lines.push_back(new Line(pois_[i][0], pois_[0][0]));
    }

    draw(polygons, lines, drawApi_);
}
