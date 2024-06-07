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

#include <glm/vec3.hpp>

#include "ThreadSafeQueue.h"

//Ref: https://codyclaborn.me/tutorials/making-a-basic-fmod-audio-engine-in-c/
//	   https://www.fmod.com/docs/2.03/api/welcome.html

/*
Audio Engine TODO:
- Optimize the value stored in maps, we could just store the name instead of the full path, this would not allow sounds with the same name in different folders though

- Pausing all
- Support velocity for 3D Sound

- FMOD Event / bank support
- Reverb support
- Listener && Update the listener (3D audio)
- Effects

- ImGUI debug window
*/

//TODO: allow to play with a channel ID param

namespace Pengin
{
	class FModSoundSystem final : public SoundSystem
	{
	public:
		FModSoundSystem();
		virtual ~FModSoundSystem() override;

		void Update() noexcept;

		void SetVFXVolume(const float vol) noexcept;
		void SetMusicVolume(const float vol) noexcept;

		void Mute() noexcept;
		void Unmute() noexcept;
		[[nodiscard]] bool IsMuted() const noexcept;

		void Clear() noexcept;

		void LoadSound(const SoundData& soundData) noexcept;
		void UnLoadSound(const std::filesystem::path& soundPath) noexcept;

		//It is important to load the sound first if you want a valid channel id
		const ChannelId PlaySound(const SoundData& soundData) noexcept;
		bool StopPlaying(const ChannelId& channel) noexcept;

		bool Mute(const ChannelId& channel) noexcept;
		bool Unmute(const ChannelId& channel) noexcept;
		bool IsMuted(const ChannelId& channel) const noexcept;

		void SetChannel3DPosition(const ChannelId& id, const glm::vec3& position) noexcept;
		void SetChannelVolume(const ChannelId& id, float volume) noexcept;

		FModSoundSystem(const FModSoundSystem&) = delete;
		FModSoundSystem(FModSoundSystem&&) = delete;
		FModSoundSystem& operator=(const FModSoundSystem&) = delete;
		FModSoundSystem& operator=(const FModSoundSystem&&) = delete;

	private:
		struct ControlCallbackData final
		{
			GameUUID uuid;

			explicit ControlCallbackData(const GameUUID& id) :
				uuid{ id }
			{ }

			~ControlCallbackData() = default;
		};

		struct LoadCallbackData final
		{
			std::string key;

			explicit LoadCallbackData(const std::string& k) :
				key{ k }
			{ }

			~LoadCallbackData() = default;
		};

		FMOD::Studio::System* m_pStudio{ nullptr };
		FMOD::System* m_pSystem{ nullptr }; //Core API

		FMOD::ChannelGroup* m_MasterGroup{ nullptr };
		FMOD::ChannelGroup* m_pVFXGroup{ nullptr };
		FMOD::ChannelGroup* m_pMusicGroup{ nullptr };

		bool m_IsMuted{ false };

		//Channels
		std::unordered_map<GameUUID, FMOD::Channel*> m_Channels{};
		std::unordered_map<FMOD::Channel*, ControlCallbackData> m_ChannelControlCallbackData{};

		//Sounds
		std::unordered_map<std::string, FMOD::Sound*> m_Sounds{};
		std::unordered_map<std::string, FMOD::Sound*> m_LoadingSounds{};
		std::unordered_map<FMOD::Sound*, std::vector<SoundData>> m_SoundPlayRequests{}; //Requests for sounds that are currently being loaded

		//Streams
		std::unordered_map<std::string, std::vector<FMOD::Sound*>> m_Streams{}; //loaded but unused
		std::unordered_map<std::string, std::vector<FMOD::Sound*>> m_LoadingStreams{}; //loading and used
		std::unordered_map<FMOD::Sound*, SoundData> m_StreamPlayRequests{};	

		//Sounds have to be fully loaded before releasing, if not ready and unloaded we add it to this vector
		std::vector<FMOD::Sound*> m_SoundsToRelease{};
		std::unordered_map<FMOD::Sound*, LoadCallbackData> m_SoundloadedCallbackData{};

		ThreadSafeQueue<FMOD::Channel*> m_ChannelCallbackQueue{};
		ThreadSafeQueue<FMOD::Sound*> m_StreamEndCallbackQueue{};

		ThreadSafeQueue<FMOD::Sound*> m_SoundLoadedCallbackQueue{};
		ThreadSafeQueue<FMOD::Sound*> m_StreamLoadedCallbackQueue{};

		void ErrorCheck(FMOD_RESULT result) const noexcept;
		[[nodiscard]] constexpr FMOD_VECTOR VectorToFmod(const glm::vec3& vPosition) const noexcept;

		const ChannelId PlaySoundImpl(FMOD::Sound* pSound, const SoundData& soundData) noexcept;
		FMOD::Sound* LoadSoundImpl(const SoundData& soundData) noexcept;

		static FMOD_RESULT F_CALL ChannelControlCallback(FMOD_CHANNELCONTROL* channelcontrol, FMOD_CHANNELCONTROL_TYPE controltype [[maybe_unused]], FMOD_CHANNELCONTROL_CALLBACK_TYPE callbacktype, void*, void*)
		{
			if (callbacktype == FMOD_CHANNELCONTROL_CALLBACK_TYPE::FMOD_CHANNELCONTROL_CALLBACK_END)
			{
				assert(controltype == FMOD_CHANNELCONTROL_TYPE::FMOD_CHANNELCONTROL_CHANNEL);
				FMOD::Channel* pChannel = (FMOD::Channel*)channelcontrol;

				void* userData{nullptr};
				pChannel->getUserData(&userData);
				assert(userData);

				FModSoundSystem* pSys{ static_cast<FModSoundSystem*>(userData) };

				pSys->m_ChannelCallbackQueue.Emplace(pChannel);

				FMOD_MODE mode{};
				pChannel->getMode(&mode);

				if (mode & FMOD_CREATESTREAM)
				{
					FMOD::Sound* pSound{ nullptr };
					pChannel->getCurrentSound(&pSound);
					assert(pSound);

					pSys->m_StreamEndCallbackQueue.Emplace(pSound);
				}
			}

			return FMOD_OK;
		}

		static FMOD_RESULT F_CALL NonBlockLoadCallback(FMOD_SOUND* sound, FMOD_RESULT)
		{
			FMOD::Sound* pSound{ (FMOD::Sound*)sound };
			
			FMOD_OPENSTATE state{};
			pSound->getOpenState(&state, NULL, NULL, NULL);

			if (state == FMOD_OPENSTATE_SETPOSITION)
			{
				//ignore if callback is triggered by setposition
				return FMOD_OK;
			}

			void* userData{ nullptr };
			pSound->getUserData(&userData);
			assert(userData);

			FModSoundSystem* pSys{ static_cast<FModSoundSystem*>(userData) };

			if (state == FMOD_OPENSTATE_READY)
			{
				FMOD_MODE mode{};
				pSound->getMode(&mode);

				if (mode & FMOD_CREATESTREAM)
				{
					pSys->m_StreamLoadedCallbackQueue.Emplace(pSound);

				}
				else
				{
					pSys->m_SoundLoadedCallbackQueue.Emplace(pSound);
				}
			}

			return FMOD_OK;
		}
	};
}

#endif 