package local.hoshi;


public class Location
{
	public Double lat = 0.0;
	public Double lng = 0.0;


	public Location(Double lat, Double lng)
	{
		this.lat = lat;
		this.lng = lng;
	}


	public Double getLatitude()
	{
		return lat;
	} 


	public Double getLongitude()
	{
		return lng;
	}
}
