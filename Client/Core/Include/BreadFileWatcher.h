
#ifndef _BreadFileWatcher_h_
#define _BreadFileWatcher_h_

namespace Bread
{
	/*
		@date 2012.01.19
		@auth prodongi
		@desc 개발 할 때만 쓰임,
				 리소스 폴더에서 파일의 변화가 감지되면 새로 리로딩 하는 데 쓰일 예정
		@todo 
	*/
#ifdef _DEBUG
	class cFileWatcher
	{
	public:
		cFileWatcher();
		~cFileWatcher();
		void initialize();
		void update();
	};
#else
	class cFileWatcher
	{
	public:
		void initialize() {}
		void update() {}
	};
#endif
}



#endif