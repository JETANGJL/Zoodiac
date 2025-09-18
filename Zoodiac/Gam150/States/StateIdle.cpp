// ======================================================================
// File Name:		StateIdle.cpp
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
#include <States/StateIdle.h>

void StateIdle::Update(Backend::ECS::Component::PetComponent* _petComponent)
{
	const auto& transform = ECS_WORLD.GetComponent<Backend::ECS::Component::IsometricTransformComponent>(_petComponent->Entity());
    if (StateUtils::IsPetNear(transform.m_Position, *DI_GET_PLAYER_POS()))
        _petComponent->m_NextState = STATE_LIBRARY.GetResource(STATE_HAPPY);
}
