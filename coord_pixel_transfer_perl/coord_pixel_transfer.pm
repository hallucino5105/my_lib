#!/usr/bin/perl

# 日本以外は誤差がありそう

package GoogleMapsCoordTransfer;


use strict;
use warnings;
use Data::Dumper;
use Math::Trig;
use Math::Round;
use POSIX qw(:math_h);


sub new
{
	my $class = shift;
	my $this = {
		zoomLevelMin => 0,
		zoomLevelMax => 19,
		tileWidth    => 256,
		tileHeight   => 256,
		latMin       => sprintf("%.16f", (atan(sinh(((128 - (256 -   0)) * pi) / 128))) * 180/pi),
		latMax       => sprintf("%.16f", (atan(sinh(((128 - (256 - 256)) * pi) / 128))) * 180/pi),
		lngMin       => sprintf("%.16f", (((  0 * pi) / 128) - pi) * 180/pi),
		lngMax       => sprintf("%.16f", (((256 * pi) / 128) - pi) * 180/pi)
	};

	return bless($this, $class);
}


sub DESTROY
{
}


sub fromLatLngToWorldCoord
{
	my $this = shift;
	my ($lat, $lng) = (sprintf("%.16f", shift), sprintf("%.16f", shift));

	if(($lat < $this->{latMin} || $lat > $this->{latMax}) || ($lng < $this->{lngMin} || $lng > $this->{lngMax})) {
		die "out of range latlng.";
	}

	my $R = 128/pi;
	my ($lat_rad, $lng_rad) = ($lat * pi/180, $lng * pi/180);

	return (
		sprintf("%.16f", $R * ($lng_rad + pi)),
		sprintf("%.16f", -$R/2 * log((1 + sin($lat_rad)) / (1 - sin($lat_rad))) + 128)
		);
}


sub fromWorldCoordToLatLng
{
	my $this = shift;
	my ($wx, $wy) = @_;

	if(($wx < 0 || $wx > $this->{tileWidth}) || ($wy < 0 || $wy > $this->{tileHeight})) {
		die "out of range world coordinate.";
	}

	my $R = 128/pi;

	return (
		sprintf("%.16f", (atan(sinh(((128 - $wy) * pi) / 128))) * 180/pi),
		sprintf("%.16f", ((($wx * pi) / 128) - pi) * 180/pi)
		);
}


# ズームレベルを考慮した画面上のピクセル座標を返す
sub fromLatLngToPixelCoord
{
	my $this = shift;
	my ($lat, $lng, $zoomLevel) = @_;

	if(!defined($zoomLevel) || $zoomLevel < $this->{zoomLevelMin} || $zoomLevel > $this->{zoomLevelMax}) {
		die "out of range zoom level.";
	}

	my ($wx, $wy) = $this->fromLatLngToWorldCoord($lat, $lng);

	return ($wx * (2 ** $zoomLevel), $wy * (2 ** $zoomLevel));
}


sub fromPixelCoordToLatLng
{
	my $this = shift;
	my ($px, $py, $zoomLevel) = @_;

	if(!defined($zoomLevel) || $zoomLevel < $this->{zoomLevelMin} || $zoomLevel > $this->{zoomLevelMax}) {
		die "out of range zoom level.";
	}

	my ($wx, $wy) = (
		sprintf("%.16f", $px / (2 ** $zoomLevel)), 
		sprintf("%.16f", $py / (2 ** $zoomLevel))
		);

	return $this->fromWorldCoordToLatLng($wx, $wy);
}


# タイル番号を返す
sub fromLatLngToTileCoord
{
	my $this = shift;
	my ($lat, $lng, $zoomLevel) = @_;

	my ($px, $py) = $this->fromLatLngToPixelCoord($lat, $lng, $zoomLevel);

	return (
		sprintf("%u", floor($px / $this->{tileWidth})), 
		sprintf("%u", floor($py / $this->{tileHeight})),
		# タイル内のピクセル座標
		sprintf("%.16f", $px % $this->{tileWidth}),
		sprintf("%.16f", $py % $this->{tileHeight})
		);
}


# タイルの左上隅(0, 0)を返す
sub fromTileCoordToLatLng
{
	my $this = shift;
	my ($tx, $ty, $zoomLevel) = @_;

	my ($px, $py) = ($tx * $this->{tileWidth}, $ty * $this->{tileHeight});

	return $this->fromPixelCoordToLatLng($px, $py, $zoomLevel);
}


sub zoomLevelRange
{
	my $this = shift;

	return ($this->{zoomLevelMin}, $this->{zoomLevelMax});
}


sub test
{
	my $this = shift;

	print "latMin = $this->{latMin}\n";
	print "latMax = $this->{latMax}\n";
	print "lngMin = $this->{lngMin}\n";
	print "lngMax = $this->{lngMax}\n";

	my ($lat, $lng) = (34.1234, 134.1234);
	#my ($lat, $lng) = $this->fromWorldCoordToLatLng(2, 23);
	print "(lat, lng) = $lat, $lng\n";

	my ($wx, $wy) = $this->fromLatLngToWorldCoord($lat, $lng);
	print "(wx, wy)   = $wx, $wy\n";

	my ($px, $py) = $this->fromLatLngToPixelCoord($lat, $lng, 7);
	print "(px, py)   = $px, $py\n";

	my ($tx, $ty) = $this->fromLatLngToTileCoord($lat, $lng, 7);
	print "(tx, ty)   = $tx, $ty\n";
}



1;


# my %GIS_JP_NORTHERNMOST  = ('lat', 45.5577777777778, 'lng', 148.753888888889);
# my %GIS_JP_SOURTHERNMOST = ('lat', 20.4252777777778, 'lng', 136.069722222222);
# my %GIS_JP_EASTERNMOST   = ('lat', 24.2830555555556, 'lng', 153.986388888889);
# my %GIS_JP_WESTERNMOST   = ('lat', 24.4494444444444, 'lng', 122.933611111111);

# my $_zoomLevel = $ARGV[0];
# my $_lat       = $ARGV[1];
# my $_lng       = $ARGV[2];

# my $mapWidth;
# my $mapHeight;


# private function fromLatLngToContainerPixel(latLng:LatLng):Point
# {
# 	var centerLat:Number = (MAX_LAT - MIN_LAT) / 2;
# 	var centerLon:Number = (MAX_LON - MIN_LON) / 2;
# 	var maxZoom:int = 19;
# 	var mapWidth:int  = _map.width;
# 	var mapHeight:int = _map.height;
	
# 	return new Point(
# 		Math.round(0.709505 * Math.pow(maxZoom, 2) * (latLng.lng() - centerLon) + mapWidth / 2), 
# 		Math.round(
# 			- 40.743664 
# 			* Math.pow(maxZoom, 2) 
# 			* asinh(Math.tan(latLng.lat() * Math.PI / 180)) 
# 			- asinh(Math.tan(centerLat * Math.PI / 180))) 
# 		+ mapHeight / 2);
# }
