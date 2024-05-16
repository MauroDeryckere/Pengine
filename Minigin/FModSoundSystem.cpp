#include "FModSoundSystem.h"

#include "DebugOutput.h"

namespace Pengin
{
	FModSoundSytem::FModSoundSytem()
	{
		ErrorCheck(FMOD::Studio::System::create(&m_pStudio));

		constexpr int MAX_CHANNELS{ 512 };

		ErrorCheck(m_pStudio->initialize(MAX_CHANNELS, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, NULL));
		ErrorCheck(m_pStudio->getCoreSystem(&m_pSystem));
		ErrorCheck(m_pSystem->getMasterChannelGroup(&m_MasterGroup));

		m_pSystem->createChannelGroup("VFX", &m_pVFXGroup);
		m_pSystem->createChannelGroup("Music", &m_pMusicGroup);
	}

	FModSoundSytem::~FModSoundSytem()
	{
		ErrorCheck(m_pStudio->unloadAll());
		ErrorCheck(m_pStudio->release());
	}

	void FModSoundSytem::Update() noexcept
	{
		//Requests for souns that are loading
		{
			std::vector<size_t> executedReqs{};
			for (size_t idx{ 0 }; idx < m_LoadingRequests.size(); ++idx)
			{
				if ( m_Sounds.find( m_LoadingRequests[idx].soundPath.string() ) != end(m_Sounds))
				{
					//play
					FMOD::Sound* pSound{ m_Sounds.at(m_LoadingRequests[idx].soundPath.string())};
					PlaySoundImpl(pSound, m_LoadingRequests[idx]);

					executedReqs.emplace_back(idx);
				}
			}

			for (auto it = executedReqs.rbegin(); it != executedReqs.rend(); ++it)
			{
				m_LoadingRequests.erase(m_LoadingRequests.begin() + *it);
			}
		}
		//-------------------------
		
		//-------------------------

		//Currently Loading sounds that have been cancelled
		{
			std::vector<size_t> loaded{};
			for (size_t idx{ 0 }; idx < m_ToRelVec.size(); ++idx)
			{
				FMOD_OPENSTATE openstate;
				m_ToRelVec[idx]->getOpenState(&openstate, 0, 0, 0);

				if (openstate == FMOD_OPENSTATE::FMOD_OPENSTATE_READY)
				{
					m_ToRelVec[idx]->release();
					loaded.emplace_back(idx);
				}
			}
			for (auto it = loaded.rbegin(); it != loaded.rend(); ++it)
			{
				m_ToRelVec.erase(m_ToRelVec.begin() + *it);
			}
		}
		//---------------------------------------------------

		//Underlying system and core Update
		ErrorCheck(m_pStudio->update());
	}

	void FModSoundSytem::LoadSound(SoundData& soundData) noexcept
	{
		assert(std::filesystem::exists(soundData.soundPath) && "Sound path invalid, doesn't exist");

		if (soundData.isStream)
		{
			LoadSoundImpl(soundData);
			return;
		}


		if (m_Sounds.find(soundData.soundPath.string()) != end(m_Sounds))
		{
			DEBUG_OUT(soundData.soundPath.string() << " Already loaded");
			return;
		}
		else if (m_LoadingSounds.find(soundData.soundPath.string()) != end(m_LoadingSounds))
		{
			DEBUG_OUT(soundData.soundPath.string() << " Is already loading");
			return;
		}

		LoadSoundImpl(soundData);
	}

	void FModSoundSytem::UnLoadSound(const std::filesystem::path& soundPath) noexcept
	{
		if (auto it = m_Sounds.find(soundPath.string()); it != m_Sounds.end())
		{
			ErrorCheck(it->second->release());
			m_Sounds.erase(it);
			return;
		}

		if (auto it = m_LoadingSounds.find(soundPath.string()); it != m_LoadingSounds.end())
		{
			FMOD_OPENSTATE openstate;
			it->second->getOpenState(&openstate, 0, 0, 0);

			if (openstate == FMOD_OPENSTATE::FMOD_OPENSTATE_READY)
			{
				m_LoadingSounds.erase(it);

				//If the sound has open requests and has to be unloaded
				std::erase_if(m_LoadingRequests, [&](const SoundData& soundData) 
					{
						return soundData.soundPath.string() == soundPath.string();
					});
			}
			else //Not done loading yet, release when loaded
			{
				m_ToRelVec.emplace_back(it->second);

				m_LoadingSounds.erase(it);

				//But remove the requests already
				std::erase_if(m_LoadingRequests, [&](const SoundData& soundData)
					{
						return soundData.soundPath.string() == soundPath.string();
					});
			}

			return;
		}

		DEBUG_OUT("Warning: trying to unload a sound that was never loaded: " << soundPath.string());
	}

	const ChannelId FModSoundSytem::PlaySound(SoundData& soundData) noexcept
	{
		assert(std::filesystem::exists(soundData.soundPath) && "Sound path invalid, doesn't exist");

		if (auto loadedIt{ m_Sounds.find(soundData.soundPath.string()) }; loadedIt != end(m_Sounds))
		{
			return PlaySoundImpl(loadedIt->second, soundData);
		}
		else if (auto loadingIt{ m_LoadingSounds.find(soundData.soundPath.string()) }; loadingIt != end(m_LoadingSounds))
		{
			m_LoadingRequests.emplace_back(soundData);
			return GameUUID::INVALID_UUID;
		}
		else
		{
			LoadSoundImpl(soundData);
			m_LoadingRequests.emplace_back(soundData);
			return GameUUID::INVALID_UUID;
		}
	}

	void FModSoundSytem::SetChannel3DPosition(const GameUUID& id, const glm::vec3& position) noexcept
	{
		assert(id);

		auto it = m_Channels.find(id);
		if (it == end(m_Channels))
		{
			DEBUG_OUT("Channel for id: " << id.GetUUID_PrettyStr() << "does not exist");
			return;
		}

		const FMOD_VECTOR fmodPosition{ VectorToFmod(position) };
		ErrorCheck(it->second->set3DAttributes(&fmodPosition, NULL));
	}

	void FModSoundSytem::SetChannelVolume(const GameUUID& id, float volume) noexcept
	{
		assert(id);

		auto it = m_Channels.find(id);
		if (it == end(m_Channels))
		{
			DEBUG_OUT("Channel for id: " << id.GetUUID_PrettyStr() << "does not exist");
			return;
		}

		ErrorCheck(it->second->setVolume(volume));
	}

	void FModSoundSytem::SetVFXVolume(const float vol) noexcept
	{
		ErrorCheck(m_pVFXGroup->setVolume(vol));
	}

	void FModSoundSytem::SetMusicVolume(const float vol) noexcept
	{
		ErrorCheck(m_pMusicGroup->setVolume(vol));
	}

	void FModSoundSytem::MuteAll() noexcept
	{
		ErrorCheck(m_MasterGroup->setMute(true));
		m_IsMuted = true;
	}

	void FModSoundSytem::UnmuteAll() noexcept
	{
		ErrorCheck(m_MasterGroup->setMute(false));
		m_IsMuted = false;
	}

	bool FModSoundSytem::IsMuted() const noexcept
	{
		return m_IsMuted;
	}

	void FModSoundSytem::ErrorCheck(FMOD_RESULT result) const noexcept
	{
		if (result != FMOD_OK) 
		{
			std::cout << "FMOD ERROR: " << result << "\n";
		}
	}

	const ChannelId FModSoundSytem::PlaySoundImpl(FMOD::Sound* pSound, SoundData& soundData) noexcept
	{
		FMOD::Channel* pChannel{ nullptr };

		switch (soundData.soundType)
		{
		case SoundData::SoundType::VFX:
			ErrorCheck(m_pSystem->playSound(pSound, m_pVFXGroup, true, &pChannel));
			break;
		case SoundData::SoundType::Music:
			ErrorCheck(m_pSystem->playSound(pSound, m_pMusicGroup, true, &pChannel));
			break;
		default:
			break;
		}
		
		if (pChannel)
		{
			auto channelId = ChannelId{};

			ErrorCheck(pChannel->setCallback(&ChannelControlCallback));

			auto* pair = new std::pair<FModSoundSytem*, ChannelId>{ this, channelId };
			ErrorCheck(pChannel->setUserData(pair));

			FMOD_MODE currMode{};
			ErrorCheck(pSound->getMode(&currMode));

			if (currMode & FMOD_3D)
			{
				const FMOD_VECTOR fmodPosition{ VectorToFmod(soundData.position) };
				ErrorCheck(pChannel->set3DAttributes(&fmodPosition, nullptr));
			}

			ErrorCheck(pChannel->setVolume(soundData.volume));
			ErrorCheck(pChannel->setPaused(false)); //Unpause the sound

			m_Channels[channelId] = pChannel;
			return channelId;
		}

		return GameUUID::INVALID_UUID;
	}

	void FModSoundSytem::LoadSoundImpl(SoundData& soundData) noexcept
	{
		const FMOD_MODE flags =
			FMOD_DEFAULT
			| (soundData.is3D ? FMOD_3D : FMOD_2D)
			| (soundData.isLooping ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF)
			| (soundData.isStream ? FMOD_CREATESTREAM : FMOD_CREATECOMPRESSEDSAMPLE)
			| FMOD_NONBLOCKING; //Loading on a separate thread

		FMOD::Sound* pSound{ nullptr };
		FMOD_CREATESOUNDEXINFO exinfo;

		memset(&exinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));
		exinfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
		exinfo.nonblockcallback = &NonBlockLoadCallback;

		auto* pair = new std::pair<FModSoundSytem*, SoundData> {this, soundData };
		exinfo.userdata = static_cast<void*>(pair);

		ErrorCheck(m_pSystem->createSound(soundData.soundPath.string().c_str(), flags, &exinfo, &pSound));

		if (pSound)
		{
			if (soundData.isStream)
			{
				//add to stream set
			}
			else
			{
				m_LoadingSounds[soundData.soundPath.string()] = pSound;
			}
		}
	}
}