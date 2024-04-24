#ifndef AUDIOENGINE
#define AUDIOENGINE

#include <fmod_studio.hpp>
#include <fmod.hpp>

#include <string>
#include <math.h>
#include <map>
#include <vector>
#include <iostream>

#include <glm/vec3.hpp>

namespace Pengin
{
	struct Implementation final
	{
		Implementation();
		~Implementation();

		void Update();

		FMOD::Studio::System* mpStudioSystem;
		FMOD::System* mpSystem;

		int mnNextChannelId;

		using SoundMap = std::map<std::string, FMOD::Sound*>;
		using ChannelMap = std::map<int, FMOD::Channel*>;

		SoundMap mSounds;
		ChannelMap mChannels;

		//using EventMap = std::map<std::string, FMOD::Studio::EventInstance*>;
		//using BankMap = std::map<std::string, FMOD::Studio::Bank*>;
		//BankMap mBanks;
		//EventMap mEvents;
	};

	//Commented all functions not related to simple sound playing for now;

	class AudioEngine final
	{
	public:
		static void Init();
		static void Update();
		static void Shutdown();
		static int ErrorCheck(FMOD_RESULT result);

		//void LoadBank(const std::string& strBankName, FMOD_STUDIO_LOAD_BANK_FLAGS flags);
		//void LoadEvent(const std::string& strEventName);
		void LoadSound(const std::string& strSoundName, bool b3d = true, bool bLooping = false, bool bStream = false);
		void UnLoadSound(const std::string& strSoundName);
		//void Set3dListenerAndOrientation(const glm::vec3& vPosition, const glm::vec3& vLook, const glm::vec3& vUp);
		
		//Returns the channelId
		int PlaySounds(const std::string& strSoundName, const glm::vec3& vPos = { 0, 0, 0 }, float fVolumedB = 0.0f);

		//void PlayEvent(const std::string& strEventName);
		//void StopChannel(int nChannelId);
		//void StopEvent(const std::string& strEventName, bool bImmediate = false);
		//void GetEventParameter(const std::string& strEventName, const std::string& strEventParameter, float* parameter);
		//void SetEventParameter(const std::string& strEventName, const std::string& strParameterName, float fValue);
		//void StopAllChannels();
		void SetChannel3dPosition(int nChannelId, const glm::vec3& vPosition);
		void SetChannelVolume(int nChannelId, float fVolumedB);
		//bool IsPlaying(int nChannelId) const;
		//bool IsEventPlaying(const std::string& strEventName) const;
		float dbToVolume(float dB);
		float VolumeTodB(float volume);
		FMOD_VECTOR VectorToFmod(const glm::vec3& vPosition);
	};
}

#endif 