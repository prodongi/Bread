
#ifndef _BreadEntityFadeManager_h_
#define _BreadEntityFadeManager_h_

#include "BreadDeclaration.h"
#include "BreadBucketMap.h"

namespace Bread
{
	/*
	*/
	struct sEntityFadeInfo
	{
		enum { FADE_IN, FADE_OUT };

		void setType(uint type);
		void modifyType(uint type)	{	m_type = type;	}	/// @brief 타입만 변화, in <-> out 에서 쓰일 수 있음
		bool update(float elapsedtime, BUID const& buid, float v);

	private:
		uint m_type;
		float m_alpha;
	};
	/*
		@date 2010.00.00
		@auth prodongi
		@desc 
		@todo 
	*/
	class cEntityFadeManager : public cBucketMap<BUID, sEntityFadeInfo>
	{
		declareClassCreater(cEntityFadeManager)

	public:
		virtual void update(float elapsedtime);
		void addFadeIn(BUID const& buid);
		void addFadeOut(BUID const& buid);

	protected:
		void add(BUID const& buid, uint type);

	protected:
		float m_v;	/// @brief fade 속도
	};
}



#endif