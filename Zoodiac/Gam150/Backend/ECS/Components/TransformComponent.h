// ======================================================================
// File Name:		TransformComponent.h
// Project Name:	Zoodiac
// Author(s):		Low Zhi Sheng Kitson(100%)
// Brief:			Contains Transform Component Info
// 
// All content © 2023 DigiPen Institute of Technology Singapore. All 
// rights reserved.
// ======================================================================


#ifndef TRANSFORM_COMPONENT_H_
#define TRANSFORM_COMPONENT_H_

#include <AEEngine.h>
#include <Backend/ECS/ECS_Components.h>

namespace Backend::ECS::Component
{
	/**
	 * \brief	Component that holds the position of an entity.
	 */
	class TransformComponent : public ECS_Component
	{
	protected:
	public:
		AEVec2 m_Position;
		bool m_IsStatic;
		float m_Angle;
		AEVec2 m_Scale;
		AEMtx33 m_Transform;

		/**
		 * \brief			Constructor.
		 * \param _entity	Entity ID.
		 */
		TransformComponent(const ECS_Entity _entity = 0)
			: ECS_Component{ _entity },
			m_Position{},
			m_IsStatic{ false },
			m_Angle{ 0.f },
			m_Scale{ 1, 1 },
			m_Transform{}
		{
		}

		/**
		 * \brief	Destructor.
		 */
		~TransformComponent() override = default;
		AEVec2& Position() { return m_Position; }
		void Position(AEVec2 _position) { m_Position = _position; }
	};
}

#endif