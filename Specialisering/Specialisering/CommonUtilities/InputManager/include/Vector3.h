#pragma once
#include <cmath>
namespace CommonUtilities
{
	template<typename T>
	class Vector3
	{
	public:
		T x;
		T y;
		T z;

		//Creates a null-vector
		Vector3<T>() : x(0), y(0), z(0) {}
		//Creates a vector (aX, aY, aZ)
		Vector3<T>(const T& aX, const T& aY, const T& aZ) : x(aX), y(aY), z(aZ) {}
		//Copy Constructor
		Vector3<T>(const Vector3<T>& aVector)
			:
			x(aVector.x),
			y(aVector.y),
			z(aVector.z)
		{ }
		//Assignment Operator
		Vector3<T>& operator=(const Vector3<T> aVector3);
		//Destructor
		~Vector3<T>() = default;
		//Return Square Root
		T LengthSqr() const;
		//Return Length of vector
		T Length();
		//Normalize
		Vector3<T> Normalize();
		//Returns the dot product of this and aVector
		T Dot(const Vector3<T>& aVector) const;
		//Returns the cross product of this and aVector
		Vector3<T> Cross(const Vector3<T>& aVector) const;

		bool operator==(const Vector3<T> aVector);

	};
	//Length squared of vector 4 
	template<typename T>
	inline T Vector3<T>::LengthSqr() const
	{
		return (x * x) + (y * y) + (z * z);
	}
	//Lenght of Vector3
	template<typename T>
	inline T Vector3<T>::Length()
	{
		return sqrt((x * x) + (y * y) + (z * z));
	}
	//Get Normals
	template<typename T>
	inline Vector3<T> Vector3<T>::Normalize()
	{
		if (x == 0 && y == 0 && z == 0)
		{
			x = 0;
			y = 0;
			z = 0;
		}
		else
		{
			(*this) /= Length();
		}
		return *this;
	}
	template<typename T>
	inline T Vector3<T>::Dot(const Vector3<T>& aVector) const
	{
		return x * aVector.x + y * aVector.y + z * aVector.z;
	}
	template<typename T>
	inline Vector3<T> Vector3<T>::Cross(const Vector3<T>& aVector) const
	{
			return Vector3<T>(y * aVector.z - z * aVector.y, z * aVector.x - x*aVector.z,x*aVector.y - y * aVector.x);
	}
	template<typename T>
	inline bool Vector3<T>::operator==(const Vector3<T> aVector)
	{
		if (x == aVector.x && y == aVector.y && z == aVector.z)
		{
			return true;
		}
		return false;
	}
}

namespace CU = CommonUtilities;

// = Operator
template <class T>
inline CU::Vector3<T>& CU::Vector3<T>::operator=(const CU::Vector3<T> aVector3)
{
	// Check for self-assignment
	if (this != &aVector3)
	{
		x = aVector3.x;
		y = aVector3.y;
		z = aVector3.z;
	}
	return *this;
}
//Return sum of two Vector operators
template <class T>
inline CU::Vector3<T> operator+(const CU::Vector3<T>& aVector0, const CU::Vector3<T>& aVector1)
{

	return CU::Vector3<T>(aVector0.x + aVector1.x, aVector0.y + aVector1.y, aVector0.z + aVector1.z);
}
//Return the difference
template <class T>
inline CU::Vector3<T> operator-(const CU::Vector3<T>& aVector0, const CU::Vector3<T>& aVector1)
{

	return CU::Vector3<T>(aVector0.x - aVector1.x, aVector0.y - aVector1.y, aVector0.z - aVector1.z);
}
//Returns the vector aVector multiplied by the scalar aScalar
template <class T>
inline CU::Vector3<T> operator*(const CU::Vector3<T>& aVector, const T& aScalar)
{
	auto newVector = aVector;
	newVector.x *= aScalar;
	newVector.y *= aScalar;
	newVector.z *= aScalar;
	return newVector;
}
//Returns the vector aVector multiplied by the scalar aScalar
template <class T>
inline CU::Vector3<T> operator*(const T& aScalar, const CU::Vector3<T>& aVector)
{
	auto newVector = aVector;
	newVector.x *= aScalar;
	newVector.y *= aScalar;
	newVector.z *= aScalar;
	return newVector;
}
template <class T>
inline bool operator!=(const CU::Vector3<T>& aVector, const CU::Vector3<T>& bVector)
{
	return(aVector.x != bVector.x && aVector.y != bVector.y && aVector.z != bVector.z);
};
template <class T>
inline CU::Vector3<T> operator*=( CU::Vector3<T>& aVector, const CU::Vector3<T>& bVector)
{
	aVector.x *= bVector.x;
	aVector.y *= bVector.y;
	aVector.z *= bVector.z;
	return aVector;
}
template <class T>
inline CU::Vector3<T> operator*(const CU::Vector3<T>& aVector, const CU::Vector3<T>& bVector)
{
	CU::Vector3<T> vec;
	vec.x = aVector.x * bVector.x;
	vec.y = aVector.y * bVector.y;
	vec.z = aVector.z * bVector.z;
	return vec;
}
//Returns the vector aVector divided by the scalar aScalar (equivalent to aVector multiplied by 1/aScalar)

template <class T>
inline CU::Vector3<T> operator/(const CU::Vector3<T>& aVector, const T& aScalar)
{
	aVector.x / aScalar;
	aVector.y / aScalar;
	aVector.z / aScalar;

	return aVector;
}
//Equivalent to setting aVector0 to (aVector0 + aVector1)
template <class T>
inline void operator+=(CU::Vector3<T>& aVector0, const CU::Vector3<T>& aVector1)
{
	aVector0.x += aVector1.x;
	aVector0.y += aVector1.y;
	aVector0.z += aVector1.z;
}
//Equivalent to setting aVector0 to (aVector0 - aVector1)
template <class T>
inline void operator-=(CU::Vector3<T>& aVector0, const CU::Vector3<T>& aVector1)
{
	aVector0.x -= aVector1.x;
	aVector0.y -= aVector1.y;
	aVector0.z -= aVector1.z;
}
//Equivalent to setting aVector to (aVector * aScalar)
template <class T>
inline void operator*=(CU::Vector3<T>& aVector, const T& aScalar)
{
	aVector.x *= aScalar;
	aVector.y *= aScalar;
	aVector.z *= aScalar;
}
//Equivalent to setting aVector to (aVector / aScalar)
template <class T>
inline void operator/=(CU::Vector3<T>& aVector, const T& aScalar)
{
	aVector.x /= aScalar;
	aVector.y /= aScalar;
	aVector.z /= aScalar;
}

using Vector3f = CU::Vector3<float>;
using Vector3d = CU::Vector3<double>;
using Vector3i = CU::Vector3<int>;


