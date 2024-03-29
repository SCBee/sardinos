#pragma once

#include <QList>
#include <QObject>
#include <QTimer>
#include <chrono>
#include <iostream>
#include <string>

#include <LmCdl/I_VcsiMapExtensionApi.h>
#include <Target.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

class ImageProcessor : public QObject
{
    Q_OBJECT
public:
    ImageProcessor(QList<Target>& targets,
                   const volatile double& latitude,
                   const volatile double& longitude,
                   const volatile double& altitude);

    void init(const std::string& uri);
    void stop();

private:
    const volatile double& latitude_;
    const volatile double& longitude_;
    const volatile double& altitude_;

    bool processing_ = false;

    QList<Target>& targets_;

    std::chrono::system_clock::time_point lastProcess_;

    cv::Mat* currentFrame_ = new cv::Mat();

    void addTarget(cv::Mat mat, cv::Rect boundingRect);

    QGeoCoordinate calcLocation(cv::Mat mat, cv::Rect boundingrect);

    void processFrame(const cv::Mat& frame);
};