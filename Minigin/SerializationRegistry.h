#ifndef PENGIN_SERIALIZATIONREGISTRY
#define PENGIN_SERIALIZATIONREGISTRY

#include "Singleton.h"
#include "DebugOutput.h"

#include "ECS.h"
#include "GameUUID.h"
#include "FieldSerializer.h"

#include <functional>
#include <unordered_map>
#include <typeindex>
#include <cassert>
#include <iostream>

#include "EntityId.h"

namespace Pengin
{
	using SerializeFunc = std::function<void(const FieldSerializer&, const ECS&, const EntityId, std::vector<uint8_t>&)>;
	using DeSerializeFunc = std::function<void(const FieldSerializer&, ECS&, const EntityId, const std::unordered_map<std::string, std::vector<uint8_t>>&, const std::unordered_map<GameUUID, EntityId>&)>;

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

		template<typename ComponentType>
		[[nodiscard]] const SerializeFunc& GetSerFunction() const noexcept
		{
			std::type_index typeIdx{ typeid(ComponentType) };

			auto it = m_SerMap.find(typeIdx);
			
			assert(it != end(m_SerMap) && "No Serialization function");

			return it->second();
		}
		//GetFunction()

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


#define CONCATENATE_IMPL(s1, s2) s1##s2
#define CONCATENATE(s1, s2) CONCATENATE_IMPL(s1, s2)
#define REGISTER_SERIALIZATION_FUNCTION_WITH_TYPEID(ComponentType, SerializeFunc) \
    namespace { \
        static const bool CONCATENATE(ComponentType, _registerSer) = []() { \
            Pengin::SerializationRegistry::GetInstance().RegisterSerializationFunction<ComponentType>(SerializeFunc); \
            return true; \
        }(); \
    }

#define REGISTER_DESERIALIZATION_FUNCTION_WITH_TYPEID(ComponentType, DeSerializeFunc) \
    namespace { \
        static const bool CONCATENATE(ComponentType, _registerDeSer) = []() { \
            Pengin::SerializationRegistry::GetInstance().RegisterDeSerializationFunction<ComponentType>(DeSerializeFunc); \
            return true; \
        }(); \
    }

#endif