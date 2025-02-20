#pragma once
#include "CharacterBase.h"
#include"EnemyStateBase.h"
#include "DxLib.h"
#include <memory>

class Player;
class Stage;

class LongDistanceEnemy :public CharacterBase, public std::enable_shared_from_this<LongDistanceEnemy>
{
public:

	LongDistanceEnemy();

	virtual ~LongDistanceEnemy();

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init(int handle, VECTOR enemyPos);

	/// <summary>
	/// エネミーの行動
	/// </summary>
	/// <param name="player">プレイヤー</param>
	virtual void Update(Stage& stage,const Player& player, VECTOR playerPos);

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw();

	/// <summary>
	/// プレイヤーと当たったかどうか
	/// </summary>
	/// <param name="playar"></param>
	void HitPlayer(Player& player);

	/// <summary>
	/// プレイヤーのどの攻撃に当たったのか
	/// </summary>
	void HitAnyPlayerAttack(Player& player);

private:
	/// <summary>
	/// 当たり判定を更新
	/// </summary>
	void UpdateCol();
private:

	//遠距離の敵の状態
	std::shared_ptr<EnemyStateBase> m_pEnemyState;

	//遠距離攻撃時に投げるもののモデルハンドル
	int m_throwingHandle;

	//カプセルの進行方向
	VECTOR m_attackDirection;
};

