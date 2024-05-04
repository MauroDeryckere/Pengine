#ifndef DISPLAYCOMPONENT
#define DISPLAYCOMPONENT

#include <string>

namespace Pengin
{
	struct TxtDisplayComponent final
	{
		TxtDisplayComponent() = default;

		TxtDisplayComponent(const std::string& prefix, const std::string& postfix = {}) :
			prefix{ prefix },
			postfix{ postfix }
		{}

		~TxtDisplayComponent() = default;

		std::string prefix{};
		std::string postfix{};
		static void Serialize(const FieldSerializer& fieldSer, const ECS& ecs, const EntityId id, std::vector<uint8_t>& fieldVector)
		{ 
			const auto& comp = ecs.GetComponent<TxtDisplayComponent>(id);

			fieldSer.SerializeField("Prefix", comp.prefix, fieldVector);
			fieldSer.SerializeField("Postfix", comp.postfix, fieldVector);
		}
	};

	REGISTER_SERIALIZATION_FUNCTION(TxtDisplayComponent, TxtDisplayComponent::Serialize);
}

#endif