#include "UIBar.h"
#include "Player.h"
#include "Game.h"
#include "DxLib.h"
#include "CharacterBase.h"
#include <vector>
#include <algorithm>
#include <cassert>
#include<cmath>

namespace
{
	//プレイヤーのHPバー
	constexpr float kUIBar = 1920.0f;

	//敵のHPバー
	constexpr float kEnemyBarX = 10.0f;

	constexpr float kEnemyBarY = 2.0f;

	//描画する座標位置
	constexpr float kPosY = 10.0f;

	//UIバーの位置
	constexpr float kPlayerBarX = 10.0f;

}



UIBar::UIBar():m_playerBarHandle(-1), m_playerHpBarHandle(-1), m_playerDamageBarHandle(-1),m_playerMpBarHandle(-1), m_hpBarX(0.0f),m_damageBarX(0.0f),
m_mpBarX(0.0f), m_specialMoveGaugeBarX(0.0f), m_enemyDamage(0)
{
}

UIBar::~UIBar()
{
	//UI画像の削除
	DeleteGraph(m_playerBarHandle);
	DeleteGraph(m_playerHpBarHandle);
	DeleteGraph(m_playerDamageBarHandle);
	DeleteGraph(m_playerMpBarHandle);
	DeleteGraph(m_playerSpecialMoveGaugeHandle);
	DeleteGraph(m_enemyBarHandle);
	DeleteGraph(m_enemyHpBarHandle);
	DeleteGraph(m_enemyDamageBarHandle);
}

void UIBar::Init()
{
	//UI関係のロード
	m_playerBarHandle = LoadGraph("data/Ui/Bar.png");
	m_playerHpBarHandle = LoadGraph("data/UI/HpBar.png");
	m_playerDamageBarHandle = LoadGraph("data/UI/DamageBar.png");
	m_playerMpBarHandle = LoadGraph("data/UI/MpBar.png");
	m_playerSpecialMoveGaugeHandle = LoadGraph("data/UI/specialmovegauge.png");
	m_enemyBarHandle = LoadGraph("data/UI/enemy_hp.png");
	m_enemyHpBarHandle = LoadGraph("data/UI/enemy_hpBar.png");
	m_enemyDamageBarHandle = LoadGraph("data/UI/enemy_damagebar.png");
}

void UIBar::Update()
{

}

void UIBar::DrawPlayerGaugeBar(const Player& player)
{
	DrawExtendGraphF(kPlayerBarX, 0.0f, Game::kScreenWindidth, Game::kScreenHeight, m_playerBarHandle, true);
	
	float damageBar = static_cast<float>(player.GetHp()) / static_cast<float>(player.GetMaxHp());
	damageBar = std::max(0.0f, damageBar);
	float hpBar = Game::kScreenWindidth * damageBar; // HPバーの長さを計算

	// ダメージバーの描画
	DrawExtendGraphF(kPlayerBarX, 0.0f, hpBar, Game::kScreenHeight, m_playerHpBarHandle, true);

	// HPバーを左端固定で描画（左端を 0.0f にする）
	DrawExtendGraphF(kPlayerBarX, 0.0f, hpBar, Game::kScreenHeight, m_playerHpBarHandle, true);

	// MPバーの計算
	float mpBar = static_cast<float>(player.GetMp()) / static_cast<float>(player.GetMaxMp());
	mpBar = std::min(1.0f, std::max(0.0f, mpBar));
	float mpBarWidth = kUIBar * mpBar;

	// MPバーの描画
	DrawExtendGraphF(kPlayerBarX, 0.0f, mpBarWidth, Game::kScreenHeight, m_playerMpBarHandle, true);

	// 必殺技ゲージの計算
	float specialMoveGaugeBar = static_cast<float>(player.GetSpecialMoveGauge()) / 100.0f;
	specialMoveGaugeBar = std::min(1.0f, std::max(0.0f, specialMoveGaugeBar));
	float specialMoveGaugeWidth = kUIBar * specialMoveGaugeBar;

	// 必殺技ゲージの描画
	if (specialMoveGaugeBar > 0.0f)
	{
		DrawExtendGraphF(kPlayerBarX, 0.0f, specialMoveGaugeWidth, Game::kScreenHeight, m_playerSpecialMoveGaugeHandle, true);
	}
}


void UIBar::DrawEnemyGaugeBar(const CharacterBase& characterBase)
{
	// 敵の頭上の位置を計算する
	VECTOR headPos = VAdd(characterBase.GetPos(), VGet(0.0f, kPosY, 0.0f));

	// カメラの視野内にいるかどうかを判定
	if (CheckCameraViewClip(headPos) == TRUE)
	{
		// ワールド座標をスクリーン座標に変換する
		VECTOR screenPos = ConvWorldPosToScreenPos(headPos);


		// HPバーの背景を描画
		DrawExtendGraphF(screenPos.x - kEnemyBarX, screenPos.y - kEnemyBarY,
						 screenPos.x + kEnemyBarX, screenPos.y + kEnemyBarY, m_enemyBarHandle, true);

		// 敵のHPが0以下になる場合、ダメージ部分を表示しない
		if (characterBase.GetHp() <= 0.0f)
		{
			m_enemyDamage = 0.0f;
		}

		// ダメージバーの長さを計算
		float hp = characterBase.GetHp() + m_enemyDamage;
		float damageHpRatio = hp / characterBase.GetMaxHp();
		float damageHpLength = kEnemyBarX * damageHpRatio;

		// ダメージバーを描画
		DrawExtendGraphF(screenPos.x - kEnemyBarX, screenPos.y - kEnemyBarY,
						 (screenPos.x - kEnemyBarX) + damageHpLength, screenPos.y + kEnemyBarY, m_enemyDamageBarHandle, true);

		// HPバーの長さを計算
		float hpRatio = static_cast<float>(characterBase.GetHp()) / static_cast<float>(characterBase.GetMaxHp());
		float hpLength = kEnemyBarX * hpRatio;

		// HPバーを描画
		DrawExtendGraphF(screenPos.x - kEnemyBarX, screenPos.y - kEnemyBarY,
						 (screenPos.x - kEnemyBarX) + hpLength, screenPos.y + kEnemyBarY, m_enemyHpBarHandle, true);

	}
}