// ======================================================================
// File Name:		LevelEditor.cpp
// Project Name:	Zoodiac
// Author(s):		Lim Geng Yang(50%), Low Zhi Sheng Kitson(50%)
// Brief:			Contains State Functionality
// 
// All content © 2023 DigiPen Institute of Technology Singapore. All 
// rights reserved.
// ======================================================================

#ifndef LEVEL_EDITOR_H_
#define LEVEL_EDITOR_H_

#include <iostream>
#include <stack>
#include <tuple>

#include <AEEngine.h>
#include <Backend/Engine_Common.h>
#include <Backend/Utils.h>
#include <Backend/VecOperators.h>
#include <Backend/ECS/ECS_Factory.h>
#include <Backend/ECS/ECS_IncludeComponents.h>
#include <Backend/JSONSerializer.h>

namespace LevelEditor
{
	using LevelEditorSystem = Backend::ECS::ECS_System<Backend::ECS::Component::IsometricTransformComponent, Backend::ECS::Component::SpriteComponent>;
	using PrefabSystem = Backend::ECS::ECS_System <Backend::ECS::Component::PrefabComponent>;
	using PlayerSystem = Backend::ECS::ECS_System<Backend::ECS::Component::PlayerComponent, Backend::ECS::Component::IsometricTransformComponent>;

	/**
	 * \brief Generates the Level Editor System.
	 * \tparam Scene The scene.
	 * \return The Level Editor System.
	 */
	template<int Scene>
	auto GenerateLevelEditorSystem() -> LevelEditorSystem&
	{
		static std::stack<Backend::ECS::ECS_Entity> recentlyAddedEntites;
		static std::stack<Backend::ECS::Component::PrefabComponent> recentlyRemovedEntites;

		static AEVec2 mousePos;
		static int layer = 0;
		static size_t index = 0;
		static bool genTile = false, destroyTile = false;
		static float offHeight = static_cast<f32>(AEGetWindowHeight()) / 2.0f,
			offWidth = static_cast<f32>(AEGetWindowWidth()) / 2.0f;
		static const auto& prefabs = PREFAB_LIBRARY.GetAllKeys();
		static Backend::ECS::ECS_Entity currEntity = Backend::ECS::ECS_MaxEntities + 1;
		static auto preUpdateFunc = []()
		{
			if (currEntity > Backend::ECS::ECS_MaxEntities)
				currEntity = Backend::ECS::Factory::ECS_Factory::CreatePrefabEntity(prefabs[index]);

			s32 screenX, screenY;
			f32 camX, camY;
			if (AEInputCheckTriggered(AEVK_UP))
				std::cout << "Layer: " << ++layer << "\nTile: " << prefabs[index] << "\n";
			else if (AEInputCheckTriggered(AEVK_DOWN))
				std::cout << "Layer: " << --layer << "\nTile: " << prefabs[index] << "\n";
			if (AEInputCheckTriggered(AEVK_LEFT) && index > 0)
				Backend::ECS::Factory::ECS_Factory::CreatePrefabEntity(prefabs[--index], currEntity);
			else if (AEInputCheckTriggered(AEVK_RIGHT) && index < prefabs.size() - 1)
				Backend::ECS::Factory::ECS_Factory::CreatePrefabEntity(prefabs[++index], currEntity);
			if (AEInputCheckCurr(AEVK_LBUTTON))
				genTile = true;
			else genTile = false;
			if (AEInputCheckCurr(AEVK_RBUTTON))
				destroyTile = true;
			else destroyTile = false;
			if (AEInputCheckCurr(AEVK_LCTRL))
			{
				if (AEInputCheckCurr(AEVK_Z) && !recentlyAddedEntites.empty())
				{
					recentlyRemovedEntites.push(ECS_WORLD.GetComponent<Backend::ECS::Component::PrefabComponent>(recentlyAddedEntites.top()));
					ECS_WORLD.FlagEntityForDeletion(recentlyAddedEntites.top());
					recentlyAddedEntites.pop();
				}
				else if (AEInputCheckCurr(AEVK_Y) && !recentlyRemovedEntites.empty())
				{
					const auto prefabData = recentlyRemovedEntites.top();
					const auto entity = Backend::ECS::Factory::ECS_Factory::CreatePrefabEntity(prefabData.m_Key);
					auto& prefabComponent = ECS_WORLD.AddComponent<Backend::ECS::Component::PrefabComponent>(entity);
					auto& transform = ECS_WORLD.GetComponent<Backend::ECS::Component::IsometricTransformComponent>(entity);
					prefabComponent.m_Height = transform.m_Height = prefabData.m_Height;
					prefabComponent.m_Pos = transform.m_Position = prefabData.m_Pos;
					prefabComponent.m_Key = prefabData.m_Key;
					recentlyRemovedEntites.pop();
					recentlyAddedEntites.push(entity);
				}
			}

			AEInputGetCursorPosition(&screenX, &screenY);
			AEGfxGetCamPosition(&camX, &camY);
			mousePos = { static_cast<f32>(screenX) + camX - offWidth, static_cast<f32>(-screenY) + camY + offHeight };
			auto& transform = ECS_WORLD.GetComponent<Backend::ECS::Component::IsometricTransformComponent>(currEntity);
			mousePos *= Backend::ECS::Component::IsometricTransformComponent::s_IsoInverse;
			Backend::SnapToGrid(mousePos);
			transform.m_Height = static_cast<float>(layer) * 30.0f;
			transform.m_Position = mousePos;
		};

		static auto updateFunc =
			[](const std::tuple<Backend::ECS::Component::IsometricTransformComponent*, Backend::ECS::Component::SpriteComponent*>& _components)
		{
			auto& transform = std::get<Backend::ECS::Component::IsometricTransformComponent*>(_components);
			auto& sprite = std::get<Backend::ECS::Component::SpriteComponent*>(_components);
			if (transform->Entity() == currEntity)
				return;
			const bool heightCheck = transform->m_Height == layer * 30.0f || transform->m_Height == 1000;
			if (transform->m_Position == mousePos)
			{
				if (heightCheck)
				{
					sprite->m_Blue = 0;
					sprite->m_Green = 0;
				}
				else
				{
					sprite->m_Red = 0;
					sprite->m_Green = 0;
					return;
				}
			}
			else return;
			if (!genTile && !destroyTile)
				return;
			genTile = false;
			if (destroyTile)
			{
				ECS_WORLD.FlagEntityForDeletion(transform->Entity());
				destroyTile = false;
				std::cout << "Tile destroyed\n";
			}
		};

		static auto postUpdateFunc = []()
		{
			if (genTile)
			{
				auto& prefabComponent = ECS_WORLD.AddComponent<Backend::ECS::Component::PrefabComponent>(currEntity);
				prefabComponent.m_Key = prefabs[index];
				prefabComponent.m_Height = static_cast<float>(layer) * 30.0f;
				prefabComponent.m_Pos = mousePos;
				ECS_WORLD.RegisterEntityIntoSystems(currEntity);
				recentlyAddedEntites.push(currEntity);
				while (!recentlyRemovedEntites.empty())
					recentlyRemovedEntites.pop();
				std::cout << AEFrameRateControllerGetFrameCount() << ": Entity Created: " << currEntity << " at position (" << mousePos.x << ", " << mousePos.y << ")" << std::endl;
				currEntity = Backend::ECS::ECS_MaxEntities + 1;
				genTile = false;
			}
		};

		static LevelEditorSystem levelEditorSystem{ updateFunc, nullptr, preUpdateFunc, postUpdateFunc };
		return levelEditorSystem;
	}

	/**
	 * \brief			Generates the player system.
	 * \tparam Scene	The scene.
	 * \return			The player system.
	 */
	template<int Scene>
	auto GenerateLevelEditorPlayerSystem() -> PlayerSystem&
	{
		static auto updateFunc =
			[](const std::tuple<Backend::ECS::Component::PlayerComponent*, Backend::ECS::Component::IsometricTransformComponent*>& _components)
		{
			auto& iso = std::get<Backend::ECS::Component::IsometricTransformComponent*>(_components);
			AEVec2 move{};
			if (AEInputCheckCurr(AEVK_W))
				move.y = 1;
			else if (AEInputCheckCurr(AEVK_S))
				move.y = -1;
			if (AEInputCheckCurr(AEVK_A))
				move.x = -1;
			else if (AEInputCheckCurr(AEVK_D))
				move.x = 1;
			if (move.x != 0.f && move.y != 0.f)
				NormalizeVec2(move);
			move *= Backend::ECS::Component::IsometricTransformComponent::s_IsoInverse;
			iso->m_Position += move * 5;
			AEGfxSetCamPosition(iso->m_IsometricPosition.x, iso->m_IsometricPosition.y);
		};
		static PlayerSystem levelEditorPlayerSystem{ updateFunc };
		return levelEditorPlayerSystem;
	}
	
	/**
	 * \brief Generates the Scene Saving System.
	 * \tparam Scene The scene.
	 * \return The Scene Saving System.
	 */
	template<int Scene>
	auto GeneratePrefabSystem() -> PrefabSystem&
	{
		static bool save = false;
		static auto preUpdateFunc = []()
		{
			if (AEInputCheckTriggered(AEVK_F))
			{
				save = true;
				Backend::JSONSerializer::GenerateScene();
			}
		};
		static auto updateFunc =
			[](const std::tuple<Backend::ECS::Component::PrefabComponent*>& _components)
		{
			if (!save)
				return;

			auto& prefab = std::get<Backend::ECS::Component::PrefabComponent*>(_components);
			Backend::JSONSerializer::SaveSceneEntity(*prefab);

		};
		static auto postUpdateFunc = []()
		{
			if (save)
			{
				save = false;
				Backend::JSONSerializer::GenerateSceneEnd();
			}
		};
		static PrefabSystem prefabSystem{ updateFunc, nullptr, preUpdateFunc, postUpdateFunc };
		return prefabSystem;
	}
}

#endif