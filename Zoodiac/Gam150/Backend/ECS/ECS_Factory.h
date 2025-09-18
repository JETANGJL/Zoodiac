// ======================================================================
// File Name:		ECS_Factory.h
// Project Name:	Zoodiac
// Author(s):		Low Zhi Sheng Kitson(20%), Lim Geng Yang(20%),
//					Lye Pin Liang Xavier(20%), Muhammad Dzulhafiz(20%),
//					Ang Jie Le Jet(20%)
// Brief:			Contains ECS Factory functionality
// 
// All content © 2023 DigiPen Institute of Technology Singapore. All 
// rights reserved.
// ======================================================================

#ifndef ECS_FACTORY_H_
#define ECS_FACTORY_H_

#include <AEEngine.h>
#include <functional>

#include <Backend/ECS/ECS_Common.h>
#include <Backend/ECS/ECS_Prefab.h>

namespace Backend::ECS::Factory
{
	/**
	 * \brief	Factory class for creating entities.
	 */
	class ECS_Factory
	{
	private:
		/**
		* \brief				Adds a component from prefab json.
		* \param _entity		Entity that owns the component to be added.
		* \param _componentID	ID of the component to be added.
		* \param _prefab		Prefab with data of component.
		* \return				Returns true if successful.
		*/
		template<typename Component1, typename Component2, typename ...ComponentOthers>
		static auto AddComponentViaPrefab(ECS_Entity _entity, uint32_t _componentID, ECS_Prefab& _prefab)->bool;

		/**
		* \brief				Adds a component from prefab json.
		* \param _entity		Entity that owns the component to be added.
		* \param _componentID	ID of the component to be added.
		* \param _prefab		Prefab with data of component.
		* \return				Returns true if successful.
		*/
		template<typename Component>
		static auto AddComponentViaPrefab(ECS_Entity _entity, uint32_t _componentID, ECS_Prefab& _prefab)->bool;
	public:
		/**
		* \brief		Creates text entity.
		* \param _pos	Position of the entity.
		* \param _text	Text to create.
		* \param _scale	Scale of the text.
		* \return		The entity.
		*/
		static auto CreateTextEntity(const AEVec2& _pos, const std::string& _text, float _scale)->ECS_Entity;

		/**
		* \brief			Creates tile entity.
		* \param _position	Position of the entity.
		* \param _height	Text to create.
		* \param _r			Red value.
		* \param _g			Green value.
		* \param _b			Blue value.
		* \param _tile		Tile sprite.
		* \return			The entity.
		*/
		static auto CreateTileEntity(const AEVec2& _position, float _height = 0, float _r = 0, float _g = 0,
			float _b = 0, const std::string& _tile = "rtile")->ECS_Entity;

		/**
		* \brief			Creates border entity.
		* \param _position	Position of the entity.
		* \return			The entity.
		*/
		static auto CreateBorderEntity(const AEVec2& _position)->ECS_Entity;

		/**
		* \brief			Creates destroyer entity.
		* \param _position	Position of the entity.
		* \param _height	Height of the entity.
		* \return			The entity.
		*/
		static auto CreateDestroyerEntity(const AEVec2& _position, float _height = 1000,
			float _minHeight = -1000)->ECS_Entity;

		/**
		* \brief				Creates player entity.
		* \param _followCamera	Camera follows player if true.
		* \return				The entity.
		*/
		static auto CreatePlayerEntity(bool _followCamera = true)->ECS_Entity;

		/**
		* \brief	Creates spawner entity.
		* \return	The entity.
		*/
		static auto CreateSpawnerEntity(const std::function<ECS_Entity(AEVec2, AEVec2)>& _spawnFunc, const AEVec2& _minRange,
			const AEVec2& _maxRange, const AEVec2& _direction, unsigned _spawn, float _timer,
			float _startTime = 0)->ECS_Entity;

		/**
		* \brief			Creates cheese.
		* \param _position	Position of the cheese.
		* \param _direction	Direction of the cheese.
		* \return			The cheese.
		*/
		static auto CreateCheeseEntity(const AEVec2& _position, const AEVec2& _direction)->ECS_Entity;

		/**
		* \brief			Creates dodging block entity.
		* \param _position	Position of the dodging block.
		* \param _direction	Direction of the dodging block.
		* \return			The entity.
		*/
		static auto CreateDodgingBlockEntity(const AEVec2& _position, const AEVec2& _direction)->ECS_Entity;

		/**
		* \brief				Creates minigame entity.
		* \return				The Entity.
		*/
		static auto CreateMinigameEntity(uint32_t _target, uint32_t _hitTarget, Component::MinigameComponent::Tag _tag = Component::MinigameComponent::Tag::TAG_SCORE, float _timer = 0.0f) -> ECS_Entity;
 
		/**
		* \brief				Creates prefab entity.
		* \param _prefabName	Prefab Name.
		* \param _entity		The Entity.
		* \return				The Entity.
		*/
		static auto CreatePrefabEntity(const std::string& _prefabName,
			ECS_Entity _entity = ECS_MaxEntities + 1)->ECS_Entity;

		/**
		* \brief		Creates pet entity.
		* \param _pet	Pet type.
		* \return		The Entity.
		*/
		static auto CreatePetEntity(Component::PlayerComponent::Pet_Type _pet)->ECS_Entity;

		/**
		* \brief				Creates all pet entity.
		* \param _currentPet	Current player pet type.
		* \param _pet			Last pet unlocked.
		* \return				The Entity.
		*/
		static auto CreateAllPetEntities(Component::PlayerComponent::Pet_Type _currentPet, Component::PlayerComponent::Pet_Type _pet) -> void;

		/**
		* \brief			Creates arrow entity.
		* \param _position	Position of the arrow entity.
		* \param _direction	Direction of the arrow entity.
		* \return			The entity.
		*/
		static auto CreateArrowEntity(const AEVec2& _position, const AEVec2& _direction)->ECS_Entity;

		/**
		* \brief			Creates button entity.
		* \param _position	Position of the button entity.
		* \param _text		Text of the button entity.
		* \param _tag		Type the button entity.
		* \return			The entity.
		*/
		static auto CreateButtonEntity(const AEVec2& _position, const std::string& _text, Component::UILogicComponent::Tag _tag)->ECS_Entity;

		/**
		* \brief		Creates instruction entity.
		* \param _text	Text of the instruction entity.
		* \return		The entity.
		*/
		static auto CreateInstructionEntity(Scene _text)->ECS_Entity;

		/**
		* \brief					Creates background entity.
		* \param _backgroundname	Background name.
		* \return					The entity.
		*/
		static auto CreateBackgroundScreen(const std::string& _backgroundname)->ECS_Entity;

		/**
		* \brief			Creates _pageNo entity.
		* \param _pageNo	Page number of the credits.
		* \return			The entity.
		*/
		static auto CreateCreditScreen(int _pageNo) -> ECS_Entity;

		/**
		* \brief		Creates win text entity.
		* \param _win	Win if true, lose if false.
		* \return		The entity.
		*/
		static auto CreateWinTextEntity(bool _win = true)->ECS_Entity;

		/**
		* \brief		Creates pause button entity.
		* \return		The entity.
		*/
		static auto CreatePauseButtonEntity()->ECS_Entity;

		/**
		* \brief		Creates prompt minigame entity.
		* \return		The entity.
		*/
		static auto CreatePromptMinigameEntity()->ECS_Entity;

		/**
		* \brief			Creates controls entity.
		* \param _position	Position of the entity;
		* \return			The entity.
		*/
		static auto CreateControlsEntity(AEVec2 _position)->ECS_Entity;

		/**
		* \brief			Creates sub pause button entity.
		* \param _position	Position of the entity;
		* \param _tag		Tag of the entity;
		* \return			The entity.
		*/
		static auto CreateSubPauseButtonEntity(const AEVec2& _position, Component::UILogicComponent::Tag _tag)->ECS_Entity;

		/**
		* \brief		Creates pause menu.
		* \return		The entity.
		*/
		static auto CreatePauseMenu() -> void;

		/**
		* \brief	Creates particle entity.
		* \param	Particle behaviour.
		* \return	The entity.
		*/
		static auto CreateParticleEntity(const std::string& _texture, AEVec2 _position, AEVec2 _dir, float _height, float _maxHeight, float _life, float _sizeBegin, float _sizeEnd, float _alphaBegin, float _alphaEnd, bool _fly) -> ECS_Entity;

		/**
		* \brief		Creates cloud emitter menu.
		* \return		The entity.
		*/
		static auto CreateCloudEmitterEntity() -> ECS_Entity;

		/**
		* \brief		Creates splash screen entity.
		* \param _index	Index of splash screen.
		* \return		The entity.
		*/
		static auto CreateSplashScreenEntity(int _index) -> ECS_Entity;

		static auto CreateTutorialEntity(AEVec2 _position)->ECS_Entity;

	};

}

#include <Backend/ECS/ECS.h>

namespace Backend::ECS::Factory
{

	template <typename Component1, typename Component2, typename ... ComponentOthers>
	auto ECS_Factory::AddComponentViaPrefab(const ECS_Entity _entity, const uint32_t _componentID,
	                                        ECS_Prefab& _prefab) -> bool
	{
		if(AddComponentViaPrefab<Component1>(_entity,_componentID,_prefab))
			return true;
		return AddComponentViaPrefab<Component2, ComponentOthers...>(_entity, _componentID, _prefab);
	}

	template <typename Component>
	auto ECS_Factory::AddComponentViaPrefab(const ECS_Entity _entity, const uint32_t _componentID,
	                                        ECS_Prefab& _prefab) -> bool
	{
		if (_componentID != ECS_Component::GetComponentID<Component>())
			return false;
		const auto& component = _prefab.GetComponent<ECS_ALL_COMPONENTS>(_componentID);
		auto& addComponent = ECS_WORLD.AddComponent<Component>(_entity);
		addComponent = *(std::get<Component*>(component.value()));
		addComponent.Entity(_entity);
		return true;
	}
}

#endif