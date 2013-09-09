
#ifndef _BreadBucketAtlMap_h_
#define _BreadBucketAtlMap_h_

#include <atlcoll.h>
#include <atlstr.h>
#include <assert.h>

namespace Bread
{
	/*
		@date 2010.00.00
		@auth prodongi
		@desc 
		@todo 
	*/
	template <class KEY, class V, size_t bucketSize>
	class cBucketHash
	{
	private:
		typedef ATL::CAtlMap<CString, V> atl_t;
		typedef typename atl_t::CPair atl_pair;

	public:
		cBucketHash()
		{
			m_atl = new atl_t;
			m_atl->InitHashTable(bucketSize);
		}
		virtual ~cBucketHash()
		{
			safeDelete(m_atl);
		}
		bool is(KEY const& key) const
		{
			atl_pair* atl_p = m_atl->Lookup(key.c_str());
			return (atl_p) ? true : false;
		}
		V const& get(KEY const& key) const
		{
			static V emptyV;
			atl_pair* atl_p = m_atl->Lookup(key.c_str());
			return (atl_p) ? atl_p->m_value : emptyV;
		}
		void get(KEY const& key, V* value) const
		{
			atl_pair* atl_p = m_atl->Lookup(key.c_str());
			if (atl_p)
				*value = atl_p->m_value;
		}
		V const& getFirst() const
		{
			atl_pair* atl_p;
			POSITION pos = m_atl->GetStartPosition();
			if (pos)
			{
				atl_p = m_atl->GetNext(pos);
				return atl_p->m_value;
			}
			return NULL;
		}
		bool insert(KEY const& key, V t, bool setNull = false)
		{
			if (is(key))
			{
				assert(0 && "Failed AtlMap insert : exist key value");
				return false;
			}
			unreferencedParameter(setNull);
			m_atl->SetAt(key.c_str(), t);
			return true;
		}
		bool remove(KEY const& key)
		{
			if (!is(key))
				return false;
			return m_atl->RemoveKey(key.c_str());
		}
		template <class Fn> void forEach(Fn func)
		{
			atl_pair* atl_p;
			POSITION pos = m_atl->GetStartPosition();
			while (pos)
			{
				atl_p = m_atl->GetNext(pos);
				func(atl_p->m_value);
			}
		}
		size_t getBucketSize() const	{	return m_atl->GetHashTableSize();	}
		size_t getCount() const			{	return m_atl->GetCount();			}

	protected:
		atl_t* m_atl;
	};
	/*
		@date 2010.00.00
		@auth prodongi
		@desc 
		@todo 
	*/
	template <class KEY, class V, size_t bucketSize>
	class cBucketHash<KEY, V*, bucketSize>
	{
	private:
		typedef ATL::CAtlMap<CString, V*> atl_t;
		typedef typename atl_t::CPair atl_pair;

	public:
		cBucketHash()
		{
			m_atl = new atl_t;
			m_atl->InitHashTable(bucketSize);
		}
		virtual ~cBucketHash()
		{
			atl_pair* atl_p;
			POSITION pos = m_atl->GetStartPosition();
			while (pos)
			{
				atl_p = m_atl->GetNext(pos);
				safeDelete(atl_p->m_value);
			}
			safeDelete(m_atl);
		}
		bool is(KEY const& key) const
		{
			atl_pair* atl_p = m_atl->Lookup(key.c_str());
			return (atl_p) ? true : false;
		}
		V* get(KEY const& key) const
		{
			atl_pair* atl_p = m_atl->Lookup(key.c_str());
			return (atl_p) ? atl_p->m_value : NULL;
		}
		V* getFirst() const
		{
			atl_pair* atl_p;
			POSITION pos = m_atl->GetStartPosition();
			if (pos)
			{
				atl_p = m_atl->GetNext(pos);
				return atl_p->m_value;
			}
			return NULL;
		}
		bool insert(KEY const& key, V** t, bool setNull = false)
		{
			if (is(key))
			{
				assert(0 && "Failed AtlMap insert : exist key value");
				return false;
			}
			m_atl->SetAt(key.c_str(), *t);
			if (setNull)
				*t = NULL;

			return true;
		}
		V* create(KEY const& key)
		{
			if (is(key))
			{
				return NULL;
			}

			return new V;
		}
		bool remove(KEY const& key)
		{
			V* v = get(key);
			if (!v)
				return false;
			safeDelete(v);
			return m_atl->RemoveKey(key.c_str());
		}
		V* create()
		{
			return new V;
		}
		template <class Fn> void forEach(Fn func)
		{
			atl_pair* atl_p;
			POSITION pos = m_atl->GetStartPosition();
			while (pos)
			{
				atl_p = m_atl->GetNext(pos);
				func(atl_p->m_value);
			}
		}
		size_t getBucketSize() const	{	return m_atl->GetHashTableSize();	}
		size_t getCount() const			{	return m_atl->GetCount();			}

	protected:
		atl_t* m_atl;
	};

	/*
	*/
	template <class V, size_t bucketSize>
	class cBucketHash<unsigned int, V, bucketSize>
	{
	private:
		typedef unsigned int KEY;
		typedef ATL::CAtlMap<KEY, V> atl_t;
		typedef typename atl_t::CPair atl_pair;

	public:
		cBucketHash()
		{
			m_atl = new atl_t;
			m_atl->InitHashTable(bucketSize);
		}
		virtual ~cBucketHash()
		{
			safeDelete(m_atl);
		}
		bool is(KEY const& key) const
		{
			atl_pair* atl_p = m_atl->Lookup(key);
			return (atl_p) ? true : false;
		}
		V get(KEY const& key) const
		{
			atl_pair* atl_p = m_atl->Lookup(key);
			return (atl_p) ? atl_p->m_value : NULL;
		}
		void get(KEY const& key, V* value) const
		{
			atl_pair* atl_p = m_atl->Lookup(key);
			if (atl_p)
				*value = atl_p->m_value;
		}
		V getFirst() const
		{
			atl_pair* atl_p;
			POSITION pos = m_atl->GetStartPosition();
			if (pos)
			{
				atl_p = m_atl->GetNext(pos);
				return atl_p->m_value;
			}
			return NULL;
		}
		bool insert(KEY const& key, V t, bool setNull = false)
		{
			if (is(key))
			{
				assert(0 && "Failed AtlMap insert : exist key value");
				return false;
			}
			unreferencedParameter(setNull);
			m_atl->SetAt(key, t);
			return true;
		}
		bool remove(KEY const& key)
		{
			if (!is(key))
				return false;
			return m_atl->RemoveKey(key);
		}
		template <class Fn> void forEach(Fn func)
		{
			atl_pair* atl_p;
			POSITION pos = m_atl->GetStartPosition();
			while (pos)
			{
				atl_p = m_atl->GetNext(pos);
				func(atl_p->m_value);
			}
		}
		size_t getBucketSize() const	{	return m_atl->GetHashTableSize();	}
		size_t getCount() const			{	return m_atl->GetCount();			}

	protected:
		atl_t* m_atl;
	};
}



#endif