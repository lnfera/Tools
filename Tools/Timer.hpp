#pragma once
#include <chrono>
namespace Tools
{
	class Timer
	{
	public:
		Timer()
		{
			myStart = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
		}
		~Timer() = default;

		//Call this once per frame
		void Update()
		{
			double timeNow = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
			myTotalTime = (float)(timeNow - myStart);
			myDeltaTime = (float)(timeNow - myPreviousTime);
			myPreviousTime = timeNow;
		}

		float GetDeltaTime() { return myDeltaTime; }
		float GetTotalTime() { return myTotalTime;  }

	private:
		double myStart = 0.f;
		double myPreviousTime = 0.f;
		float myDeltaTime = 0.f;
		float myTotalTime = 0.f;
	};

}