// ======================================================================
// File Name:		MovementComponent.h
// Project Name:	Zoodiac
// Author(s):		Lye Pin Liang Xavier(100%)
// Brief:			Contains Movement Component Info
// 
// All content © 2023 DigiPen Institute of Technology Singapore. All 
// rights reserved.
// ======================================================================


#ifndef MOVEMENT_COMPONENT_H_
#define MOVEMENT_COMPONENT_H_

#include <AEEngine.h>

#include <Backend/ECS/ECS_Components.h>

namespace Backend::ECS::Component
{
	/**
	 * \brief	Component that holds the movement of an entity.
	 */
	class MovementComponent : public ECS_Component
	{
	public:

		AEVec2 m_Direction;
		AEVec2 m_PrevPosition;
		bool m_Jump;
		bool m_OnGround;
		float m_MaxHeight;
		float m_OriginalHeight;
		float m_JumpSpeed;
		float m_Gravity;
		float m_FallingSpeed;
		float m_Speed;

		/**
		 * \brief			Constructor.
		 * \param _entity	Entity ID.
		 */
		MovementComponent(const ECS_Entity _entity = 0)
			: ECS_Component{ _entity },
			m_Direction{ 0,0 },
			m_PrevPosition{ 0,0 },
			m_Jump{ false },
			m_OnGround{ true },
			m_MaxHeight{ 0.0f },
			m_OriginalHeight{ 0.0f },
			m_JumpSpeed{ 0.0f },
			m_Gravity{ 0.0f },
			m_FallingSpeed{ 0.0f },
			m_Speed{ 0.0f }
		{
		}

		/**
		 * \brief	Destructor.
		 */
		~MovementComponent() override = default;
	};
}

#endif