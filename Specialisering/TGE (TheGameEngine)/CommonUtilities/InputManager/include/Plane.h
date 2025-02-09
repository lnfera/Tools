#pragma once
#include "Vector3.h"
namespace CommonUtilities
{
	template <typename T>
	class Plane
	{
	public:
		union
		{
			Vector3<T> myPoint;
			Vector3<T> point;
		};
		union
		{
			Vector3<T> myNormal;
			Vector3<T> normal;
		};
		//Constructors
		Plane();
		Plane(const Vector3<T>& aPoint0, const Vector3<T>& aPoint1, const Vector3<T>& aPoint2);
		Plane(const Vector3<T>& aPoint0, const Vector3<T>& aNormal);
		Plane(const Plane<T>& aPlane);
		//Init
		void InitWith3Points(const Vector3<T>& aPoint0, const Vector3<T>& aPoint1, const Vector3<T>& aPoint2);
		void InitWithPointAndNormal(const Vector3<T>& aPoint, const Vector3<T>& aNormal);
		//Operators 
		Plane<T>& operator=(const Plane<T> aPlane);
		//Destructor
		~Plane<T>() = default;
		//Functions
		bool IsInside(const Vector3<T>& aPosition) const;
		const Vector3<T>& GetNormal() const;
	};
	//Constructors
	template<typename T>
	inline Plane<T>::Plane() :
		myPoint({ 0,0,0 }),
		myNormal({ 0,0,0 })
	{
	}
	template<typename T>
	inline Plane<T>::Plane(const Vector3<T>& aPoint0, const Vector3<T>& aPoint1, const Vector3<T>& aPoint2)
	{
		InitWith3Points(aPoint0, aPoint1, aPoint2);
	}
	template<typename T>
	inline Plane<T>::Plane(const Vector3<T>& aPoint0, const Vector3<T>& aNormal) :
		myPoint(aPoint0),
		myNormal(aNormal)
	{
	}

	template<typename T>
	inline Plane<T>::Plane(const Plane<T>& aPlane):
		myNormal(aPlane.normal),
		myPoint(aPlane.point)
	{
		
	}
	//Init
	template<typename T>
	inline void Plane<T>::InitWith3Points(const Vector3<T>& aPoint0, const Vector3<T>& aPoint1, const Vector3<T>& aPoint2)
	{
		CU::Vector3<T> v0 = aPoint2 - aPoint0;
		CU::Vector3<T> v1 = aPoint1 - aPoint0;
		myNormal = v1.Cross(v0).Normalize();
		myPoint = aPoint0;
	}
	template<typename T>
	inline void Plane<T>::InitWithPointAndNormal(const Vector3<T>& aPoint, const Vector3<T>& aNormal)
	{
		myPoint = aPoint;
		myNormal = aNormal;
	}
	//Functions
	template<typename T>
	inline bool Plane<T>::IsInside(const Vector3<T>& aPosition) const
	{
		if (CU::Vector3<T>(aPosition - myPoint).Dot(myNormal) <= 0)
		{
			return true;
		}
		return false;
	}
	template<typename T>
	inline const Vector3<T>& Plane<T>::GetNormal() const
	{
		return myNormal;
	}
}
namespace CU = CommonUtilities;
//Operator = 
template <typename T>
inline CU::Plane<T>& CU::Plane<T>::operator=(const CU::Plane<T> aPlane)
{
	myNormal = aPlane.normal;
	myPoint = aPlane.point;
	return *this;
}