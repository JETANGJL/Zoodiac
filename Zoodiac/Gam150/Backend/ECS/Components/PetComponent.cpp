// ======================================================================
// File Name:		PetComponent.cpp
// Project Name:	Zoodiac
// Author(s):		Ang Jie Le Jet(100%)
// Brief:			Contains Pet Component Info
// 
// All content © 2023 DigiPen Institute of Technology Singapore. All 
// rights reserved.
// ======================================================================

#include <Backend/ECS/Components/PetComponent.h>
#include <States/State.h>
#include <Backend/ECS/ECS.h>
#include <Backend/ECS/Components/MovementComponent.h>

namespace Backend::ECS::Component
{
	void PetComponent::Update()
	{
		if (m_State != m_NextState)
		{
			auto& movement = ECS_WORLD.GetComponent<Backend::ECS::Component::MovementComponent>(Entity());
			movement.m_Direction = {0,0};
			m_State = m_NextState;
		}
		if (m_State)
			m_State->Update(this);
	}
}
