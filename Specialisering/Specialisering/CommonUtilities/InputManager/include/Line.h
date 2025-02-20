#pragma once
#include "Vector.h"

namespace CommonUtilities
{
	template<typename T>
	class Line
	{
	public:
		//Variables
		union
		{
			CU::Vector2 <T> myPos0;
			CU::Vector2 <T> pos0;
		};
		union
		{
			CU::Vector2 <T> myPos1;
			CU::Vector2 <T> pos1;
		};
		//Constructors
		Line();
		Line(const Line<T>& aLine);
		Line(const CU::Vector2<T>& aPoint0, const CU::Vector2<T>& aPoint1);
		//Functions
		void InitWith2Points(const CU::Vector2<T>& aPoint0, const CU::Vector2<T>& aPoint1);
		void InitWithPointAndDirection(const Vector2<T>& aPoint, const Vector2<T>& aDirection);
		bool IsInside(const CU::Vector2<T>& aPoint) const;
		const CU::Vector2<T> GetDirection() const;
		const CU::Vector2<T> GetNormal() const;

	};
	//Constructors
	template<typename T> // Default const
	inline Line<T>::Line() :
		myPos0({ 0,0 }),
		myPos1({ 0,0 })
	{
	}
	template<typename T>// Copy const
	inline Line<T>::Line(const Line<T>& aLine) :
		myPos0(aLine.x),
		myPos1(aLine.y)
	{
	}
	template<typename T>	// Referensed const
	inline  Line<T>::Line(const CU::Vector2<T>& aPoint0, const CU::Vector2<T>& aPoint1)
	{
		myPos0 = aPoint0;
		myPos1 = aPoint1;
	}
	//Initializers
	template<typename T>
	inline void Line<T>::InitWith2Points(const CU::Vector2<T>& aPoint0, const CU::Vector2<T>& aPoint1)
	{
		myPos0 = aPoint0;
		myPos1 = aPoint1;
	}
	template<typename T>
	inline void Line<T>::InitWithPointAndDirection(const Vector2<T>& aPoint, const Vector2<T>& aDirection)
	{
		myPos0 = aPoint;
		myPos1 = aPoint + aDirection;
	}
	// Functions
	template<typename T>
	inline bool Line<T>::IsInside(const CU::Vector2<T>& aPoint) const
	{
		return GetNormal().Dot(aPoint - myPos0) <= 0;
	}
	template<typename T>
	inline const CU::Vector2<T> Line<T>::GetDirection() const
	{
		return myPos1 - myPos0;
	}
	template<typename T>
	inline const CU::Vector2<T> Line<T>::GetNormal() const
	{
		return Vector2<T>(-GetDirection().y, GetDirection().x).GetNormalized();
	}
}
namespace CU = CommonUtilities;