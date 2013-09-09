
#ifndef _BreadFps_h_
#define _BreadFps_h_


namespace Bread
{
	class cFps
	{
	public:
		cFps() : m_fps(0.0f) {}
		~cFps() {}

		float getFps() const	{	return m_fps;	}
		void update();
	private:
		float m_fps;
	};
}









#endif