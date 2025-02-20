#pragma once
#pragma once
#include "Vector.h"
namespace CommonUtilities
{
	template<typename T>
	class Ray
	{
	public:
		union
		{
			Vector3<T> myDirection;
			Vector3<T> direction;
		};
		union
		{
			Vector3<T> myOrigin;
			Vector3<T> origin;
		};
		// Default constructor: there is no ray, the origin and direction are the 
		// zero vector.
		Ray();
		// Copy constructor.
		Ray(const Ray<T>& aRay);
		// Constructor that takes two points that define the ray, the direction is 
		// aDirectionm and the origin is aOrigin.
		Ray(const Vector3<T>& aOrigin, const Vector3<T>& aDirection);
		// Init the ray with two points, the same as the constructor above.
		void InitWith2Points(const Vector3<T>& aOrigin, const Vector3<T>& aPoint);
		// Init the ray with an origin and a direction.
		void InitWithOriginAndDirection(const Vector3<T>& aOrigin, const Vector3<T>& aDirection);

		Ray<T>& operator=(const Ray<T> aRay);

	};
	template<typename T>
	inline Ray<T>::Ray() :
		myOrigin({ 0,0,0 }),
		myDirection({ 0,0,0 })
	{
	}
	template<typename T>
	inline Ray<T>::Ray(const Ray<T>& aRay):
		myOrigin(aRay.origin),
		myDirection(Vector3f(aRay.direction).Normalize())
	{
	}
	template<typename T>
	inline Ray<T>::Ray(const Vector3<T>& aOrigin, const Vector3<T>& aDirection):
		myOrigin(aOrigin),
		myDirection(Vector3f(aDirection).Normalize())
	{
	}
	template<typename T>
	inline void Ray<T>::InitWith2Points(const Vector3<T>& aOrigin, const Vector3<T>& aPoint)
	{
		myOrigin = aOrigin;
		myDirection = Vector3f(aPoint- aOrigin).Normalize() ;
	}
	template<typename T>
	inline void Ray<T>::InitWithOriginAndDirection(const Vector3<T>& aOrigin, const Vector3<T>& aDirection)
	{
		myOrigin = aOrigin;
		myDirection = Vector3f(aDirection).Normalize();
	}
	template<typename T>
	inline Ray<T>& Ray<T>::operator=(const Ray<T> aRay)
	{
		myDirection = aRay.direction;
		myOrigin = aRay.origin;
		return *this;
	}
}
namespace CU = CommonUtilities;