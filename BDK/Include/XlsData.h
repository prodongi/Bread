
#ifndef _XlsData_h_
#define _XlsData_h_





namespace Bread
{
	/**
	* \class cXlsDataSet
	*
	* \date 2008-11-20
	*
	* \author prodongi
	*/
	class cXlsData
	{
	public:
		cXlsData();
		~cXlsData();
		bool load(const char* fileName);
	};
}











#endif