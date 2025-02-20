#pragma once
#include <chrono>
namespace CommonUtilities
{
	class Timer
	{
	public:

		Timer();
		Timer(const Timer& aTimer) = delete;
		Timer& operator=(const Timer& aTimer) = delete;
		void Update();

		float GetDeltaTime() const;
		double GetTotalTime() const;
	private:
		double myStart;
		double myPreviousTime;
		float myDeltaTime = 0.0f;
		double myTotalTime = 0.0f;
	};
}
namespace CU = CommonUtilities;

