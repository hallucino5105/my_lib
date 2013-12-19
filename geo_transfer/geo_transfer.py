#!/usr/bin/python
# coding: utf-8

# 参考
# http://d.hatena.ne.jp/RobinEgg/20090419/p1
# http://homepage3.nifty.com/Nowral/02_DATUM/GeoConv.html

# 変換は誤差が出るので注意



import math



rad = math.pi / 180

# 日本測地系における赤道半径、扁平率、第一離心率
tky = 6377397.155
tire_t = 1/299.152813
#ec_t = tire_t * (2 - tire_t)
ec_t = 2*tire_t - tire_t*tire_t

# 世界測地系における赤道半径、扁平率、第一離心率
wgs = 6378137
tire_w = 1/298.257223
#ec_w = tire_w * (2 - tire_w)
ec_w = 2*tire_w - tire_w*tire_w



def llh2xyz(b, l, h, eq, ec):
    # 楕円体(b, l, h)から直行座標系(x, y, z)へ
    # Ellipoid to Rectangular Coordinate System
    b   = b * rad
    l   = l * rad
    sb  = math.sin(b)
    cb  = math.cos(b)
    rn  = eq / math.sqrt(1 - ec * sb * sb)

    x = (rn + h) * cb * math.cos(l)
    y = (rn + h) * cb * math.sin(l)
    z = (rn * (1 - ec) + h) * sb

    return (x, y, z)


def xyz2llh(x, y, z, eq, ec):
    # 直行座標系(x, y, z)から楕円体(b, l, h)へ
    # Rectangular Coordinate System to Ellipoid
    bda = math.sqrt(1 - ec)

    p   = math.sqrt(x*x + y*y)
    t   = math.atan2(z, p*bda)
    st  = math.sin(t)
    ct  = math.cos(t)
    b   = math.atan2(z + ec * eq/bda * st * st * st, p - ec * eq * ct * ct * ct)
    l   = math.atan2(y, x);

    sb  = math.sin(b);
    rn  = eq / math.sqrt(1 - ec * sb * sb)
    h   = p / math.cos(b) - rn;

    #return (b / rad, l / rad, h)
    return (b / rad, l / rad)


def tokyo2wgs(lat, lng, height = 0):
    # 日本測地系から世界測地系へ
    # 引数は 緯度, 経度, 高度
    dx = -148
    dy = 507
    dz = 681

    rcs = llh2xyz(lat, lng, height, tky, ec_t)
    ellips = xyz2llh(rcs[0] + dx, rcs[1] + dy, rcs[2] + dz, wgs, ec_w)

    return (ellips)


def wgs2tokyo(lat, lng, height = 0):
    # 世界測地系から日本測地系へ
    # 引数は 緯度, 経度, 高度
    dx = +128
    dy = -481
    dz = -664

    rcs = llh2xyz(lat, lng, height, wgs, ec_w)
    ellips = xyz2llh(rcs[0] + dx, rcs[1] + dy, rcs[2] + dz, tky, ec_t)

    return (ellips)


#def llh2xyz(b, l, h, eq, ec):
#    # 楕円体(b, l, h)から直行座標系(x, y, z)へ
#    # Ellipoid to Rectangular Coordinate System
#    b_rad = b * rad
#    l_rad = l * rad
#
#    rn = eq / math.sqrt(1 - ec * math.pow(math.sin(b_rad), 2))
#
#    x = (rn + h) * math.cos(b_rad) * math.cos(l_rad)
#    y = (rn + h) * math.cos(b_rad) * math.sin(l_rad)
#    z = (rn * (1 - ec) + h) * math.sin(b_rad)
#
#    return (x, y, z)
#
#
#def xyz2llh(x, y, z, eq, ec):
#    # 直行座標系(x, y, z)から楕円体(b, l, h)へ
#    # Rectangular Coordinate System to Ellipoid
#    bda = math.sqrt(1 - ec)
#
#    p = math.sqrt(x * x + y * y)
#    t = math.atan2(z, p * bda)
#
#    b = math.atan2(z + ec * eq / bda * math.pow(math.sin(t), 3), p - ec * eq * math.pow(math.cos(t), 3))
#    l = math.atan2(y, x)
#
#    h = p / math.cos(b) - eq / math.sqrt(1 - ec * math.pow(math.sin(b), 2))
#
#    #return (b / rad, l / rad, h)
#    return (b / rad, l / rad)
#
#
#def tokyo2wgs(lat, lng, height = 0):
#    # 日本測地系から世界測地系へ
#    # 引数は 緯度, 経度, 高度
#    dx = -148
#    dy = 507
#    dz = 681
#
#    rcs = llh2xyz(lat, lng, height, tky, ec_t)
#    ellips = xyz2llh(rcs[0] + dx, rcs[1] + dy, rcs[2] + dz, wgs, ec_w)
#
#    return (ellips)
#
#
#def wgs2tokyo(lat, lng, height = 0):
#    # 世界測地系から日本測地系へ
#    # 引数は 緯度, 経度, 高度
#    dx = +128
#    dy = -481
#    dz = -664
#
#    rcs = llh2xyz(lat, lng, height, wgs, ec_w)
#    ellips = xyz2llh(rcs[0] + dx, rcs[1] + dy, rcs[2] + dz, tky, ec_t)
#
#    return (ellips)


#print tokyo2wgs(1, 1)
#print wgs2tokyo(1, 1)

