#ifndef INPUTBUFFER
#define INPUTBUFFER

#include "Singleton.h"
#include "InputCommand.h"
#include "InputManager.h"

#include <cassert>
#include <chrono>
#include <vector>
#include <memory>
#include <iostream>

namespace Pengin
{
    struct InputRecord
    {
        std::shared_ptr<InputCommand> pAction;
        std::chrono::high_resolution_clock::time_point timestamp;
    };

    class InputBuffer final : public dae::Singleton<InputBuffer>
    {
    public:
        void RecordInput(std::shared_ptr<InputCommand> pAction);
        [[nodiscard]] bool CheckCombo(const InputCombo& combo) const;
        
        void ClearBuffer() { m_Buffer.clear(); }

        InputBuffer(const InputBuffer&) = delete;
        InputBuffer(InputBuffer&&) = delete;
        InputBuffer& operator=(const InputBuffer&) = delete;
        InputBuffer& operator=(const InputBuffer&&) = delete;

    private:
        friend class dae::Singleton<InputBuffer>;
        InputBuffer() = default;
        ~InputBuffer() = default;

        std::vector<InputRecord> m_Buffer;

        constinit static const int MAX_BUFFER_SIZE{ 20 };
        static constexpr float ERROR_MARGIN_TIME{ 200.f }; //ms
    };
}

#endif