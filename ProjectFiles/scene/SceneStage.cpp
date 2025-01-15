#include "SceneStage.h"
#include "SceneResult.h"
#include "EnemyManager.h"
#include "SceneClear.h"
#include "Player.h"
#include "Camera.h"
#include "Stage.h"
#include "Boss.h"
#include "PlayerStatus.h"
#include "SoundManager.h"
#include "Pad.h"
#include "Game.h"

namespace
{
	//フェードイン、フェードアウトの数値
	constexpr int kFadeValue = 255;

	//フェード値の増減
	constexpr int kFadeUpDown =5;

	//シャドウマップを制作するときに使用する値
	constexpr int kShadowMap = 1024;

	//シャドウマップの描画範囲
	constexpr float kShadowMapRange = 7500.0f;

	//ライトの方向
	constexpr float kLight = 0.5f;
}



SceneStage::SceneStage(const int StageNumber):m_barHandle(-1), m_hpBarHandle(-1), m_mpBarHandle(-1), m_operationHandle(-1),
m_specialMoveOperationHandle(-1), m_warpPointOperationHandle(-1), m_allOperatioHandle(-1), m_shadowMapHandle(-1), m_stageKinds(0),m_bossHandle(-1),
m_isPlayerDie(false),m_isWarpPoint(false),m_isSpecialMoveAvailable(false),m_isGameClear(false), m_cameraAngle(VGet(0.0f, 0.0f, 0.0f))

{
	m_stageKinds = StageNumber;
}

SceneStage::~SceneStage()
{	
	m_pPlayer.reset();
	m_pPlayer = nullptr;

	m_pCamera.reset();
	m_pCamera = nullptr;

	m_pStage.reset();
	m_pStage = nullptr;

	//ステージ1の場合
	if (m_stageKinds == Stage1)
	{
		m_pEnemyManager.reset();
		m_pEnemyManager = nullptr;
	}
	else if (m_stageKinds == Stage2)
	{
		m_pBoss.reset();
		m_pBoss = nullptr;
	}

	m_pPlayerStatus.reset();
	m_pPlayerStatus = nullptr;

	//UI画像の削除
	DeleteGraph(m_barHandle);
	DeleteGraph(m_hpBarHandle);
	DeleteGraph(m_mpBarHandle);
	DeleteGraph(m_operationHandle);
	DeleteGraph(m_specialMoveOperationHandle);
	DeleteGraph(m_warpPointOperationHandle);
	DeleteGraph(m_allOperatioHandle);
	

	// シャドウマップの削除
	DeleteShadowMap(m_shadowMapHandle);
}

void SceneStage::Init()
{
	if (m_stageKinds == Stage1)
	{
		m_pStage = std::make_shared<Stage>(Stage1);
		m_pEnemyManager = std::make_shared<EnemyManager>();
		m_pEnemyManager->Init();
	}

	if (m_stageKinds == Stage2)
	{
		m_pStage = std::make_shared<Stage>(Stage2);
		m_pBoss = std::make_shared<Boss>();

		m_bossHandle = MV1LoadModel("data/model/enemy/Boss.mv1");
		m_pBoss->Init(m_bossHandle);
	}

	m_pCamera = std::make_shared<Camera>();
	m_pCamera->Init();
	
	m_pPlayer = std::make_shared<Player>();
	m_pPlayer->Init();
	
	m_pPlayerStatus = std::make_shared<PlayerStatus>();
	m_pPlayerStatus->Init();


	//フェード値の初期設定
	m_fadeAlpha = kFadeValue;

	//初期化
	m_isPlayerDie = false;
	m_isGameClear = false;

	//UI関連画像のロード
	m_barHandle = LoadGraph("data/Ui/Bar.png");
	m_hpBarHandle = LoadGraph("data/UI/HpBar.png");
	m_mpBarHandle = LoadGraph("data/UI/MpBar.png");
	m_operationHandle = LoadGraph("data/UI/operation.png");
	m_specialMoveOperationHandle = LoadGraph("data/UI/specialmoveoperation.png");
	m_warpPointOperationHandle = LoadGraph("data/UI/warpOperation.png");
	m_allOperatioHandle = LoadGraph("data/UI/alloperation.png");

	//シャドウマップの作成
	m_shadowMapHandle = MakeShadowMap(kShadowMap, kShadowMap);

	// シャドウマップが想定するライトの方向もセット
	SetShadowMapLightDirection(m_shadowMapHandle, VGet(kLight, -kLight, kLight));

	// シャドウマップに描画する範囲を設定
	SetShadowMapDrawArea(m_shadowMapHandle, VGet(-kShadowMapRange, -1.0f, -kShadowMapRange), VGet(kShadowMapRange, kShadowMapRange, kShadowMapRange));
	 
	
	//ゲームプレイ中のBGMを再生する
	m_pSoundManager->GamePlayBGM();
}

std::shared_ptr<SceneBase> SceneStage::Update(const Pad& pad)
{

	if (m_stageKinds == Stage1)
	{
		Stage1Update(pad);
	}
	if (m_stageKinds == Stage2)
	{
		Stage2Update(pad);
	}

	//フェードイン、フェードアウトを行う
	Fade();

	if (m_isPlayerDie && m_fadeAlpha >= kFadeValue)
	{
		//ステージごとの終了時の処理
		if (m_stageKinds == Stage1)
		{
			m_pPlayerStatus->StatOutput(m_pEnemyManager->GetStatusPoint());
		}
		

		//プレイヤーのHPが0になったのでリザルト画面に遷移する
		return std::make_shared<SceneResult>();
	}
	if (m_isGameClear && m_fadeAlpha >= kFadeValue)
	{
		//ステージごとの終了時の処理
		if (m_stageKinds == Stage1)
		{
			m_pPlayerStatus->StatOutput(m_pEnemyManager->GetStatusPoint());
		}
		if (m_stageKinds == Stage2)
		{
			m_pPlayerStatus->StatOutput(m_pBoss->GetStatusPoint());
		}
		
		return std::make_shared<SceneClear>();
	}

	return shared_from_this();

}

void SceneStage::Draw()
{
	// シャドウマップへの描画の準備
	ShadowMap_DrawSetup(m_shadowMapHandle);

	// 描画に使用するシャドウマップを設定
	SetUseShadowMap(0, m_shadowMapHandle);

	//ステージの描画
	m_pStage->Draw();

	//プレイヤーの描画
	m_pPlayer->Draw();

	//ステージの種類によって描画するものを変更する
	if (m_stageKinds == Stage1)
	{
		//エネミーの描画
		m_pEnemyManager->Draw();
	}
	if (m_stageKinds == Stage2)
	{
		m_pBoss->Draw();
	}
	
	//UIの描画
	DrawGraph(0, 0, m_barHandle, true);
	DrawGraph(0, 0, m_hpBarHandle, true);
	DrawGraph(0, 0, m_mpBarHandle, true);


	OperationUI();

	if (!m_isPlayerDie)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha); //半透明で表示
		DrawBoxAA(0, 0, Game::kScreenWindidth, Game::kScreenHeight, GetColor(0, 0, 0), true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); //不透明に戻しておく
	}
	//ゲームオーバー時のフェードの描画
	if (m_isPlayerDie)
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

#ifdef _DEBUG
	DrawFormatString(0, 0, GetColor(0,0,0), "SceneStage1");

#endif
}

void SceneStage::End()
{
}

//ステージ1での処理
void SceneStage::Stage1Update(const Pad& pad)
{
	//プレイヤーが死んだかどうかを受け取る
	m_isPlayerDie = m_pPlayer->GetIsPlayerDie();

	//ゲームをクリアしたかどうかを受け取る
	m_isGameClear = m_pEnemyManager->GetGameClear();

	//プレイヤーがワープできる地点にいるかを受け取る
	m_isWarpPoint = m_pPlayer->GetIsWarpPoint();

	//プレイヤーが必殺技を撃てる状態かを受け取る
	m_isSpecialMoveAvailable = m_pPlayer->GetIsSpecialMoveAvailable();

	m_pPlayer->Update(*m_pStage, pad, *m_pCamera);

	m_pEnemyManager->Update(*m_pStage, *m_pPlayer);

	m_pCamera->Update(m_pPlayer->GetPos());
}

void SceneStage::Stage2Update(const Pad& pad)
{
	//プレイヤーが死んだかどうかを受け取る
	m_isPlayerDie = m_pPlayer->GetIsPlayerDie();

	//ゲームをクリアしたかどうかを受け取る
	m_isGameClear = m_pBoss->GetGameClear();

	//プレイヤーの動き
	m_pPlayer->Update(*m_pStage, pad, *m_pCamera);

	//ボスの動き
	m_pBoss->Update(*m_pStage, *m_pPlayer, m_pPlayer->GetPos());

	//カメラの動き
	m_pCamera->Update(m_pPlayer->GetPos());
}

void SceneStage::HpBarDraw(int hp, int mp)
{
	//UIの描画
	DrawGraph(0, 0, m_barHandle, true);
	DrawGraph(0, 0, m_hpBarHandle, true);
	DrawGraph(0, 0, m_mpBarHandle, true);
}

void SceneStage::OperationUI()
{
	if (!m_isWarpPoint && !m_isSpecialMoveAvailable)
	{
		DrawGraph(0, 0, m_operationHandle, true);
	}
	if(m_isWarpPoint&& !m_isSpecialMoveAvailable)
	{
		DrawGraph(0, 0, m_warpPointOperationHandle, true);
	}
	if (m_isSpecialMoveAvailable && m_isWarpPoint)
	{
		DrawGraph(0, 0, m_allOperatioHandle, true);
	}
	
	if(m_isSpecialMoveAvailable&& !m_isWarpPoint)
	{
		DrawGraph(0, 0, m_specialMoveOperationHandle, true);
	}
}

void SceneStage::Fade()
{
	//フェードイン、フェードアウト
	if (!m_isPlayerDie&&!m_isGameClear)
	{
		m_fadeAlpha -= kFadeUpDown;
		if (m_fadeAlpha <= 0)
		{
			m_fadeAlpha = 0;
		}
	}
	if (m_isGameClear)
	{
		m_fadeAlpha += kFadeUpDown;
		if (m_fadeAlpha >= kFadeValue)
		{
			m_fadeAlpha = kFadeValue;
		}
	}
	if (m_isPlayerDie)
	{
		m_fadeAlpha += kFadeUpDown;
		if (m_fadeAlpha >= kFadeValue)
		{
			m_fadeAlpha = kFadeValue;
		}
	}
}
