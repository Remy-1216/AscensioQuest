#pragma once
#include "DxLib.h"
#include <string>
#include <vector>
#include <map>
#include <memory>

class Camera;
class Pad;
class CharacterStateBase;
class Stage;
class Player;
class SoundManager;
class EffectManager;
class UIBar;

class CharacterBase
{
public:
	/// <summary>
	/// ステータス構造体
	/// </summary>
	struct Status
	{
		float maxHp = 0.0f;				// 最大HP
		float maxMp = 0.0f;				// 最大MP
		float attackPower = 0.0f;			// 攻撃力
		float magicAttackPower = 0.0f;		// 魔法攻撃力
		float defensePower = 0.0f;			// 防御力
		float walkSpeed = 0.0f;			// 歩き速度
		float runSpeed = 0.0f;				// 走り速度
	};

	/// <summary>
	/// アニメーション関連
	/// </summary>
	struct AnimInfo
	{
		int number = 0;				//アニメーションの番号
		float loopFrame = 0.0f;		//アニメーションの最初の時間
		float endFrame = 0.0f;		//アニメーションの最後の時間
		float playSpeed = 0.0f;		//アニメーションの速度
	};

	/// <summary>
	/// 当たり判定関連
	/// </summary>
	struct CollisionInfo
	{
		VECTOR capsuleStartPoint = VGet(0.0f,0.0f,0.0f);				//胴体の当たり判定用のカプセルを形成する二点中の一点の座標
		VECTOR capsuleEndPoint = VGet(0.0f, 0.0f, 0.0f);				//胴体の当たり判定用のカプセルを形成する二点中の一点の座標
		VECTOR attackCapsuleStartPoint = VGet(0.0f, 0.0f, 0.0f);		//攻撃時に使うカプセルを形成する二点中の一点の座標
		VECTOR attackCapsuleEndPoint = VGet(0.0f, 0.0f, 0.0f);			//攻撃時に使うカプセルを形成する二点中の一点の座標
		VECTOR magicCapsuleStartPoint = VGet(0.0f, 0.0f, 0.0f);			//魔法攻撃に使うカプセルを形成する二点中の一点の座標
		VECTOR magicCapsuleEndPoint = VGet(0.0f, 0.0f, 0.0f);			//魔法攻撃に使うカプセルを形成する二点中の一点の座標
		VECTOR specialMoveStartPoint = VGet(0.0f, 0.0f, 0.0f);			//必殺技に使うカプセルを形成する二点中の一点の座標
		VECTOR specialMoveEndPoint = VGet(0.0f, 0.0f, 0.0f);			//必殺技に使うカプセルを形成する二点中の一点の座標
		float radius = 0.0f;											//胴体の当たり判定用のカプセルの半径
		float attackRadius = 0.0f;										//攻撃時に使うカプセルの半径
		float magicRadius = 0.0f;										//魔法攻撃に使うカプセルの半径
		float specialMoveRadius = 0.0f;									//必殺技に使うカプセルの半径
	};

	/// <summary>
	/// キャラクターの座標位置関連
	/// </summary>
	struct CharacterPos
	{
		float posX = 0.0f; //座標位置X
		float posY = 0.0f;	//座標位置Y
		float posZ = 0.0f;	//座標位置Z
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
	

public:
	CharacterBase(int handle);
	virtual ~CharacterBase();

	/// <summary>
	/// シャドウマップへの描画の準備
	/// </summary>
	void DrawShadowModel();

	//アニメーションの進行
	//ループしたかどうかを返す
	/// <summary>
	/// アニメーションを変更
	/// </summary>
	/// <param name="animName"></param>
	void ChangeAnim(std::string animName);			

	/// <summary>
	/// アニメーションを更新
	/// </summary>
	void UpdateAnim();			

	/// <summary>
	/// アニメーションの総再生時間を取得する
	/// </summary>
	float GetAnimTotalTime(std::string animName);

	/// <summary>
	/// 指定のキャラクターに向かって歩く
	/// </summary>
	/// <param name="stage">ステージ</param>
	/// <param name="distancePos">移動量</param>
	void ComingCharacter(Stage& stage,VECTOR moveVec);

	/// <summary>
	/// 近接型エネミーの攻撃時
	/// </summary>
	/// <param name="animTime">アニメーションの時間</param> 
	void ShortDistanceEnemyAttack(float animTime);

	/// <summary>
	/// 遠距離エネミーの攻撃時
	/// </summary>
	/// <param name="animTime">アニメーションの時間</param>
	void LongDistanceEnemyAttack();

	/// <summary>
	/// ボスの攻撃時
	/// </summary>
	/// <param name="attackKinds">攻撃の種類</param>
	void BossAttack(int attackKinds);

	/// <summary>
	/// ノックバック
	/// </summary>
	void KnockBack(VECTOR m_vector);

	/// <summary>
	/// 無敵時間
	/// </summary>
	void InvincibleTime();

	/// <summary>
	/// 向いている方向の更新
	/// </summary>
	/// <param name="playerpos"></param>
	void UpdateAngle(VECTOR playerpos);

	/// <summary>
	/// 死んだときの動き
	/// </summary>
	void Die();

	//シーンを抜けるときの処理
	void End();		

	/// <summary>
	/// プレイヤーの攻撃が当たった時の処理
	/// </summary>
	bool HitPlayerAttack(VECTOR attackStart, VECTOR attackEnd, float radius, int attackPower);

	/// <summary>
	/// HPを渡す
	/// </summary>
	/// <returns></returns>
	int GetHp() const { return m_hp; }

	/// <summary>
	/// MPを渡す
	/// </summary>
	/// <returns></returns>
	int GetMp() const { return m_mp; }

	/// <summary>
	/// 最大HPを渡す
	/// </summary>
	/// <returns></returns>
	int GetMaxHp() const { return m_maxHp; }

	/// <summary>
	/// 攻撃が当たった時に入るダメージ数を渡す
	/// </summary>
	/// <returns></returns>
	int GetAttackPower() const { return m_attackPower; }

	/// <summary>
	/// 魔法攻撃が当たった時に入るダメージ数を渡す
	/// </summary>
	/// <returns></returns>
	int GetMagicPower() const { return m_magicAttackPower; }

	/// <summary>
	/// アニメーションの再生している時間
	/// </summary>
	/// <returns></returns>
	float GetCurrentAnimTime() const { return m_currentAnimTime; }

	/// <summary>
	/// アニメーションのループが終わる時間を渡す
	/// </summary>
	float GetAnimLoopEndTime() const { return m_animLoopEndTime; }

	/// <summary>
	/// アニメーションの再生速度を渡す
	/// </summary>
	float GetAnimSpeed() const { return m_animPlaySpeed; }

	/// <summary>
	/// 歩く速さを渡す
	/// </summary>
	float GetWalkSpeed() const { return m_walkSpeed; }

	/// <summary>
	/// 他のキャラクターと自分自身が当たったかどうかを渡す
	/// </summary>
	/// <returns></returns>
	bool GetHitCharacter() const { return m_isHitCharacter; }

	/// <summary>
	/// キャラクターに攻撃が当たったかどうかを渡す
	/// </summary>
	bool GetHitCharacterAttack() const { return m_isHitAttack; }

	/// <summary>
	/// 死んだかどうかを渡す
	/// </summary>
	bool GetIsKnockedDown() const { return m_isKnockedDown; }

	/// <summary>
	/// チュートリアル4を行っているかどうか
	/// </summary>
	bool GetFourTutorial()const { return m_isFourTutorial; }

	/// <summary>
	/// キャラクターの座標を渡す
	/// </summary>
	VECTOR GetPos()const { return m_pos; }

	/// <summary>
	/// 攻撃の当たり判定カプセル座標(始点)を渡す
	/// </summary>
	/// <returns></returns>
	VECTOR GetAttackCapsuleStart() const { return m_attackCapsuleStart; }

	/// <summary>
	/// 攻撃の当たり判定カプセル座標(終点)を渡す
	/// </summary>
	/// <returns></returns>
	VECTOR GetAttackCapsuleEnd() const { return m_attackCapsuleEnd; }

	/// <summary>
	/// 攻撃の当たり判定カプセルの半径を渡す
	/// </summary>
	/// <returns></returns>
	float GetAttackRadius() const { return m_attackRadius; }

	/// <summary>
	/// 当たり判定のカプセル座標(始点)を渡す
	/// </summary>
	/// <returns></returns>
	VECTOR GetBodyCapsuleStart() const { return m_capsuleStart; }

	/// <summary>
	/// 当たり判定のカプセル座標(終点)を渡す
	/// </summary>
	/// <returns></returns>
	VECTOR GetBodyCapsuleEnd() const { return m_capsuleEnd; }

	/// <summary>
	/// カプセルの半径を渡す
	/// </summary>
	/// <returns></returns>
	float GetBodyRadius() const { return m_radius; }

	/// <summary>
	/// 魔法攻撃の当たり判定のカプセル座標(始点)を渡す
	/// </summary>
	/// <returns></returns>
	VECTOR GetMagicCapsuleStart() const { return m_magicCapsuleStart; }

	/// <summary>
	///  魔法攻撃の当たり判定のカプセル座標(終点)を渡す
	/// </summary>
	/// <returns></returns>
	VECTOR GetMagicCapsuleEnd() const { return m_magicCapsuleEnd;}

	/// <summary>
	/// 魔法攻撃の当たり判定のカプセルの半径を渡す
	/// </summary>
	/// <returns></returns>
	float GetMagicCapsuleRadius() const { return m_magicRadius; }

	/// <summary>
	/// 必殺技のカプセル座標(始点)を渡す
	/// </summary>
	/// <returns></returns>
	VECTOR GetSpecialMoveStart() const { return m_specialMoveStart; }

	/// <summary>
	/// 必殺技のカプセル座標(終点)を渡す
	/// </summary>
	/// <returns></returns>
	VECTOR GetSpecialMoveEnd() const { return m_specialMoveEnd; }

	/// <summary>
	/// 必殺技のカプセルの半径を渡す
	/// </summary>
	/// <returns></returns>
	float GetSpecialMoveRadius() const { return m_specialMoveRadius; }


	/// <summary>
	/// Stateを渡す
	/// </summary>
	/// <returns></returns>
	Status GetStatus() const { return m_status; }
protected:

	CharacterPos m_characterPos;

	//サウンドマネージャー
	std::shared_ptr<SoundManager> m_pSoundManager;

	//エフェクトマネージャー
	std::shared_ptr<EffectManager> m_pEffectManager;

	//UIバー
	std::shared_ptr<UIBar> m_pUIBar;

	//アニメーション情報
	std::map<std::string, AnimInfo> m_animData;		// アニメーションのデータ
	int m_currentPlayAnim;		// 現在のアニメーション
	int m_prevPlayAnim;			// 前に再生していたアニメーション
	float m_animBlendRate;		// 現在と過去のアニメーションのブレンド率
	float m_currentAnimTime;	// 現在のアニメーション再生時間
	float m_prevAnimTime;		// 前のアニメーション再生時間
	float m_totalAnimTime;		// アニメーションの総再生時間
	float m_animPlaySpeed; 		// アニメーションの再生速度
	float m_animLoopStartTime;	// アニメーションのループが始まる時間
	float m_animLoopEndTime;	// アニメーションのループが終わる時間
	bool m_isLoopAnim;			// アニメーションをループさせるか

	//ステータス関連
	Status m_status;		// ステータス
	int m_hp;				// HP
	float m_mp;				// MP
	int m_attackPower;		// 攻撃力
	int m_magicAttackPower;	// 魔法攻撃力
	int m_defensePower;		// 防御力
	float m_walkSpeed;		//歩く速さ
	float m_runSpeed;		//走る速さ

	//当たり判定関連
	CollisionInfo m_collisionInfo;		//キャラクター当たり判定
	VECTOR m_capsuleStartPoint;			//カプセルを形成する二点中の一点の座標
	VECTOR m_capsuleEndPoint;			//カプセルを形成する二点中の一点の座標
	VECTOR m_attackCapsuleStartPoint;	//攻撃のカプセルを形成する二点中の一点の座標
	VECTOR m_attackCapsuleEndPoint;		//攻撃のカプセルを形成する二点中の一点の座標
	VECTOR m_magicCapsuleStartPoint;	//魔法攻撃のカプセルを形成する二点中の一点の座標
	VECTOR m_magicCapsuleEndPoint;		//魔法攻撃のカプセルを形成する二点中の一点の座標
	VECTOR m_specialMoveStartPoint;		//必殺技のカプセルを形成する二点中の一点の座標
	VECTOR m_specialMoveEndPoint;		//必殺技のカプセルを形成する二点中の一点の座標
	float m_radius;						//半径
	float m_attackRadius;				//攻撃時のカプセルの半径
	float m_magicRadius;				//魔法攻撃のカプセルの半径
	float m_specialMoveRadius;			//必殺技のカプセルの半径

	//当たり判定保存用
	VECTOR m_capsuleStart;
	VECTOR m_capsuleEnd;
	VECTOR m_attackCapsuleStart;
	VECTOR m_attackCapsuleEnd;
	VECTOR m_magicCapsuleStart;
	VECTOR m_magicCapsuleEnd;
	VECTOR m_specialMoveStart;
	VECTOR m_specialMoveEnd;
		  
	//キャラクターの種類
	int m_characterKind;

	//モデルのハンドル
	int m_handle;

	//モデルのポリゴン数
	int m_modelPolygon;

	//重力
	float m_gravity;

	//最大ステータス
	int m_maxHp;

	//無敵時間
	int m_invincibleTime;

	//無敵時間の上限
	int m_invincibleTimeLimit;

	//キャラクター向いている方向
	float m_characterAngle;

	//攻撃の方向
	float m_attackAngle;

	//キャラクターと当たったか
	bool m_isHitCharacter;

	//キャラクターのアニメーションを止めるか
	bool m_isStopAnimation;

	//攻撃が当たったかどうか
	bool m_isHitAttack;

	//倒れたかどうか
	bool m_isKnockedDown;

	//無敵かどうか
	bool m_isInvincible;

	//近距離型エネミーが攻撃を行ったかどうか
	bool m_isShortDistanceEnemyAttack;

	//遠距離型エネミーが攻撃を行ったかどうか
	bool m_isLongDistanceEnemyAttack;

	//遠距離攻撃をしているかどうか
	bool m_isAttack;

	//遠距離攻撃が移動しているかどうか
	bool m_isAttackMove;

	//ボスが弱攻撃を行ったかどうか
	bool m_isLightAttack;

	//ボスが強攻撃を行ったかどうか
	bool m_isStorongAttack;

	//ボスが遠距離攻撃を行ったかどうか
	bool m_isThrowing;

	//敵の胴体に当たっているか
	bool m_isHitEnemy;

	//死んだと同時に動かさないためのbool
	bool m_isMove;

	//チュートリアル4かどうか
	bool m_isFourTutorial;

	//モデルの座標
	VECTOR m_pos;

	// 向くべき方向のベクトル
	VECTOR m_moveDir;

	//どれだけ進むのか
	VECTOR m_distance;

	//ノックバック
	VECTOR m_knockBack;

	//３Ｄモデル２から３Ｄモデル１に向かうベクトルを算出
	VECTOR m_subVector;
};

