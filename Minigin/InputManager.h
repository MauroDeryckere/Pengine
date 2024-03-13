#ifndef INPUTMANAGER
#define INPUTMANAGER

#include "Singleton.h"

#include "Windows.h"
#include "XInput.h"

#include "InputCommand.h"
#include "InputDevice.h"

#include <unordered_map>
#include <vector>
#include <memory>
#include <functional>

namespace Pengin
{

    enum class KeyBoardKey : unsigned
    {
        A
    };

    //enum class MouseButton : unsigned
    //{
     //   Left
    //};

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
        //void MapMouseAction(MouseButton button, InputState inputState, std::unique_ptr<InputCommand> pInputAction);

    private:
        std::vector<std::unique_ptr<InputDevice>> m_InputDevices;

        enum class Devices : char
        {
            Keyboard,
            //Mouse,
            Controller,

            DEVICE_COUNT
        };
    };
}

#endif
