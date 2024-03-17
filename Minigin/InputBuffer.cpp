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

    for (size_t bufferIdx{ 0 }; bufferIdx < m_Buffer.size(); ++bufferIdx)
    {
        if (m_Buffer[bufferIdx].pAction == combo.pComboActions[comboIdx])
        {
            if (bufferIdx > 0 && comboIdx > 0)
            {
                auto timeDiff = m_Buffer[bufferIdx].timestamp - m_Buffer[bufferIdx - 1].timestamp;
                auto allowedTimeDiff = combo.allowedDelay[comboIdx - 1];

                auto secTimeDiff = std::chrono::duration_cast<std::chrono::seconds>(timeDiff).count();

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
        }
    }

    return false;
}
