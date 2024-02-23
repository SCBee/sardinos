#include "ImageProcessor.h"
#include <QLabel>
#include <QPixmap>

void ImageProcessor::init(std::string uri)
{
    // Initialize VideoCapture with the GStreamer pipeline
    cv::VideoCapture cap(uri, cv::CAP_GSTREAMER);
    if (!cap.isOpened()) {
        std::cerr << "Failed to open video stream." << std::endl;
        return;
    }

    cv::Mat frame;
    cv::namedWindow("Video Stream", cv::WINDOW_AUTOSIZE);

    // Main loop to read and display frames
    std::cout << "Streaming inside of VCSi..." << std::endl;
    while (true) {
        if (!cap.read(frame)) {  // Read a new frame from the video stream
            std::cerr << "Failed to read frame from the stream." << std::endl;
            break;
        }

        processFrame(frame);

        cv::imshow("[INTERNAL] VCSi Video Stream", frame);  // Display the frame

        // Break the loop when 'ESC' is pressed
        if (cv::waitKey(1) == 27) {
            break;
        }
    }

    // Cleanup
    cap.release();
    cv::destroyAllWindows();
}

void ImageProcessor::processFrame(cv::Mat frame)
{
    cv::Mat grayImage;

    cv::cvtColor(frame, grayImage, cv::COLOR_RGB2GRAY);

    // Threshold the image to create a binary mask of 0-intensity pixels
    cv::Mat mask;
    cv::threshold(grayImage, mask, 109, 255, cv::THRESH_BINARY_INV);

    // Find contours of the 0-intensity pixels
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    if (contours.size() == 0) return;

    // Find the largest contour
    size_t largestContourIndex = 0;
    double largestContourArea = 0.0;
    for (size_t i = 0; i < contours.size(); i++) {
        double area = cv::contourArea(contours[i]);
        if (area > largestContourArea) {
            largestContourArea = area;
            largestContourIndex = i;
        }
    }

    // Get the bounding rectangle of the largest contour
    cv::Rect boundingRect = cv::boundingRect(contours[largestContourIndex]);

    // Increase the size of the rectangle by 20 pixels in height and width
    boundingRect.x -= 20;
    boundingRect.y -= 20;
    boundingRect.width += 40;  // 20 pixels added to each side (left and right)
    boundingRect.height += 40; // 20 pixels added to each side (top and bottom)

    // Ensure the rectangle does not go out of bounds
    boundingRect &= cv::Rect(0, 0, frame.cols, frame.rows);

    cv::Mat targetFoundMat = frame.clone();
    // Draw the rectangle around the largest contour on the original image
    cv::rectangle(targetFoundMat, boundingRect, cv::Scalar(255, 255, 0), 2);

    QImage image(targetFoundMat.data, targetFoundMat.cols, targetFoundMat.rows, targetFoundMat.step, QImage::Format_RGB888);

    QLabel label;
    label.setPixmap(QPixmap::fromImage(image));
    
    // Set window properties
    label.setWindowTitle("Target Found");
    label.show();
    
}