
#ifndef _BreadEphemeris_h_
#define _BreadEphemeris_h_

/*
	BreadEphemeris.h
	Copyright (c) prodongi. All rights resserved.

	date : 2012-04-25
	purpose : game engine gems1 chapter 15에서 갖고 옴
*/

#include "BreadVector3.h"
#include "BreadMatrix3.h"
#include "BreadLocalTime.h"
#include "BreadGeographicLocation.h"

namespace Bread
{	
	/** An enumeration of planets that are visible to the naked eye. */
	enum Planets
	{
		MERCURY = 0,
		VENUS,
		EARTH,
		MARS,
		JUPITER,
		SATURN,
		NUM_PLANETS
	};

	/** A structure to store the calculated position and magnitude of a given planet. */
	struct sPlanet
	{
		float lastEpochDaysCalculated;
		float rightAscension;
		float declination;
		float visualMagnitude;
	};

	/** This class computes the positions of the sun and moon in the sky, and the
	positions and magnitudes of visible planets as well. It also provides methods for
	converting between ecliptic, equatorial, and horizon coordinates. */
	class cEphemeris
	{
	public:
		/** Default constructor. */
		cEphemeris();

		/** Recomputes the sun and moon positions based on the time and cGeographicLocation
		passed in. Planet positions are cached, but will be recomputed if the time
		differs by more than one hour from the previous call to Update(). */
		void update(const cLocalTime& time, const cGeographicLocation& location);

		/** Returns the position of the sun in equatorial coordinates. Requires that
		Update() was previously called.  태양의 적도 위치*/
		sVector3 getSunPositionEquatorial() const 
		{
			return sunEq;
		}

		/** Returns the position of the moon in equatorial coordinates. Requires that
		Update() was previously called. 달의 적도 위치*/
		sVector3 getMoonPositionEquatorial() const 
		{
			return moonEq;
		}

		/** Returns the position of the sun in geographic coordinates. Requires that
		Update() was previously called. 태양의 지리적 위치*/
		sVector3 getSunPositionGeographic() const 
		{
			return sunGeo;
		}

		/** Returns the position of the moon in geographic coordinates. Requires that
		Update() was previously called. 달의 지리적 위치*/
		sVector3 getMoonPositionGeographic() const 
		{
			return moonGeo;
		}


		/** Returns the position of the sun in ecliptic coordinates. Requires that
		Update() was previously called. 태양의 황도 위치*/
		sVector3 getSunPositionEcliptic() const 
		{
			return sunEcl;
		}

		/** Returns the position of the moon in ecliptic coordinates. Requires that
		Update() was previously called. 달의 황도 위치*/
		sVector3 getMoonPositionEcliptic() const 
		{
			return moonEcl;
		}

		/** Returns the position of the sun in horizon coordinates. Requires that
		Update() was previously called. 태양의 수평선 위치*/
		sVector3 getSunPositionHorizon() const 
		{
			return sunHoriz;
		}

		/** Returns the position of the moon in horizon coordiantes. Requires that
		Update() was previously called. 달의 수평선 위치*/
		sVector3 getMoonPositionHorizon() const 
		{
			return moonHoriz;
		}

		/** Retrieves the "phase angle" of the moon. A full moon has a phase angle of 180
		degrees; a new moon has a phase angle of 0 degrees. Requires that Update()
		was previously called. 달의 위상 각도*/
		float getMoonPhaseAngle() const 
		{
			return moonPhaseAngle;
		}

		/** Retrieves the phase of the moon in terms of percentage of the moon that is
		illuminated. Ranges from 0 (new moon) to 1.0 (full moon.) Requires that Update()
		was previously called. 달의 위상(주기)*/
		float getMoonPhase() const 
		{
			return moonPhase;
		}

		/** Retrieves the distance between the Earth and the Moon in kilometers. Useful
		for determining the brightness of the moon, in conjunction with the moon's phase.
		Requires that Update() was previously called. 지구와 달까지의 거리*/
		float getMoonDistanceKM() const 
		{
			return moonDistance;
		}

		/** Retrieves the horizon coordinates and visual magnitude of the visible planets.
		Requires that Update() was previously called.

		\param planet A member of the Planets enumeration specifying which planet you which to
		obtain the position of (excluding the Earth).
		\param ra The Right Ascension of the planet requested.
		\param dec The declination of the planet requested.
		\param visualMagnitude The visual magnitude of the planet requested (remember lower values
		are brighter)
		*/
		void getPlanetPosition(int planet, float& ra, float& dec, float& visualMagnitude) const;

		/** Retrieves a 3x3 matrix that will transform ecliptic coordinates to horizon coordinates.
		Requires that Update() was previously called. */
		sMatrix3 getEclipticToHorizonMatrix() const 
		{
			return eclipticToHorizon;
		}

		/** Retrieves a 3x3 matrix that will transform equatorial coordinates (x through
		the vernal equinox) to geographic coordinates (x through the prime meridian.) */
		sMatrix3 GetEquatorialToGeographicMatrix() const {
			return equatorialToGeographic;
		}

		/** Retrieves a 3x3 matrix that will transform equatorial coordinates to horizon
		coordinates. Requires that Update() was previously called. */
		sMatrix3 getEquatorialToHorizonMatrix() const 
		{
			return equatorialToHorizon;
		}

		/** Retrieves a 3x3 matrix to transform horizon coordinates to equatorial
		coordinates. Requires that Update() was previously called. */
		sMatrix3 getHorizonToEquatorialMatrix() const 
		{
			return horizonToEquatorial;
		}

		/** Retrieves a 3x3 matrix to transform horizon coordinates to geocentric
		coordinates. Requires that Update() was previously called. */
		sMatrix3 getHorizonToGeographicMatrix() const 
		{
			return horizonToGeographic;
		}

		/** Returns the fractional number of centuries elapsed since January 1, 2000 GMT,
		terrestrial time (this is "atomic clock time," which does not account for leap seconds
		to correct for slowing of the Earth's rotation). */
		float getEpochCenturies() const 
		{
			return T;
		}

	private:
		/** Calculates the position of the sun; called by Update(). */
		void computeSunPosition();

		/** Calculates the position of the Earth; called by Update(). This must be called
		prior to calling ComputePlanetPosition(). */
		void computeEarthPosition();

		/** Calculates the position of the moon; called by Update(). */
		void computeMoonPosition();

		/** Computes the position of a given planet from the Planets enumeration. Called
		by Update(). Requires that ComputeEarthPosition() was previously called. */
		void computePlanetPosition(int planet);

		/** Models atmospheric refraction for objects close to the horizon. This does
		not model variations in atmosphere pressure and temperature.

		\param elevation The elevation of the object above the horizon, in radians.
		\return The apparent elevation of the object above the horizon, in radians,
		after simulating atmospheric refraction.
		*/
		float refract(float elevation);

		/** Converts polar coordinates to cartesian coordinates. */
		sVector3 toCartesian(float r, float latitude, float longitude);

		sMatrix3 equatorialToHorizon;
		sMatrix3 eclipticToHorizon;
		sMatrix3 eclipticToEquatorial;
		sMatrix3 equatorialToGeographic;
		sMatrix3 horizonToEquatorial;
		sMatrix3 horizonToGeographic;

		float sunEclipticLongitude;
		float moonPhase, moonPhaseAngle, moonDistance;
		float GMST, LMST;
		sMatrix3 precession;

		float T, Tuncorr, epochDays;
		sVector3 moonEq, sunEq;
		sVector3 moonEcl, sunEcl;
		sVector3 sunHoriz, moonHoriz;
		sVector3 moonGeo, sunGeo;

		float R, L, e;

		cLocalTime lastTime;
		cGeographicLocation lastLocation;
		sPlanet planets[NUM_PLANETS];
	};
}



#endif