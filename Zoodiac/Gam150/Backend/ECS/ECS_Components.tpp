// ======================================================================
// File Name:		ECS_Component.tpp
// Project Name:	Zoodiac
// Author(s):		Low Zhi Sheng Kitson(100%)
// Brief:			Contains ECS Component functionality
// 
// All content © 2023 DigiPen Institute of Technology Singapore. All 
// rights reserved.
// ======================================================================

#ifndef _ECS_COMPONENTS_TPP
#define _ECS_COMPONENTS_TPP

namespace Backend::ECS
{
	template <typename Component>
	auto ECS_Component::GetComponentID(const ECS_ComponentType _id) -> ECS_ComponentType
	{ 
		static_assert(std::is_base_of_v<ECS_Component, Component>, "RegisterComponent must contain template argument of type Component");

		static ECS_ComponentType id = 0;
		if (!id)
			id = _id;
		return id;
	}

	template <typename Component>
	ECS_ComponentType ECS_Component::GetBitsetIndex()
	{
		static_assert(std::is_base_of_v<ECS_Component, Component>, "RegisterComponent must contain template argument of type Component");

		static ECS_ComponentType bitsetIndex = GetComponentID<Component>() - 1;
		return bitsetIndex;
	}
}

#endif