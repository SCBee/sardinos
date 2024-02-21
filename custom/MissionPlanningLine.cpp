#include <MissionPlanningLine.h>

MissionPlanningLine::MissionPlanningLine(const QGeoCoordinate& start,
                                         const QGeoCoordinate& end)
    : start_(start)
    , end_(end)
{
    path_.append(start_);
    path_.append(end_);
}

MissionPlanningLine::~MissionPlanningLine() = default;

QPen MissionPlanningLine::pen() const
{
    QPen pen = QPen(Qt::SolidLine);
    pen.setWidth(3);
    pen.setColor(QColor(72, 0, 102, 140));
    return pen;
}

QGeoPath MissionPlanningLine::path() const
{
    return path_;
}

[[maybe_unused]] void MissionPlanningLine::updateStart(
    const QGeoCoordinate& newStart)
{
    start_ = newStart;

    path_.clear();
    path_.append(start_);
    path_.append(end_);
}

[[maybe_unused]] void MissionPlanningLine::updateEnd(
    const QGeoCoordinate& newEnd)
{
    end_ = newEnd;

    path_.clear();
    path_.append(start_);
    path_.append(end_);
}