// ======================================================================
// File Name:		SpawnerComponent.h
// Project Name:	Zoodiac
// Author(s):		Lye Pin Liang Xavier(100%)
// Brief:			Contains Spawner Component Info
// 
// All content © 2023 DigiPen Institute of Technology Singapore. All 
// rights reserved.
// ======================================================================

#ifndef SPAWNER_COMPONENT_H_
#define SPAWNER_COMPONENT_H_

#include <AEEngine.h>
#include <functional>

#include <Backend/ECS/ECS_Components.h>

namespace Backend::ECS::Component
{
	/**
	 * \brief	Component that holds the spawner of an entity.
	 */
	class SpawnerComponent : public ECS_Component
	{
	public:
		float m_Timer;
		float m_CurrentTime;
		std::function<ECS_Entity(AEVec2, AEVec2)> m_GenerateEntity;
		AEVec2 m_MinRange;
		AEVec2 m_MaxRange;
		unsigned m_SpawnCount;
		unsigned m_SpawnPerInterval;
		unsigned m_CurrSpawnCount;
		AEVec2 m_Direction;

		/**
		 * \brief			Constructor.
		 * \param _entity	Entity ID.
		 */
		SpawnerComponent(const ECS_Entity _entity = 0)
			: ECS_Component{ _entity },
			m_Timer{ 0.0f },
			m_CurrentTime{ 0.0f },
			m_GenerateEntity{ nullptr },
			m_MinRange{ 0.0f,0.0f },
			m_MaxRange{ 0.0f,0.0f },
			m_SpawnCount{ 0 },
			m_SpawnPerInterval{ 0 },
			m_CurrSpawnCount{ 0 },
			m_Direction{0, 0}
		{
		}

		/**
		 * \brief	Destructor.
		 */
		~SpawnerComponent() override = default;
	};
}

#endif