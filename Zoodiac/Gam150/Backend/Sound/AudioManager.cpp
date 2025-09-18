// ======================================================================
// File Name:		AudioManager.cpp
// Project Name:	Zoodiac
// Author(s):		Low Zhi Sheng Kitson(50%), Ang Jie Le Jet(50%)
// Brief:			Contains Audio Manager functionality
// 
// All content © 2023 DigiPen Institute of Technology Singapore. All 
// rights reserved.
// ======================================================================


#include <AEEngine.h>
#include <Backend/Engine_Common.h>
#include <Backend/Utils.h>
#include <Backend/Sound/AudioManager.h>
#include <Backend/ECS/ECS.h>

namespace Backend::Audio
{

	AudioManager::AudioManager()
	{
		for (unsigned int scene = 0; scene < Scene::NUM_SCENE ; ++scene)
			m_SoundEffectsGroup.emplace(static_cast<Scene>(scene), AEAudioCreateGroup());
		for (const auto& i : m_SoundEffectsGroup)
			AEAudioSetGroupVolume(i.second, 0.5);
	}

	void AudioManager::LoadAudio()
	{
		m_Sounds.emplace(Sound::JUMP_SOUND, AEAudioLoadSound("Assets/sound/jump.wav"));
		m_Sounds.emplace(Sound::OVERWORLD_MUSIC, AEAudioLoadSound("Assets/sound/Music_Overworld.ogg"));
		m_Sounds.emplace(Sound::WILDWORLD_MUSIC, AEAudioLoadSound("Assets/sound/Music_Wildworld.ogg"));
		m_Sounds.emplace(Sound::MINIGAME_MAINMENU_MUSIC, AEAudioLoadSound("Assets/sound/Music_MinigameorMainmenu.ogg"));
		m_Sounds.emplace(Sound::BALLCOLLECT_SOUND, AEAudioLoadSound("Assets/sound/Sound_Ballcollect.wav"));
		m_Sounds.emplace(Sound::BIRD_SOUND, AEAudioLoadSound("Assets/sound/Sound_Birds.ogg"));
		m_Sounds.emplace(Sound::WATERSPLASH_SOUND, AEAudioLoadSound("Assets/sound/Water_Splash.wav"));
		m_Sounds.emplace(Sound::UIBUTTON_SOUND, AEAudioLoadSound("Assets/sound/Ui_Button.wav"));
		m_Sounds.emplace(Sound::FALLING_SOUND, AEAudioLoadSound("Assets/sound/Falling.wav"));
		m_Sounds.emplace(Sound::GAMECOMPLETE_SOUND, AEAudioLoadSound("Assets/sound/Ta Da.wav"));
	}

	void AudioManager::PlayAudio(Sound _soundIndex, s32 _loops) const
	{
		AEAudioPlay(m_Sounds.at(_soundIndex), m_SoundEffectsGroup.at(ECS_WORLD.GetCurrentScene()), m_EffectVolume, 1.0f, _loops);
	}

	void AudioManager::PauseAudio() const
	{
		AEAudioPauseGroup(m_SoundEffectsGroup.at(ECS_WORLD.GetCurrentScene()));
	}

	void AudioManager::ResumeAudio() const
	{
		AEAudioResumeGroup(m_SoundEffectsGroup.at(ECS_WORLD.GetCurrentScene()));
	}

	void AudioManager::StopAudio() const
	{
		AEAudioStopGroup(m_SoundEffectsGroup.at(ECS_WORLD.GetCurrentScene()));
	}
}
