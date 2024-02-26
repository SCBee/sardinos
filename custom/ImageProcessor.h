#pragma once

#include <QList>
#include <QObject>
#include <QTimer>
#include <chrono>
#include <iostream>
#include <string>

#include <DroneTelemetry.h>
#include <LmCdl/I_VcsiMapExtensionApi.h>
#include <Target.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

class ImageProcessor : public QObject
{
    Q_OBJECT
public:
    ImageProcessor(QList<Target>& targets, DroneTelemetry* droneTelemetry);

    void init(const std::string& uri);
    void stop();

private:
    DroneTelemetry* droneTelemetry;

    bool processing_ = false;

    QList<Target>& targets_;

    std::chrono::system_clock::time_point lastProcess_;

    cv::Mat* currentFrame_ = new cv::Mat();

    void addTarget(cv::Mat mat, cv::Rect boundingRect);

    QGeoCoordinate calcLocation(cv::Mat mat, cv::Rect boundingrect);

    void processFrame(const cv::Mat& frame);
};