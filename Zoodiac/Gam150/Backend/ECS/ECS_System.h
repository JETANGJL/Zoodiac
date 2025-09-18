// ======================================================================
// File Name:		ECS_System.cpp
// Project Name:	Zoodiac
// Author(s):		Low Zhi Sheng Kitson(20%), Lim Geng Yang(20%),
//					Lye Pin Liang Xavier(20%), Muhammad Dzulhafiz(20%),
//					Ang Jie Le Jet(20%)
// Brief:			Contains Specific ECS Systems functionality
// 
// All content © 2023 DigiPen Institute of Technology Singapore. All 
// rights reserved.
// ======================================================================

#ifndef ECS_SYSTEM_H_
#define ECS_SYSTEM_H_

#include <iostream>
#include <ranges>
#include <tuple>
#include <cmath>

#include <AEEngine.h>
#include <Backend/Engine_Common.h>
#include <Backend/Utils.h>
#include <Backend/VecOperators.h>
#include <Backend/ECS/ECS_Factory.h>
#include <Backend/ECS/ECS_IncludeComponents.h>
#include <Backend/Graphics/Mesh.h>

namespace Backend::ECS::System
{
	using AnimationSystem			= ECS_System<Component::SpriteComponent, Component::AnimationComponent>;
	using IsometricSystem			= ECS_System<Component::IsometricTransformComponent>;
	using IsometricRendererSystem	= ECS_System<Component::IsometricTransformComponent, Component::SpriteComponent>;
	using IsometricMovementSystem	= ECS_System<Component::IsometricTransformComponent, Component::MovementComponent>;
	using PlayerSystem				= ECS_System<Component::PlayerComponent, Component::MovementComponent, Component::IsometricTransformComponent>;
	using IsometricCollisionSystem	= ECS_System<Component::IsometricTransformComponent, Component::ColliderComponent>;
	using SpawnerSystem				= ECS_System<Component::SpawnerComponent>;
	using TileSystem				= ECS_System<Component::TileComponent, Component::SpriteComponent>;
	using PlayerCameraSystem		= ECS_System<Component::PlayerComponent, Component::IsometricTransformComponent>;
	using MinigameSystem			= ECS_System<Component::MinigameComponent>;
	using PetSystem					= ECS_System<Component::PetComponent>;
	using ArrowSystem				= ECS_System<Component::ArrowComponent, Component::IsometricTransformComponent>;
	using TextRenderingSystem       = ECS_System<Component::TextRenderingComponent>;
	using UILogicSystem 			= ECS_System<Component::UILogicComponent, Component::RectTransformComponent>;
	using RectTransformSystem 		= ECS_System<Component::RectTransformComponent>;
	using UIRenderingSystem			= ECS_System<Component::RectTransformComponent, Component::SpriteComponent>;
	using PortalResponseSystem		= ECS_System<Component::PortalComponent>;
	using AnimationLogicSystem 		= ECS_System<Component::AnimationComponent, Component::MovementComponent, Component::AnimationKeyComponent>;
	using MovementBasedEmitterSystem= ECS_System<Component::EmitterComponent, Component::MovementComponent>;
	using EmitterSystem				= ECS_System<Component::EmitterComponent, Component::IsometricTransformComponent>;
	using ParticleSystem			= ECS_System<Component::ParticleComponent, Component::IsometricTransformComponent, Component::SpriteComponent>;
	using CheeseSystem				= ECS_System<Component::CheeseComponent, Component::SpriteComponent>;

	/**
	 * \brief			Generates the animation system.
	 * \tparam Scene	The scene.
	 * \return			The animation system.
	 */
	template<int Scene>
	AnimationSystem& GenerateAnimationSystem()
	{
		static auto updateFunc = 
			[](const std::tuple<Component::SpriteComponent*, Component::AnimationComponent*>& _components)
		{
			auto& animation = std::get<Component::AnimationComponent*>(_components);
			animation->m_CurrTime += GetDeltaTime();
			if (animation->m_CurrTime > animation->m_AnimTime)
			{
				auto& sprite = std::get<Component::SpriteComponent*>(_components);
				animation->m_CurrFrameIndex = (++animation->m_CurrFrameIndex < animation->m_TotalAnims) ? animation->m_CurrFrameIndex : 0;
				sprite->m_TextureKey = animation->m_Animations->at(animation->m_CurrFrameIndex);
				animation->m_CurrTime = 0;
			}
		};
		static AnimationSystem animationSystem{ updateFunc };
		return animationSystem;
	}

	/**
	 * \brief			Generates the isometric system.
	 * \tparam Scene	The scene.
	 * \return			The isometric system.
	 */
	template<int Scene>
	IsometricSystem& GenerateIsometricSystem()
	{
		static float globalRot{}, globalScale = 1.f;
		static bool trip = false, rotFlip = false, scaleFlip;
		static float elapsed{}, dt;
		static auto preUpdateFunc =
			[]()
		{
				
			dt = GetDeltaTime();
			elapsed += dt;
			if (AEInputCheckTriggered(AEVK_Q))
				trip = !trip;
			if (trip)
			{
				if (globalRot > 10 || globalRot < -10)
					rotFlip = !rotFlip;
				if (rotFlip)
					globalRot += dt * 15.f;
				else
					globalRot -= dt * 15.f;

				if (globalScale > 1.2f || globalScale < 0.8f)
					scaleFlip = !scaleFlip;
				if (scaleFlip)
					globalScale += dt;
				else
					globalScale -= dt;
			}
			else
			{
				globalRot = 0;
				globalScale = 1.f;
			}
		};

		static auto updateFunc =
			[](const std::tuple<Component::IsometricTransformComponent*>& _components)
		{
			auto& iso = std::get<Component::IsometricTransformComponent*>(_components);
			AEMtx33 rotMat, scaleMat, transMat;
			iso->m_IsometricPosition = iso->m_Position * Component::IsometricTransformComponent::s_IsoTransform;
			if (iso->m_Tag == Component::IsometricTransformComponent::TAG_NONTILE)
				iso->m_IsometricPosition.y += iso->m_Height + (15 * iso->m_Scale.y); // plus 1/4 size of sprite (change once scale is available)
			else if (iso->m_Tag == Component::IsometricTransformComponent::TAG_WATER)
				iso->m_IsometricPosition.y += iso->m_Height + 7 * sin(5 * (elapsed + (f32)(iso->m_Position.x + iso->m_Position.y)));
			else if (iso->m_Tag == Component::IsometricTransformComponent::TAG_FALLING)
				iso->m_IsometricPosition.y += iso->m_Height + 4 * sin(20 * (elapsed + (f32)(iso->m_Position.x + iso->m_Position.y)));
			else 
                iso->m_IsometricPosition.y += iso->m_Height;
			AEMtx33Scale(&scaleMat, iso->m_Scale.x, iso->m_Scale.y);
			AEMtx33RotDeg(&rotMat, iso->m_Angle);
			AEMtx33Trans(&transMat, iso->m_IsometricPosition.x, iso->m_IsometricPosition.y);
			AEMtx33Concat(&iso->m_Transform, &scaleMat, &rotMat);
			AEMtx33Concat(&iso->m_Transform, &transMat, &iso->m_Transform);
			if (trip)
			{
				const AEVec2 offset{
					-DI_GET_PLAYER_POS()->x + iso->m_IsometricPosition.x,
					-DI_GET_PLAYER_POS()->y + iso->m_IsometricPosition.y
				};
				AEMtx33Scale(&scaleMat, globalScale, globalScale);
				AEMtx33RotDeg(&rotMat, globalRot);
				AEMtx33Trans(&transMat, offset.x, offset.y);
				AEMtx33Concat(&iso->m_Transform, &transMat, &iso->m_Transform);
				AEMtx33Concat(&iso->m_Transform, &rotMat, &iso->m_Transform);
				AEMtx33Concat(&iso->m_Transform, &scaleMat, &iso->m_Transform);
				AEMtx33Trans(&transMat, -offset.x, -offset.y);
				AEMtx33Concat(&iso->m_Transform, &transMat, &iso->m_Transform);
			}
		};
		static auto postUpdateFunc =
			[]()
		{
		};
		static IsometricSystem isometricSystem{ updateFunc, nullptr,  preUpdateFunc ,postUpdateFunc };
		return isometricSystem;
	}

	/**
	 * \brief			Generates the isometric rendering system.
	 * \tparam Scene	The scene.
	 * \return			The isometric rendering system.
	 */
	template<int Scene>
	IsometricRendererSystem& GenerateIsometricRenderingSystem()
	{
		static f32 cullMinY, cullMaxY, cullMinX, cullMaxX;
		static auto preUpdateFunc = []()
		{
			cullMinY = AEGfxGetWinMinY();
			cullMaxY = AEGfxGetWinMaxY();
			cullMinX = AEGfxGetWinMinX();
			cullMaxX = AEGfxGetWinMaxX();
		};

		static Mesh mesh;
		static auto updateFunc =
			[](const std::tuple<Component::IsometricTransformComponent*, Component::SpriteComponent*>& _components)
		{
			const auto& transform = std::get<Component::IsometricTransformComponent*>(_components);
			const auto& sprite = std::get<Component::SpriteComponent*>(_components);
			if (transform->m_Transform.m[0][2]+ transform->m_Scale.x * 30 <= cullMinX || transform->m_Transform.m[0][2] - transform->m_Scale.x * 30 >= cullMaxX ||
				transform->m_Transform.m[1][2] + transform->m_Scale.y * 30 <= cullMinY || transform->m_Transform.m[1][2] - transform->m_Scale.y * 30 >= cullMaxY)
				return;

			auto texture = TEXTURE_LIBRARY.GetResource(sprite->m_TextureKey);
			if (!texture)
			{
				TEXTURE_LIBRARY.AddResource(sprite->m_TextureKey, AEGfxTextureLoad(sprite->m_TextureKey.c_str()));
				texture = TEXTURE_LIBRARY.GetResource(sprite->m_TextureKey);
			}

			mesh.RenderMesh(transform->m_Transform, texture, sprite->m_Red, sprite->m_Green, sprite->m_Blue, sprite->m_Alpha);
		};
		static IsometricRendererSystem isometricRenderingSystem{ updateFunc, nullptr, preUpdateFunc };

		static auto sortFunc =
			[](ECS_Entity const& _first, ECS_Entity const& _second) -> bool
		{
			auto& firstTransform = std::get<Component::IsometricTransformComponent*>(isometricRenderingSystem.m_Components.at(_first));
			auto& secondTransform = std::get<Component::IsometricTransformComponent*>(isometricRenderingSystem.m_Components.at(_second));
			auto& firstSprite = std::get<Component::SpriteComponent*>(isometricRenderingSystem.m_Components.at(_first));
			auto& secondSprite = std::get<Component::SpriteComponent*>(isometricRenderingSystem.m_Components.at(_second));

			float firstY = firstTransform->m_Position.y;
			float secondY = secondTransform->m_Position.y;
			if (firstSprite->m_Layer == Component::SpriteComponent::DYNAMIC)
				firstY -= 30;
			if (secondSprite->m_Layer == Component::SpriteComponent::DYNAMIC)
				secondY -= 30;

			if (firstY > secondY)
				return true;
			if (firstY == secondY && (firstTransform->m_Position.x > secondTransform->m_Position.x))
				return true;
			if ((firstY == secondY && firstTransform->m_Position.x == secondTransform->m_Position.x) &&
				firstTransform->m_IsometricPosition.y < secondTransform->m_IsometricPosition.y)
				return true;

			return false;
		};
		isometricRenderingSystem.m_CompareFunction = sortFunc;
		isometricRenderingSystem.m_UseSortedOrder = true;
		return isometricRenderingSystem;
	}

	/**
	 * \brief			Generates the isometric movement system.
	 * \tparam Scene	The scene.
	 * \return			The isometric movement system.
	 */
	template<int Scene>
	IsometricMovementSystem& GenerateIsometricMovementSystem()
	{
		static auto updateFunc = 
			[](const std::tuple<Component::IsometricTransformComponent*, Component::MovementComponent*>& _components)
		{
			auto& transform = std::get<Component::IsometricTransformComponent*>(_components);
			auto& movement = std::get<Component::MovementComponent*>(_components);
			movement->m_PrevPosition = transform->m_Position;
			const float movementMultiplier = GetDeltaTime() * 35.0f;
			transform->m_Position += movement->m_Direction * movement->m_Speed * movementMultiplier;

			if (movement->m_Jump)
			{
				if (transform->m_Height < movement->m_OriginalHeight + movement->m_MaxHeight)
				{
					transform->m_Height += GetDeltaTime() * movement->m_JumpSpeed;
					transform->m_MinHeight += GetDeltaTime() * movement->m_JumpSpeed;
				}
				else
					movement->m_Jump = false;
			}
			else if (!movement->m_OnGround && !movement->m_Jump)
			{
				movement->m_FallingSpeed += movement->m_Gravity;
				if (movement->m_FallingSpeed > movement->m_JumpSpeed)
				{
					movement->m_FallingSpeed = movement->m_JumpSpeed;
				}
				transform->m_Height -= movement->m_FallingSpeed * GetDeltaTime();
				transform->m_MinHeight -= movement->m_FallingSpeed * GetDeltaTime();
			}
		};
		static IsometricMovementSystem isometricMovementSystem{ updateFunc };
		return isometricMovementSystem;
	}

	/**
	 * \brief			Generates the player system.
	 * \tparam Scene	The scene.
	 * \return			The player system.
	 */
	template<int Scene>
	PlayerSystem& GeneratePlayerSystem()
	{
		static auto updateFunc = 
			[](const std::tuple<Component::PlayerComponent*, Component::MovementComponent*, Component::IsometricTransformComponent*>& _components)
		{
			auto& iso = std::get<Component::IsometricTransformComponent*>(_components);
			auto& movement = std::get<Component::MovementComponent*>(_components);
			movement->m_Direction.x = 0.f;
			movement->m_Direction.y = 0.f;
			if (AEInputCheckCurr(AEVK_W) && AEInputCheckCurr(AEVK_A)) // CARTESEAN up
				movement->m_Direction.y = 1.f;
			else if (AEInputCheckCurr(AEVK_S) && AEInputCheckCurr(AEVK_D)) // CARTESEAN down
				movement->m_Direction.y = -1.f;
			else if (AEInputCheckCurr(AEVK_A) && AEInputCheckCurr(AEVK_S)) // CARTESEAN left
				movement->m_Direction.x = -1.f;
			else if (AEInputCheckCurr(AEVK_W) && AEInputCheckCurr(AEVK_D)) // CARTESEAN right
				movement->m_Direction.x = 1.f;
			else
			{
				if (AEInputCheckCurr(AEVK_W))
					movement->m_Direction.y = 1.f;
				else if (AEInputCheckCurr(AEVK_S))
					movement->m_Direction.y = -1.f;
				if (AEInputCheckCurr(AEVK_A))
					movement->m_Direction.x = -1.f;
				else if (AEInputCheckCurr(AEVK_D))
					movement->m_Direction.x = 1.f;
				if (movement->m_Direction.x != 0.f && movement->m_Direction.y != 0.f)
					NormalizeVec2(movement->m_Direction);
				movement->m_Direction *= Component::IsometricTransformComponent::s_IsoInverse;
			}
			if (AEInputCheckTriggered(AEVK_SPACE))
			{
				if (movement->m_OnGround)
				{
					AUDIO_MANAGER.PlayAudio(Audio::AudioManager::Sound::JUMP_SOUND, 0);
					movement->m_OriginalHeight = iso->m_Height;
					movement->m_Jump = true;
					movement->m_OnGround = false;
				}
			}
			
			auto& player = std::get<Component::PlayerComponent*>(_components);

			if(iso->m_Height < -300)
			{
				iso->m_Height = 90;
				iso->m_MinHeight = iso->m_Height - 30;
				iso->m_Position.x = 0;
				iso->m_Position.y = 0;
				if (ECS_WORLD.GetCurrentScene() > SCENE_WILDLANDS)
				{
					player->m_Hit++;
				}
			}

			if (ECS_WORLD.GetCurrentScene() == SCENE_OVERWORLD)
			{
				if (*DI_GET_SCENE_PET() < *DI_GET_CURRENT_PET())
				{
					ECS::Factory::ECS_Factory::CreateAllPetEntities(*DI_GET_SCENE_PET(), *DI_GET_CURRENT_PET());
					*DI_GET_SCENE_PET() = *DI_GET_CURRENT_PET();
				}
			}

		};
		static PlayerSystem playerSystem{ updateFunc };
		return playerSystem;
	}

	/**
	 * \brief				Add Entity to Collision Map
	 * \param _collisions	Map
	 * \param _lhs			Entity 1
	 * \param _rhs			Entity 2
	 */
	inline auto AddToCollisionMap(std::unordered_map<ECS_Entity, std::vector<ECS_Entity>>& _collisions, const ECS_Entity& _lhs, const ECS_Entity& _rhs)->void
	{
		if (!_collisions.contains(_lhs))
			_collisions.emplace(_lhs, std::vector<ECS_Entity>{});
		_collisions.at(_lhs).push_back(_rhs);
	}

	/**
	 * \brief			Generates the collision system.
	 * \tparam Scene	The scene.
	 * \return			The collision system.
	 */
	template<int Scene>
	IsometricCollisionSystem& GenerateIsometricCollisionSystem()
	{
		static std::unordered_map<ECS_Entity, std::vector<ECS_Entity>> collisions;
		static std::unordered_map<ECS_Entity, std::vector<ECS_Entity>> isoCollisions;
		static IsometricCollisionSystem collisionSystem{ nullptr };
		static auto updateFunc = [](const std::tuple<Component::IsometricTransformComponent*, Component::ColliderComponent*>& _components)
		{
			auto& iso = std::get<Component::IsometricTransformComponent*>(_components);
			if (iso->m_IsStatic)
				return;
			auto& collider = std::get<Component::ColliderComponent*>(_components);

			for (auto& val : collisionSystem.m_Components | std::views::values)
			{
				auto& isoOther = std::get<Component::IsometricTransformComponent*>(val);

				if (isoOther->Entity() == iso->Entity())
					continue;
				auto& otherCollider = std::get<Component::ColliderComponent*>(val);

				bool inObject = AETestRectToRect(&iso->m_Position, collider->m_Size.x, collider->m_Size.y, &isoOther->m_Position, otherCollider->m_Size.x, otherCollider->m_Size.y)
					&& (iso->m_MinHeight <= isoOther->m_Height && iso->m_Height >= isoOther->m_MinHeight);
				if (inObject)
					AddToCollisionMap(collisions, iso->Entity(), isoOther->Entity());
				inObject = AETestRectToRect(&iso->m_IsometricPosition, 45, 45, &isoOther->m_IsometricPosition, 45, 45);
				if (inObject)
					AddToCollisionMap(isoCollisions, iso->Entity(), isoOther->Entity());
			}
			if (collider->m_Tag == Component::ColliderComponent::TAG_PLAYER && !collisions.contains(iso->Entity()))
			{
				auto& movement = ECS_WORLD.GetComponent<Component::MovementComponent>(iso->Entity());
				movement.m_OnGround = false;
			}
		};

		static auto postUpdateFunc = []()
		{
			for (const auto& key : collisions | std::views::keys)
			{
				auto& collider = std::get<Component::ColliderComponent*>(collisionSystem.m_Components.at(key));
				if (collider->m_Tag == Component::ColliderComponent::TAG_TILE || collider->m_Tag == Component::ColliderComponent::TAG_DESTROYER_TILE)
					continue;
				auto& iso = std::get<Component::IsometricTransformComponent*>(collisionSystem.m_Components.at(key));
				auto& movement = ECS_WORLD.GetComponent<Component::MovementComponent>(key);
				for (const auto& j : collisions.at(key))
				{
					switch (auto& otherCollider = std::get<Component::ColliderComponent*>(collisionSystem.m_Components.at(j)); otherCollider->m_Tag)
					{
					case Component::ColliderComponent::TAG_INTERACTABLE:
						if (collider->m_Tag == Component::ColliderComponent::TAG_PLAYER)
						{
							ECS_WORLD.FlagEntityForDeletion(j);
							auto& player = ECS_WORLD.GetComponent<Component::PlayerComponent>(iso->Entity());
							++player.m_Score;
						}
						break;
					case Component::ColliderComponent::TAG_TILE:
					{
						auto& otherHeight = std::get<Component::IsometricTransformComponent*>(collisionSystem.m_Components.at(j));
						auto& otherSprite = ECS_WORLD.GetComponent<Component::SpriteComponent>(j);
						if (collider->m_Tag == Component::ColliderComponent::TAG_ARROW)
						{
							movement.m_OnGround = true;
							otherSprite.m_Red = 1;
							otherSprite.m_Blue = 0;
							otherSprite.m_Green = 0;
							break;
						}
						if (iso->m_MinHeight == otherHeight->m_Height)
						{
							if(collider->m_Tag == Component::ColliderComponent::TAG_PLAYER)
							{
								const auto& tile = ECS_WORLD.GetComponent<Component::TileComponent>(otherSprite.Entity());
								if(tile.m_Tag == Component::TileComponent::TAG_WINTILE)
								{
									auto& player = ECS_WORLD.GetComponent<Component::PlayerComponent>(collider->Entity());
									++player.m_Score;
								}
							}
							movement.m_OriginalHeight = iso->m_Height;
							movement.m_OnGround = true;
							otherSprite.m_Red = 0;
							otherSprite.m_Blue = 0;
							otherSprite.m_Green = 1;
							break;
						}
						if (!movement.m_OnGround && !movement.m_Jump && iso->m_MinHeight < otherHeight->m_Height && iso->m_MinHeight > otherHeight->m_MinHeight)
						{
							if (iso->m_Height - movement.m_OriginalHeight > 60 && collider->m_Tag == Component::ColliderComponent::TAG_PLAYER)
							{
								iso->m_MinHeight = movement.m_OriginalHeight - 30.f;
								/*movement.m_FallingSpeed += static_cast<float>(AEFrameRateControllerGetFrameTime()) * movement.m_Gravity;
								iso->m_Height -= movement.m_FallingSpeed;
								iso->m_MinHeight -= movement.m_FallingSpeed;*/
								iso->m_Position = movement.m_PrevPosition;
							}
							else
							{
								movement.m_OriginalHeight = iso->m_Height;
								iso->m_MinHeight = otherHeight->m_Height;
							}
							iso->m_Height = iso->m_MinHeight + 30.0f;
							movement.m_FallingSpeed = 0.0f;
							movement.m_OnGround = true;
							if (collider->m_Tag == Component::ColliderComponent::TAG_FALLING_TILE)
							{
								auto& layer = ECS_WORLD.GetComponent<Component::SpriteComponent>(key);
								layer.m_Layer = Component::SpriteComponent::STATIC;
								collider->m_Tag = Component::ColliderComponent::TAG_TILE;
							}
						}
						iso->m_Position = movement.m_PrevPosition;
						break;
					}
					case Component::ColliderComponent::TAG_BORDER:
					{
						if (collider->m_Tag == Component::ColliderComponent::TAG_SLIDING_TILE)
                            movement.m_Speed += 2.f;
                        else if(collider->m_Tag == Component::ColliderComponent::TAG_ARROW)
                            movement.m_Speed = 8.0f;
                        else
                            iso->m_Position = movement.m_PrevPosition;
                        break;
					}
						
					case Component::ColliderComponent::TAG_PLAYER:
					case Component::ColliderComponent::TAG_FALLING_TILE:
						break;
					case Component::ColliderComponent::TAG_SLIDING_TILE:
						if (collider->m_Tag == Component::ColliderComponent::TAG_PLAYER)
						{
							ECS_WORLD.FlagEntityForDeletion(j);
							auto& player = ECS_WORLD.GetComponent<Component::PlayerComponent>(iso->Entity());
							++player.m_Hit;
						}
						break;
					case Component::ColliderComponent::TAG_DESTROYER_TILE:
						if (collider->m_Tag == Component::ColliderComponent::TAG_SLIDING_TILE ||
							collider->m_Tag == Component::ColliderComponent::TAG_FALLING_TILE)
							ECS_WORLD.FlagEntityForDeletion(collider->Entity());
						break;
					case Component::ColliderComponent::TAG_ARROW: 
						break;
					case Component::ColliderComponent::TAG_PORTAL:
						if (collider->m_Tag == Component::ColliderComponent::TAG_PLAYER)
						{
							auto& portal = ECS_WORLD.GetComponent<Component::PortalComponent>(j);
							portal.collide = true;
						}
						break;
					case Component::ColliderComponent::NUM_TAG: 
						break;
					default: ;
					}

				}
			}
			for (const auto& key : isoCollisions | std::views::keys)
			{
				auto& collider = std::get<Component::ColliderComponent*>(collisionSystem.m_Components.at(key));
				auto& iso = std::get<Component::IsometricTransformComponent*>(collisionSystem.m_Components.at(key));
				for (const auto& j : isoCollisions.at(key))
				{
					switch (auto& otherCollider = std::get<Component::ColliderComponent*>(collisionSystem.m_Components.at(j)); otherCollider->m_Tag)
					{
					case Component::ColliderComponent::TAG_TILE:
					{
						if (collider->m_Tag != Component::ColliderComponent::TAG_PLAYER)
							break;
						auto& otherSprite = ECS_WORLD.GetComponent<Component::SpriteComponent>(j);
						auto& isoOther = std::get<Component::IsometricTransformComponent*>(collisionSystem.m_Components.at(j));
						if (iso->m_Height <= isoOther->m_Height && isoOther->m_IsometricPosition.y < iso->m_IsometricPosition.y + (iso->m_Height - iso->m_MinHeight) + 15) // 1/4 of size.y 
							otherSprite.m_Alpha = 0.3f;
						if (iso->m_Height <= isoOther->m_Height && isoOther->m_Position.x - 15 > iso->m_Position.x || isoOther->m_Position.y - 15 > iso->m_Position.y)
							otherSprite.m_Alpha = 1;
						break;
					}
					case Component::ColliderComponent::TAG_PLAYER:
					case Component::ColliderComponent::TAG_BORDER:
					case Component::ColliderComponent::TAG_INTERACTABLE:
					case Component::ColliderComponent::TAG_FALLING_TILE:
					case Component::ColliderComponent::NUM_TAG:
						break;
					default: ;
					}
				}
			}
			isoCollisions.clear();
			collisions.clear();
		};

		collisionSystem.m_PostUpdateFunction = std::move(postUpdateFunc);
		collisionSystem.m_UpdateFunction = std::move(updateFunc);
		return collisionSystem;
	}

	/**
	 * \brief			Generates a spawner system for the given scene.
	 * \tparam Scene	The scene to generate the spawner system for.
	 * \return			The generated spawner system.
	 */
	template<int Scene>
	SpawnerSystem& GenerateSpawnerSystem()
	{
		static auto updateFunc = [](const std::tuple<Component::SpawnerComponent*>& _components)
		{
			auto& spawner = std::get<Component::SpawnerComponent*>(_components);
			spawner->m_CurrentTime += GetDeltaTime();
			if (spawner->m_CurrentTime >= spawner->m_Timer &&
				spawner->m_CurrSpawnCount < spawner->m_SpawnCount) 
			{
				for (unsigned i = 0; i < spawner->m_SpawnPerInterval; ++i)
				{
					spawner->m_GenerateEntity(
						{
							static_cast<float>(RandomInt(static_cast<int>(spawner->m_MinRange.x),
							                             static_cast<int>(spawner->m_MaxRange.x)) * 30.f),
							static_cast<float>(RandomInt(static_cast<int>(spawner->m_MinRange.y),
							                             static_cast<int>(spawner->m_MaxRange.y)) * 30.f)
						}, spawner->m_Direction);
					++spawner->m_SpawnCount;
				}
				spawner->m_CurrentTime = 0;
			}
		};
		static SpawnerSystem spawnerSystem{ updateFunc };
		return spawnerSystem;
	}

	/**
	 * \brief			Generates a tile system for the given scene.
	 * \tparam Scene	The scene to generate the tile system for.
	 * \return			The generated tile system.
	 */
	template<int Scene>
	TileSystem& GenerateTileSystem()
	{
		static auto updateFunc = [](const std::tuple<Component::TileComponent*, Component::SpriteComponent*>& _components)
		{
			const auto& tile = std::get<Component::TileComponent*>(_components);
			auto& sprite = std::get<Component::SpriteComponent*>(_components);
			sprite->m_Red = tile->m_Red;
			sprite->m_Green = tile->m_Green;
			sprite->m_Blue = tile->m_Blue;
			sprite->m_Alpha = tile->m_Alpha;
			tile->m_Timer -= GetDeltaTime();
			if (tile->m_Tag == Component::TileComponent::TAG_FALLINGGAME)
			{
				auto& iso = ECS_WORLD.GetComponent<Component::IsometricTransformComponent>(tile->Entity());
				if (tile->m_Timer < 0.7f)
				{
					iso.m_Tag = Component::IsometricTransformComponent::TAG_FALLING;
				}
				if (tile->m_Timer < 0.0f)
				{
					iso.m_IsStatic = false;
					iso.m_Tag = Component::IsometricTransformComponent::TAG_TILE;
					auto& movement = ECS_WORLD.AddComponent<Component::MovementComponent>(tile->Entity());
					auto& collider = ECS_WORLD.GetComponent<Component::ColliderComponent>(tile->Entity());
					collider.m_Tag = Component::ColliderComponent::TAG_FALLING_TILE;
					collider.m_Size = { 15,15 };
					movement.m_Jump = false;
					movement.m_OnGround = false;
					movement.m_MaxHeight = 60;
					movement.m_OriginalHeight = 0.0f;
					movement.m_JumpSpeed = 400.0f;
					movement.m_Gravity = 150.0f;
					movement.m_FallingSpeed = 3.0f;
					movement.m_Speed = 8.0f;
					movement.m_Direction = { 0.f, 0.f };
					ECS_WORLD.RegisterEntityIntoSystems(tile->Entity());
				}
			}
		};
		static TileSystem tileSystem{ updateFunc };
		return tileSystem;
	}

	/**
	 * \brief			Generates the player camera system.
	 * \tparam Scene	The scene.
	 * \return			The player camera system.
	 */
	template<int Scene>
	PlayerCameraSystem& GeneratePlayerCameraSystem()
	{
		static AEVec2 initialPosition{ 0.f,0.f };
		static bool positionInitialized = false;
		static auto updateFunc =
			[](const std::tuple<Component::PlayerComponent*, Component::IsometricTransformComponent*>& _components)
		{
			const auto& iso = std::get<Component::IsometricTransformComponent*>(_components);
			const auto& player = std::get<Component::PlayerComponent*>(_components);
			if (!positionInitialized)
				initialPosition = { iso->m_Transform.m[0][2], iso->m_Transform.m[1][2] };
			if(player->m_FollowCamera)
				AEGfxSetCamPosition(iso->m_Transform.m[0][2], iso->m_Transform.m[1][2]);
			else
				AEGfxSetCamPosition(initialPosition.x, initialPosition.y);
		};
		static PlayerCameraSystem playerCameraSystem{ updateFunc };
		playerCameraSystem.m_PostUpdateFunction = nullptr;
		return playerCameraSystem;
	}

	/**
	 * \brief			Generates the Minigame System.
	 * \tparam Scene	The scene.
	 * \return			The Minigame System.
	 */
	template<int Scene>
	MinigameSystem& GenerateMinigameSystem()
	{
		static auto updateFunc =
			[](const std::tuple<Component::MinigameComponent*>& _components)
		{
			auto& minigame = std::get<Component::MinigameComponent*>(_components);
			if (minigame->m_MinigameComplete)
				return;
			minigame->m_CurrTime += GetDeltaTime();
			bool win = (*DI_GET_SCORE() >= minigame->m_ScoreTarget);
			bool lose = (*DI_GET_HIT() >= minigame->m_HitTarget);
			if(minigame->m_CurrTime >= minigame->m_Timer)
			{ 
				switch(minigame->m_Tag)
				{
				case Component::MinigameComponent::Tag::TAG_TIMERWIN: 
					win = true;
					break;
				case Component::MinigameComponent::Tag::TAG_TIMERLOSE: 
					lose = true;
					break;
				default: 
					break;
				}
			}
			
			if(win)
			{
				AUDIO_MANAGER.PlayAudio(Audio::AudioManager::GAMECOMPLETE_SOUND,1);
				if (*DI_GET_PET() == Component::PlayerComponent::NUM_PET && *DI_GET_CURRENT_PET() != Component::PlayerComponent::NUM_PET)
				{
					(*DI_GET_CURRENT_PET()) = static_cast<Component::PlayerComponent::Pet_Type>(*DI_GET_CURRENT_PET() + 1);
				}
				else if (*DI_GET_CURRENT_PET() != Component::PlayerComponent::NUM_PET)
				{
					(*DI_GET_CURRENT_PET()) = static_cast<Component::PlayerComponent::Pet_Type>(*DI_GET_CURRENT_PET() + 1);
					(*DI_GET_PET()) = static_cast<Component::PlayerComponent::Pet_Type>(*DI_GET_PET() + 1);
				}
				JSONSerializer::SaveData(*DI_GET_CURRENT_PET());
				minigame->m_MinigameComplete = true;
				Factory::ECS_Factory::CreateWinTextEntity();
			}
			else if (lose)
			{
				minigame->m_MinigameComplete = true;
				Factory::ECS_Factory::CreateWinTextEntity(false);
			}

			if (minigame->m_MinigameComplete)
				*DI_GET_PAUSE() = true;
		};
		static MinigameSystem minigameSystem{ updateFunc };
		return minigameSystem;
	}

	/**
	 * \brief			Generates the Pet system.
	 * \tparam Scene	The scene.
	 * \return			The Pet system.
	 */
	template<int Scene>
	PetSystem& GeneratePetSystem()
	{
		static auto updateFunc = [](const std::tuple<Component::PetComponent*>& _components)
		{
			auto& pet = std::get<Component::PetComponent*>(_components);
			if (ECS_WORLD.GetCurrentScene() == SCENE_WILDLANDS)
			{
				if (*DI_GET_SCENE_PET() != *DI_GET_PET())
				{
					ECS_WORLD.FlagEntityForDeletion(pet->Entity());
					*DI_GET_SCENE_PET() = *DI_GET_PET();
					ECS::Factory::ECS_Factory::CreatePetEntity(*DI_GET_PET());
				}
			}
			pet->Update();
		};
		static PetSystem petSystem{ updateFunc };
		return petSystem;
	}

	/**
	 * \brief			Generates the Arrow System.
	 * \tparam Scene	The scene.
	 * \return			The Arrow system.
	 */
	template<int Scene>
	ArrowSystem& GenerateArrowSystem()
	{
		static auto updateFunc = [](const std::tuple<Component::ArrowComponent*, Component::IsometricTransformComponent* >& _components)
		{
			auto& arrow = std::get<Component::ArrowComponent*>(_components);
			auto& transform = std::get<Component::IsometricTransformComponent*>(_components);
			transform->m_Position.x = AEWrap(transform->m_Position.x, arrow->m_StartPosition.x, arrow->m_EndPosition.x);
			transform->m_Position.y = AEWrap(transform->m_Position.y, arrow->m_StartPosition.y, arrow->m_EndPosition.y);
			arrow->m_Lifetime -= GetDeltaTime();
			if(arrow->m_Lifetime < 0)
				ECS_WORLD.FlagEntityForDeletion(transform->Entity());
		};
		static ArrowSystem arrowSystem{ updateFunc };
		return arrowSystem;
	}

	/**
	 * \brief			Generates the Text Rendering system.
	 * \tparam Scene	The scene.
	 * \return			The Text Rendering system.
	 */
	template<int Scene>
	TextRenderingSystem& GenerateTextRenderingSystem()
	{
		static auto preUpdateFunc = 
			[]()
		{
			AEGfxSetRenderMode(AE_GFX_RM_COLOR);
			AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		};

		static auto updateFunc = 
			[](const std::tuple<Component::TextRenderingComponent*>& _components)
		{
			auto& text = std::get<Component::TextRenderingComponent*>(_components);
			for (const auto& str : text->m_Text)
			{
				const float normalizedX = (str.m_ScreenPosition.x - str.m_Text.length() / 2.0f * 30.0f / 2) / AEGetWindowWidth() * 2.0f;
				const float normalizedY = (str.m_ScreenPosition.y / AEGetWindowHeight() * 2.0f);
				AEGfxPrint(*FONT_LIBRARY.GetResource("a"), const_cast<s8*>(str.m_Text.c_str()), normalizedX,
				           normalizedY, str.m_Scale, 1, 1, 1);
			}
		};

		static TextRenderingSystem textRenderingSystem {  updateFunc, nullptr, preUpdateFunc };
		return textRenderingSystem;
	}

	/**
	 * \brief			Generates the animation logic system.
	 * \tparam Scene	The scene.
	 * \return			The animation logic system.
	 */
	template<int Scene>
	AnimationLogicSystem& GenerateAnimationLogicSystem()
	{
		static auto updateFunc =
			[](const std::tuple<Component::AnimationComponent*, Component::MovementComponent*, Component::AnimationKeyComponent*>& _components)
		{
			auto& movement = std::get<Component::MovementComponent*>(_components);
			auto& animation = std::get<Component::AnimationComponent*>(_components);
			auto& key = std::get<Component::AnimationKeyComponent*>(_components);
			if(movement->m_Direction.x == 0.f && movement->m_Direction.y == 0.f)
			{
				animation->m_CurrTime = 0;
				return;
			}
			if (movement->m_Direction.x < 0.f && movement->m_Direction.y < 0.f)
			{
				if (key->m_LastDir == key->DIR_DOWN) return;
				key->m_LastDir = key->DIR_DOWN;
			}
			else if (movement->m_Direction.x > 0.f && movement->m_Direction.y > 0.f)
			{
				if (key->m_LastDir == key->DIR_UP) return;
				key->m_LastDir = key->DIR_UP;
			}
			else if (movement->m_Direction.x <= 0.f && movement->m_Direction.y >= 0.f)
			{
				if (key->m_LastDir == key->DIR_LEFT) return;
				key->m_LastDir = key->DIR_LEFT;
			}
			else
			{
				if (key->m_LastDir == key->DIR_RIGHT) return;
				key->m_LastDir = key->DIR_RIGHT;
			}
			animation->m_Animations = ANIMATION_LIBRARY.GetResource(key->m_Key[key->m_LastDir]);
			animation->m_CurrTime += animation->m_AnimTime;
		};
		static AnimationLogicSystem animationLogicSystem{ updateFunc };
		return animationLogicSystem;
	}

	/**
	 * \brief			Generates the UI logic system.
	 * \tparam Scene	The scene.
	 * \return			The UI logic system.
	 */
	template<int Scene>
	UILogicSystem& GenerateUILogicSystem()
	{
		static s32 mousePosX, mousePosY;
		static s32 xModifier = AEGetWindowWidth() / 2;
		static s32 yModifier = AEGetWindowHeight() / 2;
		static bool hidePauseButtons = false;
		static auto preUpdateFunc = []()
		{
			AEInputGetCursorPosition(&mousePosX, &mousePosY);
			mousePosX -= xModifier;
			mousePosY -= yModifier;
			mousePosY = -mousePosY;
			if (AEInputCheckTriggered(AEVK_ESCAPE))
			{
				*DI_GET_PAUSE() = !*DI_GET_PAUSE();
				hidePauseButtons = *DI_GET_PAUSE();
			}
		};

		static auto updateFunc = [](const std::tuple<Component::UILogicComponent*, Component::RectTransformComponent*>& _components)
		{	
			auto& ui = std::get<Component::UILogicComponent*>(_components);
			auto& transform = std::get<Component::RectTransformComponent*>(_components);
			if(AEInputCheckTriggered(AEVK_SPACE))
			{
				switch (ui->m_Tag)
				{
				case Component::UILogicComponent::TAG_MINIGAME_INSTRUCTION:
					ECS_WORLD.FlagEntityForDeletion(ui->Entity());
					*DI_GET_PAUSE() = false;
					return;
				case Component::UILogicComponent::TAG_MINIGAME_ENDMINIGAME:
					*DI_GET_PAUSE() = false;
					ECS_WORLD.FlagUnloadScene();
					return;
				case Component::UILogicComponent::TAG_TUTORIAL_CONTROLS:
					ECS_WORLD.FlagEntityForDeletion(ui->Entity());
					return;
				default:
					break;
				}
			}
			switch (ui->m_Tag)
			{
			case Component::UILogicComponent::TAG_GAME_PAUSE:
			case Component::UILogicComponent::TAG_PAUSEMENU_CONTINUE:
			case Component::UILogicComponent::TAG_PAUSEMENU_RETURNTOMENU:
			case Component::UILogicComponent::TAG_PAUSEMENU_CONTROLS:
			case Component::UILogicComponent::TAG_PAUSEMENU_EXIT:
				transform->m_Render = hidePauseButtons;
				break;
			default:
				break;
			}
	
			if ((mousePosX >= transform->m_MinPosition.x && mousePosY >= transform->m_MinPosition.y) &&
				(mousePosX <= transform->m_MaxPosition.x && mousePosY <= transform->m_MaxPosition.y) && 
				transform->m_Render)
			{
				if (!ui->m_IsHovering && ui->m_Tag != Component::UILogicComponent::TAG_GAME_PAUSE)
				{
					AUDIO_MANAGER.PlayAudio(Audio::AudioManager::Sound::UIBUTTON_SOUND, 0);
					transform->m_Scale = transform->m_Scale * 1.05f;
					ui->m_IsHovering = true;
				}
				else if (AEInputCheckTriggered(AEVK_LBUTTON))
				{
					switch (ui->m_Tag)
					{
					case Component::UILogicComponent::TAG_MAINMENU_PLAY:
						ECS_WORLD.LoadScene(SCENE_OVERWORLD);
						break;
					case Component::UILogicComponent::TAG_MAINMENU_OPTIONS:
						ECS_WORLD.LoadScene(SCENE_OPTIONS);
						break;
					case Component::UILogicComponent::TAG_MAINMENU_TUTORIAL:
						ECS_WORLD.LoadScene(SCENE_TUTORIAL);
						break;
					case Component::UILogicComponent::TAG_MAINMENU_CREDITS:
						ECS_WORLD.LoadScene(SCENE_CREDIT1);
						break;
					case Component::UILogicComponent::TAG_CREDITS1_NEXT:
						ECS_WORLD.LoadScene(SCENE_CREDIT2);
						break;
					case Component::UILogicComponent::TAG_CREDITS2_NEXT:
						ECS_WORLD.LoadScene(SCENE_CREDIT3);
						break;
					case Component::UILogicComponent::TAG_CREDITS3_NEXT:
						ECS_WORLD.LoadScene(SCENE_CREDIT4);
						break;
					case Component::UILogicComponent::TAG_CREDITS4_NEXT:
						ECS_WORLD.FlagUnloadSceneUntil(SCENE_CREDIT1);
						break;
					case Component::UILogicComponent::TAG_PAUSEMENU_EXIT:
					case Component::UILogicComponent::TAG_MAINMENU_EXIT:
						*DI_GET_EXIT() = 0;
						break;
					case Component::UILogicComponent::TAG_PAUSEMENU_RETURNTOMENU:
						hidePauseButtons = false;
					case Component::UILogicComponent::TAG_CREDITS_RETURNTOMAINMENU:
						ECS_WORLD.FlagUnloadSceneUntil(SCENE_MAINMENU);
						break;
					case Component::UILogicComponent::TAG_PAUSEMENU_CONTINUE:
						*DI_GET_PAUSE() = false;
						hidePauseButtons = false;
						break;
					case Component::UILogicComponent::TAG_PAUSEMENU_CONTROLS:
						ECS::Factory::ECS_Factory::CreateControlsEntity({ -20,-75 });
						break;
					case Component::UILogicComponent::TAG_OPTIONS_FULLSCREEN:
						AESysToggleFullScreen(true);
						break;
					case Component::UILogicComponent::TAG_OPTIONS_MINIMIZED:
						AESysToggleFullScreen(false);
						break;
					case Component::UILogicComponent::TAG_OPTIONS_RETURNTOMENU:
						ECS_WORLD.FlagUnloadSceneUntil(SCENE_MAINMENU);
						break;
					case Component::UILogicComponent::TAG_OPTIONS_CLEARDATA:
						JSONSerializer::SaveData(Component::PlayerComponent::PET_NIL);
						break;
					default:
						break;
					}
				}
			}
			else if (ui->m_IsHovering)
			{
				transform->m_Scale.x = transform->m_Scale.x / 1.05f;
				transform->m_Scale.y = transform->m_Scale.y / 1.05f;
				ui->m_IsHovering = false;
			}
			
		};

		static UILogicSystem uiLogicSystem{ updateFunc, nullptr, preUpdateFunc };
		return uiLogicSystem;
	}

	/**
	 * \brief			Generates the Rect Transform system.
	 * \tparam Scene	The scene.
	 * \return			The Rect Transform system.
	 */
	template<int Scene>
	RectTransformSystem& GenerateRectTransformSystem()
	{
		static f32 camX{}, camY{};

		static auto updateFunc = [](const std::tuple<Component::RectTransformComponent*>& _components)
		{
			auto& transform = std::get<Component::RectTransformComponent*>(_components);

			AEMtx33 scale, rot, trans;
			AEGfxGetCamPosition(&camX, &camY);

			transform->m_Height = 60 * transform->m_Scale.y;
			transform->m_Width = 60 * transform->m_Scale.x;

			transform->m_MinPosition = { transform->m_Position.x - transform->m_Width / 2 ,transform->m_Position.y - transform->m_Height / 2 };
			transform->m_MaxPosition = { transform->m_Position.x + transform->m_Width / 2 , transform->m_Position.y + transform->m_Height / 2 };

			AEMtx33Scale(&scale, transform->m_Scale.x, transform->m_Scale.y);
			AEMtx33Rot(&rot, transform->m_Angle);
			AEMtx33Trans(&trans, transform->m_Position.x + camX, transform->m_Position.y + camY);
			AEMtx33Concat(&transform->m_Transform, &scale, &rot);
			AEMtx33Concat(&transform->m_Transform, &trans, &transform->m_Transform);
		};

		static RectTransformSystem rectTransformSystem{ updateFunc };
		return rectTransformSystem; 
	}

	/**
	 * \brief			Generates the UI Rendering system.
	 * \tparam Scene	The scene.
	 * \return			The UI Rendering system.
	 */
	template<int Scene>
	UIRenderingSystem& GenerateRectTransformRenderingSystem()
	{
		static Mesh mesh;
		static auto UpdateFunc = [](const std::tuple< Component::RectTransformComponent*, Component::SpriteComponent*>& _components)
		{
			auto& transform = std::get<Component::RectTransformComponent*>(_components);
			auto& sprite = std::get<Component::SpriteComponent*>(_components);

			if (transform->m_Render)
			{
				auto texture = TEXTURE_LIBRARY.GetResource(sprite->m_TextureKey);
				if (!texture)
				{
					TEXTURE_LIBRARY.AddResource(sprite->m_TextureKey, AEGfxTextureLoad(sprite->m_TextureKey.c_str()));
					texture = TEXTURE_LIBRARY.GetResource(sprite->m_TextureKey);
				}
				mesh.RenderMesh(transform->m_Transform, texture, sprite->m_Red, sprite->m_Green, sprite->m_Blue, sprite->m_Alpha);
			}

		};
		static UIRenderingSystem rectTransformRenderingSystem{ UpdateFunc };
		return rectTransformRenderingSystem;
	}

	/**
	 * \brief			Generates the Portal system.
	 * \tparam Scene	The scene.
	 * \return			The Portal system.
	 */
	template<int Scene>
	PortalResponseSystem& GeneratePortalSystem()
	{
		static auto updateFunc =
			[](const std::tuple<Component::PortalComponent*>& _components)
		{
			auto& portal = std::get<Component::PortalComponent*>(_components);
			auto& emitter = ECS_WORLD.AddComponent<Component::EmitterComponent>(portal->Entity());
			if(portal->pet == *DI_GET_PET() || *DI_GET_CURRENT_PET() >= Component::PlayerComponent::NUM_PET || portal->pet == Component::PlayerComponent::PET_NIL)
			{
				emitter.m_Emit = true;
				if (portal->collide)
				{
					portal->collide = false;
					if (AEInputCheckTriggered(AEVK_E))
					{
						if (portal->scene <= ECS_WORLD.GetCurrentScene())
							ECS_WORLD.FlagUnloadScene();
						else
							ECS_WORLD.LoadScene(static_cast<Backend::Scene>(portal->scene));
					}
				}
			}
			else 
				emitter.m_Emit = false;
		};
		static PortalResponseSystem portalResponseSystem{ updateFunc };
		return portalResponseSystem;
	}

	/**
	 * \brief			Generate the Movement Based Emitter system.
	 * \tparam Scene	The scene.
	 * \return			The Movement Based Emitter system.
	 */
	template<int Scene>
	MovementBasedEmitterSystem& GenerateMovementBasedEmitterSystem()
	{
		static auto updateFunc =
			[](const std::tuple<Component::EmitterComponent*, Component::MovementComponent*>& _components)
		{
			auto& emit = std::get<Component::EmitterComponent*>(_components);
			auto& movement = std::get<Component::MovementComponent*>(_components);

			switch (emit->m_Type)
			{
			case Component::EmitterComponent::MOVE:
				if ((movement->m_Direction.x != 0.f || movement->m_Direction.y != 0.f) && movement->m_OnGround)
					emit->m_Emit = true;
				else 
					emit->m_Emit = false;
				break;
			case Component::EmitterComponent::POOF:
				if (movement->m_OnGround)
					emit->m_Emit = true;
				else 
					emit->m_Emit = false;
				break;
			}
		};
		static MovementBasedEmitterSystem movementBasedEmitterSystem { updateFunc };
		return movementBasedEmitterSystem;
	}

	/**
	 * \brief			Generate the Emitter system.
	 * \tparam Scene	The scene.
	 * \return			The Emitter system.
	 */
	template<int Scene>
	EmitterSystem& GenerateEmitterSystem()
	{
		static auto updateFunc =
			[](const std::tuple<Component::EmitterComponent*, Component::IsometricTransformComponent*>& _components)
		{
			auto& emit = std::get<Component::EmitterComponent*>(_components);
			auto& iso = std::get<Component::IsometricTransformComponent*>(_components);
			if (!emit->m_Emit || emit->m_Cooldown > 0.f)
			{
				emit->m_Cooldown -= GetDeltaTime();
				return;
			}
			AEVec2 randPos;
			switch (emit->m_Type)
			{
			case Component::EmitterComponent::MOVE:
				for (int i = 0; i < 2; ++i)
				{
					randPos = { iso->m_Position.x + RandomRange(-10.f, 10.f), iso->m_Position.y + RandomRange(-15.f, 15.f) };
					Factory::ECS_Factory::CreateParticleEntity("Assets/sprites/dirt.png", randPos, { 0.f, 0.f }, iso->m_MinHeight,
						10.f, 0.1f, 0.15f, 0.15f, 1.f, 1.f, true);
				}
				emit->m_Cooldown = 0.5f;
				break;
			case Component::EmitterComponent::FALL:
				for (int i = 0; i < 3; ++i)
				{
					randPos = { iso->m_Position.x + RandomRange(-10.f, 10.f), iso->m_Position.y + RandomRange(-15.f, 15.f) };
					Factory::ECS_Factory::CreateParticleEntity("Assets/sprites/dirt.png", randPos, { 0.f, 0.f }, iso->m_Height,
						0.f, 5.f, 0.2f, 0.2f, 1.f, 1.f, true);
				}
				emit->m_Cooldown = 4.f;
				break;
			case Component::EmitterComponent::PORTAL:
				Factory::ECS_Factory::CreateParticleEntity("Assets/tiles/portal.png", iso->m_Position, { 0.f, 0.f }, iso->m_Height,
					100.f, 0.1f, 1.f, 1.f, .4f, 0.2f, true);
				emit->m_Cooldown = 0.1f;
				break;
			case Component::EmitterComponent::POOF:
				for (int i = 0; i < 8; ++i)
				{
					const float dir = 2 * PI / 8 * i;
					Factory::ECS_Factory::CreateParticleEntity("Assets/sprites/poof.png", iso->m_Position, { cosf(dir), sinf(dir) }, iso->m_Height,
						0.f, 1.f, .4f, .8f, .6f, 0.0f, false);
				}
				emit->m_Cooldown = 2.1f;
				break;
			case Component::EmitterComponent::CLOUD:
			{
				for (int i = 0; i < 2; ++i)
				{
					const float randSize = RandomRange(2.f, 7.f);
					Factory::ECS_Factory::CreateParticleEntity("Assets/sprites/cloud.png", { iso->m_Position.x, RandomRange(-1000.f, 2500.f) },
						{ 1.f, 0.f }, iso->m_Height, 0.f, 300.f, randSize, randSize, 1.f, 1.f, false);
				}
				emit->m_Cooldown = RandomRange(2.f, 5.f);
				break;
			}
			case Component::EmitterComponent::NUM_TYPE: break;
			}
		};
		static EmitterSystem emitterSystem{ updateFunc };
		return emitterSystem;
	}

	/**
	 * \brief			Generates the Particle system.
	 * \tparam Scene	The scene
	 * \return			The Particle system.
	 */
	template<int Scene>
	ParticleSystem& GenerateParticleSystem()
	{
		static auto updateFunc =
			[](const std::tuple<Component::ParticleComponent*, Component::IsometricTransformComponent*, Component::SpriteComponent*>& _components)
		{
			auto& particle = std::get<Component::ParticleComponent*>(_components);
			auto& iso = std::get<Component::IsometricTransformComponent*>(_components);
			auto& sprite = std::get<Component::SpriteComponent*>(_components);

			if (particle->m_LifeRemain <= 0.f)
			{
				ECS_WORLD.FlagEntityForDeletion(particle->Entity());
				return;
			}
			particle->m_LifeRemain -= GetDeltaTime();
			sprite->m_Alpha = particle->m_AlphaBegin - (particle->m_AlphaBegin - particle->m_AlphaEnd) * (particle->m_Lifetime - particle->m_LifeRemain) / particle->m_Lifetime;
			iso->m_Scale = { particle->m_SizeBegin - (particle->m_SizeBegin - particle->m_SizeEnd) * (particle->m_Lifetime - particle->m_LifeRemain) / particle->m_Lifetime,
				particle->m_SizeBegin - (particle->m_SizeBegin - particle->m_SizeEnd) * (particle->m_Lifetime - particle->m_LifeRemain) / particle->m_Lifetime };
		};
		static ParticleSystem particleSystem{ updateFunc };
		return particleSystem;
	}

	/**
	 * \brief			Generates the Cheese system.
	 * \tparam Scene	The scene.
	 * \return			The Cheese system.
	 */
	template<int Scene>
	CheeseSystem& GenerateCheeseSystem()
	{
		static auto updateFunc =
			[](const std::tuple<Component::CheeseComponent*, Component::SpriteComponent*>& _components)
		{
			auto& cheese = std::get<Component::CheeseComponent*>(_components);
			auto& sprite = std::get<Component::SpriteComponent*>(_components);
			if(cheese->m_Lifetime < 0.f)
				ECS_WORLD.FlagEntityForDeletion(cheese->Entity());
			cheese->m_Lifetime -= GetDeltaTime();
			sprite->m_Alpha = cheese->m_Lifetime / cheese->m_OriginalTime;
		};
		static CheeseSystem cheeseSystem{ updateFunc };
		return cheeseSystem;
	}

	/**
	 * \brief			Generates the Splash Screen Cheese system.
	 * \tparam Scene	The scene.
	 * \return			The Splash Screen Cheese system.
	 */
	template<int Scene>
	CheeseSystem& GenerateSplashScreenCheeseSystem()
	{
		static bool skip = false;
		static int no = 0;
		static auto preUpdateFunc =
			[]()
		{
			if (AEInputCheckTriggered(AEVK_LBUTTON))
				skip = true;
		};
		static auto updateFunc =
			[](const std::tuple<Component::CheeseComponent*, Component::SpriteComponent*>& _components)
		{
			auto& cheese = std::get<Component::CheeseComponent*>(_components);
			auto& sprite = std::get<Component::SpriteComponent*>(_components);

			cheese->m_Lifetime -= GetDeltaTime();
			if(cheese->m_Lifetime <= 1.f)
				sprite->m_Alpha = cheese->m_Lifetime / (cheese->m_OriginalTime - 2.f);

			if (skip)
			{
				skip = false;
				cheese->m_Lifetime = 0.f;
			}
			if (cheese->m_Lifetime > 0.f)
				return;

			if (cheese->m_Lifetime <= 0.f)
			{
				if (no == 1)
					ECS_WORLD.LoadScene(SCENE_MAINMENU);
				if (no < 2)
					ECS_WORLD.FlagEntityForDeletion(cheese->Entity());
			}
			if(++no < 2)
			{
				Factory::ECS_Factory::CreateSplashScreenEntity(no);
				return;
			}
		};
		static CheeseSystem cheeseSystem{ updateFunc, nullptr, preUpdateFunc};
		return cheeseSystem;
	}
}

#endif