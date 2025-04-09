#pragma once
#include "EnemyStateBase.h"
class EnemyStateDamage : public EnemyStateBase
{
	public:
		EnemyStateDamage(std::shared_ptr<CharacterBase>enemy) :EnemyStateBase(enemy) {}

		void Init(const int  characterKinds);

		virtual void Update(Stage& stage, const Player& player, const int  characterKinds);

		virtual void Draw(const int  characterKinds);

		virtual StateKind GetKind() override { return StateKind::kAttack; }

private:

	/// <summary>
	/// 近距離型エネミーがダメージを受けた時の処理
	/// </summary>
	void ShortDistanveEnemyDamage(Stage& stage, const Player& player, const int  characterKinds);

	/// <summary>
	/// 遠距離型エネミーがダメージを受けた時の処理
	/// </summary>
	void LongDistanveEnemyDamage(Stage& stage, const Player& player, const int  characterKinds);

	/// <summary>
	/// ボスがダメージ受けた時の処理
	/// </summary>
	void BossDamage(Stage& stage, const Player& player, const int  characterKinds);

	/// <summary>
	/// チュートリアル用の敵がダメージを受けた時の処理
	/// </summary>
	void TutorialEnemyDamage(Stage& stage, const Player& player, const int  characterKinds);

private:
};

