#ifndef PENGIN_GRIDCOMPONENT
#define PENGIN_GRIDCOMPONENT

#include <vector>
#include <cassert>
#include <utility>   
#include <type_traits> 

#include "SerializationRegistry.h"

namespace Pengin
{
	struct GridComponent final
	{
		std::vector<uint8_t> cells{ };

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
			cells(rows * cols, 0)
		{}

		GridComponent(uint16_t _rows, uint16_t _cols, uint16_t _cellWidth, uint16_t _cellHeight, std::vector<uint8_t>&& data) :
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

		[[nodiscard]] std::pair<uint32_t, uint32_t> GetCellPosition(uint16_t row, uint16_t col) const noexcept
		{
			assert(IsWithinBounds(row, col) && "out of bounds");
			return std::pair<uint32_t, uint32_t>{ static_cast<uint32_t>(col * cellWidth), static_cast<uint32_t>(row * cellHeight) };
		}
		
		void Set(uint16_t row, uint16_t col, const uint8_t& value) noexcept
		{
			assert(IsWithinBounds(row, col) && "out of bounds");
			cells[(row * cols) + col] = value;
		}
		void Reset(uint16_t row, uint16_t col) noexcept
		{
			assert(IsWithinBounds(row, col) && "out of bounds");
			cells[(row * cols) + col] = 0;
		}

		[[nodiscard]] uint8_t& At(uint16_t row, uint16_t col) noexcept
		{
			assert(IsWithinBounds(row, col) && "out of bounds");
			return cells[(row * cols) + col];
		}
		[[nodiscard]] uint8_t At(uint16_t row, uint16_t col) const noexcept
		{
			assert(IsWithinBounds(row, col) && "out of bounds");
			return cells[(row * cols) + col];
		}

		static void Serialize(const FieldSerializer& fieldSer, const ECS& ecs, const EntityId id, std::vector<uint8_t>& fieldVector)
		{
			const auto& comp = ecs.GetComponent<GridComponent>(id);

			fieldSer.SerializeField("Cells", comp.cells, fieldVector);

			fieldSer.SerializeField("Rows", comp.rows, fieldVector);
			fieldSer.SerializeField("Cols", comp.cols, fieldVector);

			fieldSer.SerializeField("CellWidth", comp.cellWidth, fieldVector);
			fieldSer.SerializeField("CellHeight", comp.cellHeight, fieldVector);
		}
		static void Deserialize(const FieldSerializer& fieldSer, ECS& ecs, const EntityId id, const std::unordered_map<std::string, std::vector<uint8_t>>& serializedFields, const std::unordered_map<GameUUID, EntityId>& entityMap [[maybe_unused]] )
		{
			auto& comp = ecs.AddComponent<GridComponent>(id);

			fieldSer.DeserializeField("Cells", comp.cells, serializedFields);

			fieldSer.DeserializeField("Rows", comp.rows, serializedFields);
			fieldSer.DeserializeField("Cols", comp.cols, serializedFields);

			fieldSer.DeserializeField("CellWidth", comp.cellWidth, serializedFields);
			fieldSer.DeserializeField("CellHeight", comp.cellHeight, serializedFields);
		}
	};

	REGISTER_SERIALIZATION_FUNCTION(GridComponent, GridComponent::Serialize);
	REGISTER_DESERIALIZATION_FUNCTION(GridComponent, GridComponent::Deserialize);
}

#endif