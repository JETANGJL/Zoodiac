// ======================================================================
// File Name:		MinigameComponent.h
// Project Name:	Zoodiac
// Author(s):		Low Zhi Sheng Kitson(100%)
// Brief:			Contains Minigame Component Info
// 
// All content © 2023 DigiPen Institute of Technology Singapore. All 
// rights reserved.
// ======================================================================


#ifndef MINIGAME_COMPONENT_H_
#define MINIGAME_COMPONENT_H_

#include <Backend/ECS/ECS_Components.h>

namespace Backend::ECS::Component
{
	/**
	 * \brief	Component that holds the minigame information of an entity.
	 */
	class MinigameComponent : public ECS_Component
	{
	public:
		enum class Tag
		{
			TAG_TIMERWIN,
			TAG_TIMERLOSE,
			TAG_SCORE,

			NUM_TAG
		};
		Tag m_Tag;
		uint32_t m_HitTarget;
		uint32_t m_ScoreTarget;
		bool m_MinigameComplete;
		float m_Timer;
		float m_CurrTime;

		MinigameComponent(const ECS_Entity _entity = 0)
			: ECS_Component{ _entity },
			m_Tag{ Tag::TAG_SCORE },
			m_HitTarget{ 0 },
			m_ScoreTarget{ 0 },
			m_MinigameComplete{ false },
			m_Timer{ 0.f },
			m_CurrTime{ 0.f }
		{
		}

		/**
		 * \brief	Destructor.
		 */
		~MinigameComponent() override = default;
	};
}

#endif