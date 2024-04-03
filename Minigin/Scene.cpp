#include "Scene.h"

#include "Entity.h"
#include "TransformComponent.h"
#include "Renderer.h"

#include "TextComponent.h"

#include "imgui.h"

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

	bool Scene::DestroyEntity(Entity entity)
	{
		return DestroyEntity(entity.GetEntityId());
	}

	bool Scene::DestroyEntity(const EntityId entityId)
	{
		return m_Ecs.DestroyEntity(entityId);
	}

	void Scene::FixedUpdate()
	{
	}

	void Scene::Update()
	{
		m_TextSystem->Update();
		m_FPSSystem->Update();
		m_MovementSystem->Update();
		m_WorldPosSystem->Update();
	}

	void Scene::Render() const
	{
		m_RenderSystem->Render();
	}

	void Scene::RenderImGUI()
	{
		static EntityId selectedEntity = NULL_ENTITY_ID;

		ImGui::Begin("Scene information");

		auto comps = m_Ecs.GetComponents<TransformComponent>();
		for (auto it = comps.begin(); it != comps.end(); ++it)
		{
			auto id = comps.GetIdFromIterator(it);
			const auto& transform = *it;

			if (ImGui::TreeNode(("Entity ID: " + EntityToString(id)).c_str()))
			{
				if (ImGui::TreeNode("Transform"))
				{
					if (ImGui::BeginTable("TransformTable", 4))
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
						if (ImGui::BeginTable("TransformRelation", 2))
						{
							ImGui::TableSetupColumn("Relation");
							ImGui::TableSetupColumn("Entity ID", ImGuiTableColumnFlags_WidthStretch);
							ImGui::TableHeadersRow();

							ImGui::TableNextRow();
							ImGui::TableNextColumn();
							ImGui::TextUnformatted("Parent");
							ImGui::TableNextColumn();
							ImGui::Text(std::format("{}", EntityToString(transform.relation.parent)).c_str());

							ImGui::TableNextRow();
							ImGui::TableNextColumn();
							ImGui::TextUnformatted("Children");
							ImGui::TableNextColumn();
							ImGui::Text(std::format("{}", transform.relation.children).c_str());

							ImGui::TableNextRow();
							ImGui::TableNextColumn();
							ImGui::TextUnformatted("First Child");
							ImGui::TableNextColumn();
							ImGui::Text(std::format("{}", EntityToString(transform.relation.firstChild)).c_str());

							ImGui::TableNextRow();
							ImGui::TableNextColumn();
							ImGui::TextUnformatted("Previous Sibling");
							ImGui::TableNextColumn();
							ImGui::Text(std::format("{}", EntityToString(transform.relation.prevSibling)).c_str());

							ImGui::TableNextRow();
							ImGui::TableNextColumn();
							ImGui::TextUnformatted("Next Sibling");
							ImGui::TableNextColumn();
							ImGui::Text(std::format("{}", EntityToString(transform.relation.nextSibling)).c_str());

							ImGui::EndTable();
						}

						ImGui::TreePop();
					}

					ImGui::TreePop();
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