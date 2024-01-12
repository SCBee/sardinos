#include <QGeoCoordinate>

struct BoundingBox {
    public:
    QGeoCoordinate SW;
    QGeoCoordinate NW;
    QGeoCoordinate SE;
    QGeoCoordinate NE;

    QList<QGeoCoordinate> list(){
        return { SW, NW, SE, NE };
    }
};