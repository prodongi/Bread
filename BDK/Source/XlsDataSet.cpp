
#include "XlsDataSet.h"
#include "XlsData.h"
#include <Windows.h>


namespace Bread
{

	/*
	*	
	*/
	cXlsDataSet::cXlsDataSet() : m_dataRow(NULL)
	{
	}
	/*
	*	
	*/
	cXlsDataSet::~cXlsDataSet()
	{
		if (m_dataRow)
		{
			delete[] m_dataRow;
			m_dataRow = NULL;
		}
	}
	/*
	 *	
	 */
	bool cXlsDataSet::load(const char* fileName)
	{
		return true;
	}





}




