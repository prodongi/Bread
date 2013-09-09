
#ifndef _BreadDebug_h_
#define _BreadDebug_h_

#include "BreadTrace.h"
#include "BreadD3DHeader.h"

namespace Bread
{
	/// 이 함수를 호출 하는 것은 비용이 비싸므로 꼭 필요한 곳에서만 활성화 해야 된다.
	void enableFPExeptions();

	bool checkHResultReturn(HRESULT hr);
	void checkHResult(HRESULT hr);

	enum {	RENDER_SCENE,
			RENDER_SCENE_OCTREE,
			RENDER_MESH_CULLFIGURE,
			RENDER_BONE,
			RENDER_GRID,
			RENDER_TERRAIN,
			RENDER_PICK,
			RENDER_WIRE,
			RENDER_NORMAL,
			RENDER_TANGENT,
			RENDER_BINORMAL,
			RENDER_RENDERLIST_CULLFIGURE,
			RENDER_PROFILE,
			RENDER_DEBUG_INFO,
			RENDER_FPS,
			RENDER_MOVE_PROXY_PATH,
			RENDER_END
		};
	/*
	*/
	class cRenderChannel
	{
	public:
		enum {	MAX_CHANNEL_NUM = 100	};	/// @brief 100로 충분한 공간을 확보해 놓는다
		
#pragma pack(push, 1)
		struct sChannel
		{
			sChannel() : m_is(false), m_value1(0), m_value2(0) {}
			int	 m_value1;
			int	 m_value2;
			bool m_is;
		};
#pragma pack(pop)

		static void initialize();
		static sChannel m_channel[MAX_CHANNEL_NUM];
	};
	#define _setIsRender(channel, is)			\
		assert(channel < cRenderChannel::MAX_CHANNEL_NUM);		\
		cRenderChannel::m_channel[channel].m_is = is
	#define _setIsRenderValue1(channel, value)	\
		assert(channel < cRenderChannel::MAX_CHANNEL_NUM);		\
		cRenderChannel::m_channel[channel].m_value1 = value
	#define _setIsRenderValue2(channel, value)	\
		assert(channel < cRenderChannel::MAX_CHANNEL_NUM);		\
		cRenderChannel::m_channel[channel].m_value2 = value
	#define _getIsRender(channel)				\
		cRenderChannel::m_channel[channel].m_is
	#define _getIsRenderValue1(channel)			\
		cRenderChannel::m_channel[channel].m_value1
	#define _getIsRenderValue2(channel)			\
		cRenderChannel::m_channel[channel].m_value2
}

#endif