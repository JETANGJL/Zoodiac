// ======================================================================
// File Name:		ECS_EntityStack.h
// Project Name:	Zoodiac
// Author(s):		Low Zhi Sheng Kitson(100%)
// Brief:			Contains ECS Entity Stack functionality
// 
// All content © 2023 DigiPen Institute of Technology Singapore. All 
// rights reserved.
// ======================================================================

#ifndef ECS_ENTITY_STACK_H_
#define ECS_ENTITY_STACK_H_

#include <stack>
#include <vector>

#include <Backend/ECS/ECS_Common.h>

namespace Backend::ECS
{
	/**
	 * \brief	An entity stack.
	 */
	class ECS_EntityStack
	{
	private:
		std::stack<ECS_Entity> m_FreeStack; //	Stack of free entities
		std::vector<ECS_Entity> m_InUseEntites; //	List of in use entities
	public:
		/**
		 * \brief	Default constructor.
		 */
		ECS_EntityStack();

		/**
		 * \brief	Gets an entity.
		 * \return	The entity.
		 */
		auto GetEntity() -> ECS_Entity;

		/**
		 * \brief			Frees an entity.
		 * \param _entity	The entity.
		 */
		auto FreeEntity(ECS_Entity _entity) -> void;

		/**
		 * \brief	Gets the last entity.
		 * \return	The last entity.
		 */
		[[nodiscard]] auto GetLast() const -> ECS_Entity;
	};
}

#endif