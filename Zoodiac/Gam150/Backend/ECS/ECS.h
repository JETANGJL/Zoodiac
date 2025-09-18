// ======================================================================
// File Name:		ECS.h
// Project Name:	Zoodiac
// Author(s):		Low Zhi Sheng Kitson(80%), Lim Geng Yang(5%),
//					Lye Pin Liang Xavier(5%), Muhammad Dzulhafiz(5%),
//					Ang Jie Le Jet(5%)
// Brief:			Contains ECS Systems and functionality
// 
// All content © 2023 DigiPen Institute of Technology Singapore. All 
// rights reserved.
// ======================================================================

#ifndef ECS_H_
#define ECS_H_

#include <algorithm>
#include <assert.h>
#include <functional>
#include <stack>
#include <tuple>
#include <unordered_map>

#include <Backend/Singleton.h>
#include <Backend/ECS/ECS_Common.h>
#include <Backend/ECS/ECS_ComponentPool.h>
#include <Backend/ECS/ECS_EntityStack.h>
#include <Backend/SceneManagement/SceneCommon.h>

namespace Backend::ECS
{
	/**
	 * \brief	Abstract System Class
	 */
	class ECS_AbstractSystem
	{
	protected:
		ECS_Signature m_Signature;	//	Signature of the system
		ECS_Entity m_EntityCount;	//	Number of entities in the system

		/**
		 * \brief			Private copy constructor.
		 * \param _other	Object to copy.
		 */
		ECS_AbstractSystem(const ECS_AbstractSystem& _other) = default;

		/**
		 * \brief			Private copy assignment operator.
		 * \param _other	Object to copy.
		 */
		auto operator=(const ECS_AbstractSystem& _other) -> ECS_AbstractSystem& = default;

		/**
		 * \brief			Private move constructor.
		 * \param _other	Object to move.
		 */
		ECS_AbstractSystem(ECS_AbstractSystem&& _other) = default;

		/**
		 * \brief			Private move assignment operator.
		 * \param _other	Object to move.
		 */
		auto operator=(ECS_AbstractSystem&& _other) -> ECS_AbstractSystem& = default;


	public:
		/**
		 * \brief	Default constructor.
		 */
		ECS_AbstractSystem()
			: m_EntityCount{ 0 }
		{
		}

		/**
		 * \brief	Destructor.
		 */
		virtual ~ECS_AbstractSystem() = default;

		/**
		 * \brief	Initializes the system.
		 */
		virtual auto InitializeSystem() -> void = 0;

		/**
		 * \brief			Adds to system.
		 * \param _entity	Entity ID.
		 */
		virtual auto AddToSystem(ECS_Entity _entity) -> void = 0;

		/**
		 * \brief	Updates the system.
		 */
		virtual auto UpdateSystem() -> void = 0;

		/**
		 * \brief			Removes from system.
		 * \param _entity	Entity ID.
		 */
		virtual auto RemoveFromSystem(ECS_Entity _entity) -> void = 0;

		/**
		 * \brief	Signature of the system.
		 * \return	Signature.
		 */
		[[nodiscard]] auto Signature() const -> const ECS_Signature& { return m_Signature; }
	};

	/**
	 * \brief				Component System Class
	 * \tparam Components	Component types
	 */
	template <class...Components>
	class ECS_System final : public ECS_AbstractSystem
	{
	private:
		using UpdateFunction = std::function<void(const std::tuple<Components*...>&)>;
		using CompareFunction = std::function<bool(ECS_Entity const&, ECS_Entity const&)>;

		/**
		 * \brief					Initializes the signature.
		 * \tparam Component1		The current component.
		 * \tparam Component2		The next component.
		 * \tparam OtherComponents	The other components.
		 */
		template<class Component1, class Component2, class ...OtherComponents>
		auto InitializeSignature() -> void;

		/**
		 * \brief					Initializes the signature.
		 * \tparam Component		The current component.
		 */
		template<class Component>
		auto InitializeSignature() -> void;

		/**
		 * \brief					Adds a entity to the system.
		 * \tparam Component1		The current component.
		 * \tparam Component2		The next component.
		 * \tparam OtherComponents	The other components.
		 * \param _entity			Entity ID.
		 */
		template<class Component1, class Component2, class ...OtherComponents>
		auto AddToSystem(ECS_Entity _entity) -> void;

		/**
		 * \brief					Adds a entity to the system.
		 * \tparam Component		The current component.
		 * \param _entity			Entity ID.
		 */
		template<class Component>
		auto AddToSystem(ECS_Entity _entity) -> void;

		/**
		 * \brief			Private copy constructor.
		 * \param _other	Object to copy.
		 */
		ECS_System(const ECS_System& _other) = default;

		/**
		 * \brief			Private copy assignment operator.
		 * \param _other	Object to copy.
		 */
		auto operator=(const ECS_System& _other) -> ECS_System& = default;

		/**
		 * \brief			Private move constructor.
		 * \param _other	Object to move.
		 */
		ECS_System(ECS_System&& _other) = default;

		/**
		 * \brief			Private move assignment operator.
		 * \param _other	Object to move.
		 */
		auto operator=(ECS_System&& _other) -> ECS_System& = default;

	public:
		std::unordered_map<ECS_Entity, std::tuple<Components*...>> m_Components;
		std::vector<ECS_Entity> m_Entities;

		UpdateFunction m_UpdateFunction;
		CompareFunction m_CompareFunction;
		std::function<void()> m_PreUpdateFunction;
		std::function<void()> m_PostUpdateFunction;
		bool m_UseSortedOrder = false;

		/**
		 * \brief						Constructor.
		 * \param _update				The update function.
		 * \param _compare				The compare function.
		 * \param _preUpdateFunction	The pre update function.
		 * \param _postUpdateFunction	The post update function.
		 */
		explicit ECS_System(UpdateFunction _update,
		           CompareFunction _compare = nullptr,
		           std::function<void()> _preUpdateFunction = nullptr,
		           std::function<void()> _postUpdateFunction = nullptr)
			: ECS_AbstractSystem(),
			m_UpdateFunction{ std::move(_update) },
			m_CompareFunction{ std::move(_compare) },
			m_PreUpdateFunction{ std::move(_preUpdateFunction) },
			m_PostUpdateFunction{ std::move(_postUpdateFunction) }
		{
		}

		/**
		 * \brief	Destructor.
		 */
		~ECS_System() override = default;

		/**
		 * \brief	Initializes the system.
		 */
		auto InitializeSystem() -> void override;

		/**
		 * \brief			Adds to system.
		 * \param _entity	Entity ID.
		 */
		auto AddToSystem(ECS_Entity _entity) -> void override;

		/**
		 * \brief	Updates the system.
		 */
		auto UpdateSystem() -> void override;

		/**
		 * \brief			Removes from system.
		 * \param _entity	Entity ID.
		 */
		auto RemoveFromSystem(ECS_Entity _entity) -> void override;

	};

	/**
	 * \brief	ECS Controller
	 */
	class ECS_World final : public Singleton<ECS_World>
	{
	private:
		inline static ECS_ComponentType s_ComponentCount = 0;

		using EntityMap = std::unordered_map<uint32_t, std::unordered_map<ECS_Entity, ECS_Signature>>;
		using DeletionQueue = std::unordered_map<uint32_t, std::vector<ECS_Entity>>;
		using ComponentPools = std::unordered_map<ECS_ComponentType, AbstractComponentPool*>;
		using SystemMap = std::unordered_map<uint32_t, std::unordered_map<std::uint16_t, ECS_AbstractSystem&>>;
		using SceneStack = std::stack<Scene>;

		ECS_Entity m_EntityCount;
		ECS_EntityStack m_EntityStack;
		EntityMap m_Entities;
		DeletionQueue m_DeletionEntities;
		ComponentPools m_ComponentPools;
		SystemMap m_Systems;
		SceneStack m_SceneStack;

		bool m_UnloadScene = false;
		bool m_UnloadSceneUntil = false;
		Scene m_SceneToUnloadUntil = SCENE_MAINMENU;

		/**
		 * \brief			Adds an entity to the world.
		 * \param _entity	Entity ID.
		 * \return			True if successful.
		 */
		auto AddEntity(ECS_Entity _entity) -> bool;

		/**
		 * \brief			Removes an entity from the world.
		 * \param _entity	Entity ID.
		 * \return			True if successful.
		 */
		auto RemoveEntity(ECS_Entity _entity) -> bool;

		/**
		 * \brief				Registers a component.
		 * \tparam Component	The component type.
		 * \return				True if successful.
		 */
		template <typename Component>
		auto RegisterComponent() -> bool;

		/**
		 * \brief				Registers a component.
		 * \tparam Component	The component type.
		 * \return				True if successful.
		 */
		template <typename Component, typename Component1, typename ...Components>
		auto RegisterComponent() -> bool;

		/**
		 * \brief			Private copy constructor.
		 * \param _other	Object to copy.
		 */
		ECS_World(const ECS_World& _other) = default;

		/**
		 * \brief			Private copy assignment operator.
		 * \param _other	Object to copy.
		 */
		auto operator=(const ECS_World& _other) -> ECS_World& = default;

		/**
		 * \brief			Private move constructor.
		 * \param _other	Object to move.
		 */
		ECS_World(ECS_World&& _other) = default;

		/**
		 * \brief			Private move assignment operator.
		 * \param _other	Object to move.
		 */
		auto operator=(ECS_World&& _other) -> ECS_World& = default;

	public:
		/**
		 * \brief	Constructor.
		 */
		ECS_World() : m_EntityCount{ 0 }, m_SceneToUnloadUntil{SCENE_MAINMENU} {}

		/**
		 * \brief	Destructor.
		 */
		~ECS_World() override;

		/**
		 * \brief				Gets a free component.
		 * \tparam Component	The component type.
		 * \return				The component.
		 */
		template <typename Component>
		auto GetComponent() -> Component&;

		/**
		 * \brief				Searches for the component of an entity.
		 * \tparam Component	The component type.
		 * \param _entity		The entity.
		 * \return				The component.
		 */
		template <typename Component>
		auto GetComponent(ECS_Entity _entity) -> Component&;

		/**
		 * \brief				Frees a component belonging to an entity.
		 * \tparam Component	The component type.
		 * \param _entity		The entity.
		 */
		template <typename Component>
		auto FreeComponent(ECS_Entity _entity) -> void;

		/**
		 * \brief	Initializes ECS.
		 */
		auto InitializeECS() -> void;

		/**
		 * \brief	Updates ECS.
		 */
		auto UpdateECS() -> void;

		/**
		 * \brief	Generates an entity.
		 * \return	The entity.
		 */
		auto GenerateEntity() -> ECS_Entity;

		/**
		 * \brief				Adds a component to an entity.
		 * \tparam Component	The component type.
		 * \param _entity		The entity.
		 * \return				The added component.
		 */
		template<typename Component>
		auto AddComponent(ECS_Entity _entity) -> Component&;

		/**
		 * \brief				Removes a component from an entity.
		 * \tparam Component	The component type.
		 * \param _entity		The entity.
		 * \return				True if successful.
		 */
		template<typename Component>
		auto RemoveComponent(ECS_Entity _entity) -> bool;

		/**
		 * \brief			Registers an entity in to all available systems.
		 * \param _entity	The entity.
		 */
		auto RegisterEntityIntoSystems(ECS_Entity _entity) -> void;

		/**
		 * \brief			Removes an entity from all available systems.
		 * \param _entity	The entity.
		 */
		auto RemoveEntityFromSystems(ECS_Entity _entity) -> void;

		/**
		 * \brief			Re register the entity into all available systems. 
		 * \param _entity	The entity.
		 */
		auto EntitySignatureChanged(ECS_Entity _entity) -> void;

		/**
		 * \brief			Generates all systems for a scene.
		 * \tparam Scene	The scene.
		 */
		template<int Scene>
		auto GenerateSceneSystem() -> void;

		/**
		 * \brief			Load a scene.
		 * \param _scene	The scene.
		 */
		auto LoadScene(Scene _scene) -> void;

		/**
		 * \brief			Unload the current scene.
		 */
		auto UnloadScene() -> void;

		/**
		 * \brief			Unload the current scene at the end of the frame.
		 */
		auto FlagUnloadScene() -> void;

		/**
		 * \brief			Unload the scene until specified scene at the end of the frame.
		 */
		auto FlagUnloadSceneUntil(Scene _scene) -> void;

		/**
		 * \brief			Gets the current scene.
		 * \return			The current scene.
		 */
		auto GetCurrentScene() -> Scene;

		/**
		 * \brief			Flags an entity for deletion at the end of this frame.
		 * \param _entity	The entity.
		 */
		auto FlagEntityForDeletion(ECS_Entity _entity) -> void;
	};

	template<class ...Components>
	auto ECS_System<Components...>::InitializeSystem() -> void
	{
		InitializeSignature<Components...>();
	}

	template<class ...Components>
	auto ECS_System<Components...>::AddToSystem(ECS_Entity _entity) -> void
	{
		if (m_Components.count(_entity))
			return;
		m_Components.emplace(_entity, std::tuple<Components*...>{});
		AddToSystem<Components...>(_entity);
		m_Entities.push_back(_entity);
		++m_EntityCount;
	}

	template<class ...Components>
	auto ECS_System<Components...>::UpdateSystem() -> void
	{
		if (m_PreUpdateFunction)
			m_PreUpdateFunction();
		if (m_UseSortedOrder && m_CompareFunction)
		{
			std::ranges::sort(m_Entities, m_CompareFunction);
			for(auto i : m_Entities)
				m_UpdateFunction(m_Components.at(i));
		}
		else if(m_UpdateFunction)
		{
			auto end = m_Components.end();
			for (auto i = m_Components.begin(); i != end; ++i)
				m_UpdateFunction(i->second);
		}
		if (m_PostUpdateFunction)
			m_PostUpdateFunction();
	}

	template<class ...Components>
	auto ECS_System<Components...>::RemoveFromSystem(ECS_Entity _entity) -> void
	{
		if (!m_Components.count(_entity))
			return;
		m_Components.erase(_entity);
		std::erase(m_Entities, _entity);
		--m_EntityCount;
	}

	template<class ...Components>
	template<class Component1, class Component2, class ...OtherComponents>
	auto ECS_System<Components...>::InitializeSignature() -> void
	{
		InitializeSignature<Component1>();
		InitializeSignature<Component2, OtherComponents...>();
	}

	template<class ...Components>
	template<class Component>
	auto ECS_System<Components...>::InitializeSignature() -> void
	{
		m_Signature.set(ECS_Component::GetBitsetIndex<Component>(), true);
	}

	template<class ...Components>
	template<class Component1, class Component2, class ...OtherComponents>
	auto ECS_System<Components...>::AddToSystem(const ECS_Entity _entity) -> void
	{
		AddToSystem<Component1>(_entity);
		AddToSystem<Component2, OtherComponents...>(_entity);
	}

	template<class ...Components>
	template<class Component>
	auto ECS_System<Components...>::AddToSystem(const ECS_Entity _entity) -> void
	{
		auto& component = std::get<Component*>(m_Components.at(_entity));
		component = &ECS_World::GetInstance().GetComponent<Component>(_entity);
	}

	template<typename Component>
	auto ECS_World::RegisterComponent() -> bool
	{
		static_assert(std::is_base_of_v<ECS_Component, Component>, "RegisterComponent must contain template argument of type Component");

		if (ECS_Component::GetComponentID<Component>())
			return false;
		ECS_Component::GetComponentID<Component>(++s_ComponentCount);
		AbstractComponentPool* componentPool = new ComponentPool<Component>();
		m_ComponentPools.emplace(ECS_Component::GetComponentID<Component>(), componentPool);
		return true;
	}

	template <typename Component, typename Component1, typename ... Components>
	auto ECS_World::RegisterComponent() -> bool
	{
		RegisterComponent<Component>();
		return RegisterComponent<Component1, Components...>();
	}

	template<typename Component>
	auto ECS_World::GetComponent() -> Component&
	{
		static_assert(std::is_base_of_v<ECS_Component, Component>, "RegisterComponent must contain template argument of type Component");
		assert(ECS_Component::GetComponentID<Component>());
		return static_cast<ComponentPool<Component>*>(m_ComponentPools.at(ECS_Component::GetComponentID<Component>()))->GetComponent();
	}

	template<typename Component>
	auto ECS_World::GetComponent(ECS_Entity _entity) -> Component&
	{
		static_assert(std::is_base_of_v<ECS_Component, Component>, "RegisterComponent must contain template argument of type Component");
		assert(ECS_Component::GetComponentID<Component>());
		return static_cast<ComponentPool<Component>*>(m_ComponentPools.at(ECS_Component::GetComponentID<Component>()))->GetComponent(_entity);
	}

	template<typename Component>
	auto ECS_World::FreeComponent(ECS_Entity _entity) -> void
	{
		static_assert(std::is_base_of_v<ECS_Component, Component>, "RegisterComponent must contain template argument of type Component");
		assert(!ECS_Component::GetComponentID<Component>());
		static_cast<ComponentPool<Component>*>(m_ComponentPools.at(ECS_Component::GetComponentID<Component>()))->FreeComponent(_entity);
	}

	template<typename Component>
	auto ECS_World::AddComponent(ECS_Entity _entity) -> Component&
	{
		static_assert(std::is_base_of_v<ECS_Component, Component>, "RegisterComponent must contain template argument of type Component");
		if (!m_Entities.at(m_SceneStack.top()).contains(_entity) || m_Entities.at(m_SceneStack.top()).at(_entity).test(ECS_Component::GetBitsetIndex<Component>()))
			return GetComponent<Component>(_entity);
		auto& component = GetComponent<Component>();
		component.Entity(_entity);
		m_Entities.at(m_SceneStack.top()).at(_entity).set(ECS_Component::GetBitsetIndex<Component>(), true);
		return component;
	}

	template<typename Component>
	auto ECS_World::RemoveComponent(const ECS_Entity _entity) -> bool
	{
		static_assert(std::is_base_of_v<ECS_Component, Component>, "RegisterComponent must contain template argument of type Component");
		if (!m_Entities.contains(_entity) || !m_Entities.at(m_SceneStack.top()).at(_entity).test(ECS_Component::GetBitsetIndex<Component>()))
			return false;
		m_Entities.at(m_SceneStack.top()).at(_entity).set(ECS_Component::GetBitsetIndex<Component>(), false);
		FreeComponent<Component>(_entity);
		return true;
	}

}

#ifdef REQUIRE_ECS_SYSTEMS_

#include <Backend/ECS/ECS_System.h>
#include <LevelEditor/LevelEditor.h>

namespace Backend::ECS
{
	template<int Scene>
	auto ECS_World::GenerateSceneSystem() -> void
	{
		auto& system = m_Systems.at(Scene);
		uint32_t counter = 0;
		system.emplace(counter++, System::GeneratePlayerSystem<Scene>());
		system.emplace(counter++, System::GenerateAnimationLogicSystem<Scene>());
		system.emplace(counter++, System::GenerateAnimationSystem<Scene>());
		system.emplace(counter++, System::GenerateMovementBasedEmitterSystem<Scene>());
		system.emplace(counter++, System::GenerateEmitterSystem<Scene>());
		system.emplace(counter++, System::GenerateParticleSystem<Scene>());
		system.emplace(counter++, System::GenerateTileSystem<Scene>());
		system.emplace(counter++, System::GenerateIsometricMovementSystem<Scene>());
		system.emplace(counter++, System::GenerateIsometricCollisionSystem<Scene>());
		if constexpr (Scene == SCENE_OVERWORLD || Scene == SCENE_WILDLANDS || Scene == SCENE_TUTORIAL)
		{
			system.emplace(counter++, System::GeneratePetSystem<Scene>());
			system.emplace(counter++, System::GeneratePortalSystem<Scene>());
		}
		system.emplace(counter++, System::GenerateIsometricSystem<Scene>());
		system.emplace(counter++, System::GeneratePlayerCameraSystem<Scene>());
		if constexpr (Scene == SCENE_MINIGAME_DODGECOLLECTION || Scene == SCENE_MINIGAME_DODGING ||
			Scene == SCENE_MINIGAME_COLLECTION || Scene == SCENE_MINIGAME_STAGGEREDODGING)
		{
			system.emplace(counter++, System::GenerateArrowSystem<Scene>());
			system.emplace(counter++, System::GenerateSpawnerSystem<Scene>());
		}
		system.emplace(counter++, System::GenerateUILogicSystem<Scene>());
		system.emplace(counter++, System::GenerateRectTransformSystem<Scene>());
		system.emplace(counter++, System::GenerateIsometricRenderingSystem<Scene>());
		system.emplace(counter++, System::GenerateRectTransformRenderingSystem<Scene>());
		system.emplace(counter++, System::GenerateTextRenderingSystem<Scene>());
		system.emplace(counter++, System::GenerateMinigameSystem<Scene>());
		system.emplace(counter, System::GenerateCheeseSystem<Scene>());
		GenerateSceneSystem<Scene - 1>();
	}

	template<>
	inline auto ECS_World::GenerateSceneSystem<SCENE_MAINMENU>() -> void
	{
		auto& system = m_Systems.at(SCENE_MAINMENU);
		uint32_t counter = 0;
		system.emplace(counter++, System::GenerateUILogicSystem<SCENE_MAINMENU>());
		system.emplace(counter++, System::GenerateRectTransformSystem<SCENE_MAINMENU>());
		system.emplace(counter++, System::GenerateRectTransformRenderingSystem<SCENE_MAINMENU>());
		system.emplace(counter, System::GenerateTextRenderingSystem<SCENE_MAINMENU>());
	}

	template<>
	inline auto ECS_World::GenerateSceneSystem<SCENE_SPLASHSCREEN>() -> void
	{
		auto& system = m_Systems.at(SCENE_SPLASHSCREEN);
		uint32_t counter = 0;
		system.emplace(counter++, System::GenerateRectTransformSystem<SCENE_SPLASHSCREEN>());
		system.emplace(counter++, System::GenerateRectTransformRenderingSystem<SCENE_SPLASHSCREEN>());
		system.emplace(counter++, System::GenerateSplashScreenCheeseSystem<SCENE_SPLASHSCREEN>());
		GenerateSceneSystem<SCENE_SPLASHSCREEN - 1>();
	}

	template <>
	inline auto ECS_World::GenerateSceneSystem<SCENE_LEVELEDITOR>() -> void
	{
		auto& system = m_Systems.at(SCENE_LEVELEDITOR);
		uint32_t counter = 0;
		system.emplace(counter++, System::GenerateTileSystem<SCENE_LEVELEDITOR>());
		system.emplace(counter++, LevelEditor::GenerateLevelEditorPlayerSystem<SCENE_LEVELEDITOR>());
		system.emplace(counter++, LevelEditor::GenerateLevelEditorSystem<SCENE_LEVELEDITOR>());
		system.emplace(counter++, System::GenerateIsometricSystem<SCENE_LEVELEDITOR>());
		system.emplace(counter++, System::GenerateIsometricRenderingSystem<SCENE_LEVELEDITOR>());
		system.emplace(counter, LevelEditor::GeneratePrefabSystem<SCENE_LEVELEDITOR>());
		GenerateSceneSystem<SCENE_LEVELEDITOR - 1>();
	}
}
#endif

#endif