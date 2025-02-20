#pragma once
#include "DxLib.h"
#include <memory>

class Boss;
class Player;
class CharacterBase;

class BossAI
{
public:

	struct AI
	{
		int rushAttackProbability = 0;		//突進攻撃の確率
		int lightAttackProbability = 0;		//通常攻撃の確率
		int storongAttackProbability = 0;	//強攻撃の確率
		int throwingAttackProbability = 0;	//遠距離攻撃の確率
		int jumpProbability = 0;			//ジャンプする確率
		int statusUpProbability = 0;		//強化する確率
	};

public:

	BossAI();
	virtual ~BossAI();

	void Init();

	//	どのような状況下なのかを調べる
	int StateSet(CharacterBase& boss,const  Player& player);

private:

	/// <summary>
	/// AIの動きを決める
	/// </summary>
	/// <returns></returns>
	int AIMotion(AI  bossAI);

private:
	/// <summary>
	/// Stateの種類
	/// </summary>
	enum BossStateKinds
	{
		kIdle,
		kWalk,
		kRushAttack,
		kLightAttack,
		kStorongAttack,
		kThrowingAttack,
		kJump,
		kDamage,
		kStatusUp,
		kDie
	};
private:

	AI m_bossAI;

	//ランダム
	int m_probability;

	//ボスがどのような動きをするか
	int m_state;

	//ボスとプレイヤーの距離
	float m_distance;

};

