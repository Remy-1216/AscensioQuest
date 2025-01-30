#pragma once
#include "DxLib.h"
#include <memory>

class Boss;
class Player;
class CharacterBase;

class BossAI
{
public:

	BossAI();
	virtual ~BossAI();

	void Init();

	int StateSet(CharacterBase& boss,const  Player& player);

private:

	/// <summary>
	/// ボスの残りHPが高く、プレイヤーが近い場合の行動
	/// </summary>
	int HpHighlyRangeNear();

	/// <summary>
	/// ボスの残りHPが高く、プレイヤーが遠い場合の行動
	/// </summary>
	/// <returns></returns>
	int HpHighlyRangeFar();

	/// <summary>
	/// ボスの残りHPが低く、プレイヤーが近い場所にいる場合の行動
	/// </summary>
	/// <returns></returns>
	int HpLowRangeNear();


	/// <summary>
	/// ボスの残りHPが低く、プレイヤーが遠い場所に場合の行動
	/// </summary>
	/// <returns></returns>
	int HpLowRangeFar();

private:
	/// <summary>
	/// Stateの種類
	/// </summary>
	enum BossStateKinds
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
private:

	//ランダム
	int m_probability;

	//ボスがどのような動きをするか
	int m_state;

	//ボスとプレイヤーの距離
	float m_distance;

};

