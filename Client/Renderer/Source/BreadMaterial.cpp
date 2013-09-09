
#include "BreadMaterial.h"
#include "BreadUberShaderFragment.h"

namespace Bread
{
	cMaterial* cMaterial::m_emptyMtl = NULL;
	/*
	*/
	cMaterial const* cMaterial::getEmptyMtl()
	{
		return m_emptyMtl;
	}
	/*
	*/
	void cMaterial::createEmptyMtl()
	{
		assert(!m_emptyMtl);
		m_emptyMtl = new cMaterial;

		m_emptyMtl->m_buid = _T("empty");
		m_emptyMtl->m_diffuse = sVector3(1.0f, 1.0f, 1.0f);
		m_emptyMtl->m_specular = sVector3(1.0f, 1.0f, 1.0f);
		m_emptyMtl->m_ambient = sVector3(1.0f, 1.0f, 1.0f);
		m_emptyMtl->m_opacity = 1.0f;
		m_emptyMtl->m_glossiness = 1.0f;
		m_emptyMtl->m_specularLevel = 1.0f;
		m_emptyMtl->m_shaderBuid = _T("");
	}
	/*
	*/
	void cMaterial::deleteEmptyMtl()
	{
		safeDelete(m_emptyMtl);
	}


	/*
	 *	
	 */
	cMaterial::cMaterial()
	{
		for (int i = 0; i < MAPNUM; ++i)
			m_filename[i][0] = 0x00;
	}
	/*
	 *	
	 */
	cMaterial::~cMaterial()
	{

	}
	/*
	*/
	void cMaterial::copy(cMaterial& copyData, uint /*shareType*/)
	{
		copyData.m_buid			= m_buid;
		copyData.m_diffuse		= m_diffuse;
		copyData.m_specular		= m_specular;
		copyData.m_ambient		= m_ambient;
		copyData.m_opacity		= m_opacity;
		copyData.m_glossiness	= m_glossiness;
		copyData.m_specularLevel = m_specularLevel;
		copyData.m_shaderBuid	= m_shaderBuid;
		copyData.m_meshList.assign(m_meshList.begin(), m_meshList.end());
		for (int i = 0; i < MAPNUM; ++i)
		{
			_tcsncpy(copyData.m_filename[i], m_filename[i], NAME_LEN);
			copyData.m_mapBuid[i] = m_mapBuid[i];
		}
	}
}