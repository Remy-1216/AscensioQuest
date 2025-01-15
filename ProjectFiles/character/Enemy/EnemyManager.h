#pragma once
#include "DxLib.h"
#include <memory>

class ShortDistanceEnemy;
class LongDistanceEnemy;
class Player;
class Stage;
class Apple;

class EnemyManager
{
public:

	struct Pos
	{
		float enemyPosX1 = 0.0f;		//座標位置X
		float enemyPosY1 = 0.0f;		//座標位置Y
		float enemyPosZ1 = 0.0f;		//座標位置Z
		float enemyPosX2 = 0.0f;		//座標位置X
		float enemyPosY2 = 0.0f;		//座標位置Y
		float enemyPosZ2 = 0.0f;		//座標位置Z
		float enemyPosX3 = 0.0f;		//座標位置X
		float enemyPosY3 = 0.0f;		//座標位置Y
		float enemyPosZ3 = 0.0f;		//座標位置Z
		float enemyPosX4 = 0.0f;		//座標位置X
		float enemyPosY4 = 0.0f;		//座標位置Y
		float enemyPosZ4 = 0.0f;		//座標位置Z
		float enemyPosX5 = 0.0f;		//座標位置X
		float enemyPosY5 = 0.0f;		//座標位置Y
		float enemyPosZ5 = 0.0f;		//座標位置Z
		float enemyPosX6 = 0.0f;		//座標位置X
		float enemyPosY6 = 0.0f;		//座標位置Y
		float enemyPosZ6 = 0.0f;		//座標位置Z
		float enemyPosX7 = 0.0f;		//座標位置X
		float enemyPosY7 = 0.0f;		//座標位置Y
		float enemyPosZ7 = 0.0f;		//座標位置Z
		float enemyPosX8 = 0.0f;		//座標位置X
		float enemyPosY8 = 0.0f;		//座標位置Y
		float enemyPosZ8 = 0.0f;		//座標位置Z
	};

public:
	EnemyManager();
	virtual ~EnemyManager();

	void Init();

	void Update(Stage& stage,Player&player);

	void Draw();


	/// <summary>
	/// ステータスポイントを渡す
	/// </summary>
	/// <returns></returns>
	int GetStatusPoint()const { return m_statusPoint; }

	/// <summary>
	/// ゲームをクリアしたかを渡す
	/// </summary>
	/// <returns></returns>
	bool GetGameClear() const { return m_isGameClear; }

private:

	/// <summary>
	/// エネミーの数
	/// </summary>
	static constexpr int kEnemyNum = 8;
private:

	/// <summary>
	/// 近距離エネミーの生成
	/// </summary>
	void CreateShortDistanceEnemy();

	/// <summary>
	///	遠距離エネミーの生成
	/// </summary>
	void CreateLongDistanceEnemy();

	/// <summary>
	/// エネミーの座標を読み取る
	/// </summary>
	void LoadPos();

	/// <summary>
	/// エネミーが死んだときに確率で回復アイテムを落とす
	/// </summary>
	void DrapItem(VECTOR enemyPos);

private:

	/// <summary>
	/// 遠距離エネミー
	/// </summary>
	std::shared_ptr<LongDistanceEnemy> m_pLongDistanceEnemy[kEnemyNum];

	/// <summary>
	/// 近距離エネミー
	/// </summary>
	std::shared_ptr<ShortDistanceEnemy> m_pShortDistanceEnemy[kEnemyNum];

	/// <summary>
	/// 近距離型エネミーのHP
	/// </summary>
	int m_ShortDistanceEnemyHp[kEnemyNum];

	/// <summary>
	/// 遠距離型エネミーのHP
	/// </summary>
	int m_LongDistanceEnemyHp[kEnemyNum];

	/// <summary>
	/// 近距離型エネミーの位置を設定
	/// </summary>
	VECTOR m_shortDistanceEnemyPos[kEnemyNum];

	/// <summary>
	/// 近距離型エネミーの位置を設定
	/// </summary>
	VECTOR m_longDistanceEnemyPos[kEnemyNum];
private:

	std::shared_ptr<Apple> m_pApple;

	/// <summary>
	/// 近距離型エネミーの座標情報
	/// </summary>
	Pos m_shotDistanveEnemyPos;


	/// <summary>
	/// 遠距離型エネミーの座標情報
	/// </summary>
	Pos m_longDistanveEnemyPos;

	/// <summary>
	/// 近距離型エネミーのハンドル
	/// </summary>
	int m_shortDistanceEnemyHandle;

	/// <summary>
	/// 遠距離型エネミーのハンドル
	/// </summary>
	int m_longDistanceEnemyHandle;

	/// <summary>
	/// エネミーの人数
	/// </summary>
	int m_enemyNum;

	/// <summary>
	/// 敵が死んだときに受け取るステータスポイント
	/// </summary>
	int m_statusPoint;

	/// <summary>
	/// プレイヤーの攻撃力
	/// </summary>
	int m_playerAttackPower;

	/// <summary>
	/// アイテムドロップ回数
	/// </summary>
	int m_dorps;

	/// <summary>
	/// アイテムが出現する確率
	/// </summary>
	int m_probability;

	/// <summary>
	/// 攻撃当たり判定のカプセルの半径
	/// </summary>
	float m_playerAttackRadius;

	/// <summary>
	/// 必殺技の当たり判定のカプセルの半径
	/// </summary>
	float m_playerSpecialMoveRadius;

	/// <summary>
	/// 敵が死んだかどうか
	/// </summary>
	bool m_isDeadEnemy;

	/// <summary>
	/// ゲームをクリアしたかどうか
	/// </summary>
	bool m_isGameClear;
};

