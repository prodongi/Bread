
#ifndef _BreadShader_h_
#define _BreadShader_h_

#include "BreadDeclaration.h"
#include "BreadD3DHeader.h"
#include "BreadHelperClass.h"

namespace Bread
{
	class cMesh;
	class cMaterial;
	class cEntityModel;
	class cTexture;
	class cShaderEffect;
	struct sMatrix4;
	struct sVector2;
	struct sVector3;
	struct sVector4;
	
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
	* \date 2009-10-20
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
	class cShader
	{
	public:
		cShader();
		virtual ~cShader();
		virtual void setMaterialParam(cMaterial const* /*mtl*/) {}
		virtual void setMeshParam(cMesh const* /*mesh*/, cEntityModel const* /*entity*/) {}
		virtual void update(float /*elapsedtime*/, cMesh* /*mesh*/, cMaterial const* /*mtl*/, cEntityModel* /*entity*/) {}
		virtual void setDefaultTexture() {}
		
		bool createEffect(std_string const& filename, LPDIRECT3DDEVICE9 device, DWORD flags, LPD3DXEFFECTPOOL pool = NULL);
		void begin(uint* pass);
		void end();
		void beginPass(uint p);
		void endPass();
		void lostDevice();
		void resetDevice();
		D3DXHANDLE insertTechnique(TCHAR const* name);
		D3DXHANDLE insertParameter(TCHAR const* name);
		void setTechnique(TCHAR const* name);
		void setMatrix(TCHAR const* name, D3DXMATRIXA16 const* mat);
		void setMatrix(TCHAR const* name, sMatrix4 const* mat);
		void setMatrixArray(TCHAR const* name, D3DXMATRIXA16 const* mat, uint num);
		void setMatrixArray(TCHAR const* name, sMatrix4 const* mat, uint num);
		void setVector(TCHAR const* name, D3DXVECTOR2 const* vec);
		void setVector(TCHAR const* name, D3DXVECTOR3 const* vec);
		void setVector(TCHAR const* name, D3DXVECTOR4 const* vec);
		void setVector(TCHAR const* name, sVector2 const* vec);
		void setVector(TCHAR const* name, sVector3 const* vec);
		void setVector(TCHAR const* name, sVector4 const* vec);
		void setVectorArray(TCHAR const* name, D3DXVECTOR4 const* vec, uint num);
		void setFloat(TCHAR const* name, float f);
		void setFloatArray(TCHAR const* name, float const* f, uint num);
		void setInt(TCHAR const* name, int i);
		void setIntArray(TCHAR const* name, int const* i, uint num);
		void setBool(TCHAR const* name, BOOL b);
		void setValue(TCHAR const* name, void* value, int size);
		void setTexture(TCHAR const* name, cTexture const* tex);
		void setTechnique(D3DXHANDLE handle);
		void setMatrix(D3DXHANDLE handle, D3DXMATRIXA16 const* mat);
		void setMatrix(D3DXHANDLE handle, sMatrix4 const* mat);
		void setMatrixArray(D3DXHANDLE handle, D3DXMATRIXA16 const* mat, uint num);
		void setMatrixArray(D3DXHANDLE handle, sMatrix4 const* mat, uint num);
		void setVector(D3DXHANDLE handle, D3DXVECTOR2 const* vec);
		void setVector(D3DXHANDLE handle, D3DXVECTOR3 const* vec);
		void setVector(D3DXHANDLE handle, D3DXVECTOR4 const* vec);
		void setVector(D3DXHANDLE handle, sVector2 const* vec);
		void setVector(D3DXHANDLE handle, sVector3 const* vec);
		void setVector(D3DXHANDLE handle, sVector4 const* vec);
		void setVectorArray(D3DXHANDLE handle, D3DXVECTOR4 const* vec, uint num);
		void setFloat(D3DXHANDLE handle, float f);
		void setFloatArray(D3DXHANDLE handle, float const* f, uint num);
		void setInt(D3DXHANDLE handle, int i);
		void setIntArray(D3DXHANDLE handle, int const* i, uint num);
		void setBool(D3DXHANDLE handle, BOOL b);
		void setValue(D3DXHANDLE handle, void* value, int size);
		void setTexture(D3DXHANDLE handle, cTexture const* tex);

		BUID const& getBuid() const	{	return m_buid;	}
		propertyFunc(void*, UserData, m_userData)

		static DWORD makeFlags();

	protected:
		virtual void registHandle() {}
		virtual bool initialize() {	return true;	}

	private:
		static TCHAR const* makeBUID(std_string const& filename);

	protected:
		BUID m_buid;
		std_string m_filename;
		cShaderEffect* m_effect;
		void* m_userData;
		cStdMapHelper<std_string, D3DXHANDLE, cStdMapHelperClear> m_handle;
	};
}









#endif