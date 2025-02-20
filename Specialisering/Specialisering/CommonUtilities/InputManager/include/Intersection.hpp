#pragma once
#include "Plane.h"
#include "Sphere.hpp"
#include "AABB3D.hpp"
#include "Ray.hpp"
#include <algorithm>
#define OUT
namespace CommonUtilities
{

	//Plane raycast
	template<typename T>
	bool IntersectionPlaneRay(const Plane<T>& aPlane, const Ray<T>& aRay, OUT Vector3<T>& aOutIntersectionPoint, const T aMaxDistance = 0);
	//AABB raycast
	template<typename T>
	bool IntersectionAABBRay(const AABB3D<T>& aAABB, const Ray<T>& aRay, OUT Vector3<T>& aOutIntersectionPoint, const T aMaxDistance = 0);
	//Sphere raycast
	template<typename T>
	bool IntersectionSphereRay(const Sphere<T>& aSphere, const Ray<T>& aRay, OUT Vector3<T>& aOutIntersectionPoint, const T aMaxDistance = 0);



	//Plane  Raycast
	template<typename T>
	bool IntersectionPlaneRay(const Plane<T>& aPlane, const Ray<T>& aRay, OUT Vector3<T>& aOutIntersectionPoint, const T aMaxDistance)
	{
		if (aPlane.IsInside(aRay.origin))
		{
			return false;
		}
		T determinator = aPlane.normal.Dot(aRay.direction);

		//Check for paralell 
		if (determinator != 0)
		{
			T t = aPlane.normal.Dot(aPlane.point - aRay.origin) / determinator;
			if (t >= 0 || (t < 0 && (aRay.origin - aPlane.point).Dot(aPlane.normal) == 0))
			{
				//Check if the ray hits inside the max distance
				if (aMaxDistance > 0 && CU::Vector3<T>(aRay.origin + aRay.direction * t).Length() > aMaxDistance)
				{
					//The target is out of range!
					return false;
				}
				//Set the outersection point
				aOutIntersectionPoint = aRay.origin + aRay.direction * t;

				//Intersection found!
				return true;
			}
		}
		else
		{
			// Check if the ray's origin is on the plane
			if ((aRay.origin - aPlane.point).Dot(aPlane.normal) == 0)
			{

				aOutIntersectionPoint = aRay.origin;
				//Intersection found!
				return true;

			}
		}
		//No intersection found!
		return false;
	}
	//AABB Raycast
	template<typename T>
	bool IntersectionAABBRay(const AABB3D<T>& aAABB, const Ray<T>& aRay, OUT Vector3<T>& aOutIntersectionPoint, const T aMaxDistance)
	{
		//Check if the origin point is within the AABB
		if (aAABB.IsInside(aRay.origin))
		{
			//The ray is inside the AABB
			aOutIntersectionPoint = aRay.origin;
			return true;
		}
		//Base variabels for readability
		CU::Vector3<T> ray(aRay.direction);
		CU::Vector3<T> v1 = aAABB.minPos;
		CU::Vector3<T> v2 = aAABB.maxPos;


		//Gets the closest hit point and the one futherest away
		CU::Vector3<T> dis1 = { std::min(ray.x - v1.x, ray.x - v2.x) ,std::min(ray.y - v1.y, ray.y - v2.y), std::min(ray.z - v1.z, ray.z - v2.z) };
		CU::Vector3<T> dis2 = { std::max(ray.x - v1.x, ray.x - v2.x) ,std::max(ray.y - v1.y, ray.y - v2.y), std::max(ray.z - v1.z, ray.z - v2.z) };

		//Check each axis if the ray hits 
		if (v1.x <= dis1.x && dis1.x <= v2.x || v2.x <= dis1.x && dis1.x <= v1.x)
		{
			if (v1.y <= dis1.y && dis1.y <= v2.y || v2.y <= dis1.y && dis1.y <= v1.y)
			{
				if (v1.z <= dis1.z && dis1.z <= v2.z || v2.z <= dis1.z && dis1.z <= v1.z)
				{
					//Distance Check
					if (aMaxDistance > 0 && CU::Vector3<T>(dis2 + aRay.origin).Length() > aMaxDistance)
					{
						//Target is too far away!
						return false;
					}
					//Set the outersection point
					aOutIntersectionPoint = dis2 + aRay.origin;
					//Intersection found!
					return true;
				}
			}
		}
		//Check if the ray's origin is inside the box 
		if (aAABB.IsInside(aRay.origin))
		{
			//Set the outersection point
			aOutIntersectionPoint = dis2 + aRay.origin;
			//Intersection found!
			return true;
		}
		//No intersection found
		return false;
	}
	//Sphere Raycast
	template<typename T>
	bool IntersectionSphereRay(const Sphere<T>& aSphere, const Ray<T>& aRay, OUT Vector3<T>& aOutIntersectionPoint, const T aMaxDistance)
	{


		CU::Vector3<T> rayToSphere = aSphere.centerPosition - aRay.origin;
		T t = rayToSphere.Dot(aRay.direction);
		bool isInside = aSphere.IsInside(aRay.origin);

		if (t < T{})
		{
			return false;
		}
		CU::Vector3<T> closestPoint = aRay.origin + aRay.direction * t;
		if (aSphere.IsInside(closestPoint))
		{
			Vector3<T> yV = aSphere.centerPosition - closestPoint;
			T ySqr = yV.LengthSqr();
			T x = sqrt((aSphere.radius * aSphere.radius) - ySqr);

			if (aMaxDistance != 0 && t - x > aMaxDistance)
			{
				return false;
			}

			aOutIntersectionPoint = aRay.origin + aRay.direction * (t - x);
			return true;
		}
		else
		{
			//No Intersection detected!
			return false;
		}
	}
}
namespace CU = CommonUtilities;