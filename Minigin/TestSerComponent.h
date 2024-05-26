#pragma once

#include "Serializer.h"
#include "EntityId.h"
#include "ECS.h"

#include <memory>
#include <list>
#include <map>
#include <iostream>
#include <deque>

#include "SerializationRegistry.h"

namespace Pengin
{
	struct TestSerComponent final
	{
		enum class TestEnum : int8_t
		{
			Test = 0,
			TestNumTwo = 1,
			TestNumThree = 2
		};

		struct TestSructSer
		{
			int data = 1000;
			std::string dataStr = "TESTTEST";
			static void Serialize(const FieldSerializer& fieldSer, const TestSructSer& serStruct, std::vector<uint8_t>& fieldVector, const ECS& ecs)
			{
				fieldSer.SerializeField("TESTDATA", serStruct.data, ecs, fieldVector);
				fieldSer.SerializeField("TESSTRING", serStruct.dataStr, ecs, fieldVector);
			}
			static void Deserialize(const FieldSerializer& fieldSer, TestSructSer& deserStruct, const std::unordered_map<std::string, std::vector<uint8_t>>& fields, const std::unordered_map<GameUUID, EntityId>& entityMap [[maybe_unused]])
			{
				fieldSer.DeserializeField("TESTDATA", deserStruct.data, fields, entityMap);
				fieldSer.DeserializeField("TESSTRING", deserStruct.dataStr, fields, entityMap);
			}
		};

		uint8_t test1 = 2;
		uint16_t test2 = 2;
		uint32_t test3 = 2;
		int8_t test4 = 2;
		int16_t test5 = 2;
		int32_t test6 = 2;		
		
		bool test7 = false;
		char test8 = 'a';

		std::string test9 = "Test for string";
		
		float test10 = 10.209f;
		double test11 = 10.20000;

		int64_t test12 = 200;

		TestEnum enumTest{ TestEnum::TestNumThree };

		TestSructSer testStructSer{ };

		std::vector<int> testVec{2,3,4};
		std::vector<std::vector<int>> testVecOfVecs{ {1,2,3}, {4,5,6} };

		std::list<int> testList{1,2,3};

		std::map<std::string, int> testMap1{ {"Test", 1},  {"Test2", 2}};
		std::unordered_map<int, int> testMap2{ {10, 1},  {20, 2} };

		std::vector<TestSructSer> testVecOfStructs{ TestSructSer{10}, TestSructSer{20} };

		std::deque<int> testDeque{ 10, 20, 30 };

		std::vector<std::unordered_map<std::string, std::vector<std::vector<std::list<std::map<int, int>>>>>> crazyTestCase = {{ {"Test", {{{{{1,1}}}}}}} };

		std::vector<std::string> testVecOfStr {"test", "test2"};

		static void Serialize(const FieldSerializer& fieldSer, const ECS& ecs, const EntityId id, std::vector<uint8_t>& fieldVector)
		{
			const auto& comp = ecs.GetComponent<TestSerComponent>(id);

			fieldSer.SerializeField("test1", comp.test1, ecs, fieldVector);
			fieldSer.SerializeField("test2", comp.test2, ecs, fieldVector);
			fieldSer.SerializeField("test3", comp.test3, ecs, fieldVector);
			fieldSer.SerializeField("test4", comp.test4, ecs, fieldVector);
			fieldSer.SerializeField("test5", comp.test5, ecs, fieldVector);
			fieldSer.SerializeField("test6", comp.test6, ecs, fieldVector);

			fieldSer.SerializeField("test7", comp.test7,ecs, fieldVector);
			fieldSer.SerializeField("test8", comp.test8, ecs, fieldVector);
			fieldSer.SerializeField("test9", comp.test9, ecs, fieldVector);

			fieldSer.SerializeField("test10", comp.test10,ecs,  fieldVector);
			fieldSer.SerializeField("test11", comp.test11, ecs, fieldVector);

			fieldSer.SerializeField("test12", comp.test12, ecs, fieldVector);

			fieldSer.SerializeField("enumTest", comp.enumTest, ecs, fieldVector);
			
			fieldSer.SerializeField("structTest", comp.testStructSer, ecs, fieldVector);

			fieldSer.SerializeField("TestVec", comp.testVec, ecs, fieldVector);
			fieldSer.SerializeField("TestVecOfVecs", comp.testVecOfVecs, ecs, fieldVector);

			fieldSer.SerializeField("Testlist", comp.testList, ecs, fieldVector);

			fieldSer.SerializeField("TestMap1", comp.testMap1, ecs, fieldVector);
			fieldSer.SerializeField("TestMap2", comp.testMap2, ecs, fieldVector);

			fieldSer.SerializeField("TestVecOfStructs", comp.testVecOfStructs, ecs, fieldVector);
			fieldSer.SerializeField("TestDeque", comp.testDeque, ecs, fieldVector);

			fieldSer.SerializeField("Crazy test case", comp.crazyTestCase, ecs, fieldVector);

			fieldSer.SerializeField("vecOfStr", comp.testVecOfStr, ecs, fieldVector);
		}

		static void Deserialize(const FieldSerializer& fieldSer, ECS& ecs, const EntityId id, const std::unordered_map<std::string, std::vector<uint8_t>>& serializedFields, const std::unordered_map<GameUUID, EntityId>& entityMap [[maybe_unused]])
		{
			auto& comp = ecs.AddComponent<TestSerComponent>(id);

			fieldSer.DeserializeField("test1", comp.test1, serializedFields, entityMap);
			fieldSer.DeserializeField("test2", comp.test2, serializedFields, entityMap);
			fieldSer.DeserializeField("test3", comp.test3, serializedFields, entityMap);
			fieldSer.DeserializeField("test4", comp.test4, serializedFields, entityMap);
			fieldSer.DeserializeField("test5", comp.test5, serializedFields, entityMap);
			fieldSer.DeserializeField("test6", comp.test6, serializedFields, entityMap);

			fieldSer.DeserializeField("test7", comp.test7, serializedFields, entityMap);
			fieldSer.DeserializeField("test8", comp.test8, serializedFields, entityMap);
			fieldSer.DeserializeField("test9", comp.test9, serializedFields, entityMap);

			fieldSer.DeserializeField("test10", comp.test10, serializedFields, entityMap);
			fieldSer.DeserializeField("test11", comp.test11, serializedFields, entityMap);

			fieldSer.DeserializeField("test12", comp.test12, serializedFields, entityMap);

			fieldSer.DeserializeField("enumTest", comp.enumTest, serializedFields, entityMap);

			fieldSer.DeserializeField("structTest", comp.testStructSer, serializedFields, entityMap);

			fieldSer.DeserializeField("TestVec", comp.testVec, serializedFields, entityMap);
			fieldSer.DeserializeField("TestVecOfVecs", comp.testVecOfVecs, serializedFields, entityMap);
			
			fieldSer.DeserializeField("Testlist", comp.testList, serializedFields, entityMap);

			fieldSer.DeserializeField("TestMap1", comp.testMap1, serializedFields, entityMap);
			fieldSer.DeserializeField("TestMap2", comp.testMap2, serializedFields, entityMap);

			fieldSer.DeserializeField("TestVecOfStructs", comp.testVecOfStructs, serializedFields, entityMap);
			fieldSer.DeserializeField("TestDeque", comp.testDeque, serializedFields, entityMap);

			fieldSer.DeserializeField("Crazy test case", comp.crazyTestCase, serializedFields, entityMap);

			fieldSer.DeserializeField("vecOfStr", comp.testVecOfStr, serializedFields, entityMap);
		}
	};

	REGISTER_SERIALIZATION_FUNCTION(TestSerComponent, TestSerComponent::Serialize); 
	REGISTER_DESERIALIZATION_FUNCTION(TestSerComponent, TestSerComponent::Deserialize);
}