#pragma once
#include "CharacterBase.h"
#include "PlayerStateBase.h"
#include "DxLib.h"
#include <memory>

class Stage;
class PlayerStatus;

class Player : public CharacterBase,public std::enable_shared_from_this<Player>
{
public:
	Player();

	virtual ~Player();

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init();

	/// <summary>
	/// 動き
	/// </summary>
	/// <param name="pad">コントローラー</param>
	/// <param name="camera">カメラ</param>
	void Update(Stage& stage, const Pad& pad,const Camera& camera);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 動いているときの処理
	/// </summary>
	/// <param name="moveVec">移動量</param>
	/// <param name="playerAngle">プレイヤーの向き</param>
	void Move(Stage& stage, const VECTOR& moveDirection);

	/// <summary>
	/// 攻撃をしているときの処理
	/// </summary>
	void Attack(int buttonCount,float animTime);

	/// <summary>
	/// 魔法攻撃をしているときの処理
	/// </summary>
	void MagicAttack();

	/// <summary>
	/// 必殺技を打つ時の処理
	/// </summary>
	/// <param name="gage"></param>
	void SpecialMove(const Pad& pad, bool isSpecialMove);

	/// <summary>
	/// ワープ地点に入った場合の処理
	/// </summary>
	/// <param name="warpTarget"></param>
	void Warp(const Pad& pad,VECTOR warpTarget);

	/// <summary>
	/// ダメージを受けた時の処理
	/// </summary>
	void Damage(float damage, int enemyType);

	/// <summary>
	///  HPの回復
	/// </summary>
	/// <param name="recoveryQuantity">回復量</param>
	void RecoverHp(int recoveryQuantity);

	/// <summary>
	/// プレイヤーと敵が当たったかどうか
	/// </summary>
	/// <param name="HitCollisionStart">当たり判定時に使うカプセルの二点中の一点の座標</param>
	/// <param name="HitCollisionEnd">当たり判定時に使うカプセルの二点中の一点の座標</param>
	/// <param name="HitRadius">カプセルの半径</param>
	bool HitEnemy(VECTOR enemyPos,VECTOR HitEnemyCollisionStart, VECTOR HitEnemyCollisionEnd, float HitEnemyRadius);

	/// <summary>
	/// プレイヤーに敵の攻撃が当たったかどうか
	/// </summary>
	/// <param name="enemyPos">敵の位置</param>
	/// <param name="HitEnemyAttackCollisionStart">当たり判定時に使うカプセルの二点中の一点の座標</param>
	/// <param name="HitEnemyAttackCollisionEnd">当たり判定時に使うカプセルの二点中の一点の座標</param>
	/// <param name="HitAttackEnemyRadius">カプセルの半径</param>
	/// <param name="enemyAttackPower">敵の攻撃力</param>
	/// <returns></returns>
	bool HitEnemyAttack(VECTOR enemyPos, VECTOR HitEnemyAttackCollisionStart, VECTOR HitEnemyAttackCollisionEnd, float HitAttackEnemyRadius);

	/// <summary>
	/// 攻撃が敵に当たったかどうかを受け取り、当たった時の処理と、当たっていなかった時の処理を行う
	/// </summary>
	/// <param name="hitAttack">攻撃が当たったかを受け取る</param>
	void HitAttack(bool hitAttack);

	/// <summary>
	/// 最大MPを渡す
	/// </summary>
	/// <returns></returns>
	int GetMaxMp() const { return m_maxMp; }

	/// <summary>
	/// 必殺技ゲージを渡す
	/// </summary>
	/// <returns></returns>
	int GetSpecialMoveGauge() const { return m_specialMoveGauge; }

	/// <summary>
	/// 受けたダメージを渡す
	/// </summary>
	/// <returns></returns>
	int GetDamage()const { return m_damage; }

	/// <summary>
	/// 足元にある球体の半径を渡す
	/// </summary>
	/// <returns></returns>
	float GetSphereRadius() const { return m_sphereRadius; }

	/// <summary>
	/// 死んだかどうかを渡す
	/// </summary>
	/// <returns></returns>
	bool GetIsPlayerDie() const { return m_isDie; }

	/// <summary>
	/// エネミーの攻撃や胴体に当たったかどうかを渡す
	/// </summary>
	/// <returns></returns>
	bool GetIsHitEnemy() const { return m_isHitEnemy; }

	/// <summary>
	/// 必殺技をうったかどうか
	/// </summary>
	/// <returns></returns>
	bool GetIsSpecialMove()const { return m_isSpecialMove; }

	/// <summary>
	/// プレイヤーがワープできる地点にいるかどうかを渡す
	/// </summary>
	/// <returns></returns>
	bool GetIsWarpPoint() const { return m_isWarpPoint; }

	/// <summary>
	/// 必殺技ゲージが100を超えているかどうか
	/// </summary>
	/// <returns></returns>
	bool GetIsSpecialMoveAvailable() const { return m_isSpecialMoveAvailable; }


private:

	/// <summary>
	/// 当たり判定の更新
	/// </summary>
	void UpdateCol();

private:
	//プレイヤーの状態
	std::shared_ptr<PlayerStateBase> m_pState;

	//ステータス強化した数値
	std::shared_ptr<PlayerStatus> m_pPlayerStatus;

	//ステータスアップされた値
	int m_hpUp;				//HPUP
	int m_attackUp;		//攻撃力UP
	int m_magicAttackUp;	//魔法攻撃力UP
	int m_defensePowerUp;	//防御力UP

	//最大ステータス
	int m_maxMp;

	//受けたダメージ
	int m_damage;

	//必殺技ゲージ
	int m_specialMoveGauge;

	//足元にある球体の半径
	float m_sphereRadius;

	//攻撃しているかどうか
	bool m_isAttack;

	//攻撃の当たり判定を出すかどうか
	bool m_isAttackCollision;

	//魔法攻撃をしているかどうか
	bool m_isMagicAttack;

	//魔法攻撃が移動しているかどうか
	bool m_isMagicAttackMove;

	//必殺技が撃てるかどうか
	bool m_isSpecialMoveAvailable;

	//必殺技をしているかどうか
	bool m_isSpecialMove;

	//エネミーの胴体に当たっているか
	bool m_isHitEnemy;

	//エネミーの攻撃が当たっているか
	bool m_isHitEnemyAttack;

	//ワープできる地点にいるかどうか
	bool m_isWarpPoint;

	//カメラの方向を変換した値
	VECTOR m_movementDirection;

	//球の位置
	VECTOR m_spherePos;

	//カプセルの進行方向
	VECTOR m_magicDirection;

};

