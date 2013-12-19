// huhuhu

#include <cstdio>
#include <cstdarg>
#include <cmath>
#include <tuple>
#include <stdexcept>
#include <str2num.h>
#include "coord_pixel_transfer.h"


namespace hoshi
{


using namespace std;
using namespace boost;



coord_pixel_transfer::coord_pixel_transfer() :
    ZOOM_LEVEL_MIN(0), 
    ZOOM_LEVEL_MAX(19), 
    TILE_WIDTH(256), 
    TILE_HEIGHT(256), 
    // LAT_MIN(str2num<double>("%.16f", (atan(sinh(((128 - (256 -   0)) * M_PI) / 128))) * 180/M_PI)), 
    // LAT_MAX(str2num<double>("%.16f", (atan(sinh(((128 - (256 - 256)) * M_PI) / 128))) * 180/M_PI)), 
    // LNG_MIN(str2num<double>("%.16f", (((  0 * M_PI) / 128) - M_PI) * 180/M_PI)), 
    // LNG_MAX(str2num<double>("%.16f", (((256 * M_PI) / 128) - M_PI) * 180/M_PI)),
    LAT_MIN((atan(sinh(((128 - (256 -   0)) * M_PI) / 128))) * 180/M_PI), 
    LAT_MAX((atan(sinh(((128 - (256 - 256)) * M_PI) / 128))) * 180/M_PI), 
    LNG_MIN((((  0 * M_PI) / 128) - M_PI) * 180/M_PI), 
    LNG_MAX((((256 * M_PI) / 128) - M_PI) * 180/M_PI),
    PI(M_PI), 
    R(128/M_PI)
{
}


coord_pixel_transfer::~coord_pixel_transfer()
{
}


tuple<double, double> coord_pixel_transfer::fromLatLngToWorldCoord(double lat, double lng)
{
    // lat = str2num<double>("%.16f", lat);
    // lng = str2num<double>("%.16f", lng);

    if((lat < LAT_MIN || lat > LAT_MAX) || (lng < LNG_MIN || lng > LNG_MAX)) {
        throw new out_of_range("out of range latlng");
    }

    double lat_rad = lat * PI/180;
    double lng_rad = lng * PI/180;

    return make_tuple(
        // str2num<double>("%.16f", R * (lng_rad + PI)), 
        // str2num<double>("%.16f", -R/2 * log((1 + sin(lat_rad)) / (1 - sin(lat_rad))) + 128));
        R * (lng_rad + PI), -R/2 * log((1 + sin(lat_rad)) / (1 - sin(lat_rad))) + 128);
}


tuple<double, double> coord_pixel_transfer::fromWorldCoordToLatLng(double wx, double wy)
{
    if((wx < 0 || wx > TILE_WIDTH) || (wy < 0 || wy > TILE_HEIGHT)) {
        throw new out_of_range("out of range world coordinate");
    }

    return make_tuple(
        // str2num<double>("%.16f", (atan(sinh(((128 - wy) * PI) / 128))) * 180/PI), 
        // str2num<double>("%.16f", (((wx * PI) / 128) - PI) * 180/PI));
        (atan(sinh(((128 - wy) * PI) / 128))) * 180/PI, (((wx * PI) / 128) - PI) * 180/PI);

}

tuple<unsigned int, unsigned int> coord_pixel_transfer::fromLatLngToPixelCoord(double lat, double lng, double zoomLevel)
{
    if(zoomLevel < ZOOM_LEVEL_MIN || zoomLevel > ZOOM_LEVEL_MAX) {
        throw new out_of_range("out of range zoom level");
    }

    tuple<double, double> wc;

    try {
        wc = this->fromLatLngToWorldCoord(lat, lng);
    } catch(out_of_range&) {
        throw;
    }

    return make_tuple(
        floor(get<0>(wc) * pow(2, zoomLevel)),
        floor(get<1>(wc) * pow(2, zoomLevel)));
}


tuple<double, double> coord_pixel_transfer::fromPixelCoordToLatLng(unsigned int px, unsigned int py, double zoomLevel)
{
    if(zoomLevel < ZOOM_LEVEL_MIN || zoomLevel > ZOOM_LEVEL_MAX) {
        throw new out_of_range("out of range zoom level");
    }

    // double wx = str2num<double>("%.16f", px / pow(2, zoomLevel));
    // double wy = str2num<double>("%.16f", py / pow(2, zoomLevel));
    double wx = px / pow(2, zoomLevel);
    double wy = py / pow(2, zoomLevel);

    tuple<double, double> latlng;

    try {
        latlng = this->fromWorldCoordToLatLng(wx, wy);
    } catch(out_of_range) {
        throw;
    }

    return latlng;
}


tuple<double, double, unsigned int, unsigned int>
coord_pixel_transfer::fromLatLngToTileCoord(double lat, double lng, double zoomLevel)
{
    tuple<unsigned int, unsigned int> pc;

    try {
        pc = this->fromLatLngToPixelCoord(lat, lng, zoomLevel);
    } catch(out_of_range&) {
        throw;
    }

    return make_tuple(
        // str2num<double>("%u", floor(get<0>(pc) / TILE_WIDTH)), 
        // str2num<double>("%u", floor(get<1>(pc) / TILE_HEIGHT)),
        // // タイル内のピクセル座標
        // str2num<unsigned int>("%.16f", get<0>(pc) % TILE_WIDTH),
        // str2num<unsigned int>("%.16f", get<1>(pc) % TILE_HEIGHT));
        floor(get<0>(pc) / TILE_WIDTH), 
        floor(get<1>(pc) / TILE_HEIGHT),
        // タイル内のピクセル座標
        get<0>(pc) % TILE_WIDTH,
        get<1>(pc) % TILE_HEIGHT);
}


tuple<double, double> coord_pixel_transfer::fromTileCoordToLatLng(unsigned int tx, unsigned int ty, double zoomLevel)
{
    unsigned int px = tx * TILE_WIDTH;
    unsigned int py = tx * TILE_HEIGHT;

    tuple<double, double> latlng;

    try {
        latlng = fromPixelCoordToLatLng(px, py, zoomLevel);
    } catch(out_of_range&) {
        throw;
    }

    return latlng;
}


tuple<double, double> coord_pixel_transfer::zoomLevelRange()
{
    return make_tuple(ZOOM_LEVEL_MIN, ZOOM_LEVEL_MAX);
}


}

