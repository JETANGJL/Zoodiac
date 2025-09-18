// ======================================================================
// File Name:		PrefabComponent.h
// Project Name:	Zoodiac
// Author(s):		Low Zhi Sheng Kitson(100%)
// Brief:			Contains Prefab Component Info
// 
// All content © 2023 DigiPen Institute of Technology Singapore. All 
// rights reserved.
// ======================================================================

#ifndef PREFAB_COMPONENT_H_
#define PREFAB_COMPONENT_H_

#include <Backend/ECS/ECS_Components.h>

namespace Backend::ECS::Component
{
	/**
	 * \brief	Component for serializer
	 */
	class PrefabComponent : public ECS_Component
	{
	public:
		std::string m_Key;
		AEVec2 m_Pos;
		float m_Height;

		/**
		 * \brief			Constructor.
		 * \param _entity	Entity ID.
		 */
		PrefabComponent(const ECS_Entity _entity = 0)
			:ECS_Component{ _entity },
			m_Key{""},
			m_Pos{},
			m_Height{}
		{
		}

		/**
		 * \brief	Destructor.
		 */
		~PrefabComponent() override = default;
	};
}

#endif
