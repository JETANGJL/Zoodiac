// ======================================================================
// File Name:		ECS_Factory.cpp
// Project Name:	Zoodiac
// Author(s):		Low Zhi Sheng Kitson(20%), Lim Geng Yang(20%),
//					Lye Pin Liang Xavier(20%), Muhammad Dzulhafiz(20%),
//					Ang Jie Le Jet(20%)
// Brief:			Contains ECS Factory functionality
// 
// All content © 2023 DigiPen Institute of Technology Singapore. All 
// rights reserved.
// ======================================================================

#include <Backend/ECS/ECS.h>
#include <Backend/ECS/ECS_Factory.h>
#include <Backend/ECS/ECS_IncludeComponents.h>
#include <Backend/Engine_Common.h>
#include <States/StateIdle.h>

namespace Backend::ECS::Factory
{
	auto ECS_Factory::CreateTextEntity(const AEVec2& _pos, const std::string& _text, float _scale) -> ECS_Entity
	{
		const auto entity = ECS_WORLD.GenerateEntity();

		auto& text = ECS_WORLD.AddComponent<Component::TextRenderingComponent>(entity);
		text.m_Text.clear();
		text.m_Text.emplace_back(Component::TextRenderingComponent::TextInfo
			{ _text, _pos, _scale });
		text.m_Font = "a";
		ECS_WORLD.RegisterEntityIntoSystems(entity);
		return ECS_Entity();

	}

	auto ECS_Factory::CreateTileEntity(const AEVec2& _position, const float _height, const float, const float, const float,
		const std::string& _tile) -> ECS_Entity
	{
		const auto entity = ECS_WORLD.GenerateEntity();
		auto& transform = ECS_WORLD.AddComponent<Component::IsometricTransformComponent>(entity);
		transform.m_Position = { _position };
		transform.m_IsStatic = true;
		if (_tile == "water")transform.m_Tag = Component::IsometricTransformComponent::TAG_WATER;
		else transform.m_Tag = Component::IsometricTransformComponent::TAG_TILE;
		transform.m_Height = _height;
		transform.m_MinHeight = _height - 30;
		transform.m_Scale = { 1.f, 1.f };

		AEMtx33MultVec(&transform.m_IsometricPosition,
			&Component::IsometricTransformComponent::s_IsoTransform,
			&transform.m_Position);
		transform.m_IsometricPosition.y += transform.m_Height;

		auto& sprite = ECS_WORLD.AddComponent<ECS::Component::SpriteComponent>(entity);
		auto& tile = ECS_WORLD.AddComponent<ECS::Component::TileComponent>(entity);
		const std::string key = "Assets/tiles/" + _tile + ".png";
		sprite.m_TextureKey = key;
		sprite.m_Layer = Component::SpriteComponent::STATIC;
		tile.m_Red = sprite.m_Red = 1;//_r; //RandomInt(0, 100) / 100.0f;
		tile.m_Blue = sprite.m_Green = 1;//_g; //RandomInt(0, 100) / 100.0f;
		tile.m_Green = sprite.m_Blue = 1;//_b; //RandomInt(0, 100) / 100.0f;
		tile.m_Alpha = sprite.m_Alpha = 1;
		tile.m_Timer = 100;

		auto& collider = ECS_WORLD.AddComponent<Component::ColliderComponent>(entity);
		collider.m_Size = { 15,15 };
		collider.m_Tag = Component::ColliderComponent::TAG_TILE;

		ECS_WORLD.RegisterEntityIntoSystems(entity);
		return entity;
	}

	ECS_Entity ECS_Factory::CreateBorderEntity(const AEVec2& _position)
	{
		const auto entity = ECS_WORLD.GenerateEntity();
		auto& transform = ECS_WORLD.AddComponent<Component::IsometricTransformComponent>(entity);
		transform.m_Position = { _position };
		transform.m_IsStatic = true;
		transform.m_Tag = Component::IsometricTransformComponent::TAG_TILE;
		transform.m_Height = 1000;
		transform.m_MinHeight = -1000;
		transform.m_Scale = { 1.f, 1.f };

		AEMtx33MultVec(&transform.m_IsometricPosition,
			&Component::IsometricTransformComponent::s_IsoTransform,
			&transform.m_Position);
		transform.m_IsometricPosition.y += transform.m_Height;

		auto& collider = ECS_WORLD.AddComponent<Component::ColliderComponent>(entity);
		collider.m_Size = { 30,30 };
		collider.m_Tag = Component::ColliderComponent::TAG_BORDER;

		ECS_WORLD.RegisterEntityIntoSystems(entity);
		return entity;
	}

	ECS_Entity ECS_Factory::CreateDestroyerEntity(const AEVec2& _position, const float _height, const float _minHeight)
	{
		const auto entity = ECS_WORLD.GenerateEntity();
		auto& transform = ECS_WORLD.AddComponent<Component::IsometricTransformComponent>(entity);
		transform.m_Position = { _position };
		transform.m_IsStatic = true;
		transform.m_Tag = Component::IsometricTransformComponent::TAG_TILE;
		transform.m_Height = _height;
		transform.m_MinHeight = _minHeight;
		transform.m_Scale = { 1.f, 1.f };

		AEMtx33MultVec(&transform.m_IsometricPosition,
			&Component::IsometricTransformComponent::s_IsoTransform,
			&transform.m_Position);
		transform.m_IsometricPosition.y += transform.m_Height;

		auto& collider = ECS_WORLD.AddComponent<Component::ColliderComponent>(entity);
		collider.m_Size = { 15,15 };
		collider.m_Tag = Component::ColliderComponent::TAG_DESTROYER_TILE;

		ECS_WORLD.RegisterEntityIntoSystems(entity);
		return entity;
	}

	ECS_Entity ECS_Factory::CreatePlayerEntity(bool _followCamera)
	{
		const auto entity = ECS_WORLD.GenerateEntity();
		auto& transform = ECS_WORLD.AddComponent<Component::IsometricTransformComponent>(entity);
		transform.m_Position = { 0,0 };
		transform.m_IsStatic = false;
		transform.m_Tag = Component::IsometricTransformComponent::TAG_NONTILE;
		DEPENDENCY_INJECTOR.RegisterDependency(ECS_WORLD.GetCurrentScene(), DependencyInjector::Dependency::PLAYER_POS, &transform.Position());
		transform.m_Height = 30;
		transform.m_MinHeight = transform.m_Height - 30;
		transform.m_Scale = { 1.f, 1.f };

		auto& key = ECS_WORLD.AddComponent<ECS::Component::AnimationKeyComponent>(entity);
		key.m_Key = { "playerd", "playeru", "playerl", "playerr" };

		auto& animation = ECS_WORLD.AddComponent<ECS::Component::AnimationComponent>(entity);
		animation.m_Animations = ANIMATION_LIBRARY.GetResource(key.m_Key[0]);
		animation.m_TotalAnims = animation.m_Animations->size();

		auto& sprite = ECS_WORLD.AddComponent<Component::SpriteComponent>(entity);
		sprite.m_TextureKey = animation.m_Animations->at(0);
		sprite.m_Red = 1.0f;
		sprite.m_Blue = 1.0f;
		sprite.m_Green = 1.0f;
		sprite.m_Alpha = 1.0f;
		sprite.m_Layer = Component::SpriteComponent::DYNAMIC;

		auto& player = ECS_WORLD.AddComponent<Component::PlayerComponent>(entity);
		player.m_Hit = 0;
		player.m_Score = 0;
		player.m_FollowCamera = _followCamera;
		player.m_ScenePet = Component::PlayerComponent::PET_NIL;
		DEPENDENCY_INJECTOR.RegisterDependency(ECS_WORLD.GetCurrentScene(), DependencyInjector::Dependency::SCORE, &player.m_Score);
		DEPENDENCY_INJECTOR.RegisterDependency(ECS_WORLD.GetCurrentScene(), DependencyInjector::Dependency::HIT, &player.m_Hit);
		DEPENDENCY_INJECTOR.RegisterDependency(ECS_WORLD.GetCurrentScene(), DependencyInjector::Dependency::PET, &player.m_Pet);
		DEPENDENCY_INJECTOR.RegisterDependency(ECS_WORLD.GetCurrentScene(), DependencyInjector::Dependency::CURRENT_PET, &player.m_CurrentPet);
		DEPENDENCY_INJECTOR.RegisterDependency(ECS_WORLD.GetCurrentScene(), DependencyInjector::Dependency::SCENE_PET, &player.m_ScenePet);
		auto& movement = ECS_WORLD.AddComponent<Component::MovementComponent>(entity);
		movement.m_Jump = false;
		movement.m_OnGround = true;
		movement.m_MaxHeight = 60;
		movement.m_OriginalHeight = 0.0f;
		movement.m_JumpSpeed = 400.0f;
		movement.m_Gravity = 150.0f;
		movement.m_FallingSpeed = 0.0f;
		movement.m_Speed = 4.0f;

		auto& collider = ECS_WORLD.AddComponent<Component::ColliderComponent>(entity);
		collider.m_Size = { 15,15 };
		collider.m_Tag = Component::ColliderComponent::TAG_PLAYER;

		auto& emit = ECS_WORLD.AddComponent<Component::EmitterComponent>(entity);
		emit.m_Emit = false;
		emit.m_Type = Component::EmitterComponent::MOVE;

		ECS_WORLD.RegisterEntityIntoSystems(entity);
		return entity;
	}

	ECS_Entity ECS_Factory::CreateSpawnerEntity(const std::function<ECS_Entity(AEVec2, AEVec2)>& _spawnFunc,
		const AEVec2& _minRange, const AEVec2& _maxRange, const AEVec2& _direction,
		const unsigned _spawn, const float _timer, const float _startTime)
	{
		const auto entity = ECS_WORLD.GenerateEntity();
		auto& spawner = ECS_WORLD.AddComponent<Component::SpawnerComponent>(entity);
		spawner.m_Timer = _timer;
		spawner.m_CurrentTime = _startTime;
		spawner.m_GenerateEntity = _spawnFunc;
		spawner.m_MaxRange = _maxRange;
		spawner.m_MinRange = _minRange;
		spawner.m_SpawnCount = 15;
		spawner.m_SpawnPerInterval = _spawn;
		spawner.m_CurrSpawnCount = 0;
		spawner.m_Direction = _direction;

		ECS_WORLD.RegisterEntityIntoSystems(entity);
		return entity;
	}

	ECS_Entity ECS_Factory::CreateCheeseEntity(const AEVec2& _position, const AEVec2& _direction)
	{
		const auto entity = ECS_WORLD.GenerateEntity();
		auto& transform = ECS_WORLD.AddComponent<Component::IsometricTransformComponent>(entity);
		transform.m_IsStatic = false;
		transform.m_Position = _position;
		transform.m_MinHeight = transform.m_Height = 5 * 30.0f;
		transform.m_Tag = Component::IsometricTransformComponent::TAG_NONTILE;
		transform.m_Scale = { 1.f, 1.f };

		auto& sprite = ECS_WORLD.AddComponent<Component::SpriteComponent>(entity);
		sprite.m_TextureKey = "Assets/tiles/gender3.png";
		sprite.m_Red = 1.0f;
		sprite.m_Blue = 1.0f;
		sprite.m_Green = 1.0f;
		sprite.m_Alpha = 1.0f;
		sprite.m_Layer = Component::SpriteComponent::DYNAMIC;

		auto& collider = ECS_WORLD.AddComponent<Component::ColliderComponent>(entity);
		collider.m_Size = { 15,15 };
		collider.m_Tag = Component::ColliderComponent::TAG_INTERACTABLE;

		auto& movement = ECS_WORLD.AddComponent<Component::MovementComponent>(entity);
		movement.m_OnGround = false;
		movement.m_Gravity = 50.0f;
		movement.m_Jump = false;
		movement.m_JumpSpeed = 200.f;
		movement.m_FallingSpeed = 0.0f;
		movement.m_Direction = _direction;

		auto& emit = ECS_WORLD.AddComponent<Component::EmitterComponent>(entity);
		emit.m_Emit = false;
		emit.m_Type = Component::EmitterComponent::POOF;

		auto& cheese = ECS_WORLD.AddComponent<Component::CheeseComponent>(entity);
		cheese.m_OriginalTime = cheese.m_Lifetime = 4.0f;

		ECS_WORLD.RegisterEntityIntoSystems(entity);
		return entity;
	}

	ECS_Entity ECS_Factory::CreateDodgingBlockEntity(const AEVec2& _position, const AEVec2& _direction)
	{
		const auto entity = ECS_WORLD.GenerateEntity();
		auto& transform = ECS_WORLD.AddComponent<Component::IsometricTransformComponent>(entity);
		transform.m_IsStatic = false;
		transform.m_Position = _position;
		transform.m_Height = 1 * 30.0f;
		transform.m_MinHeight = transform.m_Height - 30.0f;
		transform.m_Tag = Component::IsometricTransformComponent::TAG_NONTILE; // change to NONTILE if using zoodiac assets
		transform.m_Scale = { 1.f, 1.f };

		auto& key = ECS_WORLD.AddComponent<ECS::Component::AnimationKeyComponent>(entity);
		key.m_Key = { "cowd", "cowu", "cowl", "cowr" };

		auto& animation = ECS_WORLD.AddComponent<ECS::Component::AnimationComponent>(entity);
		animation.m_AnimTime = 0.1f;
		animation.m_Animations = ANIMATION_LIBRARY.GetResource(key.m_Key[0]);
		animation.m_TotalAnims = animation.m_Animations->size();

		auto& sprite = ECS_WORLD.AddComponent<Component::SpriteComponent>(entity);
		sprite.m_TextureKey = animation.m_Animations->at(0);
		sprite.m_Red = 1.0f;
		sprite.m_Blue = 1.0f;
		sprite.m_Green = 1.0f;
		sprite.m_Alpha = 1.0f;
		sprite.m_Layer = Component::SpriteComponent::DYNAMIC;

		auto& collider = ECS_WORLD.AddComponent<Component::ColliderComponent>(entity);
		collider.m_Size = { 15,15 };
		collider.m_Tag = Component::ColliderComponent::TAG_SLIDING_TILE;

		auto& movement = ECS_WORLD.AddComponent<Component::MovementComponent>(entity);
		movement.m_OnGround = true;
		movement.m_Gravity = 0.0f;
		movement.m_Jump = false;
		movement.m_FallingSpeed = 0.0f;
		movement.m_Speed = 4.0f;
		movement.m_Direction = _direction;

		ECS_WORLD.RegisterEntityIntoSystems(entity);
		CreateArrowEntity(_position, _direction);
		return entity;
	}

	ECS_Entity ECS_Factory::CreateMinigameEntity(const uint32_t _target, const uint32_t _hitTarget,
		Component::MinigameComponent::Tag _tag, float _timer)
	{
		const auto entity = ECS_WORLD.GenerateEntity();
		auto& minigame = ECS_WORLD.AddComponent<Component::MinigameComponent>(entity);
		minigame.m_ScoreTarget = _target;
		minigame.m_HitTarget = _hitTarget;
		minigame.m_MinigameComplete = false;
		minigame.m_CurrTime = 0.0f;
		minigame.m_Tag = _tag;
		minigame.m_Timer = _timer;

		ECS_WORLD.RegisterEntityIntoSystems(entity);
		return entity;
	}

	ECS_Entity ECS_Factory::CreatePrefabEntity(const std::string& _prefabName, const ECS_Entity _entity)
	{
		const auto entity = _entity > ECS_MaxEntities ? ECS_WORLD.GenerateEntity() : _entity;
		const auto prefab = PREFAB_LIBRARY.GetResource(_prefabName);
		for (uint32_t i = 0; i < prefab->m_Signature.size(); ++i)
		{
			if (!prefab->m_Signature[i])
				continue;
			AddComponentViaPrefab<ECS_ALL_COMPONENTS>(entity, static_cast<ECS_ComponentType>(i) + 1, *prefab);
		}
		auto& transform = ECS_WORLD.GetComponent<Component::IsometricTransformComponent>(entity);
		transform.m_IsStatic = true;
		transform.m_Position = { 0,0 };
		transform.m_Height = 0;
		transform.m_MinHeight = 0 - 30;
		transform.m_Angle = 0;
		transform.m_Scale = { 1.f, 1.f };
		if (_prefabName == "water")
			transform.m_Tag = Component::IsometricTransformComponent::TAG_WATER;
		else if (_prefabName == "tree")
			transform.m_Tag = Component::IsometricTransformComponent::TAG_NONTILE;
		else
			transform.m_Tag = Component::IsometricTransformComponent::TAG_TILE;

		ECS_WORLD.RegisterEntityIntoSystems(entity);
		return entity;
	}

	ECS_Entity ECS_Factory::CreatePetEntity(Component::PlayerComponent::Pet_Type _pet)
	{
		const auto entity = ECS_WORLD.GenerateEntity();
		auto& transform = ECS_WORLD.AddComponent<ECS::Component::IsometricTransformComponent>(entity);
		if (ECS_WORLD.GetCurrentScene() == SCENE_OVERWORLD)
		{
			transform.m_Position = { -30.0, -750 };
			transform.m_Height = 60;
		}
		else
		{
			transform.m_Position = { 60,60 };
			transform.m_Height = 60;
		}
		transform.m_Scale = { 1.f, 1.f };
		transform.m_IsStatic = false;
		transform.m_MinHeight = transform.m_Height - 30;
		transform.m_Tag = Component::IsometricTransformComponent::TAG_NONTILE;

		auto& pet = ECS_WORLD.AddComponent<ECS::Component::PetComponent>(entity);
		pet.m_TimeSpent = 0;
		pet.m_Angle = 0;
		pet.m_Radius = 20;
		pet.m_State = pet.m_NextState = STATE_LIBRARY.GetResource(StateType::STATE_IDLE);

		auto& movement = ECS_WORLD.AddComponent<ECS::Component::MovementComponent>(entity);
		movement.m_Jump = false;
		movement.m_OnGround = true;
		movement.m_MaxHeight = 80;
		movement.m_OriginalHeight = 60.0f;
		movement.m_JumpSpeed = 300.0f;
		movement.m_Gravity = 150.0f;
		movement.m_FallingSpeed = 20.0f;
		movement.m_Speed = 2.f;

		auto& key = ECS_WORLD.AddComponent<ECS::Component::AnimationKeyComponent>(entity);

		switch (_pet)
		{
		case Component::PlayerComponent::PET_RAT:
			key.m_Key = { "ratd", "ratu", "ratl", "ratr" };
			movement.m_Speed = 2.5f;
			transform.m_Scale = { 0.5f, 0.5f };
			break;
		case Component::PlayerComponent::PET_DOG:
			key.m_Key = { "dogd", "dogu", "dogl", "dogr" };
			movement.m_Speed = 2.0f;
			break;
		case Component::PlayerComponent::PET_COW:
			key.m_Key = { "cowd", "cowu", "cowl", "cowr" };
			movement.m_Speed = 1.0f;
			break;
		case Component::PlayerComponent::PET_MONKEY:
			key.m_Key = { "monkeyd", "monkeyu", "monkeyl", "monkeyr" };
			transform.m_Scale = { 1.2f, 1.2f };
			movement.m_Speed = 1.5f;
			break;
		case Component::PlayerComponent::PET_CHICKEN:
			key.m_Key = { "chickend", "chickenu", "chickenl", "chickenr" };
			transform.m_Scale = { 0.5f, 0.5f };
			movement.m_Speed = 1.7f;
			break;
		default:
			key.m_Key = { "dogd", "dogu", "dogl", "dogr" };
		}

		auto& animation = ECS_WORLD.AddComponent<ECS::Component::AnimationComponent>(entity);
		animation.m_AnimTime = 0.1f;
		animation.m_Animations = ANIMATION_LIBRARY.GetResource(key.m_Key[0]);
		animation.m_TotalAnims = animation.m_Animations->size();

		auto& sprite = ECS_WORLD.AddComponent<ECS::Component::SpriteComponent>(entity);
		sprite.m_TextureKey = animation.m_Animations->at(0);
		sprite.m_Red = 1.0f;
		sprite.m_Blue = 1.0f;
		sprite.m_Green = 1.0f;
		sprite.m_Alpha = 1.0f;
		sprite.m_Layer = Component::SpriteComponent::DYNAMIC;

		auto& collider = ECS_WORLD.AddComponent<ECS::Component::ColliderComponent>(entity);
		collider.m_Size = { 15,15 };
		collider.m_Tag = ECS::Component::ColliderComponent::TAG_PET;

		ECS_WORLD.RegisterEntityIntoSystems(entity);

		return entity;
	}

	void ECS_Factory::CreateAllPetEntities(Component::PlayerComponent::Pet_Type _currentPet ,Component::PlayerComponent::Pet_Type _pet)
	{
		if (_pet == Component::PlayerComponent::Pet_Type::PET_NIL)
			return;
		
		for (Component::PlayerComponent::Pet_Type i = static_cast<Component::PlayerComponent::Pet_Type>( _currentPet + 1); i <= _pet; )
		{
			ECS::Factory::ECS_Factory::CreatePetEntity(i);
			i = static_cast<Component::PlayerComponent::Pet_Type>(i + 1);
		}
	}

	ECS_Entity ECS_Factory::CreateArrowEntity(const AEVec2& _position, const AEVec2& _direction)
	{
		const auto entity = ECS_WORLD.GenerateEntity();
		auto& arrow = ECS_WORLD.AddComponent<Component::ArrowComponent>(entity);
		if (_direction.x < 0 || _direction.y < 0)
		{
			arrow.m_StartPosition = _position + (_direction * 10 * 60);
			arrow.m_EndPosition = _position + (_direction * 5 * 60);
		}
		else
		{
			arrow.m_StartPosition = _position + (_direction * 5 * 60);
			arrow.m_EndPosition = arrow.m_StartPosition + (_direction * 5 * 60);
		}
		arrow.m_Lifetime = 2.f;

		auto& transform = ECS_WORLD.AddComponent<Component::IsometricTransformComponent>(entity);
		transform.m_IsStatic = false;
		transform.m_Position = _position + (_direction * 5 * 60);
		transform.m_Height = 1 * 30.0f;
		transform.m_MinHeight = transform.m_Height - 30.0f;
		transform.m_Scale = { 1.f, 1.f };
		transform.m_Tag = Component::IsometricTransformComponent::TAG_NONTILE;

		auto& sprite = ECS_WORLD.AddComponent<Component::SpriteComponent>(entity);
		sprite.m_TextureKey = "Assets/tiles/shadow.png";
		sprite.m_Red = 1.0f;
		sprite.m_Blue = 1.0f;
		sprite.m_Green = 1.0f;
		sprite.m_Alpha = 0.f;
		sprite.m_Layer = Component::SpriteComponent::DYNAMIC;

		auto& collider = ECS_WORLD.AddComponent<Component::ColliderComponent>(entity);
		collider.m_Size = { 15,15 };
		collider.m_Tag = Component::ColliderComponent::TAG_ARROW;

		auto& movement = ECS_WORLD.AddComponent<Component::MovementComponent>(entity);
		movement.m_OnGround = true;
		movement.m_Gravity = 0.0f;
		movement.m_Jump = false;
		movement.m_FallingSpeed = 0.0f;
		movement.m_Speed = 8.0f;
		movement.m_Direction = _direction; //2.f

		ECS_WORLD.RegisterEntityIntoSystems(entity);
		return entity;
	}

	ECS_Entity ECS_Factory::CreateButtonEntity(const AEVec2& _position, const std::string& _text, Component::UILogicComponent::Tag _tag)
	{
		const auto entity = ECS_WORLD.GenerateEntity();

		auto& transform = ECS_WORLD.AddComponent<ECS::Component::RectTransformComponent>(entity);
		transform.m_Position = { _position.x,_position.y };
		transform.m_Scale = { 3.5f, 1.6f };
		transform.m_Render = true;

		auto& sprite = ECS_WORLD.AddComponent<ECS::Component::SpriteComponent>(entity);
		sprite.m_TextureKey = "Assets/UI/brownrect.png";
		sprite.m_Red = 1.0f;
		sprite.m_Blue = 1.0f;
		sprite.m_Green = 1.0f;
		sprite.m_Alpha = 1.0f;
		sprite.m_Layer = Component::SpriteComponent::DYNAMIC;

		auto& ui = ECS_WORLD.AddComponent<ECS::Component::UILogicComponent>(entity);
		ui.m_Tag = _tag;

		auto& text = ECS_WORLD.AddComponent<ECS::Component::TextRenderingComponent>(entity);
		text.m_Text.emplace_back(Component::TextRenderingComponent::TextInfo
			{ _text,{ _position.x ,_position.y -5 },0.5f });

		ECS_WORLD.RegisterEntityIntoSystems(entity);
		return entity;
	}

	ECS_Entity ECS_Factory::CreateInstructionEntity(Scene _text)
	{
		const auto entity = ECS_WORLD.GenerateEntity();

		auto& transform = ECS_WORLD.AddComponent<ECS::Component::RectTransformComponent>(entity);
		transform.m_Position = { 0,0 };
		transform.m_Scale = { 11,9 };
		transform.m_Render = true;

		auto& sprite = ECS_WORLD.AddComponent<ECS::Component::SpriteComponent>(entity);
		sprite.m_TextureKey = "Assets/UI/howtoplay.png";
		sprite.m_Red = 1.0f;
		sprite.m_Blue = 1.0f;
		sprite.m_Green = 1.0f;
		sprite.m_Alpha = 1.0f;
		sprite.m_Layer = Component::SpriteComponent::DYNAMIC;

		auto& ui = ECS_WORLD.AddComponent<ECS::Component::UILogicComponent>(entity);
		ui.m_Tag = Component::UILogicComponent::Tag::TAG_MINIGAME_INSTRUCTION;

		auto& text = ECS_WORLD.AddComponent<ECS::Component::TextRenderingComponent>(entity);
		text.m_Text.clear();

		switch (_text)
		{
		case SCENE_MINIGAME_COLLECTION: 
			text.m_Text.emplace_back(Component::TextRenderingComponent::TextInfo
				{ "1) Collect the Cheese",{ 15.0f,110.0f }, 0.6f });
			text.m_Text.emplace_back(Component::TextRenderingComponent::TextInfo
				{ "2) Collect 5 to win",{ 25.0f,60.0f }, 0.6f });
			break;
		case SCENE_MINIGAME_CLIMBING: 
			text.m_Text.emplace_back(Component::TextRenderingComponent::TextInfo
				{ "1) Reach to the Top",{ 10.0f,110.0f }, 0.6f });
			text.m_Text.emplace_back(Component::TextRenderingComponent::TextInfo
				{ "2) Avoid Falling down!",{ 30.0f,60.0f }, 0.6f });
			break;
		case SCENE_MINIGAME_DODGING: 
			text.m_Text.emplace_back(Component::TextRenderingComponent::TextInfo
				{ "1) Animals are  coming",{ 10.0f,110.0f }, 0.6f });
			text.m_Text.emplace_back(Component::TextRenderingComponent::TextInfo
				{ "in all directions!!",{ 10.0f,80.0f }, 0.6f });
			text.m_Text.emplace_back(Component::TextRenderingComponent::TextInfo
				{ "2) Avoid all of them!!",{ 10.0f,30.0f }, 0.6f });
			break;
		case SCENE_MINIGAME_RUNNER: 
			text.m_Text.emplace_back(Component::TextRenderingComponent::TextInfo
				{ "1) Tiles are falling from ",{ 45.0f,110.0f }, 0.6f });
			text.m_Text.emplace_back(Component::TextRenderingComponent::TextInfo
				{ "the back",{ -70.0f,80.0f }, 0.6f });
			text.m_Text.emplace_back(Component::TextRenderingComponent::TextInfo
				{ "2) Run to the top fast!",{ 20.0f,30.0f }, 0.6f });
			break;
		case SCENE_MINIGAME_STAGGEREDODGING: 
			text.m_Text.emplace_back(Component::TextRenderingComponent::TextInfo
				{ "1) Animals are  coming ",{ 20.0f,110.0f }, 0.6f });
			text.m_Text.emplace_back(Component::TextRenderingComponent::TextInfo
				{ "2) Avoid all of them!! ",{ 25.0f,60.0f }, 0.6f });
			break;
		case SCENE_MINIGAME_DODGECOLLECTION: 
			text.m_Text.emplace_back(Component::TextRenderingComponent::TextInfo
				{ "1) Animals are  coming",{ 10.0f,110.0f }, 0.6f });
			text.m_Text.emplace_back(Component::TextRenderingComponent::TextInfo
				{ "in all directions!!",{ 10.0f,80.0f }, 0.6f });
			text.m_Text.emplace_back(Component::TextRenderingComponent::TextInfo
				{ "2) Avoid all of them!!",{ 10.0f,30.0f }, 0.6f });
			text.m_Text.emplace_back(Component::TextRenderingComponent::TextInfo
				{ "3) Collect 10 Cheese!!",{ 10.0f,-20.0f }, 0.6f });
			break;
		default: 
			break;
		}
		text.m_Text.emplace_back(Component::TextRenderingComponent::TextInfo
			{ "Press Spacebar to continue",{ 80.0f,-180.0f }, 0.4f });

		ECS_WORLD.RegisterEntityIntoSystems(entity);
		return entity;
	}

	ECS_Entity ECS_Factory::CreateParticleEntity(const std::string& _texture, AEVec2 _position, AEVec2 _dir, float _height, float _maxHeight, float _life, float _sizeBegin, float _sizeEnd, float _alphaBegin, float _alphaEnd, bool _fly)
	{
		const auto entity = ECS_WORLD.GenerateEntity();

		auto& particle = ECS_WORLD.AddComponent<Component::ParticleComponent>(entity);
		particle.m_Lifetime = _life;
		particle.m_LifeRemain = _life;
		particle.m_SizeBegin = _sizeBegin;
		particle.m_SizeEnd = _sizeEnd;
		particle.m_AlphaBegin = _alphaBegin;
		particle.m_AlphaEnd = _alphaEnd;

		auto& iso = ECS_WORLD.AddComponent<Component::IsometricTransformComponent>(entity);
		iso.m_Tag = Component::IsometricTransformComponent::TAG_NONTILE;
		iso.m_IsStatic = false;
		iso.m_Position = _position;
		iso.m_Height = _height;
		iso.m_MinHeight = _height - 30.f;
		iso.m_Angle = 0.f;
		iso.m_Scale = { particle.m_SizeBegin, particle.m_SizeBegin };

		auto& movement = ECS_WORLD.AddComponent<Component::MovementComponent>(entity);
		movement.m_Speed = 1.f;
		movement.m_Direction = _dir;
		movement.m_Jump = _fly;
		movement.m_OnGround = !_fly;
		movement.m_JumpSpeed = 300.f;
		movement.m_OriginalHeight = iso.m_Height;
		movement.m_FallingSpeed = 0.f;
		movement.m_Gravity = 50.f;

		auto& sprite = ECS_WORLD.AddComponent<ECS::Component::SpriteComponent>(entity);
		sprite.m_TextureKey = _texture;
		sprite.m_Red = 1.f;
		sprite.m_Blue = 1.f;
		sprite.m_Green = 1.f;
		sprite.m_Alpha = particle.m_AlphaBegin;
		sprite.m_Layer = Component::SpriteComponent::DYNAMIC;

		ECS_WORLD.RegisterEntityIntoSystems(entity);
		return entity;
	}

	ECS_Entity ECS_Factory::CreateBackgroundScreen(const std::string& _backgroundname)
	{
		const auto entity = ECS_WORLD.GenerateEntity();
		auto& transform = ECS_WORLD.AddComponent<ECS::Component::RectTransformComponent>(entity);
		transform.m_Position = { 0,0 };
		transform.m_Scale = { 29,17 };
		transform.m_Render = true;

		auto& sprite = ECS_WORLD.AddComponent<ECS::Component::SpriteComponent>(entity);
		if (_backgroundname == "menubackground")
		{
			sprite.m_TextureKey = "Assets/menubackground.png";
		}
		if (_backgroundname == "skybackground")
		{
			sprite.m_TextureKey = "Assets/UI/skybackground.png";
		}
		sprite.m_Red = 1.0f;
		sprite.m_Blue = 1.0f;
		sprite.m_Green = 1.0f;
		sprite.m_Alpha = 1.0f;

		ECS_WORLD.RegisterEntityIntoSystems(entity);
		return entity;
	}

	ECS_Entity ECS_Factory::CreateCreditScreen(int _pageNo)
	{
		const auto entity = ECS_WORLD.GenerateEntity();
		auto& transform = ECS_WORLD.AddComponent<ECS::Component::RectTransformComponent>(entity);
		transform.m_Position = { 0, 0 };
		transform.m_Scale = { 10,10 };
		transform.m_Render = true;

		auto& sprite = ECS_WORLD.AddComponent<ECS::Component::SpriteComponent>(entity);
		sprite.m_TextureKey = "Assets/UI/credits2_" + std::to_string(_pageNo) + ".png";
		sprite.m_Red = 1.0f;
		sprite.m_Blue = 1.0f;
		sprite.m_Green = 1.0f;
		sprite.m_Alpha = 1.0f;
		sprite.m_Layer = Component::SpriteComponent::DYNAMIC;

		auto& movement = ECS_WORLD.AddComponent<Component::MovementComponent>(entity);
		movement.m_Speed = 4.0f;
		movement.m_Direction = { 0,5 };

		ECS_WORLD.RegisterEntityIntoSystems(entity);
		return entity;
	}

	ECS_Entity ECS_Factory::CreateWinTextEntity(bool _win)
	{
		const auto entity = ECS_WORLD.GenerateEntity();

		auto& transform = ECS_WORLD.AddComponent<ECS::Component::RectTransformComponent>(entity);
		transform.m_Position = { 0,0};
		transform.m_Scale = { 12.f,7.f };
		transform.m_Render = true;

		auto& sprite = ECS_WORLD.AddComponent<ECS::Component::SpriteComponent>(entity);
		sprite.m_TextureKey = _win ? "Assets/UI/win.png" : "Assets/UI/lose.png";
		sprite.m_Red = 1.0f;
		sprite.m_Blue = 1.0f;
		sprite.m_Green = 1.0f;
		sprite.m_Alpha = 1.0f;
		sprite.m_Layer = Component::SpriteComponent::DYNAMIC;

		auto& ui = ECS_WORLD.AddComponent<ECS::Component::UILogicComponent>(entity);
		ui.m_Tag = Component::UILogicComponent::Tag::TAG_MINIGAME_ENDMINIGAME;

		auto& text = ECS_WORLD.AddComponent<ECS::Component::TextRenderingComponent>(entity);
		text.m_Text.clear();
		text.m_Text.emplace_back(Component::TextRenderingComponent::TextInfo
			{ "Press Spacebar to continue",{ 100,-70 }, 0.3f });

		ECS_WORLD.RegisterEntityIntoSystems(entity);
		return entity;
	}

	ECS_Entity ECS_Factory::CreatePauseButtonEntity()
	{	
		const auto entity = ECS_WORLD.GenerateEntity();

		auto& transform = ECS_WORLD.AddComponent<ECS::Component::RectTransformComponent>(entity);
		transform.m_Position = { 0,0 };
		transform.m_Scale = { 13.f,9.f };
		transform.m_Render = false;

		auto& sprite = ECS_WORLD.AddComponent<ECS::Component::SpriteComponent>(entity);
		sprite.m_TextureKey = "Assets/UI/pause.png";
		sprite.m_Red = 1.0f;
		sprite.m_Blue = 1.0f;
		sprite.m_Green = 1.0f;
		sprite.m_Alpha = 1.0f;
		sprite.m_Layer = Component::SpriteComponent::DYNAMIC;

		auto& ui = ECS_WORLD.AddComponent<ECS::Component::UILogicComponent>(entity);
		ui.m_Tag = Component::UILogicComponent::Tag::TAG_GAME_PAUSE;

		ECS_WORLD.RegisterEntityIntoSystems(entity);
		return entity;
	}

	ECS_Entity ECS_Factory::CreatePromptMinigameEntity()
	{
		const auto entity = ECS_WORLD.GenerateEntity();

		auto& transform = ECS_WORLD.AddComponent<ECS::Component::RectTransformComponent>(entity);
		transform.m_Position = { 0,-300.f };
		transform.m_Scale = { 10.f,4.f };
		transform.m_Render = true;

		auto& sprite = ECS_WORLD.AddComponent<ECS::Component::SpriteComponent>(entity);
		sprite.m_TextureKey = "Assets/UI/Dialogue.png";
		sprite.m_Red = 1.0f;
		sprite.m_Blue = 1.0f;
		sprite.m_Green = 1.0f;
		sprite.m_Alpha = 1.0f;
		sprite.m_Layer = Component::SpriteComponent::DYNAMIC;

		auto& ui = ECS_WORLD.AddComponent<ECS::Component::UILogicComponent>(entity);
		ui.m_Tag = Component::UILogicComponent::Tag::TAG_OVERWORLD_TELEPORTPROMPT;

		auto& text = ECS_WORLD.AddComponent<ECS::Component::TextRenderingComponent>(entity);
		text.m_Text.emplace_back(Component::TextRenderingComponent::TextInfo
			{ "Press 'e' to start minigame",{ -30.f,-320.f }, 0.6f });
		text.m_Text.emplace_back(Component::TextRenderingComponent::TextInfo
		{ "Press Spacebar to continue", { 100.f,-350.f }, 0.3f });
		ECS_WORLD.RegisterEntityIntoSystems(entity);
		return entity;
	}

	ECS_Entity ECS_Factory::CreateControlsEntity(AEVec2 _position)
	{
		const auto entity = ECS_WORLD.GenerateEntity();
		
		auto& transform = ECS_WORLD.AddComponent<ECS::Component::RectTransformComponent>(entity);
		transform.m_Position = { _position };
		transform.m_Scale = { 15.f,14.f };
		transform.m_Render = true;

		auto& sprite = ECS_WORLD.AddComponent<ECS::Component::SpriteComponent>(entity);
		sprite.m_TextureKey = "Assets/UI/Controls.png";
		sprite.m_Red = 1.0f;
		sprite.m_Blue = 1.0f;
		sprite.m_Green = 1.0f;
		sprite.m_Alpha = 1.0f;
		sprite.m_Layer = Component::SpriteComponent::DYNAMIC;

		auto& ui = ECS_WORLD.AddComponent<ECS::Component::UILogicComponent>(entity);
		ui.m_Tag = Component::UILogicComponent::Tag::TAG_TUTORIAL_CONTROLS;
		ECS_WORLD.RegisterEntityIntoSystems(entity);
		return entity;
	}

	ECS_Entity ECS_Factory::CreateSubPauseButtonEntity(const AEVec2& _position, Component::UILogicComponent::Tag _tag)
	{
		const auto entity = ECS_WORLD.GenerateEntity();

		auto& transform = ECS_WORLD.AddComponent<ECS::Component::RectTransformComponent>(entity);
		transform.m_Position = { _position };
		transform.m_Scale = { 3.255f,1.255f };
		transform.m_Render = false;

		auto& ui = ECS_WORLD.AddComponent<ECS::Component::UILogicComponent>(entity);
		ui.m_Tag = _tag;

		auto& sprite = ECS_WORLD.AddComponent<ECS::Component::SpriteComponent>(entity);

		switch(_tag)
		{
		case Component::UILogicComponent::TAG_PAUSEMENU_CONTINUE: 
			sprite.m_TextureKey = "Assets/UI/continuebutton.png";
			break;
		case Component::UILogicComponent::TAG_PAUSEMENU_RETURNTOMENU: 
			sprite.m_TextureKey = "Assets/UI/mainmenubutton.png";
			break;
		case Component::UILogicComponent::TAG_PAUSEMENU_CONTROLS:
			sprite.m_TextureKey = "Assets/UI/controlsbutton.png";
			break;
		case Component::UILogicComponent::TAG_PAUSEMENU_EXIT: 
			sprite.m_TextureKey = "Assets/UI/quitgamebutton.png";
			break;
		default: 
			break;
		}

		sprite.m_Red = 1.0f;
		sprite.m_Blue = 1.0f;
		sprite.m_Green = 1.0f;
		sprite.m_Alpha = 1.0f;
		sprite.m_Layer = Component::SpriteComponent::DYNAMIC;

		ECS_WORLD.RegisterEntityIntoSystems(entity);
		return entity;
	}

	auto ECS_Factory::CreatePauseMenu() -> void
	{
		CreatePauseButtonEntity();
		CreateSubPauseButtonEntity({ 20,90.f }, Component::UILogicComponent::TAG_PAUSEMENU_CONTINUE);
		CreateSubPauseButtonEntity({ 20,15.f }, Component::UILogicComponent::TAG_PAUSEMENU_RETURNTOMENU);
		CreateSubPauseButtonEntity({ 20,-60.f }, Component::UILogicComponent::TAG_PAUSEMENU_CONTROLS);
		CreateSubPauseButtonEntity({ 20,-135.f }, Component::UILogicComponent::TAG_PAUSEMENU_EXIT);
	}
	
	ECS_Entity ECS_Factory::CreateCloudEmitterEntity()
	{
		const auto entity = ECS_WORLD.GenerateEntity();
		
		auto& emit = ECS_WORLD.AddComponent<Component::EmitterComponent>(entity);
		emit.m_Emit = true;
		emit.m_Type = Component::EmitterComponent::CLOUD;

		auto& iso = ECS_WORLD.AddComponent<Component::IsometricTransformComponent>(entity);
		iso.m_Tag = Component::IsometricTransformComponent::TAG_NONTILE;
		iso.m_IsStatic = false;
		iso.m_Position = { -1000, 0 };
		iso.m_Height = -500;

		ECS_WORLD.RegisterEntityIntoSystems(entity);
		return entity;
	}

	ECS_Entity ECS_Factory::CreateSplashScreenEntity(int _index)
	{
		const auto entity = ECS_WORLD.GenerateEntity();
		auto& transform = ECS_WORLD.AddComponent<ECS::Component::RectTransformComponent>(entity);
		transform.m_Position = { 0,0 };
		if (_index)
			transform.m_Scale = { 16, 4 };
		else
			transform.m_Scale = { 20, 6 };
		transform.m_Render = true;

		auto& sprite = ECS_WORLD.AddComponent<ECS::Component::SpriteComponent>(entity);
		sprite.m_TextureKey = ANIMATION_LIBRARY.GetResource("splash")->at(_index);
		sprite.m_Red = 1.0f;
		sprite.m_Blue = 1.0f;
		sprite.m_Green = 1.0f;
		sprite.m_Alpha = 1.0f;
		sprite.m_Layer = Component::SpriteComponent::DYNAMIC;
		
		auto& timer = ECS_WORLD.AddComponent<ECS::Component::CheeseComponent>(entity);
		timer.m_Lifetime = timer.m_OriginalTime = 3.f;

		ECS_WORLD.RegisterEntityIntoSystems(entity);
		return entity;
	}


	ECS_Entity ECS_Factory::CreateTutorialEntity(AEVec2 _position)
	{
		const auto entity = ECS_WORLD.GenerateEntity();

		auto& transform = ECS_WORLD.AddComponent<ECS::Component::RectTransformComponent>(entity);
		transform.m_Position = { _position };
		transform.m_Scale = { 10.f,8.f };
		transform.m_Render = true;

		auto& sprite = ECS_WORLD.AddComponent<ECS::Component::SpriteComponent>(entity);
		sprite.m_TextureKey = "Assets/UI/TutorialPage.png";
		sprite.m_Red = 1.0f;
		sprite.m_Blue = 1.0f;
		sprite.m_Green = 1.0f;
		sprite.m_Alpha = 1.0f;
		sprite.m_Layer = Component::SpriteComponent::DYNAMIC;

		auto& ui = ECS_WORLD.AddComponent<ECS::Component::UILogicComponent>(entity);
		ui.m_Tag = Component::UILogicComponent::Tag::TAG_TUTORIALS_HOWTOPLAY;
		ECS_WORLD.RegisterEntityIntoSystems(entity);
		return entity;
	}
}
