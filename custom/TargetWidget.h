#pragma once

#include <QApplication>
#include <QGeoCoordinate>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>
#include <iostream>

class TargetWidget : public QWidget
{
public:
    TargetWidget(const double lat, const double lon, const QImage image)
    {
        // Create a QVBoxLayout to contain the button
        QVBoxLayout* layout = new QVBoxLayout(this);

        // Create a QPushButton
        auto button = new QPushButton("View Target");

        layout->addWidget(button);

        latLabel_   = new QLabel("Latitude: " + QString::number(lat));
        lonLabel_   = new QLabel("Longitude: " + QString::number(lon));
        imageLabel_ = new QLabel();

        imageLabel_->setPixmap(QPixmap::fromImage(image));

        setLayout(layout);


        connect(button,
                &QPushButton::clicked,
                [=]()
                {
                    visible_ = !visible_;
                    if (visible_) {
                        layout->addWidget(latLabel_);
                        layout->addWidget(lonLabel_);
                        layout->addWidget(imageLabel_);
                        button->setText("Close");
                    } else {
                        layout->removeWidget(latLabel_);
                        layout->removeWidget(lonLabel_);
                        layout->removeWidget(imageLabel_);
                        button->setText("View Target");
                    }
                    update();
                });
    }

private:
    QLabel* latLabel_;
    QLabel* lonLabel_;
    QLabel* imageLabel_;
    bool visible_ = false;
};