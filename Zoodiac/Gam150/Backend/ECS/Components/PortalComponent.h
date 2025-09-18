// ======================================================================
// File Name:		PortalComponent.h
// Project Name:	Zoodiac
// Author(s):		Lye Pin Liang Xavier(100%)
// Brief:			Contains Portal Component Info
// 
// All content © 2023 DigiPen Institute of Technology Singapore. All 
// rights reserved.
// ======================================================================


#ifndef PORTAL_COMPONENT_H_
#define PORTAL_COMPONENT_H_

#include <Backend/ECS/ECS_Components.h>
#include <Backend/SceneManagement/SceneCommon.h>
#include <Backend/ECS/Components/PlayerComponent.h>

namespace Backend::ECS::Component
{
	/**
	 * \brief	Component that holds the portal data of an entity.
	 */
	class PortalComponent : public ECS_Component
	{
	protected:
	public:
		
		Scene scene;
		bool collide;
		PlayerComponent::Pet_Type pet;

		/**
		 * \brief			Constructor.
		 * \param _entity	Entity ID.
		 */
		explicit PortalComponent(const ECS_Entity _entity = 0)
			: ECS_Component{ _entity },
			scene{ SCENE_OVERWORLD },
			collide{ false },
			pet {PlayerComponent::PET_NIL}
		{
		}

		/**
		 * \brief	Destructor.
		 */
		~PortalComponent() override = default;
	};
}

#endif