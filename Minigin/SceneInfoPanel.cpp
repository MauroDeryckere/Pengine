#include "SceneInfoPanel.h"

#include "ImGui.h"
#include "Renderer.h"

#include "ECS.h"

#include "Scene.h"

#include "TagComponent.h"
#include "TransformComponent.h"
#include "RectColliderComponent.h"
#include "SpriteComponent.h"
#include "AnimationComponent.h"
#include "UUIDComponent.h"

#include <string>
#include <format>

namespace Pengin
{
	void SceneInfoPanel::Render(ECS& ecs)
	{
		static bool EDITOR_MODE = false;
		static bool KEEPCHILDREN_ON_DESTROY = true;

		std::vector<EntityId> esToDestroy;

		ImGui::Begin("Scene information");
		ImGui::Checkbox("Editor Mode", &EDITOR_MODE);

		auto comps = ecs.GetComponents<TransformComponent>();

		for (auto it = comps.begin(); it != comps.end(); ++it)
		{
			auto id = comps.GetIdFromIterator(it);
			const auto& transform = *it;

			const bool hasSpriteComp{ ecs.HasComponent<SpriteComponent>(id) };

			const std::string label = ecs.HasComponent<TagComponent>(id) ?
				(ecs.GetComponent<TagComponent>(id).tag + " (entity ID: " + EntityIdToString(id) + " )") :
				("Entity ID: " + EntityIdToString(id));

			const bool treeNodeOpened = ImGui::TreeNode(label.c_str());

			if (treeNodeOpened)
			{
				assert(ecs.HasComponent<UUIDComponent>(id));
				const auto& uuidComp = ecs.GetComponent<UUIDComponent>(id);
				ImGui::Text(std::format("Entity UUID: {}", uuidComp.uuid.GetUUID_PrettyStr()).c_str());
			}

			if (hasSpriteComp) //Display a checkbox at treenode level to allow toggling visibility
			{
				ImVec2 checkboxSize = ImGui::CalcTextSize("");
				ImVec2 availableSize = ImGui::GetContentRegionAvail();
				float xPos = availableSize.x - checkboxSize.x - 5.0f; 

				ImGui::SameLine(xPos);
				ImGui::Checkbox(("##" + std::to_string(id)).c_str(), &ecs.GetComponent<SpriteComponent>(id).isVisible);
			}


			if (EDITOR_MODE)
			{
				ImGui::Indent();
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.1f, 1.0f)); // Red color for the button
				if (ImGui::Button(("Destroy##destroy" + std::to_string(id)).c_str()))
				{
					esToDestroy.emplace_back(id);
				}
				ImGui::PopStyleColor();
				ImGui::Unindent();
			}

			if (treeNodeOpened)
			{
				RenderTransformInfo(transform);

				if (ecs.HasComponent<RectColliderComponent>(id))
				{
					RenderRectCollInfo(ecs, id, transform);
				}

				if (hasSpriteComp)
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

		for (const auto entity : esToDestroy)
		{
			m_pScene->DestroyEntity(entity, KEEPCHILDREN_ON_DESTROY);
		}

		if (EDITOR_MODE)
		{
			RenderEditorWindow(ecs, &KEEPCHILDREN_ON_DESTROY);
		}
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
				ImGui::Text("(%d, %d, %d, %d)", rectColl.collRect.x, rectColl.collRect.y, rectColl.collRect.width, rectColl.collRect.height);

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::TextUnformatted("World Location Rect");
				ImGui::TableNextColumn();
				ImGui::Text("(%d, %d, %d, %d)", static_cast<int>(transform.worldPos.x + rectColl.collRect.x * transform.scale.x),
												static_cast<int>(transform.worldPos.y + rectColl.collRect.y  * transform.scale.y),
												rectColl.collRect.width * static_cast<int>(transform.scale.x), 
												rectColl.collRect.height * static_cast<int>(transform.scale.y));

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
					dae::Renderer::GetInstance().DrawRect(UtilStructs::Rect16{ static_cast<int16_t>(transform.worldPos.x + rectColl.collRect.x * transform.scale.x),
																				static_cast<int16_t>(transform.worldPos.y + rectColl.collRect.y * transform.scale.y),
																				static_cast<int16_t>(rectColl.collRect.width * transform.scale.x), 
																				static_cast<int16_t>(rectColl.collRect.height * transform.scale.y)}, 
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
				ImGui::Text("(%d, %d, %d, %d)", sprite.sourceRect.x, sprite.sourceRect.y, sprite.sourceRect.width, sprite.sourceRect.height);

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
		const bool isAniDataEmpty{ aniComp.animations.empty() };

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
					ImGui::Text("%s", aniComp.isPlaying ? "True" : "False");

					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::TextUnformatted("Current animation index");
					ImGui::TableNextColumn();
					ImGui::Text("%d", aniComp.currAnimationIdx);

					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::TextUnformatted("Current frame index");
					ImGui::TableNextColumn();
					ImGui::Text("%d", aniComp.currFrame);

					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::TextUnformatted("Current frame time");
					ImGui::TableNextColumn();
					ImGui::Text("%f", aniComp.frameTimer);

					ImGui::EndTable();
				}

				if (ImGui::TreeNode("Animations"))
				{
					for (size_t idx{ 0 }; const auto & animation : aniComp.animations)
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

	void SceneInfoPanel::RenderEditorWindow(ECS& ecs, bool* keepChildren)
	{
		ecs;
		ImGui::Begin("Editor window");

		ImGui::Checkbox("Keep children on destroy ##Keepchildren", keepChildren);
		ImGui::Unindent();

		ImGui::End();
	}
}