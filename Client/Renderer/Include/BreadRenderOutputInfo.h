
#ifndef _BreadRenderOutputInfo_h_
#define _BreadRenderOutputInfo_h_


namespace Bread
{
	class cRenderOutputInfo
	{
	public:
		cRenderOutputInfo() {};
		~cRenderOutputInfo() {};

		static void clear();
		static void add(unsigned int primitiveType, unsigned int vertexNum, unsigned int faceNum);

		static void addVertexNumber(unsigned int primitiveCount)	{	m_vertexNumber += primitiveCount;	}
		static void addFaceNumber(unsigned int primitiveCount)		{	m_faceNumber += primitiveCount;	}

		static unsigned int getVertexNumber()	{	return m_vertexNumber;	}
		static unsigned int getFaceNumber() 	{	return m_faceNumber;	}
		static unsigned int getDpNumber()		{	return m_dpNumber;		}
		static bool enable()					{	return m_enable;		}

	private:
		static unsigned int m_vertexNumber;
		static unsigned int m_faceNumber;
		static unsigned int m_dpNumber;
		static bool m_enable;
	};
}







#endif