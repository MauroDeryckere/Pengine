#ifndef INPUTMANAGER
#define INPUTMANAGER

#include "Singleton.h"

#include "InputCommand.h"
#include "InputDevice.h"
#include "InputBuffer.h"

#include "InputKeys.h"

#include <vector>
#include <memory>

namespace Pengin
{
    class InputBuffer;

    enum class UserType : char
    {
        Keyboard,
        Controller
    };

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

        [[nodiscard]] size_t RegisterUser(UserType usertype) noexcept; //returns the userIndex

        void MapControllerAction(size_t userIdx, ControllerButton button, InputState inputState, std::shared_ptr<InputCommand> pInputAction) noexcept;
        void MapKeyboardAction(size_t userIdx, KeyBoardKey key, InputState inputState, std::shared_ptr<InputCommand> pInputAction) noexcept;

        //void MapMouseAction(userIdx, MouseButton button, InputState inputState, std::unique_ptr<InputCommand> pInputAction);

        //unmapping TODO

        void MapCombo(size_t userIdx, const InputCombo& combo) noexcept;

        InputManager(const InputManager&) = delete;
        InputManager(InputManager&&) = delete;
        InputManager& operator=(const InputManager&) = delete;
        InputManager& operator=(const InputManager&&) = delete;

    private:
        friend class dae::Singleton<InputManager>;
        InputManager() = default;
        ~InputManager() = default;

        std::vector<std::pair<UserType, std::vector<std::unique_ptr<InputDevice>>>> m_RegisteredUsers;
        std::vector<std::vector<InputCombo>> m_NewInputCombos;
        std::vector<std::unique_ptr<InputBuffer>> m_InputBuffers;


        static constexpr unsigned MAX_ALLOWED_CONTROLLERS{ 4 };
    };
}

#endif
