#!/usr/bin/env python
# coding: utf-8


#import sys
#import os
import math
from decimal import Decimal


"""

google mapsのパクリ

lat -90 ~ +90
lng -180 ~ +180
をz(oom) levelに応じてメッシュ状に分割する。

latlngをそれぞれ
lat -90 ~ +90 -> (+90) -> (*2) -> 0 ~ 360
lng -180 ~ +180 -> (+180) -> 0 ~ 360
として0~360度区間にマップする。

マップした緯度経度を 360.0/(2**(zlevel)) で割る
商の整数部分が座標のメッシュ番号となる。

メッシュ番号は
mapperd lat, lng: 0, 0
 > lat, lng: -90, -180
を原点とする。

zlevelの範囲と書式表現は下記の通り。

zlevel: 0 <= z <= 19
format: [zlevel],[lat mesh position],[lng mesh position]


（例）33.49, 137.12 z=5

33.49, 137.12
 > (33.49 + 90) * 2, (137.12 + 180)
 = 236.98, 317.12
 > floor(236.98/(360.0/2**5)), floor(317.12/(360.0/2**5))
 = 21, 28

5,21,28
(zlevel = 5, mesh x = 21, mesh y = 28)


DBに格納するメッシュデータについて
zlevelに応じて1次〜5次メッシュを定義する。
次元は必要に応じて増減可能。

赤道上での1度の大きさを
2*pi*6378150)/360 [m/度]
とする。

各次元のメッシュ長は次のとおりとなる。
(2*pi*6378150)/360) * (360/2**zlevel) [m]

m1 = z5  1252.3km mesh (1000kmメッシュの近似)
m2 = z8  156.54km mesh (100km)
m3 = z12 9.7840km mesh (10km)
m4 = z13 4.8920km mesh (5km)
m5 = z15 1.2230km mesh (1km)
m6 = z19 0.0764km mesh (0.1km)

書式表現は下記の通り。

format:
[m1 lat pos]-[m1 lng pos],
[m2 lat pos]-[m2 lng pos],
[m3 lat pos]-[m3 lng pos],
[m4 lat pos]-[m4 lng pos],
[m5 lat pos]-[m5 lng pos],
[m5 lat pos]-[m5 lng pos]
（一行で）

（例）35.46, 49.38

m:  z: mesh position
1:  5: 22 20
2:  8: 178 163
3: 12: 2854 2609
4: 13: 5709 5219
5: 15: 22839 20878
6: 19: 365428 334058

22-20,178-163,2854-2609,5709-5219,22839-20878,365428-334058

文字列として表現する場合は、各次元の取りうる最大値が
32-32,256-256,4096-4096,8192-8192,32768-32768,524288-524288
となるので、59文字分の長さがあれば良い。

"""

class WorldMesh:
    # 任意桁の小数を扱うためlat,lngは文字列で渡す
    def __init__(self, lat, lng):
        if type(lat) is not str:
            raise TypeError("Invalid literal for __init__: lat")

        if type(lng) is not str:
            raise TypeError("Invalid literal for __init__: lng")

        self.__original_lat = Decimal(lat)
        self.__original_lng = Decimal(lng)

        if not -90 <= self.__original_lat <= 90 \
                or not -180 <= self.__original_lng <= 180:
            raise ValueError("LatLng out of range")

        self.__mappered_lat = (self.__original_lat + 90) * 2
        self.__mappered_lng = self.__original_lng + 180


    # zlevelを伴うメッシュコードの文字列表現
    def GetMesh1String(self, zlevel):
        m = self.__CalcMeshIndexFromZ(zlevel, self.__mappered_lat, self.__mappered_lng)
        return "%d-%d-%d" % (m["z"], m["ilat"], m["ilng"])


    # インデックス番号を返す
    def GetMesh1Index(self, zlevel):
        m = self.__CalcMeshIndexFromZ(zlevel, self.__mappered_lat, self.__mappered_lng)
        return (m["ilat"], m["ilng"])


    # 1次〜6次メッシュの文字列表現
    def GetMesh2String(self):
        m1 = self.__CalcMeshIndexFromZ( 5, self.__mappered_lat, self.__mappered_lng)
        m2 = self.__CalcMeshIndexFromZ( 8, self.__mappered_lat, self.__mappered_lng)
        m3 = self.__CalcMeshIndexFromZ(12, self.__mappered_lat, self.__mappered_lng)
        m4 = self.__CalcMeshIndexFromZ(13, self.__mappered_lat, self.__mappered_lng)
        m5 = self.__CalcMeshIndexFromZ(15, self.__mappered_lat, self.__mappered_lng)
        m6 = self.__CalcMeshIndexFromZ(19, self.__mappered_lat, self.__mappered_lng)

        return "%d-%d,%d-%d,%d-%d,%d-%d,%d-%d,%d-%d" % (
                m1["ilat"], m1["ilng"],
                m2["ilat"], m2["ilng"],
                m3["ilat"], m3["ilng"],
                m4["ilat"], m4["ilng"],
                m5["ilat"], m5["ilng"],
                m6["ilat"], m6["ilng"],
                )


    def __CalcMeshIndexFromZ(self, zlevel, lat, lng):
        if type(zlevel) is not int:
            raise TypeError("Invalid literal for GetMeshString1: zlevel")

        if not 0 <= zlevel <= 19:
            raise ValueError("Z Level out of range")

        lat_index = int(math.floor(lat / (Decimal("360.0") / 2 ** zlevel)))
        lng_index = int(math.floor(lng / (Decimal("360.0") / 2 ** zlevel)))

        return {
                "z": zlevel,
                "ilat": lat_index,
                "ilng": lng_index,
                }

