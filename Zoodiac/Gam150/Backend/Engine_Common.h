// ======================================================================
// File Name:		Engine_Common.h
// Project Name:	Zoodiac
// Author(s):		Low Zhi Sheng Kitson(94%), Lim Geng Yang(1%),
//					Lye Pin Liang Xavier(4%), Muhammad Dzulhafiz(1%)
// Brief:			Contains Audio Manager functionality
// 
// All content © 2023 DigiPen Institute of Technology Singapore. All 
// rights reserved.
// ======================================================================

#ifndef ENGINE_COMMON_H_
#define ENGINE_COMMON_H_

#include <AEEngine.h>
#include <string>
#include <array>

#include <Backend/ResourceLibrary.h>
#include <Backend/VecOperators.h>
#include <Backend/DependencyInjection/DependencyInjector.h>
#include <Backend/ECS/ECS_Prefab.h>
#include <Backend/Sound/AudioManager.h>
#include <Backend/Utils.h>

#define ANIMATION_LIBRARY	Backend::Resource::ResourceLibrary<std::string, std::vector<std::string>, std::nullptr_t>::GetInstance()
#define FONT_LIBRARY		Backend::Resource::ResourceLibrary<std::string, s8, decltype(&Backend::DestroyFont)>::GetInstance()
#define TEXTURE_LIBRARY		Backend::Resource::ResourceLibrary<std::string, AEGfxTexture, decltype(&AEGfxTextureUnload)>::GetInstance()
#define PREFAB_LIBRARY		Backend::Resource::ResourceLibrary<std::string, Backend::ECS::ECS_Prefab, std::nullptr_t>::GetInstance()
#define STATE_LIBRARY		Backend::Resource::ResourceLibrary<StateType, State, std::nullptr_t>::GetInstance()

#define DEPENDENCY_INJECTOR Backend::DependencyInjector::GetInstance()
#define AUDIO_MANAGER		Backend::Audio::AudioManager::GetInstance()

#define DI_GET_PLAYER_POS()			DEPENDENCY_INJECTOR.GetDependency<AEVec2>(ECS_WORLD.GetCurrentScene(), Backend::DependencyInjector::Dependency::PLAYER_POS)
#define DI_GET_SCORE() 				DEPENDENCY_INJECTOR.GetDependency<unsigned int>(ECS_WORLD.GetCurrentScene(), Backend::DependencyInjector::Dependency::SCORE)
#define DI_GET_HIT() 				DEPENDENCY_INJECTOR.GetDependency<unsigned int>(ECS_WORLD.GetCurrentScene(), Backend::DependencyInjector::Dependency::HIT)
#define DI_GET_PET() 				DEPENDENCY_INJECTOR.GetDependency<Backend::ECS::Component::PlayerComponent::Pet_Type>(ECS_WORLD.GetCurrentScene(), Backend::DependencyInjector::Dependency::PET)
#define DI_GET_PAUSE()				DEPENDENCY_INJECTOR.GetDependency<bool>(Backend::Scene::NUM_SCENE, Backend::DependencyInjector::Dependency::PAUSE)
#define DI_GET_CURRENT_PET()		DEPENDENCY_INJECTOR.GetDependency<Backend::ECS::Component::PlayerComponent::Pet_Type>(ECS_WORLD.GetCurrentScene(), Backend::DependencyInjector::Dependency::CURRENT_PET)
#define DI_GET_EXIT() 				DEPENDENCY_INJECTOR.GetDependency<int>(Backend::Scene::NUM_SCENE, Backend::DependencyInjector::Dependency::EXIT)
#define DI_GET_SCENE_PET()			DEPENDENCY_INJECTOR.GetDependency<Backend::ECS::Component::PlayerComponent::Pet_Type>(ECS_WORLD.GetCurrentScene(), Backend::DependencyInjector::Dependency::SCENE_PET)

namespace Backend
{
	constexpr signed int	WindowWidth = 1600;
	constexpr signed int	WindowHeight = 900;
	constexpr unsigned int	MaxFrameRate = 60;
	constexpr char			GameName[] = "Zoodiac";

	/**
	 * \brief	Gets delta time.
	 * \return	Delta time if the game is not paused, 0 otherwise.
	 */
	inline auto GetDeltaTime() -> float
	{
		return !*DI_GET_PAUSE() ? static_cast<float>(AEFrameRateControllerGetFrameTime()) : 0.f;
	}
}

#endif