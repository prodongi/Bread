
#include "BreadRand.h"
#include <stdlib.h>
#include "MersenneTwister.h"

namespace Bread
{
	/*
	*/
	unsigned long sRandLinearCongruential::rand()
	{
		return ::rand();
	}
	/*
	*/
	unsigned long sRandLinearCongruential::rand(unsigned long range)
	{
		return rand()%range;
	}
	/*
	*/
	sRandMersenneTwister::sRandMersenneTwister() : m_mt(NULL)
	{
	}
	/*
	*/
	sRandMersenneTwister::~sRandMersenneTwister()
	{
		if (m_mt) delete m_mt;
	}
	/*
	*/
	void sRandMersenneTwister::initialize()
	{
		::MTRand::uint32 seed[ ::MTRand::N ];
		for (int n = 0; n < ::MTRand::N; ++n)
			seed[n] = 23 * n;  // fill with anything

		m_mt = new ::MTRand(seed);
	}
	/*
	*/
	unsigned long sRandMersenneTwister::rand()
	{
		return m_mt->randInt();
	}
	/*
	*/
	unsigned long sRandMersenneTwister::rand(unsigned long range)
	{
		return m_mt->randInt(range);
	}
	/*
	*/
	void sRandWellRng512::initialize()
	{
		index = 0;

		for (int i = 0; i < 16; ++i)
			state[i] = ::rand();		
	}
	/*
	*/
	unsigned long sRandWellRng512::rand()
	{
		unsigned long a, b, c, d;
		a = state[index];
		c = state[(index+13)&15];
		b = a^c^(a<<16)^(c<<15);
		c = state[(index+9)&15];
		c ^= (c>>11);
		a = state[index] = b^c;
		d = a^((a<<5)&0xDA442D20UL);
		index = (index + 15)&15;
		a = state[index];
		state[index] = a^b^d^(a<<2)^(b<<18)^(c<<28);
		return state[index];
	}
	/*
	*/
	unsigned long sRandWellRng512::rand(unsigned long range)
	{
		return rand()%range;
	}
}