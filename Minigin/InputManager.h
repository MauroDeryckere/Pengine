#ifndef INPUTMANAGER
#define INPUTMANAGER

#include "Singleton.h"

#include "InputCommand.h"
#include "InputDevice.h"

#include "InputKeys.h"

#include <vector>
#include <memory>

namespace Pengin
{
    struct InputCombo
    {
        std::vector<std::shared_ptr<InputCommand>> pComboActions;
        std::shared_ptr<InputCommand> pResultingAction;

        std::vector<float> allowedDelay;
    }; 

    class InputManager final : public dae::Singleton<InputManager>
    {
    public:
        [[nodiscard]] bool ProcessInput() noexcept;

        void MapControllerAction(ControllerButton button, InputState inputState, std::shared_ptr<InputCommand> pInputAction) noexcept;
        void MapKeyboardAction(KeyBoardKey key, InputState inputState, std::shared_ptr<InputCommand> pInputAction) noexcept;
        //void MapMouseAction(MouseButton button, InputState inputState, std::unique_ptr<InputCommand> pInputAction);

        //unmapping TODO

        void MapCombo(const InputCombo& combo) noexcept;

        InputManager(const InputManager&) = delete;
        InputManager(InputManager&&) = delete;
        InputManager& operator=(const InputManager&) = delete;
        InputManager& operator=(const InputManager&&) = delete;

    private:
        friend class dae::Singleton<InputManager>;
        InputManager(); //TODO: make sure to check if keyboard, controller,...
        ~InputManager() = default;

        std::vector<std::unique_ptr<InputDevice>> m_InputDevices;

        std::vector<InputCombo> m_Combos;

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
