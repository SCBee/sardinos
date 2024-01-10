#include <MissionPlanningPolygonDrawing.h>
#include <QSet>
#include <QGeoRectangle>
#include <MissionPlanningPolygon.h>

MissionPlanningPolygonDrawing::MissionPlanningPolygonDrawing() {
}

MissionPlanningPolygonDrawing::~MissionPlanningPolygonDrawing() {}

const QSet<LmCdl::I_VectorDataPointDrawing *> &
MissionPlanningPolygonDrawing::pointDrawings(const QGeoRectangle &area) const {
    return points_;
}

const QSet<LmCdl::I_VectorDataLineDrawing *> &
MissionPlanningPolygonDrawing::lineDrawings(const QGeoRectangle &area) const {
    return lines_;
}

const QSet<LmCdl::I_VectorDataPolygonDrawing *> &
MissionPlanningPolygonDrawing::polygonDrawings(const QGeoRectangle &area) const {
    return polygons_;
}

const QSet<LmCdl::I_VectorDataPolygonDrawing *> &MissionPlanningPolygonDrawing::polygonDrawings() const {
    return polygons_;
}

void MissionPlanningPolygonDrawing::addPolygon(MissionPlanningPolygon *polygon) {
    polygons_.insert(polygon);
}

void MissionPlanningPolygonDrawing::clear() {
    polygons_.clear();
}

void MissionPlanningPolygonDrawing::update() {
    emit visibilityChanged(true);
    emit drawingChanged();
}

bool MissionPlanningPolygonDrawing::visible() const { return true; }
