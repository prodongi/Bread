
#include "BreadWeather.h"
#include "BreadEntityMesh.h"
#include "BreadShaderList.h"
#include "BreadSceneManager.h"
#include "BreadMeshCreater.h"
#include "BreadRenderPropertyManager.h"
#include "BreadCameraManager.h"
#include "BreadTextureManager.h"
#include "Perlin.h"

namespace Bread
{
	/*
	*/
	cWeather::cWeather() : m_particle(NULL), 
						   m_shader(NULL), 
						   m_alphaFade(1.0f),
						   m_type(CLEAR)
	{
	}
	/*
	*/
	cWeather::~cWeather()
	{
	}
	/*
	*/
	bool cWeather::initialize(cEngineConfig const* config)
	{
		if (!createParticle(config->getWeatherParticleNum(), config->getWeatherParticleBoxSize()))
			return false;
		if (!createShader())
			return false;
		createTexture(config->getWeatherTexture());
		initParameter();

		return true;
	}
	/*
	*/
	void cWeather::finalize()
	{
		safeDelete(m_particle);
		safeDelete(m_shader);
	}
	/*
	*/
	bool cWeather::createParticle(int particleNum, float particleBoxSize)
	{
		assert(!m_particle);

		m_particleBox = particleBoxSize;

		BUID buid = _T("weatherParticle");
		m_particle = new cEntityMesh;
		cEntityModel* model = dynamicCast<cEntityModel*>(m_particle);
		model->initialize(buid);

		sCullFigure cullFigure;
		cullFigure.m_box.set(0.0f, m_particleBox, 0.0f, m_particleBox, 0.0f, m_particleBox);
		cullFigure.m_sphere.m_radius = m_particleBox;
		cullFigure.copyToBackup();
		m_particle->createRenderData(&cullFigure, _T(""), _T(""), false, false);

		if (!_getMeshCreater()->createWeatherParticle(buid, particleNum, m_particleBox))
			return false;

		return true;
	}
	/*
	*/
	bool cWeather::createShader()
	{
		assert(!m_shader);

		m_shader = new cWeatherShader;
		if (!m_shader->createEffect(_T("weather.fx"), cD3DSystem::getD3DDevice(), cShader::makeFlags()))
			return false;

		m_shader->setTexture(m_shader->m_diffuseTexture, _getTextureMgr()->getDefault());

		return true;
	}
	/*
	*/
	void cWeather::createTexture(TCHAR const* texName)
	{
		std_string filename(texName);
		_getTextureMgr()->loadTexture(filename);
		m_texBuid = cTexture::makeBUID(filename);
	}
	/*
	*/
	void cWeather::initParameter()
	{
		m_noise = V3(0.0f, 10000.0f, 20000.0f);
		m_lighting = V4(1.0f, 1.0f, 1.0f, 1.0f);

		//setParameterType(HEAVY_SNOW);

		for (int i = 0; i < PASS_NUM; ++i)
		{
			sWeatherData* data = m_data + i;
			data->m_speedScale = 0.75f + cMath::rand(0.5f, 1000.0f);
			data->m_offset = sVector3(0.0f, 0.0f, 0.0f);
			data->m_wind = sVector3(0.0f, 0.0f, 0.0f);
			data->m_random.x = cMath::rand(m_particleBox, 100.0f);
			data->m_random.y = cMath::rand(m_particleBox, 100.0f);
			data->m_random.z = cMath::rand(m_particleBox, 100.0f);
		}

		setDensity(20.0f);
	}
	/*
	*/
	void cWeather::setParameterType(int type)
	{
		switch (type)
		{
		case LIGHT_RAIN:		m_param.m_density = 3.0f;
								m_param.m_speed = 4.5f;
								m_param.m_length = 0.15f;
								m_param.m_width = 0.02f;
								m_param.m_alpha = 0.10f;
								m_param.m_wind = 1.0f;
								break;
		case HEAVY_RAIN:		m_param.m_density = 10.0f;
								m_param.m_speed = 8.0f;
								m_param.m_length = 0.25f;
								m_param.m_width = 0.01f;
								m_param.m_alpha = 0.15f;
								m_param.m_wind = 6.0f;
								break;
		case LIGHT_SNOW:		m_param.m_density = 5.0f;
								m_param.m_speed = 1.5f;
								m_param.m_length = 0.03f;
								m_param.m_width = 0.03f;
								m_param.m_alpha = 0.3f;
								m_param.m_wind = 1.0f;
								break;
		case HEAVY_SNOW:		m_param.m_density = 15.0f;
								m_param.m_speed = 1.5f;
								m_param.m_length = 0.03f;
								m_param.m_width = 0.03f;
								m_param.m_alpha = 0.3f;
								m_param.m_wind = 1.0f;
								break;
		case DEFAULT:			m_param.m_density = 5.0f;
								m_param.m_speed = 8.0f;
								m_param.m_length = 0.5f;
								m_param.m_width = 0.05f;
								m_param.m_alpha = 0.5f;
								m_param.m_wind = 0.0f;
								break;
		}
	}
	/*
	*/
	void cWeather::setType(int type)
	{
		m_type = type;
		setParameterType(type);
	}
	/*
	*/
	void cWeather::update(float elapsedtime)
	{
		if (CLEAR == m_type)
		{
		}
		else
		{
			updateData(elapsedtime);
			updateShader(elapsedtime);
		}
	}
	/*
	*/
	void cWeather::updateData(float elapsedtime)
	{
		float s = elapsedtime * 5000.0f;
		m_noise.x += s;
		m_noise.y += s;
		m_noise.z += s;

		PerlinNoiseDefault p_noise;
		sVector3 noise;
		noise.x = p_noise.SoftNoise1D((int)m_noise.x);
		noise.y = p_noise.SoftNoise1D((int)m_noise.y) * 0.5f;
		noise.z = p_noise.SoftNoise1D((int)m_noise.z);

		#pragma omp parallel
		{
			#pragma omp for
			for (int i = 0; i < m_pass; ++i)
			{
				updateData(elapsedtime, noise, i);
			}
		}
	}
	/*
	*/
	void cWeather::updateData(float elapsedtime, sVector3 const& noise, int pass)
	{
		sVector3 temp;
		sWeatherData* data = m_data + pass;
		// gravity
		float gravity = m_param.m_speed * elapsedtime * data->m_speedScale;
		// wind
		mulVec3(noise, m_param.m_wind, data->m_wind);
		// offset
		mulVec3(data->m_wind, elapsedtime, temp);
		data->m_offset += temp;
		data->m_offset.y += gravity;
		// wrap offset
		cMath::wrap(data->m_offset.x, 0.0f, m_particleBox);
		cMath::wrap(data->m_offset.y, 0.0f, m_particleBox);
		cMath::wrap(data->m_offset.z, 0.0f, m_particleBox);
	}
	/*
	*/
	void cWeather::updateShader(float elapsedtime)
	{
		cCamera* cam = _getCameraMgr()->getMain();
		// camera position
		sVector3 camPos(cam->getEye().x,
						cam->getEye().y,
						cam->getEye().z);

		// wvp wvp_prev
		sMatrix4 vp(cam->getVP());
		m_shader->setMatrix(m_shader->m_matWVP, &vp);
		// 현재 쓰이지 않고 있다.
		//m_shader->setMatrix(m_shader->m_matWVPPrev, &m_wvpPrev);
		//m_wvpPrev = vp;

		// scale size
		float lengthScale = (m_param.m_length/m_param.m_speed)/elapsedtime;
		float verticalZoom = cam->getProj().m[1][1]/1.778341f;
		float width = m_param.m_width * verticalZoom;
		sVector3 sizeScale(width, lengthScale, 0.0f);
		m_shader->setVector(m_shader->m_sizeScale, &sizeScale);

		// camera pos
		m_shader->setVector(m_shader->m_viewPosition, &camPos);

		// lighting
		m_shader->setVector(m_shader->m_lighting, &m_lighting);

		// forward
		float boxOffset = m_particleBox * 0.5f;
		sMatrix4 matView = cam->getView();
		sVector3 forward(matView.m[0][2] * boxOffset, matView.m[1][2] * boxOffset, matView.m[2][2] * boxOffset);
		m_shader->setVector(m_shader->m_forward, &forward);

		float alpha = m_param.m_density;
		for (int i = 0; i < m_pass; ++i)
		{
			sWeatherData* data = m_data + i;
			// offset
			addVec3(data->m_random, data->m_offset, data->m_tempOffset);
			// velocity
			data->m_tempVelocity.x = data->m_wind.x * elapsedtime;
			data->m_tempVelocity.y = data->m_wind.y * elapsedtime + data->m_speedScale * m_param.m_speed * elapsedtime;
			data->m_tempVelocity.z = data->m_wind.z * elapsedtime;
			// alpha
			if (1.0f < alpha)	alpha = 1.0f;
			data->m_tempAlpha = alpha * m_param.m_alpha * m_alphaFade;
		}
	}
	/*
	*/
	void cWeather::render()
	{
		if (CLEAR == m_type)
		{
		}
		else
		{
			_getRenderPropertyMgr()->begin();
			_getRenderPropertyMgr()->setProperty(sRenderProperty::ZWRITEENABLE, FALSE);
			_getRenderPropertyMgr()->setProperty(sRenderProperty::ALPHABLENDENABLE, TRUE);
			_getRenderPropertyMgr()->setProperty(sRenderProperty::ALPHATESTENABLE, TRUE);
			_getRenderPropertyMgr()->setProperty(sRenderProperty::SRCBLEND, D3DBLEND_ONE);
			_getRenderPropertyMgr()->setProperty(sRenderProperty::DESTBLEND, D3DBLEND_ONE);
			_getRenderPropertyMgr()->setProperty(sRenderProperty::CULLMODE, D3DCULL_NONE);

			m_shader->setTexture(m_shader->m_diffuseTexture, _getTextureMgr()->getTexture(m_texBuid));

			for (int i = 0; i < m_pass; ++i)
			{
				sWeatherData* data = m_data + i;
				m_shader->setVector(m_shader->m_positionOffset, &data->m_tempOffset);
				m_shader->setVector(m_shader->m_velocity, &data->m_tempVelocity);
				m_shader->setFloat(m_shader->m_alpha, data->m_tempAlpha);

				_getSceneMgr()->renderEntitySingleShader(m_shader, _T("T"), m_particle);
			}

			_getRenderPropertyMgr()->end();
		}
	}
	/*
	*/
	void cWeather::setDensity(float density)
	{
		m_param.m_density = density;
		m_pass = (int)density;
	}
}