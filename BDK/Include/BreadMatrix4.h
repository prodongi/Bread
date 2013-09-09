
#ifndef _BreadMatrix4_h_
#define _BreadMatrix4_h_

#include "BreadMath.h"
#include "BreadVector3.h"
#include "BreadVector4.h"
#include "BreadBounding.h"

namespace Bread
{
	struct sMatrix4;

	template <typename T1, typename T2, typename T3> inline void addMat4(T1 const& mat1, T2 const& mat2, T3& out);
	template <typename T1, typename T2, typename T3> inline void subMat4(T1 const& mat1, T2 const& mat2, T3& out);
	template <typename T1, typename T2> inline void mulMat4(T1 const& mat1, T2 const& mat2, sMatrix4& out);
	template <typename T1, typename T2> inline void mulMat4(T1 const& m, T2 const& v, sVector3& out);
	template <typename T1, typename T2> inline void mulMat4(T1 const& m, T2 const& v, sVector4& out);
	template <typename T> inline void mulMat4(T const& m, sBox const& b, sBox& out);


	struct sMatrix4
	{
	public:
		sMatrix4() {}
		sMatrix4(bool setIentity)
		{
			if (setIentity)
				identity();
		}
		sMatrix4(float m00, float m01, float m02, float m03,
				 float m10, float m11, float m12, float m13,
				 float m20, float m21, float m22, float m23,
				 float m30, float m31, float m32, float m33)
		{
			m[0][0] = m00;
			m[0][1] = m01;
			m[0][2] = m02;
			m[0][3] = m03;

			m[1][0] = m10;
			m[1][1] = m11;
			m[1][2] = m12;
			m[1][3] = m13;

			m[2][0] = m20;
			m[2][1] = m21;
			m[2][2] = m22;
			m[2][3] = m23;

			m[3][0] = m30;
			m[3][1] = m31;
			m[3][2] = m32;
			m[3][3] = m33;
		}
		template <typename T>
		sMatrix4(T const& t)
		{
			memcpy(m, t.m, sizeof (float) * 4 * 4);
		}
		~sMatrix4()
		{
		}

		void set(float const* m16)
		{
			memcpy(m_, m16, sizeof (float) * 16);
		}

		template <typename T>
		inline sMatrix4& operator = (const T& m4)
		{
			m[0][0]=m4.m[0][0]; m[0][1]=m4.m[0][1]; m[0][2]= m4.m[0][2]; m[0][3]=m4.m[0][3];
			m[1][0]=m4.m[1][0]; m[1][1]=m4.m[1][1]; m[1][2]= m4.m[1][2]; m[1][3]=m4.m[1][3];
			m[2][0]=m4.m[2][0]; m[2][1]=m4.m[2][1]; m[2][2]= m4.m[2][2]; m[2][3]=m4.m[2][3];
			m[3][0]=m4.m[3][0]; m[3][1]=m4.m[3][1]; m[3][2]= m4.m[3][2]; m[3][3]=m4.m[3][3];

			return *this;
		}

		template <typename T>
		inline sMatrix4& operator += (const T& m4)
		{
			m[0][0]+=m4.m[0][0]; m[0][1]+=m4.m[0][1]; m[0][2]+= m4.m[0][2]; m[0][3]+=m4.m[0][3];
			m[1][0]+=m4.m[1][0]; m[1][1]+=m4.m[1][1]; m[1][2]+= m4.m[1][2]; m[1][3]+=m4.m[1][3];
			m[2][0]+=m4.m[2][0]; m[2][1]+=m4.m[2][1]; m[2][2]+= m4.m[2][2]; m[2][3]+=m4.m[2][3];
			m[3][0]+=m4.m[3][0]; m[3][1]+=m4.m[3][1]; m[3][2]+= m4.m[3][2]; m[3][3]+=m4.m[3][3];

			return *this;
		}

		template <typename T>
		inline sMatrix4& operator -= (const T& m4)
		{
			m[0][0]-=m4.m[0][0]; m[0][1]-=m4.m[0][1]; m[0][2]-= m4.m[0][2]; m[0][3]-=m4.m[0][3];
			m[1][0]-=m4.m[1][0]; m[1][1]-=m4.m[1][1]; m[1][2]-= m4.m[1][2]; m[1][3]-=m4.m[1][3];
			m[2][0]-=m4.m[2][0]; m[2][1]-=m4.m[2][1]; m[2][2]-= m4.m[2][2]; m[2][3]-=m4.m[2][3];
			m[3][0]-=m4.m[3][0]; m[3][1]-=m4.m[3][1]; m[3][2]-= m4.m[3][2]; m[3][3]-=m4.m[3][3];

			return *this;
		}

		inline float operator [] (int index) const
		{
			return m_[index];
		}

		//Ogre에서 갖고 옴
		template <typename T>
		inline void inverse(T& out) const
		{
			float m00 = m[0][0], m01 = m[0][1], m02 = m[0][2], m03 = m[0][3];
			float m10 = m[1][0], m11 = m[1][1], m12 = m[1][2], m13 = m[1][3];
			float m20 = m[2][0], m21 = m[2][1], m22 = m[2][2], m23 = m[2][3];
			float m30 = m[3][0], m31 = m[3][1], m32 = m[3][2], m33 = m[3][3];

			float v0 = m20 * m31 - m21 * m30;
			float v1 = m20 * m32 - m22 * m30;
			float v2 = m20 * m33 - m23 * m30;
			float v3 = m21 * m32 - m22 * m31;
			float v4 = m21 * m33 - m23 * m31;
			float v5 = m22 * m33 - m23 * m32;

			float t00 = + (v5 * m11 - v4 * m12 + v3 * m13);
			float t10 = - (v5 * m10 - v2 * m12 + v1 * m13);
			float t20 = + (v4 * m10 - v2 * m11 + v0 * m13);
			float t30 = - (v3 * m10 - v1 * m11 + v0 * m12);

			float invDet = cMath::inv(t00 * m00 + t10 * m01 + t20 * m02 + t30 * m03);

			float d00 = t00 * invDet;
			float d10 = t10 * invDet;
			float d20 = t20 * invDet;
			float d30 = t30 * invDet;

			float d01 = - (v5 * m01 - v4 * m02 + v3 * m03) * invDet;
			float d11 = + (v5 * m00 - v2 * m02 + v1 * m03) * invDet;
			float d21 = - (v4 * m00 - v2 * m01 + v0 * m03) * invDet;
			float d31 = + (v3 * m00 - v1 * m01 + v0 * m02) * invDet;

			v0 = m10 * m31 - m11 * m30;
			v1 = m10 * m32 - m12 * m30;
			v2 = m10 * m33 - m13 * m30;
			v3 = m11 * m32 - m12 * m31;
			v4 = m11 * m33 - m13 * m31;
			v5 = m12 * m33 - m13 * m32;

			float d02 = + (v5 * m01 - v4 * m02 + v3 * m03) * invDet;
			float d12 = - (v5 * m00 - v2 * m02 + v1 * m03) * invDet;
			float d22 = + (v4 * m00 - v2 * m01 + v0 * m03) * invDet;
			float d32 = - (v3 * m00 - v1 * m01 + v0 * m02) * invDet;

			v0 = m21 * m10 - m20 * m11;
			v1 = m22 * m10 - m20 * m12;
			v2 = m23 * m10 - m20 * m13;
			v3 = m22 * m11 - m21 * m12;
			v4 = m23 * m11 - m21 * m13;
			v5 = m23 * m12 - m22 * m13;

			float d03 = - (v5 * m01 - v4 * m02 + v3 * m03) * invDet;
			float d13 = + (v5 * m00 - v2 * m02 + v1 * m03) * invDet;
			float d23 = - (v4 * m00 - v2 * m01 + v0 * m03) * invDet;
			float d33 = + (v3 * m00 - v1 * m01 + v0 * m02) * invDet;

			out.m[0][0] = d00;
			out.m[0][1] = d01;
			out.m[0][2] = d02;
			out.m[0][3] = d03;

			out.m[1][0] = d10;
			out.m[1][1] = d11;
			out.m[1][2] = d12;
			out.m[1][3] = d13;

			out.m[2][0] = d20;
			out.m[2][1] = d21;
			out.m[2][2] = d22;
			out.m[2][3] = d23;

			out.m[3][0] = d30;
			out.m[3][1] = d31;
			out.m[3][2] = d32;
			out.m[3][3] = d33;
		}

		/**
		 * @brief 대각선의 합
		 */
		inline float trace() const
		{
			return m[0][0] + m[1][1] + m[2][2] + m[3][3];
		}
		/**
		 * @brief 전치 행렬
		 * @par	  (A+B)T = AT + BT
		 *		  (aA)T = aAT
		 */
		template <typename T>
		inline void transpose(T& out) const
		{
			out.m[0][0] = m[0][0];
			out.m[0][1] = m[1][0];
			out.m[0][2] = m[2][0];
			out.m[0][3] = m[3][0];
			
			out.m[1][0] = m[0][1];
			out.m[1][1] = m[1][1];
			out.m[1][2] = m[2][1];
			out.m[1][3] = m[3][1];
			
			out.m[2][0] = m[0][2];
			out.m[2][1] = m[1][2];
			out.m[2][2] = m[2][2];
			out.m[2][3] = m[3][2];
							
			out.m[3][0] = m[0][3];
			out.m[3][1] = m[1][3];
			out.m[3][2] = m[2][3];
			out.m[3][3] = m[3][3];
		}
		/*
		*/
		template <typename T>
		inline void translation(T const& v)
		{
			m[3][0] = v.x;
			m[3][1] = v.y;
			m[3][2] = v.z;
		}
		/*
		*/
		inline void identity()
		{
			m[0][0] = 1;	m[0][1] = 0;	m[0][2] = 0;	m[0][3] = 0;
			m[1][0] = 0;	m[1][1] = 1;	m[1][2] = 0;	m[1][3] = 0;
			m[2][0] = 0;	m[2][1] = 0;	m[2][2] = 1;	m[2][3] = 0;
			m[3][0] = 0;	m[3][1] = 0;	m[3][2] = 0;	m[3][3] = 1;
		}
		/*
		 *	
		 */
		template <typename T1, typename T2>
		inline void lerp(T1 const& a, T2 const& b, float t)
		{
			for (int i = 0; i < 16; ++i)
				m_[i] = cMath::lerp(a.m_[i], b.m_[i], t);
		}
		/*
		 *	
		 */
		template <typename T>
		inline void to(T& mat) const
		{
			memcpy(mat.m, m, sizeof (float) * 4 * 4);
		}
		/*
		*/
		template <typename T1, typename T2>
		inline void transform(T1 const& v, T2& out) const
		{
			out.x = m[0][0]*v.x + m[1][0]*v.y + m[2][0]*v.z + m[3][0]*v.w;
			out.y = m[0][1]*v.x + m[1][1]*v.y + m[2][1]*v.z + m[3][1]*v.w;
			out.z = m[0][2]*v.x + m[1][2]*v.y + m[2][2]*v.z + m[3][2]*v.w;
			out.w = m[0][3]*v.x + m[1][3]*v.y + m[2][3]*v.z + m[3][3]*v.w;
		}
		/*
		*/
		template <typename T1, typename T2>
		inline void transformCoord(T1 const& v, T2& out) const
		{
			T1 _v = v;
			out.x = m[0][0]*_v.x + m[1][0]*_v.y + m[2][0]*_v.z;
			out.y = m[0][1]*_v.x + m[1][1]*_v.y + m[2][1]*_v.z;
			out.z = m[0][2]*_v.x + m[1][2]*_v.y + m[2][2]*_v.z;
		}
		/*
		*/
		template <typename T1, typename T2>
		void makeViewMatrix(T1 const& eye, T2 const& at)
		{
			static sVector3 axisX;
			static sVector3 axisY;
			static sVector3 axisZ;

			calcAxis(eye, at, axisX, axisY, axisZ);

			m[0][0] = axisX.x;			m[0][1] = axisY.x; 			m[0][2] = axisZ.x; 			m[0][3] = 0.0f;
			m[1][0] = axisX.y;			m[1][1] = axisY.y; 			m[1][2] = axisZ.y; 			m[1][3] = 0.0f;
			m[2][0] = axisX.z;  		m[2][1] = axisY.z; 			m[2][2] = axisZ.z; 			m[2][3] = 0.0f;
			m[3][0] = -axisX.dot(eye);	m[3][1] = -axisY.dot(eye);	m[3][2] = -axisZ.dot(eye);	m[3][3] = 1.0f;
		}
		/*
		*/
		template <typename T1, typename T2>
		void makeViewMatrixDir(T1 const& eye, T2 const& dir)
		{
			static sVector3 at;
			addVec3(eye, dir, at);
			makeViewMatrix(eye, at);

		}
		void makePerspectiveMatrix(float fovY, float aspect, float zNear, float zFar);
		void makeOrthoMatrix(float w, float h, float zNear, float zFar);
		void rotationX(float radian);
		void rotationY(float radian);
		void rotationZ(float radian);
		template <typename T> void getA(T& v) const	{	v.x = m[0][0];	v.y = m[1][0];	v.z = m[2][0];	}
		template <typename T> void getB(T& v) const	{	v.x = m[0][1];	v.y = m[1][1];	v.z = m[2][1];	}
		template <typename T> void getC(T& v) const	{	v.x = m[0][2];	v.y = m[1][2];	v.z = m[2][2];	}
		template <typename T> void getD(T& v) const	{	v.x = m[3][0];	v.y = m[3][1];	v.z = m[3][2];	}
		template <typename T> void setA(T const& v)	{	m[0][0] = v.x;	m[1][0] = v.y;	m[2][0] = v.z;	}
		template <typename T> void setB(T const& v)	{	m[0][1] = v.x;	m[1][1] = v.y;	m[2][1] = v.z;	}
		template <typename T> void setC(T const& v)	{	m[0][2] = v.x;	m[1][2] = v.y;	m[2][2] = v.z;	}
		template <typename T> void setD(T const& v)	{	m[3][0] = v.x;	m[3][1] = v.y;	m[3][2] = v.z;	}

	public:
		union 
		{
			float m[4][4];
			float m_[16];
		};

		static const sMatrix4 IDENTITY;
	};

	/*
	 *	
	 */
	 template <typename T1, typename T2, typename T3>
	 void addMat4(T1 const& mat1, T2 const& mat2, T3& out)
	 {
		 for (int i = 0; i < 16; ++i)
			 out.m_[i] = mat1.m_[i] + mat2.m_[i];
	 }
	 /*
	  *	
	  */
	 template <typename T1, typename T2, typename T3>
	 void subMat4(T1 const& mat1, T2 const& mat2, T3& out)
	 {
		 for (int i = 0; i < 16; ++i)
			 out.m_[i] = mat1.m_[i] - mat2.m_[i];
	 }
	 /*
	  *	
	  */
	 template <typename T1, typename T2>
	 void mulMat4(T1 const& mat1, T2 const& mat2, sMatrix4& out)
	 {
		// assert(&mat1 != &out && _T("mat1 is equall out"));
		// assert(&mat2 != &out && _T("mat2 is equall out"));

		 out.m[0][0] = mat1.m[0][0]*mat2.m[0][0] + mat1.m[0][1]*mat2.m[1][0] + mat1.m[0][2]*mat2.m[2][0] + mat1.m[0][3]*mat2.m[3][0];
		 out.m[0][1] = mat1.m[0][0]*mat2.m[0][1] + mat1.m[0][1]*mat2.m[1][1] + mat1.m[0][2]*mat2.m[2][1] + mat1.m[0][3]*mat2.m[3][1];
		 out.m[0][2] = mat1.m[0][0]*mat2.m[0][2] + mat1.m[0][1]*mat2.m[1][2] + mat1.m[0][2]*mat2.m[2][2] + mat1.m[0][3]*mat2.m[3][2];
		 out.m[0][3] = mat1.m[0][0]*mat2.m[0][3] + mat1.m[0][1]*mat2.m[1][3] + mat1.m[0][2]*mat2.m[2][3] + mat1.m[0][3]*mat2.m[3][3];

		 out.m[1][0] = mat1.m[1][0]*mat2.m[0][0] + mat1.m[1][1]*mat2.m[1][0] + mat1.m[1][2]*mat2.m[2][0] + mat1.m[1][3]*mat2.m[3][0];
		 out.m[1][1] = mat1.m[1][0]*mat2.m[0][1] + mat1.m[1][1]*mat2.m[1][1] + mat1.m[1][2]*mat2.m[2][1] + mat1.m[1][3]*mat2.m[3][1];
		 out.m[1][2] = mat1.m[1][0]*mat2.m[0][2] + mat1.m[1][1]*mat2.m[1][2] + mat1.m[1][2]*mat2.m[2][2] + mat1.m[1][3]*mat2.m[3][2];
		 out.m[1][3] = mat1.m[1][0]*mat2.m[0][3] + mat1.m[1][1]*mat2.m[1][3] + mat1.m[1][2]*mat2.m[2][3] + mat1.m[1][3]*mat2.m[3][3];

		 out.m[2][0] = mat1.m[2][0]*mat2.m[0][0] + mat1.m[2][1]*mat2.m[1][0] + mat1.m[2][2]*mat2.m[2][0] + mat1.m[2][3]*mat2.m[3][0];
		 out.m[2][1] = mat1.m[2][0]*mat2.m[0][1] + mat1.m[2][1]*mat2.m[1][1] + mat1.m[2][2]*mat2.m[2][1] + mat1.m[2][3]*mat2.m[3][1];
		 out.m[2][2] = mat1.m[2][0]*mat2.m[0][2] + mat1.m[2][1]*mat2.m[1][2] + mat1.m[2][2]*mat2.m[2][2] + mat1.m[2][3]*mat2.m[3][2];
		 out.m[2][3] = mat1.m[2][0]*mat2.m[0][3] + mat1.m[2][1]*mat2.m[1][3] + mat1.m[2][2]*mat2.m[2][3] + mat1.m[2][3]*mat2.m[3][3];

		 out.m[3][0] = mat1.m[3][0]*mat2.m[0][0] + mat1.m[3][1]*mat2.m[1][0] + mat1.m[3][2]*mat2.m[2][0] + mat1.m[3][3]*mat2.m[3][0];
		 out.m[3][1] = mat1.m[3][0]*mat2.m[0][1] + mat1.m[3][1]*mat2.m[1][1] + mat1.m[3][2]*mat2.m[2][1] + mat1.m[3][3]*mat2.m[3][1];
		 out.m[3][2] = mat1.m[3][0]*mat2.m[0][2] + mat1.m[3][1]*mat2.m[1][2] + mat1.m[3][2]*mat2.m[2][2] + mat1.m[3][3]*mat2.m[3][2];
		 out.m[3][3] = mat1.m[3][0]*mat2.m[0][3] + mat1.m[3][1]*mat2.m[1][3] + mat1.m[3][2]*mat2.m[2][3] + mat1.m[3][3]*mat2.m[3][3];
	 }
	 /*
	 *	
	 */
	template <typename T1, typename T2>
	void mulMat4(T1 const& m, T2 const& v, sVector3& out)
	{
		static T2 tempV;
		tempV = v;
		out.x = (m.m[0][0]*tempV.x + m.m[1][0]*tempV.y + m.m[2][0]*tempV.z + m.m[3][0]);
		out.y = (m.m[0][1]*tempV.x + m.m[1][1]*tempV.y + m.m[2][1]*tempV.z + m.m[3][1]);
		out.z = (m.m[0][2]*tempV.x + m.m[1][2]*tempV.y + m.m[2][2]*tempV.z + m.m[3][2]);
	}
	/*
	 *	
	 */
	template <typename T1, typename T2>
	void mulMat4(T1 const& m, T2 const& v, sVector4& out)
	{
		out.x = (m.m[0][0]*v.x + m.m[1][0]*v.y + m.m[2][0]*v.z + m.m[3][0]*v.w);
		out.y = (m.m[0][1]*v.x + m.m[1][1]*v.y + m.m[2][1]*v.z + m.m[3][1]*v.w);
		out.z = (m.m[0][2]*v.x + m.m[1][2]*v.y + m.m[2][2]*v.z + m.m[3][2]*v.w);
		out.w = (m.m[0][3]*v.x + m.m[1][3]*v.y + m.m[2][3]*v.z + m.m[3][3]*v.w);
	}
	/*
	*/
	template <typename T>
	void mulMat4(T const& m, sBox const& b, sBox& out)
	{
		for (uint i = 0; i < sBox::VERT_NUM; ++i)
		{
			mulMat4(m, b.m_v[i], out.m_v[i]);
		}
	}
}










#endif
















