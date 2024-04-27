#include "FModSoundSystem.h"

#include "DebugOutput.h"

namespace Pengin
{
	FModSoundSytem::FModSoundSytem()
	{
		ErrorCheck(FMOD::Studio::System::create(&m_pStudio));
		ErrorCheck(m_pStudio->initialize(32, FMOD_STUDIO_INIT_LIVEUPDATE, FMOD_INIT_PROFILE_ENABLE, NULL));
		ErrorCheck(m_pStudio->getCoreSystem(&m_pSystem));
		ErrorCheck(m_pSystem->getMasterChannelGroup(&m_MasterGroup));
	}

	FModSoundSytem::~FModSoundSytem()
	{
		ErrorCheck(m_pStudio->unloadAll());
		ErrorCheck(m_pStudio->release());
	}

	void FModSoundSytem::Update() noexcept
	{
		
		//erase any stopped channels
		{
			std::vector<ChannelMap::iterator> pStoppedChannels{};
			for (auto it = begin(m_Channels); it != end(m_Channels); ++it)
			{
				bool isPlaying{ false };
				it->second->isPlaying(&isPlaying); //isplaying is still true even if paused (see. FMod API)

				if (!isPlaying)
				{
					pStoppedChannels.emplace_back(it);
				}
			}

			for (auto& it : pStoppedChannels)
			{
				m_Channels.erase(it);
			}
		}
		//-------------------------

		//Currently loading requests
		{
			std::vector<size_t> executedReqs{};
			for (size_t idx{ 0 }; idx < m_LoadingRequests.size(); ++idx)
			{
				auto it = m_LoadingSounds.find(m_LoadingRequests[idx].soundPath.string());
				assert(it != end(m_LoadingSounds));

				FMOD::Sound* pSound{ it->second };
				assert(pSound);

				FMOD_OPENSTATE openstate;
				pSound->getOpenState(&openstate, 0, 0, 0);

				if (openstate == FMOD_OPENSTATE::FMOD_OPENSTATE_READY)
				{
					executedReqs.emplace_back(idx);
					m_LoadingSounds.erase(it);

					PlaySound(pSound, m_LoadingRequests[idx]);

					m_Sounds[m_LoadingRequests[idx].soundPath.string()] = pSound;
				}
			}

			for (auto it = executedReqs.rbegin(); it != executedReqs.rend(); ++it)
			{
				m_LoadingRequests.erase(m_LoadingRequests.begin() + *it);
			}
		}
		//-------------------------

		//Check if anything was loaded that was not requested to be played
		{
			std::vector<LoadingMap::iterator> pLoadedSounds{};
			for (auto it = begin(m_LoadingSounds); it != end(m_LoadingSounds); ++it)
			{
				FMOD::Sound* pSound{ it->second };
				assert(pSound);

				FMOD_OPENSTATE openstate;
				pSound->getOpenState(&openstate, 0, 0, 0);

				if (openstate == FMOD_OPENSTATE::FMOD_OPENSTATE_READY)
				{
					pLoadedSounds.emplace_back(it);
					m_Sounds[it->first] = pSound;
				}
			}

			for (auto& it : pLoadedSounds)
			{
				m_LoadingSounds.erase(it);
			}
		}
		//-------------------------

		//Currently loading sounds that have been cancelled
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

		DEBUG_OUT("trying to unload a sound that was never loaded: " << soundPath.string());
	}

	const int32_t FModSoundSytem::PlaySound(const SoundData& soundData) noexcept
	{
		auto loadedIt{ m_Sounds.find(soundData.soundPath.string()) };

		if (loadedIt == end(m_Sounds)) //if sound is not loaded yet
		{
			//Is sound not currently loading
			if (auto loadingIt{ m_LoadingSounds.find(soundData.soundPath.string()) }; loadingIt == end(m_LoadingSounds))
			{
				LoadSoundImpl(soundData);
			}
			
			m_LoadingRequests.emplace_back(soundData);
			return -1;
		}

		return PlaySound(loadedIt->second, soundData);
	}

	void FModSoundSytem::SetChannel3DPosition(int32_t channelId, const glm::vec3& position) noexcept
	{
		auto it{ m_Channels.find(channelId) };
		if (it == end(m_Channels))
		{
			return;
		}

		const FMOD_VECTOR fmodPosition{ VectorToFmod(position) };
		FModSoundSytem::ErrorCheck(it->second->set3DAttributes(&fmodPosition, NULL));
	}

	void FModSoundSytem::SetChannelVolume(int32_t channelId, float volumedB) noexcept
	{
		auto it = m_Channels.find(channelId);

		if (it == end(m_Channels))
		{
			return;
		}

		ErrorCheck(it->second->setVolume(DbToVolume(volumedB)));
	}

	void FModSoundSytem::ErrorCheck(FMOD_RESULT result) const noexcept
	{
		if (result != FMOD_OK) 
		{
			std::cout << "FMOD ERROR: " << result << "\n";
		}
	}
	const int32_t FModSoundSytem::PlaySound(FMOD::Sound* pSound, const SoundData& soundData) noexcept
	{
		FMOD::Channel* pChannel{ nullptr };

		//Play the sound paused first
		ErrorCheck(m_pSystem->playSound(pSound, nullptr, true, &pChannel));

		int32_t channelId{ m_NextChannelId++ };
		if (pChannel)
		{
			FMOD_MODE currMode;
			pSound->getMode(&currMode);

			if (currMode & FMOD_3D)
			{
				const FMOD_VECTOR fmodPosition{ VectorToFmod(soundData.position) };
				ErrorCheck(pChannel->set3DAttributes(&fmodPosition, nullptr));
			}

			ErrorCheck(pChannel->setVolume(DbToVolume(soundData.volumedB)));
			ErrorCheck(pChannel->setPaused(false)); //Unpause the sound

			m_Channels[channelId] = pChannel;
		}

		return channelId;
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