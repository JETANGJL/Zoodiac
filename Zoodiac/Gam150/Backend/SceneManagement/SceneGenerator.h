// ======================================================================
// File Name:		Scene_Generator.h
// Project Name:	Zoodiac
// Author(s):		Low Zhi Sheng Kitson(20%), Lim Geng Yang(20%),
//					Lye Pin Liang Xavier(20%), Muhammad Dzulhafiz(20%),
//					Ang Jie Le Jet(20%)
// Brief:			Contains Scene Generation functionality
// 
// All content © 2023 DigiPen Institute of Technology Singapore. All 
// rights reserved.
// ======================================================================

#ifndef SCENE_GENERATOR_H_
#define SCENE_GENERATOR_H_

namespace Backend
{
	/**
	 * \brief	Scene generator class
	 */
	class SceneGenerator
	{
	public:
		/**
		 * \brief		Generates the world.
		 * \param _x	X of the world (length).
		 * \param _y	Y of the world (width).
		 */
		static auto GenerateWorld(int _x, int _y) -> void;
		/**
		 * \brief		Generates the world for collection minigame.
		 * \param _x	X of the world (length).
		 * \param _y	Y of the world (width).
		 */
		static auto GenerateQbertWorld(const int _x, const int _y) -> void;
		/**
		 * \brief		Generates the world for climbing minigame.
		 * \param _x	X of the world (length).
		 * \param _y	Y of the world (width).
		 * \param _h	height of the world.
		 */
		static auto GenerateClimbWorld(int _x, int _y, int _h) -> void;
		/**
		 * \brief		Generates the world for runner minigame.
		 * \param _x	X of the world (length).
		 * \param _y	Y of the world (width).
		 * \param _h	height of the world.
		 */
		static auto GenerateEndlessRunnerWorld(int _x, int _y, int _h)->void;
		/**
		 * \brief		Generates a single or mulitple Destroyer entities for desctruction of minigame entities.
		 * \param _min	The Minimum coordinate range.
		 * \param _max	The Maximum coordinate range.
		 */
		static auto GenerateDestroyer(AEVec2 _min, AEVec2 _max) -> void;
		/**
		 * \brief		Generates the Main Menu.
		 */
		static auto GenerateMainMenu() -> void;
		/**
		 * \brief			Generates the Credits.
		 * \param _pageNo	The number of pages for Credits.
		 */
		static auto GenerateCreditPage(int _pageNo) -> void;
		/**
		 * \brief		Generates the Splash Screen.
		 */
		static auto GenerateSplashscreen() -> void;
		/**
		 * \brief		Generates a collection minigame.
		 * \param _x	X of the world (length).
		 * \param _y	Y of the world (width).
		 */
		static auto GenerateCollectionMinigame(int _x, int _y) -> void;
		/**
		 * \brief		Generates the Dodging minigame.
		 * \param _x	X of the world (length).
		 * \param _y	Y of the world (width).
		 */
		static auto GenerateDodgingMinigame(int _x, int _y) -> void;
		/**
		 * \brief		Generates the Staggered Dodging minigame.
		 * \param _x	X of the world (length).
		 * \param _y	Y of the world (width).
		 */
		static auto GenerateStaggeredDodgingMinigame(int _x, int _y) -> void;
		/**
		 * \brief		Generates the Dodging and collection minigame.
		 * \param _x	X of the world (length).
		 * \param _y	Y of the world (width).
		 */
		static auto GenerateDodgingAndCollectMinigame(int _x, int _y) -> void;

		/**
		 * \brief		Generates level editor.
		 */
		static auto GenerateLevelEditor() -> void;
	};
}

#endif