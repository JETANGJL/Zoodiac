// ======================================================================
// File Name:		ParticleComponent.h
// Project Name:	Zoodiac
// Author(s):		Lim Geng Yang(100%)
// Brief:			Contains Particle Component Info
// 
// All content © 2023 DigiPen Institute of Technology Singapore. All 
// rights reserved.
// ======================================================================


#ifndef PARTICLE_COMPONENT_H_
#define PARTICLE_COMPONENT_H_

#include <AEEngine.h>
#include <Backend/ECS/ECS_Components.h>

namespace Backend::ECS::Component
{
	/**
	 * \brief	Component that holds the particle data of an entity.
	 */
	class ParticleComponent : public ECS_Component
	{
	protected:
	public:
		float m_Lifetime;
		float m_LifeRemain;
		float m_SizeBegin;
		float m_SizeEnd;
		float m_AlphaBegin;
		float m_AlphaEnd;
		/**
		 * \brief			Constructor.
		 * \param _entity	Entity ID.
		 */
		ParticleComponent(const ECS_Entity _entity = 0)
			: ECS_Component{ _entity },
			m_Lifetime{2.f},
			m_LifeRemain{2.f},
			m_SizeBegin{1.f},
			m_SizeEnd{1.f},
			m_AlphaBegin{1.f},
			m_AlphaEnd{0.f}
		{
		}

		/**
		 * \brief	Destructor.
		 */
		~ParticleComponent() override = default;
	};
}

#endif