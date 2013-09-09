
#include "BreadUtils.h"
#include <iostream>
#include <fstream>

namespace Bread
{
	/*
	 *	
	 */
	int hexToInt(char const* szHex)
	{
		int hex = 0; // 반환될 값. 초기에는 0이다.
		int nibble; // 16진수의 한 니블(4비트)값을 담아둘 곳

		while (*szHex) 
		{
			hex <<= 4;

			if (*szHex >= '0' && *szHex <= '9') 
			{
				nibble = *szHex - '0';
			} 
			else if (*szHex >= 'a' && *szHex <= 'f') 
			{
				nibble = *szHex - 'a' + 10;
			} 
			else if (*szHex >= 'A' && *szHex <= 'F') 
			{
				nibble = *szHex - 'A' + 10;
			} 
			else 
			{
				nibble = 0;
			}

			hex |= nibble;

			szHex++;
		}
		return hex;
	}
}





















