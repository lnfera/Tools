#pragma once
#include "Vector3.h"
namespace CommonUtilities
{
	template<typename T>
	class Sphere
	{
	public:
		union
		{
			T myRadius;
			T radius;
		};
		union
		{
			CU::Vector3<T> myCenter;
			CU::Vector3<T> centerPosition;
		};
		//Default Constructor, The 0 Constructor
		Sphere();
		//Copy Constructor, Copies values from selected sphere
		Sphere(const Sphere<T>& aSphere);
		// Constructor that takes the center position and radius of the sphere.
		Sphere(const Vector3<T>& aCenter, T aRadius);
		// Init the sphere with a center and a radius, the same as the constructor above.
		void InitWithCenterAndRadius(const Vector3<T>& aCenter, T aRadius);
		// Returns whether a point is inside the sphere: it is inside when the point is on the
		// Sphere surface or inside of the sphere.
		bool IsInside(const Vector3<T>& aPosition) const;

		Sphere<T>& operator=(const Sphere aSphere);

	private:

	};
	template<typename T>
	inline Sphere<T>::Sphere() :
		myCenter({ 0,0,0 }),
		myRadius(0)
	{
	}
	template<typename T>
	inline Sphere<T>::Sphere(const Sphere<T>& aSphere) :
		myCenter(aSphere.centerPosition),
		myRadius(aSphere.radius)
	{
	}
	template<typename T>
	inline Sphere<T>::Sphere(const Vector3<T>& aCenter, T aRadius) :
		myCenter(aCenter),
		myRadius(aRadius)
	{
	}
	template<typename T>
	inline void Sphere<T>::InitWithCenterAndRadius(const Vector3<T>& aCenter, T aRadius)
	{
		myCenter = aCenter;
		myRadius = aRadius;
	}
	template<typename T>
	inline bool Sphere<T>::IsInside(const Vector3<T>& aPosition) const
	{
		CU::Vector3<T> distance = myCenter - aPosition;

		if (distance.Length() <= myRadius)
		{
			return true;
		}
		return false;

	}
	template<typename T>
	inline Sphere<T>& Sphere<T>::operator=(const Sphere aSphere)
	{
		myCenter = aSphere.centerPosition;
		myRadius = aSphere.radius;
		return *this;
	}
}
namespace CU = CommonUtilities;
