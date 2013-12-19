// hehehe

#ifndef GOOGLE_MAPS_COORD_TRANSFER_H
#define GOOGLE_MAPS_COORD_TRANSFER_H


#include <cstdarg>
#include <iostream>
#include <string>
#include <tuple>
#include <memory>


namespace hoshi
{


using namespace std;


class coord_pixel_transfer
{
private:
    const int ZOOM_LEVEL_MIN;
    const int ZOOM_LEVEL_MAX;
    const int TILE_WIDTH;
    const int TILE_HEIGHT;
    const double LAT_MIN;
    const double LAT_MAX;
    const double LNG_MIN;
    const double LNG_MAX;
    const double PI;
    const double R;


public:
    coord_pixel_transfer();
    ~coord_pixel_transfer();

public:
    tuple<double, double> fromLatLngToWorldCoord(double lat, double lng);
    tuple<double, double> fromWorldCoordToLatLng(double wx, double wy);

    // ズームレベルを考慮した画面上のピクセル座標を返す
    tuple<unsigned int, unsigned int> fromLatLngToPixelCoord(double lat, double lng, double zoomLevel);
    tuple<double, double> fromPixelCoordToLatLng(unsigned int px, unsigned int py, double zoomLevel);

    // タイル番号を返す
    tuple<double, double, unsigned int, unsigned int> fromLatLngToTileCoord(double lat, double lng, double zoomLevel);
    // タイルの左上隅(0, 0)を返す
    tuple<double, double> fromTileCoordToLatLng(unsigned int tx, unsigned int ty, double zoomLevel);

    tuple<double, double> zoomLevelRange();
};


}


#endif
