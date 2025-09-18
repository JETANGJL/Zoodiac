// ======================================================================
// File Name:		StateAngry.cpp
// Project Name:	Zoodiac
// Author(s):		Ang Jie Le Jet(60%), Low Zhi Sheng Kitson(40%)
// Brief:			Contains Angry State Functionality
// 
// All content © 2023 DigiPen Institute of Technology Singapore. All 
// rights reserved.
// ======================================================================

#include <Backend/Engine_Common.h>
#include <Backend/ECS/ECS.h>
#include <Backend/ECS/ECS_IncludeComponents.h>
#include <States/StateAngry.h>

void StateAngry::Update(Backend::ECS::Component::PetComponent* _petComponent)
{
    const auto& transform = ECS_WORLD.GetComponent<Backend::ECS::Component::IsometricTransformComponent>(_petComponent->Entity());
    auto& movement = ECS_WORLD.GetComponent<Backend::ECS::Component::MovementComponent>(_petComponent->Entity());
    StateUtils::MoveAwayFromPoint(movement.m_Direction, transform.m_Position, *DI_GET_PLAYER_POS());
}
