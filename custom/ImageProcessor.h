#pragma once

#include <QObject>
#include <string>
#include <iostream>
#include <opencv2/opencv.hpp>

class ImageProcessor : public QObject
{
   Q_OBJECT
public:
   void init(std::string uri);
   void processFrame(cv::Mat frame);

signals:
   void targetFound(cv::Mat mat);
};