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

		ErrorCheck(m_pSystem->createChannelGroup("VFX", &m_pVFXGroup));
		ErrorCheck(m_pSystem->createChannelGroup("Music", &m_pMusicGroup));
	}

	FModSoundSytem::~FModSoundSytem()
	{
		ErrorCheck(m_pStudio->unloadAll());
		ErrorCheck(m_pStudio->release());
	}

	void FModSoundSytem::Update() noexcept
	{
		//Erase any stopped channels
		std::erase_if(m_Channels, [this](auto& pair)
		{
			bool isPlaying{ false };
			pair.second->isPlaying(&isPlaying);

			//FMOD::Sound* pSound{ nullptr };
			//pair.second->getCurrentSound(&pSound);

			//if (!pSound)
			//{
			//	return true;
			//}

			//if (!isPlaying)
			//{
			//	FMOD_MODE mode;
			//	pSound->getMode(&mode);

			//	if (mode & FMOD_CREATESTREAM)
			//	{
			//		m_SoundsToRelease.emplace_back(pSound);
			//	}
			//}

			return !isPlaying;
		});
		//-------------------------

		std::cout << "channels: " << m_Channels.size() << "\n";
		std::cout << "streams: " << m_AllocatedStreams.size() << "\n";

		{
			std::vector<size_t> executedReqs{};
			for (size_t idx = 0; auto & str : m_AllocatedStreams)
			{
				FMOD_OPENSTATE state;
				str->getOpenState(&state, 0, 0, 0);

				if (state == FMOD_OPENSTATE_READY)
				{
					DEBUG_OUT("release stream");
					str->release(); //release == fmod err here
					executedReqs.emplace_back(idx);
				}

				++idx;
			}

			for (auto it = executedReqs.rbegin(); it != executedReqs.rend(); ++it)
			{
				m_AllocatedStreams.erase(m_AllocatedStreams.begin() + *it);
			}
		}

		//Streams
		{
			std::vector<size_t> executedReqs{};
			for (size_t reqIdx{ 0 }; reqIdx < m_StreamPlayRequests.size(); ++reqIdx)
			{
				const auto& path = m_StreamPlayRequests[reqIdx].soundPath.string();

				auto it = m_LoadingStreams.find(path);
				if (it == end(m_LoadingStreams)) //means request has been cancelled
				{
					executedReqs.emplace_back(reqIdx);
					continue;
				}

				auto& vec = it->second;

				for (size_t idx{ 0 }; idx < vec.size(); ++idx)
				{
					FMOD_OPENSTATE openstate;
					ErrorCheck(vec[idx]->getOpenState(&openstate, 0, 0, 0));

					if (openstate == FMOD_OPENSTATE::FMOD_OPENSTATE_READY)
					{
						PlaySoundImpl(vec[idx], m_StreamPlayRequests[reqIdx]);
						m_AllocatedStreams.emplace_back(vec[idx]);

						executedReqs.emplace_back(reqIdx);

						vec.erase(std::begin(vec) + idx);

						if (vec.empty())
						{
							m_LoadingStreams.erase(it);
						}

						break;
					}
				}
			}

			for (auto it = executedReqs.rbegin(); it != executedReqs.rend(); ++it)
			{
				m_StreamPlayRequests.erase(m_StreamPlayRequests.begin() + *it);
			}
		}
		//-------------------------


		//Requests for sounds that are loading
		{
			std::vector<size_t> executedReqs{};
			for (size_t idx{ 0 }; idx < m_SoundPlayRequests.size(); ++idx)
			{
				auto it = m_LoadingSounds.find(m_SoundPlayRequests[idx].soundPath.string());
				
				if (it == end(m_LoadingSounds))
				{
					//means the sound was unloaded, requests are cancelled.
					executedReqs.emplace_back(idx);
					continue;
				}

				FMOD::Sound* pSound{ it->second };
				assert(pSound);

				FMOD_OPENSTATE openstate;
				ErrorCheck(pSound->getOpenState(&openstate, 0, 0, 0));

				if (openstate == FMOD_OPENSTATE::FMOD_OPENSTATE_READY || 
					openstate == FMOD_OPENSTATE::FMOD_OPENSTATE_PLAYING)  //Either has to be ready or currently playing to be allowed to be played
				{
					executedReqs.emplace_back(idx);
					PlaySoundImpl(pSound, m_SoundPlayRequests[idx]);
				}
			}

			for (auto it = executedReqs.rbegin(); it != executedReqs.rend(); ++it)
			{
				m_SoundPlayRequests.erase(m_SoundPlayRequests.begin() + *it);
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

		//Sounds that have to be released
		{
			std::vector<size_t> idxesToRemove{};

			//std::lock_guard<std::mutex> lock{ m_SoundsReleaseMutex };
			for (size_t idx{ 0 }; idx < m_SoundsToRelease.size(); ++idx)
			{
				FMOD_OPENSTATE openstate;
				ErrorCheck(m_SoundsToRelease[idx]->getOpenState(&openstate, 0, 0, 0));

				if (openstate == FMOD_OPENSTATE::FMOD_OPENSTATE_READY) //Only free when ready to avoid stalls
				{
					ErrorCheck(m_SoundsToRelease[idx]->release());
					idxesToRemove.emplace_back(idx);
				}
			}
			for (auto it = idxesToRemove.rbegin(); it != idxesToRemove.rend(); ++it)
			{
				m_SoundsToRelease.erase(m_SoundsToRelease.begin() + *it);
			}
		}
		//---------------------------------------------------

		//Underlying system and core Update
		ErrorCheck(m_pStudio->update());
	}

	void FModSoundSytem::LoadSound(const SoundData& soundData) noexcept
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

		if (m_LoadingSounds.find(soundData.soundPath.string()) != end(m_LoadingSounds))
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
			ErrorCheck(it->second->getOpenState(&openstate, 0, 0, 0));

			if (openstate == FMOD_OPENSTATE::FMOD_OPENSTATE_READY)
			{
				it->second->release();
			}
			else //Not done loading yet, release when loaded
			{
				//std::lock_guard<std::mutex> lock{ m_SoundsReleaseMutex };
				m_SoundsToRelease.emplace_back(it->second);
			}

			m_LoadingSounds.erase(it);
			return;
		}

		bool found{ false };
		if (auto it = m_Streams.find(soundPath.string()); it != m_Streams.end())
		{
			auto& vec{ it->second };

			for (auto& snd : vec)
			{
				FMOD_OPENSTATE openstate;
				snd->getOpenState(&openstate, 0, 0, 0);

				if (openstate == FMOD_OPENSTATE::FMOD_OPENSTATE_READY)
				{
					snd->release();
				}
				else
				{
					//std::lock_guard<std::mutex> lock{ m_SoundsReleaseMutex };
					m_SoundsToRelease.emplace_back(snd);
				}
			}

			m_Streams.erase(it);
			found = true;
		}
		if (auto it = m_LoadingStreams.find(soundPath.string()); it != m_LoadingStreams.end())
		{
			auto& vec{ it->second };

			for (auto& snd : vec)
			{
				FMOD_OPENSTATE openstate;
				snd->getOpenState(&openstate, 0, 0, 0);

				if (openstate == FMOD_OPENSTATE::FMOD_OPENSTATE_READY)
				{
					snd->release();
				}
				else
				{
					//std::lock_guard<std::mutex> lock{ m_SoundsReleaseMutex };
					m_SoundsToRelease.emplace_back(snd);
				}
			}

			m_LoadingStreams.erase(it);
			found = true;
		}

		if (!found)
		{
			DEBUG_OUT("Warning: trying to unload a sound that was never loaded: " << soundPath.string());
		}
	}

	const ChannelId FModSoundSytem::PlaySound(const SoundData& soundData) noexcept
	{
		assert(std::filesystem::exists(soundData.soundPath) && "Sound path invalid, doesn't exist");

		if (soundData.isStream)
		{
			if (auto loadedIt{ m_Streams.find(soundData.soundPath.string()) }; loadedIt != end(m_Streams))
			{
				const auto id = PlaySoundImpl(loadedIt->second.back(), soundData);

				m_AllocatedStreams.emplace_back(loadedIt->second.back());

				loadedIt->second.pop_back();

				if (loadedIt->second.empty())
				{
					m_Streams.erase(loadedIt);
				}

				return id;
			}

			LoadSoundImpl(soundData);
			m_StreamPlayRequests.emplace_back(soundData);

			return GameUUID::INVALID_UUID;
		}


		if (auto loadedIt{ m_Sounds.find(soundData.soundPath.string()) }; loadedIt != end(m_Sounds))
		{
			return PlaySoundImpl(loadedIt->second, soundData);
		}
		 
		if (auto loadingIt{ m_LoadingSounds.find(soundData.soundPath.string()) }; loadingIt != end(m_LoadingSounds))
		{
			m_SoundPlayRequests.emplace_back(soundData);
			return GameUUID::INVALID_UUID;
		}

		LoadSoundImpl(soundData);
		m_SoundPlayRequests.emplace_back(soundData);
		return GameUUID::INVALID_UUID;
	}

	void FModSoundSytem::SetChannel3DPosition(const ChannelId& id, const glm::vec3& position) noexcept
	{
		auto it = m_Channels.find(id);
		if (it == end(m_Channels))
		{
			DEBUG_OUT("Channel for id: " << id.GetUUID_PrettyStr() << "does not exist");
			return;
		}

		const FMOD_VECTOR fmodPosition{ VectorToFmod(position) };
		ErrorCheck(it->second->set3DAttributes(&fmodPosition, NULL));
	}

	void FModSoundSytem::SetChannelVolume(const ChannelId& id, float volume) noexcept
	{
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

	const ChannelId FModSoundSytem::PlaySoundImpl(FMOD::Sound* pSound, const SoundData& soundData) noexcept
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
			ErrorCheck(pSound->getMode(&currMode));

			if (currMode & FMOD_3D)
			{
				const FMOD_VECTOR fmodPosition{ VectorToFmod(soundData.position) };
				ErrorCheck(pChannel->set3DAttributes(&fmodPosition, nullptr));
			}

			ErrorCheck(pChannel->setCallback(&ChannelControlCallback));
			ErrorCheck(pChannel->setUserData(this));

			ErrorCheck(pChannel->setVolume(soundData.volume));
			ErrorCheck(pChannel->setPaused(false)); //Unpause the sound

			ChannelId id{};
			m_Channels[id] = pChannel;

			return id;
		}

		return GameUUID::INVALID_UUID;
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
			if (soundData.isStream)
			{
				m_LoadingStreams[soundData.soundPath.string()].emplace_back(pSound);
			}
			else
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
}