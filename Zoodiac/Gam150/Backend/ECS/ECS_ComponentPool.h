// ======================================================================
// File Name:		ECS_ComponentPool.h
// Project Name:	Zoodiac
// Author(s):		Low Zhi Sheng Kitson(100%)
// Brief:			Contains ECS Component Pool functionality
// 
// All content © 2023 DigiPen Institute of Technology Singapore. All 
// rights reserved.
// ======================================================================

#ifndef ECS_COMPONENT_POOL_H_
#define ECS_COMPONENT_POOL_H_

#include <type_traits>
#include <vector>

#include <Backend/ECS/ECS_Components.h>

namespace Backend::ECS
{
	/**
	 * \brief Abstract Component Pool SHOULD NOT BE INSTANTIATED
	 */
	class AbstractComponentPool
	{
	protected:
		/**
		 * \brief Private Constructor
		 */
		AbstractComponentPool() = default;

		/**
		 * \brief			Private Copy Constructor
		 * \param _other	Object to copy
		 */
		AbstractComponentPool(const AbstractComponentPool& _other) = default;

		/**
		 * \brief			Private Copy Assignment Operator
		 * \param _other	Object to copy
		 */
		auto operator=(const AbstractComponentPool& _other) -> AbstractComponentPool& = default;

		/**
		 * \brief			Private Move Constructor
		 * \param _other	Object to move
		 */
		AbstractComponentPool(AbstractComponentPool&& _other) = default;

		/**
		 * \brief			Private Move Assignment Operator
		 * \param _other	Object to move
		 */
		auto operator=(AbstractComponentPool&& _other) -> AbstractComponentPool& = default;
	public:

		/**
		 * \brief Destructor
		 */
		virtual ~AbstractComponentPool() = default;

		/**
		 * \brief			Frees a component from the pool
		 * \param _entity	Entity that owns the component to be freed
		 * \return			Returns true if successful
		 */
		virtual auto FreeComponent(ECS_Entity _entity) -> bool = 0;
	};

	template <typename Component>
	class ComponentPool final : public AbstractComponentPool
	{
		static_assert(std::is_base_of_v<ECS_Component, Component>, "Component Pool must contain template argument of type Component");
	private:
		std::vector<Component> m_Pool;	// Component Pool
		unsigned m_FreeIndex;			// The current free index
	public:
		/**
		 * \brief Constructor
		 */
		ComponentPool<Component>();

		/**
		 * \brief Destructor
		 */
		~ComponentPool<Component>() override = default;

		/**
		 * \brief			Copy Constructor
		 * \param _other	Object to copy
		 */
		ComponentPool(const ComponentPool& _other) = default;

		/**
		 * \brief			Copy Assignment Operator
		 * \param _other	Object to copy
		 */
		auto operator=(const ComponentPool& _other) -> ComponentPool& = default;

		/**
		 * \brief			Move Constructor
		 * \param _other	Object to move
		 */
		ComponentPool(ComponentPool&& _other) = default;

		/**
		 * \brief			Move Assignment Operator
		 * \param _other	Object to move
		 */
		auto operator=(ComponentPool&& _other) -> ComponentPool& = default;

		/**
		 * \brief	Gets a free component
		 * \return	The free component
		 */
		auto GetComponent() -> Component&;

		/**
		 * \brief			Gets a component belonging to an entity
		 * \param _entity	The entity the component belongs to
		 * \return			The reference to the component
		 */
		auto GetComponent(ECS_Entity _entity) -> Component&;

		/**
		 * \brief			Frees a component from the pool
		 * \param _entity	Entity that owns the component to be freed
		 * \return			Returns true if successful
		 */
		auto FreeComponent(ECS_Entity _entity) -> bool override;
	};
}

#include <Backend/ECS/ECS_ComponentPool.tpp>

#endif