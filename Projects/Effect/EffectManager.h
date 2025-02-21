#pragma once
#include "DxLib.h"

class EffectManager
{
public:
	struct EffectData
	{
		int EffectTime;
		float EffectSize;
		float EffectAdjPosY;
	};

public:

	EffectManager();
	~EffectManager();
	void Init();
	void Update();
	void Draw();
	void ClearEffect();
	
	/// <summary>
	/// 魔法攻撃エフェクトの描画
	/// </summary>
	/// <param name="magicAttackPos">魔法攻撃の位置</param>
	void DrawMagicAttackEffect(VECTOR& magicAttackPos);


	/// <summary>
	/// ワープポイントエフェクトの描画
	/// </summary>
	/// <param name="worpPos">ワープポイントの位置</param>
	void DrawWarpPointEffect(const VECTOR worpPos, int warpPointNum);

	/// <summary>
	/// プレイヤーがダメージを受けた時のエフェクトの描画
	/// </summary>
	/// <param name="playerPos"></param>
	void DrawPlayerDamageEffect(VECTOR playerPos);

	/// <summary>
	/// 敵がダメージを受けた時のエフェクト
	/// </summary>
	/// <param name="enemyPos">敵の位置</param>
	void DrawEnemyDamageEffect(VECTOR enemyPos);

	/// <summary>
	/// ガード成功時のエフェクトの描画
	/// </summary>
	/// <param name="playerPos"></param>
	void DrawPlayerGuardEffect(VECTOR playerPos);


	/// <summary>
	/// アイテムが落ちているときのエフェクトの描画
	/// </summary>
	/// <param name="applePos"></param>
	void DrawItemEffect(VECTOR itemPos);


private:

	//ワープ可能箇所の数
	static constexpr int kWarpPointNum = 16;
private:

	//エフェクトのデータを読み込む
	EffectData m_effectData;

	//再生中のエフェクトハンドル
	int m_playingEffectHandle;

	//ワープポイントのエフェクト
	int m_warpPointEffectHandle;

	//魔法攻撃のエフェクト
	int m_magicAttackEffectHandle;

	//プレイヤーがダメージを受けた時のエフェクト
	int m_playerDamageEffectHandle;
	
	//敵がダメージを受けた時のエフェクト
	int m_enemyDamageEffectHandle;

	//プレイヤーのガードが成功した時のエフェクト
	int m_playerGuardEffectHandle;

	//アイテムが落ちているときのエフェクト
	int m_itemEffectHandle;

	// 魔法攻撃エフェクトを再生する時間
	float m_magicAttackEffectTime;	

	//プレイヤーがダメージを受けた時エフェクトを再生する時間
	float m_playerDamageEffectTime;

	//敵がダメージを受けた時のエフェクトを再生する時間
	float m_enemyDamageEffectTime;

	//ワープポイントエフェクトを再生する時間
	float m_warpPointEffectTime[kWarpPointNum];

	// ガードエフェクトを再生する時間
	float m_playerGuardEffectTime;

	//アイテムが落ちているときのエフェクトを再生する時間
	float m_itemEffectTime;

	//魔法攻撃エフェクトが再生されているか
	bool m_isMagicAttackEffect;

	//ワープポイントのエフェクトが再生されているか
	bool m_isWarpPointEffect[kWarpPointNum];

	//ダメージエフェクトが再生されているか
	bool m_isPlayerDamageEffect;

	//敵がダメージを受けた時のエフェクトが再生されているか
	bool m_isEnemyDamageEffect;

	//ガード成功時のエフェクトが再生されているか
	bool m_isPlayerGuardEffect;

	//アイテムが落ちているときのエフェクトが再生されているか
	bool m_isItemEffect;
};

