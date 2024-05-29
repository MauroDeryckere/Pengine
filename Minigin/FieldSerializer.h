#ifndef PENGIN_FIELDSERIALIZER
#define PENGIN_FIELDSERIALIZER

#include "ServiceLocator.h"
#include "GameUUID.h"
#include "EntityId.h"
#include "DebugOutput.h"

#include <limits>
#include <type_traits>
#include <cassert>
#include <iterator>
#include <utility>
#include <concepts>

#include <sstream>

#include <vector>
#include <unordered_map>

/*
TODO
Testing with multimap and possibily adjusting the code
Testing with custom containers (need to implment iterators so that they bahave in a similar way as STL containers)

Providing a binary save / load system
*/

namespace Pengin
{
	template<typename T>
	concept BasicTypeConcept = requires 
	{
		std::is_fundamental_v<T> || std::is_enum_v<T> || std::is_same_v<T, std::string>;
	};

	template<typename T>
	concept ContainerWithIterators = requires(T t)
	{
		typename std::iterator_traits<decltype(std::begin(t))>::iterator_category;
	}
	&& !std::is_same_v<T, std::string>;

	template<typename T>
	concept AssociativeContainer = requires(T t)
	{
		ContainerWithIterators<T>;
		typename T::key_type;
		typename T::value_type;
		typename T::mapped_type;
	};

	template<typename T, typename T2>
	concept SerClassConcept = requires(T2& serializer, const T& object, std::vector<uint8_t>&buffer, const ECS & ecs)
	{
		{ T::Serialize(serializer, object, buffer, ecs) } -> std::same_as<void>;
	}
	&& !ContainerWithIterators<T>
	&& !std::is_same_v<T, std::string>
	&& std::is_class_v<T>;

	template<typename T, typename T2>
	concept DeserClassConcept = requires(T2& serializer, T& object, const std::unordered_map<std::string, std::vector<uint8_t>>& buffer, const std::unordered_map<GameUUID, EntityId>& entityMap)
	{
		{ T::Deserialize(serializer, object, buffer, entityMap) } -> std::same_as<void>;
	}
	&& !ContainerWithIterators<T>
	&& !std::is_same_v<T, std::string>
	&& std::is_class_v<T>;
	
	template<typename T, typename T2>
	concept AllowedSerTypesConcept = requires
	{
		BasicTypeConcept<T>
		|| (SerClassConcept<T, T2>)
		|| (ContainerWithIterators<T>);
	};


	template<typename T>
	concept EmplBackConcept = requires(T t)
	{
		{ t.emplace_back(std::declval<const typename T::value_type&>()) };
	};

	template<typename T>
	concept AssociativeInsConcept = requires(T t, const typename T::key_type & key)
	{
		{ t[key] } /*-> std::convertible_to<typename T::mapped_type&>*/;
	};

	template<typename T, typename T2>
	concept AllowedDeserTypesConcept = requires(T t)
	{
		BasicTypeConcept<T>
		|| (DeserClassConcept<T, T2>)
		|| ( (EmplBackConcept<T> && ContainerWithIterators<T>) 
		  || (AssociativeInsConcept<T> && AssociativeContainer<T>) );
	};

	class FieldSerializer final
	{
	public:
		template <typename FieldType>
		void SerializeField(const char* fieldName [[maybe_unused]], const FieldType& fieldValue [[maybe_unused]], const ECS& ecs [[maybe_unused]], std::vector<uint8_t>& fieldsOut [[maybe_unused]] ) const noexcept
		{
			static_assert(!std::is_pointer_v<FieldType> , "Can not serialize a pointer");
			static_assert(AllowedSerTypesConcept<FieldType, FieldSerializer>, "Invalid serialize field type");
			
			switch (m_SerType)
			{
				case SerializationType::Json:
				{
					SerializeFieldJson(fieldName, fieldValue, ecs ,fieldsOut);
					break;
				}
				case SerializationType::Invalid:
				{
					DEBUG_OUT("Invalid Serialization type");
					break;
				}
			}
		}

		template<typename FieldType>
		void DeserializeField(const char* fieldName [[maybe_unused]], FieldType& fieldValueOut [[maybe_unused]], const std::unordered_map<std::string, std::vector<uint8_t>>& fields [[maybe_unused]], const std::unordered_map<GameUUID, EntityId>& entityMap [[maybe_unused]]) const noexcept
		{
			static_assert(!std::is_pointer_v<FieldType>, "Can not deserialize a pointer");
			static_assert(AllowedDeserTypesConcept<FieldType, FieldSerializer>, "Invalid deserialize field type");

			switch (m_SerType)
			{
				case SerializationType::Json:
				{
					DeserializeFieldJson(fieldName, fieldValueOut, fields, entityMap);
					break;
				}
				case SerializationType::Invalid:
				{
					DEBUG_OUT("Invalid Serialization type");
					break;
				}
			}
		}

	private:
		enum class SerializationType : uint8_t
		{
			Json,
			Invalid
		};

		friend class JsonSerializer;
		FieldSerializer(SerializationType serType) :
			m_SerType{ serType }
		{
			DEBUG_OUT("Initialized fieldSer");
		}
		~FieldSerializer() = default;

		const SerializationType m_SerType{ SerializationType::Invalid };

	private:
		template <typename FieldType>
		void SerializeFieldJson(const char* fieldName, const FieldType& fieldValue, const ECS& ecs, std::vector<uint8_t>& fieldsOut) const noexcept
		{
			std::ostringstream oss;

			if (fieldsOut.size() != 1) //size == 1 when we start, '{'
				oss << ",";


			oss << "\"" << fieldName << "\":";

			SerializeFieldValJson(fieldValue, ecs, oss);

			const std::string& serializedField = oss.str();

			//DEBUG_OUT("SERIALIZED FIELD: " << serializedField);

			fieldsOut.insert(fieldsOut.end(), serializedField.begin(), serializedField.end());
		}

		template <typename FieldType>
		void SerializeFieldValJson(const FieldType& fieldValue, const ECS& ecs [[maybe_unused]], std::ostringstream& oss) const noexcept
		{
			if constexpr(ContainerWithIterators<FieldType>)
			{
				oss << "[";

				for  (auto it = begin(fieldValue); it != end(fieldValue); ++it)
				{
					if constexpr (AssociativeContainer<FieldType>)
					{
						oss << "[";
						SerializeFieldValJson((*it).first, ecs, oss);
						oss << ",";
						SerializeFieldValJson((*it).second, ecs, oss);
						oss << "]";
					}
					else
					{
						SerializeFieldValJson(*it, ecs, oss);
					}
					if (it != std::prev(end(fieldValue)) && fieldValue.size() >= 1)
					{
						oss << ",";
					}
				}

				oss << "]";
			}
			else if constexpr (SerClassConcept<FieldType, FieldSerializer>)
			{
				oss << "{";

				std::vector<uint8_t> tempVec;
				const std::string& ser = oss.str();
				tempVec.insert(tempVec.end(), ser.begin(), ser.end());

				oss.clear();
				oss.str("");

				const size_t lastVecIdx{ tempVec.size() - 1 };
				FieldType::Serialize((*this), fieldValue, tempVec, ecs);

				tempVec.erase(tempVec.begin() + lastVecIdx + 1);

				const std::string convertedStr(reinterpret_cast<const char*>(tempVec.data()), tempVec.size());
				oss << convertedStr;

				oss << "}";
			}
			else if constexpr (std::is_same_v<FieldType, std::string>)
			{
				oss << "\"" << fieldValue << "\"";
			}
			else if constexpr (std::is_same_v<FieldType, bool>)
			{
				oss << (fieldValue ? "true" : "false");
			}
			else if constexpr (std::numeric_limits<FieldType>::is_integer && std::is_signed_v<FieldType> && sizeof(FieldType) == 1)
			{
				oss << static_cast<int32_t>(fieldValue); //need to cast to a larger type for << operator when 8 bit type
			}
			else if constexpr (std::numeric_limits<FieldType>::is_integer && std::is_unsigned_v<FieldType> && sizeof(FieldType) == 1)
			{
				oss << static_cast<uint32_t>(fieldValue);
			}
			else if constexpr (std::is_enum_v<FieldType>)
			{
				if constexpr (std::numeric_limits<std::underlying_type_t<FieldType>>::is_integer && std::is_unsigned_v<std::underlying_type_t<FieldType>> && sizeof(std::numeric_limits < std::underlying_type_t<FieldType>>) == 1)
				{
					oss << static_cast<uint32_t>(fieldValue);
				}
				else if constexpr (std::numeric_limits<std::underlying_type_t<FieldType>>::is_integer && std::is_signed_v<std::underlying_type_t<FieldType>> && sizeof(std::numeric_limits < std::underlying_type_t<FieldType>>) == 1)
				{
					oss << static_cast<int32_t>(fieldValue);
				}
				else
				{
					oss << static_cast<std::underlying_type_t<FieldType>>(fieldValue);
				}
			}
			else
			{
				oss << fieldValue;
			}
		}

	private:
		template<typename FieldType>
		void DeserializeFieldJson(const char* fieldName, FieldType& fieldValueOut, const std::unordered_map<std::string, std::vector<uint8_t>>& fields, const std::unordered_map<GameUUID, EntityId>& entityMap) const noexcept
		{
			auto fieldIt = fields.find(fieldName);
			assert(fieldIt != end(fields) && "Fieldname not found in the fieldsMap, incorrect name or never serialized");

			const auto& field{ fieldIt->second };
			const std::string fieldStr(reinterpret_cast<const char*>(field.data()), field.size());

			DeserializeValJson(fieldStr, fieldValueOut, entityMap);
		}

		template<typename FieldType>
		void DeserializeValJson(const std::string& fieldStr, FieldType& fieldValueOut, const std::unordered_map<GameUUID, EntityId>& entityMap [[maybe_unused]]) const noexcept
		{
			if constexpr (ContainerWithIterators<FieldType>)
			{
				if constexpr (AssociativeInsConcept<FieldType> && AssociativeContainer<FieldType>) //need [key]
				{
					const auto tempFieldsMap = ServiceLocator::GetSerializer().ParseJsonStr(fieldStr);

					FieldType container{};
					for (const auto& [idx, value] : tempFieldsMap)
					{
						typename FieldType::key_type key{};
						typename FieldType::mapped_type element{};

						const std::string convFieldStr{ reinterpret_cast<const char*>(value.data()), value.size() };
						const auto mappedPairMap = ServiceLocator::GetSerializer().ParseJsonStr(convFieldStr);
						
						const auto& mappedKey = mappedPairMap.at("0"); 
						const auto& mappedVal = mappedPairMap.at("1");

						std::string convMappKeyStr{ reinterpret_cast<const char*>(mappedKey.data()), mappedKey.size() };
						std::string convMappedValStr{ reinterpret_cast<const char*>(mappedVal.data()), mappedVal.size() };

						DeserializeValJson(convMappKeyStr, key, entityMap);
						DeserializeValJson(convMappedValStr, element, entityMap);

						container[key] = element;
					}
					fieldValueOut = container;
				}
				else if constexpr(EmplBackConcept<FieldType>) //Need container::emplace_back
				{
					const auto tempFieldsMap = ServiceLocator::GetSerializer().ParseJsonStr(fieldStr);

					FieldType container{};
					if constexpr (requires(FieldType a, size_t i) { a[i]; }&& requires(FieldType a, size_t s) { a.resize(s); })
					{
						//std::ordered_map may allow us to get rid of this resize which could be faster - TODO, compare performace
						container.resize(tempFieldsMap.size());
					}

					for (const auto& [key, value] : tempFieldsMap) 
					{
						//idx is required to ensure order of json file == order of container
						const auto idx = static_cast<size_t>(std::stoi(key));

						static_assert(std::is_default_constructible_v<typename FieldType::value_type>);
						typename FieldType::value_type element{};

						std::string convFieldStr(reinterpret_cast<const char*>(value.data()), value.size());

						DeserializeValJson(convFieldStr, element, entityMap);

						if constexpr (requires(FieldType a, size_t i) { a[i]; } && requires(FieldType a, size_t s) { a.resize(s); })
						{
							container[idx] = element;
						}
						else 
						{
							auto it = container.begin();
							std::advance(it, idx);
							*it = element;
						}
					}
					fieldValueOut = container;
				}
			}
			else if constexpr (DeserClassConcept<FieldType, FieldSerializer>)
			{
				const auto tempFieldsMap = ServiceLocator::GetSerializer().ParseJsonStr(fieldStr);
				FieldType::Deserialize((*this), fieldValueOut, tempFieldsMap, entityMap);
			}
			else if constexpr (BasicTypeConcept<FieldType>)
			{
				if constexpr (std::is_enum_v<FieldType>)
				{
					std::stringstream ss(fieldStr);
					std::underlying_type_t<FieldType> temp{};
					ss >> temp;

					fieldValueOut = static_cast<FieldType>(temp);
				}
				else
				{
					if constexpr (std::is_same_v<FieldType, std::string>)
					{
						fieldValueOut = fieldStr;
					}
					else if constexpr (std::is_same_v<FieldType, bool>)
					{
						fieldValueOut = fieldStr == "true" ? true : false;
					}
					else if constexpr ( std::numeric_limits<FieldType>::is_integer && sizeof(FieldType) == 1)
					{
						fieldValueOut = static_cast<FieldType>(std::stoi(fieldStr));
					}
					else 
					{
						std::stringstream ss(fieldStr); //ss handles the basic types conversions
						ss >> fieldValueOut;
					}
				}
			}
		}
	};
}

#endif