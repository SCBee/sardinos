#pragma once

#include <QList>
#include <QObject>
#include <QTimer>
#include <iostream>
#include <string>

#include <LmCdl/I_VcsiMapExtensionApi.h>
#include <Target.h>
#include <opencv2/opencv.hpp>

class ImageProcessor : public QObject
{
    Q_OBJECT
public:
    ImageProcessor(const volatile double& latitude,
                   const volatile double& longitude);
    void init(std::string uri);
    void processFrame(cv::Mat frame);

signals:
    void targetFound(Target target);

private:
    const volatile double& latitude_;
    const volatile double& longitude_;

    QTimer* processingTimer_;

    cv::Mat* currentFrame_ = new cv::Mat();

    void sendSignal(cv::Mat mat);
};