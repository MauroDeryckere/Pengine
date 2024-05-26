#ifndef INPUTCOMMANDS
#define INPUTCOMMANDS

#include "SceneManager.h"

#include "InputCommand.h"
#include "EventManager.h"

#include "SoundData.h"
#include "ServiceLocator.h"

#include "ECS.h"
#include "Entity.h"
#include "Components.h"

#include "glm/vec3.hpp"

#include <memory>
#include <iostream>
#include <string>

namespace Pengin
{
	class InpDebugCommand final : public InputCommand
	{
	public:

		InpDebugCommand(const UserIndex& user, const std::string& msg) :
			InputCommand{ user, "InpDebug"},
			m_Message{ msg }
		{ }

		virtual void Execute() override
		{
			DEBUG_OUT(m_Message);
			DEBUG_OUT("on user: " << GetUserIdx().GetUUID_PrettyStr());
		}

		virtual ~InpDebugCommand() override = default;

		InpDebugCommand(const InpDebugCommand&) = delete;
		InpDebugCommand& operator=(const InpDebugCommand&) = delete;
		InpDebugCommand(InpDebugCommand&&) noexcept = delete;
		InpDebugCommand& operator=(InpDebugCommand&&) noexcept = delete;

	private:
		const std::string m_Message;
	};

	class MakeSound final : public InputCommand //bound to input for now
	{
	public:
		MakeSound(const UserIndex& user, const SoundData& soundData) :
			InputCommand{ user, "MakeSound"},
			m_SoundData{ soundData }
		{ 
			assert(!soundData.soundPath.empty());
		}

		virtual void Execute() override
		{
			ServiceLocator::GetSoundSystem().PlaySound(m_SoundData);
		}

		virtual ~MakeSound() override = default;

		MakeSound(const MakeSound&) = delete;
		MakeSound& operator=(const MakeSound&) = delete;
		MakeSound(MakeSound&&) noexcept = delete;
		MakeSound& operator=(MakeSound&&) noexcept = delete;

	private:
		const SoundData m_SoundData;
	};

}

#endif