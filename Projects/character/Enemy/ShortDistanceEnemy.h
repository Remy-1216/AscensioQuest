#pragma once
#include "CharacterBase.h"
#include"EnemyStateBase.h"
#include "DxLib.h"
#include <memory>

class Player;
class Stage;

class ShortDistanceEnemy :   public CharacterBase, public std::enable_shared_from_this<ShortDistanceEnemy>
{

public:
	ShortDistanceEnemy();

	virtual ~ShortDistanceEnemy();

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init(int handle,VECTOR enemyPos);

	/// <summary>
	/// エネミーの行動
	/// </summary>
	/// <param name="player">プレイヤー</param>
	virtual void Update(Stage& stage, const Player& player, VECTOR playerPos);

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw();

	/// <summary>
	/// プレイヤーのどの攻撃を受けたのか
	/// </summary>
	/// <param name="player"></param>
	void HitAnyPlayerAttack(Player& player);

	/// <summary>
	/// プレイヤーに胴体や攻撃が当たったかどうか
	/// </summary>
	/// <param name="playar"></param>
	void HitPlayer(Player& player);
private:
	/// <summary>
	/// 当たり判定を更新
	/// </summary>
	void UpdateCol();
private:

	//近距離エネミーの状態
	std::shared_ptr<EnemyStateBase> m_pEnemyState;
};

