// ======================================================================
// File Name:		ECS_ComponentPool.tpp
// Project Name:	Zoodiac
// Author(s):		Low Zhi Sheng Kitson(100%)
// Brief:			Contains ECS Component Pool functionality
// 
// All content © 2023 DigiPen Institute of Technology Singapore. All 
// rights reserved.
// ======================================================================

#ifndef ECS_COMPONENT_POOL_TPP_
#define ECS_COMPONENT_POOL_TPP_

namespace Backend::ECS
{
	template<typename Component>
	ComponentPool<Component>::ComponentPool() : m_FreeIndex{0}
	{
		m_Pool.reserve(ECS_MaxEntities);
		for (int i = ECS_MaxEntities - 1; i > 0; --i)
			m_Pool.emplace_back(Component{});
	}

	template<typename Component>
	auto ComponentPool<Component>::GetComponent() -> Component&
	{
		assert(!(m_FreeIndex + 1 >= m_Pool.size()));
		for (ECS_Entity i = 0; i < m_FreeIndex; ++i)
		{
			if (m_Pool[i].Entity() == ECS_MaxEntities + 1)
				return m_Pool[i];
		}
		return m_Pool[m_FreeIndex++];
	}

	template<typename Component>
	auto ComponentPool<Component>::GetComponent(ECS_Entity _entity) -> Component&
	{
		for (ECS_Entity i = 0; i < m_FreeIndex; ++i)
		{
			if (m_Pool[i].Entity() == _entity)
				return m_Pool[i];
		}
		assert(false);
		return m_Pool[0];
	}

	template<typename Component>
	auto ComponentPool<Component>::FreeComponent(ECS_Entity _entity) -> bool
	{
		for (ECS_Entity i = 0; i < m_FreeIndex; ++i)
		{
			if (m_Pool[i].Entity() == _entity)
			{
				m_Pool[i].Entity(ECS_MaxEntities + 1);
				//std::swap(m_Pool[i], m_Pool[--m_FreeIndex]);
				return true;
			}
		}
		return false;
	}
}

#endif