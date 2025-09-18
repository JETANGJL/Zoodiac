// ======================================================================
// File Name:		ECS_Common.h
// Project Name:	Zoodiac
// Author(s):		Low Zhi Sheng Kitson(80%), Lim Geng Yang(5%),
//					Lye Pin Liang Xavier(5%), Muhammad Dzulhafiz(5%),
//					Ang Jie Le Jet(5%)
// Brief:			Contains ECS values and types
// 
// All content © 2023 DigiPen Institute of Technology Singapore. All 
// rights reserved.
// ======================================================================

#ifndef ECS_COMMON_H_
#define ECS_COMMON_H_

#include <bitset>

#define ECS_WORLD Backend::ECS::ECS_World::GetInstance() 

namespace Backend::ECS
{
	using ECS_Entity = std::uint64_t;
	constexpr ECS_Entity ECS_MaxEntities = 25000;
	using ECS_ComponentType = std::uint32_t;
	constexpr ECS_ComponentType ECS_MaxComponents = 22;
	using ECS_Signature = std::bitset<ECS_MaxComponents>;
}

#endif