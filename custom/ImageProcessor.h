#pragma once

//#include <QObject>
#include <string>
#include <iostream>
#include <opencv2/opencv.hpp>

namespace sardinos {
    inline void test() {
        std::cout << "hi\n";
        // Path to the input image
        std::string imagePath = R"(C:\Users\dev\Pictures\Screenshots\1.png)";

        // Load the image from the disk
        cv::Mat image = cv::imread(imagePath, cv::IMREAD_COLOR);

        // Check for failure
        if (image.empty())
        {
            std::cout << "Could not open or find the image" << std::endl;
            return;
        }

        std::string windowName = "The Bitch"; //Name of the window

        cv::namedWindow(windowName); // Create a window

        imshow(windowName, image); // Show our image inside the created window.

        cv::waitKey(0); // Wait for any keystroke in the window

        cv::destroyWindow(windowName); //destroy the created window
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