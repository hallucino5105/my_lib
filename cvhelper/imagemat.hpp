#pragma once


#include <cstdio>
#include <iostream>
#include <exception>
#include <utility>
#include <string>
#include <type_traits>
#include <boost/filesystem.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/any.hpp>
#include <opencv2/opencv.hpp>

#include <boost/type_traits.hpp>
#include <boost/utility/enable_if.hpp>


namespace hoshi
{


extern void* enabler;


class empty_
{
    explicit empty_();
};


template<typename T>
class ImageMat_ : public cv::Mat
{
public:
    typedef cv::Mat base_type;
    typedef const cv::Mat const_base_type;
    typedef T value_type;


public:
    ImageMat_<value_type>() :
        base_type()
    {
    }


    ImageMat_<value_type>(int rows, int cols, int type) :
        base_type(rows, cols, type)
    {
        initialize();
    }


    template<
        typename U = value_type,
        typename std::enable_if<std::is_same<U, cv::Vec3b>::value>::type*& = enabler>
    ImageMat_<value_type>(int rows, int cols, int type = CV_8UC3) :
        base_type(rows, cols, type)
    {
        initialize();
    }


    template<
        typename U = value_type,
        typename std::enable_if<std::is_same<U, uint8_t>::value>::type*& = enabler>
    ImageMat_<value_type>(int rows, int cols, int type = CV_8UC1) :
        base_type(rows, cols, type)
    {
        initialize();
    }


    template<
        typename U = value_type,
        typename std::enable_if<std::is_same<U, cv::Vec3f>::value>::type*& = enabler>
    ImageMat_<value_type>(int rows, int cols, int type = CV_32FC3) :
        base_type(rows, cols, type)
    {
        initialize();
    }


    template<
        typename U = value_type,
        typename std::enable_if<std::is_same<U, float>::value>::type*& = enabler>
    ImageMat_<value_type>(int rows, int cols, int type = CV_32FC1) :
        base_type(rows, cols, type)
    {
        initialize();
    }


    ImageMat_<value_type>(const_base_type& mat) :
        base_type(mat)
    {
        initialize();
    }


    // 落ちる なぜ
    //ImageMat_<T>& operator=(const_base_type& mat)
    //{
    //    *this = mat;
    //    return *this;
    //}


    ImageMat_<value_type>(base_type&& mat) : 
        base_type(mat)
    {
    }


    //ImageMat_<T>& operator=(base_type&& mat)
    //{
    //    *this = mat;
    //    return *this;
    //}


    ~ImageMat_<value_type>()
    {
    }


    void initialize()
    {
        assert(this->channels() == 1 || this->channels() == 3);
    }


protected:
    template<typename U>
    U& proxy(int j, int i)
    {
        static_assert(std::is_same<U, empty_>::value == false, "AAA");
        return this->at<U>(j, i);
    }


public:
    template<typename U = value_type>
    U& at2(int j, int i)
    {
        return proxy<U>(j, i);
    }


    template<typename U = value_type>
    const U& at2(int j, int i) const
    {
        return proxy<U>(j, i);
    }


    template<typename U = value_type>
    U& operator()(int j, int i)
    {
        return proxy<U>(j, i);
    }


    template<typename U = value_type>
    const U& operator()(int j, int i) const
    {
        return proxy<U>(j, i);
    }


    void convertToGrayscale()
    {
        if(this->channels() != 3) {
            return;
        }

        cv::cvtColor(*this, *this, CV_RGB2GRAY);
    }


    void print()
    {
        std::cout << cv::format(*this, "Python") << std::endl;
    }
};


typedef ImageMat_<empty_>    ImageMat;
typedef ImageMat_<cv::Vec3b> ImageMat3c;
typedef ImageMat_<cv::Vec3f> ImageMat3f;
typedef ImageMat_<uint8_t>   ImageMat1c;
typedef ImageMat_<float>     ImageMat1f;


namespace ImageMatUtil
{


template<typename T, typename U>
static U convertToCharIntUnit(const T& mat);


template<>
ImageMat3c convertToCharIntUnit<ImageMat3f, ImageMat3c>(const ImageMat3f& mat)
{
    ImageMat3c tmp;
    mat.convertTo(tmp, CV_8U, 255.0);
    return tmp;
}


template<>
ImageMat1c convertToCharIntUnit<ImageMat1f, ImageMat1c>(const ImageMat1f& mat)
{
    ImageMat1c tmp;
    mat.convertTo(tmp, CV_8U, 255.0);
    return tmp;
}


template<typename T, typename U>
static U convertToSingleFloatUnit(const T& mat);


template<>
ImageMat3f convertToSingleFloatUnit<ImageMat3c, ImageMat3f>(const ImageMat3c& mat)
{
    ImageMat3f tmp;
    mat.convertTo(tmp, CV_32F, 1.0/255.0);
    return tmp;
}


template<>
ImageMat1f convertToSingleFloatUnit<ImageMat1c, ImageMat1f>(const ImageMat1c& mat)
{
    ImageMat1f tmp;
    mat.convertTo(tmp, CV_32F, 1.0/255.0);
    return tmp;
}


};


};

