// ======================================================================
// File Name:		CheeseComponent.h
// Project Name:	Zoodiac
// Author(s):		Lim Geng Yang(100%)
// Brief:			Contains Arrow Component Info
// 
// All content © 2023 DigiPen Institute of Technology Singapore. All 
// rights reserved.
// ======================================================================

#ifndef CHEESE_COMPONENT_H_
#define CHEESE_COMPONENT_H_

#include <Backend/ECS/ECS_Components.h>

namespace Backend::ECS::Component
{
	/**
	 * \brief	Component that holds the cheese data of an entity.
	 */
	class CheeseComponent : public ECS_Component
	{
	protected:
	public:
		float m_Lifetime;
		float m_OriginalTime;

		/**
		 * \brief			Constructor.
		 * \param _entity	Entity ID.
		 */
		explicit CheeseComponent(const ECS_Entity _entity = 0)
			: ECS_Component{ _entity },
			m_Lifetime{ 0.f },
			m_OriginalTime{ 0.0f }
		{
		}

		/**
		 * \brief	Destructor.
		 */
		~CheeseComponent() override = default;
	};
}

#endif