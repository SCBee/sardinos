#include <QApplication>
#include <QGeoCoordinate>
#include <QLabel>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>

class DroneWidget : public QWidget
{
public:
    DroneWidget()
    {
        move(200, 0);

        latitude_label = new QLabel("Latitude: ", this);
        longitude_label = new QLabel("Longitude: ", this);
        altitude_label = new QLabel("Altitude: ", this);
        speed_label = new QLabel("Speed: ", this);
        heading_label = new QLabel("Heading: ", this);
        yaw_label = new QLabel("Yaw: ", this);
        battery_label = new QLabel("Battery: ", this);

        QVBoxLayout* layout = new QVBoxLayout(this);
        layout->addWidget(latitude_label);
        layout->addWidget(longitude_label);
        layout->addWidget(altitude_label);
        layout->addWidget(speed_label);
        layout->addWidget(heading_label);
        layout->addWidget(yaw_label);
        layout->addWidget(battery_label);
    }

    void updateValues(double latitude,
                      double longitude,
                      double altitude,
                      double heading,
                      double speed,
                      double yaw,
                      double battery)
    {
        auto degreeSymbol = static_cast<char>(176);

        latitude_label->setText("Latitude: " + QString::number(latitude));
        longitude_label->setText("Longitude: " + QString::number(longitude));
        altitude_label->setText("Altitude: " + QString::number(altitude) + " m AGL");
        speed_label->setText("Speed: " + QString::number(speed) + " m/s");
        heading_label->setText("Heading: " + QString::number(heading) + degreeSymbol);
        yaw_label->setText("Yaw: " + QString::number(yaw) + degreeSymbol);
        battery_label->setText("Battery: " + QString::number(battery * 100) + "%");
    }

private:
    QLabel* latitude_label;
    QLabel* longitude_label;
    QLabel* altitude_label;
    QLabel* speed_label;
    QLabel* heading_label;
    QLabel* yaw_label;
    QLabel* battery_label;
};