#pragma once
class Player;
class CharacterBase;

class UIBar
{
public:
	UIBar();

	virtual ~UIBar();

	void Init();

	void Update();

	//プレイヤーバーの描画
	void DrawPlayerGaugeBar(const Player& player);

	//敵のHPバーの描画
	void DrawEnemyGaugeBar(const CharacterBase& characterBase);

private:
	//プレイヤーUI関係
	int m_playerBarHandle;					//プレイヤーHPバーの灰色部分のハンドル
	int m_playerHpBarHandle;				//プレイヤーHPバーのハンドル
	int m_playerDamageBarHandle;			//プレイヤーダメージバーのハンドル
	int m_playerMpBarHandle;				//プレイヤーMPバーのハンドル
	int m_playerSpecialMoveGaugeHandle;		//プレイヤーの必殺技ゲージのハンドル

	//敵UI関係
	int m_enemyBarHandle;					//敵HPバーの灰色部分のハンドル
	int m_enemyHpBarHandle;					//敵HPバーのハンドル
	int m_enemyDamageBarHandle;				//敵ダメージバーのハンドル

	//UI背景のハンドル
	int m_stageUIBGHandle;

	//バーをどれだけ表示するか
	float m_hpBarX;
	float m_damageBarX;
	float m_mpBarX;
	float m_specialMoveGaugeBarX;
	
	//受けたダメージを受け取る
	int m_enemyDamage;
};

