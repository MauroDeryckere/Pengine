#ifndef BITSET
#define BITSET

#include <vector>
#include <climits>
#include <cassert>
#include <exception>

#include <iostream>

namespace Pengin
{
    class Bitset final
    {
    public:
        explicit Bitset(size_t amtOfBits) noexcept :
            m_AmountOfBits{ amtOfBits },
            m_Bits(static_cast<unsigned>((amtOfBits + BITS_PER_UINT - 1) / BITS_PER_UINT))
        {
            m_Bits.shrink_to_fit();
        }

        Bitset() = default;

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

        Bitset operator&(const Bitset& other) const noexcept
        {
            Bitset result(m_AmountOfBits);

            for (size_t i{ 0 }; i < m_Bits.size(); ++i) 
            {
                result.m_Bits[i] = m_Bits[i] & other.m_Bits[i];
            }
            return result;
        }

        Bitset operator|(const Bitset& other) const noexcept
        {
            Bitset result{m_AmountOfBits};

            for (size_t i{ 0 }; i < m_Bits.size(); ++i)
            {
                result.m_Bits[i] = m_Bits[i] | other.m_Bits[i];
            }
            return result;
        }

        Bitset operator^(const Bitset& other) const noexcept
        {
            Bitset result{ m_AmountOfBits };

            for (size_t i{ 0 }; i < m_Bits.size(); ++i)
            {
                result.m_Bits[i] = m_Bits[i] ^ other.m_Bits[i];
            }
            return result;
        }
        Bitset operator~() const noexcept
        {
            Bitset result{ m_AmountOfBits };

            for (size_t i{ 0 }; i < m_Bits.size(); ++i)
            {
                result.m_Bits[i] = ~m_Bits[i];
            }
            return result;
        }

        Bitset& operator&=(const Bitset& other) noexcept
        {
            for (size_t i = 0; i < m_Bits.size(); ++i)
            {
                m_Bits[i] &= other.m_Bits[i];
            }
            return *this;
        }

        Bitset& operator|=(const Bitset& other) noexcept
        {
            for (size_t i = 0; i < m_Bits.size(); ++i)
            {
                m_Bits[i] |= other.m_Bits[i];
            }
            return *this;
        }

        Bitset& operator^=(const Bitset& other) noexcept
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

        static constexpr size_t BITS_PER_UINT{ sizeof(int) * CHAR_BIT };
	};
}

#endif
