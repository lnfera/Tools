#pragma once
#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <tuple>
namespace Tools
{
	class ThreadPool
	{
	public:
		inline ThreadPool(size_t aNumOfThreads);
		inline ~ThreadPool();

		template<typename ClassRef, typename FuncRef, typename...Args>
		inline void EnqueTask(ClassRef* obj, FuncRef func, Args&&... args)
		{
			{
				std::lock_guard<std::mutex> lock(myMtx);

				auto argsTuple = std::make_tuple(std::forward<Args>(args)...);

				// Capture `args...` by perfect forwarding within the lambda
				myTasks.push([obj, func, ...capturedArgs = std::forward<Args>(args)]() mutable {
					CallFunction(obj, func, std::forward<decltype(capturedArgs)>(capturedArgs)...);
					});
			}
			myReleaseCondition.notify_one();  // Notify one worker thread
		}
		inline void EnqueTask(std::function<void()> aTask)
		{
			{
				std::lock_guard<std::mutex> lock(myMtx);
				myTasks.push(aTask);
			}
			myReleaseCondition.notify_one();
		}

		inline void WaitForTasksToFinish();

	private:
		void WorkerThread();

		template<typename ClassType, typename FuncType, typename...Args>
		static void CallFunction(ClassType* obj, FuncType func, Args&&... args)
		{
			(obj->*func)(std::forward<Args>(args)...);
		};
	private:
		std::vector<std::thread> myWorkers;
		std::queue<std::function<void()>> myTasks;
		std::condition_variable myReleaseCondition;
		std::condition_variable myWaitForAllTasksCondition;
		std::mutex myMtx;
		bool myShouldStop = false;
		std::atomic<int> myActiveTasks = 0;
	};

	ThreadPool::ThreadPool(size_t aNumOfThreads)
	{
		for (size_t i = 0; i < aNumOfThreads; ++i) {
			myWorkers.emplace_back(&ThreadPool::WorkerThread, this);
		}
	}
	inline ThreadPool::~ThreadPool()
	{
		myShouldStop = true;
		myReleaseCondition.notify_all(); // Wake up all threads to finish execution

		for (std::thread& worker : myWorkers)
		{
			worker.join();      // Wait for each thread to finish
		}
	}

	inline void ThreadPool::WaitForTasksToFinish()
	{
		std::unique_lock<std::mutex> lock(myMtx);
		myWaitForAllTasksCondition.wait(lock, [this]() {return myTasks.empty()&& myActiveTasks <= 0; });
	}

	inline void ThreadPool::WorkerThread()
	{
		while (!myShouldStop)
		{
			std::function<void()> task;
			{
				//Locks down the bracket
				std::unique_lock<std::mutex> lock(myMtx);
				myReleaseCondition.wait(lock, [this]() {return myShouldStop || !myTasks.empty(); });

				if (myShouldStop && myTasks.empty()) return;

				task = std::move(myTasks.front());	// Get task from queue
				myTasks.pop();
			}

			if (task)
			{
				myActiveTasks++;
				task();  // Execute the task
				myActiveTasks--;
			}

			myWaitForAllTasksCondition.notify_all();
		}
	}

}