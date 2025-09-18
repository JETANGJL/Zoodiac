// ======================================================================
// File Name:		AnimationKeyComponent.h
// Project Name:	Zoodiac
// Author(s):		Low Zhi Sheng Kitson(60%), Lim Geng Yang (40%)
// Brief:			Contains Animation Key Component Info
// 
// All content © 2023 DigiPen Institute of Technology Singapore. All 
// rights reserved.
// ======================================================================

#ifndef ANIMATION_KEY_COMPONENT_H_
#define ANIMATION_KEY_COMPONENT_H_

#include <string>
#include <array>

#include <Backend/ECS/ECS_Components.h>

namespace Backend::ECS::Component
{
	/**
	 * \brief	Component that holds the Animation Keys of an entity.
	 */
	class AnimationKeyComponent : public ECS_Component
	{
	public:
		enum Direction
		{
			DIR_DOWN,
			DIR_UP,
			DIR_LEFT,
			DIR_RIGHT,

			NUM_DIR
		}; Direction m_LastDir;
		std::array<std::string, 4> m_Key;

		/**
		 * \brief			Constructor.
		 * \param _entity	Entity ID.
		 */
		explicit AnimationKeyComponent(const ECS_Entity _entity = 0)
			: ECS_Component{ _entity },
			m_LastDir{ Direction::DIR_DOWN },
			m_Key{" ", " ", " ", " "}
		{
		}

		/**
		 * \brief	Destructor.
		 */
		~AnimationKeyComponent() override = default;
	};
}

#endif