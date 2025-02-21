#include "GameManager.h"
#include "EnemyManager.h"
#include "Player.h"
#include "Boss.h"

GameManager::GameManager(const int StageNumber)
{
	m_stageKinds = StageNumber;
}

GameManager::~GameManager()
{
}

void GameManager::Init()
{
	m_pEnemyManager = std::make_shared<EnemyManager>();
	m_pEnemyManager->Init();
	
}

void GameManager::Init(int handle)
{
	m_pBoss = std::make_shared<Boss>();
	m_pBoss->Init(handle);
}

void GameManager::Update(Player& player)
{
}

void GameManager::Draw()
{
}
