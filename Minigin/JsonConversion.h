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
	//--------------

	//Velocity
	void to_json(json& j, const VelocityComponent& velocity)
	{
		j =
		{
			{"Velocity", { velocity.m_Velocity.x, velocity.m_Velocity.y, velocity.m_Velocity.z } }
		};
	}
	void from_json(const json& j, VelocityComponent& velocity)
	{
		velocity.m_Velocity = {
			j["Velocity"][0].get<float>(),
			j["Velocity"][1].get<float>(),
			j["Velocity"][2].get<float>()
		};
	}
	//--------------

	//Text
	void to_json(json& j, const TextComponent& text)
	{
		j =
		{
			{"Text", text.m_Text},
			{"Path", text.m_pFont->GetPath() },
			{"FontSize", text.m_pFont->GetFontSize() },
			{"Color", { text.m_Color.r, text.m_Color.g, text.m_Color.b, text.m_Color.a } }
		};
	}
	//---------------s

	//Rect colliider
	void to_json(json& j, const RectColliderComponent& rectColl)
	{
		j =
		{
			{"CollRect",{ rectColl.m_CollRect.x,  rectColl.m_CollRect.y,  rectColl.m_CollRect.width,  rectColl.m_CollRect.height }}
		};
	}
	void from_json(const json& j, RectColliderComponent& rectColl)
	{
		rectColl.m_CollRect = UtilStructs::Rectu16{ 
			j["CollRect"][0].get<uint16_t>(),
			j["CollRect"][1].get<uint16_t>(),
			j["CollRect"][2].get<uint16_t>(),
			j["CollRect"][3].get<uint16_t>()
		};
	}
	//---------------

	//Animation
	void to_json(json& j, const AnimationData& aniData)
	{
		j =
		{
			{"Frame0SourceRect", {aniData.frame0sourceRect.x, aniData.frame0sourceRect.y, aniData.frame0sourceRect.width, aniData.frame0sourceRect.height} },
			{"FrameCount", aniData.frameCt},
			{"FrameDuration", aniData.frameDuration}
		};
	}
	void from_json(const json& j, AnimationData& aniData)
	{
		aniData.frame0sourceRect.x = j["Frame0SourceRect"][0].get<uint16_t>();
		aniData.frame0sourceRect.y = j["Frame0SourceRect"][1].get<uint16_t>();
		aniData.frame0sourceRect.width = j["Frame0SourceRect"][2].get<uint16_t>();
		aniData.frame0sourceRect.height = j["Frame0SourceRect"][3].get<uint16_t>();
		aniData.frameCt = j["FrameCount"].get<uint8_t>();
		aniData.frameDuration = j["FrameDuration"].get<float>();
	}

	void to_json(json& j, const AnimationComponent& ani)
	{
		j =
		{
			{"Animations", ani.m_Animations },
			{"CurrentAnimationIndex", ani.m_CurrAnimationIdx},
			{"CurrentAnimationFrame",ani.m_CurrFrame},
			{"CurrentFrameTime", ani.m_FrameTimer},
			{"IsPlaying", ani.m_IsPlaying}
		};
	}
	void from_json(const json& j, AnimationComponent& ani)
	{
		ani.m_Animations = j["Animations"].get<std::vector<AnimationData>>();
		ani.m_CurrAnimationIdx = j["CurrentAnimationIndex"];
		ani.m_CurrFrame = j["CurrentAnimationFrame"];
		ani.m_FrameTimer = j["CurrentFrameTime"];
		ani.m_IsPlaying = j["IsPlaying"];
	}
	//---------------
}

#endif