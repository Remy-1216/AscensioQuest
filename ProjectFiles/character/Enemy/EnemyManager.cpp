#include "EnemyManager.h""
#include "ShortDistanceEnemy.h"
#include "LongDistanceEnemy.h"
#include "Player.h"
#include "Apple.h"
#include "LoadCsv.h"
#include <iostream>
#include <string>
#include <vector>

namespace
{
	//敵が死んだときにもらえるステータスポイント
	constexpr int kStatusPoint = 1;

	//最大敵数
	constexpr int kMaxEnemy = 16;

	//アイテムドロップ率
	constexpr int kDropRate = 50;
}

EnemyManager::EnemyManager()
{
	//初期化
	for (int i = 0; i < kEnemyNum; i++)
	{
		m_shortDistanceEnemyPos[i] = VGet(0.0f, 0.0f, 0.0f);
		m_longDistanceEnemyPos[i] = VGet(0.0f, 0.0f, 0.0f);
	}

	//モデルのロード
	m_shortDistanceEnemyHandle = MV1LoadModel("data/model/enemy/Enemy_Black.mv1");
	m_longDistanceEnemyHandle = MV1LoadModel("data/model/enemy/Enemy_Blue.mv1");

	//敵の位置を読み込む
	LoadPos();

	m_enemyNum = kMaxEnemy;

	m_isGameClear = false;

	m_isDeadEnemy = false;

	m_statusPoint = 0;

	m_probability = 0;
}

EnemyManager::~EnemyManager()
{
	//モデルの削除
	MV1DeleteModel(m_shortDistanceEnemyHandle);
	MV1DeleteModel(m_longDistanceEnemyHandle);

	for (int i = 0; i < kEnemyNum; i++)
	{
		m_pShortDistanceEnemy[i].reset();
		m_pShortDistanceEnemy[i] = nullptr;

		m_pLongDistanceEnemy[i].reset();
		m_pLongDistanceEnemy[i] = nullptr;
	}

	m_pApple.reset();
	m_pApple = nullptr;
}

void EnemyManager::Init()
{
	//近距離エネミーの生成
	CreateShortDistanceEnemy();

	//遠距離エネミーの生成
	CreateLongDistanceEnemy();
}

void EnemyManager::Update(Stage& stage, Player& player)
{		
	for (int i = 0; i < kEnemyNum; i++)
	{
		if (m_pShortDistanceEnemy[i])
		{
			//近距離型の動き
			m_pShortDistanceEnemy[i]->Update(stage, player, player.GetPos());

			//プレイヤーに攻撃が当たったかどうか
			player.HitAttack(m_pShortDistanceEnemy[i]->GetHitCharacterAttack());

			//攻撃が当たったかどうか
			m_pShortDistanceEnemy[i]->HitPlayerAttack(player.GetAttackCapsuleStart(), player.GetAttackCapsuleEnd(),
		player.GetAttackRadius(), player.GetAttackPower());

			//プレイヤーの魔法攻撃が当たったかどうか
			m_pShortDistanceEnemy[i]->HitPlayerAttack(player.GetMagicCapsuleStart(), player.GetMagicCapsuleEnd(),
					player.GetMagicCapsuleRadius(), player.GetMagicPower());

			//必殺技が当たったかどうか
			m_pShortDistanceEnemy[i]->HitPlayerAttack(player.GetSpecialMoveStart(), player.GetSpecialMoveEnd(),
		player.GetSpecialMoveRadius(), player.GetAttackPower());

			//プレイヤーと当たったかどうか
			m_pShortDistanceEnemy[i]->HitPlayer(player);

			//もし死んでいたら削除する
			if (m_pShortDistanceEnemy[i]->GetIsDie())
			{
				m_statusPoint += kStatusPoint;

				m_isDeadEnemy = true;

				DrapItem(m_pShortDistanceEnemy[i]->GetPos());

				m_enemyNum--;

				m_pShortDistanceEnemy[i].reset();
				m_pShortDistanceEnemy[i] = nullptr;
			}
		}

		if (m_pLongDistanceEnemy[i])
		{
			//遠距離型の動き
			m_pLongDistanceEnemy[i]->Update(stage, player, player.GetPos());

			//プレイヤーの攻撃が当たったかどうか
			m_pLongDistanceEnemy[i]->HitPlayerAttack(player.GetAttackCapsuleStart(), player.GetAttackCapsuleEnd(),
		player.GetAttackRadius(), player.GetAttackPower());;

			//プレイヤーの魔法攻撃が当たったかどうか
			m_pLongDistanceEnemy[i]->HitPlayerAttack(player.GetMagicCapsuleStart(), player.GetMagicCapsuleEnd(),
					player.GetMagicCapsuleRadius(), player.GetMagicPower());

			//必殺技が当たったかどうか
			m_pLongDistanceEnemy[i]->HitPlayerAttack(player.GetSpecialMoveStart(), player.GetSpecialMoveEnd(),
		player.GetSpecialMoveRadius(), player.GetAttackPower());

			//当たったかどうかを渡す
			player.HitAttack(m_pLongDistanceEnemy[i]->GetHitCharacterAttack());

			//プレイヤーと当たったかどうか
			m_pLongDistanceEnemy[i]->HitPlayer(player);

			//死んでいた場合削除する
			if (m_pLongDistanceEnemy[i]->GetIsDie())
			{
				m_statusPoint += kStatusPoint;

				m_isDeadEnemy = true;

				DrapItem(m_pLongDistanceEnemy[i]->GetPos());

				m_enemyNum--;

				m_pLongDistanceEnemy[i].reset();
				m_pLongDistanceEnemy[i] = nullptr;
			}
		}
	}

	if (m_enemyNum <= 0)
	{
		m_isGameClear = true;
	}

	if (m_pApple)
	{
		m_pApple->Update();
		m_pApple->HitPlayer(player);

		if (m_pApple->GetIsHitPlayer())
		{
			m_pApple.reset();
			m_pApple = nullptr;
		}
	}
}


void EnemyManager::Draw()
{
//存在している敵を描画
	for (int i = 0; i < kEnemyNum; i++)
	{
		if (m_pShortDistanceEnemy[i])
		{
			m_pShortDistanceEnemy[i]->Draw();
		}

		if (m_pLongDistanceEnemy[i])
		{
			m_pLongDistanceEnemy[i]->Draw();
		}
	}

	//存在していたら描画
	if (m_pApple)
	{
		m_pApple->Draw();
	}
}

//近距離型の敵の生成
void EnemyManager::CreateShortDistanceEnemy()
{
	for (int i = 0; i < kEnemyNum; i++)
	{
		if (!m_pShortDistanceEnemy[i])
		{
			m_pShortDistanceEnemy[i] = std::make_shared<ShortDistanceEnemy>();

			m_pShortDistanceEnemy[i]->Init(MV1DuplicateModel(m_shortDistanceEnemyHandle), m_shortDistanceEnemyPos[i]);
		}
	}
}

//遠距離型の敵の生成
void EnemyManager::CreateLongDistanceEnemy()
{
	for (int i = 0; i < kEnemyNum; i++)
	{
		if (!m_pLongDistanceEnemy[i])
		{
			m_pLongDistanceEnemy[i] = std::make_shared <LongDistanceEnemy>();

			m_pLongDistanceEnemy[i]->Init(MV1DuplicateModel(m_longDistanceEnemyHandle),m_longDistanceEnemyPos[i]);
		}
	}
}

//敵の座標位置を読み込む
void EnemyManager::LoadPos()
{
	LoadCsv::GetInstance().LoadEnemyPos(m_shotDistanveEnemyPos, "shortdistanceenemypos");
	
	m_shortDistanceEnemyPos[0].x = m_shotDistanveEnemyPos.enemyPosX1;
	m_shortDistanceEnemyPos[0].y = m_shotDistanveEnemyPos.enemyPosY1;
	m_shortDistanceEnemyPos[0].z = m_shotDistanveEnemyPos.enemyPosZ1;

	m_shortDistanceEnemyPos[1].x = m_shotDistanveEnemyPos.enemyPosX2;
	m_shortDistanceEnemyPos[1].y = m_shotDistanveEnemyPos.enemyPosY2;
	m_shortDistanceEnemyPos[1].z = m_shotDistanveEnemyPos.enemyPosZ2;

	m_shortDistanceEnemyPos[2].x = m_shotDistanveEnemyPos.enemyPosX3;
	m_shortDistanceEnemyPos[2].y = m_shotDistanveEnemyPos.enemyPosY3;
	m_shortDistanceEnemyPos[2].z = m_shotDistanveEnemyPos.enemyPosZ3;

	m_shortDistanceEnemyPos[3].x = m_shotDistanveEnemyPos.enemyPosX4;
	m_shortDistanceEnemyPos[3].y = m_shotDistanveEnemyPos.enemyPosY4;
	m_shortDistanceEnemyPos[3].z = m_shotDistanveEnemyPos.enemyPosZ4;

	m_shortDistanceEnemyPos[4].x = m_shotDistanveEnemyPos.enemyPosX5;
	m_shortDistanceEnemyPos[4].y = m_shotDistanveEnemyPos.enemyPosY5;
	m_shortDistanceEnemyPos[4].z = m_shotDistanveEnemyPos.enemyPosZ5;

	m_shortDistanceEnemyPos[5].x = m_shotDistanveEnemyPos.enemyPosX6;
	m_shortDistanceEnemyPos[5].y = m_shotDistanveEnemyPos.enemyPosY6;
	m_shortDistanceEnemyPos[5].z = m_shotDistanveEnemyPos.enemyPosZ6;

	m_shortDistanceEnemyPos[6].x = m_shotDistanveEnemyPos.enemyPosX7;
	m_shortDistanceEnemyPos[6].y = m_shotDistanveEnemyPos.enemyPosY7;
	m_shortDistanceEnemyPos[6].z = m_shotDistanveEnemyPos.enemyPosZ7;

	m_shortDistanceEnemyPos[7].x = m_shotDistanveEnemyPos.enemyPosX8;
	m_shortDistanceEnemyPos[7].y = m_shotDistanveEnemyPos.enemyPosY8;
	m_shortDistanceEnemyPos[7].z = m_shotDistanveEnemyPos.enemyPosZ8;
	
	LoadCsv::GetInstance().LoadEnemyPos(m_longDistanveEnemyPos, "longdistanceenemypos");

	m_longDistanceEnemyPos[0].x = m_longDistanveEnemyPos.enemyPosX1;
	m_longDistanceEnemyPos[0].y = m_longDistanveEnemyPos.enemyPosY1;
	m_longDistanceEnemyPos[0].z = m_longDistanveEnemyPos.enemyPosZ1;

	m_longDistanceEnemyPos[1].x = m_longDistanveEnemyPos.enemyPosX2;
	m_longDistanceEnemyPos[1].y = m_longDistanveEnemyPos.enemyPosY2;
	m_longDistanceEnemyPos[1].z = m_longDistanveEnemyPos.enemyPosZ2;

	m_longDistanceEnemyPos[2].x = m_longDistanveEnemyPos.enemyPosX3;
	m_longDistanceEnemyPos[2].y = m_longDistanveEnemyPos.enemyPosY3;
	m_longDistanceEnemyPos[2].z = m_longDistanveEnemyPos.enemyPosZ3;

	m_longDistanceEnemyPos[3].x = m_longDistanveEnemyPos.enemyPosX4;
	m_longDistanceEnemyPos[3].y = m_longDistanveEnemyPos.enemyPosY4;
	m_longDistanceEnemyPos[3].z = m_longDistanveEnemyPos.enemyPosZ4;

	m_longDistanceEnemyPos[4].x = m_longDistanveEnemyPos.enemyPosX5;
	m_longDistanceEnemyPos[4].y = m_longDistanveEnemyPos.enemyPosY5;
	m_longDistanceEnemyPos[4].z = m_longDistanveEnemyPos.enemyPosZ5;

	m_longDistanceEnemyPos[5].x = m_longDistanveEnemyPos.enemyPosX6;
	m_longDistanceEnemyPos[5].y = m_longDistanveEnemyPos.enemyPosY6;
	m_longDistanceEnemyPos[5].z = m_longDistanveEnemyPos.enemyPosZ6;

	m_longDistanceEnemyPos[6].x = m_longDistanveEnemyPos.enemyPosX7;
	m_longDistanceEnemyPos[6].y = m_longDistanveEnemyPos.enemyPosY7;
	m_longDistanceEnemyPos[6].z = m_longDistanveEnemyPos.enemyPosZ7;

	m_longDistanceEnemyPos[7].x = m_longDistanveEnemyPos.enemyPosX8;
	m_longDistanceEnemyPos[7].y = m_longDistanveEnemyPos.enemyPosY8;
	m_longDistanceEnemyPos[7].z = m_longDistanveEnemyPos.enemyPosZ8;
	
}

void EnemyManager::DrapItem(VECTOR enemyPos)
{
	if (m_isDeadEnemy&& !m_pApple)
	{
		m_probability = GetRand(100);
		if (m_probability <= kDropRate)
		{
			m_pApple = std::make_shared<Apple>();
			m_pApple->Init(enemyPos);

			m_isDeadEnemy = false;
		}
		else
		{
			m_isDeadEnemy = false;
		}
	}
}
