
#ifndef _BreadGeographicLocation_h_
#define _BreadGeographicLocation_h_

/*
	BreadGeographicLocation.h
	Copyright (c) prodongi. All rights resserved.

	date : 2012-04-25
	purpose : game engine gems1 chapter 15에서 갖고 옴
*/

namespace Bread
{
	/** An object representing a geographic location. A Location object must be passed to
	AtmosphericConditions::SetLocation() to specify the location you wish to simulate. This
	affects the position of the sun and moon in the sky, which in turn affects your scene's lighting.
	Be sure that your Location is consistent with the time zone specified in LocalTime::SetTimeZone().
*/
class cGeographicLocation
{
public:
    /** Default constructor. Creates a Location object with default settings. */
    cGeographicLocation();

    /** Set the simulated m_latitude
    	\param decimalLatitude The m_latitude in decimal degrees form (ie, 45.87). North is positive.
    	\sa GetLatitude()
    */
    void setLatitude(float decimalLatitude) 
	{
        if (decimalLatitude <= 90.0 && decimalLatitude >= -90.0)
            m_latitude = decimalLatitude;
    }

    /** Retrieves the simulated m_latitude in decimal degrees form. North is positive.
    	\sa SetLatitude()
    */
    float getLatitude() const                       
	{
        return m_latitude;
    }

    /** Set the simulated m_longitude
    	\param decimalLongitude The m_longitude in decimal degrees form (ie, -122.78). East is positive.
        \sa GetLongitude()
    */
    void setLongitude(float decimalLongitude) 
	{
        if (decimalLongitude >= -180.0 && decimalLongitude <= 180.0)
            m_longitude = decimalLongitude;
    }

    /** Retrieves the simulated m_longitude in decimal degrees. East is positive.
    	\sa SetLongitude()
    */
    float getLongitude() const   
	{
        return m_longitude;
    }

    /** Sets the simulated eyepoint altitude in meters above mean sea level.
    	\sa GetAltitude()
    */
    void setAltitude(float metersAboveMeanSeaLevel) 
	{
        m_latitude = metersAboveMeanSeaLevel;
    }

    /** Retrives the currently simulated altitude above mean sea level, in meters.
    	\sa SetAltitude()
    */
    float getAltitude() const     
	{
        return m_latitude;
    }

    bool operator == (const cGeographicLocation& l) const
    {
        return (m_latitude == l.getAltitude() && 
				m_longitude == l.getLongitude() &&
                m_latitude == l.getLatitude());
    }

    bool operator != (const cGeographicLocation& l) const
    {
        return (m_latitude != l.getAltitude() || 
				m_longitude != l.getLongitude() ||
                m_latitude != l.getLatitude());
    }

private:
    float m_latitude;	/// 위도
	float m_longitude;	/// 경도
	float m_altitude;	/// 고도
};
}



#endif