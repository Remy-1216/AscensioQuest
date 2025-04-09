#pragma once
#include <memory>
#include <string>
#include "DxLib.h"
#include "CharacterBase.h"

class Player;
class Stage;
class EnemyStateBase
{
public:
	//エネミーの状態
	enum StateKind
	{
		kIdle,
		kWalk,
		kAttack,
		kLightAttack,
		kStorongAttack,
		kThrowingAttack,
		kJump,
		kDamage,
		kStatusUp,
		kDie
	};


	/// <summary>
	/// エネミーの種類
	/// </summary>
	enum EnemyKinds
	{
		kShortDistanceEnemy,
		kLongDistanceEnemy,
		kBoss,
		kTutorialEnemy
	};

	EnemyStateBase(std::shared_ptr<CharacterBase>pEnemy) { m_pEnemy = pEnemy; m_isDie = false; }

	virtual void Update(Stage& stage,const Player& player, const int  characterKinds) = 0;

	virtual void Draw(const int  characterKinds) = 0;

	// 現在の状態を取得
	virtual StateKind GetKind() = 0;

	/// <summary>
	/// 死んだかどうかを渡す
	/// </summary>
	/// <returns></returns>
	bool GetIsDie() const { return m_isDie; }

public:
	std::shared_ptr<EnemyStateBase> m_nextState;	// 次のStateを保存する

protected:

	std::shared_ptr<CharacterBase> m_pEnemy;

	//ボスの行動確率
	int m_probability;

	/// <summary>
	/// プレイヤーとの距離
	/// </summary>
	float m_distance;

	/// <summary>
	/// 死んだかどうか
	/// </summary>
	bool m_isDie;
};

