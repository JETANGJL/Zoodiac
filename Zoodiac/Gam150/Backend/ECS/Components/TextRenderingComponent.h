// ======================================================================
// File Name:		TextRenderingComponent.h
// Project Name:	Zoodiac
// Author(s):		Muhammad Dzulhafiz(100%)
// Brief:			Contains Text Component Info
// 
// All content © 2023 DigiPen Institute of Technology Singapore. All 
// rights reserved.
// ======================================================================

#ifndef TEXT_RENDERING_COMPONENT_H_
#define TEXT_RENDERING_COMPONENT_H_

#include <vector>
#include <Backend/ECS/ECS_Components.h>

namespace Backend::ECS::Component
{
	/**
	 * \brief	Component that holds the text data of an entity.
	 */
	class TextRenderingComponent : public ECS_Component
	{
	public:
		struct TextInfo
		{
			std::string m_Text;
			AEVec2 m_ScreenPosition;
			float m_Scale;
		};

		std::vector<TextInfo> m_Text;
		std::string m_Font;
		/**
		 * \brief			Constructor.
		 * \param _entity	Entity ID.
		 */
		TextRenderingComponent(const ECS_Entity _entity = 0)
			:ECS_Component{ _entity }
		{
		}

		/**
		 * \brief	Destructor.
		 */
		~TextRenderingComponent() override = default;
	};

}

#endif