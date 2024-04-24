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

namespace Pengin
{
    class InputBuffer;

    enum class UserType : unsigned
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

    using UserIndex = GameUUID;
    class InputManager final : public Pengin::Singleton<InputManager>
    {
    public:
        [[nodiscard]] bool ProcessInput() noexcept;
        [[nodiscard]] const UserIndex RegisterUser(UserType usertype) noexcept;
        void RegisterUser(const UserIndex& index, UserType usertype) noexcept;

        std::shared_ptr<InputCommand> MapControllerAction(const UserIndex& userIdx, ControllerButton button, InputState inputState, std::shared_ptr<InputCommand> pInputAction) noexcept;
        std::shared_ptr<InputCommand> MapKeyboardAction(const UserIndex& userIdx, KeyBoardKey key, InputState inputState, std::shared_ptr<InputCommand> pInputAction) noexcept;

        void Clear() noexcept; //this clears evrything, including users
        void Reset() noexcept; //this clears eveything, but keeps the users and devices

        //unmapping specific actions TODO (remapping)

        void MapCombo(const UserIndex& userIdx, const InputCombo& combo) noexcept;

        InputManager(const InputManager&) = delete;
        InputManager(InputManager&&) = delete;
        InputManager& operator=(const InputManager&) = delete;
        InputManager& operator=(const InputManager&&) = delete;

    private:
        friend class Pengin::Singleton<InputManager>;
        InputManager() = default;
        ~InputManager() = default;

        friend class Serializer; //Serialize / Deserialize input - TODO

        std::unordered_map<UserIndex, size_t> m_UserIdx_VecIdxMap;

        std::vector<std::pair<UserType, std::vector<std::unique_ptr<InputDevice>>>> m_RegisteredUsers;
        std::vector<std::vector<InputCombo>> m_InputCombos;
        std::vector<std::unique_ptr<InputBuffer>> m_InputBuffers;

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

        friend class InputInfoPanel;
    };
}

#endif
