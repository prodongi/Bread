
#ifndef _BreadInput_h_
#define _BreadInput_h_


namespace Bread
{
	class cInputTypedef
	{
	public:
		enum
		{
			kismsNone	= 0x00,
			kismsCtrl	= 0x01,
			kismsAlt	= 0x02,
			kismsShift	= 0x04
		};

		typedef unsigned char iSModifierState;
	};
}







#endif