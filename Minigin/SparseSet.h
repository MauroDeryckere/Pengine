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
        requires std::is_constructible_v<ValueType> && std::is_default_constructible_v<Key>
    class SparseSet final
    {
    public:
        explicit SparseSet(size_t reserveSize) : m_DenseArray{} { m_DenseArray.reserve(reserveSize); }
        SparseSet() = default;

        ~SparseSet() = default;

        SparseSet(const SparseSet&) = delete;
        SparseSet& operator=(const SparseSet&) = delete;
        SparseSet(SparseSet&& other) noexcept : m_SparseMap(std::move(other.m_SparseMap)), m_DenseArray(std::move(other.m_DenseArray)) {}
        SparseSet& operator=(SparseSet&& other) noexcept
        {
            if (this != &other)
            {
                m_SparseMap = std::move(other.m_SparseMap);
                m_DenseArray = std::move(other.m_DenseArray);
            }
            return *this;
        }
        void ReserveDense(size_t capacity) { m_DenseArray.reserve(capacity); }
        size_t DenseCapacity() { return m_DenseArray.capacity(); }
        size_t DenseSize() { return m_DenseArray.size(); }

        void Clear()
        {
            m_DenseArray.clear();
            m_SparseMap.clear();
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

        template<typename... Args>
        bool Emplace(const Key& key, Args&&... args) requires std::is_constructible_v<ValueType, Args...>
        {
            const auto [it, inserted] = m_SparseMap.emplace(key, m_DenseArray.size());

            assert(inserted && "Key already added");

            if (!inserted)
            {
                return false;
            }

            m_DenseArray.emplace_back(std::forward<Args>(args)...);
            return true;
        }

        void Remove(const Key& key)
        {
            assert(Contains(key) && "Invalid key");

            auto it{ m_SparseMap.find(key) };

            if (it != m_SparseMap.end())
            {
                const size_t index{ it->second };

                std::swap(m_DenseArray[index], m_DenseArray.back());
                m_DenseArray.pop_back();

                m_SparseMap.erase(it);
            }
        }

        bool Contains(const Key& key) const noexcept
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
    };
}

#endif