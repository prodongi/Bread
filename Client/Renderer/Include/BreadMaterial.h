
#ifndef _BreadMaterial_h_
#define _BreadMaterial_h_

#include "BreadDeclaration.h"
#include "BreadVector3.h"

namespace Bread
{
	/**
	 * \ingroup Renderer
	 *
	 *
	 * \par requirements
	 * win98 or later\n
	 * win2k or later\n
	 * MFC\n
	 *
	 * \version 1.0
	 * first version
	 *
	 * \date 2009-09-30
	 *
	 * \author Prodongi
	 *
	 * \par license
	 * This code is absolutely free to use and modify. The code is provided "as is" with
	 * no expressed or implied warranty. The author accepts no liability if it causes
	 * any damage to your computer, causes your pet to fall ill, increases baldness
	 * or makes your car start emitting strange noises when you start it up.
	 * This code has no bugs, just undocumented features!
	 * 
	 * \todo 
	 *
	 * \bug 
	 *
	 */
	class cMaterial
	{
	public:
		cMaterial();
		~cMaterial();
		bool isAlpha() const	{	return m_filename[OPACITY][0] != 0x00;	}
		void copy(cMaterial& copyData, uint shareType);

		static cMaterial const* getEmptyMtl();
		static void createEmptyMtl();
		static void deleteEmptyMtl();

	public:
		enum	{	AMBIENT, DIFFUSE, SPECULAR, SPECULARLEVEL, GLOSSINESS, SELFILLUMINATION, 
					OPACITY, FILTER, BUMP, REFLECTION, REFRACTION, DISPLACEMENT, MAPNUM };

		typedef std::vector<BUID>	vec_mesh;
		typedef vec_mesh::iterator	iter_mesh;
		typedef vec_mesh::const_iterator	citer_mesh;

		/// @brief buid는 material의 이름과 같다
		BUID		m_buid;
		sVector3	m_diffuse;
		sVector3	m_specular;
		sVector3	m_ambient;
		float		m_opacity;
		float		m_glossiness;
		float		m_specularLevel;
		TCHAR		m_filename[MAPNUM][NAME_LEN];
		BUID		m_mapBuid[MAPNUM];
		BUID		m_shaderBuid;
		vec_mesh	m_meshList;

	private:
		static cMaterial* m_emptyMtl;
	};
}













#endif