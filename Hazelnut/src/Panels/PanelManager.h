#pragma once

#include "Hazel/Core/Log.h"

#include <string>
#include <unordered_map>

namespace Hazel {

	class EditorPanel
	{
	public:
		virtual void OnImGuiRender() = 0;
	};

	struct PanelItem
	{
		EditorPanel* Panel = nullptr;
		bool Displayed = false;

		PanelItem() = default;
		PanelItem(EditorPanel* panel, bool display)
			: Panel(panel), Displayed(display) {}
	};

	class PanelManager
	{
	public:
		static std::unordered_map<std::string, PanelItem>& GetPanels()
		{
			static std::unordered_map<std::string, PanelItem> m_Panels;
			return m_Panels;
		}

		static EditorPanel* Get(const std::string& name)
		{
			auto& panels = GetPanels();

			if (panels.count(name) == 0)
			{
				HZ_WARN("Panel was not registered with manager");
				return nullptr;
			}

			return panels[name].Panel;
		}

		template<typename T>
		static T* Get(const std::string& name)
		{
			static_assert(std::is_base_of<EditorPanel, T>());
			return dynamic_cast<T*>(Get(name));
		}

		template<typename T>
		static T* Register(const std::string& name, bool display = true)
		{
			static_assert(std::is_base_of<EditorPanel, T>());

			auto& panels = GetPanels();
			HZ_ASSERT(panels.count(name) == 0, "Can't register panel that is already being managed! (Check name is not already in use)");

			T* newPanel = new T;
			panels.emplace(name, PanelItem(newPanel, display));

			return newPanel;
		}

		template<typename T>
		static T* Register(const std::string& name, T* panel, bool display = true)
		{
			static_assert(std::is_base_of<EditorPanel, T>()::value);

			auto& panels = GetPanels();
			HZ_ASSERT(panels.count(name) == 0, "Can't register panel that is already being managed! (Check name is not already in use)");

			panels.emplace(name, PanelItem(panel, display));

			return panel;
		}

		static void Clear() 
		{
			for (auto& [key, panel] : GetPanels())
				delete panel.Panel;

			GetPanels().clear(); 
		}

		static void Delete(const std::string& name)
		{
			auto& panels = GetPanels();

			if (panels.count(name) == 0)
			{
				HZ_WARN("Panel was not registered with manager");
				return;
			}

			delete panels[name].Panel;
			panels.erase(name);
		}

		static void RenderPanels()
		{
			for (auto& [key, panel] : GetPanels())
			{
				if (panel.Panel && panel.Displayed) panel.Panel->OnImGuiRender();
			}
		}
	};

}
