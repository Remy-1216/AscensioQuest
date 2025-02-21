#pragma once
#include "DxLib.h"
#include <string>
#include <vector>
#include <map>
#include <memory>

class EnemyManager;
class Boss;
class Player;
class GameManager
{
public:
	//ステージの種類
	enum Stagekinds
	{
		Stage1,
		Stage2,
	};

public:
	GameManager(const int StageNumber);

	virtual ~GameManager();

	void Init();

	void Init(int handle);

	void Update(Player& player);

	void Draw();

private:

	std::shared_ptr<EnemyManager> m_pEnemyManager;

	std::shared_ptr<Boss> m_pBoss;

	//ステージの種類
	int m_stageKinds;
};

