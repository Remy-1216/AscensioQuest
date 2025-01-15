#pragma once
#include "DxLib.h"
#include <memory>
class Player;
class EffectManager;

class Apple
{
public:
	Apple();

	virtual ~Apple();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="enemyPos">死んだ敵の位置</param>
	void Init(VECTOR enemyPos);

	/// <summary>
	///　リンゴの動き 
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();


	/// <summary>
	/// プレイヤーに当たったときの処理
	/// </summary>
	void HitPlayer(Player& player);


	/// <summary>
	/// プレイヤーに当たったかどうかを渡す
	/// </summary>
	/// <returns></returns>
	bool GetIsHitPlayer() const { return m_isHitPlayer; }

private:

	//エフェクト
	std::shared_ptr<EffectManager> m_pEffectManager;

	//モデルのハンドル
	int m_handle;

	//プレイヤーに当たったかどうか
	bool m_isHitPlayer;

	//リンゴの座標位置
	VECTOR m_pos;

	//プレイヤーとの距離
	VECTOR m_distance;
};

