
/*
BreadEphemeris.cpp
Copyright (c) prodongi. All rights resserved.
*/

#include "stdafx.h"
#include "BreadMath.h"
#include "BreadEphemeris.h"

namespace Bread
{
	#define PLANET_DELTA_DAYS (1.0 / 24.0)

	cEphemeris::cEphemeris()
	{
		lastTime.setYear(0);

		for (int i = 0; i < NUM_PLANETS; i++)
		{
			planets[i].lastEpochDaysCalculated = 0;
		}
	}

	void cEphemeris::update(const cLocalTime& time, const cGeographicLocation& location)
	{
		bool timeChanged = false;
		bool locationChanged = false;

		if (time != lastTime)
		{
			timeChanged = true;
			lastTime = time;
		}

		if (location != lastLocation)
		{
			locationChanged = true;
			lastLocation = location;
		}

		if (timeChanged || locationChanged)
		{
			T = time.getEpoch2000Centuries(true);
			Tuncorr = time.getEpoch2000Centuries(false);
			epochDays = time.getEpoch1990Days(false);

			sMatrix3 Rx, Ry, Rz;
			Rx.rotationX(-0.1118f * T);
			Ry.rotationY(0.00972f * T);
			Rz.rotationZ(-0.01118f * T);
			precession = Rz * (Ry * Rx);

			GMST = 4.894961f + 230121.675315f * Tuncorr; // cMath::degToRad
			LMST = GMST + cMath::degToRad(location.getLongitude()); // cMath::degToRad

			float latitude = cMath::degToRad(location.getLatitude());

			e = 0.409093f - 0.000227f * T;

			Ry.rotationX(-(latitude - cMath::PI / 2.0f));
			Rz.rotationZ(LMST);
			Rx.rotationY(-e);
			equatorialToHorizon = Ry * Rz * precession;
			eclipticToHorizon = Ry * Rz * Rx * precession;
			eclipticToEquatorial = Rx;

			equatorialToGeographic.rotationZ(GMST);

			equatorialToHorizon.transpose(horizonToEquatorial);
			horizonToGeographic = equatorialToGeographic * horizonToEquatorial;

			computeSunPosition();
			computeMoonPosition();
			computeEarthPosition();

			for (int i = 0; i < NUM_PLANETS; i++)
			{
				if (i != EARTH)
				{
					computePlanetPosition(i);
				}
			}
		}
	}

	sVector3 cEphemeris::toCartesian(float r, float latitude, float longitude)
	{
		sVector3 v;
		v.x = r * cMath::cosf(longitude) * cMath::cosf(latitude); // n
		v.y = r * cMath::sinf(longitude) * cMath::cosf(latitude); // e
		v.z = r * cMath::sinf(latitude); // up

		return v;
	}

	float cEphemeris::refract(float elevation)
	{
		/*
		*    Refraction correction, cMath::radToDeg
		*        Zimmerman, John C.  1981.  Sun-pointing programs and their
		*            accuracy.
		*            SAND81-0761, Experimental Systems Operation Division 4721,
		*            Sandia National Laboratories, Albuquerque, NM.
		*/

		//float prestemp;    /* temporary pressure/temperature correction */
		float refcor;      /* temporary refraction correction */
		float tanelev;     /* tangent of the solar elevation angle */

		/* If the sun is near zenith, the algorithm bombs; refraction near 0 */
		if ( elevation > cMath::degToRad(85.0f))
			refcor = 0.0;

		/* Otherwise, we have refraction */
		else {
			tanelev = cMath::tanf ( elevation );
			if ( elevation >= cMath::degToRad(5.0f) )
			{
				refcor  = 58.1f / tanelev -
					0.07f / ( pow (tanelev,3) ) +
					0.000086f / ( pow (tanelev,5) );
			}
			else if ( elevation >= cMath::degToRad(-0.575f) )
			{
				float degElev = cMath::radToDeg(elevation);
				refcor  = 1735.0f +
					degElev * ( -518.2f + degElev * ( 103.4f +
					degElev * ( -12.79f + degElev * 0.711f ) ) );
			}
			else
			{
				refcor  = -20.774f / tanelev;
			}
			//prestemp    =
			//    ( pdat->press * 283.0 ) / ( 1013.0 * ( 273.0 + pdat->temp ) );
			//refcor     *= prestemp / 3600.0;
			refcor /= 3600.0f;
		}

		/* Refracted solar elevation angle */
		float out = elevation + cMath::degToRad(refcor);

		return out;
	}

	void cEphemeris::computeSunPosition()
	{
		float M = 6.24f + 628.302f * T;

		float longitude = 4.895048f + 628.331951f * T + (0.033417f - 0.000084f * T) * sin(M)
			+ 0.000351f * cMath::sinf(2.0f * M);
		float latitude = 0.0f;
		float geocentricDistance = 1.000140f - (0.016708f - 0.000042f * T) * cos(M) -
			0.000141f * cMath::cosf(2.0f * M); // AU's

		sunEclipticLongitude = longitude;
		sunEcl = toCartesian(geocentricDistance, latitude, longitude);

		sunEq = eclipticToEquatorial * sunEcl;
		sunGeo = equatorialToGeographic * sunEq;

		// Oriented -x=n z=up y=east
		sVector3 tmp = eclipticToHorizon * sunEcl;
		sunHoriz.x = tmp.y; // x is east
		sunHoriz.y = tmp.z; // y is up
		sunHoriz.z = tmp.x; // -z is north

		// Account for atmospheric refraction.
		sVector3 tmp2 = sunHoriz;
		float R = tmp2.length();
		tmp2.normalize();
		float elev = asin(tmp2.y);
		elev = refract(elev);
		sunHoriz.y = R * cMath::sinf(elev);

	}

	static void inRange(float &in)
	{
		while (in > 2.0f * cMath::PI) in -= 2.0f * cMath::PI;
		while (in < 0.0f) in += 2.0f * cMath::PI;
	}

	void cEphemeris::computeMoonPosition()
	{
		float lp = 3.8104f + 8399.7091f * T;
		float m = 6.2300f + 628.3019f * T;
		float f = 1.6280f + 8433.4663f * T;
		float mp = 2.3554f + 8328.6911f * T;
		float d = 5.1985f + 7771.3772f * T;

		float longitude =
			lp
			+ 0.1098f * cMath::sinf(mp)
			+ 0.0222f * cMath::sinf(2*d - mp)
			+ 0.0115f * cMath::sinf(2*d)
			+ 0.0037f * cMath::sinf(2*mp)
			- 0.0032f * cMath::sinf(m)
			- 0.0020f * cMath::sinf(2*f)
			+ 0.0010f * cMath::sinf(2*d - 2*mp)
			+ 0.0010f * cMath::sinf(2*d - m - mp)
			+ 0.0009f * cMath::sinf(2*d + mp)
			+ 0.0008f * cMath::sinf(2*d - m)
			+ 0.0007f * cMath::sinf(mp - m)
			- 0.0006f * cMath::sinf(d)
			- 0.0005f * cMath::sinf(m + mp);

		float latitude =
			+ 0.0895f * cMath::sinf(f)
			+ 0.0049f * cMath::sinf(mp + f)
			+ 0.0048f * cMath::sinf(mp - f)
			+ 0.0030f * cMath::sinf(2*d  - f)
			+ 0.0010f * cMath::sinf(2*d + f - mp)
			+ 0.0008f * cMath::sinf(2*d - f - mp)
			+ 0.0006f * cMath::sinf(2*d + f);

		float pip =
			+ 0.016593f
			+ 0.000904f * cMath::cosf(mp)
			+ 0.000166f * cMath::cosf(2*d - mp)
			+ 0.000137f * cMath::cosf(2*d)
			+ 0.000049f * cMath::cosf(2*mp)
			+ 0.000015f * cMath::cosf(2*d + mp)
			+ 0.000009f * cMath::cosf(2*d - m);

		float dMoon = 1.0f / pip; // earth radii

		moonEcl = toCartesian(dMoon, latitude, longitude);

		moonEq = eclipticToEquatorial * moonEcl;
		moonGeo = equatorialToGeographic * moonEq;

		sVector3 tmp = (eclipticToHorizon * moonEcl);
		moonHoriz.x = tmp.y;
		moonHoriz.y = tmp.z;
		moonHoriz.z = tmp.x;

		inRange(longitude);
		inRange(sunEclipticLongitude);
		moonPhaseAngle = fabs(longitude - sunEclipticLongitude);
		inRange(moonPhaseAngle);
		moonPhase = 0.5f * (1.0f - cMath::cosf(moonPhaseAngle));

		// Convert to +- 180 cMath::radToDeg
		if (moonPhaseAngle > cMath::PI)
		{
			moonPhaseAngle = cMath::PI - moonPhaseAngle;
		}

		moonDistance = (moonHoriz - sVector3(0, 0, 1)).length() * 6378.137f;
	}

	struct sOrbitalElements
	{
		float period;
		float epochLongitude;
		float perihelionLongitude;
		float eccentricity;
		float semiMajorAxis;
		float inclination;
		float longitudeAscendingNode;
		float angularDiameter;
		float visualMagnitude;
	};

	sOrbitalElements planetElements[NUM_PLANETS] =
	{
		// Mercury
		{0.240852f, cMath::degToRad(60.750646f), cMath::degToRad(77.299833f), 0.205633f, 0.387099f,
		cMath::degToRad(7.004540f), cMath::degToRad(48.212740f), 6.74f, -0.42f},

		// Venus
		{0.615211f, cMath::degToRad(88.455855f), cMath::degToRad(131.430236f), 0.006778f, 0.723332f,
		cMath::degToRad(3.394535f), cMath::degToRad(76.589820f), 16.92f, -4.40f},

		// Earth
		{1.00004f, cMath::degToRad(99.403308f), cMath::degToRad(102.768413f), 0.016713f, 1.00000f,
		0, 0, 0, 0},

		// Mars
		{1.880932f, cMath::degToRad(240.739474f), cMath::degToRad(335.874939f), 0.093396f, 1.523688f,
		cMath::degToRad(1.849736f), cMath::degToRad(49.480308f), 9.36f, -1.52f},

		// Jupiter
		{11.863075f, cMath::degToRad(90.638185f), cMath::degToRad(14.170747f), 0.048482f, 5.202561f,
		cMath::degToRad(1.303613f), cMath::degToRad(100.353142f), 196.74f, -9.40f},

		// Saturn
		{29.471362f, cMath::degToRad(287.690033f), cMath::degToRad(92.861407f), 0.055581f, 9.554747f,
		cMath::degToRad(2.488980f), cMath::degToRad(113.576139f), 165.60f, -8.88f}
	};

	void cEphemeris::computeEarthPosition()
	{
		float Np = ( (2.0f * cMath::PI) / 365.242191f ) * (epochDays / planetElements[EARTH].period);
		inRange(Np);

		float Mp = Np + planetElements[EARTH].epochLongitude - planetElements[EARTH].perihelionLongitude;

		L = Np + 2.0f * planetElements[EARTH].eccentricity * sin(Mp) +
			planetElements[EARTH].epochLongitude;

		inRange(L);

		float vp = L - planetElements[EARTH].perihelionLongitude;

		R = (planetElements[EARTH].semiMajorAxis * (1.0f - planetElements[EARTH].eccentricity
			* planetElements[EARTH].eccentricity)) / (1.0f + planetElements[EARTH].eccentricity
			* cos(vp));
	}

	void cEphemeris::getPlanetPosition(int planet, float& ra, float& dec, float& visualMagnitude) const
	{
		if (planet < NUM_PLANETS)
		{
			ra = planets[planet].rightAscension;
			dec = planets[planet].declination;
			visualMagnitude = planets[planet].visualMagnitude;
		}
	}

	void cEphemeris::computePlanetPosition(int planet)
	{
		if ((epochDays - planets[planet].lastEpochDaysCalculated) < PLANET_DELTA_DAYS)
		{
			return;
		}
		else
		{
			planets[planet].lastEpochDaysCalculated = epochDays;
		}

		float Np = ( (2.0f * cMath::PI) / 365.242191f ) * (epochDays / planetElements[planet].period);
		inRange(Np);

		float Mp = Np + planetElements[planet].epochLongitude - planetElements[planet].perihelionLongitude;

		float heliocentricLongitude = Np + 2.0f * planetElements[planet].eccentricity * sin(Mp) +
			planetElements[planet].epochLongitude;

		inRange(heliocentricLongitude);

		float vp = heliocentricLongitude - planetElements[planet].perihelionLongitude;

		float r = (planetElements[planet].semiMajorAxis * (1.0f - planetElements[planet].eccentricity
			* planetElements[planet].eccentricity)) / (1.0f + planetElements[planet].eccentricity
			* cos(vp));

		float heliocentricLatitude = asin(sin(heliocentricLongitude -
			planetElements[planet].longitudeAscendingNode) * sin(planetElements[planet].inclination));

		inRange(heliocentricLatitude);

		float y = sin(heliocentricLongitude - planetElements[planet].longitudeAscendingNode) *
			cos(planetElements[planet].inclination);

		float x = cos(heliocentricLongitude - planetElements[planet].longitudeAscendingNode);

		float projectedHeliocentricLongitude = atan2(y, x) + planetElements[planet].longitudeAscendingNode;

		float projectedRadius = r * cos(heliocentricLatitude);

		float eclipticLongitude;

		if (planet > EARTH)
		{
			eclipticLongitude = atan( (R * sin(projectedHeliocentricLongitude - L))
				/ (projectedRadius - R * cos(projectedHeliocentricLongitude - L)) )
				+ projectedHeliocentricLongitude;
		}
		else
		{
			eclipticLongitude = cMath::PI + L + atan( (projectedRadius * sin(L - projectedHeliocentricLongitude))
				/ (R - projectedRadius * cos(L - projectedHeliocentricLongitude)) );
		}

		inRange(eclipticLongitude);

		float eclipticLatitude = atan( (projectedRadius * cMath::tanf(heliocentricLatitude)
			* sin(eclipticLongitude - projectedHeliocentricLongitude))
			/ (R * sin(projectedHeliocentricLongitude - L)));

		float ra = atan2( (cMath::sinf(eclipticLongitude) * cMath::cosf(e) - cMath::tanf(eclipticLatitude) * cMath::sinf(e))
			, cMath::cosf(eclipticLongitude));

		float dec = cMath::asinf(sin(eclipticLatitude) * cMath::cosf(e) + cMath::cosf(eclipticLatitude) * cMath::sinf(e)
			* sin(eclipticLongitude));

		float dist2 = R * R + r * r - 2.0f * R * r * cMath::cosf(heliocentricLongitude - L);
		float dist = cMath::sqrtf(dist2);

		float d = eclipticLongitude - heliocentricLongitude;
		float phase = 0.5f * (1.0f + cMath::cosf(d));

		float visualMagnitude;

		if (planet == VENUS)
		{
			d *= cMath::radToDeg(d);
			visualMagnitude = -4.34f + 5.0f * log10(r*dist) + 0.013f * d + 4.2E-7f  * d*d*d;
		}
		else
		{
			visualMagnitude = 5.0f * log10( (r * dist) / cMath::sqrtf(phase) ) + planetElements[planet].visualMagnitude;
		}

		planets[planet].rightAscension = ra;
		planets[planet].declination = dec;
		planets[planet].visualMagnitude = visualMagnitude;
	}
}