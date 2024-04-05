#include "Scene.h"

#include "Entity.h"
#include "TransformComponent.h"
#include "Renderer.h"

#include "TextComponent.h"
#include "SpriteComponent.h"
#include "AnimationComponent.h"
#include "RectColliderComponent.h"

#include "imgui.h" //RenderImgui --> TODO: move to sep function /  class

#include <format>

namespace Pengin
{
	unsigned int Scene::m_IdCounter = 0;

	Scene::Scene(const std::string& name) : 
		m_Name{ name }
	{ }

	Entity Scene::CreateEntity(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
	{
		const auto id{ m_Ecs.CreateEntity() };

		Entity entity{ id, weak_from_this() };
		entity.AddComponent<TransformComponent>(position, rotation, scale);

		return entity;
	}

	bool Scene::DestroyEntity(Entity entity, bool keepChildren)
	{
		entity.SetParent({ NULL_ENTITY_ID, shared_from_this() }, keepChildren);
		return m_Ecs.DestroyEntity(entity.GetEntityId());
	}

	bool Scene::DestroyEntity(const EntityId entityId, bool keepChildren)
	{
		return DestroyEntity({entityId, shared_from_this() }, keepChildren);
	}

	void Scene::FixedUpdate()
	{
	}

	void Scene::Update() //TODO: when empty, wrapper
	{
		m_TextSystem->Update();
		m_FPSSystem->Update();
		m_MovementSystem->Update();
		m_CollSystem->Update();

		m_AnimationSystem->Update();

		m_WorldPosSystem->Update();
	}

	void Scene::Render() const
	{
		m_RenderSystem->Render();
	}

	void Scene::RenderImGUI()//Todo, different windows / panels - in class / function
	{
		static EntityId selectedEntity = NULL_ENTITY_ID;

		ImGui::Begin("Scene information");

		auto comps = m_Ecs.GetComponents<TransformComponent>();
		for (auto it = comps.begin(); it != comps.end(); ++it)
		{
			auto id = comps.GetIdFromIterator(it);
			const auto& transform = *it;

			if (ImGui::TreeNode(("Entity ID: " + EntityIdToString(id)).c_str()))
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

				if (m_Ecs.HasComponent<RectColliderComponent>(id))
				{
					const auto& rectColl = m_Ecs.GetComponent<RectColliderComponent>(id);

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
							ImGui::Text("(%d, %d, %d, %d)", static_cast<uint16_t>(transform.worldPos.x) + rectColl.m_CollRect.x, static_cast<uint16_t>(transform.worldPos.y) + rectColl.m_CollRect.y, rectColl.m_CollRect.width, rectColl.m_CollRect.height);

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
								dae::Renderer::GetInstance().DrawRect(UtilStructs::Rectu16{ static_cast<uint16_t>(transform.worldPos.x + rectColl.m_CollRect.x),  static_cast<uint16_t>(transform.worldPos.y + rectColl.m_CollRect.y), rectColl.m_CollRect.width, rectColl.m_CollRect.height }, SDL_Color{255, 0, 0, 255});
							}
						}

						ImGui::TreePop();
					}
				}

				if (m_Ecs.HasComponent<SpriteComponent>(id))
				{
					const auto& sprite = m_Ecs.GetComponent<SpriteComponent>(id);

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

							ImGui::EndTable();
						}

						ImGui::TreePop();
					}
				}

				if (m_Ecs.HasComponent<AnimationComponent>(id))
				{
					const auto& aniComp = m_Ecs.GetComponent<AnimationComponent>(id);
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
								for (size_t idx{0};  const auto& animation : aniComp.m_Animations)
								{
									animation;

									const std::string label{ "Animation " + std::to_string(idx) + " data"};

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

				if (ImGui::TreeNode("Owned components"))
				{
					for (auto&& comp : m_Ecs.GetAllComponents(id))
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
}