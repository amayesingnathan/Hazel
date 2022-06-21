#pragma once

#include "PanelManager.h"

#include "Hazel/Renderer/Texture.h"

#include <filesystem>

namespace Hazel {

	class ContentBrowserPanel : public EditorPanel
	{
	public:
		ContentBrowserPanel();

		void OnImGuiRender() override;
	private:
		std::filesystem::path m_CurrentDirectory;
		
		Ref<Texture2D> m_DirectoryIcon;
		Ref<Texture2D> m_FileIcon;
	};

}
