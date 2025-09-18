// ======================================================================
// File Name:		ArrowComponent.h
// Project Name:	Zoodiac
// Author(s):		Lye Pin Liang Xavier(100%)
// Brief:			Contains Arrow Component Info
// 
// All content © 2023 DigiPen Institute of Technology Singapore. All 
// rights reserved.
// ======================================================================

#ifndef ARROW_COMPONENT_H_
#define ARROW_COMPONENT_H_

#include <AEEngine.h>

#include <Backend/ECS/ECS_Components.h>

namespace Backend::ECS::Component
{
	/**
	 * \brief	Component that holds the arrow data of an entity.
	 */
	class ArrowComponent : public ECS_Component
	{
	protected:
	public:
		AEVec2 m_StartPosition;
		AEVec2 m_EndPosition;
		float m_Lifetime;

		/**
		 * \brief			Constructor.
		 * \param _entity	Entity ID.
		 */
		explicit ArrowComponent(const ECS_Entity _entity = 0)
			: ECS_Component{ _entity },
			m_StartPosition{ 0.f,0.f },
			m_EndPosition{ 0.f,0.f },
			m_Lifetime{ 0.f }
		{
		}

		/**
		 * \brief	Destructor.
		 */
		~ArrowComponent() override = default;
	};
}

#endif