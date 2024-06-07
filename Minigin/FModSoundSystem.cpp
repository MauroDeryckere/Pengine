#include "FModSoundSystem.h"

#include "DebugOutput.h"

namespace Pengin
{
	FModSoundSystem::FModSoundSystem()
	{
		ErrorCheck(FMOD::Studio::System::create(&m_pStudio));

		constexpr int MAX_CHANNELS{ 512 };

		ErrorCheck(m_pStudio->initialize(MAX_CHANNELS, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, NULL));
		ErrorCheck(m_pStudio->getCoreSystem(&m_pSystem));
		ErrorCheck(m_pSystem->getMasterChannelGroup(&m_MasterGroup));

		ErrorCheck(m_pSystem->createChannelGroup("VFX", &m_pVFXGroup));
		ErrorCheck(m_pSystem->createChannelGroup("Music", &m_pMusicGroup));
	}

	FModSoundSystem::~FModSoundSystem()
	{
		ErrorCheck(m_pStudio->unloadAll());
		ErrorCheck(m_pStudio->release());
	}

	void FModSoundSystem::Update() noexcept
	{
		std::erase_if(m_SoundsToRelease, [this](auto& sound)
			{
				FMOD_OPENSTATE openstate;
				ErrorCheck(sound->getOpenState(&openstate, NULL, NULL, NULL));

				if (openstate == FMOD_OPENSTATE::FMOD_OPENSTATE_READY) 
				{
					//Only free when ready to avoid stalls
					ErrorCheck(sound->release());
					return true;
				}

				return false;
			}
		);

		{
			FMOD::Channel* pChannel{ nullptr };
			while (m_ChannelCallbackQueue.TryPop(pChannel))
			{
				assert(pChannel);

				auto ctrlIt{ m_ChannelControlCallbackData.find(pChannel) };
				assert(ctrlIt != end(m_ChannelControlCallbackData));

				m_Channels.erase(ctrlIt->second.uuid);
				m_ChannelControlCallbackData.erase(ctrlIt);
			}

			FMOD::Sound* pSound{ nullptr };
			while (m_StreamEndCallbackQueue.TryPop(pSound))
			{
				assert(pSound);

				FMOD_OPENSTATE state{};
				pSound->getOpenState(&state, NULL, NULL, NULL);

				if (state == FMOD_OPENSTATE::FMOD_OPENSTATE_READY)
				{
					pSound->release();
				}
				else
				{
					m_SoundsToRelease.emplace_back(pSound);
				}
			}
		}

		{
			FMOD::Sound* pSound{ nullptr };
			while (m_StreamLoadedCallbackQueue.TryPop(pSound))
			{
				assert(pSound);

				auto cbIt = m_SoundloadedCallbackData.find(pSound);
				assert(cbIt != end(m_SoundloadedCallbackData));

				auto loadingIt = m_LoadingStreams.find(cbIt->second.key);
				if (loadingIt == end(m_LoadingStreams))
				{
					m_StreamPlayRequests.erase(pSound);
					m_SoundloadedCallbackData.erase(cbIt);
					continue;
				}

				auto reqIt = m_StreamPlayRequests.find(pSound);
				if (reqIt != end(m_StreamPlayRequests))
				{
					PlaySoundImpl(pSound, reqIt->second);

					m_StreamPlayRequests.erase(reqIt);
					std::erase_if(loadingIt->second, [&pSound](FMOD::Sound* sound) {return sound == pSound; });

					if (loadingIt->second.empty())
					{
						m_LoadingStreams.erase(loadingIt);
					}
				}
				else
				{
					m_Streams[cbIt->second.key].emplace_back(pSound);
				}

				m_SoundloadedCallbackData.erase(cbIt);
			}
		}

		{
			FMOD::Sound* pSound{ nullptr };
			while (m_SoundLoadedCallbackQueue.TryPop(pSound))
			{
				assert(pSound);

				auto cbIt = m_SoundloadedCallbackData.find(pSound);
				assert(cbIt != end(m_SoundloadedCallbackData));

				auto loadingIt = m_LoadingSounds.find(cbIt->second.key);
				if (loadingIt == end(m_LoadingSounds)) 
				{
					//cant find in loading means it was cancelled
					m_SoundPlayRequests.erase(pSound);
					m_SoundloadedCallbackData.erase(cbIt);
					continue;
				}

				m_LoadingSounds.erase(loadingIt);
				m_Sounds[cbIt->second.key] = pSound;

				auto reqIt = m_SoundPlayRequests.find(pSound);
				if (reqIt != end(m_SoundPlayRequests))
				{
					for (auto& soundData : reqIt->second)
					{
						PlaySoundImpl(pSound, soundData);
					}

					m_SoundPlayRequests.erase(reqIt);
				}

				m_SoundloadedCallbackData.erase(cbIt);
			}
		}

		//Underlying system and core Update
		ErrorCheck(m_pStudio->update());
	}

	void FModSoundSystem::LoadSound(const SoundData& soundData) noexcept
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

	void FModSoundSystem::UnLoadSound(const std::filesystem::path& soundPath) noexcept
	{
		if (auto it = m_Sounds.find(soundPath.string()); it != m_Sounds.end())
		{
			FMOD_OPENSTATE openstate;
			ErrorCheck(it->second->getOpenState(&openstate, 0, 0, 0));
			if (openstate == FMOD_OPENSTATE::FMOD_OPENSTATE_READY)
			{
				ErrorCheck(it->second->release());
			}
			else
			{
				m_SoundsToRelease.emplace_back(it->second);
			}

			m_Sounds.erase(it);
			return;
		}

		if (auto it = m_LoadingSounds.find(soundPath.string()); it != m_LoadingSounds.end())
		{
			FMOD_OPENSTATE openstate;
			ErrorCheck(it->second->getOpenState(&openstate, 0, 0, 0));

			if (openstate == FMOD_OPENSTATE::FMOD_OPENSTATE_READY)
			{
				ErrorCheck(it->second->release());
			}
			else
			{
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
					m_SoundsToRelease.emplace_back(snd);
				}
			}

			m_LoadingStreams.erase(it);
			found = true;
		}

		if (!found)
		{
			DEBUG_OUT("Warning: trying to unload a sound that is not loaded: " << soundPath.string());
		}
	}

	void FModSoundSystem::Clear() noexcept
	{
		//TODO
	}

	const ChannelId FModSoundSystem::PlaySound(const SoundData& soundData) noexcept
	{
		assert(std::filesystem::exists(soundData.soundPath) && "Sound path invalid, doesn't exist");

		if (soundData.isStream)
		{
			if (auto loadedIt{ m_Streams.find(soundData.soundPath.string()) }; loadedIt != end(m_Streams))
			{
				const auto id = PlaySoundImpl(loadedIt->second.back(), soundData);

				loadedIt->second.pop_back();

				if (loadedIt->second.empty())
				{
					m_Streams.erase(loadedIt);
				}

				return id;
			}

			auto pstr = LoadSoundImpl(soundData);
			m_StreamPlayRequests[pstr] = soundData;

			return GameUUID::INVALID_UUID;
		}

		if (auto loadedIt{ m_Sounds.find(soundData.soundPath.string()) }; loadedIt != end(m_Sounds))
		{
			return PlaySoundImpl(loadedIt->second, soundData);
		}
		 
		if (auto loadingIt{ m_LoadingSounds.find(soundData.soundPath.string()) }; loadingIt != end(m_LoadingSounds))
		{
			m_SoundPlayRequests[loadingIt->second].emplace_back(soundData);
			return GameUUID::INVALID_UUID;
		}

		auto pSound = LoadSoundImpl(soundData);
		assert(pSound);
		m_SoundPlayRequests[pSound].emplace_back(soundData);

		return GameUUID::INVALID_UUID;
	}

	bool FModSoundSystem::StopPlaying(const ChannelId& channel) noexcept
	{
		auto it = m_Channels.find(channel);

		if (it == end(m_Channels))
		{
			std::cout << channel.GetUUID_PrettyStr() << "\n";
			std::cout << "not found\n";
			return false;
		}

		ErrorCheck(it->second->stop());

		return true;
	}

	bool FModSoundSystem::Mute(const ChannelId& channel) noexcept
	{
		auto it = m_Channels.find(channel);

		if (it == end(m_Channels))
		{
			return false;
		}

		ErrorCheck(it->second->setMute(true));

		return true;
	}

	bool FModSoundSystem::Unmute(const ChannelId& channel) noexcept
	{
		auto it = m_Channels.find(channel);

		if (it == end(m_Channels))
		{
			return false;
		}

		ErrorCheck(it->second->setMute(false));

		return true;
	}

	bool FModSoundSystem::IsMuted(const ChannelId& channel) const noexcept
	{
		auto it = m_Channels.find(channel);

		if (it == end(m_Channels))
		{
			return false;
		}

		bool muted{ };
		ErrorCheck(it->second->getMute(&muted));

		return muted;
	}

	void FModSoundSystem::SetChannel3DPosition(const ChannelId& id, const glm::vec3& position) noexcept
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

	void FModSoundSystem::SetChannelVolume(const ChannelId& id, float volume) noexcept
	{
		auto it = m_Channels.find(id);
		if (it == end(m_Channels))
		{
			DEBUG_OUT("Channel for id: " << id.GetUUID_PrettyStr() << "does not exist");
			return;
		}

		ErrorCheck(it->second->setVolume(volume));
	}

	void FModSoundSystem::SetVFXVolume(const float vol) noexcept
	{
		ErrorCheck(m_pVFXGroup->setVolume(vol));
	}

	void FModSoundSystem::SetMusicVolume(const float vol) noexcept
	{
		ErrorCheck(m_pMusicGroup->setVolume(vol));
	}

	void FModSoundSystem::Mute() noexcept
	{
		ErrorCheck(m_MasterGroup->setMute(true));
		m_IsMuted = true;
	}

	void FModSoundSystem::Unmute() noexcept
	{
		ErrorCheck(m_MasterGroup->setMute(false));
		m_IsMuted = false;
	}

	bool FModSoundSystem::IsMuted() const noexcept
	{
		return m_IsMuted;
	}

	void FModSoundSystem::ErrorCheck(FMOD_RESULT result) const noexcept
	{
		if (result != FMOD_OK) 
		{
			std::cout << "FMOD ERROR: " << result << "\n";
		}
	}

	constexpr FMOD_VECTOR FModSoundSystem::VectorToFmod(const glm::vec3& vPosition) const noexcept
	{
		return FMOD_VECTOR{ vPosition.x, vPosition.y, vPosition.z };
	}

	const ChannelId FModSoundSystem::PlaySoundImpl(FMOD::Sound* pSound, const SoundData& soundData) noexcept
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

			ChannelId id{};

			m_Channels[id] = pChannel;

			ErrorCheck(pChannel->setCallback(&ChannelControlCallback));
			ErrorCheck(pChannel->setUserData(this));

			m_ChannelControlCallbackData.emplace(pChannel, ControlCallbackData{id} );
			
			ErrorCheck(pChannel->setVolume(soundData.volume));
			ErrorCheck(pChannel->setPaused(false)); //Unpause the sound

			return id;
		}

		return GameUUID::INVALID_UUID;
	}

	FMOD::Sound* FModSoundSystem::LoadSoundImpl(const SoundData& soundData) noexcept
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
		exinfo.userdata = this;

		const auto res = m_pSystem->createSound(soundData.soundPath.string().c_str(), flags, &exinfo, &pSound);
		assert(res != FMOD_RESULT::FMOD_ERR_FORMAT && "Bad file format"); 

		ErrorCheck(res);
	
		if (pSound)
		{
			if (soundData.isStream)
			{
				m_LoadingStreams[soundData.soundPath.string()].emplace_back(pSound);
			}
			else
			{
				m_LoadingSounds[soundData.soundPath.string()] = pSound;
			}

			m_SoundloadedCallbackData.emplace(pSound, LoadCallbackData{ soundData.soundPath.string() });
		}

		return pSound;
	}
}