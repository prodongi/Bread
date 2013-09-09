
#ifndef _BreadLocalTime_h_
#define _BreadLocalTime_h_

/*
	BreadLocalTime.h
	Copyright (c) prodongi. All rights resserved.

	date : 2012-04-25
	purpose : game engine gems1 chapter 15¿¡¼­ °®°í ¿È
*/

#include <time.h>

namespace Bread
{

	/** An enumeration of defined time zones worldwide.
	Time zones are expressed as the hour correction (prior to daylight savings adjustments)
	from GMT. This enum provides names for the civilian time zones, and notes their military
	equivalents.
	*/
	enum TimeZones
	{
		GMT  = 0,   /// ZULU         Greenwich Mean Time, UTC, Western European (WET)
		CET  = 1,   /// ALPHA        Central European
		EET  = 2,   /// BETA         Eastern European
		BT   = 3,   /// CHARLIE      Baghdad
		CCT  = 8,   /// HOTEL        China Coast
		JST  = 9,   /// INDIA        Japan Standard
		GST  = 10,  /// KILO         Guam Standard
		IDLE = 12,  /// MIKE         International Date Line East, NZST (New Zealand Standard)
		WAT  = -1,  /// NOVEMBER     West Africa
		AT   = -2,  /// OSCAR        Azores
		AST  = -4,  /// QUEBEC       Atlantic Standard
		EST  = -5,  /// ROMEO        Eastern Standard
		CST  = -6,  /// SIERRA       Central Standard
		MST  = -7,  /// TANGO        Mountain Standard
		PST  = -8,  /// UNIFORM      Pacific Standard
		YST  = -9,  /// VICTOR       Yukon Standard
		AHST = -10, /// WHISKEY      Alaska-Hawaii Standard; Central Alaska (CAT), Hawaii Std (HST)
		NT   = -11, /// X-RAY        Nome
		IDLW = -12  /// YANKEE       International Date Line West
	};

	/** A class to represent the simulated local time, time zone, and daylight savings time observation.
	Be sure that your Location object represents a location consistent with the
	time zone you have specified as well.
	*/
	class cLocalTime
	{
	public:
		cLocalTime();
		virtual ~cLocalTime() {}

		/** Populates the LocalTime class based on epoch seconds (ie, as returned from
		\c time(NULL) ). Internally, it calls
		\c gmtime() on the value passed in, and populates the class based on the its results.
		The time zone will be set to GMT.

		\param time Seconds elapsed since midnight, January 1, 1970 UTC.
		*/
		void setFromEpochSeconds(time_t time);

		/** Set the calendar year to simulate. We only handle Gregorian calendar years,
		which means that years before 1582 will not be accurately simulated.
		\sa GetYear()
		*/
		void setYear(int year)                      
		{
			if (year >= 1582)
				m_localYear = year;
		}

		/** Retrieves the Gregorian calendar year.
		\sa SetYear()
		*/
		int  getYear() const           
		{
			return m_localYear;
		}

		/** Sets the calendar month to simulate.
		\param month The month to simulate, ranging from 1 (January) to 12 (December).
		\sa GetMonth();
		*/
		void setMonth(int month)      
		{
			if (month > 0 && month <= 12)
				m_localMonth = month;
		}

		/** Retrieves the calendar month (1-12).
		\sa SetMonth()
		*/
		int  getMonth() const 
		{
			return m_localMonth;
		}

		/** Sets the day of the month to simulate.
		\param day The day of month, ranging from 1-31.
		\sa GetDay()
		*/
		void setDay(int day)    
		{
			if (day > 0 && day <= 31)
				m_localDay = day;
		}

		/** Retrieves the day of the month (1-31).
		\sa SetDay()
		*/
		int getDay() const  
		{
			return m_localDay;
		}

		/** Sets the hour of day to simulate.
		\param hour The hour of day, ranging from 0-23 (0=midnight, 12=noon, 23=11PM)
		\sa GetHour()
		*/
		void setHour(int hour)  
		{
			if (hour >= 0 && hour < 24)
				m_localHours = hour;
		}

		/** Retrieves the hour of day (0-23).
		\sa SetHour()
		*/
		int  getHour() const
		{
			return m_localHours;
		}

		/** Sets the minute of the hour to simulate.
		\param minutes Minutes from 0-59
		\sa GetMinutes()
		*/
		void setMinutes(int minutes) 
		{
			if (minutes >= 0 && minutes < 60)
				m_localMinutes = minutes;
		}

		/** Retrieves the minute of the hour (0-59).
		\sa SetMinutes()
		*/
		int  getMinutes() const   
		{
			return m_localMinutes;
		}

		/** Sets the second of the minute to simulate.
		\param seconds Seconds from 0-59.
		\sa GetSeconds()
		*/
		void setSeconds(float seconds)                   
		{
			if (seconds >= 0 && seconds < 60)
				m_localSeconds = seconds;
		}

		/** Retrieves the second of the minute (0-59).
		\sa SetSeconds()
		*/
		float getSeconds() const 
		{
			return m_localSeconds;
		}

		/** Sets if Daylight Savings Time is currently observed in the simulation. Not required
		if using SetFromEpochSeconds(), but must be set otherwise.
		\sa GetObservingDaylightSavingsTime()
		*/
		void setObservingDaylightSavingsTime(bool inDST)    
		{
			m_observingDST = inDST;
		}

		/** Retrieves if daylight savings time is observed in the simulation.
		\sa SetObservingDaylightSavingsTime()
		*/
		bool getObservingDaylightSavingsTime() const    
		{
			return m_observingDST;
		}

		/** Specifies the time zone being simulated. This is an hour offset from GMT, use the
		#TimeZones enumerated type as a convenient way to specify the hour offset for a known
		time zone. Be sure that this time zone is consistent with the Location you specify.

		\param zone Hour offset from GMT, ignoring daylight savings time. ie, PST is -8.
		Use the #TimeZones enumeration to obtain the hour offset for specific time
		zones.
		\sa GetTimeZone()
		*/
		void setTimeZone(float zone)            
		{
			if (zone >= -12.0 && zone <= 12.0)
				m_zoneCorrection = zone;
		}

		/** Retrieves the currently simulated time zone.
		\return The hour offset from GMT, ignoring daylight savings time.
		\sa SetTimeZone()
		*/
		float getTimeZone() const  
		{
			return m_zoneCorrection;
		}

		/** Retrieves the Julian Date that this LocalTime object represents. Julian Dates are used
		for astronomical calculations (such as our own ephemeris model) and represent
		days and fractions since noon Universal Time on January 1, 4713 BCE on the Julian calendar.
		Note that due to precision limitations of 64-bit floats, the resolution of the date returned
		may be as low as within 8 hours.

		\param terrestrialTime Specifying terrestrial time means you want atomic clock time, not
		corrected by leap seconds to account for slowing of the Earth's
		rotation, as opposed to GMT which does account for leap seconds.

		\return the Julian date that this object represents.
		*/
		float getJulianDate(bool terrestrialTime) const;

		/** Obtains centuries and fraction since January 1, 2000 represented by this object. Used for
		internal astronomical calculations. Since this number is smaller than that returned by
		GetJulianDate(), it is of higher precision.

		\param terrestrialTime Specifying terrestrial time means you want atomic clock time, not
		corrected by leap seconds to account for slowing of the Earth's
		rotation, as opposed to GMT which does account for leap seconds.

		\return The fractional number of centuries elapsed since January 1, 2000.
		*/
		float getEpoch2000Centuries(bool terrestrialTime) const;

		/** Obtains days elapsed since January 1, 1990 represented by this object on the Julian
		calendar. Used for internal astronomical calculations. Since this number is smaller than
		that returned by GetJulianDate(), it is of higher precision.

		\param terrestrialTime Specifying terrestrial time means you want atomic clock time, not
		corrected by leap seconds to account for slowing of the Earth's
		rotation, as opposed to GMT which does account for leap seconds.

		\return The fractional number of days elapsed since January 1, 1990.
		*/
		float getEpoch1990Days(bool terrestrialTime) const;

		/** Populate the object based on the system's current local time settings. */
		void setFromSystemTime();

		/** Add the given number of seconds to the time represented by this object. */
		void addSeconds(long seconds);

		bool operator == (const cLocalTime& t) const
		{
			return (m_localSeconds == t.getSeconds() && m_localMinutes == t.getMinutes()
				&& m_localYear == t.getYear() && m_localMonth == t.getMonth() && m_localDay == t.getDay()
				&& m_localHours == t.getHour() && m_zoneCorrection == t.getTimeZone()
				&& m_observingDST == t.getObservingDaylightSavingsTime());
		}

		bool operator != (const cLocalTime& t) const
		{
			return (m_localSeconds != t.getSeconds() || m_localMinutes != t.getMinutes()
				|| m_localYear != t.getYear() || m_localMonth != t.getMonth() || m_localDay != t.getDay()
				|| m_localHours != t.getHour() || m_zoneCorrection != t.getTimeZone()
				|| m_observingDST != t.getObservingDaylightSavingsTime());
		}

		cLocalTime& operator = (const cLocalTime& t)
		{
			m_localSeconds = t.getSeconds();
			m_localMinutes = t.getMinutes();
			m_localYear = t.getYear();
			m_localMonth = t.getMonth();
			m_localDay = t.getDay();
			m_localHours = t.getHour();
			m_zoneCorrection = t.getTimeZone();
			m_observingDST = t.getObservingDaylightSavingsTime();

			return *this;
		}

	private:
		int m_localYear, m_localMonth, m_localDay, m_localHours, m_localMinutes;
		float m_zoneCorrection;
		float m_localSeconds;
		bool m_observingDST;
	};
}



#endif