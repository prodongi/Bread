
#include "BreadVolumeTexture.h"
#include "BreadFileSystem.h"
#include "BreadD3DSystemClassify.h"

namespace Bread
{
	int p[512],permutation[] =  { 
		151,160,137,91,90,15,
		131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
		190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
		88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
		77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
		102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
		135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
		5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
		223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
		129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
		251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
		49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
		138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180 };

	/* shaderx 3 volume cloud¿¡¼­ ·£´ý ÅØ½ºÃç »ý¼º ÇÔ¼ö °®°í ¿È
	*/
		double inline fade(double t) { return t * t * t * (t * (t * 6 - 15) + 10); }
		double inline lerp(double t, double a, double b) { return a + t * (b - a); }
		double inline grad(int hash, double x, double y, double z) {
			int h = hash & 15;                      // CONVERT LO 4 BITS OF HASH CODE
			double u = h<8||h==12||h==13 ? x : y,   // INTO 12 GRADIENT DIRECTIONS.
				v = h<4||h==12||h==13 ? y : z;
			return ((h&1) == 0 ? u : -u) + ((h&2) == 0 ? v : -v);
		}

	double noise(double x, double y, double z) {

		int X = (int)floor(x) & 255,                  // FIND UNIT CUBE THAT
			Y = (int)floor(y) & 255,                  // CONTAINS POINT.
			Z = (int)floor(z) & 255;
		x -= floor(x);                                // FIND RELATIVE X,Y,Z
		y -= floor(y);                                // OF POINT IN CUBE.
		z -= floor(z);
		double u = fade(x),                                // COMPUTE FADE CURVES
			v = fade(y),                                // FOR EACH OF X,Y,Z.
			w = fade(z);
		int A = p[X  ]+Y, AA = p[A]+Z, AB = p[A+1]+Z,      // HASH COORDINATES OF
			B = p[X+1]+Y, BA = p[B]+Z, BB = p[B+1]+Z;      // THE 8 CUBE CORNERS,

		return lerp(w, lerp(v, lerp(u, grad(p[AA  ], x  , y  , z   ),  // AND ADD
			grad(p[BA  ], x-1, y  , z   )), // BLENDED
			lerp(u, grad(p[AB  ], x  , y-1, z   ),  // RESULTS
			grad(p[BB  ], x-1, y-1, z   ))),// FROM  8
			lerp(v, lerp(u, grad(p[AA+1], x  , y  , z-1 ),  // CORNERS
			grad(p[BA+1], x-1, y  , z-1 )), // OF CUBE
			lerp(u, grad(p[AB+1], x  , y-1, z-1 ),
			grad(p[BB+1], x-1, y-1, z-1 ))));
	}
	float AveragedNoise(float x,float y, float z)
	{

		float noisev=(float)((noise(x*5,y*5,z*5)*(5-x*5)*(5-y*5)*(5-z*5))+
			(noise(x*5-5,y*5,z*5)*(x*5)*(5-y*5)*(5-z*5))+
			(noise(x*5-5,y*5-5,z*5)*(x*5)*(y*5)*(5-z*5))+
			(noise(x*5,y*5-5,z*5)*(5-x*5)*(y*5)*(5-z*5))+

			(noise(x*5-5,y*5,z*5-5)*(x*5)*(5-y*5)*(z*5))+
			(noise(x*5-5,y*5-5,z*5-5)*(x*5)*(y*5)*(z*5))+
			(noise(x*5,y*5-5,z*5-5)*(5-x*5)*(y*5)*(z*5))+
			(noise(x*5,y*5,z*5-5)*(5-x*5)*(5-y*5)*(z*5)))/(5*5*5);
		return noisev;
	}
	VOID WINAPI RandTexture (D3DXVECTOR4* pOut, const D3DXVECTOR3* pTexCoord, 
		const D3DXVECTOR3* /*pTexelSize*/, LPVOID /*pData*/)
	{

		//	for(int j=0;j<4;j++)
		//	pOut[0][j]=(float) (rand() / (float) RAND_MAX);

		//pOut[0][3]=z;
		//	float alpha=sqrt((z-.5)*(z-.5)+(y-.5)*(y-.5)+(x-.5)*(x-.5));
		//	*pOut = D3DXVECTOR4(1,1,1,1-alpha*3);	
		float noisev=AveragedNoise(pTexCoord->x,pTexCoord->y,pTexCoord->z);
		float dxnoisev=AveragedNoise(pTexCoord->x+.1f,pTexCoord->y,pTexCoord->z)-AveragedNoise(pTexCoord->x,pTexCoord->y,pTexCoord->z)/.1f;
		float dynoisev=AveragedNoise(pTexCoord->x,pTexCoord->y+.1f,pTexCoord->z)-AveragedNoise(pTexCoord->x,pTexCoord->y,pTexCoord->z)/.1f;
		float dznoisev=AveragedNoise(pTexCoord->x,pTexCoord->y,pTexCoord->z+.1f)-AveragedNoise(pTexCoord->x,pTexCoord->y,pTexCoord->z)/.1f;		
		float normaliz=sqrt(dxnoisev*dxnoisev+dynoisev*dynoisev+dznoisev*dznoisev);
		float normalx=(dxnoisev/normaliz);
		float normaly=(dynoisev/normaliz);
		float normalz=(dznoisev/normaliz);
		normalx+=1;
		normalx/=2;
		normaly+=1;
		normaly/=2;
		normalz+=1;
		normalz/=2;
		*pOut = D3DXVECTOR4(normalx,normaly,normalz,noisev);

	}

	sVolumeTexture::sVolumeTexture() : m_tex(NULL)
	{
		for (int i2=0; i2 < 256 ; i2++) p[256+i2] = p[i2] = permutation[i2];
	}
	/*
	*/
	bool sVolumeTexture::load(std_string const& filename)
	{
		std_string realPath;
		if (!_getFileSystem()->findRealPath(filename, realPath))
		{
			return false;
		}

		if (!checkHResultReturn(D3DXCreateVolumeTextureFromFile(cD3DSystem::getD3DDevice(), realPath.c_str(), &m_tex)))
			return false;

		trace(_T("load texture : %s\n"), filename.c_str());

		return true;
	}
	/*
	*/
	void sVolumeTexture::getDesc(D3DVOLUME_DESC& desc)
	{
		if (FAILED(m_tex->GetLevelDesc(0, &desc)))
		{
			assert(0 && "Failed texture get level desc");
		}
	}
	/*
	*/
	bool sVolumeTexture::fillNoise()
	{
		if (!m_tex)
		{
			assert(0 && "m_tex is null");
			return false;
		}

		if (!checkHResultReturn(D3DXFillVolumeTexture(m_tex, RandTexture, NULL)))
			return false;

		return true;
	}
	/*
	*/
	bool sVolumeTexture::create(int width, int height, int depth, D3DFORMAT format)
	{
		if (m_tex)
		{
			assert(0 && "m_text isn't null");
			return false;
		}

		if (!checkHResultReturn(D3DXCreateVolumeTexture(cD3DSystem::getD3DDevice(), width, height, depth, 
								D3DX_DEFAULT, 0, format, D3DPOOL_MANAGED, &m_tex)))
			return false;

		return true;
	}
}