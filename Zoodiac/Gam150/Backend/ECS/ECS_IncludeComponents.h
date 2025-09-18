// ======================================================================
// File Name:		ECS_IncludeComponents.cpp
// Project Name:	Zoodiac
// Author(s):		Low Zhi Sheng Kitson(20%), Lim Geng Yang(20%),
//					Lye Pin Liang Xavier(20%), Muhammad Dzulhafiz(20%),
//					Ang Jie Le Jet(20%)
// Brief:			Update whenever a new component is added
// 
// All content © 2023 DigiPen Institute of Technology Singapore. All 
// rights reserved.
// ======================================================================

#ifndef ECS_INCLUDE_COMPONENTS_H_
#define ECS_INCLUDE_COMPONENTS_H_

#include <Backend/ECS/Components/AnimationComponent.h>
#include <Backend/ECS/Components/ColliderComponent.h>
#include <Backend/ECS/Components/IsometricTransformComponent.h>
#include <Backend/ECS/Components/MovementComponent.h>
#include <Backend/ECS/Components/MinigameComponent.h>
#include <Backend/ECS/Components/PetComponent.h>
#include <Backend/ECS/Components/PlayerComponent.h>
#include <Backend/ECS/Components/SpawnerComponent.h>
#include <Backend/ECS/Components/SpriteComponent.h>
#include <Backend/ECS/Components/TileComponent.h>
#include <Backend/ECS/Components/TransformComponent.h>
#include <Backend/ECS/Components/PrefabComponent.h>
#include <Backend/ECS/Components/ArrowComponent.h>
#include <Backend/ECS/Components/TextRenderingComponent.h>
#include <Backend/ECS/Components/UILogicComponent.h>
#include <Backend/ECS/Components/RectTransformComponent.h>
#include <Backend/ECS/Components/AnimationKeyComponent.h>
#include <Backend/ECS/Components/PortalComponent.h>
#include <Backend/ECS/Components/ParticleComponent.h>
#include <Backend/ECS/Components/EmitterComponent.h>
#include <Backend/ECS/Components/CheeseComponent.h>
#include <tuple>

#define ECS_ALL_COMPONENT_POINTERS	Backend::ECS::Component::AnimationComponent*,			\
									Backend::ECS::Component::ColliderComponent*,			\
									Backend::ECS::Component::IsometricTransformComponent*,	\
									Backend::ECS::Component::MovementComponent*,			\
									Backend::ECS::Component::MinigameComponent*,			\
									Backend::ECS::Component::PlayerComponent*,				\
									Backend::ECS::Component::SpawnerComponent*,				\
									Backend::ECS::Component::SpriteComponent*,				\
									Backend::ECS::Component::TileComponent*,				\
									Backend::ECS::Component::TransformComponent*,			\
									Backend::ECS::Component::PrefabComponent*,				\
									Backend::ECS::Component::PetComponent*,					\
									Backend::ECS::Component::ArrowComponent*,				\
									Backend::ECS::Component::TextRenderingComponent*,		\
									Backend::ECS::Component::UILogicComponent*,				\
									Backend::ECS::Component::RectTransformComponent*,		\
									Backend::ECS::Component::AnimationKeyComponent*,		\
									Backend::ECS::Component::PortalComponent*,				\
									Backend::ECS::Component::ParticleComponent*,			\
									Backend::ECS::Component::EmitterComponent*,				\
									Backend::ECS::Component::CheeseComponent*

#define ECS_ALL_COMPONENTS	Backend::ECS::Component::AnimationComponent,			\
							Backend::ECS::Component::ColliderComponent,				\
							Backend::ECS::Component::IsometricTransformComponent,	\
							Backend::ECS::Component::MovementComponent,				\
							Backend::ECS::Component::MinigameComponent,				\
							Backend::ECS::Component::PlayerComponent,				\
							Backend::ECS::Component::SpawnerComponent,				\
							Backend::ECS::Component::SpriteComponent,				\
							Backend::ECS::Component::TileComponent,					\
							Backend::ECS::Component::TransformComponent,			\
							Backend::ECS::Component::PrefabComponent,				\
							Backend::ECS::Component::PetComponent,					\
							Backend::ECS::Component::ArrowComponent,				\
							Backend::ECS::Component::TextRenderingComponent,        \
							Backend::ECS::Component::UILogicComponent,				\
							Backend::ECS::Component::RectTransformComponent,		\
							Backend::ECS::Component::AnimationKeyComponent,			\
							Backend::ECS::Component::PortalComponent,				\
							Backend::ECS::Component::ParticleComponent,				\
							Backend::ECS::Component::EmitterComponent,				\
							Backend::ECS::Component::CheeseComponent

namespace Backend::ECS::Component
{
	using AllComponents = std::tuple<ECS_ALL_COMPONENTS>;
}

#endif
