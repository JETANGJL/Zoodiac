// ======================================================================
// File Name:		ECS_EntityStack.cpp
// Project Name:	Zoodiac
// Author(s):		Low Zhi Sheng Kitson(100%)
// Brief:			Contains ECS Entity Stack functionality
// 
// All content © 2023 DigiPen Institute of Technology Singapore. All 
// rights reserved.
// ======================================================================

#include <cassert>

#include <Backend/ECS/ECS_EntityStack.h>

namespace Backend::ECS
{
    ECS_EntityStack::ECS_EntityStack()
    {
        for (ECS_Entity i = 0; i < ECS_MaxEntities; ++i)
            m_FreeStack.push(i);
    }

    auto ECS::ECS_EntityStack::GetEntity() -> ECS_Entity
    {
        assert(!m_FreeStack.empty());

        const ECS_Entity entity = m_FreeStack.top();
        m_InUseEntites.push_back(entity);
        m_FreeStack.pop();
        return entity;
    }

    auto ECS_EntityStack::FreeEntity(const ECS_Entity _entity) -> void
    {
        std::erase_if(m_InUseEntites,
                      [&](const ECS_Entity _i) -> bool
                      {
	                      if (_i == _entity)
	                      {
		                      m_FreeStack.push(_entity);
		                      return true;
	                      }
	                      return false;
                      });
    }

    auto ECS_EntityStack::GetLast() const -> ECS_Entity
    {
        return *(m_InUseEntites.end() - 1);
    }
}
