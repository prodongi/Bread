
#ifndef _BreadEntityManager_h_
#define _BreadEntityManager_h_

#include <assert.h>
#include "BreadBucketHash.h"
#include "BreadEntityModel.h"
#include "BreadEntityAvatar.h"

#define _getEntityMgr()	Bread::cEntityManager::getInstance()

namespace Bread
{
	#define ENTITYMANAGER_BUCKET_SIZE	1000

	/**
	 * \ingroup Renderer
	 *
	 *
	 * \par requirements
	 * win98 or later\n
	 * win2k or later\n
	 * MFC\n
	 *
	 * \version 1.0
	 * first version
	 *
	 * \date 2009-10-02
	 *
	 * \author Prodongi
	 *
	 * \par license
	 * This code is absolutely free to use and modify. The code is provided "as is" with
	 * no expressed or implied warranty. The author accepts no liability if it causes
	 * any damage to your computer, causes your pet to fall ill, increases baldness
	 * or makes your car start emitting strange noises when you start it up.
	 * This code has no bugs, just undocumented features!
	 * 
	 * \todo 
	 *
	 * \bug 
	 *
	 */
	class cEntityManager : public cBucketHash<BUID, cEntityModel*, ENTITYMANAGER_BUCKET_SIZE>
	{
		declareInstance(cEntityManager)
		declareClassCreater(cEntityManager)

	public:
		virtual void update(float elapsedtime);
		cEntityAvatar* getAvatar() const;
		BUID const& getAvatarBuid() const	{	return m_avatarBuid;	}

	protected:
		//
		template <typename T>
		cEntityModel* createEntity(BUID const& buid, BUID const& renderDataBuid, BUID const& aniDataBuid, 
								   sVector3 const& pos, float radian, bool isSceneObject)
		{
			if (is(buid))
				return NULL;
			cEntityModel* entity = new T;
			entity->initialize(buid, pos, radian, renderDataBuid, aniDataBuid, isSceneObject);
			insert(buid, &entity);

			return entity;
		}
		//
		template <typename T>
		cEntityModel* createEntity(BUID const& buid)
		{
			if (is(buid))
				return NULL;
			cEntityModel* entity = new T;
			entity->initialize(buid);
			insert(buid, &entity);

			return entity;
		}
		// @brief createEntity + registAvatar를 수동으로 호출해 줘도 된다.
		template <typename T>
		cEntityModel* createAvatar(BUID const& buid)
		{
			cEntityModel* entity = createEntity<T>(buid);
			if (entity)
				registAvatar(buid);

			return entity;
		}
		// @brief 아바타 등록, createAvatar를 호출하면 registAvatar를 따로 호출 안 해줘도 된다.
		void registAvatar(BUID const& buid)	{	m_avatarBuid = buid;	}
		
	protected:
		BUID m_avatarBuid;
		friend class cSceneManager;
	};
}









#endif