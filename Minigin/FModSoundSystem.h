#ifndef FMODSOUNDSYSTEM
#define FMODSOUNDSYSTEM

#include "SoundSystem.h"

#include <fmod_studio.hpp>
#include <fmod.hpp>

#include <string>
#include <map>
#include <vector>
#include <iostream>

#include <glm/vec3.hpp>


namespace Pengin
{
	class FModSoundSytem final : public SoundSystem
	{
	public:
		FModSoundSytem();
		virtual ~FModSoundSytem() override;

		virtual void Update() override;
		virtual void LoadSound(const std::string& soundName, bool is3D = true, bool isLooping = false, bool isStream = false) override;
		virtual void UnLoadSound(const std::string& soundName) override;

		//Returns the channelId in case specific changes to volume,... have to be made after loading	
		virtual const int PlaySounds(const std::string& soundName, const glm::vec3& position = { 0, 0, 0 }, float volumedB = 0.f) override; 

		virtual void SetChannel3DPosition(int channelId, const glm::vec3& position) override;
		virtual void SetChannelVolume(int channelId, float volumedB) override;

		FModSoundSytem(const FModSoundSytem&) = delete;
		FModSoundSytem(FModSoundSytem&&) = delete;
		FModSoundSytem& operator=(const FModSoundSytem&) = delete;
		FModSoundSytem& operator=(const FModSoundSytem&&) = delete;

	private:
		FMOD::Studio::System* m_pStudio{ nullptr };
		FMOD::System* m_pSystem{ nullptr };

		int m_NextChannelId;

		//Currently loaded sounds
		using SoundMap =	std::map<std::string, FMOD::Sound*>;

		//Currently in use channels
		using ChannelMap =	std::map<int32_t, FMOD::Channel*>;

		SoundMap m_Sounds{};
		ChannelMap m_Channels{};
		//It could also be a possibility to maintain a reverse mapping here / a relation between sound and channel



		//using EventMap = std::map<std::string, FMOD::Studio::EventInstance*>;
		//using BankMap = std::map<std::string, FMOD::Studio::Bank*>;
		 
		//BankMap mBanks;
		//EventMap mEvents;

		void ErrorCheck(FMOD_RESULT result);

		[[nodiscard]] constexpr FMOD_VECTOR VectorToFmod(const glm::vec3& vPosition) const noexcept
		{
			return FMOD_VECTOR{ vPosition.x, vPosition.y, vPosition.z };
		}
	};

	//Commented all functions not related to simple sound playing for now - can add these if more complex sound is required later on
	//Ref: https://codyclaborn.me/tutorials/making-a-basic-fmod-audio-engine-in-c/

	//void LoadBank(const std::string& strBankName, FMOD_STUDIO_LOAD_BANK_FLAGS flags);
	//void LoadEvent(const std::string& strEventName);
	//void Set3dListenerAndOrientation(const glm::vec3& vPosition, const glm::vec3& vLook, const glm::vec3& vUp);
	//void PlayEvent(const std::string& strEventName);
	//void StopChannel(int nChannelId);
	//void StopEvent(const std::string& strEventName, bool bImmediate = false);
	//void GetEventParameter(const std::string& strEventName, const std::string& strEventParameter, float* parameter);
	//void SetEventParameter(const std::string& strEventName, const std::string& strParameterName, float fValue);
	//void StopAllChannels();
	//bool IsPlaying(int nChannelId) const;
	//bool IsEventPlaying(const std::string& strEventName) const;
}

#endif 