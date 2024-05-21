#ifndef INPUTMANAGER
#define INPUTMANAGER

#include "Singleton.h"

#include "CoreIncludes.h"

#include "InputCommand.h"
#include "InputDevice.h"
#include "InputBuffer.h"

#include "InputKeys.h"

#include <vector>
#include <memory>
#include <unordered_set>

namespace Pengin
{
    using UserIndex = GameUUID;

    class InputBuffer;

    enum class UserType : unsigned
    {
        Keyboard,
        Controller
    };

    struct InputCombo final
    {
        std::vector<std::shared_ptr<InputCommand>> pComboActions;
        std::vector<float> allowedDelay;

        std::shared_ptr<InputCommand> pResultingAction;
    }; 

    class InputManager final : public Singleton<InputManager>
    {
    public:
        [[nodiscard]] bool ProcessInput() noexcept;

        [[nodiscard]] const UserIndex RegisterUser(UserType usertype) noexcept;
        void RegisterUser(const UserIndex& index, UserType usertype) noexcept;

        std::shared_ptr<InputCommand> MapControllerAction(const UserIndex& userIdx, ControllerButton button, InputState inputState, std::shared_ptr<InputCommand> pInputAction) noexcept;
        std::shared_ptr<InputCommand> MapKeyboardAction(const UserIndex& userIdx, KeyBoardKey key, InputState inputState, std::shared_ptr<InputCommand> pInputAction) noexcept;

        void MapCombo(const UserIndex& userIdx, const InputCombo& combo) noexcept;

        [[nodiscard]] bool IsActionExecuted(const UserIndex& user, const std::string& actionName) const noexcept;

        void Clear() noexcept; //this clears everything, including users
        void Reset() noexcept; //this clears everything, but keeps the users and devices

        InputManager(const InputManager&) = delete;
        InputManager(InputManager&&) = delete;
        InputManager& operator=(const InputManager&) = delete;
        InputManager& operator=(const InputManager&&) = delete;

    private:
        friend class Singleton<InputManager>;
        InputManager() = default;
        ~InputManager() = default;

        friend class InputInfoPanel;
        friend class JsonSerializer;

        std::unordered_map<UserIndex, size_t> m_UserIdx_VecIdxMap; //Maps the userindex (UUID) to the correspending index in all vectors below

        std::vector<std::pair<UserType, std::vector<std::unique_ptr<InputDevice>>>> m_RegisteredUsers;
        
        std::vector<std::vector<InputCombo>> m_InputCombos;
        std::vector<std::unique_ptr<InputBuffer>> m_InputBuffers;
        
        std::vector<std::unordered_set<std::string>> m_ExecutedActionsThisFrame;

        static constexpr unsigned MAX_ALLOWED_CONTROLLERS{ 4 };

        enum class KeyboardDevices
        {
            Keyboard = 0,
            Mouse = 1,
        };

        enum class ControllerDevices
        {
            Controller = 0
        };

    };
}

#endif
