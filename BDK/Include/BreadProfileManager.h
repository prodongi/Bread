
#ifndef _BreadProfileManager_h_
#define _BreadProfileManager_h_

#include <tchar.h>
#include "BreadDefinition.h"

typedef void (*callBackProfileRender)(int x, int y, TCHAR const* str);

namespace Bread
{
	/*
	*/
	class cProfileNode
	{
	public:
		cProfileNode(string_id id, cProfileNode* parent);
		~cProfileNode();
		cProfileNode* getSubNode(string_id id);	
		void call();
		bool callOff();
		void reset();
		void print(int x, int y, int depth);

		cProfileNode* getParent() const		{	return m_parent;	}
		cProfileNode* getFirstChild() const	{	return m_firstChild;}
		cProfileNode* getSibling() const	{	return m_sibling;	}
		string_id getId() const				{	return m_id;		}
		int getTotalCall() const			{	return m_totalCall;	}
		float getTotalTime() const			{	return m_totalTime;	}

		static void setRenderCallBack(callBackProfileRender _callBackRender)
		{
			m_callBackRender = _callBackRender;
		}

	private:
		string_id m_id;
		int m_totalCall;
		float m_totalTime;
		__int64 m_startTime;
		int m_recursionCounter;

		cProfileNode* m_parent;
		cProfileNode* m_lastChild;
		cProfileNode* m_firstChild;
		cProfileNode* m_sibling;

		static callBackProfileRender m_callBackRender;
	};

	/*
	*/
	class cProfileIterator
	{
	public:
		cProfileIterator(cProfileNode* start);
		void getFirst();
		void getNext();
		bool isDone() const;
		bool enter(int depth);
		void render(int x, int y, int depth);

		string_id getId()			{	return m_child->getId();			}
		int getTotalCall()			{	return m_child->getTotalCall();	}
		float getTotalTime()		{	return m_child->getTotalTime();	}
		string_id getParentId()		{	return m_parent->getId();			}
		int getParentTotalCall()	{	return m_parent->getTotalCall();	}
		float getParentTotalTime()	{	return m_parent->getTotalTime();	}

	private:
		void render(int x, int& y, int depth, cProfileNode* node);

	private:
		cProfileNode* m_parent;
		cProfileNode* m_child;
	};

	/*
		@date 2012.
		@auth prodongi
		@desc gpg3¿¡¼­ °®°í ¿È
		@todo 
	*/
	class cProfileManager
	{
	public:
		static void start(string_id id);
		static void stop();
		static void reset();
		static void render(int x, int y);

		static int m_renderDepth;

	private:
		static cProfileNode m_root;
		static cProfileNode* m_current;
	};
}



#endif