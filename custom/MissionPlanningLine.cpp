#include <MissionPlanningLine.h>

MissionPlanningLine::MissionPlanningLine(QGeoCoordinate start, QGeoCoordinate end)
  : start_(start), end_(end) {
    path_.append(start_);
    path_.append(end_);
}

MissionPlanningLine::~MissionPlanningLine(){}

QPen MissionPlanningLine::pen() const
{
    QPen pen = QPen(Qt::SolidLine);
    pen.setWidth(2);
    pen.setColor(QColor(9, 60, 214, 170));
    return pen;
}

QGeoPath MissionPlanningLine::path() const { return QGeoPath(path_); }


void MissionPlanningLine::updateStart(QGeoCoordinate newStart){
    start_ = newStart;

    path_.clear();
    path_.append(start_);
    path_.append(end_);
}

void MissionPlanningLine::updateEnd(QGeoCoordinate newEnd){
    end_ = newEnd;

    path_.clear();
    path_.append(start_);
    path_.append(end_);
}