#ifndef TEXTCOMPONENT
#define TEXTCOMPONENT

#include "ResourceManager.h"
#include "SerializationRegistry.h"

#include "Font.h"
#include "glm/vec4.hpp"

#include <string>
#include <memory>

namespace Pengin
{
	struct TextComponent final
	{
		std::string text;
		std::shared_ptr<dae::Font> pFont;
		glm::u8vec4 color{ 255, 255, 255, 255 };

		bool needsTextureChange{ true };

		TextComponent(const std::string& fontPath, unsigned fontSize, const std::string& text = "<EMPTY STRING>", const glm::u8vec4& color = { 255, 255, 255, 255 }) :
			text{ text },
			pFont{ dae::ResourceManager::GetInstance().LoadFont(fontPath, fontSize) },
			color{ color },
			needsTextureChange{ true }
		{
			assert(fontSize > 0 && "Can not have fontsize 0");
		}

		~TextComponent() = default;

		void SetText(const std::string& newText, const glm::u8vec4 newColor = {  }) //allows to change the text without having to set all variables manually
		{
			text = newText;
			needsTextureChange = true;

			if (newColor != glm::u8vec4{})
			{
				color = newColor;
			}
		}

		static void Serialize(const FieldSerializer& fieldSer, const ECS& ecs, const EntityId id, std::vector<uint8_t>& fieldVector)
		{
			const auto& comp = ecs.GetComponent<TextComponent>(id);

			fieldSer.SerializeField("Text", comp.text, fieldVector);

			fieldSer.SerializeField("FontPath", comp.pFont ? comp.pFont->GetPath() : "NO PATH", fieldVector);
			fieldSer.SerializeField<unsigned>("FontSize", comp.pFont ? comp.pFont->GetFontSize() : 0, fieldVector);

			const auto& color = comp.color;
			fieldSer.SerializeField("Color", std::vector<uint8_t>{color.r, color.g, color.b, color.a}, fieldVector);
		}
		static void Deserialize(const FieldSerializer& fieldSer, ECS& ecs, const EntityId id, const std::unordered_map<std::string, std::vector<uint8_t>>& serializedFields, const std::unordered_map<GameUUID, EntityId>& entityMap [[maybe_unused]] )
		{
			std::string fontPath{};
			fieldSer.DeserializeField("FontPath",fontPath, serializedFields);
			assert(fontPath != "NO PATH");

			unsigned fontSize{};
			fieldSer.DeserializeField("FontSize", fontSize, serializedFields);
			assert(fontSize != 0);

			auto& textComp = ecs.AddComponent<TextComponent>(id, fontPath, fontSize);

			std::vector<uint8_t> color{ };
			fieldSer.DeserializeField("Color", color, serializedFields);
			textComp.color = glm::u8vec4{ color[0], color[1], color[2], color[3] };

			fieldSer.DeserializeField("Text", textComp.text, serializedFields);

			textComp.needsTextureChange = true; //Always need to generate a texture upon deserializing
		}
	};

	REGISTER_SERIALIZATION_FUNCTION(TextComponent, TextComponent::Serialize);
	REGISTER_DESERIALIZATION_FUNCTION(TextComponent, TextComponent::Deserialize);
}

#endif