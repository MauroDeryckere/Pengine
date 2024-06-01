#ifndef PENGIN_GRIDCOMPONENT
#define PENGIN_GRIDCOMPONENT

#include <vector>
#include <cassert>
#include <utility>   
#include <type_traits> 

#include "SerializationRegistry.h"
#include "UUIDComponent.h"

namespace Pengin
{
	struct GridCellData final
	{
		uint8_t type{ 0 };
		EntityId entity{ NULL_ENTITY_ID };

		GridCellData() = default;
		GridCellData(uint8_t t) :
			type{ t },
			entity{ NULL_ENTITY_ID }
		{}

		~GridCellData() = default;
		
		void Reset() noexcept
		{
			type = 0;
			entity = NULL_ENTITY_ID;
		}

		static void Serialize(const FieldSerializer& fieldSer, const GridCellData& serStruct, std::vector<uint8_t>& fieldVector, const ECS& ecs)
		{
			fieldSer.SerializeField("Type", serStruct.type, ecs, fieldVector);
			fieldSer.SerializeField("EntityId", serStruct.entity  == NULL_ENTITY_ID ? "NULL_UUID" : ecs.GetComponent<UUIDComponent>(serStruct.entity).uuid.GetUUID_PrettyStr(), ecs, fieldVector);
		}
		static void Deserialize(const FieldSerializer& fieldSer, GridCellData& deserStruct, const std::unordered_map<std::string, std::vector<uint8_t>>& fields, const std::unordered_map<GameUUID, EntityId>& entityMap [[maybe_unused]] )
		{
			fieldSer.DeserializeField("Type", deserStruct.type, fields, entityMap);
			
			std::string uuidStr{ };
			fieldSer.DeserializeField("EntityId", uuidStr, fields, entityMap);

			assert(!uuidStr.empty());
			uuidStr == "NULL_UUID" ? deserStruct.entity = NULL_ENTITY_ID : deserStruct.entity = entityMap.at(GameUUID{ uuidStr });
		}
	};

	struct GridComponent final
	{
		std::vector<GridCellData> cells{ };

		uint16_t rows{ 0 };
		uint16_t cols{ 0 };

		uint16_t cellWidth{ 0 };
		uint16_t cellHeight{ 0 };

		GridComponent() = default;

		GridComponent(uint16_t _rows, uint16_t _cols, uint16_t _cellWidth, uint16_t _cellHeight) :
			rows{ _rows },
			cols{ _cols },
			cellWidth{ _cellWidth },
			cellHeight{ _cellHeight },
			cells(rows* cols, GridCellData{})
		{}

		GridComponent(uint16_t _rows, uint16_t _cols, uint16_t _cellWidth, uint16_t _cellHeight, std::vector<GridCellData>&& data) :
			rows{ _rows },
			cols{ _cols },
			cellWidth{ _cellWidth },
			cellHeight{ _cellHeight },
			cells{ std::move(data) }
		{
			assert(cells.size() == static_cast<size_t>(rows) * static_cast<size_t>(cols) && "Invalid data size for grid construction");
		}

		~GridComponent() = default;

		[[nodiscard]] uint32_t GetTotalWidth() const noexcept { return cols * cellWidth; }
		[[nodiscard]] uint32_t GetTotalHeight() const noexcept { return rows * cellHeight; }
		[[nodiscard]] bool IsWithinBounds(uint16_t row, uint16_t col) const noexcept { return row < rows && col < cols; }

		[[nodiscard]] std::pair<uint16_t, uint16_t> GetCellCoords(uint32_t x, uint32_t y) const noexcept
		{
			//x and y is in grid space		
			assert(x < GetTotalWidth() && y < GetTotalHeight() && "Coordinates out of bounds");

			const uint16_t row = static_cast<uint16_t>(y / cellHeight);
			const uint16_t col = static_cast<uint16_t>(x / cellWidth);

			assert(IsWithinBounds(row, col));

			return std::make_pair(row, col);
		}

		[[nodiscard]] std::pair<uint32_t, uint32_t> GetCellPosition(uint16_t row, uint16_t col) const noexcept
		{
			assert(IsWithinBounds(row, col) && "out of bounds");
			return std::pair<uint32_t, uint32_t>{ static_cast<uint32_t>(col * cellWidth), static_cast<uint32_t>(row * cellHeight) };
		}
		
		void SetType(uint16_t row, uint16_t col, const uint8_t& value) noexcept
		{
			assert(IsWithinBounds(row, col) && "out of bounds");
			cells[(row * cols) + col].type = value;
		}
		void ResetType(uint16_t row, uint16_t col) noexcept
		{
			assert(IsWithinBounds(row, col) && "out of bounds");
			cells[(row * cols) + col].type = 0;
		}

		[[nodiscard]] GridCellData& At(uint16_t row, uint16_t col) noexcept
		{
			assert(IsWithinBounds(row, col) && "out of bounds");
			return cells[(row * cols) + col];

		}
		[[nodiscard]] const GridCellData& At(uint16_t row, uint16_t col) const noexcept
		{
			assert(IsWithinBounds(row, col) && "out of bounds");
			return cells[(row * cols) + col];
		}

		static void Serialize(const FieldSerializer& fieldSer, const ECS& ecs, const EntityId id, std::vector<uint8_t>& fieldVector)
		{
			const auto& comp = ecs.GetComponent<GridComponent>(id);

			fieldSer.SerializeField("Cells", comp.cells, ecs, fieldVector);

			fieldSer.SerializeField("Rows", comp.rows, ecs, fieldVector);
			fieldSer.SerializeField("Cols", comp.cols, ecs, fieldVector);

			fieldSer.SerializeField("CellWidth", comp.cellWidth, ecs, fieldVector);
			fieldSer.SerializeField("CellHeight", comp.cellHeight, ecs, fieldVector);
		}
		static void Deserialize(const FieldSerializer& fieldSer, ECS& ecs, const EntityId id, const std::unordered_map<std::string, std::vector<uint8_t>>& serializedFields, const std::unordered_map<GameUUID, EntityId>& entityMap [[maybe_unused]] )
		{
			auto& comp = ecs.AddComponent<GridComponent>(id);

			fieldSer.DeserializeField("Cells", comp.cells, serializedFields, entityMap);

			//for (int idx{0}; auto & cell : comp.cells)
			//{
			//	std::cout << static_cast<int>(cell.type) << " ";
			//	++idx;
			//	if (idx % 13 == 0)
			//	{
			//		std::cout << "\n";
			//	}
			//}

			fieldSer.DeserializeField("Rows", comp.rows, serializedFields, entityMap);
			fieldSer.DeserializeField("Cols", comp.cols, serializedFields, entityMap);

			fieldSer.DeserializeField("CellWidth", comp.cellWidth, serializedFields, entityMap);
			fieldSer.DeserializeField("CellHeight", comp.cellHeight, serializedFields, entityMap);
		}
	};

	REGISTER_SERIALIZATION_FUNCTION(GridComponent, GridComponent::Serialize);
	REGISTER_DESERIALIZATION_FUNCTION(GridComponent, GridComponent::Deserialize);
}

#endif