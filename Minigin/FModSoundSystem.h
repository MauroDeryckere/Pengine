#ifndef FMODSOUNDSYSTEM
#define FMODSOUNDSYSTEM

#include "SoundSystem.h"
#include "SoundData.h"

#include <fmod_studio.hpp>
#include <fmod.hpp>

#include <string>
#include <map>
#include <unordered_map>
#include <vector>

#include <glm/vec3.hpp>

//Ref: https://codyclaborn.me/tutorials/making-a-basic-fmod-audio-engine-in-c/
//	   https://www.fmod.com/docs/2.03/api/welcome.html

/*
Audio Engine TODO:
- Pausing all
- Support velocity for 3D Sound
- Stop playing a Sound

- FMOD Event / bank support
- Reverb support
- Listener && Update the listener (3D audio)
- Effects

- ImGUI debug window
*/

namespace Pengin
{
	class FModSoundSytem final : public SoundSystem
	{
	public:
		FModSoundSytem();
		virtual ~FModSoundSytem() override;

		void Update() noexcept;

		void LoadSound(const SoundData& soundData) noexcept;
		void UnLoadSound(const std::filesystem::path& soundPath) noexcept;

		//It is important to load the sound first if you want a valid channel idx
		const ChannelId PlaySound(const SoundData& soundData) noexcept;

		void SetVFXVolume(const float vol) noexcept;
		void SetMusicVolume(const float vol) noexcept;

		void MuteAll() noexcept;
		void UnmuteAll() noexcept;
		[[nodiscard]] bool IsMuted() const noexcept;

		void SetChannel3DPosition(const ChannelId& id, const glm::vec3& position) noexcept;
		void SetChannelVolume(const ChannelId& id, float volume) noexcept;

		FModSoundSytem(const FModSoundSytem&) = delete;
		FModSoundSytem(FModSoundSytem&&) = delete;
		FModSoundSytem& operator=(const FModSoundSytem&) = delete;
		FModSoundSytem& operator=(const FModSoundSytem&&) = delete;

	private:
		//Loaded sounds
		using SoundMap = std::unordered_map<std::string, FMOD::Sound*>;
		//In use channels
		using ChannelMap = std::unordered_map<GameUUID, FMOD::Channel*>;
		//The sounds that are being loaded
		using LoadingMap = std::unordered_map<std::string, FMOD::Sound*>;
		//Requests for sounds that are currently being loaded
		using RequestVec = std::vector<SoundData>;
		//Sounds have to be fully loaded before releasing, if a user decides to cancel it, we have to maintain a vector of anything to be released
		using ToReleaseVec = std::vector<FMOD::Sound*>;

		FMOD::Studio::System* m_pStudio{ nullptr };
		FMOD::System* m_pSystem{ nullptr }; //Core API

		FMOD::ChannelGroup* m_MasterGroup{ nullptr };
		FMOD::ChannelGroup* m_pVFXGroup{ nullptr };
		FMOD::ChannelGroup* m_pMusicGroup{ nullptr };

		bool m_IsMuted{ false };

		SoundMap m_Sounds{};
		ChannelMap m_Channels{};

		LoadingMap m_LoadingSounds{};
		RequestVec m_LoadingRequests{};
		ToReleaseVec m_ToRelVec{};

		void ErrorCheck(FMOD_RESULT result) const noexcept;

		[[nodiscard]] constexpr FMOD_VECTOR VectorToFmod(const glm::vec3& vPosition) const noexcept
		{
			return FMOD_VECTOR{ vPosition.x, vPosition.y, vPosition.z };
		}

		//Allows playing the sound without additional map lookups
		const ChannelId PlaySoundImpl(FMOD::Sound* pSound, const SoundData& soundData) noexcept;
		//Allows loading without additional checks
		void LoadSoundImpl(const SoundData& soundData) noexcept;


		//TODO add support for these systems
		//using EventMap = std::map<std::string, FMOD::Studio::EventInstance*>;
		//using BankMap = std::map<std::string, FMOD::Studio::Bank*>;

		//BankMap mBanks;
		//EventMap mEvents;
	};
}

#endif 