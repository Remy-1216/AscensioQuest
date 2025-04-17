#pragma once
#include "CharacterBase.h"
#include"EnemyStateBase.h"

class TutorialEnemy :public CharacterBase, public std::enable_shared_from_this<TutorialEnemy>
{
public:

	//チュートリアル種類
	enum TutorialKinds
	{
		kTurorialStart,
		kFirstTutorial,		//第一チュートリアル
		kSecondTutorial,	//第二チュートリアル
		kThirdTutorial,		//第三チュートリアル
		kFourTutorial,		//第四チュートリアル
		kFifthTutorial,		//第五チュートリアル
		kSixTutorial,		//第六チュートリアル
	};

public:
	TutorialEnemy();

	virtual ~TutorialEnemy();

	virtual void Init(int handle,VECTOR enemyPos);

	virtual void Update(Stage& stage, Player& player, int tutorialKinds);

	virtual void Draw();

	//攻撃を受けた回数を渡す
	int GetAttackHitTimes() const {return m_attackHitTimes;}

	//魔法攻撃を受けた回数を渡す
	int GetMagicAttackHitTimes()const { return m_magicAttackHitTimes; }

	//必殺技を受けた回数を渡す
	int GetSpecialMoveHitTimes()const { return m_specialMoveHitTimes; }



private:

	/// <summary>
	/// チュートリアルの処理
	/// </summary>
	void TutorialUpdate(int tutorialKinds);

	/// <summary>
	/// プレイヤーに攻撃が当たった時の処理
	/// </summary>
	void HitPlayer(Player & player);

	/// <summary>
	/// プレイヤーのどの攻撃を受けたのか
	/// </summary>
	/// <param name="player"></param>
	void HitAnyPlayerAttack(Player& player);

	/// <summary>
	/// 当たり判定の更新
	/// </summary>
	void UpdateCol();

private:

	//敵の状態
	std::shared_ptr<EnemyStateBase> m_pEnemyState;

	//プレイヤーの剣の攻撃に当たった回数
	int m_attackHitTimes;

	//プレイヤーの魔法攻撃に当たった回数
	int m_magicAttackHitTimes;

	//プレイヤーの必殺技に当たった回数
	int m_specialMoveHitTimes;
};

