#pragma once

#include "Serializer.h"
#include "EntityId.h"
#include "ECS.h"

#include <iostream>

namespace Pengin
{
	struct TestSerComponent final
	{
		int test = 10;

		//TODO maybe it can be simplified && actual serialization logic (return string,... (to be decided))
		static void Serialize(const ECS& ecs, const EntityId id)
		{
			const auto& comp = ecs.GetComponent<TestSerComponent>(id);
			comp;
			//SerializeField("test", &comp.test, FieldType::Int);
		}

		static void DeSerialize(ECS& ecs, const EntityId id)
		{
			ecs;
			id;

			std::cout << "DeSerialized TestSerComponent: " << "\n";
		}

	};

	REGISTER_SERIALIZATION_FUNCTION(TestSerComponent, TestSerComponent::Serialize); 
	REGISTER_DESERIALIZATION_FUNCTION(TestSerComponent, TestSerComponent::DeSerialize);
}