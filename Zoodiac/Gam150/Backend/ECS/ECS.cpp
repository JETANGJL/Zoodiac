// ======================================================================
// File Name:		ECS.cpp
// Project Name:	Zoodiac
// Author(s):		Low Zhi Sheng Kitson(80%), Lim Geng Yang(5%),
//					Lye Pin Liang Xavier(5%), Muhammad Dzulhafiz(5%),
//					Ang Jie Le Jet(5%)
// Brief:			Contains ECS functionality
// 
// All content � 2023 DigiPen Institute of Technology Singapore. All 
// rights reserved.
// ======================================================================

#define REQUIRE_ECS_SYSTEMS_
#include <ranges>
#include <Backend/Engine_Common.h>
#include <Backend/JSONSerializer.h>
#include <Backend/ECS/ECS.h>
#include <Backend/ECS/ECS_Common.h>
#include <Backend/ECS/ECS_Factory.h>
#include <Backend/SceneManagement/SceneCommon.h>
#include <Backend/SceneManagement/SceneGenerator.h>

namespace Backend::ECS
{
	ECS_World::~ECS_World()
	{
		for (const auto& val : m_ComponentPools | std::views::values)
			delete val;
		m_ComponentPools.clear();
		m_Systems.clear();
	}

	auto ECS_World::InitializeECS() -> void
	{
		m_EntityCount = 0;
		s_ComponentCount = 0;

		RegisterComponent<ECS_ALL_COMPONENTS>();
		JSONSerializer::ParsePrefabs();
		
		for (uint32_t i = SCENE_MAINMENU; i < MaxScene; ++i)
		{
			m_Systems.emplace(i, std::unordered_map<std::uint16_t, ECS_AbstractSystem&>{});
			m_Entities.emplace(i, std::unordered_map<ECS_Entity, ECS_Signature>{});
			m_DeletionEntities.emplace(i, std::vector<ECS_Entity>{});
		}

		GenerateSceneSystem<SCENE_LEVELEDITOR>();
	
		for (uint32_t i = SCENE_MAINMENU; i < MaxScene; ++i)
		{
			for (auto& val : m_Systems.at(i) | std::views::values)
				val.InitializeSystem();
		}

		LoadScene(SCENE_SPLASHSCREEN);
	}

	auto ECS_World::UpdateECS() -> void
	{
		for (auto& val : m_Systems.at(m_SceneStack.top()) | std::views::values)
			val.UpdateSystem();
		if (!m_DeletionEntities.at(m_SceneStack.top()).empty())
		{
			for (const auto& i : m_DeletionEntities.at(m_SceneStack.top()))
				RemoveEntity(i);
			m_DeletionEntities.at(m_SceneStack.top()).clear();
		}
		if(m_UnloadScene)
		{
			m_UnloadScene = false;
			UnloadScene();
		}
		else if(m_UnloadSceneUntil)
		{
			m_UnloadSceneUntil = false;
			while (m_SceneStack.top() != m_SceneToUnloadUntil)
				UnloadScene();
		}
	}

	auto ECS_World::GenerateEntity() -> ECS_Entity
	{
		const ECS_Entity entity = m_EntityStack.GetEntity();
		if(AddEntity(entity))
			++m_EntityCount;
		return entity;
	}

	auto ECS_World::AddEntity(ECS_Entity _entity) -> bool
	{
		if(m_Entities.at(m_SceneStack.top()).contains(_entity))
			return false;
		m_Entities.at(m_SceneStack.top()).emplace(_entity, ECS_Signature{});
		return true;
	}
	
	auto ECS_World::RemoveEntity(const ECS_Entity _entity) -> bool
	{
		if (!m_Entities.at(m_SceneStack.top()).contains(_entity))
			return false;
		RemoveEntityFromSystems(_entity);
		ECS_Signature signature = m_Entities.at(m_SceneStack.top()).at(_entity);
		for (unsigned i = 0; i < ECS_MaxComponents; ++i)
		{
			if (signature[i])
				m_ComponentPools.at(i + 1)->FreeComponent(_entity);
		}
		m_Entities.at(m_SceneStack.top()).erase(_entity);
		m_EntityStack.FreeEntity(_entity);
		--m_EntityCount;
		return true;
	}

	auto ECS_World::RegisterEntityIntoSystems(const ECS_Entity _entity) -> void
	{
		if (!m_Entities.at(m_SceneStack.top()).contains(_entity))
			return;

		const ECS_Signature signature = m_Entities.at(m_SceneStack.top()).at(_entity);
		for (auto& val : m_Systems.at(m_SceneStack.top()) | std::views::values)
		{
			const ECS_Signature& systemSignature = val.Signature();
			if ((signature & systemSignature) == systemSignature)
				val.AddToSystem(_entity);
		}
	}

	auto ECS_World::RemoveEntityFromSystems(const ECS_Entity _entity) -> void
	{
		if (!m_Entities.at(m_SceneStack.top()).contains(_entity))
			return;

		const ECS_Signature signature = m_Entities.at(m_SceneStack.top()).at(_entity);
		for (auto& val : m_Systems.at(m_SceneStack.top()) | std::views::values)
		{
			if (const ECS_Signature& systemSignature = val.Signature(); (signature & systemSignature) == systemSignature)
				val.RemoveFromSystem(_entity);
		}
	}

	auto ECS_World::EntitySignatureChanged(const ECS_Entity _entity) -> void
	{
		RemoveEntityFromSystems(_entity);
		RegisterEntityIntoSystems(_entity);
	}

	auto ECS_World::LoadScene(const Scene _scene) -> void
	{
		if (_scene >= NUM_SCENE)
			return;
		if (!m_SceneStack.empty())
			AUDIO_MANAGER.PauseAudio();
		m_SceneStack.push(_scene);
		AEGfxSetCamPosition(0, 0);
		if (_scene != SCENE_MAINMENU && _scene < SCENE_CREDIT1)
			ECS::Factory::ECS_Factory::CreatePauseMenu();
		switch (_scene)
		{
		case Backend::SCENE_SPLASHSCREEN:
			SceneGenerator::GenerateSplashscreen();
			break;
		case Backend::SCENE_MAINMENU:
			SceneGenerator::GenerateMainMenu();
			break;
		case Backend::SCENE_CREDIT1:
			SceneGenerator::GenerateCreditPage(1);
			break;
		case Backend::SCENE_CREDIT2:
			SceneGenerator::GenerateCreditPage(2);
			break;
		case Backend::SCENE_CREDIT3:
			SceneGenerator::GenerateCreditPage(3);
			break;
		case Backend::SCENE_CREDIT4:
			SceneGenerator::GenerateCreditPage(4);
			break;
		case Backend::SCENE_OPTIONS:
			ECS::Factory::ECS_Factory::CreateBackgroundScreen("skybackground");
			ECS::Factory::ECS_Factory::CreateButtonEntity({ 0, 150 }, "Fullscreen", Component::UILogicComponent::TAG_OPTIONS_FULLSCREEN);
			ECS::Factory::ECS_Factory::CreateButtonEntity({ 0, 0 }, "Minimized", Component::UILogicComponent::TAG_OPTIONS_MINIMIZED);
			ECS::Factory::ECS_Factory::CreateButtonEntity({ 0, -150 }, "Clear Data", Component::UILogicComponent::TAG_OPTIONS_CLEARDATA);
			ECS::Factory::ECS_Factory::CreateButtonEntity({ -500, 400 }, "Main Menu", Component::UILogicComponent::TAG_OPTIONS_RETURNTOMENU);
			break;
		case Backend::SCENE_OVERWORLD:
			*DI_GET_PAUSE() = false;
			AUDIO_MANAGER.PlayAudio(Audio::AudioManager::Sound::OVERWORLD_MUSIC, -1);
			JSONSerializer::ParseScene("Assets/json/overworld2.json");
			JSONSerializer::LoadSaveData();
			ECS::Factory::ECS_Factory::CreatePlayerEntity();
			ECS::Factory::ECS_Factory::CreateCloudEmitterEntity();
			break;
		case Backend::SCENE_TUTORIAL:
			*DI_GET_PAUSE() = false;
			JSONSerializer::ParseScene("Assets/json/scene_tutorial.json");
			ECS::Factory::ECS_Factory::CreateTutorialEntity({350.0f, 200.0f});
			ECS::Factory::ECS_Factory::CreatePlayerEntity();
			ECS::Factory::ECS_Factory::CreateCloudEmitterEntity();
			break;
		case Backend::SCENE_WILDLANDS:
			*DI_GET_PAUSE() = false;
			AUDIO_MANAGER.PlayAudio(Audio::AudioManager::Sound::WILDWORLD_MUSIC, -1);
			JSONSerializer::ParseScene("Assets/json/wildlife1.json");
			ECS::Factory::ECS_Factory::CreatePlayerEntity();
			ECS::Factory::ECS_Factory::CreateCloudEmitterEntity();
			ECS::Factory::ECS_Factory::CreatePetEntity(*DI_GET_PET());
			break;
		case Backend::SCENE_MINIGAME_COLLECTION:
			*DI_GET_PAUSE() = true;
			ECS::Factory::ECS_Factory::CreateInstructionEntity(_scene);
			AUDIO_MANAGER.PlayAudio(Audio::AudioManager::Sound::BALLCOLLECT_SOUND, -1);
			SceneGenerator::GenerateCollectionMinigame(8, 8);
			ECS::Factory::ECS_Factory::CreateCloudEmitterEntity();
			break;
		case Backend::SCENE_MINIGAME_CLIMBING:
			*DI_GET_PAUSE() = true;
			ECS::Factory::ECS_Factory::CreateInstructionEntity(_scene);
			AUDIO_MANAGER.PlayAudio(Audio::AudioManager::Sound::BALLCOLLECT_SOUND, -1);
			SceneGenerator::GenerateClimbWorld(4, 30, 30 );
			ECS::Factory::ECS_Factory::CreateCloudEmitterEntity();
			break;
		case Backend::SCENE_MINIGAME_DODGING:
			*DI_GET_PAUSE() = true;
			AUDIO_MANAGER.PlayAudio(Audio::AudioManager::Sound::BALLCOLLECT_SOUND, -1);
			ECS::Factory::ECS_Factory::CreateInstructionEntity(_scene);
			SceneGenerator::GenerateDodgingMinigame(10, 10);
			ECS::Factory::ECS_Factory::CreateCloudEmitterEntity();
			break;
		case Backend::SCENE_MINIGAME_RUNNER:
			*DI_GET_PAUSE() = true;
			ECS::Factory::ECS_Factory::CreateInstructionEntity(_scene);
			AUDIO_MANAGER.PlayAudio(Audio::AudioManager::Sound::BALLCOLLECT_SOUND, -1);
			SceneGenerator::GenerateEndlessRunnerWorld(4, 100, 3);
			ECS::Factory::ECS_Factory::CreateCloudEmitterEntity();
			break;
		case Backend::SCENE_MINIGAME_DODGECOLLECTION:
			*DI_GET_PAUSE() = true;
			AUDIO_MANAGER.PlayAudio(Audio::AudioManager::Sound::BALLCOLLECT_SOUND, -1);
			ECS::Factory::ECS_Factory::CreateInstructionEntity(_scene);
			SceneGenerator::GenerateDodgingAndCollectMinigame(10, 10);
			ECS::Factory::ECS_Factory::CreateCloudEmitterEntity();
			break;
		case Backend::SCENE_MINIGAME_STAGGEREDODGING:
			*DI_GET_PAUSE() = true;
			AUDIO_MANAGER.PlayAudio(Audio::AudioManager::Sound::BALLCOLLECT_SOUND, -1);
			ECS::Factory::ECS_Factory::CreateInstructionEntity(_scene);
			SceneGenerator::GenerateStaggeredDodgingMinigame(10, 10);
			ECS::Factory::ECS_Factory::CreateCloudEmitterEntity();
			break;
		case Backend::SCENE_LEVELEDITOR:
			SceneGenerator::GenerateLevelEditor();
			JSONSerializer::ParseScene("Assets/json/scene.json", true);
			break;
		case NUM_SCENE:
			break;
		}
	}

	auto ECS_World::UnloadScene() -> void
	{
		if (m_SceneStack.size() <= 1)
			return;

		std::vector<ECS_Entity> temp;
		for (const auto& key : m_Entities.at(m_SceneStack.top()) | std::views::keys)
			temp.push_back(key);
		for (const auto& i : temp)
			RemoveEntity(i);
		AUDIO_MANAGER.StopAudio();
		m_Entities.at(m_SceneStack.top()).clear();
		m_SceneStack.pop();
		AUDIO_MANAGER.ResumeAudio();
	}

	auto ECS_World::FlagUnloadScene() -> void
	{
		m_UnloadScene = true;
	}

	auto ECS_World::FlagUnloadSceneUntil(Scene _scene) -> void
	{
		auto sceneStack = m_SceneStack;
		m_SceneToUnloadUntil = _scene;
		while (!sceneStack.empty() && sceneStack.top() != _scene)
			sceneStack.pop();
		if (!sceneStack.empty())
			m_UnloadSceneUntil = true;
	}

	auto ECS_World::GetCurrentScene() -> Scene
	{
		return m_SceneStack.top();
	}

	auto ECS_World::FlagEntityForDeletion(const ECS_Entity _entity) -> void
	{
		if (!m_Entities.at(m_SceneStack.top()).contains(_entity))
			return;
		m_DeletionEntities.at(m_SceneStack.top()).push_back(_entity);
	}
}
