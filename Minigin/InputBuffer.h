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
#include <deque>

namespace Pengin
{
    class InputBuffer final
    {
    public:
        InputBuffer() = default;
        ~InputBuffer() = default;

        void RecordInput(std::shared_ptr<InputCommand> pAction) noexcept;
        [[nodiscard]] bool CheckCombo(const InputCombo& combo) const noexcept;
        
        void ClearBuffer() noexcept { m_Buffer.clear(); }

        InputBuffer(const InputBuffer&) = delete;
        InputBuffer(InputBuffer&&) = delete;
        InputBuffer& operator=(const InputBuffer&) = delete;
        InputBuffer& operator=(const InputBuffer&&) = delete;

    private:   
        struct InputRecord
        {
            std::shared_ptr<InputCommand> pAction;
            std::chrono::high_resolution_clock::time_point timestamp;
        };

        std::deque<InputRecord> m_Buffer;

        static constexpr int MAX_BUFFER_SIZE{ 20 };
        static constexpr float ERROR_MARGIN_TIME{ 200.f }; //ms
    };
}

#endif