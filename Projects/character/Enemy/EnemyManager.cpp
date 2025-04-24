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
	const char* const kUpperLeft = "upperLeft";
	const char* const kLowerRight = "lowerRight";

	//敵が死んだときにもらえるステータスポイント
	constexpr int kStatusPoint = 5;

	//最大敵数
	constexpr int kMaxEnemy = 16;

	//エリアごとの敵の人数
	constexpr int kFour = 4;
	constexpr int kThree = 3;
	constexpr int kTwo = 2;
	constexpr int kOne = 1;
	constexpr int kZero = 0;

	//アイテムドロップ率
	constexpr int kDropRate = 50;

	//テキストを描画する位置
	constexpr int kPosX = 800;
	constexpr int kPosY = 10;
	constexpr int kTextPosX = 800;
	constexpr int kTextPosY = 200;

	//アイコンの位置
	constexpr int kIconPosX = 1000;
	constexpr int kIconPosY = 0;

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

	//マップの端(エリアの端)を読む込む
	LoadCsv::GetInstance().LoadMapPosData(m_upperLeft, kUpperLeft);
	LoadCsv::GetInstance().LoadMapPosData(m_lowerRight, kLowerRight);

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

	DeleteGraph(m_fullEnemyHandle);
	DeleteGraph(m_threeEnemysHandle);
	DeleteGraph(m_twoEnemysHandle);
	DeleteGraph(m_oneEnemyHandle);
	DeleteGraph(m_zeroEnemyHandle);

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
	//敵が全員残っているときのアイコンをロードする
	m_fullEnemyHandle = LoadGraph("data/UI/FullEnemy.png");

	//残り敵数が3人の時のアイコンをロードする
	m_threeEnemysHandle = LoadGraph("data/UI/threeenemys.png");

	//残り敵数が2人の時のアイコンをロードする
	m_twoEnemysHandle = LoadGraph("data/UI/twoenemys.png");

	//残り敵数が1人の時のアイコンをロードする
	m_oneEnemyHandle = LoadGraph("data/UI/oneenemy.png");

	//敵が全滅した時のアイコンをロードする
	m_zeroEnemyHandle = LoadGraph("data/UI/zeroenemy.png");

	//近距離エネミーの生成
	CreateShortDistanceEnemy();

	//遠距離エネミーの生成
	CreateLongDistanceEnemy();

	//担当エリア決め
	AreaDecide();
}

void EnemyManager::Update(Stage& stage, Player& player)
{		
	//プレイヤーの座標を受け取る
	m_playerPos = player.GetPos();

	for (int i = 0; i < kEnemyNum; i++)
	{
		if (m_pShortDistanceEnemy[i])
		{
			//近距離型の動き
			m_pShortDistanceEnemy[i]->Update(stage, player, player.GetPos());
			
			//プレイヤーの攻撃に当たったかどうか
			m_pShortDistanceEnemy[i]->HitAnyPlayerAttack(player);

			//プレイヤーに胴体や攻撃が当たったかどうか
			m_pShortDistanceEnemy[i]->HitPlayer(player);

			//プレイヤーに攻撃が当たったかどうか
			player.HitAttack(m_pShortDistanceEnemy[i]->GetHitCharacterAttack());

			//もし死んでいたら削除する
			if (m_pShortDistanceEnemy[i]->GetIsKnockedDown())
			{
				//死んだときの処理
				ShortEnemyDies(i);
			}
		}

		if (m_pLongDistanceEnemy[i])
		{
			//遠距離型の動き
			m_pLongDistanceEnemy[i]->Update(stage, player, player.GetPos());
		
			//プレイヤーの攻撃に当たったかどうか
			m_pLongDistanceEnemy[i]->HitAnyPlayerAttack(player);

			//プレイヤーに自身の胴体や攻撃が当たったかどうか
			m_pLongDistanceEnemy[i]->HitPlayer(player);

			//プレイヤーに攻撃が当たったかどうか
			player.HitAttack(m_pLongDistanceEnemy[i]->GetHitCharacterAttack());

			//死んでいた場合削除する
			if (m_pLongDistanceEnemy[i]->GetIsKnockedDown())
			{
				LongEnemyDies(i);
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

	//エリアにいる残り敵数アイコンの描画
	DrawEnemuNum();

	//全体に残っている敵の数
	DrawFormatStringFToHandle(kTextPosX, kTextPosY, GetColor(0, 0, 0), Font::m_fontHandle[static_cast<int>(Font::FontId::kSize55_4)], "全エリアに残った敵の数%4d\n", m_enemyNum);

#ifdef _DEBUG
	DrawFormatString(0, 850, GetColor(0, 0, 0), "エリア1の敵の数:%4d", m_area.area1EnemyNum);
	DrawFormatString(0, 900, GetColor(0, 0, 0), "エリア2の敵の数:%4d", m_area.area2EnemyNum);
	DrawFormatString(0, 950, GetColor(0, 0, 0), "エリア3の敵の数:%4d", m_area.area3EnemyNum);
	DrawFormatString(0, 100, GetColor(0, 0, 0), "エリア4の敵の数:%4d", m_area.area4EnemyNum);
#endif
}

void EnemyManager::DrawShadowModel()
{
	//存在している敵を描画
	for (int i = 0; i < kEnemyNum; i++)
	{
		if (m_pShortDistanceEnemy[i])
		{
			m_pShortDistanceEnemy[i]->DrawShadowModel();
		}

		if (m_pLongDistanceEnemy[i])
		{
			m_pLongDistanceEnemy[i]->DrawShadowModel();
		}
	}

}

//敵の位置によって担当エリアを決める
void EnemyManager::AreaDecide()
{
	for (int i = 0; i < kEnemyNum; i++)
	{
		if (m_pShortDistanceEnemy[i]->GetPos().x >= m_upperLeft.map2PosX && m_pShortDistanceEnemy[i]->GetPos().z <= m_upperLeft.map2PosZ && 
			m_pShortDistanceEnemy[i]->GetPos().x <= m_lowerRight.map2PosX && m_pShortDistanceEnemy[i]->GetPos().z >= m_lowerRight.map2PosZ)
		{
			m_area.area1EnemyNum++;
		}
		else if (m_pShortDistanceEnemy[i]->GetPos().x >= m_upperLeft.map3PosX && m_pShortDistanceEnemy[i]->GetPos().z <= m_upperLeft.map3PosZ &&
			m_pShortDistanceEnemy[i]->GetPos().x <= m_lowerRight.map3PosX && m_pShortDistanceEnemy[i]->GetPos().z >= m_lowerRight.map3PosZ)
		{
			m_area.area2EnemyNum++;
		}
		else if (m_pShortDistanceEnemy[i]->GetPos().x >= m_upperLeft.map4PosX && m_pShortDistanceEnemy[i]->GetPos().z <= m_upperLeft.map4PosZ &&
			m_pShortDistanceEnemy[i]->GetPos().x <= m_lowerRight.map4PosX && m_pShortDistanceEnemy[i]->GetPos().z >= m_lowerRight.map4PosZ)
		{
			m_area.area3EnemyNum++;
		}
		else if (m_pShortDistanceEnemy[i]->GetPos().x >= m_upperLeft.map5PosX && m_pShortDistanceEnemy[i]->GetPos().z <= m_upperLeft.map5PosZ &&
			m_pShortDistanceEnemy[i]->GetPos().x <= m_lowerRight.map5PosX && m_pShortDistanceEnemy[i]->GetPos().z >= m_lowerRight.map5PosZ)
		{
			m_area.area4EnemyNum++;
		}

		if (m_pLongDistanceEnemy[i]->GetPos().x >= m_upperLeft.map2PosX && m_pLongDistanceEnemy[i]->GetPos().z <= m_upperLeft.map2PosZ &&
			m_pLongDistanceEnemy[i]->GetPos().x <= m_lowerRight.map2PosX && m_pLongDistanceEnemy[i]->GetPos().z >= m_lowerRight.map2PosZ)
		{
			m_area.area1EnemyNum++;
		}
		else if (m_pLongDistanceEnemy[i]->GetPos().x >= m_upperLeft.map3PosX && m_pLongDistanceEnemy[i]->GetPos().z <= m_upperLeft.map3PosZ &&
			m_pLongDistanceEnemy[i]->GetPos().x <= m_lowerRight.map3PosX && m_pLongDistanceEnemy[i]->GetPos().z >= m_lowerRight.map3PosZ)
		{
			m_area.area2EnemyNum++;
		}
		else if (m_pLongDistanceEnemy[i]->GetPos().x >= m_upperLeft.map4PosX && m_pLongDistanceEnemy[i]->GetPos().z <= m_upperLeft.map4PosZ &&
			m_pLongDistanceEnemy[i]->GetPos().x <= m_lowerRight.map4PosX && m_pLongDistanceEnemy[i]->GetPos().z >= m_lowerRight.map4PosZ)
		{
			m_area.area3EnemyNum++;
		}
		else if (m_pLongDistanceEnemy[i]->GetPos().x >= m_upperLeft.map5PosX && m_pLongDistanceEnemy[i]->GetPos().z <= m_upperLeft.map5PosZ &&
			m_pLongDistanceEnemy[i]->GetPos().x <= m_lowerRight.map5PosX && m_pLongDistanceEnemy[i]->GetPos().z >= m_lowerRight.map5PosZ)
		{
			m_area.area4EnemyNum++;
		}
	}
	
}

void EnemyManager::DrawEnemuNum()
{
	//エリア1にいる時
	if (m_playerPos.x >= m_upperLeft.map2PosX && m_playerPos.z <= m_upperLeft.map2PosZ &&
			m_playerPos.x <= m_lowerRight.map2PosX && m_playerPos.z >= m_lowerRight.map2PosZ)
	{
		DrawEnemyNumArea(m_area.area1EnemyNum);
	}
	//エリア2にいる時
	else if (m_playerPos.x >= m_upperLeft.map3PosX && m_playerPos.z <= m_upperLeft.map3PosZ &&
			m_playerPos.x <= m_lowerRight.map3PosX && m_playerPos.z >= m_lowerRight.map3PosZ)
	{
		DrawEnemyNumArea(m_area.area2EnemyNum);
	}
	//エリア3にいる時
	else if (m_playerPos.x >= m_upperLeft.map4PosX && m_playerPos.z <= m_upperLeft.map4PosZ &&
			m_playerPos.x <= m_lowerRight.map4PosX && m_playerPos.z >= m_lowerRight.map4PosZ)
	{
		DrawEnemyNumArea(m_area.area3EnemyNum);
	}
	//エリア4にいる時
	else if (m_playerPos.x >= m_upperLeft.map5PosX && m_playerPos.z <= m_upperLeft.map5PosZ &&
		m_playerPos.x <= m_lowerRight.map5PosX && m_playerPos.z >= m_lowerRight.map5PosZ)
	{
		DrawEnemyNumArea(m_area.area4EnemyNum);
	}

}

void EnemyManager::DrawEnemyNumArea(int enemyNum)
{	
	//残り敵数を描画
	DrawFormatStringFToHandle(kPosX, kPosY, GetColor(0, 0, 0), Font::m_fontHandle[static_cast<int>(Font::FontId::kSize64_4)], "残りエリア敵数\n");


	if (enemyNum == kFour)
	{
		DrawGraph(kIconPosX, kIconPosY, m_fullEnemyHandle, true);
	}
	else if (enemyNum == kThree)
	{
		DrawGraph(kIconPosX, kIconPosY, m_threeEnemysHandle, true);
	}
	else if (enemyNum == kTwo)
	{
		DrawGraph(kIconPosX, kIconPosY, m_twoEnemysHandle, true);
	}
	else if (enemyNum == kOne)
	{
		DrawGraph(kIconPosX, kIconPosY, m_oneEnemyHandle, true);
	}
	else if (enemyNum == kZero)
	{
		DrawGraph(kIconPosX, kIconPosY, m_zeroEnemyHandle, true);
	}
}

//近距離タイプの敵が死んだときの処理
void EnemyManager::ShortEnemyDies(int enemyNum)
{
	ShortEnemyDiesArea(enemyNum);

	m_statusPoint += kStatusPoint;

	m_isDeadEnemy = true;

	DrapItem(m_pShortDistanceEnemy[enemyNum]->GetPos());

	m_enemyNum--;

	m_pShortDistanceEnemy[enemyNum].reset();
	m_pShortDistanceEnemy[enemyNum] = nullptr;	
}

//どのエリアの敵が死んだのか確認
void EnemyManager::ShortEnemyDiesArea(int enemyNum)
{
	if (m_pShortDistanceEnemy[enemyNum]->GetPos().x >= m_upperLeft.map2PosX && m_pShortDistanceEnemy[enemyNum]->GetPos().z <= m_upperLeft.map2PosZ &&
			m_pShortDistanceEnemy[enemyNum]->GetPos().x <= m_lowerRight.map2PosX && m_pShortDistanceEnemy[enemyNum]->GetPos().z >= m_lowerRight.map2PosZ)
	{
		m_area.area1EnemyNum--;
	}
	else if (m_pShortDistanceEnemy[enemyNum]->GetPos().x >= m_upperLeft.map3PosX && m_pShortDistanceEnemy[enemyNum]->GetPos().z <= m_upperLeft.map3PosZ &&
		m_pShortDistanceEnemy[enemyNum]->GetPos().x <= m_lowerRight.map3PosX && m_pShortDistanceEnemy[enemyNum]->GetPos().z >= m_lowerRight.map3PosZ)
	{
		m_area.area2EnemyNum--;
	}
	else if (m_pShortDistanceEnemy[enemyNum]->GetPos().x >= m_upperLeft.map4PosX && m_pShortDistanceEnemy[enemyNum]->GetPos().z <= m_upperLeft.map4PosZ &&
		m_pShortDistanceEnemy[enemyNum]->GetPos().x <= m_lowerRight.map4PosX && m_pShortDistanceEnemy[enemyNum]->GetPos().z >= m_lowerRight.map4PosZ)
	{
		m_area.area3EnemyNum--;
	}
	else if (m_pShortDistanceEnemy[enemyNum]->GetPos().x >= m_upperLeft.map5PosX && m_pShortDistanceEnemy[enemyNum]->GetPos().z <= m_upperLeft.map5PosZ &&
		m_pShortDistanceEnemy[enemyNum]->GetPos().x <= m_lowerRight.map5PosX && m_pShortDistanceEnemy[enemyNum]->GetPos().z >= m_lowerRight.map5PosZ)
	{
		m_area.area4EnemyNum--;
	}
}

//遠距離タイプの敵が死んだときの処理
void EnemyManager::LongEnemyDies(int enemyNum)
{
	LongEnemyDiesArea(enemyNum);

	m_statusPoint += kStatusPoint;

	m_isDeadEnemy = true;

	DrapItem(m_pLongDistanceEnemy[enemyNum]->GetPos());

	m_enemyNum--;

	m_pLongDistanceEnemy[enemyNum].reset();
	m_pLongDistanceEnemy[enemyNum] = nullptr;
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

//どのエリアの敵が死んだのか確認
void EnemyManager::LongEnemyDiesArea(int enemyNum)
{
	if (m_pLongDistanceEnemy[enemyNum]->GetPos().x >= m_upperLeft.map2PosX && m_pLongDistanceEnemy[enemyNum]->GetPos().z <= m_upperLeft.map2PosZ &&
		m_pLongDistanceEnemy[enemyNum]->GetPos().x <= m_lowerRight.map2PosX && m_pLongDistanceEnemy[enemyNum]->GetPos().z >= m_lowerRight.map2PosZ)
	{
		m_area.area1EnemyNum--;
	}
	else if (m_pLongDistanceEnemy[enemyNum]->GetPos().x >= m_upperLeft.map3PosX && m_pLongDistanceEnemy[enemyNum]->GetPos().z <= m_upperLeft.map3PosZ &&
		m_pLongDistanceEnemy[enemyNum]->GetPos().x <= m_lowerRight.map3PosX && m_pLongDistanceEnemy[enemyNum]->GetPos().z >= m_lowerRight.map3PosZ)
	{
		m_area.area2EnemyNum--;
	}
	else if (m_pLongDistanceEnemy[enemyNum]->GetPos().x >= m_upperLeft.map4PosX && m_pLongDistanceEnemy[enemyNum]->GetPos().z <= m_upperLeft.map4PosZ &&
		m_pLongDistanceEnemy[enemyNum]->GetPos().x <= m_lowerRight.map4PosX && m_pLongDistanceEnemy[enemyNum]->GetPos().z >= m_lowerRight.map4PosZ)
	{
		m_area.area3EnemyNum--;
	}
	else if (m_pLongDistanceEnemy[enemyNum]->GetPos().x >= m_upperLeft.map5PosX && m_pLongDistanceEnemy[enemyNum]->GetPos().z <= m_upperLeft.map5PosZ &&
		m_pLongDistanceEnemy[enemyNum]->GetPos().x <= m_lowerRight.map5PosX && m_pLongDistanceEnemy[enemyNum]->GetPos().z >= m_lowerRight.map5PosZ)
	{
		m_area.area4EnemyNum--;
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

//ドロップ品を落とすか
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
