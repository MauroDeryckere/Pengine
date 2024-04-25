#include "FModSoundSystem.h"

namespace Pengin
{
	FModSoundSytem::FModSoundSytem()
	{
		FModSoundSytem::ErrorCheck(FMOD::Studio::System::create(&m_pStudio));
		FModSoundSytem::ErrorCheck(m_pStudio->initialize(32, FMOD_STUDIO_INIT_LIVEUPDATE, FMOD_INIT_PROFILE_ENABLE, NULL));
		FModSoundSytem::ErrorCheck(m_pStudio->getCoreSystem(&m_pSystem));
	}

	FModSoundSytem::~FModSoundSytem()
	{
		FModSoundSytem::ErrorCheck(m_pStudio->unloadAll());
		FModSoundSytem::ErrorCheck(m_pStudio->release());
	}

	void FModSoundSytem::Update() 
	{
		std::vector<ChannelMap::iterator> pStoppedChannels;
		for (auto it = m_Channels.begin(); it != m_Channels.end(); ++it)
		{
			bool isPlaying = false;
			it->second->isPlaying(&isPlaying);

			if (!isPlaying)
			{
				pStoppedChannels.emplace_back(it);
			}
		}

		for (auto& it : pStoppedChannels)
		{
			m_Channels.erase(it);
		}

		FModSoundSytem::ErrorCheck(m_pStudio->update());
	}

	void FModSoundSytem::LoadSound(const std::string& soundName, bool is3D, bool isLooping, bool isStream)
	{
		auto it{ m_Sounds.find(soundName) };

		if (it != end(m_Sounds))
		{
			return;
		}

		const FMOD_MODE mode = FMOD_DEFAULT | 
						(is3D ? FMOD_3D : FMOD_2D) | 
						(isLooping ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF) | 
						(isStream ? FMOD_CREATESTREAM : FMOD_CREATECOMPRESSEDSAMPLE);

		FMOD::Sound* pSound{ nullptr };

		FModSoundSytem::ErrorCheck(m_pSystem->createSound(soundName.c_str(), mode, nullptr, &pSound));

		if (pSound) 
		{
			m_Sounds[soundName] = pSound;
		}
	}

	void FModSoundSytem::UnLoadSound(const std::string& soundName)
	{
		auto it = m_Sounds.find(soundName);
		if (it == m_Sounds.end())
		{
			return;
		}

		FModSoundSytem::ErrorCheck(it->second->release());
		m_Sounds.erase(it);
	}

	const int FModSoundSytem::PlaySounds(const std::string& soundName, const glm::vec3& position, float volumedB)
	{
		int channelId{ m_NextChannelId++ };
		auto it{ m_Sounds.find(soundName) };

		if (it == end(m_Sounds)) //Loud sound if not loaded yet
		{
			LoadSound(soundName);
			it = m_Sounds.find(soundName);

			if (it == end(m_Sounds))
			{
				std::cout << "Error:loaded sound is not found in map, likely did not load correctly \n"; //Might need to throw here in future / handle error if we ever end up here
				return -1;
			}
		}

		FMOD::Channel* pChannel{ nullptr };
		//Play the sound paused first
		FModSoundSytem::ErrorCheck(m_pSystem->playSound(it->second, nullptr, true, &pChannel));

		if (pChannel)
		{
			FMOD_MODE currMode;
			it->second->getMode(&currMode);

			if (currMode & FMOD_3D) 
			{
				const FMOD_VECTOR fmodPosition{ VectorToFmod(position) };
				FModSoundSytem::ErrorCheck(pChannel->set3DAttributes(&fmodPosition, nullptr));
			}

			FModSoundSytem::ErrorCheck(pChannel->setVolume(DbToVolume(volumedB)));
			FModSoundSytem::ErrorCheck(pChannel->setPaused(false)); //Unpause the sound

			m_Channels[channelId] = pChannel;
		}

		return channelId;
	}

	void FModSoundSytem::SetChannel3DPosition(int channelId, const glm::vec3& position)
	{
		auto it{ m_Channels.find(channelId) };
		if (it == end(m_Channels))
		{
			return;
		}

		const FMOD_VECTOR fmodPosition{ VectorToFmod(position) };
		FModSoundSytem::ErrorCheck(it->second->set3DAttributes(&fmodPosition, NULL));
	}

	void FModSoundSytem::SetChannelVolume(int channelId, float volumedB)
	{
		auto tFoundIt = m_Channels.find(channelId);
		if (tFoundIt == m_Channels.end())
			return;

		FModSoundSytem::ErrorCheck(tFoundIt->second->setVolume(DbToVolume(volumedB)));
	}

	void FModSoundSytem::ErrorCheck(FMOD_RESULT result) 
	{
		if (result != FMOD_OK) 
		{
			std::cout << "FMOD ERROR: " << result << "\n";
		}
	}
}

//Comented functions from ref (need to be updated if used)

/*void AudioEngine::LoadBank(const std::string& strBankName, FMOD_STUDIO_LOAD_BANK_FLAGS flags) {
		auto tFoundIt = sgpImplementation->mBanks.find(strBankName);
		if (tFoundIt != sgpImplementation->mBanks.end())
			return;
		FMOD::Studio::Bank* pBank;
		AudioEngine::ErrorCheck(sgpImplementation->mpStudioSystem->loadBankFile(strBankName.c_str(), flags, &pBank));
		if (pBank) {
			sgpImplementation->mBanks[strBankName] = pBank;
		}
	}

	void AudioEngine::LoadEvent(const std::string& strEventName) {
		auto tFoundit = sgpImplementation->mEvents.find(strEventName);
		if (tFoundit != sgpImplementation->mEvents.end())
			return;
		FMOD::Studio::EventDescription* pEventDescription = NULL;
		AudioEngine::ErrorCheck(sgpImplementation->mpStudioSystem->getEvent(strEventName.c_str(), &pEventDescription));
		if (pEventDescription) {
			FMOD::Studio::EventInstance* pEventInstance = NULL;
			AudioEngine::ErrorCheck(pEventDescription->createInstance(&pEventInstance));
			if (pEventInstance) {
				sgpImplementation->mEvents[strEventName] = pEventInstance;
			}
		}
	}

	void AudioEngine::PlayEvent(const std::string& strEventName) {
		auto tFoundit = sgpImplementation->mEvents.find(strEventName);
		if (tFoundit == sgpImplementation->mEvents.end()) {
			LoadEvent(strEventName);
			tFoundit = sgpImplementation->mEvents.find(strEventName);
			if (tFoundit == sgpImplementation->mEvents.end())
				return;
		}
		tFoundit->second->start();
	}

	void AudioEngine::StopEvent(const std::string& strEventName, bool bImmediate) {
		auto tFoundIt = sgpImplementation->mEvents.find(strEventName);
		if (tFoundIt == sgpImplementation->mEvents.end())
			return;
		FMOD_STUDIO_STOP_MODE eMode;
		eMode = bImmediate ? FMOD_STUDIO_STOP_IMMEDIATE : FMOD_STUDIO_STOP_ALLOWFADEOUT;
		AudioEngine::ErrorCheck(tFoundIt->second->stop(eMode));
	}

	bool AudioEngine::IsEventPlaying(const std::string& strEventName) const {
		auto tFoundIt = sgpImplementation->mEvents.find(strEventName);
		if (tFoundIt == sgpImplementation->mEvents.end())
			return false;

		FMOD_STUDIO_PLAYBACK_STATE* state = NULL;
		if (static_cast<unsigned>(tFoundIt->second->getPlaybackState(state)) == static_cast<unsigned>(FMOD_STUDIO_PLAYBACK_PLAYING)) {
			return true;
		}
		return false;
	}

	void AudioEngine::GetEventParameter(const std::string& strEventName, const std::string& strParameterName, float* parameter) {
		auto tFoundIt = sgpImplementation->mEvents.find(strEventName);
		if (tFoundIt == sgpImplementation->mEvents.end())
			return;

		AudioEngine::ErrorCheck(tFoundIt->second->getParameterByName(strParameterName.c_str(), parameter));
	}

	void AudioEngine::SetEventParameter(const std::string& strEventName, const std::string& strParameterName, float fValue) {
		auto tFoundIt = sgpImplementation->mEvents.find(strEventName);
		if (tFoundIt == sgpImplementation->mEvents.end())
			return;

		AudioEngine::ErrorCheck(tFoundIt->second->setParameterByName(strParameterName.c_str(), fValue));
	}*/