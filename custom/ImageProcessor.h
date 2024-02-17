#pragma once

//#include <QObject>
#include <string>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>


namespace sardinos {
    inline void test() {
        // GStreamer pipeline string adapted for OpenCV
        std::string pipeline = "udpsrc port=5200 caps = \"application/x-rtp, media=(string)video, clock-rate=(int)90000, encoding-name=(string)H264, payload=(int)96\" ! rtph264depay ! decodebin ! videoconvert ! appsink";

        // Initialize VideoCapture with the GStreamer pipeline
        cv::VideoCapture cap(pipeline, cv::CAP_GSTREAMER);
        if (!cap.isOpened()) {
            std::cerr << "Failed to open video stream." << std::endl;
            return;
        }

        cv::Mat frame;
        cv::namedWindow("Video Stream", cv::WINDOW_AUTOSIZE);

        // Main loop to read and display frames
        std::cout << "Streaming inside of VCSi..." << std::endl;
        while (true) {
            if (!cap.read(frame)) { // Read a new frame from the video stream
                std::cerr << "Failed to read frame from the stream." << std::endl;
                break;
            }

            cv::imshow("[INTERNAL] VCSi Video Stream", frame); // Display the frame

            // Break the loop when 'ESC' is pressed
            if (cv::waitKey(1) == 27) {
                break;
            }
        }

        // Cleanup
        cap.release();
        cv::destroyAllWindows();
    }
}

//class ImageProcessor : public QObject
//{
//    Q_OBJECT
//public:
//    void init();
//
//signals:
//    void targetFound(/*cv::Mat mat*/);
//};