#!/usr/bin/perl

package location;

use strict;
use warnings;


sub peano2tokyo
{
	my $peano = shift;

	my @x_pat = (0, 1, 0, 1, 2, 3, 2, 3, 0, 1, 0, 1, 2, 3, 2, 3);
	my @y_pat = (0, 0, 1, 1, 0, 0, 1, 1, 2, 2, 3, 3, 2, 2, 3, 3);
	my $xl = 0;
	my $yl = 0;

	for ( my $i = 0; $i < length($peano); $i++ ) 
	{
#print $i,"\t";
		my $idx = hex(substr($peano, $i, 1));
#print substr($peano, $i, 1), "\t";
#print $idx,"\t";
#print $xl, "\t";
		$xl = ( $xl << 2 ) | $x_pat[$idx];
		$yl = ( $yl << 2 ) | $y_pat[$idx];
#print $x_pat[$idx], "\t";
#print $xl, "\t";
#print $y_pat[$idx], "\t";
#print $yl, "\n";
#print "\n";
	}

	return ($yl/1048576.0, $xl/1048576.0);
}


1;
