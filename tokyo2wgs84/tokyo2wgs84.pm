#!/usr/bin/perl

package location;

use strict;
use warnings;
use Location::GeoTool;


sub tokyo2wgs84()
{
	my ($lat, $lon) = @_;

	my $geo_tokyo = Location::GeoTool->new({changeMyself => 0,enableWantarray => 0});
	$geo_tokyo->set_coord($lat, $lon, 'tokyo', 'degree');

	my $geo_wgs = $geo_tokyo->datum_wgs84;
	my ($lat_wgs, $lon_wgs) = ($geo_wgs->lat,$geo_wgs->long);

	return ($lat_wgs, $lon_wgs);
	#print $lat_wgs . " " . $lon_wgs . "\n";
}


1;
