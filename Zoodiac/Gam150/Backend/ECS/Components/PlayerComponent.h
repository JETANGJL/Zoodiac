// ======================================================================
// File Name:		PlayerComponent.h
// Project Name:	Zoodiac
// Author(s):		Low Zhi Sheng Kitson(40%), Lye Pin Liang Xavier(30%),
//					Lim Geng Yang(30%)
// Brief:			Contains Player Component Info
// 
// All content © 2023 DigiPen Institute of Technology Singapore. All 
// rights reserved.
// ======================================================================


#ifndef PLAYER_COMPONENT_H_
#define PLAYER_COMPONENT_H_

#include <Backend/ECS/ECS_Components.h>

namespace Backend::ECS::Component
{
	/**
	 * \brief	Component that holds the player of an entity.
	 */
	class PlayerComponent : public ECS_Component
	{
	public:
		enum Pet_Type
		{
			PET_NIL,
			PET_RAT,
			PET_DOG,
			PET_COW,
			PET_MONKEY,
			PET_CHICKEN,

			NUM_PET
		};
		bool m_FollowCamera;
		uint32_t m_Score, m_Hit; 
		inline static Pet_Type m_Pet{ PET_RAT }, m_CurrentPet{ PET_NIL };
		Pet_Type m_ScenePet;
		/**
		 * \brief			Constructor.
		 * \param _entity	Entity ID.
		 */
		PlayerComponent(const ECS_Entity _entity = 0)
			:ECS_Component{ _entity },
			m_FollowCamera{true},
			m_Score{ 0 },
			m_Hit{ 0 },
			m_ScenePet {PET_NIL}
		{
		}

		/**
		 * \brief	Destructor.
		 */
		~PlayerComponent() override = default;
	};
}

#endif