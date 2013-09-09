
#ifndef _BreadHelperClass_h_
#define _BreadHelperClass_h_

namespace Bread
{
	/*
	*	
	*/
	template <class T>
	struct sUpdateTime
	{
		sUpdateTime() : m_active(false), m_loop(false), m_toggle(false), m_toggleDir(true)
		{
			m_elapsedtime = (T)0;
			m_upateTime = (T)0;
		}
		void begin(T updateTime, bool loop = false)
		{
			m_active = true;
			m_loop = loop;
			m_elapsedtime = (T)0;
			m_upateTime = updateTime;
		}
		void end()
		{
			m_active = false;
		}
		T getRate() const
		{
			if (m_upateTime < (T)0) return (T)0;
			return min(m_elapsedtime / m_upateTime, (T)1);
		}
		bool update(T elapsedtime)
		{
			if (!m_active)
				return false;

			if (m_toggle)
			{
				// inc
				if (m_toggleDir)    
				{
					m_elapsedtime += elapsedtime;
					if (m_elapsedtime >= m_upateTime)
					{
						m_toggleDir = false;
						if (m_loop)    m_elapsedtime = m_upateTime;
						else        end();
						return false;
					}
				}
				// dec
				else
				{
					m_elapsedtime -= elapsedtime;
					if (m_elapsedtime <= (T)0)
					{
						m_toggleDir = true;
						if (m_loop)    m_elapsedtime = (T)0;
						else        end();
						return false;
					}
				}
			}
			else
			{
				m_elapsedtime += elapsedtime;
				if (m_elapsedtime >= m_upateTime)
				{
					if (m_loop)    m_elapsedtime -= m_upateTime;
					else        end();
					return false;
				}
			}

			return true;
		}
		bool m_active;
		bool m_loop;
		T m_elapsedtime;
		T m_upateTime;

		// toggle
		bool m_toggle;
		// toggle πÊ«‚ true : inc, false : dec
		bool m_toggleDir;
	};


	/*
	*	
	*/
	class cStdMapHelperClear_p
	{
	public:
		cStdMapHelperClear_p() {};
		~cStdMapHelperClear_p() {};
		template <typename TKey, typename TValue>
		void clear(std::map<TKey, TValue>& list)
		{
			safeClearMap<std::map<TKey, TValue>::iterator>(list);
		}
	};
	/*
	 *	
	 */
	class cStdMapHelperClear
	{
	public:
		cStdMapHelperClear() {};
		~cStdMapHelperClear() {};
		template <typename TKey, typename TValue>
		void clear(std::map<TKey, TValue>& list)
		{
			list.clear();	
		}
	};
	/*
	 *	
	 */
	template <typename TKey, typename TValue, typename TClear>
	class cStdMapHelper
	{
	public:
		typedef std::map<TKey, TValue> map_;
		typedef typename map_::iterator it_;

		cStdMapHelper() {};
		~cStdMapHelper()
		{
			m_clear.clear<TKey, TValue>(m_list);
		}
		bool get(TKey const& key, TValue& value)
		{
			it_ it = m_list.find(key);
			if (it != m_list.end())
			{
				value = it->second;
				return true;
			}
			return false;
		}
		TValue const& get(TKey const& key)
		{
			it_ it = m_list.find(key);
			if (it != m_list.end())
			{
				return it->second;
			}

			static TValue tvalue;
			return tvalue;
		}
		bool is(TKey const& key)
		{
			it_ it = m_list.find(key);
			if (it == m_list.end())
				return false;
			return true;
		}
		void insert(TKey const& key, TValue const& value)
		{
			if (is(key))
				return ;
			m_list.insert(std::make_pair(key, value));
		}
		void erase(TKey const& key)
		{
			m_list.erase(key);
		}
	public:
		map_ m_list;
		TClear m_clear;
	};

	/*
	 *	
	 */
	template <typename T>
	struct sUpdateFunctor
	{
		sUpdateFunctor(float elapsedtime) : m_elapsedtime(elapsedtime) {}
		void operator ()(T* t)
		{
			t->update(m_elapsedtime);
		}
		float m_elapsedtime;
	};
}





#endif