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

    /*
    // Bitwise OR operator
    MyBitset operator|(const MyBitset& other) const {
        MyBitset result(m_size);
        for (size_t i = 0; i < m_bits.size(); ++i) {
            result.m_bits[i] = m_bits[i] | other.m_bits[i];
        }
        return result;
    }

    // Bitwise XOR operator
    MyBitset operator^(const MyBitset& other) const {
        MyBitset result(m_size);
        for (size_t i = 0; i < m_bits.size(); ++i) {
            result.m_bits[i] = m_bits[i] ^ other.m_bits[i];
        }
        return result;
    }

    // Bitwise complement operator
    MyBitset operator~() const {
        MyBitset result(m_size);
        for (size_t i = 0; i < m_bits.size(); ++i) {
            result.m_bits[i] = ~m_bits[i];
        }
        return result;
    }*/


        /*
            // Bitwise AND assignment operator
    MyBitset& operator&=(const MyBitset& other) {
        for (size_t i = 0; i < m_bits.size(); ++i) {
            m_bits[i] &= other.m_bits[i];
        }
        return *this;
    }
        */

	private:
		size_t m_AmountOfBits;
		std::vector<unsigned> m_Bits;

        static constexpr size_t BITS_PER_UINT{ sizeof(int) * CHAR_BIT };
	};
}

#endif
