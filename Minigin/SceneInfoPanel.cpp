#include "SceneInfoPanel.h"

#include "ImGui.h"
#include "Renderer.h"

#include "ECS.h"

#include "TransformComponent.h"
#include "RectColliderComponent.h"
#include "SpriteComponent.h"
#include "AnimationComponent.h"

#include <string>
#include <format>

namespace Pengin
{
	void SceneInfoPanel::Render(ECS& ecs)
	{
		ImGui::Begin("Scene information");
		auto comps = ecs.GetComponents<TransformComponent>();

		for (auto it = comps.begin(); it != comps.end(); ++it)
		{
			auto id = comps.GetIdFromIterator(it);
			const auto& transform = *it;

			if (ImGui::TreeNode(("Entity ID: " + EntityIdToString(id)).c_str()))
			{
				RenderTransformInfo(transform);

				if (ecs.HasComponent<RectColliderComponent>(id))
				{
					RenderRectCollInfo(ecs, id, transform);
				}

				if (ecs.HasComponent<SpriteComponent>(id))
				{
					RenderSpriteInfo(ecs, id);
				}

				if (ecs.HasComponent<AnimationComponent>(id))
				{
					RenderAnimationInfo(ecs, id);
				}

				if (ImGui::TreeNode("Owned components"))
				{
					for (auto&& comp : ecs.GetAllComponents(id))
					{
						ImGui::Text(comp.name());
					}

					ImGui::TreePop();
				}

				ImGui::TreePop();
			}
		}

		ImGui::End();
	}

	void SceneInfoPanel::RenderTransformInfo( const TransformComponent& transform)
	{
		if (ImGui::TreeNode("Transform"))
		{
			if (ImGui::BeginTable("Transform Table", 4))
			{
				ImGui::TableSetupColumn("Component");
				ImGui::TableSetupColumn("X", ImGuiTableColumnFlags_WidthStretch);
				ImGui::TableSetupColumn("Y", ImGuiTableColumnFlags_WidthStretch);
				ImGui::TableSetupColumn("Z", ImGuiTableColumnFlags_WidthStretch);
				ImGui::TableHeadersRow();

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::TextUnformatted("World Position");
				ImGui::TableNextColumn();
				ImGui::Text("%f", transform.worldPos.x);
				ImGui::TableNextColumn();
				ImGui::Text("%f", transform.worldPos.y);
				ImGui::TableNextColumn();
				ImGui::Text("%f", transform.worldPos.z);

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::TextUnformatted("Local Position");
				ImGui::TableNextColumn();
				ImGui::Text("%f", transform.localPos.x);
				ImGui::TableNextColumn();
				ImGui::Text("%f", transform.localPos.y);
				ImGui::TableNextColumn();
				ImGui::Text("%f", transform.localPos.z);

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::TextUnformatted("Rotation");
				ImGui::TableNextColumn();
				ImGui::Text("%f", transform.rotation.x);
				ImGui::TableNextColumn();
				ImGui::Text("%f", transform.rotation.y);
				ImGui::TableNextColumn();
				ImGui::Text("%f", transform.rotation.z);

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::TextUnformatted("Scale");
				ImGui::TableNextColumn();
				ImGui::Text("%f", transform.scale.x);
				ImGui::TableNextColumn();
				ImGui::Text("%f", transform.scale.y);
				ImGui::TableNextColumn();
				ImGui::Text("%f", transform.scale.z);

				ImGui::EndTable();
			}

			if (ImGui::TreeNode("Relation"))
			{
				if (ImGui::BeginTable("Transform Relation", 2))
				{
					ImGui::TableSetupColumn("Relation");
					ImGui::TableSetupColumn("Entity ID", ImGuiTableColumnFlags_WidthStretch);
					ImGui::TableHeadersRow();

					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::TextUnformatted("Parent");
					ImGui::TableNextColumn();
					ImGui::Text(std::format("{}", EntityIdToString(transform.relation.parent)).c_str());

					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::TextUnformatted("Children");
					ImGui::TableNextColumn();
					ImGui::Text(std::format("{}", transform.relation.children).c_str());

					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::TextUnformatted("First Child");
					ImGui::TableNextColumn();
					ImGui::Text(std::format("{}", EntityIdToString(transform.relation.firstChild)).c_str());

					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::TextUnformatted("Previous Sibling");
					ImGui::TableNextColumn();
					ImGui::Text(std::format("{}", EntityIdToString(transform.relation.prevSibling)).c_str());

					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::TextUnformatted("Next Sibling");
					ImGui::TableNextColumn();
					ImGui::Text(std::format("{}", EntityIdToString(transform.relation.nextSibling)).c_str());

					ImGui::EndTable();
				}

				ImGui::TreePop();
			}

			ImGui::TreePop();
		}
	}

	void SceneInfoPanel::RenderRectCollInfo(ECS& ecs, const EntityId id, const TransformComponent& transform)
	{
		const auto& rectColl = ecs.GetComponent<RectColliderComponent>(id);

		if (ImGui::TreeNode("Rect Collider Component"))
		{
			if (ImGui::BeginTable("Sprite table", 2))
			{
				ImGui::TableSetupColumn("Data");
				ImGui::TableSetupColumn(" ", ImGuiTableColumnFlags_WidthStretch);
				ImGui::TableHeadersRow();

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::TextUnformatted("Collider Rect");
				ImGui::TableNextColumn();
				ImGui::Text("(%d, %d, %d, %d)", rectColl.m_CollRect.x, rectColl.m_CollRect.y, rectColl.m_CollRect.width, rectColl.m_CollRect.height);

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::TextUnformatted("World Location Rect");
				ImGui::TableNextColumn();
				ImGui::Text("(%d, %d, %d, %d)", static_cast<int>(transform.worldPos.x + rectColl.m_CollRect.x * transform.scale.x),
												static_cast<int>(transform.worldPos.y + rectColl.m_CollRect.y  * transform.scale.y),
												rectColl.m_CollRect.width * static_cast<int>(transform.scale.x), 
												rectColl.m_CollRect.height * static_cast<int>(transform.scale.y));

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::TextUnformatted("Enable Collider Debug Drawing");
				ImGui::TableNextColumn();

				static std::unordered_map<EntityId, bool> checkboxStates;
				bool& enableCollider = checkboxStates[id];
				ImGui::Checkbox("##EnableCollider", &enableCollider);

				ImGui::EndTable();

				if (enableCollider)
				{
					dae::Renderer::GetInstance().DrawRect(UtilStructs::Rect16{ static_cast<int16_t>(transform.worldPos.x + rectColl.m_CollRect.x * transform.scale.x),
																				static_cast<int16_t>(transform.worldPos.y + rectColl.m_CollRect.y * transform.scale.y),
																				static_cast<int16_t>(rectColl.m_CollRect.width * transform.scale.x), 
																				static_cast<int16_t>(rectColl.m_CollRect.height * transform.scale.y)}, 
																				SDL_Color{255, 0, 0, 255});
				}
			}

			ImGui::TreePop();
		}
	}
	void SceneInfoPanel::RenderSpriteInfo(ECS& ecs, const EntityId id)
	{
		auto& sprite = ecs.GetComponent<SpriteComponent>(id);

		if (ImGui::TreeNode("Sprite Component"))
		{
			if (ImGui::BeginTable("Sprite table", 2))
			{
				ImGui::TableSetupColumn("Data");
				ImGui::TableSetupColumn(" ", ImGuiTableColumnFlags_WidthStretch);
				ImGui::TableHeadersRow();

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::TextUnformatted("Source Rect");
				ImGui::TableNextColumn();
				ImGui::Text("(%d, %d, %d, %d)", sprite.m_SourceRect.x, sprite.m_SourceRect.y, sprite.m_SourceRect.width, sprite.m_SourceRect.height);

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::TextUnformatted("Toggle sprite visibility");
				ImGui::TableNextColumn();

				ImGui::Checkbox("##EnableCollider", &sprite.isVisible);

				ImGui::EndTable();
			}

			ImGui::TreePop();
		}
	}
	void SceneInfoPanel::RenderAnimationInfo(ECS& ecs, const EntityId id)
	{
		const auto& aniComp = ecs.GetComponent<AnimationComponent>(id);
		const bool isAniDataEmpty{ aniComp.m_Animations.empty() };

		if (ImGui::TreeNode("Animation Component"))
		{
			if (!isAniDataEmpty)
			{
				if (ImGui::BeginTable("Animation table", 2))
				{
					ImGui::TableSetupColumn("Data");
					ImGui::TableSetupColumn(" ", ImGuiTableColumnFlags_WidthStretch);
					ImGui::TableHeadersRow();

					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::TextUnformatted("Is playing");
					ImGui::TableNextColumn();
					ImGui::Text("%s", aniComp.m_IsPlaying ? "True" : "False");

					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::TextUnformatted("Current animation index");
					ImGui::TableNextColumn();
					ImGui::Text("%d", aniComp.m_CurrAnimationIdx);

					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::TextUnformatted("Current frame index");
					ImGui::TableNextColumn();
					ImGui::Text("%d", aniComp.m_CurrFrame);

					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::TextUnformatted("Current frame time");
					ImGui::TableNextColumn();
					ImGui::Text("%f", aniComp.m_FrameTimer);

					ImGui::EndTable();
				}

				if (ImGui::TreeNode("Animations"))
				{
					for (size_t idx{ 0 }; const auto & animation : aniComp.m_Animations)
					{
						const std::string label{ "Animation " + std::to_string(idx) + " data" };

						if (ImGui::TreeNode(label.c_str()))
						{
							if (ImGui::BeginTable("Animation data table", 2))
							{
								ImGui::TableSetupColumn("Data");
								ImGui::TableSetupColumn(" ", ImGuiTableColumnFlags_WidthStretch);
								ImGui::TableHeadersRow();

								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								ImGui::TextUnformatted("frame count");
								ImGui::TableNextColumn();
								ImGui::Text("%d", animation.frameCt);

								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								ImGui::TextUnformatted("frame duration");
								ImGui::TableNextColumn();
								ImGui::Text("%d", animation.frameDuration);

								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								ImGui::TextUnformatted("frame 0 source rect");
								ImGui::TableNextColumn();
								ImGui::Text("(%d, %d, %d, %d)", animation.frame0sourceRect.x, animation.frame0sourceRect.y, animation.frame0sourceRect.width, animation.frame0sourceRect.height);

								ImGui::EndTable();
							}

							ImGui::TreePop();
						}

						++idx;
					}

					ImGui::TreePop();
				}
			}
			else
			{
				ImGui::Text("EMPTY ANIMATION DATA");
			}
			ImGui::TreePop();
		}
	}
}