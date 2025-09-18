// ======================================================================
// File Name:		IsometricTransformComponent.h
// Project Name:	Zoodiac
// Author(s):		Lye Pin Liang Xavier(100%)
// Brief:			Contains Isometric Transform Component Info
// 
// All content © 2023 DigiPen Institute of Technology Singapore. All 
// rights reserved.
// ======================================================================


#ifndef ISOMETRIC_TRANSFORM_COMPONENT_H_
#define ISOMETRIC_TRANSFORM_COMPONENT_H_

#include <AEEngine.h>

#include <Backend/ECS/Components/TransformComponent.h>

namespace Backend::ECS::Component
{
	/**
	 * \brief	Component that holds the isometric data of an entity.
	 */
	class IsometricTransformComponent : public TransformComponent
	{
	public:
		enum Tag
		{
			TAG_TILE,
			TAG_NONTILE,
			TAG_WATER,
			TAG_FALLING,

			NUM_TAG
		};

		Tag m_Tag;

		inline static AEMtx33 s_IsoTransform =
		{
			1, -1, 0,
			0.5, 0.5, 0,
			0, 0, 0
		};

		inline static AEMtx33 s_IsoInverse = 
		{
		0.5, 1, 0,
		-0.5, 1, 0,
		0,0,0
		};

		AEVec2 m_IsometricPosition;
		float m_Height;
		float m_MinHeight;

		/**
		 * \brief			Constructor.
		 * \param _entity	Entity ID.
		 */
		IsometricTransformComponent(const ECS_Entity _entity = 0)
			: TransformComponent{ _entity },
			m_Tag{ TAG_NONTILE },
			m_IsometricPosition{ 0.f, 0.f },
			m_Height{ 0.f },
			m_MinHeight{ 0 }
		{
		}

		/**
		 * \brief	Destructor.
		 */
		~IsometricTransformComponent()	override = default;
	};
}

#endif 