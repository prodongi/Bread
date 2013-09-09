
/*
	BreadcLocalTime.cpp
	Copyright (c) prodongi. All rights resserved.
*/

#include "stdafx.h"
#include "BreadLocalTime.h"
#include <math.h>
#if defined(WIN32) || defined(WIN64)
#include <windows.h>
#endif
#include <time.h>

namespace Bread
{
	cLocalTime::cLocalTime()
	{
		m_localYear = 2012;
		m_localMonth = 4;
		m_localDay = 22;
		m_localHours = 12;
		m_localMinutes = 0;
		m_localSeconds = 0;
		m_observingDST = true;
		m_zoneCorrection = -8;
	}

	void cLocalTime::addSeconds(long seconds)
	{
		struct tm oldDate;
		oldDate.tm_year = m_localYear - 1900;
		oldDate.tm_mon = m_localMonth - 1;
		oldDate.tm_mday = m_localDay;
		oldDate.tm_hour = m_localHours;
		oldDate.tm_min = m_localMinutes;
		oldDate.tm_sec = (int)m_localSeconds;
		oldDate.tm_isdst = m_observingDST ? 1 : 0;

		time_t time = mktime(&oldDate); //mktime assumes local time zone
		time += seconds;

		struct tm *date;
		date = localtime(&time);
		if (date)
		{
			m_localYear = date->tm_year + 1900;
			m_localMonth = date->tm_mon + 1;
			m_localDay = date->tm_mday;

			m_localHours = date->tm_hour;
			m_localMinutes = date->tm_min;
			m_localSeconds = (float)date->tm_sec;
		}
	}

	void cLocalTime::setFromEpochSeconds(time_t time)
	{
		struct tm *date;
		date = gmtime(&time);
		if (date)
		{
			m_localYear = date->tm_year + 1900;
			m_localMonth = date->tm_mon + 1;
			m_localDay = date->tm_mday;

			m_localHours = date->tm_hour;
			m_localMinutes = date->tm_min;
			m_localSeconds = (float)date->tm_sec;

			m_observingDST = date->tm_isdst > 0;

			m_zoneCorrection = 0;
		}
	}

	float cLocalTime::getJulianDate(bool terrestrialTime) const
	{
		// Convert to GMT
		float hours = (float)m_localHours + ((float)m_localMinutes + m_localSeconds / 60.0f) / 60.0f;

		if (m_observingDST)
			hours -= 1;

		hours -= m_zoneCorrection;

		float y, m;
		float d;
		d = m_localDay + (hours / 24.0f);

		if (m_localMonth < 3)
		{
			y = (float)(m_localYear - 1);
			m = (float)(m_localMonth + 12);
		}
		else
		{
			y = (float)m_localYear;
			m = (float)m_localMonth;
		}

		float JD = 1720996.5f - floor(y / 100.0f) + floor(y / 400.0f) + (365.0f * y)
			+ floor(30.6001f * (m + 1.0f)) + d;

		if (terrestrialTime)
		{
			JD += 65.0f / 60.0f / 60.0f / 24.0f;
		}

		return JD;
	}

	float cLocalTime::getEpoch2000Centuries(bool terrestrialTime) const
	{
		// Convert to GMT
		float hours = (float)m_localHours + ((float)m_localMinutes + m_localSeconds / 60.0f) / 60.0f;

		if (m_observingDST)
			hours -= 1;

		hours -= m_zoneCorrection;

		float y, m;
		float d, mantissa;
		d = (float)m_localDay + (hours / 24.0f);

		if (m_localMonth < 3)
		{
			y = (float)(m_localYear - 1);
			m = (float)(m_localMonth + 12);
		}
		else
		{
			y = (float)m_localYear;
			m = (float)m_localMonth;
		}

		mantissa = 1720996.5f - floor(y / 100.0f) + floor(y / 400.0f) + floor(365.25f * y)
			+ floor(30.6001f * (m + 1.0f));
		mantissa -= 2451545.0f;

		float JD = mantissa + d;

		if (terrestrialTime)
		{
			JD += 65.0f / 60.0f / 60.0f / 24.0f;
		}

		JD /= 36525.0f;

		return JD;
	}

	float cLocalTime::getEpoch1990Days(bool terrestrialTime) const
	{
		// Convert to GMT
		float hours = (float)m_localHours + ((float)m_localMinutes + m_localSeconds / 60.0f) / 60.0f;

		if (m_observingDST)
			hours -= 1;

		hours -= m_zoneCorrection;

		float y, m;
		float mantissa;
		float d;
		d = m_localDay + (hours / 24.0f);

		if (m_localMonth < 3)
		{
			y = (float)(m_localYear - 1);
			m = (float)(m_localMonth + 12);
		}
		else
		{
			y = (float)m_localYear;
			m = (float)m_localMonth;
		}

		mantissa = 1720996.5f - floor(y / 100.0f) + floor(y / 400.0f) + floor(365.25f * y)
			+ floor(30.6001f * (m + 1));

		mantissa -= 2447891.5f;

		float JD = mantissa + d;

		if (terrestrialTime)
		{
			JD += 65.0f / 60.0f / 60.0f / 24.0f;
		}

		return JD;
	}

	void cLocalTime::setFromSystemTime()
	{
#if defined(WIN32) || defined(WIN64)
		SYSTEMTIME sysTime;
		GetLocalTime(&sysTime);

		m_localYear = sysTime.wYear;
		m_localMonth = sysTime.wMonth;
		m_localDay = sysTime.wDay;
		m_localHours = sysTime.wHour;
		m_localMinutes = sysTime.wMinute;
		m_localSeconds = (float)sysTime.wSecond + ((float)sysTime.wMilliseconds / 1000.0f);

		TIME_ZONE_INFORMATION tz;
		DWORD result = GetTimeZoneInformation(&tz);
		m_observingDST = (result == TIME_ZONE_ID_DAYLIGHT);

		m_zoneCorrection = -(float)tz.Bias / 60.0f;
#else
		struct tm *sysTime;
		time_t now = time(NULL);
		sysTime = cLocalTime(&now);

		m_localYear = sysTime->tm_year + 1900;
		m_localMonth = sysTime->tm_mon + 1;
		m_localDay = sysTime->tm_mday;
		m_localHours = sysTime->tm_hour;
		m_localMinutes = sysTime->tm_min;
		m_localSeconds = sysTime->tm_sec;

		m_zoneCorrection = -(float)sysTime->tm_gmtoff / 3600.0;
		m_observingDST = (sysTime->tm_isdst == 1);
#endif
	}
}