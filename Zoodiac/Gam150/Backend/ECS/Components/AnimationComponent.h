// ======================================================================
// File Name:		AnimationComponent.h
// Project Name:	Zoodiac
// Author(s):		Low Zhi Sheng Kitson(60%), Lim Geng Yang (40%)
// Brief:			Contains Animation Component Info
// 
// All content © 2023 DigiPen Institute of Technology Singapore. All 
// rights reserved.
// ======================================================================

#ifndef ANIMATION_COMPONENT_H_
#define ANIMATION_COMPONENT_H_

#include <string>
#include <vector>

#include <Backend/ECS/ECS_Components.h>

namespace Backend::ECS::Component
{
	/**
	 * \brief	Component that holds the Animation of an entity.
	 */
	class AnimationComponent : public ECS_Component
	{
	public:
		std::vector<std::string>* m_Animations;
		std::size_t m_CurrFrameIndex;
		std::size_t m_TotalAnims;
		float m_CurrTime;
		float m_AnimTime;
		/**
		 * \brief			Constructor.
		 * \param _entity	Entity ID.
		 */
		explicit AnimationComponent(const ECS_Entity _entity = 0)
			: ECS_Component{ _entity },
			m_Animations{ nullptr },
			m_CurrFrameIndex{ 0 },
			m_TotalAnims{ 0 },
			m_CurrTime{ 0.0f },
			m_AnimTime{ 0.5f }
		{
		}

		/**
		 * \brief	Destructor.
		 */
		~AnimationComponent() override = default;
	};
}

#endif