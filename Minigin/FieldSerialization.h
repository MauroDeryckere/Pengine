#ifndef FIELDSERIALIZATION
#define FIELDSERIALIZATION

#include "DebugOutput.h"

namespace Pengin
{
	enum class FieldType
	{
		Int,
		Invalid
	};


	inline void SerializeField(const char* fieldName, const void* fieldValue, FieldType type) noexcept
	{
		fieldName;
		fieldValue;
		type;

		switch (type)
		{
		case Pengin::FieldType::Int:
		{
			const int* val [[maybe_unused]] = static_cast<const int*>(fieldValue);
			DEBUG_OUT(fieldName << " " << (*val) << " serialized");
			break;
		}
		case Pengin::FieldType::Invalid:
			break;

		default:
			break;
		}
	}
}

#endif