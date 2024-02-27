#pragma once

#include <QList>
#include <QObject>
#include <QTimer>
#include <chrono>
#include <iostream>
#include <string>

#include <LmCdl/I_VcsiMapExtensionApi.h>
#include <opencv2/opencv.hpp>

#include "Drone/DroneTelemetry.h"
#include "Target.h"

class ImageProcessor : public QObject
{
    Q_OBJECT
public:
    ImageProcessor(QList<Target>& targets, std::unique_ptr<DroneTelemetry>& droneTelemetry_);

    void init(const std::string& uri);
    void stop();

private:
    std::unique_ptr<DroneTelemetry>& droneTelemetry;

    bool processing_ = false;

    QList<Target>& targets_;

    std::chrono::system_clock::time_point lastProcess_;

    cv::Mat* currentFrame_ = new cv::Mat();

    void addTarget(cv::Mat mat, cv::Rect boundingRect);

    QGeoCoordinate calcLocation(const cv::Mat& mat, cv::Rect boundingrect);

    void processFrame(const cv::Mat& frame);
};