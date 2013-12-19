#!/usr/bin/python
# coding: utf-8



from math import *



class CoordPixelTransfer:
    def __init__(self):
        self.r              = 128/pi
        self.rad            = pi/180
        self.zoom_level_min = 0
        self.zoom_level_max = 20
        self.tile_width     = 256
        self.tile_height    = 256
        self.lat_min        = float("%.8f" % float(atan(sinh(((128 - (256 -   0)) * pi) / 128)) * 180/pi))
        self.lat_max        = float("%.8f" % float(atan(sinh(((128 - (256 - 256)) * pi) / 128)) * 180/pi))
        self.lng_min        = float("%.8f" % float((((  0 * pi) / 128) - pi) * 180/pi))
        self.lng_max        = float("%.8f" % float((((256 * pi) / 128) - pi) * 180/pi))


    def decimalFormat(self, num):
        return float("%.8f" % num)


    def checkLatlngRange(self, latlng):
        lat, lng = self.decimalFormat(latlng[0]), self.decimalFormat(latlng[1])
        if (not self.lat_min < lat < self.lat_max) or (not self.lng_min < lng < self.lng_max): 
            raise ValueError("latlng out of range")


    def checkTilePixelRange(self, p):
        px, py = p[0], p[1]
        if (not 0 < px < self.tile_width) or (not 0 < py <  self.tile_height):
            raise ValueError("world coordinate out of range world")
        

    def checkZoomRange(self, zoom):
        if not self.zoom_level_min < zoom < self.zoom_level_max:
            raise ValueError("zoom out of range")


    def fromLatLngToWorldCoord(self, latlng):
        self.checkLatlngRange(latlng)

        lat, lng = self.decimalFormat(latlng[0]), self.decimalFormat(latlng[1])
        lat_rad, lng_rad = lat * self.rad, lng * self.rad

        return (
            self.decimalFormat( self.r * (lng_rad + pi) ),
            self.decimalFormat( -self.r/2 * log((1 + sin(lat_rad)) / (1 - sin(lat_rad))) + 128 ))

    def fromLatLngToWorldCoordRatio(self, latlng):
        x, y = self.fromLatLngToWorldCoord(latlng)

        return ( x / float(self.tile_width), y / float(self.tile_height) )


    def fromWorldCoordToLatLng(self, p):
        self.checkTilePixelRange(p)
        
        px, py = self.decimalFormat(p[0]), self.decimalFormat(p[1])

        return (
            self.decimalFormat( atan(sinh(((128 - py) * pi) / 128)) * 180/pi ),
            self.decimalFormat( (((px * pi) / 128) - pi) * 180/pi ))


    def fromLatLngToPixelCoord(self, latlng, zoom):
        self.checkZoomRange(zoom)

        px, py = self.fromLatLngToWorldCoord(latlng)

        return ( px * (2 ** zoom), py * (2** zoom) )


    def fromPixelCoordToLatLng(self, p, zoom):
        self.checkZoomRange(zoom)

        px, py = (
            self.decimalFormat(p[0] / (2 ** zoom)), 
            self.decimalFormat(p[1] / (2 ** zoom)))

        return self.fromWorldCoordToLatLng((px, py))


    def fromLatLngToTileCoord(self, latlng, zoom):
        px, py = self.fromLatLngToPixelCoord(latlng, zoom)

        return (
            (int("%u" % floor(px / self.tile_width)), int("%u" % floor(py / self.tile_height))),
            (self.decimalFormat(px % self.tile_width), self.decimalFormat(py % self.tile_height)))


    def fromTileCoordToLatLng(self, p, zoom):
        px, py = p[0] * self.tile_width, p[1] * self.tile_height

        return self.fromPixelCoordToLatLng((px, py), zoom)


    def zoomRange(self):
        return (self.zoom_level_min, self.zoom_level_max)


    def test(self):
        print "lat_min", self.lat_min
        print "lat_max", self.lat_max
        print "lng_min", self.lng_min
        print "lng_max", self.lng_max

        latlng = (34.1234, 134.1234)

        print self.fromWorldCoordToLatLng((2, 23))
        print self.fromLatLngToWorldCoord(latlng)
        print self.fromLatLngToPixelCoord(latlng, 7)
        print self.fromLatLngToTileCoord(latlng, 7)

