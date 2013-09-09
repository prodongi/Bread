
#ifndef _BreadFVFInfo_h_
#define _BreadFVFInfo_h_

namespace Bread
{
	/*
	 *	
	 */
	struct sFVFInfo
	{
		enum {	FVF_XYZ, 
					FVF_XYZRHW,
					FVF_XYZB4,
					FVF_LASTBETA_UBYE4,
					FVF_NORMAL, 
					FVF_TANGENT, 
					FVF_BINORMAL, 
					FVF_COLOR, 
					FVF_TEX0, 
					FVF_TEX1, 
					FVF_TEX2,
					FVF_NUM 
				 };
		sFVFInfo() : m_offset(0), m_is(false), m_reserved0(0) {}
		int m_offset;
		bool m_is;
		unsigned char m_reserved0;	/// �ڵ�󿡼� ������ �� �ٸ� ������ �߰��ϱ� ���ؼ� ����ϱ⵵ ��
		unsigned char reserved[2];
	};
}

#endif