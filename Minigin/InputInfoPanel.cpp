#include "InputInfoPanel.h"

#include "ImGui.h"
#include "InputManager.h"

namespace Pengin
{
	void InputInfoPanel::Render()
	{
		ImGui::Begin("Input Info Panel");

		RenderUsers();

		ImGui::End();
	}

	void InputInfoPanel::RenderUsers()
	{
		auto& input = InputManager::GetInstance();

		if (ImGui::TreeNode("Registered Users "))
		{
			for (const auto& [userUUID, userIdx] : input.m_UserIdx_VecIdxMap)
			{
				ImGui::Text("User index: %d", userIdx);
				ImGui::Text("User UUID: %s", userUUID.GetUUID_PrettyStr().c_str());

				const char* userType;

				switch (input.m_RegisteredUsers.at(userIdx).first)
				{
				case Pengin::UserType::Keyboard:
					userType = "Keyboard";
					break;
				case Pengin::UserType::Controller:
					userType = "Controller";
					break;
				default: 
					userType = "NULL";
					break;
				}

				ImGui::Text("User type: %s", userType);

				//if (ImGui::TreeNode("Action Mapping"))
				//{
				//	if (user.first == UserType::Keyboard)
				//	{
				//		auto* pActions = static_cast<std::vector<std::unordered_map<KeyBoardKey, std::shared_ptr<InputCommand>>>*>(user.second[0]->GetMappedActions());
				//		const auto& map1 = pActions->at(0);

				//		for (const auto& action : map1)
				//		{
				//			ImGui::Text(std::to_string(static_cast<unsigned>(action.first)).c_str());
				//			ImGui::Text(typeid(action.second).name());
				//		}
				//	}

				//	ImGui::TreePop();
				//}
			}

			ImGui::TreePop();
		}
	}
}
