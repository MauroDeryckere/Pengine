#ifndef INPUTMANAGER
#define INPUTMANAGER

#include "Singleton.h"

#include "Windows.h"
#include "XInput.h"

#include "InputCommand.h"

#include <unordered_map>
#include <memory>
#include <array>

namespace Pengin
{
    class InputManager final : public dae::Singleton<InputManager>
    {
    public:
        enum class InputCode
        {
            DPadUp,
            DPadDown,
            DPadLeft,
            DPadRight,
            Start,
            Back,
            LeftThumb,
            RightThumb,
            LeftShoulder,
            RightShoulder,
            A,
            B,
            X,
            Y
        };

        enum class InputCommandType
        {
            DownThisFrame,
            UpThisFrame,
            Pressed
        };

        [[nodiscard]] bool ProcessInput();

        [[nodiscard]] bool IsDownThisFrame(InputCode button) const;
        [[nodiscard]] bool IsUpThisFrame(InputCode button) const;
        [[nodiscard]] bool IsPressed(InputCode button) const;

        void RegisterActionMapping(InputCode button, InputCommandType type, std::unique_ptr<InputCommand> pAction);

    private:
        XINPUT_STATE m_CurrentState;

        unsigned m_ButtonsPressedThisFrame;
        unsigned m_ButtonsReleasedThisFrame;

        struct InputCommandInfo
        {
            bool operator==(const InputCommandInfo& other) const
            {
                return code == other.code && type == other.type;
            }

            InputCode code;
            InputCommandType type;
        };

        struct InputCommandInfoHash
        {
            std::size_t operator()(const InputCommandInfo& info) const
            {
                // Combine the hash values of code and type using bitwise XOR
                return std::hash<InputCode>{}(info.code) ^ std::hash<InputCommandType>{}(info.type);
            }
        };

        std::unordered_map<InputCommandInfo, std::unique_ptr<InputCommand>, InputCommandInfoHash> m_InputActionMapping;

        enum class ConsoleButton : unsigned
        {
            DPadUp = 0x0001,
            DPadDown = 0x0002,
            DPadLeft = 0x0004,
            DPadRight = 0x0008,

            Start = 0x0010,
            Back = 0x0020,

            LeftThumb = 0x0040,
            RightThumb = 0x0080,
            LeftShoulder = 0x0100,
            RightShoulder = 0x0200,

            A = 0x1000,
            B = 0x2000,
            X = 0x4000,
            Y = 0x8000,
        };

        // Define a mapping between InputCode and ConsoleButton using an array
        static constexpr std::array<ConsoleButton, static_cast<size_t>(InputCode::Y) + 1> m_InputCodeToConsoleButtonMapping 
        {
            ConsoleButton::DPadUp,
            ConsoleButton::DPadDown,
            ConsoleButton::DPadLeft,
            ConsoleButton::DPadRight,
            ConsoleButton::Start,
            ConsoleButton::Back,
            ConsoleButton::LeftThumb,
            ConsoleButton::RightThumb,
            ConsoleButton::LeftShoulder,
            ConsoleButton::RightShoulder,
            ConsoleButton::A,
            ConsoleButton::B,
            ConsoleButton::X,
            ConsoleButton::Y
        };
    };
}

#endif
