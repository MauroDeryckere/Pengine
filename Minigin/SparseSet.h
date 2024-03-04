#ifndef SPARSESET
#define SPARSESET

#include <utility>
#include <type_traits>

#include <cassert>
#include <stdexcept>

#include <unordered_map>
#include <vector>
#include <iostream>

namespace Pengin
{
    template<typename ValueType, typename Key>
        requires std::is_constructible_v<ValueType>&& std::is_default_constructible_v<Key>
    class SparseSet final
    {
    public:
        explicit SparseSet(size_t reserveSize) : m_DenseArray{}, m_ReverseMapping{} { DenseReserve(reserveSize); }
        SparseSet() = default;

        ~SparseSet() = default;

        SparseSet(const SparseSet&) = delete;
        SparseSet& operator=(const SparseSet&) = delete;
        SparseSet(SparseSet&& other) noexcept : 
            m_SparseMap(std::move(other.m_SparseMap)), m_DenseArray(std::move(other.m_DenseArray)), m_ReverseMapping(std::move(other.m_ReverseMapping)) {}
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
        size_t DenseCapacity() const noexcept { return m_DenseArray.capacity(); }
        size_t DenseSize() const noexcept { return m_DenseArray.size(); }

        void Clear()
        {
            m_DenseArray.clear();
            m_SparseMap.clear();
            m_ReverseMapping.clear();
        };

        ValueType& operator[](const Key& key)
        {
            assert(Contains(key) && "Invalid key");

            const size_t index{ m_SparseMap[key] };
            return m_DenseArray[index];
        }

        const ValueType& operator[](const Key& key) const
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
        
        //GetFrom Dense Idx - TODO

        template<typename... Args>
        auto Emplace(const Key& key, Args&&... args) noexcept
            requires std::is_constructible_v<ValueType, Args...>
        {
            const auto [it, inserted] = m_SparseMap.emplace(key, m_DenseArray.size());

            assert(inserted && "Key already added");

            if (!inserted)
            {
                return m_DenseArray.end();
            }    

            m_DenseArray.emplace_back(std::forward<Args>(args)...);
            m_ReverseMapping.emplace_back(key);

            return std::prev(m_DenseArray.end());
        }

        void Remove(const Key& key) noexcept
        {
            assert(Contains(key) && "Invalid key");

            auto it{ m_SparseMap.find(key) };

            if (it != m_SparseMap.end())
            {
                const size_t index{ it->second };
                const Key lastKey{ m_ReverseMapping.back() };

                m_SparseMap[lastKey] = index;
                m_SparseMap.erase(it);

                std::swap(m_DenseArray[index], m_DenseArray.back());
                std::swap(m_ReverseMapping[index], m_ReverseMapping.back());

                m_ReverseMapping.pop_back();
                m_DenseArray.pop_back();
            }
        }

        [[nodiscard]] bool Contains(const Key& key) const noexcept
        {
            return m_SparseMap.find(key) != m_SparseMap.end();
        }

        //Allow iteration over the dense array
        using iterator = typename std::vector<ValueType>::iterator;
        using const_iterator = typename std::vector<ValueType>::const_iterator;

        iterator begin() noexcept { return m_DenseArray.begin(); }
        iterator end() noexcept { return m_DenseArray.end(); }
        const_iterator cbegin() const noexcept { return m_DenseArray.cbegin(); }
        const_iterator cend() const noexcept { return m_DenseArray.cend(); }

    private:
        std::unordered_map<Key, size_t> m_SparseMap; //Map key to dense array index
        std::vector<ValueType> m_DenseArray;

        std::vector<Key> m_ReverseMapping; //Retrieve the key from the densearray using reverse mapping
    };
}
#endif