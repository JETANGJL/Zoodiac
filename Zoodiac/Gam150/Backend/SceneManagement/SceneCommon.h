// ======================================================================
// File Name:		Scene_Common.h
// Project Name:	Zoodiac
// Author(s):		Low Zhi Sheng Kitson(20%), Lim Geng Yang(20%),
//					Lye Pin Liang Xavier(20%), Muhammad Dzulhafiz(20%),
//					Ang Jie Le Jet(20%)
// Brief:			Contains Scene Enum and information
// 
// All content © 2023 DigiPen Institute of Technology Singapore. All 
// rights reserved.
// ======================================================================

#ifndef SCENE_COMMON_H_
#define SCENE_COMMON_H_

namespace Backend
{
	/**
	 * \brief	Scene enum
	 */
	enum Scene
	{
		SCENE_MAINMENU,
		SCENE_TUTORIAL,
		SCENE_OVERWORLD,
		SCENE_WILDLANDS,
		SCENE_MINIGAME_COLLECTION,
		SCENE_MINIGAME_CLIMBING,
		SCENE_MINIGAME_DODGING,
		SCENE_MINIGAME_RUNNER,
		SCENE_MINIGAME_STAGGEREDODGING,
		SCENE_MINIGAME_DODGECOLLECTION,
		SCENE_CREDIT1,
		SCENE_CREDIT2,
		SCENE_CREDIT3,
		SCENE_CREDIT4,
		SCENE_OPTIONS,
		SCENE_SPLASHSCREEN,

		SCENE_LEVELEDITOR,
		
		NUM_SCENE
	};

	constexpr uint32_t MaxScene = NUM_SCENE; //	Max number of scenes
}

#endif