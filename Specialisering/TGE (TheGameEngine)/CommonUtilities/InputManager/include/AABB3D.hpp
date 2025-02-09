#pragma once
#include "Vector.h"
namespace CommonUtilities
{
	template<typename T>
	class AABB3D
	{
	public:
		union
		{
			Vector3<T> myMinPos;
			Vector3<T> minPos;
		};
		union
		{
			Vector3<T> myMaxPos;
			Vector3<T> maxPos;
		};
		// Default constructor: there is no AABB, both min and max points are the zero vector.
		AABB3D();
		// Copy constructor.
		AABB3D(const AABB3D<T>& aAABB3D);
		// Constructor taking the positions of the minimum and maximum corners.
		AABB3D(const Vector3<T>& aMin, const Vector3<T>& aMax);
		// Init the AABB with the positions of the minimum and maximum corners, same as
		// the constructor above.
		void InitWithMinAndMax(const Vector3<T>& aMin, const Vector3<T>& aMax);
		// Returns whether a point is inside the AABB: it is inside when the point is on any
		// of the AABB's sides or inside of the AABB.
		bool IsInside(const Vector3<T>& aPosition) const;
	};

	template<typename T>
	inline AABB3D<T>::AABB3D() :
		myMinPos({ 0,0,0 }),
		myMaxPos({ 0,0,0 })
	{
	}
	template<typename T>
	inline AABB3D<T>::AABB3D(const AABB3D<T>& aAABB3D) :
		myMinPos(aAABB3D.minPos),
		myMaxPos(aAABB3D.maxPos)
	{
	}
	template<typename T>
	inline AABB3D<T>::AABB3D(const Vector3<T>& aMin, const Vector3<T>& aMax) :
		myMinPos(aMin),
		myMaxPos(aMax)
	{
	}
	template<typename T>
	inline void AABB3D<T>::InitWithMinAndMax(const Vector3<T>& aMin, const Vector3<T>& aMax)
	{
		myMinPos = aMin;
		myMaxPos = aMax;
	}
	template<typename T>
	inline bool AABB3D<T>::IsInside(const Vector3<T>& aPosition) const
	{

		//Check x Axis
		if (aPosition.x <= myMaxPos.x && myMinPos.x <= aPosition.x || aPosition.x <= myMinPos.x && myMaxPos.x <= myMinPos.x)
		{
			//Check y Axis
			if (aPosition.y <= myMaxPos.y && myMinPos.y <= aPosition.y || aPosition.y <= myMinPos.y && myMaxPos.y <= aPosition.y)
			{
				//Check z Axis
				if (aPosition.z <= myMaxPos.z && myMinPos.z <= aPosition.z || aPosition.z <= myMinPos.z && myMaxPos.z <= aPosition.z)
				{
					return true;
				}
			}
		}
		return false;
	}
}
namespace CU = CommonUtilities;