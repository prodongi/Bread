
#ifndef _XlsConvertor_h_
#define _XlsConvertor_h_

#include <map>


namespace Bread
{
	class cXlsDataSet;

	/**
	 * \class cXlsManager
	 *
	 * \date 2008-11-20
	 *
	 * \author prodongi
	 */
	class cXlsManager
	{
	public:
		cXlsManager();
		virtual ~cXlsManager();
		bool load(const char* fileName, int id);
		void clear();

	protected:
		typedef std::map<int, cXlsDataSet*>	map_xlsdataset;
		
		map_xlsdataset* m_dataSet;
	};
}











#endif