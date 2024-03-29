#include <QGeoCoordinate>
#include <QPointer>
#include <algorithm>
#include <utility>

#include "ImageProcessor.h"

#include <qmath.h>

#include "Helpers/Sardinos.h"
#include "TargetWidget.h"

const double DFOV = 72.64;
const double HFOV = 57.12;
const double VFOV = 42.44;

ImageProcessor::ImageProcessor(QList<Target>& targets,
                               DroneTelemetry* droneTelemetry_)
    : targets_(targets)
    , droneTelemetry(droneTelemetry_)
{
}

void ImageProcessor::init(const std::string& uri)
{
    processing_ = true;

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

        if (processing_)
            processFrame(*currentFrame_);

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
}

void ImageProcessor::processFrame(const cv::Mat& frame)
{
    auto currentTime = std::chrono::system_clock::now();

    if ((currentTime - lastProcess_) < std::chrono::seconds(1))
        return;

    lastProcess_ = currentTime;

    cv::Mat hsv;

    cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);

    cv::Scalar lowerRed = cv::Scalar(0, 100, 100);

    cv::Scalar upperRed = cv::Scalar(10, 255, 255);

    cv::Mat mask;

    cv::inRange(hsv, lowerRed, upperRed, mask);

    // Find contours of the 0-intensity pixels
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    if (contours.empty()) {
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

    addTarget(targetFoundMat, boundingRect);
}

void ImageProcessor::addTarget(cv::Mat mat, cv::Rect boundingRect)
{
    auto location = calcLocation(mat, boundingRect);

    auto target = Target(location, std::move(mat));

    targets_.append(target);
    emit droneTelemetry->targetFound();
}

QGeoCoordinate ImageProcessor::calcLocation(const cv::Mat& mat,
                                            cv::Rect boundingRect)
{
    auto pixelWidth  = (double)mat.cols;
    auto pixelHeight = (double)mat.rows;

    auto midRectX = (double)boundingRect.width / 2 + boundingRect.x;
    auto midRectY = (double)boundingRect.height / 2 + boundingRect.y;

    auto xRatio =
        (std::min(std::max(midRectX, 0.0), pixelWidth) / pixelWidth) * 2 - 1;
    auto yRatio =
        (std::min(std::max(midRectY, 0.0), pixelHeight) / pixelHeight) * 2 - 1;

    auto widthMeters =
        (droneTelemetry->altitude() / tan((VFOV * (M_PI / 180))));
    auto heightMeters =
        (droneTelemetry->altitude() / tan((HFOV * (M_PI / 180))));

    auto widthChange  = xRatio * widthMeters;
    auto heightChange = -yRatio * heightMeters;

    auto angle = atan2(widthChange, heightChange) * (180 / M_PI);

    auto distance = sqrt(pow(widthChange, 2) + pow(heightChange, 2));

    return sardinos::getLocation(droneTelemetry->latitude(),
                                 droneTelemetry->longitude(),
                                 droneTelemetry->altitude(),
                                 distance,
                                 angle);
}

void ImageProcessor::stop()
{
    processing_ = false;
}