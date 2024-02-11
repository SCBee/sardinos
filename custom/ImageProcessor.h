#pragma once

#include <xstring>
#include <QObject>
#include <opencv2/videoio.hpp>
#include <opencv2/imgcodecs.hpp>

class ImageProcessor : public QObject
{
    Q_OBJECT
public:
    void init(const std::string& uri);

signals:
    void targetFound(/*cv::Mat mat*/);
};