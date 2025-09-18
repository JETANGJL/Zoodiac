// ======================================================================
// File Name:		ECS_Component.h
// Project Name:	Zoodiac
// Author(s):		Low Zhi Sheng Kitson(100%)
// Brief:			Contains ECS Component functionality
// 
// All content © 2023 DigiPen Institute of Technology Singapore. All 
// rights reserved.
// ======================================================================

#ifndef ECS_COMPONENTS_H_
#define ECS_COMPONENTS_H_

#include <Backend/ECS/ECS_Common.h>

namespace Backend::ECS
{
	/**
	 * \brief	Base class for all components
	 */
	class ECS_Component
	{
	protected:
		ECS_Entity m_EntityID;	// Entity ID
	public:

		/**
		 * \brief			Constructor
		 * \param _entity	Entity ID
		 */
		explicit ECS_Component(const ECS_Entity _entity = 0)
			: m_EntityID{ _entity }
		{
		}

		/**
		 * \brief			Copy Constructor
		 * \param _other	Component to copy
		 */
		ECS_Component(const ECS_Component& _other) = default;

		/**
		 * \brief			Copy Assignment Operator
		 * \param _other	Component to copy
		 */
		ECS_Component& operator=(const ECS_Component& _other) = default;

		/**
		 * \brief			Move Assignment Operator
		 * \param _other	Component to move
		 */
		ECS_Component(ECS_Component&& _other) = default;

		/**
		 * \brief			Move Assignment Operator
		 * \param _other	Component to move
		 */
		ECS_Component& operator=(ECS_Component&& _other) = default;

		/**
		 * \brief	Destructor
		 */
		virtual ~ECS_Component() = default;

		/**
		 * \brief	Gets the entity ID
		 * \return	Entity ID
		 */
		[[nodiscard]] ECS_Entity Entity() const { return m_EntityID; }

		/**
		 * \brief			Sets the entity ID
		 * \param _entity	Entity ID
		 */
		auto Entity(const ECS_Entity _entity) -> void { m_EntityID = _entity; }

		/**
		 * \brief				Registers a component type
		 * \tparam Component	Component type
		 * \param _id			Component ID
		 * \return				Component ID
		 */
		template <typename Component>
		static auto GetComponentID(ECS_ComponentType _id = 0) -> ECS_ComponentType;

		/**
		 * \brief				Gets the bitset index of a component type
		 * \tparam Component	Component type
		 * \return				Bitset index
		 */
		template<typename Component>
		static auto GetBitsetIndex() -> ECS_ComponentType;
	};
}

#include <Backend/ECS/ECS_Components.tpp>

#endif