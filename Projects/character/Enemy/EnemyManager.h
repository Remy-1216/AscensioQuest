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

	//エリアの敵数
	struct Area
	{
		int area1EnemyNum = 0;
		int area2EnemyNum = 0;
		int area3EnemyNum = 0;
		int area4EnemyNum = 0;
	};

	//マップの左上、右下の位置
	struct AreaPos
	{
		float map1PosX;
		float map1PosZ;
		float map2PosX;
		float map2PosZ;
		float map3PosX;
		float map3PosZ;
		float map4PosX;
		float map4PosZ;
		float map5PosX;
		float map5PosZ;
	};

	//エリアの名前

	enum AreaName
	{
		Area1,
		Area2,
		Area3,
		Area4
	};

public:
	EnemyManager();
	virtual ~EnemyManager();

	void Init();

	void Update(Stage& stage,Player&player);

	void Draw();


	/// <summary>
	/// シャドウマップへの描画の準備
	/// </summary>
	void DrawShadowModel();

	/// <summary>
	/// ステータスポイントを渡す
	/// </summary>
	/// <returns></returns>
	int GetStatusPoint()const { return m_statusPoint; }

	/// <summary>
	/// 敵の数を渡す
	/// </summary>
	/// <returns></returns>
	int GetEnemyNum()const { return m_enemyNum; }

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
	/// 各エリアの敵を設定する
	/// </summary>
	void AreaDecide();

	/// <summary>
	/// プレイヤーの位置によって変更する
	/// </summary>
	void DrawEnemuNum();

	/// <summary>
	/// 各エリアの敵の数を描画する
	/// </summary>
	void DrawEnemyNumArea(int enemyNum);

	/// <summary>
	/// 近距離タイプの敵が死んだときの処理
	/// </summary>
	void ShortEnemyDies(int enemyNum);

	/// <summary>
	/// どのエリアの近距離タイプの敵が死んだのか確認
	/// </summary>
	/// <param name="enemyNum"></param>
	void ShortEnemyDiesArea(int enemyNum);

	/// <summary>
	/// 遠距離タイプの敵が死んだときの処理
	/// </summary>
	void LongEnemyDies(int enemyNum);

	/// <summary>
	/// どのエリアの近距離タイプの敵が死んだのか確認
	/// </summary>
	/// <param name="enemyNum"></param>
	void LongEnemyDiesArea(int enemyNum);

	/// <summary>
	/// エネミーの座標を読み取る
	/// </summary>
	void LoadPos();

	/// <summary>
	/// エネミーが死んだときに確率で回復アイテムを落とす
	/// </summary>
	void DrapItem(VECTOR enemyPos);

private:

	Area m_area;

	/// <summary>
	/// 遠距離エネミー
	/// </summary>
	std::shared_ptr<LongDistanceEnemy> m_pLongDistanceEnemy[kEnemyNum];

	/// <summary>
	/// 近距離エネミー
	/// </summary>
	std::shared_ptr<ShortDistanceEnemy> m_pShortDistanceEnemy[kEnemyNum];

	//左上の座標
	AreaPos m_upperLeft;

	//右下の座標
	AreaPos m_lowerRight;

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
	/// エリア名
	/// </summary>
	AreaName m_areaName;

	/// <summary>
	/// 近距離型エネミーのハンドル
	/// </summary>
	int m_shortDistanceEnemyHandle;

	/// <summary>
	/// 遠距離型エネミーのハンドル
	/// </summary>
	int m_longDistanceEnemyHandle;

	/// <summary>
	/// 全員敵が残っているときのハンドル
	/// </summary>
	int m_fullEnemyHandle;

	/// <summary>
	/// 敵数が残り3人の時のハンドル
	/// </summary>
	int m_threeEnemysHandle;

	/// <summary>
	/// 敵数が残り2人の時のハンドル
	/// </summary>
	int m_twoEnemysHandle;

	/// <summary>
	/// 敵数が残り1人の時のハンドル
	/// </summary>
	int m_oneEnemyHandle;

	/// <summary>
	/// 敵が全滅しているときのハンドル
	/// </summary>
	int m_zeroEnemyHandle;

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

	/// <summary>
	/// プレイヤーの位置
	/// </summary>
	VECTOR m_playerPos;
};

