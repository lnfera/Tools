#include <functional>
namespace Tools
{
	/*
		
	
	*/


	template<typename ClassType, typename FuncType, typename...Args>
	void CallFunction(ClassType* obj, FuncType func, Args&&... args)
	{
		(obj->*func)(std::forward<Args>(args)...);
	};
}