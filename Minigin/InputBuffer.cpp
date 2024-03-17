#include "InputBuffer.h"

void Pengin::InputBuffer::RecordInput(std::shared_ptr<InputCommand> pAction)
{
    InputRecord record;
    record.pAction = std::move(pAction);
    record.timestamp = std::chrono::high_resolution_clock::now();

    m_Buffer.emplace_back(record);

    if (m_Buffer.size() > MAX_BUFFER_SIZE)
    {
        m_Buffer.erase(m_Buffer.begin());
    }
}

bool Pengin::InputBuffer::CheckCombo(const InputCombo& combo) const
{
    if (m_Buffer.size() < combo.pComboActions.size() || combo.pComboActions.empty())
    {
        return false;
    }

    size_t comboIdx { 0 };
    auto lastActionTime{ m_Buffer[0].timestamp };

    for (size_t bufferIdx{ 0 }; bufferIdx < m_Buffer.size(); ++bufferIdx)
    {
        const auto currentActionTime{ m_Buffer[bufferIdx].timestamp };
        const auto timeDiff { currentActionTime - lastActionTime };
        const auto secTimeDiff{ std::chrono::duration_cast<std::chrono::seconds>(timeDiff).count() };

        if (m_Buffer[bufferIdx].pAction == combo.pComboActions[comboIdx])
        {
            if (comboIdx > 0)
            {
                const auto allowedTimeDiff{ combo.allowedDelay[comboIdx - 1] };
                
                if (secTimeDiff >= allowedTimeDiff)
                {
                    comboIdx = 0;
                    continue;
                }
            }

            ++comboIdx;

            if (comboIdx == combo.pComboActions.size())
            {
                return true;
            }

            lastActionTime = currentActionTime;
        }
        else if(secTimeDiff > ERROR_MARGIN_TIME)
        {
            comboIdx = 0;
        }
    }

    return false;
}
