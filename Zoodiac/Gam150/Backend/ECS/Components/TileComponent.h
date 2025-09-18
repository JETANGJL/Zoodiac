// ======================================================================
// File Name:		TileComponent.h
// Project Name:	Zoodiac
// Author(s):		Low Zhi Sheng Kitson(100%)
// Brief:			Contains Tile Component Info
// 
// All content © 2023 DigiPen Institute of Technology Singapore. All 
// rights reserved.
// ======================================================================


#ifndef TILE_COMPONENT_H_
#define TILE_COMPONENT_H_

#include <Backend/ECS/ECS_Components.h>

namespace Backend::ECS::Component
{
	/**
	 * \brief	Component that holds the tile data of an entity.
	 */
	class TileComponent : public ECS_Component
	{
	public:
		float m_Red, m_Blue, m_Green, m_Alpha, m_Timer;

		enum Tag
		{
			TAG_OVERWORLD,
			TAG_FALLINGGAME,
			TAG_WINTILE,

			NUM_TAG
		};
		Tag m_Tag;

		/**
		 * \brief			Constructor.
		 * \param _entity	Entity ID.
		 */
		TileComponent(const ECS_Entity _entity = 0)
			: ECS_Component{ _entity },
			m_Red{ 0.0f },
			m_Blue{ 0.0f },
			m_Green{ 0.0f },
			m_Alpha{ 0.0f },
			m_Timer{ 0.0f },
			m_Tag{}
		{
		}

		/**
		 * \brief	Destructor.
		 */
		~TileComponent() override = default;
	};
}

#endif