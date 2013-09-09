
#ifndef _BreadLuminanceMapper_h_
#define _BreadLuminanceMapper_h_

/*
	BreadLuminanceMapper.h
	Copyright (c) prodongi. All rights resserved.

	date : 2012-04-dd
	purpose : game engine gems1 chapter 15¿¡¼­ °®°í ¿È
*/

#include "BreadVector3.h"

namespace Bread
{	
	/** Manages tone-mapping of high dynamic range images (such as the sky!) to
	values a computer display can reproduce. In addition to dynamic range
	compression, it also simulates scotopic vision by applying a blue-shift
	and loss of color information as lighting conditions degrade.

	See DURAND, F., AND DORSEY, J. 2000. Interactive tone mapping. In <i>Eurographics
	Workshop on Rendering</i>, 219?30 for some of the ideas behind this
	class.
	*/
	class cLuminanceMapper
	{
	public:
		/** Sets the modeled maximum luminance of the display, in "nits", or
		candela per square meter. */
		static void SetMaxDisplayLuminance(float nits);

		/** Sets the log-average of the scene's luminance as perceived by both
		the eye's rods and cones, in nits. */
		static void SetSceneLogAvg(float rodNits, float coneNits);

		/** Performs tone-mapping on an xyY color, where x and y are
		chromaticity and Y is luminance. The values passed in are modified
		by this method. Assumes that SetMaxDisplayLuminance() and
		SetSceneLogAvg() were previously called. */
		static void DurandMapper(float *x, float *y, float *Y);

		/** Performs tone-mapping on a XYZ color. The color passed in is
		modified by this method. Assumes that SetMaxDisplayLuminance() and
		SetSceneLogAvg() were previously called. */
		static void DurandMapperXYZ(sVector3 *XYZ);

		/** Returns the computed scale factors for mapping luminance for
		both the eye's rods and cones. Assumes SetMaxDisplayLuminance() and
		SetSceneLogAvg() were previously called. */
		static void GetLuminanceScales(float *rodSF, float *coneSF)
		{
			*rodSF = mR;
			*coneSF = mC;
		}

		/** Retrieves the maximum display luminance previously set by
		SetMaxDisplayLuminance(). */
		static float GetMaxDisplayLuminance() {
			return Ldmax;
		}

		/** Returns the luminance, in nits, that is mapped to the maximum
		luminance the display can represent. Luminances higher than this are displayed
		as white. Assumes SetMaxDisplayLuminance() and SetSceneLogAvg() were
		previously called. */
		static float GetBurnoutLuminance();

		/** Retrieves the computed blend factor between rod and cone perception
		based on the current lighting conditions. Assumes SetSceneLogAvg() was
		previously called. */
		static float GetRodConeBlend() {
			return k;
		}

		/** Retrieves the log-average rod and cone luminances in nits, as
		previously set by SetSceneLogAvg(). */
		static void GetSceneLogAvg(float *rodNits, float *coneNits)
		{
			*rodNits = LsavgR;
			*coneNits = LsavgC;
		}

	private:
		static void ComputeScaleFactors();
		static float Ldmax, Lsavg;
		static float mR, mC, k, LsavgR, LsavgC;
	};
}



#endif