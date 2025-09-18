// ======================================================================
// File Name:		StateWild.cpp
// Project Name:	Zoodiac
// Author(s):		Ang Jie Le Jet(60%), Low Zhi Sheng Kitson(40%)
// Brief:			Contains Wild State Functionality
// 
// All content © 2023 DigiPen Institute of Technology Singapore. All 
// rights reserved.
// ======================================================================

#include <Backend/ECS/ECS.h>
#include <Backend/ECS/Components/IsometricTransformComponent.h>
#include <Backend/ECS/Components/MovementComponent.h>
#include <Backend/ECS/Components/PetComponent.h>
#include <States/StateWild.h>

void StateWild::Update(Backend::ECS::Component::PetComponent* _petComponent)
{
    //logic for Wild
    static AEVec2 min{124.0f,124.0f};
    static AEVec2 max{ -124.0f,-124.0f };
    const auto& transform = ECS_WORLD.GetComponent<Backend::ECS::Component::IsometricTransformComponent>(_petComponent->Entity());
    auto& movement = ECS_WORLD.GetComponent<Backend::ECS::Component::MovementComponent>(_petComponent->Entity());
    StateUtils::RandomState(movement.m_Direction, transform.m_Position, min, max);
}
