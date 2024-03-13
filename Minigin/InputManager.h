#ifndef INPUTMANAGER
#define INPUTMANAGER

#include "Singleton.h"

#include "Windows.h"
#include "XInput.h"

#include "InputCommand.h"
#include "InputController.h"

#include <unordered_map>
#include <vector>
#include <memory>
#include <array>
#include <functional>


//MapAction(ungsigned key, inputState, ptr)

//In the class 
//  static_cast<ControllerButton>(key)
//  Button --> Button 


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
        Y,
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
        std::unique_ptr<InputController> m_Controller;
        //m_..

        //Vector idx == InputState
        std::vector<std::unordered_map<KeyBoardKey, std::unique_ptr<InputCommand>>> m_KeyboardActionMapping;
        std::vector<std::unordered_map<MouseButton, std::unique_ptr<InputCommand>>> m_MouseActionMapping;

        //Keyboard
        BYTE m_CurrentKBState[256];
        BYTE m_KBButtonsPressedThisFrame[256];
        BYTE m_KBButtonsReleasedThisFrame[256];
        //-------------

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
