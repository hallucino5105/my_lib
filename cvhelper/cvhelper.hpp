#pragma once


#include <cstdio>
#include <exception>
#include <utility>
#include <string>
#include <typeinfo>
#include <boost/filesystem.hpp>
#include <boost/filesystem/path.hpp>
#include <opencv2/opencv.hpp>
#include "imagemat.hpp"


namespace hoshi
{


class CvHelper
{
#define DEFAULT_WINDOW_TITLE "CV"


public:
    typedef cv::Mat base_type;
    typedef const cv::Mat const_base_type;


public:
    CvHelper();
    ~CvHelper();


private:
    template<typename T>
    static T openImageImpl(
            const std::string& path,
            uint32_t arg);


public:
    template<typename T = base_type>
    static T openImage(
            const std::string& path);


    static void displayImage(
            const_base_type& image,
            const std::string& title = DEFAULT_WINDOW_TITLE);


    static void createWindow(
            const std::string& title = DEFAULT_WINDOW_TITLE);


    static void showImage(
            const_base_type& image,
            const std::string& title = DEFAULT_WINDOW_TITLE);


    static void closeWindow(
            const std::string& title = DEFAULT_WINDOW_TITLE);


    static void convertGrayscale(
            base_type& image);


#undef DEFAULT_WINDOW_TITLE
};


};

