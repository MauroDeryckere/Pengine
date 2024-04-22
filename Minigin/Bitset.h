#ifndef BITSET
#define BITSET

#include <vector>
#include <climits>
#include <cassert>
#include <exception>

#include <iostream>

namespace Pengin
{
    class RunTimeBitset final
    {
    public:
        explicit RunTimeBitset(size_t amtOfBits) noexcept :
            m_AmountOfBits{ amtOfBits },
            m_Bits(static_cast<unsigned>((amtOfBits + BITS_PER_UINT - 1) / BITS_PER_UINT))
        {
            m_Bits.shrink_to_fit();
        }

        RunTimeBitset() = default;

        RunTimeBitset(const RunTimeBitset& other) noexcept :
            m_AmountOfBits{ other.m_AmountOfBits },
            m_Bits{ other.m_Bits }
        {
        }

        RunTimeBitset(RunTimeBitset&& other) noexcept :
            m_AmountOfBits{ other.m_AmountOfBits },
            m_Bits{ std::move(other.m_Bits) }
        {
        }


        RunTimeBitset& operator=(RunTimeBitset&& other) noexcept
        {
            try
            {
                if (this != &other) 
                {
                    if (m_AmountOfBits != other.m_AmountOfBits)
                    {
                        throw std::invalid_argument("Bitset sizes do not match");
                    }
                    m_Bits = std::move(other.m_Bits);
                }
            }
            catch (const std::exception&)
            {
                //Handle exception if needed, or just let it propagate
            }
            return *this;
        }

        RunTimeBitset& operator=(const RunTimeBitset& other)
        {
            if (this != &other)
            {
                if (m_AmountOfBits != other.m_AmountOfBits)
                {
                    throw std::invalid_argument("Bitset sizes do not match");
                }
                m_Bits = other.m_Bits;
            }
            return *this;
        }

        size_t Size() const noexcept
        {
            return m_AmountOfBits;
        }

        void Set(size_t bit) noexcept
        {
            assert(bit < m_AmountOfBits && "Bit out of bitset range");

            m_Bits[static_cast<size_t>(bit / BITS_PER_UINT)] |= (1 << static_cast<unsigned>(bit % BITS_PER_UINT));
        }

        void Clear(size_t bit) noexcept
        {
            assert(bit < m_AmountOfBits && "Bit out of bitset range");

            m_Bits[static_cast<size_t>(bit / BITS_PER_UINT)] &= ~(1 << static_cast<unsigned>(bit % BITS_PER_UINT));
        }

        [[nodiscard]] bool Test(size_t bit) const noexcept
        {
            assert(bit < m_AmountOfBits && "Bit out of bitset range");

            return (m_Bits[static_cast<size_t>(bit / BITS_PER_UINT)] & (1 << static_cast<unsigned>(bit % BITS_PER_UINT))) != 0;
        }

        void PrintBitSet() const noexcept
        {
            for (size_t i{ 0 }; i < m_Bits.size(); ++i)
            {
                for (int j{ BITS_PER_UINT - 1 }; j >= 0; --j)
                {
                    std::cout << ((m_Bits[i] >> j) & 1);
                }
                std::cout << "\n";
            }
        }

        RunTimeBitset operator&(const RunTimeBitset& other) const noexcept
        {
            RunTimeBitset result(m_AmountOfBits);

            for (size_t i{ 0 }; i < m_Bits.size(); ++i)
            {
                result.m_Bits[i] = m_Bits[i] & other.m_Bits[i];
            }

            return result;
        }

        RunTimeBitset operator|(const RunTimeBitset& other) const noexcept
        {
            RunTimeBitset result{m_AmountOfBits};

            for (size_t i{ 0 }; i < m_Bits.size(); ++i)
            {
                result.m_Bits[i] = m_Bits[i] | other.m_Bits[i];
            }
            return result;
        }

        RunTimeBitset operator^(const RunTimeBitset& other) const noexcept
        {
            RunTimeBitset result{ m_AmountOfBits };

            for (size_t i{ 0 }; i < m_Bits.size(); ++i)
            {
                result.m_Bits[i] = m_Bits[i] ^ other.m_Bits[i];
            }
            return result;
        }
        RunTimeBitset operator~() const noexcept
        {
            RunTimeBitset result{ m_AmountOfBits };

            for (size_t i{ 0 }; i < m_Bits.size(); ++i)
            {
                result.m_Bits[i] = ~m_Bits[i];
            }
            return result;
        }

        RunTimeBitset& operator&=(const RunTimeBitset& other) noexcept
        {
            for (size_t i = 0; i < m_Bits.size(); ++i)
            {
                m_Bits[i] &= other.m_Bits[i];
            }
            return *this;
        }

        RunTimeBitset& operator|=(const RunTimeBitset& other) noexcept
        {
            for (size_t i = 0; i < m_Bits.size(); ++i)
            {
                m_Bits[i] |= other.m_Bits[i];
            }
            return *this;
        }

        RunTimeBitset& operator^=(const RunTimeBitset& other) noexcept
        {
            for (size_t i = 0; i < m_Bits.size(); ++i)
            {
                m_Bits[i] ^= other.m_Bits[i];
            }
            return *this;
        }

	private:
		const size_t m_AmountOfBits;
		std::vector<unsigned> m_Bits;

        constinit static const size_t BITS_PER_UINT{ sizeof(int) * CHAR_BIT };
	};
}

#endif
