
#ifndef _BreadCloud_h_
#define _BreadCloud_h_

namespace Bread
{
	class cVolumeCloud;

	/*
		@date 2012.
		@auth prodongi
		@desc 
		@todo 
	*/
	class cCloud
	{
	public:
		cCloud();
		virtual ~cCloud();
		virtual bool initialize();
		virtual void finalize();
		virtual void render();

	protected:
		cVolumeCloud* m_volumeCloud;

	};
}



#endif