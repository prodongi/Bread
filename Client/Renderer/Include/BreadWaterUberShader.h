
#ifndef _BreadWaterUberShader_h_
#define _BreadWaterUberShader_h_

#include "BreadUberShader.h"
#include "BreadVector2.h"
#include "BreadVector4.h"

namespace Bread
{
	/*
		@date 2011.07.11
		@auth prodongi
		@desc 
		@todo 
	*/
	class cWaterUberShader : public cUberShader
	{
	public:
		cWaterUberShader();
		virtual ~cWaterUberShader();
		virtual void update(float elapsedtime, cMesh* /*mesh*/, cMaterial* /*mtl*/, cEntityModel* /*entity*/);
		virtual void setMaterialParam(cMaterial const* mtl);
		virtual void setMeshParam(cMesh const* mesh, cEntityModel const* entity);
		
	protected:
		virtual bool initialize();
		virtual void registHandle();
		void updateTime(float elapsedtime);
		void updateKeyboard();
		void modifyValue(float& value, float offset, float _min, float _max, TCHAR const* type);
		void defaultValue();

	protected:
		float m_time;
		std_string m_environmentTexBuid;
		std_string m_normalTexBuid;

		sVector2 m_textureScale;
		float m_waveFreq;
		float m_waveAmp;
		float m_bumpScale;
		sVector2 m_bumpSpeed;
		sVector4 m_deepColor;
		sVector4 m_shallowColor;
		float m_waterAmount;
		sVector4 m_reflectionColor;
		float m_reflectionAmount;
		float m_reflectionBlur;
		float m_fresnelPower;
		float m_fresnelBias;
		float m_hdrMultiplier;
	};

	/*
	*/
	cUberShader* createWaterUberShader();
}



#endif