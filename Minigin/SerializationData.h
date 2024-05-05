#ifndef PENGIN_SERIALIZATIONDATA
#define PENGIN_SERIALIZATIONDATA

#include "FieldSerializer.h"

#include "GameUUID.h"
#include "EntityId.h"
#include "ECS.h"

//TODO use these structs in the fieldserializer

namespace Pengin
{
	struct SerializationData final 
	{
		SerializationData(const FieldSerializer& fieldSer, EntityId id, const ECS& ecs, const Impl::SerImplData& data) :
			fieldSer{ fieldSer },
			id{ id },
			ecs{ ecs },
			implData{ data }
		{}

		const FieldSerializer& fieldSer;

		const EntityId id;
		const ECS& ecs;

		Impl::SerImplData implData;
	};

	namespace Impl
	{
		struct SerImplData final
		{
			SerImplData(std::vector<uint8_t>& fieldVec) :
				fieldVec{ fieldVec }
			{}

			std::vector<uint8_t>& fieldVec;
		};
	}

	struct DeserializationData final
	{
		const FieldSerializer& fieldSer;

		const EntityId id;
		ECS& ecs;

		const std::unordered_map<GameUUID, EntityId>& uuid_EntityIdMap;

		Impl::DeserImplData implData;
	};

	namespace Impl
	{
		struct DeserImplData final
		{
			std::unordered_map<std::string, std::vector<uint8_t>>& fieldMap;
		};
	}
}

#endif