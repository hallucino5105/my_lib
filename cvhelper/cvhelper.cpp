#include "cvhelper.hpp"


namespace hoshi
{


template<typename T>
T CvHelper::openImageImpl(
        const std::string& path,
        uint32_t arg)
{
    boost::filesystem::path tmp_path(path);

    if(!boost::filesystem::exists(tmp_path)) {
        throw std::runtime_error("");
    }

    T image = cv::imread(path.c_str(), arg);

    assert(image.rows > 0 && image.cols > 0);

    return image;
}


template<>
CvHelper::base_type CvHelper::openImage<CvHelper::base_type>(
        const std::string& path)
{
    return CvHelper::openImageImpl<CvHelper::base_type>(path, 1);
}


template<>
ImageMat CvHelper::openImage<ImageMat>(
        const std::string& path)
{
    return CvHelper::openImageImpl<ImageMat>(path, 1);
}


template<>
ImageMat3c CvHelper::openImage<ImageMat3c>(
        const std::string& path)
{
    return CvHelper::openImageImpl<ImageMat3c>(path, 1);
}


template<>
ImageMat1c CvHelper::openImage<ImageMat1c>(
        const std::string& path)
{
    return CvHelper::openImageImpl<ImageMat1c>(path, 0);
}


template<>
ImageMat3f CvHelper::openImage<ImageMat3f>(
        const std::string& path)
{
    ImageMat3c image  = CvHelper::openImageImpl<ImageMat3f>(path, 1);
    return ImageMatUtil::convertToSingleFloatUnit<ImageMat3c, ImageMat3f>(image);
}


template<>
ImageMat1f CvHelper::openImage<ImageMat1f>(
        const std::string& path)
{
    ImageMat1c image = CvHelper::openImageImpl<ImageMat1f>(path, 0);
    return ImageMatUtil::convertToSingleFloatUnit<ImageMat1c, ImageMat1f>(image);
}


void CvHelper::displayImage(
        CvHelper::const_base_type& image,
        const std::string& title)
{
    cv::namedWindow(title, CV_WINDOW_AUTOSIZE);
    cv::imshow(title, image);
    cv::waitKey(0);
}


void CvHelper::createWindow(
        const std::string& title)
{
    cv::namedWindow(title, CV_WINDOW_AUTOSIZE);
}


void CvHelper::showImage(
        CvHelper::const_base_type& image,
        const std::string& title)
{
    cv::imshow(title, image);
}


void CvHelper::closeWindow(
        const std::string& title)
{
    cv::destroyWindow(title);
}


void CvHelper::convertGrayscale(
        CvHelper::base_type& image)
{
    cv::cvtColor(image, image, CV_RGB2GRAY);
}


};

