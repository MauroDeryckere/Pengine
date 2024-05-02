#pragma once

#include "Serializer.h"
#include "EntityId.h"
#include "ECS.h"

#include <memory>
#include <list>
#include <map>
#include <iostream>
#include <deque>

#include "ServiceLocator.h"
#include "SerializationRegistry.h"
#include "FieldSerializer.h"

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

			//Simple data like a struct / class you want to serialize should have a function like this, taking in the struct
			static void Serialize(const FieldSerializer& fieldSer, const TestSructSer& serStruct, std::vector<uint8_t>& fieldVector)
			{
				//Serialize fields of the struct
				fieldSer.SerializeField("TESTDATA", serStruct.data, fieldVector);
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

		static void Serialize(const FieldSerializer& fieldSer, const ECS& ecs, const EntityId id, std::vector<uint8_t>& fieldVector)
		{
			const auto& comp = ecs.GetComponent<TestSerComponent>(id);

			fieldSer.SerializeField("test1", comp.test1, fieldVector);
			fieldSer.SerializeField("test2", comp.test2, fieldVector);
			fieldSer.SerializeField("test3", comp.test3, fieldVector);
			fieldSer.SerializeField("test4", comp.test4, fieldVector);
			fieldSer.SerializeField("test5", comp.test5, fieldVector);
			fieldSer.SerializeField("test6", comp.test6, fieldVector);

			fieldSer.SerializeField("test7", comp.test7, fieldVector);
			fieldSer.SerializeField("test8", comp.test8, fieldVector);
			fieldSer.SerializeField("test9", comp.test9, fieldVector);

			fieldSer.SerializeField("test10", comp.test10, fieldVector);
			fieldSer.SerializeField("test11", comp.test11, fieldVector);

			fieldSer.SerializeField("test12", comp.test12, fieldVector);

			fieldSer.SerializeField("enumTest", comp.enumTest, fieldVector);
			
			fieldSer.SerializeField("structTest", comp.testStructSer, fieldVector);

			fieldSer.SerializeField("TestVec", comp.testVec, fieldVector);
			fieldSer.SerializeField("TestVecOfVecs", comp.testVecOfVecs, fieldVector);

			fieldSer.SerializeField("Testlist", comp.testList, fieldVector);

			fieldSer.SerializeField("TestMap1", comp.testMap1, fieldVector);
			fieldSer.SerializeField("TestMap2", comp.testMap2, fieldVector);

			fieldSer.SerializeField("TestVecOfStructs", comp.testVecOfStructs, fieldVector);
			fieldSer.SerializeField("TestDeque", comp.testDeque, fieldVector);

			fieldSer.SerializeField("Crazy test case", comp.crazyTestCase, fieldVector);
		}

		static void DeSerialize(ECS& ecs, const EntityId id, const std::unordered_map<std::string, std::vector<uint8_t>>& serializedFields)
		{
			ecs;
			id;
			serializedFields;

			//auto& comp = ecs.AddComponent<TestSerComponent>(id);
			//auto& fieldSer = ServiceLocator::GetFieldSerializer();

			//fieldSer.DeserializeField("test", &comp.test, FieldSerializer::FieldType::Int, serializedFields);
			//fieldSer.DeserializeField("test2", &comp.test2, FieldSerializer::FieldType::Float, serializedFields);

			//DEBUG_OUT(comp.test);
			//DEBUG_OUT(comp.test2);
			//DEBUG_OUT(comp.test3);
			//DEBUG_OUT(comp.test4);
		}
		//--------------------
	};

	REGISTER_SERIALIZATION_FUNCTION(TestSerComponent, TestSerComponent::Serialize); 
	REGISTER_DESERIALIZATION_FUNCTION(TestSerComponent, TestSerComponent::DeSerialize);
}