#include <Polygon.h>

Polygon::Polygon(const QGeoPolygon& polygon)
    : polygon_(polygon)
{
}

Polygon::~Polygon() = default;

QGeoPolygon Polygon::polygon() const
{
    return polygon_;
}

QPen Polygon::pen() const
{
    QPen pen = QPen(Qt::SolidLine);
    pen.setWidth(4);
    pen.setColor(QColor(255, 255, 255, 100));
    return pen;
}

QBrush Polygon::brush() const
{
    return {Qt::NoBrush};
}
