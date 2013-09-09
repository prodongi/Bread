
#ifndef _BreadRand_h_
#define _BreadRand_h_

class MTRand;

namespace Bread
{
	/*
		@date 2011.07.11
		@auth prodongi
		@desc 
		@todo 
	*/
	struct sRand									
	{	
		virtual void initialize() {}
		virtual unsigned long rand() = 0;
		virtual unsigned long rand(unsigned long range) = 0;
	};
	/*	
	*/
	struct sRandLinearCongruential : public sRand	
	{	
		virtual unsigned long rand();
		virtual unsigned long rand(unsigned long range);
	};
	/*	@desc web¿¡¼­ °®°í¿È, period = 2^19937-1
	*/
	struct sRandMersenneTwister : public sRand		
	{	
		sRandMersenneTwister();
		~sRandMersenneTwister();

		virtual void initialize();
		virtual unsigned long rand();
		virtual unsigned long rand(unsigned long range);		

	private:
		::MTRand* m_mt;
	};
	/*	@desc web¿¡¼­ °®°í¿È, period = 2^512
	*/
	struct sRandWellRng512 : public sRand
	{
		virtual void initialize();
		virtual unsigned long rand();
		virtual unsigned long rand(unsigned long range);		
		
	private:
		unsigned long state[16];
		unsigned int  index;
	};
}



#endif