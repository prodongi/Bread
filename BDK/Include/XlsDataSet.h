
#ifndef _XlsDataSet_h_
#define _XlsDataSet_h_





namespace Bread
{
	class cXlsData;

	/**
	 * \class cXlsDataSet
	 *
	 * \date 2008-11-20
	 *
	 * \author prodongi
	 */
	class cXlsDataSet
	{
	public:
		cXlsDataSet();
		~cXlsDataSet();
		bool load(const char* fileName);

	private:
		cXlsData*	m_dataRow;

	};
}











#endif