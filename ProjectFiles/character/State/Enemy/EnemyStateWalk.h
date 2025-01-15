#pragma once
#include "EnemyStateBase.h"
class Player;

class BossAI;

class EnemyStateWalk :public EnemyStateBase
{
public:

	EnemyStateWalk(std::shared_ptr<CharacterBase>enemy):EnemyStateBase(enemy){}

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="characterKinds"></param>
	void Init(const int  characterKinds);

	/// <summary>
	/// 歩いているときの動き
	/// </summary>
	/// <param name="player">プレイヤー</param>
	/// <param name="characterKinds">敵の種類</param>
	virtual void Update(Stage& stage,const Player & player, const int  characterKinds);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="characterKinds"></param>
	virtual void Draw(const int  characterKinds);

	/// <summary>
	/// 現在の状態
	/// </summary>
	/// <returns></returns>
	virtual StateKind GetKind() override { return StateKind::kWalk; }

private:

	/// <summary>
	/// 近距離エネミーの歩いているときの動き
	/// </summary>
	/// <param name="stage"></param>
	/// <param name="player"></param>
	/// <param name="characterKinds"></param>
	void ShortDistanceEnemyWalk(Stage& stage, const Player& player, const int  characterKinds);

	/// <summary>
	/// 遠距離エネミーの歩いているときの動き
	/// </summary>
	/// <param name="stage"></param>
	/// <param name="player"></param>
	/// <param name="characterKinds"></param>
	void LongDistanceEnemyWalk(Stage& stage, const Player& player, const int  characterKinds);

	/// <summary>
	/// ボスの歩いているときの動き
	/// </summary>
	/// <param name="stage"></param>
	/// <param name="player"></param>
	/// <param name="characterKinds"></param>
	void BossWalk(Stage& stage, const Player& player, const int  characterKinds);

private:

	//ボスのAI
	std::shared_ptr<BossAI> m_pBossAI;

	//プレイヤーとの距離
	VECTOR m_move;

	//歩く速さ
	float m_walkSpeed;

	//攻撃のクールタイム
	int m_coolTime;
};

