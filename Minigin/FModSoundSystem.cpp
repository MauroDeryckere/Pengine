#include "FModSoundSystem.h"

#include "DebugOutput.h"

namespace Pengin
{
	FModSoundSytem::FModSoundSytem()
	{
		ErrorCheck(FMOD::Studio::System::create(&m_pStudio));

		ErrorCheck(m_pStudio->initialize(512, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, NULL));
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
		//Erase any stopped channels
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
				m_Sounds[it->first] = it->second;
				m_LoadingSounds.erase(it);
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

	void FModSoundSytem::PlaySound(const SoundData& soundData) noexcept
	{
		assert(std::filesystem::exists(soundData.soundPath) && "Sound path invalid, doesn't exist");

		if (auto loadedIt{ m_Sounds.find(soundData.soundPath.string()) }; loadedIt != end(m_Sounds))
		{
			PlaySoundImpl(loadedIt->second, soundData);
		}
		else if (auto loadingIt{ m_LoadingSounds.find(soundData.soundPath.string()) }; loadingIt != end(m_LoadingSounds))
		{
			m_LoadingRequests.emplace_back(soundData);
		}
		else
		{
			LoadSoundImpl(soundData);
			m_LoadingRequests.emplace_back(soundData);
		}
	}

	void FModSoundSytem::SetChannel3DPosition(const GameUUID& id, const glm::vec3& position) noexcept
	{
		auto it{ m_Channels.find(id) };
		if (it == end(m_Channels))
		{
			DEBUG_OUT("Channel for id: " << id.GetUUID_PrettyStr() << "does not exist");
			return;
		}

		const FMOD_VECTOR fmodPosition{ VectorToFmod(position) };
		ErrorCheck(it->second->set3DAttributes(&fmodPosition, NULL));
	}

	void FModSoundSytem::SetChannelVolume(const GameUUID& id, float volumedB) noexcept
	{
		auto it = m_Channels.find(id);
		if (it == end(m_Channels))
		{
			DEBUG_OUT("Channel for id: " << id.GetUUID_PrettyStr() << "does not exist");
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

	void FModSoundSytem::PlaySoundImpl(FMOD::Sound* pSound, const SoundData& soundData) noexcept
	{
		FMOD::Channel* pChannel{ nullptr };

		//Play the sound paused first
		ErrorCheck(m_pSystem->playSound(pSound, nullptr, true, &pChannel));
		
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

			m_Channels[soundData.soundUUID] = pChannel;
		}
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