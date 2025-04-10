#include "SceneSecondStage.h"
#include "Boss.h"
#include "SceneResult.h"
#include "SceneClear.h"
#include "Player.h"
#include "Camera.h"
#include "UIBar.h"
#include "SoundManager.h"
#include "PlayerStatus.h"
#include "Stage.h"
#include "Font.h"
#include "Game.h"

namespace
{
	//シャドウマップを制作するときに使用する値
	constexpr int kShadowMap = 2048;
	//シャドウマップの描画範囲
	constexpr float kShadowMapRange = 7500.0f;

	//ライトの方向
	constexpr float kLight = 0.5f;

	//フェードイン、フェードアウトの数値
	constexpr int kFadeValue = 255;

	//残りの敵数を表示する位置
	constexpr float kPosX = 1000.0f;
	constexpr float kPosY = 50.0f;
}

SceneSecondStage::SceneSecondStage()
{
}

SceneSecondStage::~SceneSecondStage()
{
	m_pStage.reset();
	m_pStage = nullptr;

	m_pBoss.reset();
	m_pBoss = nullptr;

	// シャドウマップの削除
	DeleteShadowMap(m_shadowMapHandle);
}

void SceneSecondStage::Init()
{
	m_pStage = std::make_shared<Stage>(Stage2);
	m_pStage->Init();

	m_pBoss = std::make_shared<Boss>();
	m_bossHandle = MV1LoadModel("data/model/enemy/Boss.mv1");
	m_pBoss->Init(m_bossHandle);

	m_pPlayer->Init();

	m_pCamera->Init();

	m_pPlayerStatus->Init();

	m_pUIBar->Init();

	//シャドウマップの作成
	m_shadowMapHandle = MakeShadowMap(kShadowMap, kShadowMap);

	// シャドウマップが想定するライトの方向もセット
	SetShadowMapLightDirection(m_shadowMapHandle, VGet(kLight, -kLight, kLight));

	// シャドウマップに描画する範囲を設定
	SetShadowMapDrawArea(m_shadowMapHandle, VGet(-kShadowMapRange, -1.0f, -kShadowMapRange), VGet(kShadowMapRange, kShadowMapRange, kShadowMapRange));

	//ゲームプレイ中のBGMを再生する
	m_pSoundManager->GamePlayBGM();
}

std::shared_ptr<SceneBase> SceneSecondStage::Update(const Pad& pad)
{
	//プレイヤーの動き
	if (!m_isOpenMenu)
	{
		m_pPlayer->Update(*m_pStage, pad, *m_pCamera);

		//カメラの動き
		m_pCamera->Update(m_pPlayer->GetPos());

		//ボスの動き
		m_pBoss->Update(*m_pStage, *m_pPlayer, m_pPlayer->GetPos());
	}


	//ゲームをクリアしたかどうかを受け取る
	m_isGameClear = m_pBoss->GetGameClear();

	//プレイヤーが倒れたかどうか
	m_isGameOver = m_pPlayer->GetIsKnockedDown();

	//フェード関係
	Fade();

	//タイトルに戻る処理
	BackToTitle(pad);


	if (m_pPlayer->GetIsPlayerDie() && m_fadeAlpha >= kFadeValue)
	{
		m_pPlayerStatus->StatOutput(m_pBoss->GetStatusPoint());

		//プレイヤーのHPが0になったのでリザルト画面に遷移する
		return std::make_shared<SceneResult>();
	}
	if (m_isGameClear && m_fadeAlpha >= kFadeValue)
	{

		m_pPlayerStatus->StatOutput(m_pBoss->GetStatusPoint());

		return std::make_shared<SceneClear>();
	}

	return shared_from_this();
}

void SceneSecondStage::Draw()
{
	// シャドウマップへの描画の準備
	ShadowMap_DrawSetup(m_shadowMapHandle);

	m_pStage->DrawShadowModel();

	m_pPlayer->DrawShadowModel();

	m_pBoss->DrawShadowModel();

	//シャドウマップへの描画を終了
	ShadowMap_DrawEnd();

	// 描画に使用するシャドウマップを設定
	SetUseShadowMap(0, m_shadowMapHandle);

	//ステージの描画
	m_pStage->Draw();

	//マップの描画
	m_pStage->DrawMap(*m_pPlayer);

	//エネミーの描画
	m_pBoss->Draw();

	//プレイヤーの描画
	m_pPlayer->Draw();

	// 描画に使用するシャドウマップの設定を解除
	SetUseShadowMap(0, -1);

	//操作説明を表示
	OperationUI();

	//HPバーなどを表示する
	m_pUIBar->DrawPlayerGaugeBar(*m_pPlayer);

	//メニュー画面を表示
	DrawMenu();

	if (!m_pPlayer->GetIsPlayerDie())
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha); //半透明で表示
		DrawBoxAA(0, 0, Game::kScreenWindidth, Game::kScreenHeight, GetColor(0, 0, 0), true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); //不透明に戻しておく
	}
	//ゲームオーバー時のフェードの描画
	if (m_pPlayer->GetIsPlayerDie())
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha); //半透明で表示
		DrawBoxAA(0, 0, Game::kScreenWindidth, Game::kScreenHeight, GetColor(157, 9, 12), true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); //不透明に戻しておく
	}
	//ゲームクリア時のフェードの描画
	if (m_isGameClear)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha); //半透明で表示
		DrawBoxAA(0, 0, Game::kScreenWindidth, Game::kScreenHeight, GetColor(240, 215, 53), true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); //不透明に戻しておく
	}
}

void SceneSecondStage::End()
{
}
