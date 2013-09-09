
#include <assert.h>

#include "XlsManager.h"
#include "XlsDataSet.h"


namespace Bread
{
	/*
	*	
	*/
	cXlsManager::cXlsManager()
	{
		m_dataSet = new std::map<int, cXlsDataSet*>;
	}
	/*
	*	
	*/
	cXlsManager::~cXlsManager()
	{
		map_xlsdataset::iterator it = m_dataSet->begin();
		for (; it != m_dataSet->end(); ++it)
			delete it->second;
		m_dataSet->clear();
		delete m_dataSet;
	}
	/*
	*	
	*/
	bool cXlsManager::load(const char* fileName, int id)
	{
		if (!fileName)
		{
			assert(fileName && "filename is NULL");
			return false;
		}

		std::map<int, cXlsDataSet*>::iterator it = m_dataSet->find(id);
		if (it != m_dataSet->end())
		{
			assert(0 && "xls is is duplication");
			return false;
		}

		cXlsDataSet* dataSet = new cXlsDataSet;
		if (dataSet->load(fileName))
			return false;

		m_dataSet->insert(std::make_pair(id, dataSet));

		return true;
	}
	/*
	*	
	*/
	void cXlsManager::clear()
	{
		map_xlsdataset::iterator it = m_dataSet->begin();
		for (; it != m_dataSet->end(); ++it)
			delete it->second;
		m_dataSet->clear();
	}




}















