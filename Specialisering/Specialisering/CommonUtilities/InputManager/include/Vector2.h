#pragma once
#include <cmath>
namespace CommonUtilities
{
	template<typename T>
	class Vector2
	{
	public:
		union
		{
			T x;
			T myX;
		};
		union
		{
			T y;
			T myY;
		};
		//Creates a null-vector
		Vector2<T>();
		//Creates a vector (aX, aY, aZ)
		Vector2<T>(const T& aX, const T& aY);
		//Copy Constructor
		Vector2<T>(const Vector2<T>& aVector);
		//Assignment Operator
		Vector2<T>& operator=(const Vector2<T> aVector2);
		//Destructor
		~Vector2<T>() = default;
		//Return Square Root
		T LengthSqr() const;
		//Return Length of vector
		T Length() const;
		//Normalize
		Vector2<T> Normalize();
		Vector2<T> GetNormalized();
		//Returns the dot product of this and aVector
		T Dot(const Vector2<T>& aVector) const;
		Vector2<T> GetRotated(const float anAngle);
		void Rotate(const float anAngle);
	};

	//Null Constructor
	template<typename T>
	inline Vector2<T>::Vector2() :
		x(0),
		y(0)
	{
	}
	//Assign constructor
	template<typename T>
	inline Vector2<T>::Vector2(const T& aX, const T& aY) :
		x(aX),
		y(aY)
	{
	}
	//Copy Constructor
	template<typename T>
	inline Vector2<T>::Vector2(const Vector2<T>& aVector) :
		x(aVector.x),
		y(aVector.y)
	{

	}
	//Length squared of vector 4 
	template<typename T>
	inline T Vector2<T>::LengthSqr() const
	{
		return (x * x) + (y * y);
	}
	//Lenght of Vector2
	template<typename T>
	inline T Vector2<T>::Length() const //calculate the magnitude		||v||
	{
		return  sqrt((x * x) + (y * y));
	}
	//Get Normals
	template<typename T>
	inline Vector2<T> Vector2<T>::Normalize()
	{
		if (x == 0 && y == 0)
		{
			x = 0;
			y = 0;
		}
		else
		{
			*this /= Length();
		}
		return *this;
	}
	template<typename T>
	inline Vector2<T> Vector2<T>::GetNormalized()
	{
		Vector2<T> vec = *this;
		if (x == 0 && y == 0)
		{
			vec.x = 0;
			vec.y = 0;
		}
		else
		{
			vec /= vec.Length();
		}
		return vec;
	}
	template<typename T>
	inline T Vector2<T>::Dot(const Vector2<T>& aVector) const
	{
		return x * aVector.x + y * aVector.y;
	}
	template<typename T>
	inline Vector2<T> Vector2<T>::GetRotated(const float aAngle)
	{
		float radians = aAngle * (3.14f / 180.0f);
		Vector2<T> vec;
		vec.x = myX * cos(radians) - myY * sin(radians);
		vec.y = myX * sin(radians) + myY * cos(radians);
		return vec;
	}
	template<typename T>
	inline void Vector2<T>::Rotate(const float aAngle)
	{
		float radians = aAngle * (3.14f / 180.0f);
		x = myX * cos(radians) - myY * sin(radians);
		y = myX * sin(radians) + myY * cos(radians);
	}
}
namespace CU = CommonUtilities;

// = Operator
template <class T> CU::Vector2<T>& CU::Vector2<T>::operator=(const CU::Vector2<T> aVector2)
{
	x = aVector2.x;
	y = aVector2.y;
	return *this;
}
//Return sum of two Vector operators
template <class T>
inline CU::Vector2<T> operator+(const CU::Vector2<T>& aVector0, const CU::Vector2<T>& aVector1)
{
	return CU::Vector2<T>(aVector0.x + aVector1.x, aVector0.y + aVector1.y);
}
//Return the difference
template <class T>
inline CU::Vector2<T> operator-(const CU::Vector2<T>& aVector0, const CU::Vector2<T>& aVector1)
{
	return CU::Vector2<T>(aVector0.x - aVector1.x, aVector0.y - aVector1.y);
}
//Returns the vector aVector multiplied by the scalar aScalar
template <class T>
inline CU::Vector2<T> operator*(const CU::Vector2<T>& aVector, const T& aScalar)
{
	return CU::Vector2<T>(aVector.x * aScalar, aVector.y * aScalar);
}
//Returns the vector aVector multiplied by the scalar aScalar
template <class T>
inline CU::Vector2<T> operator*(const T& aScalar, const CU::Vector2<T>& aVector)
{
	return CU::Vector2<T>(aScalar * aVector.x, aScalar * aVector.y);
}
//Returns the vector aVector divided by the scalar aScalar (equivalent to aVector multiplied by 1/aScalar)
template <class T>
inline CU::Vector2<T> operator/(const CU::Vector2<T>& aVector, const T& aScalar)
{
	aVector.x / aScalar;
	aVector.y / aScalar;

	return aVector;
}
//Equivalent to setting aVector0 to (aVector0 + aVector1)
template <class T>
inline void operator+=(CU::Vector2<T>& aVector0, const CU::Vector2<T>& aVector1)
{
	aVector0.x += aVector1.x;
	aVector0.y += aVector1.y;
}
//Equivalent to setting aVector0 to (aVector0 - aVector1)
template <class T>
inline void operator-=(CU::Vector2<T>& aVector0, const CU::Vector2<T>& aVector1)
{
	aVector0.x -= aVector1.x;
	aVector0.y -= aVector1.y;
}
//Equivalent to setting aVector to (aVector * aScalar)
template <class T>
inline void operator*=(CU::Vector2<T>& aVector, const T& aScalar)
{
	aVector.x *= aScalar;
	aVector.y *= aScalar;
}
//Equivalent to setting aVector to (aVector / aScalar)
template <class T>
inline void operator/=(CU::Vector2<T>& aVector, const T& aScalar)
{
	aVector.x /= aScalar;
	aVector.y /= aScalar;
}
using Vector2f = CU::Vector2<float>;
using Vector2d = CU::Vector2<double>;
using Vector2i = CU::Vector2<int>;
