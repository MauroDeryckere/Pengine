#ifndef SPARSESET
#define SPARSESET

#include <utility>
#include <type_traits>

#include <cassert>
#include <stdexcept>

#include <unordered_map>
#include <vector>
#include <algorithm>
#include <iostream>

namespace Pengin
{
    //TODO, expand conceppts, mayb provide a 'stable' sparse set where we use erase
    template<typename ValueType, typename KeyType>
        requires std::is_default_constructible_v<KeyType> && 
                 std::is_move_constructible_v<KeyType> &&
                
                (std::is_move_constructible_v <ValueType> || std::is_move_assignable_v<ValueType>) &&
                (std::is_copy_constructible_v<ValueType> || std::is_copy_assignable_v<ValueType>) 
    class SparseSet final
    {
    public:
        explicit SparseSet(size_t reserveSize) noexcept 
            : m_DenseArray{}, m_ReverseMapping{} 
        { 
            DenseReserve(reserveSize); 
        }

        SparseSet() noexcept = default;
        ~SparseSet() = default;

        SparseSet(const SparseSet&) = delete;
        SparseSet& operator=(const SparseSet&) = delete;
        SparseSet(SparseSet&& other) noexcept : 
            m_SparseMap(std::move(other.m_SparseMap)), 
            m_DenseArray(std::move(other.m_DenseArray)), 
            m_ReverseMapping(std::move(other.m_ReverseMapping)) { }

        SparseSet& operator=(SparseSet&& other) noexcept
        {
            if (this != &other)
            {
                m_SparseMap = std::move(other.m_SparseMap);
                m_DenseArray = std::move(other.m_DenseArray);
                m_ReverseMapping = std::move(other.m_ReverseMapping);
            }
            return *this;
        }

        void DenseReserve(size_t capacity) noexcept { m_DenseArray.reserve(capacity); m_ReverseMapping.reserve(capacity); }
        [[nodiscard]] size_t DenseCapacity() const noexcept { return m_DenseArray.capacity(); }
        [[nodiscard]] size_t DenseSize() const noexcept { return m_DenseArray.size(); }

        void Clear() noexcept
        {
            m_DenseArray.clear();
            m_SparseMap.clear();
            m_ReverseMapping.clear();
        };

        //Allow iteration over the dense array
        using iterator = typename std::vector<ValueType>::iterator;
        using const_iterator = typename std::vector<ValueType>::const_iterator;

        iterator begin() noexcept { return m_DenseArray.begin(); }
        iterator end() noexcept { return m_DenseArray.end(); }
        const_iterator cbegin() const noexcept { return m_DenseArray.cbegin(); }
        const_iterator cend() const noexcept { return m_DenseArray.cend(); }

        ValueType& operator[](const KeyType& key)
        {
            assert(Contains(key) && "Invalid key");

            auto it{ m_SparseMap.find(key) };
            if (it != m_SparseMap.end())
            {
                const size_t index{ it->second };
                return m_DenseArray[index];
            }

            throw std::out_of_range("Key not found in SparseSet");
        }

        const ValueType& operator[](const KeyType& key) const
        {
            assert(Contains(key) && "Invalid key");

            auto it{ m_SparseMap.find(key) };
            if (it != m_SparseMap.end())
            {
                const size_t index{ it->second };
                return m_DenseArray[index];
            }

            throw std::out_of_range("Key not found in SparseSet");
        }

        [[nodiscard]] const KeyType& GetKeyFromIterator(const_iterator it) const noexcept
        {
            const size_t index{ std::distance(cbegin(), it) };
            
            return GetKeyFromDenseIndex(index);
        }

        template<typename... Args>
        std::pair<iterator, bool> Emplace(const KeyType& key, Args&&... args) noexcept
            requires std::is_constructible_v<ValueType, Args...>
        {
            const auto [it, inserted] = m_SparseMap.emplace(key, m_DenseArray.size());

            assert(inserted && "Key already added");

            if (!inserted)
            {
                return { m_DenseArray.end(), false };
            }    

            const size_t oldCapacity{ m_DenseArray.capacity() };

            m_DenseArray.emplace_back(std::forward<Args>(args)...);
            m_ReverseMapping.emplace_back(key);

            return { std::prev(m_DenseArray.end()), (oldCapacity <= m_DenseArray.capacity()) };
        }

        void Remove(const KeyType& key) noexcept
        {
            assert(Contains(key) && "Invalid key");

            auto it{ m_SparseMap.find(key) };

            if (it != m_SparseMap.end())
            {
                const size_t index{ it->second };
                const KeyType lastKey{ m_ReverseMapping.back() };

                m_SparseMap[lastKey] = index;
                m_SparseMap.erase(it);
                
                if constexpr(std::is_move_assignable_v<ValueType>)
                {
                    m_DenseArray[index] = std::move(m_DenseArray.back());
                }
                else if constexpr(std::is_move_constructible_v<ValueType>)
                {
                    std::cout << "moveconstruct \n";
                    new (&m_DenseArray[index]) ValueType(std::move(m_DenseArray.back()));
                }

                m_ReverseMapping[index] = m_ReverseMapping.back();

                m_ReverseMapping.pop_back();
                m_DenseArray.pop_back();
            }
        }

        [[nodiscard]] bool Contains(const KeyType& key) const noexcept
        {
            return m_SparseMap.find(key) != m_SparseMap.end();
        }

    private:
        std::unordered_map<KeyType, size_t> m_SparseMap; //Map key to dense array index
        std::vector<ValueType> m_DenseArray;

        std::vector<KeyType> m_ReverseMapping; //Retrieve the key from the densearray using reverse mapping

        [[nodiscard]] inline const KeyType& GetKeyFromDenseIndex(size_t index) const noexcept
        {
            assert(index < DenseSize() && "Index out of bounds");

            return m_ReverseMapping[index];
        };
    };
}
#endif