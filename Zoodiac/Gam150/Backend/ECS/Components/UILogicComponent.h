// ======================================================================
// File Name:		UILogicComponent.h
// Project Name:	Zoodiac
// Author(s):		Muhammad Dzulhafiz(100%)
// Brief:			Contains UI Logic Component Info
// 
// All content © 2023 DigiPen Institute of Technology Singapore. All 
// rights reserved.
// ======================================================================

#ifndef UI_LOGIC_COMPONENT_H_
#define UI_LOGIC_COMPONENT_H_


#include <Backend/ECS/ECS_Components.h>

namespace Backend::ECS::Component
{
	/**
	 * \brief	Component that holds the UI Logic of an entity.
	 */
	class UILogicComponent : public ECS_Component
	{
	protected:
	public:

		bool m_IsHovering;

		enum Tag
		{	
			TAG_MAINMENU_PLAY,
			TAG_MAINMENU_TUTORIAL,
			TAG_MAINMENU_CREDITS,
			TAG_MAINMENU_OPTIONS,
			TAG_MAINMENU_EXIT,

			TAG_TUTORIALS_HOWTOPLAY,

			TAG_OPTIONS_FULLSCREEN,
			TAG_OPTIONS_CLEARDATA,
			TAG_OPTIONS_MINIMIZED,
			TAG_OPTIONS_RETURNTOMENU,
			TAG_OPTIONS_DATACLEARED,

			TAG_GAME_PAUSE,

			TAG_PAUSEMENU_CONTINUE,
			TAG_PAUSEMENU_RETURNTOMENU,
			TAG_PAUSEMENU_CONTROLS,
			TAG_PAUSEMENU_EXIT,

			TAG_TUTORIAL_CONTROLS,
			TAG_MINIGAME_INSTRUCTION,
			TAG_MINIGAME_ENDMINIGAME,
			TAG_OVERWORLD_TELEPORTPROMPT,

			TAG_CREDITS1_NEXT,
			TAG_CREDITS2_NEXT,
			TAG_CREDITS3_NEXT,
			TAG_CREDITS4_NEXT,
			TAG_CREDITS_RETURNTOMAINMENU,

			NUM_TAG
		}; Tag m_Tag;

		/**
		 * \brief			Constructor.
		 * \param _entity	Entity ID.
		 */
		UILogicComponent(const ECS_Entity _entity = 0)
			: ECS_Component{ _entity },
			m_IsHovering{ false },
			m_Tag{ NUM_TAG }
		{
		}

		/**
		 * \brief	Destructor.
		 */
		~UILogicComponent() override = default;
	};

}

#endif