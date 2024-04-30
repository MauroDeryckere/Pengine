#ifndef PENGIN_SERIALIZATIONREGISTRY
#define PENGIN_SERIALIZATIONREGISTRY

#include "Singleton.h"
#include "DebugOutput.h"

#include <functional>
#include <unordered_map>
#include <typeindex>
#include <iostream>

#include "EntityId.h"

namespace Pengin
{
	class ECS;

	using SerializeFunc = std::function<void(const ECS&, const EntityId)>;
	using DeSerializeFunc = std::function<void(ECS&, const EntityId)>;

	class SerializationRegistry final : public Singleton<SerializationRegistry>
	{
	public:
		template<typename ComponentType>
		void RegisterSerializationFunction(const SerializeFunc& serializeFunc) noexcept
		{
			std::type_index typeIdx{typeid(ComponentType)};

			auto it = m_SerMap.find(typeIdx);

			if (it == end(m_SerMap))
			{
				DEBUG_OUT("Registering serialization function for component: " << typeIdx.name());
				m_SerMap[typeIdx] = serializeFunc;
			}
		}

		template<typename ComponentType>
		void RegisterDeSerializationFunction(const DeSerializeFunc& deserializeFunc) noexcept
		{
			std::type_index typeIdx{ typeid(ComponentType) };

			auto it = m_DeSerMap.find(typeIdx);

			if (it == end(m_DeSerMap))
			{
				DEBUG_OUT("Registering deserialization function for component: " << typeIdx.name());
				m_DeSerMap[typeIdx] = deserializeFunc;
			}
		}

		SerializationRegistry(const SerializationRegistry&) = delete;
		SerializationRegistry& operator=(const SerializationRegistry&) = delete;
		SerializationRegistry(SerializationRegistry&&) noexcept = delete;
		SerializationRegistry& operator=(SerializationRegistry&&) noexcept = delete;

	private:
		friend class Singleton<SerializationRegistry>;
		SerializationRegistry() = default;
		~SerializationRegistry() = default;

		friend class JsonSerializer;
		std::unordered_map<std::type_index, SerializeFunc> m_SerMap;
		std::unordered_map<std::type_index, DeSerializeFunc> m_DeSerMap;
	};
}

#define REGISTER_SERIALIZATION_FUNCTION(ComponentType, SerializeFunc) \
    namespace { \
        static const bool ComponentType ## _registerSer = []() { \
            Pengin::SerializationRegistry::GetInstance().RegisterSerializationFunction<ComponentType>(SerializeFunc); \
            return true; \
        }(); \
    }

#define REGISTER_DESERIALIZATION_FUNCTION(ComponentType, DeSerializeFunc) \
    namespace { \
        static const bool ComponentType ## _registerDeSer = []() { \
            Pengin::SerializationRegistry::GetInstance().RegisterDeSerializationFunction<ComponentType>(DeSerializeFunc); \
            return true; \
        }(); \
    }

#endif