
#ifndef _BreadVector4_h_
#define _BreadVector4_h_

namespace Bread
{
	#define V4(x, y, z, w)	sVector4(x, y, z, w)

	/*
	*/
	struct sVector4
	{
	public:
		sVector4()
		{
		}
		sVector4(float _x, float _y, float _z, float _w)
		{
			x = _x;
			y = _y;
			z = _z;
			w = _w;
		}
		template <typename T> sVector4(T const& t, float _w)
		{
			x = t.x;
			y = t.y;
			z = t.z;
			w = _w;
		}
		~sVector4()
		{
		}
		inline float& operator [] (unsigned int i)
		{ 
			return (&x)[i]; 
		}
		inline float operator [] (unsigned int i) const 
		{ 
			return (&x)[i]; 
		}

		inline float length() const
		{
			return cMath::sqrtf(lengthSquare());
		}

		inline float lengthSquare() const
		{
			return x*x + y*y + z*z + w*w;
		}

		inline void normalize()
		{
			float inv = cMath::inv(length());

			x *= inv;
			y *= inv;
			z *= inv;
			w *= inv;
		}

		template <typename T>
		void to3(T& t) const
		{
			t.x = x;
			t.y = y;
			t.z = z;
		}

		template <typename T>
		void to4(T& t) const
		{
			t.x = x;
			t.y = y;
			t.z = z;
			t.w = w;
		}


	public:
		float x;
		float y;
		float z;
		float w;
	};
}












#endif











