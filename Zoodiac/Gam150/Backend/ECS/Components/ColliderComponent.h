// ======================================================================
// File Name:		ColliderComponent.h
// Project Name:	Zoodiac
// Author(s):		Lye Pin Liang Xavier(100%)
// Brief:			Contains Collider Component Info
// 
// All content © 2023 DigiPen Institute of Technology Singapore. All 
// rights reserved.
// ======================================================================


#ifndef COLLIDER_COMPONENT_H_
#define COLLIDER_COMPONENT_H_

#include <AEEngine.h>

#include <Backend/ECS/ECS_Components.h>

namespace Backend::ECS::Component
{
	/**
	 * \brief	Component that holds the collider data of an entity.
	 */
	class ColliderComponent : public ECS_Component
	{
	public:
		/**
		 * \brief	Tag for the collider.
		 */
		enum Tag
		{
			TAG_PLAYER,
			TAG_TILE,
			TAG_BORDER,
			TAG_INTERACTABLE,
			TAG_FALLING_TILE,
			TAG_SLIDING_TILE,
			TAG_DESTROYER_TILE,
			TAG_ARROW,
			TAG_PORTAL,
			TAG_PET,

			NUM_TAG
		};

		Tag m_Tag;
		AEVec2 m_Size;

		/**
		 * \brief			Constructor.
		 * \param _entity	Entity ID.
		 */
		ColliderComponent(ECS_Entity _entity = 0)
			: ECS_Component{ _entity },
			m_Tag{ Tag::NUM_TAG },
			m_Size{ 60,60 }
		{
		}

		/**
		 * \brief	Destructor.
		 */
		~ColliderComponent() override = default;
	};
}

#endif