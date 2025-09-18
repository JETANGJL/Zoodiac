// ======================================================================
// File Name:		PetComponent.h
// Project Name:	Zoodiac
// Author(s):		Ang Jie Le Jet(100%)
// Brief:			Contains Pet Component Info
// 
// All content © 2023 DigiPen Institute of Technology Singapore. All 
// rights reserved.
// ======================================================================

#ifndef PET_COMPONENT_H_
#define PET_COMPONENT_H_

#include <States/State.h>
#include <Backend/ECS/ECS_Common.h>
#include <Backend/ECS/ECS_Components.h>

namespace Backend::ECS::Component
{
	/**
	 * \brief	Component that holds the pet data of an entity.
	 */
	class PetComponent : public ECS_Component
	{
	public:
		State* m_State = nullptr;
		State* m_NextState = nullptr;
		float m_Angle;
		float m_TimeSpent;
		float m_Radius;

		/**
		 * \brief			Constructor
		 * \param _entity	Entity
		 */
		PetComponent(const ECS_Entity _entity = 0)
			: ECS_Component(_entity),
			m_Angle{0.0f},
			m_TimeSpent{0.0f},
			m_Radius{0.0f}
		{}
		
		/**
		 * \brief Destructor
		 */
		~PetComponent() override = default;

		/**
		 * \brief Updates the entity based on current pet behaviour
		 */
		void Update();
	};

}

#endif