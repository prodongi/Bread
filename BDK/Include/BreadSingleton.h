
#ifndef _BreadSingleton_h_
#define _BreadSingleton_h_

namespace Bread
{
#define declareSingleton(CLASS)		\
	private:							\
		static CLASS* m_instance;		\
		CLASS();						\
	public:								\
		static CLASS* getSingleton()	\
		{								\
			if (!m_instance)			\
			{							\
				m_instance = new CLASS;	\
			}							\
			return m_instance;			\
		}								\
		static void release()			\
		{								\
			if (m_instance)				\
				delete m_instance;		\
		}


#define implementSingleton(CLASS)		\
	CLASS* CLASS::m_instance = 0;

}





#endif