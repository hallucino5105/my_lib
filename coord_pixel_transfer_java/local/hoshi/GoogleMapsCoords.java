package local.hoshi;

import local.hoshi.Point;
import local.hoshi.Location;
import local.hoshi.Tuple.Pair;


public class GoogleMapsCoords
{
	private static final double PI = Math.PI;
	private static final double R  = 128/PI;
	
	private static final int _tileWidth  = 256;
	private static final int _tileHeight = 256;
	private static final double _zoomLevelMin = 0.0;
	private static final double _zoomLevelMax = 19.0;
	private static final double _latMin = Double.parseDouble(String.format("%3.8f", (Math.atan(Math.sinh(((128 - (256 -   0)) * PI) / 128))) * 180/PI));
	private static final double _latMax = Double.parseDouble(String.format("%3.8f", (Math.atan(Math.sinh(((128 - (256 - 256)) * PI) / 128))) * 180/PI));
	private static final double _lngMin = Double.parseDouble(String.format("%3.8f", (((  0 * PI) / 128) - PI) * 180/PI));
	private static final double _lngMax = Double.parseDouble(String.format("%3.8f", (((256 * PI) / 128) - PI) * 180/PI));


	public GoogleMapsCoords() 
	{
	}
	
	
	public Point fromLatLngToWorldCoord(double lat, double lng)
	{
		Double latCast = Double.parseDouble(String.format("%3.8f", lat));
		Double lngCast = Double.parseDouble(String.format("%3.8f", lng));

		if((latCast < _latMin || latCast > _latMax) || (lngCast < _lngMin || lngCast > _lngMax)) {
			throw new IllegalArgumentException("location out of range.");
		}
		
		Double latRad = latCast * PI/180;
		Double lngRad = lngCast * PI/180;
		
		return new Point(
				Double.parseDouble(String.format("%u", Math.round(-R/2 * Math.log((1 + Math.sin(latRad)) / (1 - Math.sin(latRad))) + 128))),
				Double.parseDouble(String.format("%u", Math.round(R * (lngRad + PI)))));
	}
	
	
	public Location fromWorldCoordToLatLng(double wx, double wy)
	{
	    if((wx < 0 || wx > _tileWidth) || (wy < 0 || wy > _tileHeight)) {
	    	throw new IllegalArgumentException("world coordinate out of range.");
	    }

	    return new Location(
	    		Double.parseDouble(String.format("%3.8f", (Math.atan(Math.sinh(((128 - wx) * PI) / 128))) * 180/PI)),
	    		Double.parseDouble(String.format("%3.8f", (((wy * PI) / 128) - PI) * 180/PI)));
	}
	
	
	// ズームレベルを考慮した画面上のピクセル座標を返す                                                                                                                                                      
	public Point fromLatLngToPixelCoord(double lat, double lng, double zoomLevel)
	{
	    if(zoomLevel < _zoomLevelMin || zoomLevel > _zoomLevelMax) {
	    	throw new IllegalArgumentException("zoom level out of range.");
	    }

	    Point pc = this.fromLatLngToWorldCoord(lat, lng);

	    return new Point(pc.x * Math.pow(2, zoomLevel), pc.y * Math.pow(2, zoomLevel));
	}


	public Location fromPixelCoordToLatLng(int px, int py, double zoomLevel)
	{
		if(zoomLevel < _zoomLevelMin || zoomLevel > _zoomLevelMax) {
	    	throw new IllegalArgumentException("zoom level out of range.");
	    }

		double wx = px / Math.pow(2, zoomLevel);
		double wy = py / Math.pow(2, zoomLevel);

	    return this.fromWorldCoordToLatLng(wx, wy);
	}
	
	
	// タイル番号を返す                                                                                                                                                                                      
	public Pair<Location, Point> fromLatLngToTileCoord(double lat, double lng, double zoomLevel)
	{
		Point pc = this.fromLatLngToPixelCoord(lat, lng, zoomLevel);
		
		return new Pair<Location, Point>(
				new Location(
						Double.parseDouble(String.format("%u", Math.floor(pc.x / _tileWidth))),
						Double.parseDouble(String.format("%u", Math.floor(pc.y / _tileHeight)))),
				// タイル内のピクセル座標
				new Point(
						Double.parseDouble(String.format("%u", pc.x % _tileWidth)),
						Double.parseDouble(String.format("%u", pc.y % _tileHeight))));
	}


	 // タイルの左上隅(0, 0)を返す                                                                                                                                                                            
	 public Location fromTileCoordToLatLng(int tx, int ty, double zoomLevel)
	 {
	     int px = tx * _tileWidth;
	     int py = ty * _tileHeight;

	     return this.fromPixelCoordToLatLng(px, py, zoomLevel);
	 }


	 public Pair<Double, Double> zoomLevelRange()
	 {
	     return new Pair<Double, Double>(_zoomLevelMin, _zoomLevelMax);
	 }
}
