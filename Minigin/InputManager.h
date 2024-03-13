#ifndef INPUTMANAGER
#define INPUTMANAGER

#include "Singleton.h"

#include "Windows.h"
#include "XInput.h"

#include "InputCommand.h"

#include <unordered_map>
#include <vector>
#include <memory>
#include <array>
#include <functional>

namespace Pengin
{
    enum class KeyBoardKey : unsigned
    {
        A = 0x41
    };

    enum class MouseButton : unsigned
    {
        Left
    };

    enum class ControllerButton : unsigned
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

    enum class InputState
    {
        DownThisFrame,
        UpThisFrame,
        Pressed,
        Movement,

        STATE_COUNT
    };

    class InputManager final : public dae::Singleton<InputManager>
    {
    public:
        InputManager();

        [[nodiscard]] bool ProcessInput();

        void MapControllerAction(ControllerButton button, InputState inputState, std::unique_ptr<InputCommand> pInputAction);
        void MapKeyboardAction(KeyBoardKey key, InputState inputState, std::unique_ptr<InputCommand> pInputAction);
        void MapMouseAction(MouseButton button, InputState inputState, std::unique_ptr<InputCommand> pInputAction);

    private:
        //Vector idx == InputState
        std::vector<std::unordered_map<KeyBoardKey, std::unique_ptr<InputCommand>>> m_KeyboardActionMapping;
        std::vector<std::unordered_map<MouseButton, std::unique_ptr<InputCommand>>> m_MouseActionMapping;
        std::vector<std::unordered_map<ControllerButton, std::unique_ptr<InputCommand>>> m_ControllerActionMapping;

        //Keyboard
        BYTE m_CurrentKBState[256];
        BYTE m_KBButtonsPressedThisFrame[256];
        BYTE m_KBButtonsReleasedThisFrame[256];

        //-------------

        //Controller
        XINPUT_STATE m_CurrentState;

        unsigned m_ButtonsPressedThisFrame;
        unsigned m_ButtonsReleasedThisFrame;
        //-----------------------------------

        enum class Devices : char
        {
            Keyboard,
            Mouse,
            Controller
        };

        typedef std::function<bool(Devices, unsigned)> StateFunction;

        [[nodiscard]] bool IsDownThisFrame(Devices device, unsigned btn) const;
        [[nodiscard]] bool IsUpThisFrame(Devices device, unsigned btn) const;
        [[nodiscard]] bool IsPressed(Devices device, unsigned btn) const;

        StateFunction stateFunctions[static_cast<size_t>(InputState::STATE_COUNT)] = 
        {
            [this](Devices device, unsigned btn) { return IsDownThisFrame(device, btn); },
            [this](Devices device, unsigned btn) { return IsUpThisFrame(device, btn); },
            [this](Devices device, unsigned btn) { return IsPressed(device, btn); }
        };

        void ProcessInputActions();
    };
}

#endif
