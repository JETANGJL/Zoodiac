// ======================================================================
// File Name:		SpriteComponent.h
// Project Name:	Zoodiac
// Author(s):		Low Zhi Sheng Kitson(100%)
// Brief:			Contains Sprite Component Info
// 
// All content © 2023 DigiPen Institute of Technology Singapore. All 
// rights reserved.
// ======================================================================


#ifndef SPRITE_COMPONENT_H_
#define SPRITE_COMPONENT_H_

#include <string>

#include <Backend/ECS/ECS_Components.h>

namespace Backend::ECS::Component
{
	/**
	 * \brief	Component that holds the sprite of an entity.
	 */
	class SpriteComponent : public ECS_Component
	{
	protected:
	public:
		enum RenderLayer
		{
			STATIC,
			DYNAMIC,
			NUM_LAYER,
		};
		RenderLayer m_Layer;
		std::string m_TextureKey;
		float m_Red, m_Blue, m_Green, m_Alpha;

		/**
		 * \brief			Constructor.
		 * \param _entity	Entity ID.
		 */
		SpriteComponent(const ECS_Entity _entity = 0)
			: ECS_Component{ _entity },
			m_Layer{ RenderLayer::NUM_LAYER },
			m_TextureKey{ " " },
			m_Red{ 0.0f },
			m_Blue{ 0.0f },
			m_Green{ 0.0f },
			m_Alpha{ 0.0f }
		{
		}

		/**
		 * \brief	Destructor.
		 */
		~SpriteComponent() override = default;
	};
}

#endif