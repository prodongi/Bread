
#ifndef _BreadUberShaderFragment_h_
#define _BreadUberShaderFragment_h_

#include "BreadDeclaration.h"

#define _getUberShaderFragment()	Bread::cUberShaderFragment::getInstance()

namespace Bread
{
#define FRAGMENT_NONE			_T("none")
#define FRAGMENT_BASE			_T("base")
#define FRAGMENT_SPECULAR		_T("specular")
#define FRAGMENT_BUMP			_T("bump")
#define FRAGMENT_DEFERRED		_T("deferred")
#define FRAGMENT_GPU_ANI		_T("gpu_ani")
#define FRAGMENT_POM			_T("pom")
#define FRAGMENT_VISUAL_NORMAL	_T("visual_normal")
#define FRAGMENT_VEGETATION_ANI	_T("vegetation_ani")

	/*
	*/
	enum FRAGMENT_TYPE
	{
		_NONE_,					// none
		_BASE_,					// base
		_SPECULAR_,				// specular
		_BUMP_,					// bump
		_DEFERRED_,				// deferred
		_GPU_ANI_,				// gpuani
		_POM_,					// parallax occlusion mapping
		_VISUAL_NORMAL_,		// visual normal
		_VEGETATION_ANI_,		// vegetatin animation
		FRAGMENT_NUM,
	};
	/*
		@date 2012.12.
		@auth prodongi
		@desc 
		@todo 
	*/
	class cUberShaderFragment
	{
		declareInstance(cUberShaderFragment)

	public:
		cUberShaderFragment();
		virtual ~cUberShaderFragment();
		virtual void initialize();
		virtual void addDefinitionFragments(std::string const& srcEffectData, uint fragments, std::string& effectData);
		uint stringToFragment(TCHAR const* str);
		bool is(uint fragment, uint fragments);

	protected:
		virtual void addFragments();
		void fragmentToDefinition(uint fragments, std::string& effectData);

	protected:
		struct sFragment
		{
			sFragment(uint flag, std_string const& token, std::string const& definition) : m_flag(flag), m_token(token), m_definition(definition) {}
			uint m_flag;
			std_string m_token;			/// @brief 쉐이더 구분자  (bfx에 설정된 텍스트)
			std::string m_definition;		/// @brief 전처리기
		};

		std::vector<sFragment>	m_list;
	};
}



#endif