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
		//Erase any stopped channels
		for (auto it{ m_Channels.begin() }; it != m_Channels.end(); )
		{
			auto& channelVec{ it->second };

			std::erase_if(channelVec, [](auto& ptr) 
				{
					bool isPlaying{ false };
					ptr->isPlaying(&isPlaying);
					return !isPlaying;
				});

			channelVec.empty() ? it = m_Channels.erase(it) : ++it;
		}
		//-------------------------

		//Requests for souns that are loading
		{
			std::vector<size_t> executedReqs{};
			for (size_t idx{ 0 }; idx < m_LoadingRequests.size(); ++idx)
			{
				auto it = m_LoadingSounds.find(m_LoadingRequests[idx].soundPath.string());
				assert(it != end(m_LoadingSounds));

				FMOD::Sound* pSound{ it->second };
				assert(pSound);

				FMOD_OPENSTATE openstate;
				ErrorCheck(pSound->getOpenState(&openstate, 0, 0, 0));

				if (openstate == FMOD_OPENSTATE::FMOD_OPENSTATE_READY || 
					openstate == FMOD_OPENSTATE::FMOD_OPENSTATE_PLAYING)  //Either has to be ready or currently playing to be allowed to be played
				{
					executedReqs.emplace_back(idx);
					PlaySoundImpl(pSound, m_LoadingRequests[idx]);
				}
			}

			for (auto it = executedReqs.rbegin(); it != executedReqs.rend(); ++it)
			{
				m_LoadingRequests.erase(m_LoadingRequests.begin() + *it);
			}
		}
		//-------------------------
		
		//Check if the currently loading sounds have been loaded, and if so add them to the loaded map
		for (auto it = m_LoadingSounds.begin(); it != m_LoadingSounds.end(); )
		{
			FMOD::Sound* pSound = it->second;

			FMOD_OPENSTATE openstate;
			ErrorCheck(pSound->getOpenState(&openstate, 0, 0, 0));

			if (openstate == FMOD_OPENSTATE::FMOD_OPENSTATE_READY)
			{
				m_Sounds[it->first] = pSound;
				it = m_LoadingSounds.erase(it);
			}
			else
			{
				++it;
			}
		}
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

	void FModSoundSytem::LoadSound(const SoundData& soundData) noexcept
	{
		assert(std::filesystem::exists(soundData.soundPath) && "Sound path invalid, doesn't exist");

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

	const ChannelIndex FModSoundSytem::PlaySound(const SoundData& soundData) noexcept
	{
		assert(std::filesystem::exists(soundData.soundPath) && "Sound path invalid, doesn't exist");

		if (auto loadedIt{ m_Sounds.find(soundData.soundPath.string()) }; loadedIt != end(m_Sounds))
		{
			return PlaySoundImpl(loadedIt->second, soundData);
		}
		else if (auto loadingIt{ m_LoadingSounds.find(soundData.soundPath.string()) }; loadingIt != end(m_LoadingSounds))
		{
			m_LoadingRequests.emplace_back(soundData);
			return INVALID_CHANNEL_IDX;
		}
		else
		{
			LoadSoundImpl(soundData);
			m_LoadingRequests.emplace_back(soundData);
			return INVALID_CHANNEL_IDX;
		}
	}

	void FModSoundSytem::SetAllChannels3DPosition(const GameUUID& id, const glm::vec3& position) noexcept
	{
		auto it{ m_Channels.find(id) };
		if (it == end(m_Channels))
		{
			DEBUG_OUT("Channel for id: " << id.GetUUID_PrettyStr() << "does not exist");
			return;
		}

		const FMOD_VECTOR fmodPosition{ VectorToFmod(position) };

		auto& channelVec = it->second;
		for (auto channel : channelVec)
		{
			ErrorCheck(channel->set3DAttributes(&fmodPosition, NULL));
		}
	}

	void FModSoundSytem::SetAllChannelsVolume(const GameUUID& id, float volume) noexcept
	{
		auto it = m_Channels.find(id);
		if (it == end(m_Channels))
		{
			DEBUG_OUT("Channel for id: " << id.GetUUID_PrettyStr() << "does not exist");
			return;
		}

		auto& channelVec = it->second;
		for (auto channel : channelVec)
		{
			ErrorCheck(channel->setVolume(volume));
		}
	}

	void FModSoundSytem::SetChannel3DPosition(const GameUUID& id, ChannelIndex idx, const glm::vec3& position) noexcept
	{
		if (idx < 0)
		{
			DEBUG_OUT("Invalid channel index for id: " << id.GetUUID_PrettyStr());
			return;
		}

		auto it = m_Channels.find(id);
		if (it == end(m_Channels))
		{
			DEBUG_OUT("Channel for id: " << id.GetUUID_PrettyStr() << "does not exist");
			return;
		}
		
		auto& vec{ it->second };
		if (idx >= vec.size())
		{
			DEBUG_OUT("Channel for id: " << id.GetUUID_PrettyStr() << "does out of bounds (invalid, sound likely already removed)");
			return;
		}

		const FMOD_VECTOR fmodPosition{ VectorToFmod(position) };
		ErrorCheck(vec[idx]->set3DAttributes(&fmodPosition, NULL));
	}

	void FModSoundSytem::SetChannelVolume(const GameUUID& id, ChannelIndex idx, float volume) noexcept
	{
		if (idx < 0)
		{
			DEBUG_OUT("Invalid channel index for id: " << id.GetUUID_PrettyStr());
			return;
		}

		auto it = m_Channels.find(id);
		if (it == end(m_Channels))
		{
			DEBUG_OUT("Channel for id: " << id.GetUUID_PrettyStr() << "does not exist");
			return;
		}

		auto& vec{ it->second };
		if (idx >= vec.size())
		{
			DEBUG_OUT("Channel for id: " << id.GetUUID_PrettyStr() << "does out of bounds (invalid, sound likely already removed)");
			return;
		}

		ErrorCheck(vec[idx]->setVolume(volume));
	}

	void FModSoundSytem::SetVFXVolume(const float vol) noexcept
	{
		ErrorCheck(m_pVFXGroup->setVolume(vol));
	}

	void FModSoundSytem::SetMusicVolume(const float vol) noexcept
	{
		ErrorCheck(m_pMusicGroup->setVolume(vol));
	}

	void FModSoundSytem::ErrorCheck(FMOD_RESULT result) const noexcept
	{
		if (result != FMOD_OK) 
		{
			std::cout << "FMOD ERROR: " << result << "\n";
		}
	}

	const ChannelIndex FModSoundSytem::PlaySoundImpl(FMOD::Sound* pSound, const SoundData& soundData) noexcept
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
			FMOD_MODE currMode;
			pSound->getMode(&currMode);

			if (currMode & FMOD_3D)
			{
				const FMOD_VECTOR fmodPosition{ VectorToFmod(soundData.position) };
				ErrorCheck(pChannel->set3DAttributes(&fmodPosition, nullptr));
			}

			ErrorCheck(pChannel->setVolume(soundData.volume));
			ErrorCheck(pChannel->setPaused(false)); //Unpause the sound


			auto& vec = m_Channels[soundData.soundUUID];
			vec.emplace_back(pChannel);
			return static_cast<ChannelIndex>(vec.size() - 1);
		}

		return INVALID_CHANNEL_IDX;
	}

	void FModSoundSytem::LoadSoundImpl(const SoundData& soundData) noexcept
	{
		const FMOD_MODE flags =
			FMOD_DEFAULT
			| (soundData.is3D ? FMOD_3D : FMOD_2D)
			| (soundData.isLooping ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF)
			| (soundData.isStream ? FMOD_CREATESTREAM : FMOD_CREATECOMPRESSEDSAMPLE)
			| FMOD_NONBLOCKING; //Loading on a separate thread

		FMOD::Sound* pSound{ nullptr };
		ErrorCheck(m_pSystem->createSound(soundData.soundPath.string().c_str(), flags, nullptr, &pSound));

		if (pSound)
		{
			FMOD_OPENSTATE openstate;
			pSound->getOpenState(&openstate, 0, 0, 0);

			if (openstate != FMOD_OPENSTATE::FMOD_OPENSTATE_READY)
			{
				m_LoadingSounds[soundData.soundPath.string()] = pSound;
			}
			else
			{
				m_Sounds[soundData.soundPath.string()] = pSound;
			}
		}
	}
}