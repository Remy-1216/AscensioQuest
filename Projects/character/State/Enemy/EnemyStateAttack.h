#pragma once
#include "EnemyStateBase.h"
class EnemyStateAttack : public EnemyStateBase
{
public:
	
	EnemyStateAttack(std::shared_ptr<CharacterBase>enemy) :EnemyStateBase(enemy) {}

	void Init(const int  characterKinds,int attackKinds);

	virtual void Update(Stage& stage, const Player& player, const int  characterKinds);

	virtual void Draw(const int  characterKinds);

	virtual StateKind GetKind() override { return StateKind::kAttack; }

private:

	/// <summary>
	/// 近距離型エネミーの攻撃
	/// </summary>
	void ShortDistanveEnemyAttack(Stage& stage, const Player& player, const int  characterKinds);

	/// <summary>
	/// 遠距離型エネミーの攻撃
	/// </summary>
	void LongDistanveEnemyAttack(Stage& stage, const Player& player, const int  characterKinds);
	
	/// <summary>
	/// ボスの攻撃
	/// </summary>
	void BossAttack(Stage& stage, const Player& player, const int  characterKinds);

	/// <summary>
	/// チュートリアル用のエネミーの攻撃
	/// </summary>
	void TutorialEnemyAttack(Stage& stage, const Player& player, const int  characterKinds);

private:

	//攻撃の種類
	int m_attackKinds;

	//攻撃した回数
	int m_attackNum;
};

