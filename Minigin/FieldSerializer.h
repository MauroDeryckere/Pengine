#ifndef PENGIN_FIELDSERIALIZER
#define PENGIN_FIELDSERIALIZER

#include "DebugOutput.h"

#include <limits>
#include <type_traits>
#include <cassert>
#include <iterator>
#include <utility>
#include <concepts>

#include <sstream>

#include <vector>

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
		typename T::mapped_type;
	};

	template<typename T, typename T2>
	concept ClassConcept = requires(T2& serializer, const T& object, std::vector<uint8_t>&buffer)
	{
		{ T::Serialize(serializer, object, buffer) } -> std::same_as<void>;
	}
	&& !ContainerWithIterators<T>
	&& !std::is_same_v<T, std::string>
	&& std::is_class_v<T>;
	
	template<typename T, typename T2>
	concept AllowSerTypesConcept = requires
	{
		BasicTypeConcept<T>
		|| (ClassConcept<T, T2>)
		|| (ContainerWithIterators<T>);
	};

	class FieldSerializer final
	{
	public:
		template <typename FieldType>
		void SerializeField(const char* fieldName [[maybe_unused]], const FieldType& fieldValue [[maybe_unused]], std::vector<uint8_t>& fieldsOut [[maybe_unused]]) const noexcept
		{
			static_assert(!std::is_pointer_v<FieldType> , "Can not serialize a pointer");
			static_assert(AllowSerTypesConcept<FieldType, FieldSerializer>, "Invalid Serialize field type");
			
			switch (m_SerType)
			{
				case SerializationType::Json:
				{
					SerializeFieldJson(fieldName, fieldValue, fieldsOut);
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
		void DeserializeField(const char* fieldName [[maybe_unused]], FieldType& fieldValueOut [[maybe_unused]], const std::vector<uint8_t>& serializedFields [[maybe_unused]]) const noexcept
		{
			switch (m_SerType)
			{
				case SerializationType::Json:
				{
					DeserializeFieldJson(fieldName, fieldValueOut, serializedFields);
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
		void SerializeFieldJson(const char* fieldName, const FieldType& fieldValue, std::vector<uint8_t>& fieldsOut) const noexcept
		{
			std::ostringstream oss;

			if (fieldsOut.size() != 1) //size == 1 when we start, '{'
				oss << ",";

			if constexpr (ClassConcept<FieldType, FieldSerializer>)
			{
				oss << "\"" << typeid(FieldType).name() << "\":";
			}
			else
			{
				oss << "\"" << fieldName << "\":";
			}

			SerializeFieldValJson(fieldValue, oss);

			const std::string& serializedField = oss.str();

			//DEBUG_OUT("SERIALIZED FIELD: " << serializedField);

			fieldsOut.insert(fieldsOut.end(), serializedField.begin(), serializedField.end());
		}

		template <typename FieldType>
		void SerializeFieldValJson(const FieldType& fieldValue, std::ostringstream& oss) const noexcept
		{
			if constexpr(ContainerWithIterators<FieldType>)
			{
				oss << "[";

				for  (auto it = begin(fieldValue); it != end(fieldValue); ++it)
				{
					if constexpr (AssociativeContainer<FieldType>)
					{
						oss << "[";
						SerializeFieldValJson((*it).first, oss);
						oss << ",";
						SerializeFieldValJson((*it).second, oss);
						oss << "]";
					}
					else
					{
						SerializeFieldValJson(*it, oss);
					}
					if (it != std::prev(end(fieldValue)) && fieldValue.size() >= 1)
					{
						oss << ",";
					}
				}

				oss << "]";
			}
			else if constexpr (ClassConcept<FieldType, FieldSerializer>)
			{
				oss << "{";

				std::vector<uint8_t> tempVec;
				const std::string& ser = oss.str();
				tempVec.insert(tempVec.end(), ser.begin(), ser.end());

				oss.clear();
				oss.str("");

				const size_t lastVecIdx{ tempVec.size() - 1 };
				FieldType::Serialize((*this), fieldValue, tempVec);

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
			void DeserializeFieldJson(const char* fieldName, FieldType& fieldValueOut, const std::vector<uint8_t>& serializedFields) const noexcept
			{
				fieldName;
				fieldValueOut;
				serializedFields;
				//Deserialization of a field code
			}

	};
}

#endif