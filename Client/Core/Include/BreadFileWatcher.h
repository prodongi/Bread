
#ifndef _BreadFileWatcher_h_
#define _BreadFileWatcher_h_

namespace Bread
{
	/*
		@date 2012.01.19
		@auth prodongi
		@desc ���� �� ���� ����,
				 ���ҽ� �������� ������ ��ȭ�� �����Ǹ� ���� ���ε� �ϴ� �� ���� ����
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