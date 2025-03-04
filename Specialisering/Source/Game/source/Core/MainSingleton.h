#pragma once
#include <memory>
namespace Tga
{
	class ShaderManager;
	class MainDrawer;
}

namespace Tga
{
	class MainSingleton
	{
	public: // Singleton Pattern
		inline static MainSingleton& GetInstance()
		{
			static MainSingleton instance; // This is the Singleton instance
			return instance;
		}
		MainSingleton(const MainSingleton&) = delete;
		MainSingleton& operator=(const MainSingleton&) = delete;
	public:
		void Init();

		ShaderManager* GetShaderManager() { return myShaderManager.get(); }
		MainDrawer* GetMainDrawer() { return myMainDrawer.get(); }

	private: // Singleton Pattern
		// Behind the scenes
		MainSingleton() {};
		~MainSingleton() {};
	private:
		std::shared_ptr<ShaderManager> myShaderManager;
		std::shared_ptr<MainDrawer> myMainDrawer;
	};
}

#define MSI_() Tga::MainSingleton::GetInstance()