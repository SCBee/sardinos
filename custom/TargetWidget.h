#pragma once

#include <QApplication>
#include <QGeoCoordinate>
#include <QLabel>
#include <QPainter>
#include <QPushButton>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>
#include <iostream>

#include <opencv2/opencv.hpp>

class TargetWidget : public QWidget
{
public:
    TargetWidget(const double lat, const double lon, const cv::Mat& mat)
    {
        // Create a QVBoxLayout to contain the button
        auto* layout = new QVBoxLayout(this);

        // Create a QPushButton
        auto button = new QPushButton("View Target");

        layout->addWidget(button);

        latLabel_   = new QLabel("Latitude: " + QString::number(lat));
        lonLabel_   = new QLabel("Longitude: " + QString::number(lon));
        imageLabel_ = new QLabel();

        auto image = QImage((uchar*)mat.data,
                            mat.cols,
                            mat.rows,
                            static_cast<int>(mat.step),
                            QImage::Format_RGB888);

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

protected:
    void paintEvent(QPaintEvent* event) override
    {
        Q_UNUSED(event);
        QPainter painter(this);
        painter.setBrush(Qt::red);
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(rect().topLeft(), 5, 5);
    }

private:
    QLabel* latLabel_;
    QLabel* lonLabel_;
    QLabel* imageLabel_;
    bool visible_ = false;
};