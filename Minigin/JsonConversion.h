#ifndef JSONCONVERSION
#define JSONCONVERSION

//provide the json specialization functions to convert components

#include "Components.h"
#include <json.hpp>

namespace Pengin
{
	using json = nlohmann::ordered_json;

	//NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE();

	//Transforms
	void to_json(json& j, const TransformComponent& transform)
	{
		j =
		{
			{"worldPos", {transform.worldPos.x, transform.worldPos.y, transform.worldPos.z} },
			{"localPos", {transform.localPos.x, transform.localPos.y, transform.localPos.z} },
			{"rotation", {transform.rotation.x, transform.rotation.y, transform.rotation.z} },
			{"scale",    {transform.scale.x, transform.scale.y, transform.scale.z} },
			{"relation",
				{
					{"children", transform.relation.children},
					{"firstChild", transform.relation.firstChild},
					{"prevSibling", transform.relation.prevSibling},
					{"nextSibling", transform.relation.nextSibling},
					{"parent", transform.relation.parent}
				}
			},
			{"isPosDirty", transform.isPosDirty}
		};
	}
	void from_json(const json& j, TransformComponent& transform)
	{
		transform.worldPos = {
			j["worldPos"][0].get<float>(),
			j["worldPos"][1].get<float>(),
			j["worldPos"][2].get<float>()
		};
		transform.localPos = {
			j["localPos"][0].get<float>(),
			j["localPos"][1].get<float>(),
			j["localPos"][2].get<float>()
		};
		transform.rotation = {
			j["rotation"][0].get<float>(),
			j["rotation"][1].get<float>(),
			j["rotation"][2].get<float>()
		};
		transform.scale = {
			j["scale"][0].get<float>(),
			j["scale"][1].get<float>(),
			j["scale"][2].get<float>()
		};

		transform.relation.children = j["relation"]["children"].get<size_t>();
		transform.relation.firstChild = j["relation"]["firstChild"].get<EntityId>();
		transform.relation.prevSibling = j["relation"]["prevSibling"].get<EntityId>();
		transform.relation.nextSibling = j["relation"]["nextSibling"].get<EntityId>();
		transform.relation.parent = j["relation"]["parent"].get<EntityId>();
		transform.isPosDirty = j["isPosDirty"].get<bool>();
	}
	//----------
	
	//Sprites
	void to_json(json& j, const SpriteComponent& sprite)
	{
		j =
		{
			{"Source rect", { sprite.m_SourceRect.x, sprite.m_SourceRect.y, sprite.m_SourceRect.width, sprite.m_SourceRect.height } },
			{"is visible", sprite.isVisible },
			{"path", sprite.m_pTexture ? sprite.m_pTexture->GetPath() : "NO PATH" },
		};
	}
	//----------------
}

#endif