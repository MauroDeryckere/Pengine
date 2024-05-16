#ifndef FMODSOUNDSYSTEM
#define FMODSOUNDSYSTEM

#include "SoundSystem.h"
#include "SoundData.h"

#include "DebugOutput.h"

#include <fmod_studio.hpp>
#include <fmod.hpp>

#include <string>
#include <map>
#include <unordered_map>
#include <vector>
#include <mutex>
#include <unordered_set>

#include <glm/vec3.hpp>

//Ref: https://codyclaborn.me/tutorials/making-a-basic-fmod-audio-engine-in-c/
//	   https://www.fmod.com/docs/2.03/api/welcome.html

/*
Audio Engine TODO:
- Pausing all
- Support velocity for 3D Sound
- Stop playing a Sound

- FMOD Event / bank support
- Reverb support
- Listener && Update the listener (3D audio)
- Effects

- ImGUI debug window
*/

namespace Pengin
{
	class FModSoundSytem final : public SoundSystem
	{
	public:
		FModSoundSytem();
		virtual ~FModSoundSytem() override;

		void Update() noexcept;

		void LoadSound(SoundData& soundData) noexcept;
		void UnLoadSound(const std::filesystem::path& soundPath) noexcept;

		//It is important to load the sound first if you want a valid channel idx
		const ChannelId PlaySound(SoundData& soundData) noexcept;

		void SetVFXVolume(const float vol) noexcept;
		void SetMusicVolume(const float vol) noexcept;

		void MuteAll() noexcept;
		void UnmuteAll() noexcept;
		[[nodiscard]] bool IsMuted() const noexcept;

		void SetChannel3DPosition(const GameUUID& id, const glm::vec3& position) noexcept;
		void SetChannelVolume(const GameUUID& id, float volume) noexcept;

		FModSoundSytem(const FModSoundSytem&) = delete;
		FModSoundSytem(FModSoundSytem&&) = delete;
		FModSoundSytem& operator=(const FModSoundSytem&) = delete;
		FModSoundSytem& operator=(const FModSoundSytem&&) = delete;

	private:
		mutable std::mutex m_ChannelEndCallbackMutex; 
		mutable std::mutex m_LoadCallbackMutex; 

		//How we deal with streams: 
		/*
		- A stream can only be played once
		- Loading a stream doesn't mean we play, still have to call play
		- 
		*/

		//map<path, sound*> sounds

		//uoset<Sound*> loadedStreams
		
		
		// stream Reqs

		// if stream, should not be in sounds map but separate map
		// 
		// 
		// 
		// streams - released upon end of play

		//PlaySound(stream)
		//-> Load the stream
		//-> add to playstream reqs
		//-> if in playstream reqs upon load, play

		//Loaded sounds
		using SoundMap = std::unordered_map<std::string, FMOD::Sound*>;
		//The sounds that are being loaded
		using LoadingMap = std::unordered_map<std::string, FMOD::Sound*>;

		//In use channels
		using ChannelMap = std::unordered_map<ChannelId, FMOD::Channel*>;

		//Requests for sounds that are currently being loaded
		using RequestVec = std::vector<SoundData>;
		//Sounds have to be fully loaded before releasing, if a user decides to cancel it, we have to maintain a vector of anything to be released
		using ToReleaseVec = std::vector<FMOD::Sound*>;

		FMOD::Studio::System* m_pStudio{ nullptr };
		FMOD::System* m_pSystem{ nullptr }; //Core API

		FMOD::ChannelGroup* m_MasterGroup{ nullptr };
		FMOD::ChannelGroup* m_pVFXGroup{ nullptr };
		FMOD::ChannelGroup* m_pMusicGroup{ nullptr };

		bool m_IsMuted{ false };

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
		const ChannelId PlaySoundImpl(FMOD::Sound* pSound, SoundData& soundData) noexcept;

		//Allows loading without additional checks
		void LoadSoundImpl(SoundData& soundData) noexcept;

		static FMOD_RESULT F_CALL NonBlockLoadCallback(FMOD_SOUND* sound, FMOD_RESULT)
		{
			FMOD::Sound* snd = (FMOD::Sound*)sound;
			
			FMOD_OPENSTATE state{};
			snd->getOpenState(&state, NULL, NULL, NULL);

			if (state == FMOD_OPENSTATE_SETPOSITION)
			{
				std::cout << "setpos callback\n";
				return FMOD_OK;
			}

			void* userData;
			snd->getUserData(&userData);
			assert(userData);

			auto* pair = static_cast<std::pair<FModSoundSytem*, SoundData>*>(userData);
			assert(pair);

			FModSoundSytem* pSoundSystem{ pair->first };
			const auto& path = pair->second.soundPath.string();

			if (pair->second.isStream)
			{
				DEBUG_OUT("stream callback\n\n");

				pSoundSystem->m_LoadingSounds.erase(path);

				delete pair;

				snd->setUserData(nullptr);

				return FMOD_OK;
			}

			{
				std::lock_guard<std::mutex> lock(pSoundSystem->m_LoadCallbackMutex);
				
				pSoundSystem->m_Sounds[path] = snd;
				pSoundSystem->m_LoadingSounds.erase(path);
			}

			delete pair;
			snd->setUserData(nullptr);

			return FMOD_OK;
		}

		static FMOD_RESULT F_CALL ChannelControlCallback(FMOD_CHANNELCONTROL* channelcontrol, FMOD_CHANNELCONTROL_TYPE controltype, FMOD_CHANNELCONTROL_CALLBACK_TYPE callbacktype, void*, void*)
		{
			if (callbacktype == FMOD_CHANNELCONTROL_CALLBACK_TYPE::FMOD_CHANNELCONTROL_CALLBACK_END)
			{
				assert(controltype == FMOD_CHANNELCONTROL_TYPE::FMOD_CHANNELCONTROL_CHANNEL);
				FMOD::Channel* channel = (FMOD::Channel*)channelcontrol; 
				
				FMOD_MODE mode;
				channel->getMode(&mode);

				if (mode & FMOD_CREATESTREAM)
				{
					FMOD::Sound* pSound;
					channel->getCurrentSound(&pSound);
					assert(pSound);

					pSound->release();

					return FMOD_OK;
				}
				
				void* userData;
				channel->getUserData(&userData);
				assert(userData);

				auto* pair = static_cast<std::pair<FModSoundSytem*, ChannelId>*>(userData);
				assert(pair);

				FModSoundSytem* pSoundSystem{ pair->first };
				{
					std::lock_guard<std::mutex> lock(pSoundSystem->m_ChannelEndCallbackMutex);

					pSoundSystem->m_Channels.erase(pair->second);
				}

				delete pair;
			}

			return FMOD_OK;
		}



		//TODO add support for these systems
		//using EventMap = std::map<std::string, FMOD::Studio::EventInstance*>;
		//using BankMap = std::map<std::string, FMOD::Studio::Bank*>;

		//BankMap mBanks;
		//EventMap mEvents;
	};
}

#endif 