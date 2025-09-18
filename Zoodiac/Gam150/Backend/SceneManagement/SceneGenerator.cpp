// ======================================================================
// File Name:		Scene_Generator.cpp
// Project Name:	Zoodiac
// Author(s):		Low Zhi Sheng Kitson(20%), Lim Geng Yang(20%),
//					Lye Pin Liang Xavier(20%), Muhammad Dzulhafiz(20%),
//					Ang Jie Le Jet(20%)
// Brief:			Contains Scene Generation functionality
// 
// All content © 2023 DigiPen Institute of Technology Singapore. All 
// rights reserved.
// ======================================================================

#include <AEEngine.h>
#include <Backend/Engine_Common.h>
#include <Backend/Utils.h>
#include <Backend/ECS/ECS_Factory.h>
#include <Backend/SceneManagement/SceneCommon.h>
#include <Backend/SceneManagement/SceneGenerator.h>

namespace Backend
{
	void SceneGenerator::GenerateWorld(const int _x, const int _y)
	{
		AEGfxSetCamPosition(0, 0);
		for (int x = -(_x / 2) - 1; x <= (_x/2); ++x)
		{
			for (int y = -(_y/2) - 1; y <= (_y/2); ++y)
			{
				const AEVec2 vec{ static_cast<float>(x * 30),static_cast<float>(y * 30) };
				if (x == -(_x / 2) - 1 || x == (_x / 2) || y == -(_y / 2) - 1 || y == (_y / 2))
					ECS::Factory::ECS_Factory::CreateBorderEntity(vec);
				else
					ECS::Factory::ECS_Factory::CreateTileEntity(vec, 0,
						(x / static_cast<float>(_x)), (y / static_cast<float>(_y)), (y / static_cast<float>(_y)), "grass");
			}
		}
		ECS::Factory::ECS_Factory::CreatePlayerEntity(false);
	}

	void SceneGenerator::GenerateQbertWorld(const int _x, const int _y)
	{
		AEGfxSetCamPosition(0, 0);
		for (int x = -(_x / 2) - 1; x <= (_x / 2); ++x)
		{
			for (int y = -(_y / 2) - 1; y <= (_y / 2); ++y)
			{
				const AEVec2 vec{ static_cast<float>(x * 30),static_cast<float>(y * 30) };
				if (x == -(_x / 2) - 1 || x == (_x / 2) || y == -(_y / 2) - 1 || y == (_y / 2))
					ECS::Factory::ECS_Factory::CreateBorderEntity(vec);
				else
				{
					ECS::Factory::ECS_Factory::CreateTileEntity(vec, x * 30.f + y * 30.f,
						(x / static_cast<float>(_x)), (y / static_cast<float>(_y)), (y / static_cast<float>(_y)), "grass");
					ECS::Factory::ECS_Factory::CreateTileEntity(vec, x * 30.f + y * 30.f - 30.0f,
						(x / static_cast<float>(_x)), (y / static_cast<float>(_y)), (y / static_cast<float>(_y)), "grass");
				}
			}
		}
		ECS::Factory::ECS_Factory::CreatePlayerEntity(false);
	}
	
	void SceneGenerator::GenerateClimbWorld(const int _x, const int _y, const int _h)
	{
		for (int y = 0, i = 0; i < _h; ++y,++i)
		{
			bool removed = false;
			for (int x = -(_x / 2) - 1; x <= (_x / 2); ++x)
			{
				const AEVec2 vec{ x * 30.f, y * 30.f };
				const auto entity =
					ECS::Factory::ECS_Factory::CreateTileEntity(vec, i * 30.f,
						1, 1, 1, "grass");
				if ((x == -(_x / 2) - 1) || (x == _x / 2))
				{
					ECS::Factory::ECS_Factory::CreateBorderEntity(vec);
					ECS::Factory::ECS_Factory::CreateTileEntity(vec, i * 30.f + 30.f,
						1, 1, 1, "Lfence");
				}
				else if (y == _y - 1)
				{
					auto& tile = ECS_WORLD.GetComponent<ECS::Component::TileComponent>(entity);
					tile.m_Tag = ECS::Component::TileComponent::TAG_WINTILE;
				}
				else if (i && !removed && (rand() % 5 < 2 || x == _x / 2 - 1))
				{
					removed = true;
					auto& tile = ECS_WORLD.GetComponent<ECS::Component::TileComponent>(entity);
					tile.m_Tag = ECS::Component::TileComponent::TAG_FALLINGGAME;
					tile.m_Timer = RandomRange(y * 0.25f, y * 0.25f + 0.25f);
					ECS::Factory::ECS_Factory::CreateDestroyerEntity(vec, -120, -150);
					auto& emit = ECS_WORLD.AddComponent<ECS::Component::EmitterComponent>(entity);
					emit.m_Emit = true;
					emit.m_Type = ECS::Component::EmitterComponent::FALL;
				}
			}
		}
		ECS::Factory::ECS_Factory::CreatePlayerEntity();
		ECS::Factory::ECS_Factory::CreateMinigameEntity(1, 1);
	}

	void SceneGenerator::GenerateEndlessRunnerWorld(const int _x, const int _y, const int _h)
	{	
		for (int x = -(_x / 2) - 1; x <= (_x / 2); ++x)
		{
			auto mult = 1;
			for (int y = 0, i = 0; y < _y; ++y)
			{
				const AEVec2 vec{30.f * x, 30.f * y};
				const auto entity = ECS::Factory::ECS_Factory::CreateTileEntity(vec, i * 30.f,
					1, 1, 1, "grass");
				const auto water = ECS::Factory::ECS_Factory::CreateTileEntity(vec, -90.f,
					1, 1, 1, "water");
				if((x == -(_x / 2) - 1) || (x == _x/2))
				{
					ECS::Factory::ECS_Factory::CreateBorderEntity(vec);
					ECS::Factory::ECS_Factory::CreateTileEntity(vec, 30.f * (i + 1),
						1, 1, 1, "Lfence");
				}
				else if(y == _y - 1)
				{
					auto& tile = ECS_WORLD.GetComponent<ECS::Component::TileComponent>(entity);
					tile.m_Tag = ECS::Component::TileComponent::TAG_WINTILE;
				}
				else if (y)
				{
					auto& tile = ECS_WORLD.GetComponent<ECS::Component::TileComponent>(entity);
					tile.m_Tag = ECS::Component::TileComponent::TAG_FALLINGGAME;
					tile.m_Timer = RandomRange(1 + (0.25f * y), 1 + (0.25f * y) + 0.25f);
					auto& emit = ECS_WORLD.AddComponent<ECS::Component::EmitterComponent>(entity);
					emit.m_Emit = true;
					emit.m_Type = ECS::Component::EmitterComponent::FALL;
					auto& destroyer = ECS_WORLD.GetComponent<ECS::Component::ColliderComponent>(water);
					destroyer.m_Tag = ECS::Component::ColliderComponent::TAG_DESTROYER_TILE;
				}

				if(!((y + 1) % 3))
					i += mult;
				if (i == _h)
					mult = -1;
				else if (i == 0)
					mult = 1;
			}
		}
		ECS::Factory::ECS_Factory::CreatePlayerEntity();
		ECS::Factory::ECS_Factory::CreateMinigameEntity(1, 1);
	}

	void SceneGenerator::GenerateDestroyer(const AEVec2 _min, const AEVec2 _max)
	{
		for (int x = static_cast<int>(_min.x); x < static_cast<int>(_max.x); ++x)
		{
			const AEVec2 vec{ static_cast<float>(x * 30),(_min.y - 1) * 30 };
			const AEVec2 vec2{ static_cast<float>(x * 30),(_max.y + 1) * 30 };
			ECS::Factory::ECS_Factory::CreateDestroyerEntity(vec);
			ECS::Factory::ECS_Factory::CreateDestroyerEntity(vec2);
		}
		for(int y = static_cast<int>(_min.y); y < static_cast<int>(_max.y); ++y)
		{
			const AEVec2 vec{ (_min.x - 1) * 30,static_cast<float>(y * 30) };
			const AEVec2 vec2{ (_max.x + 1) * 30,static_cast<float>(y * 30) };
			ECS::Factory::ECS_Factory::CreateDestroyerEntity(vec);
			ECS::Factory::ECS_Factory::CreateDestroyerEntity(vec2);
		}
	}
	
	void SceneGenerator::GenerateMainMenu()
	{
		ECS::Factory::ECS_Factory::CreateBackgroundScreen("menubackground");
		ECS::Factory::ECS_Factory::CreateTextEntity({-500, 325}, "ZOODIAC", 1.5f);
		ECS::Factory::ECS_Factory::CreateButtonEntity({ -525,-250 }, "PLAY", ECS::Component::UILogicComponent::TAG_MAINMENU_PLAY);
		ECS::Factory::ECS_Factory::CreateButtonEntity({ -275,-250 }, "TUTORIAL", ECS::Component::UILogicComponent::TAG_MAINMENU_TUTORIAL);
		ECS::Factory::ECS_Factory::CreateButtonEntity({ -25,-250 }, "CREDITS", ECS::Component::UILogicComponent::TAG_MAINMENU_CREDITS);
		ECS::Factory::ECS_Factory::CreateButtonEntity({ 225,-250 }, "OPTIONS", ECS::Component::UILogicComponent::TAG_MAINMENU_OPTIONS);
		ECS::Factory::ECS_Factory::CreateButtonEntity({ 475,-250 }, "EXIT", ECS::Component::UILogicComponent::TAG_MAINMENU_EXIT);
	}

	void SceneGenerator::GenerateCreditPage(int _pageNo)
	{
		ECS::Factory::ECS_Factory::CreateBackgroundScreen("menubackground");
		switch (_pageNo)
		{
		case 1:
			ECS::Factory::ECS_Factory::CreateTextEntity({ -300, 330 }, "ZOODIAC by WeekendWarriors", 1.3f);
			break;
		case 2:
			ECS::Factory::ECS_Factory::CreateTextEntity({ -280, 330 }, "Credits To Instructors Whom Worked With Us:", 1.0f);
			break;
		case 3:
			ECS::Factory::ECS_Factory::CreateTextEntity({ -200, 330 }, "Proudly Designed And Created At:", 1.0f);
			break;
		case 4:
			ECS::Factory::ECS_Factory::CreateTextEntity({ -100, 330 }, "CREDITS TO:", 1.0f);
			break;
		}
		ECS::Factory::ECS_Factory::CreateButtonEntity({ 650,-380 }, "NEXT ", static_cast<ECS::Component::UILogicComponent::Tag>(ECS::Component::UILogicComponent::TAG_CREDITS1_NEXT + (_pageNo - 1)));
		ECS::Factory::ECS_Factory::CreateButtonEntity({ -650, -380 }, "MENU ", ECS::Component::UILogicComponent::TAG_CREDITS_RETURNTOMAINMENU);
		ECS::Factory::ECS_Factory::CreateCreditScreen(_pageNo);
	}

	void SceneGenerator::GenerateCollectionMinigame(const int _x, const int _y)
	{
		constexpr unsigned spawnPerInterval = 1;
		GenerateQbertWorld(_x, _y);
		constexpr AEVec2 direction = { 0,0 };
		const AEVec2 minRange = { (-0.5f * _x), (-0.5f * _y) };
		const AEVec2 maxRange = { (0.5f * _x), (0.5f * _y) };
		ECS::Factory::ECS_Factory::CreateSpawnerEntity(
			ECS::Factory::ECS_Factory::CreateCheeseEntity, 
			minRange, maxRange, direction, spawnPerInterval, 0.5f);
		ECS::Factory::ECS_Factory::CreateMinigameEntity(5, 1, ECS::Component::MinigameComponent::Tag::TAG_TIMERLOSE, 20.0f);
	}

	void SceneGenerator::GenerateDodgingMinigame(const int _x, const int _y)
	{
		constexpr unsigned spawnPerInterval = 2;
		constexpr float timer = 2.0f;
		GenerateWorld(_x, _y);
		AEVec2 minRange = { _x + 6.0f, (_y * -0.5f) };
		AEVec2 maxRange = { _x + 6.0f, (_y * 0.5f) };
		AEVec2 direction = { -1,0 };
		ECS::Factory::ECS_Factory::CreateSpawnerEntity(
		ECS::Factory::ECS_Factory::CreateDodgingBlockEntity, minRange,
		    maxRange, direction, spawnPerInterval, timer);

		minRange = { -(_x + 6.0f), (_y * -0.5f) };
		maxRange = { -(_x + 6.0f), (_y * 0.5f) };
		direction = { 1,0 };
		ECS::Factory::ECS_Factory::CreateSpawnerEntity(
			ECS::Factory::ECS_Factory::CreateDodgingBlockEntity, 
			minRange, maxRange, direction, spawnPerInterval, timer);

		minRange = { (_x * -0.5f), _y + 6.0f };
		maxRange = { (_x * 0.5f), _y + 6.0f  };
		direction = { 0,-1 };
		ECS::Factory::ECS_Factory::CreateSpawnerEntity(
			ECS::Factory::ECS_Factory::CreateDodgingBlockEntity, 
			minRange, maxRange, direction, spawnPerInterval, timer);

		minRange = { (_x * -0.5f), -(_y + 6.0f) };
		maxRange = { (_x * 0.5f), -(_y + 6.0f) };
		direction = { 0,1 };
		ECS::Factory::ECS_Factory::CreateSpawnerEntity(
			ECS::Factory::ECS_Factory::CreateDodgingBlockEntity, 
			minRange, maxRange, direction, spawnPerInterval, timer);

		minRange = { -(_x + 6.0f), -(_y + 6.0f) };
		maxRange = { (_x + 6.0f), (_y + 6.0f) };
		GenerateDestroyer(minRange,maxRange);
		ECS::Factory::ECS_Factory::CreateMinigameEntity(10, 1, ECS::Component::MinigameComponent::Tag::TAG_TIMERWIN, 20);
	}
	
	void SceneGenerator::GenerateStaggeredDodgingMinigame(const int _x, const int _y)
	{	
		constexpr unsigned spawnPerInterval = 2;
		constexpr float timer = 4.0f;
		GenerateWorld(_x, _y);
		AEVec2 minRange = { _x + 6.0f, (_y * -0.5f) };
		AEVec2 maxRange = { _x + 6.0f, (_y * 0.5f) };
		AEVec2 direction = { -1,0 };
		ECS::Factory::ECS_Factory::CreateSpawnerEntity(ECS::Factory::ECS_Factory::CreateDodgingBlockEntity, minRange, maxRange, direction, spawnPerInterval, timer);

		minRange = { -(_x + 6.0f), (_y * -0.5f) };
		maxRange = { -(_x + 6.0f), (_y * 0.5f) };
		direction = { 1,0 };
		ECS::Factory::ECS_Factory::CreateSpawnerEntity(ECS::Factory::ECS_Factory::CreateDodgingBlockEntity, minRange, maxRange, direction, spawnPerInterval, timer, 1);

		minRange = { (_x * -0.5f), _y + 6.0f };
		maxRange = { (_x * 0.5f), _y + 6.0f };
		direction = { 0,-1 };
		ECS::Factory::ECS_Factory::CreateSpawnerEntity(ECS::Factory::ECS_Factory::CreateDodgingBlockEntity, minRange, maxRange, direction, spawnPerInterval, timer, 2);

		minRange = { (_x * -0.5f), -(_y + 6.0f) };
		maxRange = { (_x * 0.5f), -(_y + 6.0f) };
		direction = { 0,1 };
		ECS::Factory::ECS_Factory::CreateSpawnerEntity(ECS::Factory::ECS_Factory::CreateDodgingBlockEntity, minRange, maxRange, direction, spawnPerInterval, timer, 3);

		minRange = { -(_x + 6.0f), -(_y + 6.0f) };
		maxRange = { (_x + 6.0f), (_y + 6.0f) };
		GenerateDestroyer(minRange, maxRange);
		ECS::Factory::ECS_Factory::CreateMinigameEntity(10, 1, ECS::Component::MinigameComponent::Tag::TAG_TIMERWIN, 20);
	}
	
	void SceneGenerator::GenerateDodgingAndCollectMinigame(const int _x, const int _y)
	{	
		GenerateDodgingMinigame(_x, _y);
		constexpr unsigned spawnPerInterval = 3;
		constexpr AEVec2 direction = { 0,0 };
		const AEVec2 minRange = { (_x * -0.5f), (_y * -0.5f) };
		const AEVec2 maxRange = { (_x * 0.5f), (_y * 0.5f) };
		ECS::Factory::ECS_Factory::CreateSpawnerEntity(ECS::Factory::ECS_Factory::CreateCheeseEntity, minRange, maxRange, direction, spawnPerInterval, 2.f);
		ECS::Factory::ECS_Factory::CreateMinigameEntity(10, 1);
	}

	void SceneGenerator::GenerateLevelEditor()
	{
		ECS::Factory::ECS_Factory::CreatePlayerEntity();
	}

	void SceneGenerator::GenerateSplashscreen()
	{
		ECS::Factory::ECS_Factory::CreateSplashScreenEntity(0);
	}
}
