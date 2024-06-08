#include "PengoInputCommands.h"

#include "GameManager.h"
#include "SceneManager.h"
#include "EventManager.h"
#include "ServiceLocator.h"
#include "LevelWonEvent.h"
#include "PlayerComponent.h"
#include "UISelectorComponent.h"
#include "TextComponent.h"

#include "UIGamemodeSelectorComponent.h"
#include "UIGamemodeComponent.h"

#include "UILetterComponent.h"

void Pengo::PengoPlayGame::Execute()
{
	Pengin::EventManager::GetInstance().BroadcoastEvent(std::make_unique<Pengin::BaseEvent>("PlayGame"));
}

void Pengo::PengoSkipLevel::Execute()
{
	Pengin::EventManager::GetInstance().BroadcoastEvent(std::make_unique<LevelWonEvent>(GetUserIdx()));
}

void Pengo::MuteSounds::Execute()
{
	auto& pSoundSys = Pengin::ServiceLocator::GetSoundSystem();

	pSoundSys.IsMuted() ? pSoundSys.Unmute() : pSoundSys.Mute();
}

void Pengo::SelectLetter::Execute()
{	
	using namespace Pengin;

	auto player = SceneManager::GetInstance().GetActiveScene()->GetPlayer(GetUserIdx());
	assert(player.HasComponent<UISelectorComponent>());	

	auto& uiSelector = player.GetComponent<UISelectorComponent>();

	if (m_Direction.x > 0)
	{
		uiSelector.ScrollIdx(true);
	}
	else if (m_Direction.x < 0)
	{
		uiSelector.ScrollIdx(false);
	}
	else if (m_Direction.y > 0)
	{
		const auto id = uiSelector.letters[uiSelector.currHorIdx];

		auto letterEnt = Entity{ id, SceneManager::GetInstance().GetActiveScene() };
		auto& letterComp = letterEnt.GetComponent<UILetterComponent>();
		letterComp.ScrollLetter(true);
		letterEnt.GetComponent<TextComponent>().SetText({ letterComp.letter });
	}
	else if (m_Direction.y < 0)
	{
		const auto id = uiSelector.letters[uiSelector.currHorIdx];

		auto letterEnt = Entity{ id, SceneManager::GetInstance().GetActiveScene() };
		auto& letterComp = letterEnt.GetComponent<UILetterComponent>();
		letterComp.ScrollLetter(false);
		letterEnt.GetComponent<TextComponent>().SetText({ letterComp.letter });
	}
}

void Pengo::Continue::Execute()
{
	Pengin::EventManager::GetInstance().BroadcoastEvent(std::make_unique<Pengin::BaseEvent>("BackToMainMenu"));
}

void Pengo::SelectGameMode::Execute()
{
	using namespace Pengin;

	auto comps = SceneManager::GetInstance().GetActiveScene()->GetECS().GetComponents<UIGameModeSelectorComponent>();
	assert(player.HasComponent<UISelectorComponent>());

	if (m_Direction.y > 0)
	{
		comps.begin()->ChangeGameMode(true);
		const auto currId = comps.begin()->gamemodeIds[comps.begin()->currGameMode];

		auto gmEnt = Entity{ currId, SceneManager::GetInstance().GetActiveScene() };
		const auto& gmComp = gmEnt.GetComponent<UIGameModeComponent>();
		SceneManager::GetInstance().GetActiveScene()->GetECS().GetComponent<TextComponent>(comps.GetIdFromIterator(comps.cbegin())).SetText(gmComp.gameMode);
	}
	else if (m_Direction.y < 0)
	{
		comps.begin()->ChangeGameMode(false);
		const auto currId = comps.begin()->gamemodeIds[comps.begin()->currGameMode];

		auto gmEnt = Entity{ currId, SceneManager::GetInstance().GetActiveScene() };
		const auto& gmComp = gmEnt.GetComponent<UIGameModeComponent>();
		SceneManager::GetInstance().GetActiveScene()->GetECS().GetComponent<TextComponent>(comps.GetIdFromIterator(comps.cbegin())).SetText(gmComp.gameMode);
	}
}
