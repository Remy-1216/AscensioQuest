#include "SceneFirstStage.h"
#include "SceneResult.h"
#include "SceneClear.h"
#include "SceneTitle.h"
#include "EnemyManager.h"
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

	//UI背景の位置
	constexpr float kUIBGPosX = 980.0f;
}

SceneFirstStage::SceneFirstStage()
{
}

SceneFirstStage::~SceneFirstStage()
{
	m_pStage.reset();
	m_pStage = nullptr;

	m_pEnemyManager.reset();
	m_pEnemyManager = nullptr;

	// シャドウマップの削除
	DeleteShadowMap(m_shadowMapHandle);

	//画像の削除
	DeleteGraph(m_stageUIBGHandle);
}

void SceneFirstStage::Init()
{
	m_pStage = std::make_shared<Stage>(Stage1);
	m_pStage->Init();

	m_pEnemyManager = std::make_shared<EnemyManager>();
	m_pEnemyManager->Init();

	m_pPlayer->Init();

	m_pCamera->Init();

	m_pPlayerStatus->Init();

	m_pUIBar->Init();

	m_stageUIBGHandle = LoadGraph("data/BG/StageUIBG.png");

	//シャドウマップの作成
	m_shadowMapHandle = MakeShadowMap(kShadowMap, kShadowMap);

	// シャドウマップが想定するライトの方向もセット
	SetShadowMapLightDirection(m_shadowMapHandle, VGet(kLight, -kLight, kLight));

	// シャドウマップに描画する範囲を設定
	SetShadowMapDrawArea(m_shadowMapHandle, VGet(-kShadowMapRange, -1.0f, -kShadowMapRange), VGet(kShadowMapRange, kShadowMapRange, kShadowMapRange));

	//ゲームプレイ中のBGMを再生する
	m_pSoundManager->GamePlayBGM();
}

std::shared_ptr<SceneBase> SceneFirstStage::Update(const Pad& pad)
{
	//メニュー画面を開いていない場合動く
	if (!m_isOpenMenu)
	{
		//プレイヤーの動き
		m_pPlayer->Update(*m_pStage, pad, *m_pCamera);

		//カメラの動き
		m_pCamera->Update(m_pPlayer->GetPos());

		//敵の動き
		m_pEnemyManager->Update(*m_pStage, *m_pPlayer);
	}

	//プレイヤーがワープできる地点にいるかを受け取る
	m_isWarpPoint = m_pPlayer->GetIsWarpPoint();

	//プレイヤーが必殺技を撃てる状態かを受け取る
	m_isSpecialMoveAvailable = m_pPlayer->GetIsSpecialMoveAvailable();

	//敵の残り人数を受け取る
	m_enemyNum = m_pEnemyManager->GetEnemyNum();

	//ゲームをクリアしたかどうか
	m_isGameClear = m_pEnemyManager->GetGameClear();

	//ゲームオーバーかどうか
	m_isGameOver = m_pPlayer->GetIsPlayerDie();

	//フェード関係
	Fade();
	
	//タイトルに戻る処理
	BackToTitle(pad);

	if (m_isGameOver && m_fadeAlpha >= kFadeValue)
	{
		m_pPlayerStatus->StatOutput(m_pEnemyManager->GetStatusPoint());

		//プレイヤーのHPが0になったのでリザルト画面に遷移する
		return std::make_shared<SceneResult>();
	}
	if (m_isGameClear && m_fadeAlpha >= kFadeValue)
	{

		m_pPlayerStatus->StatOutput(m_pEnemyManager->GetStatusPoint());

		return std::make_shared<SceneClear>();
	}
	if (m_isBackToTheTitle && m_fadeAlpha >= kFadeValue)
	{
		m_pPlayerStatus->StatOutput(m_pEnemyManager->GetStatusPoint());

		return std::make_shared<SceneTitle>();
	}

	return shared_from_this();
}

void SceneFirstStage::Draw()
{
	// シャドウマップへの描画の準備
	ShadowMap_DrawSetup(m_shadowMapHandle);

	m_pStage->DrawShadowModel();

	m_pPlayer->DrawShadowModel();

	m_pEnemyManager->DrawShadowModel();

	//シャドウマップへの描画を終了
	ShadowMap_DrawEnd();

	// 描画に使用するシャドウマップを設定
	SetUseShadowMap(0, m_shadowMapHandle);

	//ステージの描画
	m_pStage->Draw();

	//マップの描画
	m_pStage->DrawMap(*m_pPlayer);

	DrawGraph(kUIBGPosX, 0, m_stageUIBGHandle, true);

	//エネミーの描画
	m_pEnemyManager->Draw();

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
	if (m_isGameOver)
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
	//タイトルに戻る時のフェードの描画
	if (m_isBackToTheTitle)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha); //半透明で表示
		DrawBoxAA(0, 0, Game::kScreenWindidth, Game::kScreenHeight, GetColor(0, 0, 0), true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); //不透明に戻しておく
	}

}

void SceneFirstStage::End()
{
}
