#pragma once

#include <QObject>
#include <string>

//#include <opencv2/imgcodecs.hpp>
//#include <opencv2/videoio.hpp>

class ImageProcessor : public QObject
{
    Q_OBJECT
public:
    void init(const std::string& uri);

signals:
    void targetFound(/*cv::Mat mat*/);
};