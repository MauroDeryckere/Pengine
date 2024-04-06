#ifndef INPUTCONTROLLER
#define INPUTCONTROLLER

#include "InputDevice.h"
#include <memory>

namespace Pengin
{
    class WindowsInputControllerImpl;

    class InputController final : public InputDevice
    {
    public:
        InputController(size_t userIdx);
        virtual ~InputController() override = default;

        virtual void ProcessInputState() override;
        virtual void ProcessMappedActions(InputBuffer* const inputBuffer) override;
        virtual void MapActionToInput(unsigned key, InputState inputState, std::shared_ptr<InputCommand> pInputAction) override;
        virtual void* GetMappedActions() override;

    private:
        std::unique_ptr<WindowsInputControllerImpl> m_WinImpl;

        virtual [[nodiscard]] unsigned GetCodeFromKey(unsigned key) const override;

        virtual [[nodiscard]] bool IsDownThisFrame(unsigned btn) const override;
        virtual [[nodiscard]] bool IsUpThisFrame(unsigned btn) const override;
        virtual [[nodiscard]] bool IsPressed(unsigned btn) const override;
    };
}

#endif
