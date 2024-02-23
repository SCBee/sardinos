#include <QGeoCoordinate>
#include <QLabel>
#include <QPixmap>
#include <QVBoxLayout>

#include "ImageProcessor.h"
#include <TargetWidget.h>

#include <LmCdl/I_Billboard.h>
#include <MathExt.h>

ImageProcessor::ImageProcessor(const volatile double& latitude,
                               const volatile double& longitude)
    : latitude_(latitude)
    , longitude_(longitude)
    , processingTimer_(new QTimer)
{
    processingTimer_->setInterval(1000);

    connect(processingTimer_,
            &QTimer::timeout,
            this,
            [this]() { this->processFrame(*currentFrame_); });
}

void ImageProcessor::init(std::string uri)
{
    processingTimer_->start();

    // Initialize VideoCapture with the GStreamer pipeline
    cv::VideoCapture cap(uri, cv::CAP_GSTREAMER);
    if (!cap.isOpened()) {
        std::cerr << "Failed to open video stream." << std::endl;
        return;
    }

    cv::namedWindow("Video Stream", cv::WINDOW_AUTOSIZE);

    // Main loop to read and display frames
    std::cout << "Streaming inside of VCSi..." << std::endl;
    while (true) {
        if (!cap.read(*currentFrame_))
        {  // Read a new frame from the video stream
            std::cerr << "Failed to read frame from the stream." << std::endl;
            break;
        }

        cv::imshow("[INTERNAL] VCSi Video Stream",
                   *currentFrame_);  // Display the frame

        // Break the loop when 'ESC' is pressed
        if (cv::waitKey(1) == 27) {
            break;
        }
    }

    // Cleanup
    cap.release();
    cv::destroyAllWindows();
    processingTimer_->stop();
}

void ImageProcessor::processFrame(cv::Mat frame)
{
    cv::Mat grayImage;

    cv::cvtColor(frame, grayImage, cv::COLOR_RGB2GRAY);

    // Threshold the image to create a binary mask of 0-intensity pixels
    cv::Mat mask;
    cv::threshold(grayImage, mask, 119, 255, cv::THRESH_BINARY_INV);

    // Find contours of the 0-intensity pixels
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(
        mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    if (contours.size() == 0) {
        // std::cerr << "RETURNING\n";
        return;
    }
    
    // Find the largest contour
    size_t largestContourIndex = 0;
    double largestContourArea  = 0.0;
    for (size_t i = 0; i < contours.size(); i++) {
        double area = cv::contourArea(contours[i]);
        if (area > largestContourArea) {
            largestContourArea  = area;
            largestContourIndex = i;
        }
    }

    // Get the bounding rectangle of the largest contour
    cv::Rect boundingRect = cv::boundingRect(contours[largestContourIndex]);

    // Increase the size of the rectangle by 20 pixels in height and width
    boundingRect.x -= 20;
    boundingRect.y -= 20;
    boundingRect.width += 40;  // 20 pixels added to each side (left and right)
    boundingRect.height += 40;  // 20 pixels added to each side (top and bottom)

    // Ensure the rectangle does not go out of bounds
    boundingRect &= cv::Rect(0, 0, frame.cols, frame.rows);

    cv::Mat targetFoundMat = frame.clone();
    // Draw the rectangle around the largest contour on the original image
    cv::rectangle(targetFoundMat, boundingRect, cv::Scalar(255, 255, 0), 2);

    cv::cvtColor(targetFoundMat, targetFoundMat, cv::COLOR_BGR2RGB);

    sendSignal(targetFoundMat);
}

void ImageProcessor::sendSignal(cv::Mat mat){

    QImage image((uchar*) mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);

    TargetWidget* widget = new TargetWidget(latitude_, longitude_, image);

    Target target = {QGeoCoordinate(latitude_, longitude_, 1300), widget, image};

    //std::cerr << "EMITTING SIGNAL\n";

    emit targetFound(target);
}