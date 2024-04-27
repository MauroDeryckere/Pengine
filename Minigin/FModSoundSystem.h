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

namespace Pengin
{
	class FModSoundSytem final : public SoundSystem
	{
	public:
		FModSoundSytem();
		virtual ~FModSoundSytem() override;

		virtual void Update() noexcept override;

		virtual void LoadSound(const SoundData& soundData) noexcept override;
		virtual void UnLoadSound(const std::filesystem::path& soundPath) noexcept override;

		//-1 == invalid
		//Returns the channelId in case specific changes to volume,... have to be made after loading	
		virtual const int32_t PlaySound(const SoundData& soundData) noexcept override;

		virtual void SetChannel3DPosition(int32_t channelId, const glm::vec3& position) noexcept override;
		virtual void SetChannelVolume(int32_t channelId, float volumedB) noexcept override;

		virtual void MuteAll() noexcept override
		{
			ErrorCheck(m_MasterGroup->setMute(true));
			m_IsMuted = true;
		}

		virtual void UnmuteAll() noexcept override
		{
			ErrorCheck(m_MasterGroup->setMute(false));
			m_IsMuted = false;
		}

		virtual [[nodiscard]] bool IsMuted() const noexcept override
		{
			return m_IsMuted; 
		}

		FModSoundSytem(const FModSoundSytem&) = delete;
		FModSoundSytem(FModSoundSytem&&) = delete;
		FModSoundSytem& operator=(const FModSoundSytem&) = delete;
		FModSoundSytem& operator=(const FModSoundSytem&&) = delete;

	private:
		//Loaded sounds
		using SoundMap = std::map<std::string, FMOD::Sound*>;
		//In use channels
		using ChannelMap = std::map<int32_t, FMOD::Channel*>;
		//The sounds that are being loaded
		using LoadingMap = std::map<std::string, FMOD::Sound*>;
		//Requests for sounds that are currently being loaded
		using RequestVec = std::vector<SoundData>;
		//Sounds have to be fully loaded before releasing, if a user decides to cancel it, we have to maintain a vector of anyhting to be released
		using ToReleaseVec = std::vector<FMOD::Sound*>;


		FMOD::Studio::System* m_pStudio{ nullptr };
		FMOD::System* m_pSystem{ nullptr }; //Core API
		FMOD::ChannelGroup* m_MasterGroup{ nullptr };
		bool m_IsMuted{ false };

		int32_t m_NextChannelId{}; //Simple counter

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
		const int32_t PlaySound(FMOD::Sound* pSound, const SoundData& soundData) noexcept;
		//Allows loading without additional checks
		void LoadSoundImpl(const SoundData& soundData) noexcept;

		//TODO add support for theese systems
		//using EventMap = std::map<std::string, FMOD::Studio::EventInstance*>;
		//using BankMap = std::map<std::string, FMOD::Studio::Bank*>;

		//BankMap mBanks;
		//EventMap mEvents;
	};
}

#endif 