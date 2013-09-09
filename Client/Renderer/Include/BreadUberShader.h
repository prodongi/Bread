
#ifndef _BreadUberShader_h_
#define _BreadUberShader_h_

#include "BreadShader.h"

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
	 * \date 2010-03-11
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
	class cUberShader : public cShader
	{
	public:
		cUberShader();
		virtual ~cUberShader();
		virtual void setMaterialParam(cMaterial const* mtl);
		virtual void setMeshParam(cMesh const* mesh, cEntityModel const* entity);
		virtual void setDefaultTexture();
		virtual void beginRenderProperty() {}
		virtual void endRenderProperty() {}

		/// @brief create combination shader effect
		virtual bool createEffect(std_string const& type, std::string const& srcEffectData, LPDIRECT3DDEVICE9 device, DWORD flags, uint fragments, LPD3DXEFFECTPOOL pool = NULL);

		static TCHAR const* makeBUID(std_string const& type, uint fragments);
		static TCHAR const* makeBUID(std_string const& type, std_string const& fragments);

	protected:
		virtual void registHandle();
		virtual void shareRegistHandle();

	protected:
		uint m_fragments;
		D3DXHANDLE* m_handle;
	};

	/*
	*/
	cUberShader* createUberShader();
}














#endif


















