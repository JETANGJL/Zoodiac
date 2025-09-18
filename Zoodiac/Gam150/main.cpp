// ======================================================================
// File Name:		main.cpp
// Project Name:	Zoodiac
// Author(s):		Low Zhi Sheng Kitson(60%), Muhammad Dzulhafiz(10%),
//					Ang Jie Le Jet(10%), Lye Pin Liang Xavier(10%),
//					Lim Geng Yang(10%)
// Brief:			Contains State Functionality
// 
// All content � 2023 DigiPen Institute of Technology Singapore. All 
// rights reserved.
// ======================================================================


#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>

#include <AEEngine.h>
#include <iostream>
#include <vector>
#include <Backend/Engine_Common.h>
#include <Backend/JSONSerializer.h>
#include <Backend/Utils.h>
#include <Backend/ECS/ECS.h>
#include <Backend/Sound/AudioManager.h>
#include <States/StateAngry.h>
#include <States/StateHappy.h>
#include <States/StateIdle.h>
#include <States/StateWild.h>

void InitAnimationLibrary()
{
	ANIMATION_LIBRARY.AddResource("playerd", new std::vector<std::string>{ "Assets/sprites/playerD1.png", "Assets/sprites/playerD2.png" });
	ANIMATION_LIBRARY.AddResource("playeru", new std::vector<std::string>{ "Assets/sprites/playerU1.png", "Assets/sprites/playerU2.png" });
	ANIMATION_LIBRARY.AddResource("playerl", new std::vector<std::string>{ "Assets/sprites/playerL1.png", "Assets/sprites/playerL2.png" });
	ANIMATION_LIBRARY.AddResource("playerr", new std::vector<std::string>{ "Assets/sprites/playerR1.png", "Assets/sprites/playerR2.png" });
	ANIMATION_LIBRARY.AddResource("dogd", new std::vector<std::string>{ "Assets/sprites/dogD1.png", "Assets/sprites/dogD2.png" });
	ANIMATION_LIBRARY.AddResource("dogu", new std::vector<std::string>{ "Assets/sprites/dogU1.png", "Assets/sprites/dogU2.png" });
	ANIMATION_LIBRARY.AddResource("dogl", new std::vector<std::string>{ "Assets/sprites/dogL1.png", "Assets/sprites/dogL2.png" });
	ANIMATION_LIBRARY.AddResource("dogr", new std::vector<std::string>{ "Assets/sprites/dogR1.png", "Assets/sprites/dogR2.png" });

	ANIMATION_LIBRARY.AddResource("cowd", new std::vector<std::string>{ "Assets/sprites/cowR1.png", "Assets/sprites/cowR2.png" });
	ANIMATION_LIBRARY.AddResource("cowu", new std::vector<std::string>{ "Assets/sprites/cowL1.png", "Assets/sprites/cowL2.png" });
	ANIMATION_LIBRARY.AddResource("cowl", new std::vector<std::string>{ "Assets/sprites/cowL1.png", "Assets/sprites/cowL2.png" });
	ANIMATION_LIBRARY.AddResource("cowr", new std::vector<std::string>{ "Assets/sprites/cowR1.png", "Assets/sprites/cowR2.png" });
	
	ANIMATION_LIBRARY.AddResource("splash", new std::vector<std::string>{ "Assets/UI/DigiPen_BLACK.png", "Assets/UI/FMODLogoWhite.png" });

	ANIMATION_LIBRARY.AddResource("monkeyd", new std::vector<std::string>{ "Assets/sprites/monkeyD1.png", "Assets/sprites/monkeyD2.png" });
	ANIMATION_LIBRARY.AddResource("monkeyu", new std::vector<std::string>{ "Assets/sprites/monkeyU1.png", "Assets/sprites/monkeyU2.png" });
	ANIMATION_LIBRARY.AddResource("monkeyl", new std::vector<std::string>{ "Assets/sprites/monkeyL1.png", "Assets/sprites/monkeyL2.png" });
	ANIMATION_LIBRARY.AddResource("monkeyr", new std::vector<std::string>{ "Assets/sprites/monkeyR1.png", "Assets/sprites/monkeyR2.png" });

	ANIMATION_LIBRARY.AddResource("ratd", new std::vector<std::string>{ "Assets/sprites/ratR1.png", "Assets/sprites/ratR2.png" });
	ANIMATION_LIBRARY.AddResource("ratu", new std::vector<std::string>{ "Assets/sprites/ratL1.png", "Assets/sprites/ratL2.png" });
	ANIMATION_LIBRARY.AddResource("ratl", new std::vector<std::string>{ "Assets/sprites/ratL1.png", "Assets/sprites/ratL2.png" });
	ANIMATION_LIBRARY.AddResource("ratr", new std::vector<std::string>{ "Assets/sprites/ratR1.png", "Assets/sprites/ratR2.png" });

	ANIMATION_LIBRARY.AddResource("chickend", new std::vector<std::string>{ "Assets/sprites/chickenR1.png", "Assets/sprites/chickenR2.png" });
	ANIMATION_LIBRARY.AddResource("chickenu", new std::vector<std::string>{ "Assets/sprites/chickenL1.png", "Assets/sprites/chickenL2.png" });
	ANIMATION_LIBRARY.AddResource("chickenl", new std::vector<std::string>{ "Assets/sprites/chickenL1.png", "Assets/sprites/chickenL2.png" });
	ANIMATION_LIBRARY.AddResource("chickenr", new std::vector<std::string>{ "Assets/sprites/chickenR1.png", "Assets/sprites/chickenR2.png" });
}


void InitStateLibrary()
{
	STATE_LIBRARY.AddResource(StateType::STATE_IDLE, new StateIdle());
	STATE_LIBRARY.AddResource(StateType::STATE_ANGRY, new StateAngry());
	STATE_LIBRARY.AddResource(StateType::STATE_HAPPY, new StateHappy());
	STATE_LIBRARY.AddResource(StateType::STATE_WILD, new StateWild());
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE,
                      _In_ LPWSTR,
                      _In_ int       nCmdShow){
	SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	int gGameRunning = 1;
	bool gamePause = false;

	AESysInit(hInstance, nCmdShow, Backend::WindowWidth, Backend::WindowHeight, 0, Backend::MaxFrameRate, true, nullptr);
	AESysToggleFullScreen(false);
	AESysSetWindowTitle(Backend::GameName);
	AESysReset();
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetBackgroundColor(0.52f, 0.80f, 0.98f);
	InitAnimationLibrary();
	ECS_WORLD.InitializeECS();
	FONT_LIBRARY.SetDestructor(&Backend::DestroyFont);
	TEXTURE_LIBRARY.SetDestructor(&AEGfxTextureUnload);
	FONT_LIBRARY.AddResource("a", new s8(AEGfxCreateFont("Assets/UI/Nice Sugar.ttf", 50)));
	InitStateLibrary();
	AUDIO_MANAGER.LoadAudio();
	DEPENDENCY_INJECTOR.RegisterDependency(Backend::Scene::NUM_SCENE, Backend::DependencyInjector::PAUSE, &gamePause);
	DEPENDENCY_INJECTOR.RegisterDependency(Backend::Scene::NUM_SCENE, Backend::DependencyInjector::EXIT, &gGameRunning);

	while (gGameRunning)
	{
		AESysFrameStart();
		AEInputUpdate();
		AEAudioUpdate();
		ECS_WORLD.UpdateECS();
		AESysFrameEnd();
	}
	AESysExit();
	return 0;
}
