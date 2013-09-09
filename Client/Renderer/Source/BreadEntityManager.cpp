
#include "BreadEntityManager.h"


namespace Bread
{
	/*
	*/
	struct sEntityManagerUpdageFunc
	{
		sEntityManagerUpdageFunc(float elapsedtime) : m_elapsedtime(elapsedtime) {}

		void operator ()(cEntityModel* model)
		{
			model->update(m_elapsedtime);
		}
		float m_elapsedtime;
	};

	/*
	 *	
	 */
	implementInstance(cEntityManager)

	/*
	 *	
	 */
	cEntityManager::cEntityManager()
	{
		assignInstance(this)
	}
	/*
	 *	
	 */
	cEntityManager::~cEntityManager()
	{
		assignInstance(NULL)
	}
	/*
	 *	
	 */
	void cEntityManager::update(float elapsedtime)
	{
		forEach(sEntityManagerUpdageFunc(elapsedtime));
	}
	/*
	*/
	cEntityAvatar* cEntityManager::getAvatar() const
	{
		if (m_avatarBuid.empty())
			return NULL;
		return dynamicCast<cEntityAvatar*>(get(m_avatarBuid));
	}
}















