// ======================================================================
// File Name:		EmitterComponent.h
// Project Name:	Zoodiac
// Author(s):		Lim Geng Yang(100%)
// Brief:			Contains Emitter Component Info
// 
// All content © 2023 DigiPen Institute of Technology Singapore. All 
// rights reserved.
// ======================================================================

#ifndef EMITTER_COMPONENT_H_
#define EMITTER_COMPONENT_H_

#include <AEEngine.h>

#include <Backend/ECS/ECS_Components.h>

namespace Backend::ECS::Component
{
	/**
	 * \brief	Component that holds the emitter data of an entity.
	 */
	class EmitterComponent : public ECS_Component
	{
	protected:
	public:
		enum Type
		{
			MOVE,
			PORTAL,
			POOF,
			FALL,
			CLOUD,

			NUM_TYPE
		}; Type m_Type;
		bool m_Emit;
		float m_Cooldown;

		/**
		 * \brief			Constructor.
		 * \param _entity	Entity ID.
		 */
		explicit EmitterComponent(const ECS_Entity _entity = 0)
			: ECS_Component{ _entity },
			m_Type{},
			m_Emit{ false },
			m_Cooldown{ 0.0f }
		{
		}

		/**
		 * \brief	Destructor.
		 */
		~EmitterComponent() override = default;
	};
}

#endif