// ======================================================================
// File Name:		RectTransformComponent.h
// Project Name:	Zoodiac
// Author(s):		Muhammad Dzulhafiz(100%)
// Brief:			Contains Rect Transform Component Info
// 
// All content © 2023 DigiPen Institute of Technology Singapore. All 
// rights reserved.
// ======================================================================

#ifndef RECT_TRANSFORM_COMPONENT_H_
#define RECT_TRANSFORM_COMPONENT_H_

#include <AEEngine.h>

#include <Backend/ECS/Components/TransformComponent.h>

namespace Backend::ECS::Component
{
	/**
	 * \brief	Component that holds the on screen data of an entity.
	 */
	class RectTransformComponent: public TransformComponent
	{
	protected:
	public:
		float m_Height;
		float m_Width;
		AEVec2 m_MinPosition;
		AEVec2 m_MaxPosition;
		bool m_Render;
		/**
		 * \brief			Constructor.
		 * \param _entity	Entity ID.
		 */
		RectTransformComponent(const ECS_Entity _entity = 0)
			:TransformComponent{ _entity },
			m_Height{ 1 },
			m_Width{ 1 },
			m_MinPosition{ },
			m_MaxPosition{ },
			m_Render{ true }
		{
		}

		/**
		 * \brief	Destructor.
		 */
		~RectTransformComponent() override = default;
	};

}

#endif