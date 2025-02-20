#pragma once
namespace CommonUtilities
{
	template<typename T>
	class Vector4
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
		union
		{
			T z;
			T myZ;
		};
		union
		{
			T w;
			T myW;
		};

		//Creates a null-vector
		Vector4<T>()
			: x(0), y(0), z(0), w(0)
		{
		}
		//Creates a vector (aX, aY, aZ)
		Vector4<T>(const T& aX, const T& aY, const T& aZ, const T& aW) : x(aX), y(aY), z(aZ), w(aW) {};
		//Copy Constructor
		Vector4<T>(const Vector4<T>& aVector) : x(aVector.x), y(aVector.y), z(aVector.z), w(aVector.w) { }
		//Assignment Operator
		Vector4<T>& operator=(const Vector4<T> aVector4);
		//Destructor
		~Vector4<T>() = default;
		//Return Square Root
		T LengthSqr() const;
		//Return Length of vector
		T Length();
		//Normalize
		Vector4<T> Normalize();
		//Returns the dot product of this and aVector
		T Dot(const Vector4<T>& aVector) const;
	};
	//Length squared of vector 4 
	template<typename T>
	inline T Vector4<T>::LengthSqr() const
	{
		return (x * x) + (y * y) + (z * z) + (w * w);
	}
	//Lenght of vector4
	template<typename T>
	inline T Vector4<T>::Length()
	{
		return sqrt((x * x) + (y * y) + (z * z) + (w * w));
	}
	//Get Normals
	template<typename T>
	inline Vector4<T> Vector4<T>::Normalize()
	{
		if (x == 0 && y == 0 && z == 0 && w == 0)
		{
			x = 0;
			y = 0;
			z = 0;
			w = 0;
		}
		else
		{
			*this /= Length();
		}
		return *this;
	}
	template<typename T>
	inline T Vector4<T>::Dot(const Vector4<T>& aVector) const
	{
		return x * aVector.x + y * aVector.y + z * aVector.z + w * aVector.w;
	}
}
namespace CU = CommonUtilities;

// = Operator
template <class T> CU::Vector4<T>& CU::Vector4<T>::operator=(const CU::Vector4<T> aVector4)
{
	x = aVector4.x;
	y = aVector4.y;
	z = aVector4.z;
	w = aVector4.w;

	return *this;
}
//Return sum of two Vector operators
template <class T> CU::Vector4<T> operator+(const CU::Vector4<T>& aVector0, const CU::Vector4<T>& aVector1)
{
	return CU::Vector4<T>(aVector0.x + aVector1.x, aVector0.y + aVector1.y, aVector0.z + aVector1.z, aVector0.w + aVector1.w);
}
//Return the difference
template <class T> CU::Vector4<T> operator-(const CU::Vector4<T>& aVector0, const CU::Vector4<T>& aVector1)
{
	return CU::Vector4<T>(aVector0.x - aVector1.x, aVector0.y - aVector1.y, aVector0.z - aVector1.z, aVector0.w - aVector1.w);
}
//Returns the vector aVector multiplied by the scalar aScalar
template <class T> CU::Vector4<T> operator*(const CU::Vector4<T>& aVector, const T& aScalar)
{
	auto newVector = aVector;
	newVector.x *= aScalar;
	newVector.y *= aScalar;
	newVector.z *= aScalar;
	newVector.w *= aScalar;
	return newVector;
}
//Returns the vector aVector multiplied by the scalar aScalar
template <class T> CU::Vector4<T> operator*(const T& aScalar, const CU::Vector4<T>& aVector)
{
	auto newVector = aVector;
	newVector.x *= aScalar;
	newVector.y *= aScalar;
	newVector.z *= aScalar;
	newVector.w *= aScalar;
	return newVector;
}
//Returns the vector aVector divided by the scalar aScalar (equivalent to aVector multiplied by 1/aScalar)
template <class T> CU::Vector4<T> operator/(const CU::Vector4<T>& aVector, const T& aScalar)
{
	aVector.x / aScalar;
	aVector.y / aScalar;
	aVector.z / aScalar;
	aVector.w / aScalar;

	return aVector;
}
//Equivalent to setting aVector0 to (aVector0 + aVector1)
template <class T> void operator+=(CU::Vector4<T>& aVector0, const CU::Vector4<T>& aVector1)
{
	aVector0.x += aVector1.x;
	aVector0.y += aVector1.y;
	aVector0.z += aVector1.z;
	aVector0.w += aVector1.w;
}
//Equivalent to setting aVector0 to (aVector0 - aVector1)
template <class T> void operator-=(CU::Vector4<T>& aVector0, const CU::Vector4<T>& aVector1)
{
	aVector0.x -= aVector1.x;
	aVector0.y -= aVector1.y;
	aVector0.z -= aVector1.z;
	aVector0.w -= aVector1.w;
}
//Equivalent to setting aVector to (aVector * aScalar)
template <class T> void operator*=(CU::Vector4<T>& aVector, const T& aScalar)
{
	aVector.x *= aScalar;
	aVector.y *= aScalar;
	aVector.z *= aScalar;
	aVector.w *= aScalar;
}
//Equivalent to setting aVector to (aVector / aScalar)
template <class T> void operator/=(CU::Vector4<T>& aVector, const T& aScalar)
{
	aVector.x /= aScalar;
	aVector.y /= aScalar;
	aVector.z /= aScalar;
	aVector.w /= aScalar;
}
using Vector4f = CU::Vector4<float>;
using Vector4d = CU::Vector4<double>;
using Vector4i = CU::Vector4<int>;



