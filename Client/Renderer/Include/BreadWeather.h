
#ifndef _BreadWeather_h_
#define _BreadWeather_h_

#include "BreadDefinition.h"
#include "BreadVector3.h"
#include "BreadMatrix4.h"

namespace Bread
{
	class cEntityMesh;
	class cWeatherShader;
	class cEngineConfig;

#pragma pack(push, 1)
	/*
	*/
	struct sWeatherParameter
	{
		float m_density;
		float m_speed;
		float m_length;
		float m_width;
		float m_alpha;
		float m_wind;
	};
	/*	update되면서 변하는 값들
	*/
	struct sWeatherData
	{
		float m_speedScale;
		float m_speedColorScale;
		sVector3 m_wind;
		sVector3 m_offset;
		sVector3 m_random;

		float m_tempAlpha;
		sVector3 m_tempOffset;	
		sVector3 m_tempVelocity;
	};
#pragma pack(pop)

	/*
		@date 2010.00.00
		@auth prodongi
		@desc 
		@todo 
	*/
	class cWeather
	{
	public:
		enum eWeatherType	{	CLEAR, LIGHT_RAIN, HEAVY_RAIN, LIGHT_SNOW, HEAVY_SNOW, DEFAULT };

	public:
		cWeather();
		virtual ~cWeather();
		virtual bool initialize(cEngineConfig const* config);
		virtual void finalize();
		virtual void update(float elapsedtime);
		virtual void render();
		void setDensity(float density);
		void setType(int type);
		
	protected:
		void setParameterType(int type);
		bool createParticle(int particleNum, float particleBoxSize);
		bool createShader();
		void createTexture(TCHAR const* texName);
		void initParameter();
		void updateData(float elapsedtime);
		void updateData(float elapsedtime, sVector3 const& noise, int pass);
		void updateShader(float elapsedtime);

	protected:
		enum {	PASS_NUM = 20 };
		
		cEntityMesh* m_particle;
		cWeatherShader* m_shader;
		sWeatherParameter m_param;
		sWeatherData m_data[PASS_NUM];
		sVector3 m_noise;
		sVector4 m_lighting;	/// @brief 라이트 칼라값
		sMatrix4 m_wvpPrev;
		float m_particleBox;
		float m_alphaFade;
		int m_pass;
		BUID m_texBuid;
		int m_type;
	};
}



#endif