#pragma once
#include "CharacterBase.h"
#include"EnemyStateBase.h"
#include "DxLib.h"
#include <memory>

class EnemyStateBase;
class Player;
class Stage;

class Boss :public CharacterBase, public std::enable_shared_from_this<Boss>
{
public:

	Boss();

	virtual ~Boss();

	virtual void Init(int handle);

	virtual void Update(Stage& stage, Player& player, VECTOR playerPos);

	virtual void Draw();

	/// <summary>
	/// プレイヤーと当たったかどうか
	/// </summary>
	/// <param name="playar"></param>
	void HitPlayer(Player& player);

	/// <summary>
	/// プレイヤーの度の攻撃に当たったのか
	/// </summary>
	/// <param name="player"></param>
	void HitAnyPlayerAttack(Player& player);

	/// <summary>
	/// キャラクターの種類が何なのかを渡す
	/// </summary>
	/// <returns></returns>
	int GetCharacterKinds() const { return m_characterKind; }

	/// <summary>
	/// ステータスポイントを渡す
	/// </summary>
	/// <returns></returns>
	int GetStatusPoint()const { return m_statusPoint; }

	/// <summary>
	/// ゲームをクリアしたかを渡す
	/// </summary>
	/// <returns></returns>
	bool GetGameClear() const { return m_isGameClear; }

private:
	/// <summary>
	/// 当たり判定を更新
	/// </summary>
	void UpdateCol();

	/// <summary>
	/// 弱攻撃を行った時の当たり判定の更新
	/// </summary>
	void LightAttackCol();

	/// <summary>
	/// 強攻撃を行った時の当たり判定の更新
	/// </summary>
	void StorongAttackCol();

	/// <summary>
	/// 遠距離攻撃を行った時
	/// </summary>
	void ThrowingCol();

	/// <summary>
	/// 死んだ後の処理
	/// </summary>
	void Died();

private:

	//遠距離の敵の状態
	std::shared_ptr<EnemyStateBase> m_pEnemyState;

	//遠距離攻撃時に投げるもののモデルハンドル
	int m_throwingHandle;

	//ボスを倒したときに受け取れるスキルポイント
	int m_statusPoint;

	//死んだと同時に動かさないためのbool
	bool m_isMove;

	//ゲームをクリアしたかどうか
	bool m_isGameClear;

	//カプセルの進行方向
	VECTOR m_attackDirection;

	//攻撃のカプセルの中間位置
	VECTOR m_attackCapsuleIntermediate;

	//右肩の位置
	VECTOR m_rightShoulderPos;

	//右肘の位置
	VECTOR m_rightElbowPos;

	//右手の位置
	VECTOR m_rightHandPos;

	//左肩の位置
	VECTOR m_leftShoulderPos;

	//左肘の位置
	VECTOR m_leftElbowPos;

	//左手の位置
	VECTOR m_leftHandPos;

};

