// ======================================================================
// File Name:		AudioManager.h
// Project Name:	Zoodiac
// Author(s):		Low Zhi Sheng Kitson(50%), Ang Jie Le Jet(50%)
// Brief:			Contains Audio Manager functionality
// 
// All content © 2023 DigiPen Institute of Technology Singapore. All 
// rights reserved.
// ======================================================================

#ifndef AUDIO_MANAGER_H_
#define AUDIO_MANAGER_H_

#include <AEAudio.h>
#include <Backend/Singleton.h>

namespace Backend::Audio
{
	constexpr int InfiniteLoop = -1;

	class AudioManager : public Singleton<AudioManager>
	{
	public:

		enum Sound
		{
			JUMP_SOUND,
			OVERWORLD_MUSIC,
			WILDWORLD_MUSIC,
			MINIGAME_MAINMENU_MUSIC,
			BALLCOLLECT_SOUND,
			BIRD_SOUND,
			WATERSPLASH_SOUND,
			UIBUTTON_SOUND,
			FALLING_SOUND,
			GAMECOMPLETE_SOUND
		};

		/**
		 * \brief Constructor.
		 */
		AudioManager();

		/**
		 * \brief Destructor.
		 */
		~AudioManager() override = default;

		/**
		 * \brief Loads audio.
		 */
		void LoadAudio();

		/**
		 * \brief				Plays audio.
		 * \param _soundIndex	Index of the sound.
		 * \param _loops		Number of loops.
		 */
		void PlayAudio(Sound _soundIndex, s32 _loops) const;

		/**
		 * \brief Pauses all audio.
		 */
		void PauseAudio() const;

		/**
		 * \brief Resumes all audio.
		 */
		void ResumeAudio() const;

		/**
		 * \brief Stops all audio.
		 */
		void StopAudio() const;

	private:
		std::map<Scene, AEAudioGroup> m_SoundEffectsGroup;
		std::map<Sound, AEAudio> m_Sounds;
		float m_Volume = 0.05f;					//volume for BackGround 1.0f (100%)
		float m_EffectVolume = 0.50f;			//volume for sound effect
	};
}

#endif // SOUND_MANAGER_H_
