#pragma once

struct ImFont;

namespace Tga
{
	struct ImGuiInterfaceImpl;
	class ImGuiInterface
	{
	public:
		static void Init();
		static void PreFrame();
		static void Render();
		static void Shutdown();

		static ImFont* GetIconFontLarge();

	private:
		static std::unique_ptr<ImGuiInterfaceImpl> myImpl;
	};


}
