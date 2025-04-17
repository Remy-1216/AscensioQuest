#include "SceneTutorial.h"
#include "SceneTitle.h"
#include "SceneSelect.h"
#include "Player.h"
#include "TutorialEnemy.h"
#include "Stage.h"
#include "Camera.h"
#include "UIBar.h"
#include "Font.h"
#include "Game.h"
#include "Pad.h"

namespace
{
	//フェードイン、フェードアウトの数値
	constexpr int kFadeValue = 256;

	//フェード値の増減
	constexpr int kFadeUpDown = 8;

	//カウントを秒に変換する
	constexpr int kCountToSecond = 60;

	//チュートリアル1をクリアするための歩行時間と走行時間
	constexpr int kMaxTime =3;

	//歩行時間の上限
	constexpr int kMaxWalkTime = 180;

	//走行時間の上限
	constexpr int kMaxRunTime = 180;

	//チュートリアル2をクリアするための攻撃回数
	constexpr int kMaxAttackCount = 5;

	//チュートリアル2をクリアするための魔法攻撃回数
	constexpr int kMaxMagicAttackCount = 3;

	//チュートリアル3をクリアするための必殺技回数
	constexpr int kMaxSpecialMoveCount = 1;

	//チュートリアル4をクリアするためのガード成功回数
	constexpr int kMaxGuardSuccessCount = 3;

	//操作説明のカウント
	constexpr int kFirstManual = 0;
	constexpr int kSecondManual = 1;
	constexpr int kThirdManual = 2;
	constexpr int kFourthManual = 3;
	constexpr int kFifthManual = 4;
	constexpr int kSixthManual = 5;

	//シャドウマップを制作するときに使用する値
	constexpr int kShadowMap = 2048;
	//シャドウマップの描画範囲
	constexpr float kShadowMapRange = 7500.0f;

	//ライトの方向
	constexpr float kLight = 0.5f;

	//テキストを描画する位置
	constexpr float kTextPosX = 450.0f;
	constexpr float kTextPosY = 720.0f;

	//テキストの背景を描画する位置
	constexpr float kTextBackPosX = 400.0f;
	constexpr float kTextBackPosY =650.0f;

	//チュートリアル中に描画する画像の位置
	constexpr float kTutorialPosY = 200.0f;

	//チュートリアル中に表示するテキストの位置
	constexpr float kTutorialTextPosX = 320.0f;
	constexpr float kTutorialTextPosY = 325.0f;
	constexpr float kFirstTutorialTextPosY = 400.0f;
	constexpr float kSecondTutorialTextPosY = 460.0f;
	constexpr float kThirdTutorialTextPosY = 395.0f;

	//動画の左上の座標
	constexpr int kUpLeftX =450;
	constexpr int kUpLeftY = 0;

	//動画の右下の座標
	constexpr int kLowRightX =1450;
	constexpr int kLowRightY = 700;

	//操作説明中に表示するUIの位置
	constexpr float kManualUIPosX = 1500.0f;
	constexpr float kManualUIPosY =700.0f;

	//敵の座標位置
	constexpr float kTutorialEnemyPosX = 5020.0f;
	constexpr float kTutorialEnemyPosY = 20.0f;
	constexpr float kTutorialEnemyPosZ =-3470.0f;

	//スタートボタンを押した回数
	constexpr int kFirstPush = 1;
	constexpr int kSecondPush = 2;
	constexpr int kThirdPush = 3;
	constexpr int kFourPush = 4;
	constexpr int kFifthPush = 5;

	//動画が止まっているか
	constexpr int kStopMovie = 0;

}

SceneTutorial::SceneTutorial() :m_textBackHandle(-1), m_firstManualHandle(-1), m_secondManualHandle(-1), m_firstTutorialHandle(-1),  m_tutorialKinds(kTurorialStart), m_walkTimes(0), m_runTimes(0), m_attackCount(0), m_magicAttackCount(0), m_specialMoveCount(0),
m_guardCount(0), m_manualCount(0), m_startPush(0), m_isFirstTutorial(false), m_isSecondTutorial(false), m_isThirdTutorial(false), m_isFourtutorial(false), m_isFifthManual(false),m_isSixTutorial(false), m_isManual(true), m_isTutorialClear(false), m_isClear(false)
{
}

SceneTutorial::~SceneTutorial()
{
	m_pPlayer.reset();
	m_pPlayer = nullptr;

	m_pCamera.reset();
	m_pCamera = nullptr;

	m_pStage.reset();
	m_pStage = nullptr;

	//ロードした画像を削除する
	DeleteGraph(m_textBackHandle);
	DeleteGraph(m_firstManualHandle);
	DeleteGraph(m_firstTutorialHandle);
	DeleteGraph(m_secondManualHandle);
	DeleteGraph(m_thirdManualHandle);
	DeleteGraph(m_fourManualHandle);
	DeleteGraph(m_fifthManualHandle);
	DeleteGraph(m_sixManualHandle);
	DeleteGraph(m_firstTutorialHandle);
	DeleteGraph(m_secondTutorialHandle);
	DeleteGraph(m_thirdTutorialHandle);
	DeleteGraph(m_fourTutorialHandle);
	DeleteGraph(m_fifthTutorialHandle);
	DeleteGraph( m_attackVideoHandle);
	DeleteGraph(m_magicAttackVideoideoHandle);
	DeleteGraph(m_warpEffectHandle);
	DeleteGraph(m_warpMovementHandle);
	DeleteGraph(m_consumeMPVideoHandle);
	DeleteGraph(m_mpRecoveryVideoHandle);
	DeleteGraph(m_exGaugeRiseVideoHandle);
	DeleteGraph(m_specialMoveVideoHandle);
	DeleteGraph(m_exGaugeHandle);
	DeleteGraph(m_statusScreenHandle);
	DeleteGraph(m_guardVideoHandle);
	DeleteGraph(m_guardSuccessVideoHandle);
	DeleteGraph(m_statusUpVideoHandle);
	

}

void SceneTutorial::Init()
{
	//初期化
	m_playVideo = 0;

	//テキストの背景をロードする
	m_textBackHandle = LoadGraph("data/BG/textBack.png");

	//操作説明中に表示するUIをロードする
	m_manualUIHandle = LoadGraph("data/UI/manualUI.png");

	//敵のモデルをロードする
	m_tutorialEnemyHandle = MV1LoadModel("data/model/enemy/Enemy_Black.mv1");

	//操作説明1の画像をロードする
	m_firstManualHandle = LoadGraph("data/BG/firsttutorial.png");

	//チュートリアル1の画像をロードする
	m_firstTutorialHandle = LoadGraph("data/UI/firsttutorial.png");

	//操作説明2の画像をロードする
	m_secondManualHandle = LoadGraph("data/BG/secondtutorial.png");

	//チュートリアル2の画像をロードする
	m_secondTutorialHandle = LoadGraph("data/UI/secondtutorial.png");

	//操作説明3の画像をロードする
	m_thirdManualHandle = LoadGraph("data/BG/thirdtutorial.png");

	//チュートリアル3の画像をロードする
	m_thirdTutorialHandle = LoadGraph("data/UI/thirdTutorial.png");

	//操作説明4の画像をロードする
	m_fourManualHandle = LoadGraph("data/BG/fourtutorial.png");

	//チュートリアル4の画像をロードする
	m_fourTutorialHandle = LoadGraph("data/UI/fourtutorial.png");

	//操作説明5の画像をロードする
	m_fifthManualHandle = LoadGraph("data/BG/fifthtutorial.png");

	//操作説明6の画像をロードする
	m_sixManualHandle = LoadGraph("data/BG/sixtutorial.png");

	//必殺技ゲージの画像をロードする
	m_exGaugeHandle = LoadGraph("data/tutorial/exgauge.png");

	//ステータス画面を描画する
	m_statusScreenHandle = LoadGraph("data/tutorial/statusscreen.png");

	//ワープポイントエフェクトの動画をロードする
	m_warpEffectHandle = LoadGraph("data/mp4/warpEffect.mp4");

	//ワープ移動の動画をロードする
	m_warpMovementHandle = LoadGraph("data/mp4/warp.mp4");

	//プレイヤーが攻撃を行っているときの動画をロードする
	 m_attackVideoHandle = LoadGraph("data/mp4/attack.mp4");

	//プレイヤーが魔法攻撃を行っているときの動画をロードする
	m_magicAttackVideoideoHandle = LoadGraph("data/mp4/magicattack.mp4");

	//MPが消費されている時の動画をロードする
	m_consumeMPVideoHandle = LoadGraph("data/mp4/consumemp.mp4");

	//MPが回復している時の動画をロードする
	m_mpRecoveryVideoHandle = LoadGraph("data/mp4/mprecovery.mp4");

	//EXゲージが上昇しているときの動画をロードする
	m_exGaugeRiseVideoHandle = LoadGraph("data/mp4/upExgauge.mp4");

	//必殺技を行っているときの動画をロードする
	m_specialMoveVideoHandle = LoadGraph("data/mp4/specialmove.mp4");

	//ガード中の動画をロードする
	m_guardVideoHandle = LoadGraph("data/mp4/guard.mp4");

	//ガード成功時の動画をロードする
	m_guardSuccessVideoHandle = LoadGraph("data/mp4/guardsuccess.mp4");

	//ステータス上げているときの動画をロードする
	m_statusUpVideoHandle = LoadGraph("data/mp4/statusup.mp4");

	//プレイヤーの初期化
	m_pPlayer = std::make_shared<Player>();
	m_pPlayer->Init();

	//敵の初期化
	m_pTutorialEnemy = std::make_shared<TutorialEnemy>();
	m_pTutorialEnemy->Init(m_tutorialEnemyHandle,VGet(kTutorialEnemyPosX, kTutorialEnemyPosY, kTutorialEnemyPosZ));

	//ステージの初期化
	m_pStage = std::make_shared<Stage>(Tutorial);
	m_pStage->Init();

	//カメラの初期化
	m_pCamera = std::make_shared<Camera>();
	m_pCamera->Init();

	//UIバーの初期化
	m_pUIBar = std::make_shared<UIBar>();
	m_pUIBar->Init();

	//フェード値の初期設定
	m_fadeAlpha = kFadeValue;

	//チュートリアルの種類を初期化
	m_tutorialKinds = kFirstTutorial;

	//シャドウマップの作成
	m_shadowMapHandle = MakeShadowMap(kShadowMap, kShadowMap);

	// シャドウマップが想定するライトの方向もセット
	SetShadowMapLightDirection(m_shadowMapHandle, VGet(kLight, -kLight, kLight));

	// シャドウマップに描画する範囲を設定
	SetShadowMapDrawArea(m_shadowMapHandle, VGet(-kShadowMapRange, -1.0f, -kShadowMapRange), VGet(kShadowMapRange, kShadowMapRange, kShadowMapRange));
}

std::shared_ptr<SceneBase> SceneTutorial::Update(const Pad& pad)
{
	//フェード関係の処理
	Fade();

	//カメラの処理
	m_pCamera->Update(m_pPlayer->GetPos());

	if (!m_isManual)
	{		
		//プレイヤーの処理
		m_pPlayer->Update(*m_pStage, pad, *m_pCamera);

		//敵の処理
		m_pTutorialEnemy->Update(*m_pStage,*m_pPlayer, m_tutorialKinds);

	}
	else
	{
		//操作説明中の処理
		ManualUpdate(pad);
	}

	//チュートリアルの処理
	TutorialUpdate();

	if (m_isClear)
	{
		return std::make_shared<SceneSelect>();
	}

#ifdef _DEBUG

	

	if (pad.IsTrigger("START"))
	{
		m_startPush++;
	}

	if(m_startPush == kFirstPush)
	{
		m_isFirstTutorial = true;
	}
	else if (m_startPush == kSecondPush)
	{
		m_isSecondTutorial = true;
	}
	else if (m_startPush == kThirdPush)
	{
		m_isThirdTutorial = true;
	}
	else if (m_startPush == kFourPush)
	{
		m_isFourtutorial = true;
	}
	else if (m_startPush == kFifthPush)
	{
		m_isFirstTutorial = true;
	}

#endif

	return shared_from_this();
}

void SceneTutorial::Draw()
{
	// シャドウマップへの描画の準備
	ShadowMap_DrawSetup(m_shadowMapHandle);

	m_pStage->DrawShadowModel();

	m_pTutorialEnemy->DrawShadowModel();

	m_pPlayer->DrawShadowModel();

	//シャドウマップへの描画を終了
	ShadowMap_DrawEnd();

	// 描画に使用するシャドウマップを設定
	SetUseShadowMap(0, m_shadowMapHandle);

	m_pStage->Draw();

	m_pTutorialEnemy->Draw();

	m_pPlayer->Draw();

	//HPバーなどを表示する
	m_pUIBar->DrawPlayerGaugeBar(*m_pPlayer);

	//チュートリアル中に描画する画像
	DrawTutorial();

	//チュートリアル中テキストを表示する
	DrawTutorialText();

	//操作説明中は描画する
	if (m_isManual)
	{
		DrawGraph(0, 0, m_textBackHandle, true);
	}

	//操作説明中に描画する画像
	DrawManual();

	//操作説明中テキストを表示する
	DrawManualText();

	//フェードの描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha); //半透明で表示
	DrawBox(0, 0, Game::kScreenWindidth, Game::kScreenHeight, GetColor(0, 0, 0), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); //不透明に戻しておく
#ifdef _DEBUG
	if (!m_isFirstTutorial)
	{
		DrawFormatString(0,600, GetColor(0, 0, 0), "チュートリアル1/5");
	}
	if (m_isFirstTutorial&&!m_isSecondTutorial)
	{
		DrawFormatString(0, 600, GetColor(0, 0, 0), "チュートリアル2/5");
	}
	if (m_isSecondTutorial && !m_isThirdTutorial)
	{
		DrawFormatString(0, 600, GetColor(0, 0, 0), "チュートリアル3/5");
	}
#endif

}

void SceneTutorial::End()
{
}

void SceneTutorial::TutorialUpdate()
{
	if (!m_isManual)
	{
		//第一チュートリアルの処理
		if (!m_isFirstTutorial)
		{
			FirstTutorial();
		}
		//第二チュートリアルの処理
		else if (!m_isSecondTutorial)
		{
			SecondTutorial();
		}
		//第三チュートリアルの処理
		else if (!m_isThirdTutorial)
		{
			ThirdTutorial();
		}
		//第四チュートリアルの処理
		else if (!m_isFourtutorial)
		{
			FourTutorial();
		}
	}
}

//第一のチュートリアル
void SceneTutorial::FirstTutorial()
{
	//プレイヤーが歩いている時間をカウント
	if (m_pPlayer->GetStateKinds() == kWalk)
	{
		m_walkTimes++;

		if (m_walkTimes >= kMaxWalkTime)
		{
			m_walkTimes = kMaxWalkTime;
		}
	}

	//プレイヤーが走っている時間をカウント
	if (m_pPlayer->GetStateKinds() == kRun)
	{
		m_runTimes++;

		if (m_runTimes >= kMaxRunTime)
		{
			m_runTimes = kMaxRunTime;
		}
	}

	//必須回数を超えたら次のチュートリアルへ
	if (m_runTimes/ kCountToSecond >= kMaxTime && m_walkTimes/ kCountToSecond >= kMaxTime)
	{
		m_isTutorialClear = true;
		m_isFirstTutorial = true;
	}
}

//第二のチュートリアル
void SceneTutorial::SecondTutorial()
{
	//攻撃を受けた回数を受け取る
	m_attackCount = m_pTutorialEnemy->GetAttackHitTimes();
	
	//魔法攻撃を受けた回数を受け取る
	m_magicAttackCount = m_pTutorialEnemy->GetMagicAttackHitTimes();


	if (m_attackCount >= kMaxAttackCount)
	{
		m_attackCount = kMaxAttackCount;
	}
	if (m_magicAttackCount >= kMaxMagicAttackCount)
	{
		m_magicAttackCount = kMaxMagicAttackCount;
	}
	

	//必須回数を超えたら次のチュートリアルへ
	if (m_attackCount >= kMaxAttackCount && m_magicAttackCount >= kMaxMagicAttackCount)
	{
		m_isTutorialClear = true;
		m_isSecondTutorial = true;
	}

}

//第三のチュートリアル
void SceneTutorial::ThirdTutorial()
{
	m_specialMoveCount = m_pTutorialEnemy->GetSpecialMoveHitTimes();

	if (m_specialMoveCount >= kMaxSpecialMoveCount && !m_pPlayer->GetIsSpecialMove())
	{
		m_isTutorialClear = true;
		m_isThirdTutorial = true;
	}
}

//第四のチュートリアル
void SceneTutorial::FourTutorial()
{
	m_guardCount  = m_pPlayer->GetGuardSuccess();

	if (m_guardCount >= kMaxGuardSuccessCount)
	{
		m_isTutorialClear = true;
		m_isFourtutorial = true;
	}

#ifdef _DEBUG

#endif
}

//操作説明中の処理
void SceneTutorial::ManualUpdate(const Pad& pad)
{
	//テキストを進める処理
	if (m_isManual)
	{
		if (pad.IsTrigger("A"))
		{
			m_manualCount++;
			m_playVideo = 0;
		}

		if (pad.IsTrigger("B"))
		{
			m_manualCount--;
			m_playVideo = 0;
		}

		if (m_manualCount <= 0)
		{
			m_manualCount = 0;
		}

		if (m_isFourtutorial && !m_isFifthManual && m_manualCount == kSixthManual)
		{
			m_manualCount = 0;
			m_isFifthManual = true;
		}
		else if (m_isFifthManual && !m_isSixTutorial && m_manualCount == kSixthManual)
		{
			m_manualCount = 0;
			m_isSixTutorial = true;
			m_isClear = true;
		}

		else if (m_manualCount == kSixthManual)
		{
			m_manualCount = 0;
			m_isManual = false;
		}
	}
}

//操作説明のテキストを描画
void SceneTutorial::DrawManualText()
{
	if (m_isManual)
	{
		//第一チュートリアルの操作説明
		if (!m_isFirstTutorial)
		{
			//移動の説明
			if (m_manualCount == kSecondManual)
			{
				DrawFormatStringFToHandle(kTextPosX, kTextPosY, GetColor(0, 0, 0), Font::m_fontHandle[static_cast<int>(Font::FontId::kSize55_4)], "左スティックを倒すと移動ができます\n");
			}
			//ダッシュの説明
			if (m_manualCount == kThirdManual)
			{
				DrawFormatStringFToHandle(kTextPosX, kTextPosY, GetColor(0, 0, 0), Font::m_fontHandle[static_cast<int>(Font::FontId::kSize55_4)], "RBボタン押しながら移動するとで\n走ることができます\n");
			}
			//カメラの説明
			if (m_manualCount == kFourthManual)
			{
				DrawFormatStringFToHandle(kTextPosX, kTextPosY, GetColor(0, 0, 0), Font::m_fontHandle[static_cast<int>(Font::FontId::kSize55_4)], "右スティックを倒すと\nカメラ移動ができます\n");
			}
			//最後の一言
			if (m_manualCount == kFifthManual)
			{
				DrawFormatStringFToHandle(kTextPosX, kTextPosY, GetColor(0, 0, 0), Font::m_fontHandle[static_cast<int>(Font::FontId::kSize55_4)], "それでは試してみましょう！\n");
			}
		}
		//第二チュートリアルの操作説明
		else if (!m_isSecondTutorial)
		{
			//通常攻撃の説明
			if (m_manualCount == kSecondManual)
			{
				DrawFormatStringFToHandle(kTextPosX, kTextPosY, GetColor(0, 0, 0), Font::m_fontHandle[static_cast<int>(Font::FontId::kSize55_4)], "Aボタンを押すと、攻撃ができます\n");
			}
			//魔法攻撃の説明
			if (m_manualCount == kThirdManual)
			{
				DrawFormatStringFToHandle(kTextPosX, kTextPosY, GetColor(0, 0, 0), Font::m_fontHandle[static_cast<int>(Font::FontId::kSize55_4)], "Bボタンを押すと、魔法攻撃ができます\n");
			}
			//魔法攻撃についての注意
			if (m_manualCount == kFourthManual)
			{
				DrawFormatStringFToHandle(kTextPosX, kTextPosY, GetColor(0, 0, 0), Font::m_fontHandle[static_cast<int>(Font::FontId::kSize55_4)], "MPを消費して攻撃を行うので\n気を付けてください\n");
			}
			//MPの回復についての説明　
			if (m_manualCount == kFifthManual)
			{
				DrawFormatStringFToHandle(kTextPosX, kTextPosY, GetColor(0, 0, 0), Font::m_fontHandle[static_cast<int>(Font::FontId::kSize55_4)], "MPは少しずつ回復します。");
			}
		}
		//第三チュートリアルの操作説明
		else if (!m_isThirdTutorial)
		{
			//必殺技のゲージについて
			if (m_manualCount == kSecondManual)
			{
				DrawFormatStringFToHandle(kTextPosX, kTextPosY, GetColor(0, 0, 0), Font::m_fontHandle[static_cast<int>(Font::FontId::kSize55_4)], "画面左下にEXゲージがあります");
			}
			//必殺技の条件に付いて
			if (m_manualCount == kThirdManual)
			{
				DrawFormatStringFToHandle(kTextPosX, kTextPosY, GetColor(0, 0, 0), Font::m_fontHandle[static_cast<int>(Font::FontId::kSize55_4)], "敵を攻撃当てるたびに、\nEXゲージがたまっていきます\n");
			}
			//必殺技について
			if (m_manualCount == kFourthManual)
			{
				DrawFormatStringFToHandle(kTextPosX, kTextPosY, GetColor(0, 0, 0), Font::m_fontHandle[static_cast<int>(Font::FontId::kSize55_4)], "ゲージがMAXになると必殺技ができます\n必殺技をするとゲージは0になります\n");
			}
			//必殺技について
			if (m_manualCount == kFifthManual)
			{
				DrawFormatStringFToHandle(kTextPosX, kTextPosY, GetColor(0, 0, 0), Font::m_fontHandle[static_cast<int>(Font::FontId::kSize55_4)], "大ダメージを与えることができます！！");
			}
		}
		//第四チュートリアルの操作説明
		else if (!m_isFourtutorial)
		{
			//ガードについて
			if (m_manualCount == kSecondManual)
			{
				DrawFormatStringFToHandle(kTextPosX, kTextPosY, GetColor(0, 0, 0), Font::m_fontHandle[static_cast<int>(Font::FontId::kSize55_4)], "LBボタンを長押ししている間\nガードが行えます");
			}
			//ガードに付いて
			if (m_manualCount == kThirdManual)
			{
				DrawFormatStringFToHandle(kTextPosX, kTextPosY, GetColor(0, 0, 0), Font::m_fontHandle[static_cast<int>(Font::FontId::kSize55_4)], "LBボタンを離すと\nガードは解除されます"); 
			}
			//ガードについて
			if (m_manualCount == kFourthManual)
			{
				DrawFormatStringFToHandle(kTextPosX, kTextPosY, GetColor(0, 0, 0), Font::m_fontHandle[static_cast<int>(Font::FontId::kSize55_4)], "ガード中は敵の攻撃を半減できます");
			}
			//ガードについて
			if (m_manualCount == kFifthManual)
			{
				DrawFormatStringFToHandle(kTextPosX, kTextPosY, GetColor(0, 0, 0), Font::m_fontHandle[static_cast<int>(Font::FontId::kSize55_4)], "敵からの攻撃から身を守りましょう！！");
			}
		}
		//第五チュートリアルの操作説明
		else if (!m_isFifthManual)
		{
			//マップ移動について
			if (m_manualCount == kSecondManual)
			{
				DrawFormatStringFToHandle(kTextPosX, kTextPosY, GetColor(0, 0, 0), Font::m_fontHandle[static_cast<int>(Font::FontId::kSize55_4)], "このようなエフェクトがある場所に行くと\nワープ移動ができます"); 
			}
			//マップ移動について
			if (m_manualCount == kThirdManual)
			{
				DrawFormatStringFToHandle(kTextPosX, kTextPosY, GetColor(0, 0, 0), Font::m_fontHandle[static_cast<int>(Font::FontId::kSize55_4)],"ワープ移動は、敵のいるステージに\n移動することができます\n");
			}
			//マップ移動について
			if (m_manualCount == kFourthManual)
			{
				DrawFormatStringFToHandle(kTextPosX, kTextPosY, GetColor(0, 0, 0), Font::m_fontHandle[static_cast<int>(Font::FontId::kSize55_4)], "Xボタンを押すとワープできます\n");
			}
			//マップ移動について
			if (m_manualCount == kFifthManual)
			{
				DrawFormatStringFToHandle(kTextPosX, kTextPosY, GetColor(0, 0, 0), Font::m_fontHandle[static_cast<int>(Font::FontId::kSize55_4)], "ワープ先には敵がいます。\n自分の好きなタイミングで移動しましょう\n");
			}
		}
		//第六チュートリアルの説明
		else if (!m_isSixTutorial)
		{
			//ステータスポイントについて
			if (m_manualCount == kSecondManual)
			{
				DrawFormatStringFToHandle(kTextPosX, kTextPosY, GetColor(0, 0, 0), Font::m_fontHandle[static_cast<int>(Font::FontId::kSize55_4)], "ステータスを強化することができます\n敵を倒すことでポイントを獲得できます");
			}
			//ステータスポイントについて
			if (m_manualCount == kThirdManual)
			{
				DrawFormatStringFToHandle(kTextPosX, kTextPosY, GetColor(0, 0, 0), Font::m_fontHandle[static_cast<int>(Font::FontId::kSize55_4)], "ポイントを消費することで、\n様々なステータスをあげることができます");
			}
			//ステータスについて　
			if (m_manualCount == kFourthManual)
			{
				DrawFormatStringFToHandle(kTextPosX, kTextPosY, GetColor(0, 0, 0), Font::m_fontHandle[static_cast<int>(Font::FontId::kSize55_4)], "ステータスを上げて、\n自身を強化しましょう！\n");
			}
			//行った後について
			if (m_manualCount == kFifthManual)
			{
				DrawFormatStringFToHandle(kTextPosX, kTextPosY, GetColor(0, 0, 0), Font::m_fontHandle[static_cast<int>(Font::FontId::kSize55_4)], "これで、チュートリアルを終わります。");
			}
		}
	}
	
}

//操作説明中に動画や画像を描画する
void SceneTutorial::DrawManual()
{
	if (m_isManual)
	{
		//操作説明中に描画するUI
		DrawGraph(kManualUIPosX, kManualUIPosY, m_manualUIHandle, true);

		if (!m_isFirstTutorial)
		{
			if (m_manualCount == kFirstManual)
			{
				DrawGraph(0, 0, m_firstManualHandle, true);
			}
		}
		else if(!m_isSecondTutorial)
		{ 
			if (m_manualCount == kFirstManual)
			{
				DrawGraph(0, 0, m_secondManualHandle, true);
			}
			if (m_manualCount == kSecondManual)
			{
				DrawExtendGraph(kUpLeftX, kUpLeftY, kLowRightX, kLowRightY,  m_attackVideoHandle, FALSE);
			}
			if (m_manualCount == kThirdManual)
			{
				DrawExtendGraph(kUpLeftX, kUpLeftY, kLowRightX, kLowRightY, m_magicAttackVideoideoHandle, FALSE);
			}
			if (m_manualCount == kFourthManual)
			{
				DrawExtendGraph(kUpLeftX, kUpLeftY, kLowRightX, kLowRightY, m_consumeMPVideoHandle, FALSE);
			}
			if (m_manualCount == kFifthManual)
			{
				DrawExtendGraph(kUpLeftX, kUpLeftY, kLowRightX, kLowRightY, m_mpRecoveryVideoHandle, FALSE);
			}
		}
		else if (!m_isThirdTutorial)
		{
			if (m_manualCount == kFirstManual)
			{
				DrawGraph(0, 0, m_thirdManualHandle, true);
			}
			if (m_manualCount == kSecondManual)
			{
				DrawExtendGraph(kUpLeftX, kUpLeftY, kLowRightX, kLowRightY, m_exGaugeHandle, FALSE);
			}
			if (m_manualCount == kThirdManual)
			{
				DrawExtendGraph(kUpLeftX, kUpLeftY, kLowRightX, kLowRightY, m_exGaugeRiseVideoHandle, FALSE);
			}
			if (m_manualCount == kFourthManual || m_manualCount == kFifthManual)
			{
				DrawExtendGraph(kUpLeftX, kUpLeftY, kLowRightX, kLowRightY, m_specialMoveVideoHandle, FALSE);
			}
		}
		else if (!m_isFourtutorial)
		{
			if (m_manualCount == kFirstManual)
			{
				DrawGraph(0, 0, m_fourManualHandle, true);
			}
			if (m_manualCount == kSecondManual || m_manualCount == kThirdManual)
			{
				DrawExtendGraph(kUpLeftX, kUpLeftY, kLowRightX, kLowRightY, m_guardVideoHandle, FALSE);
			}
			if ( m_manualCount == kFourthManual || m_manualCount == kFifthManual)
			{
				DrawExtendGraph(kUpLeftX, kUpLeftY, kLowRightX, kLowRightY, m_guardSuccessVideoHandle, FALSE);
			}
		}
		else if (!m_isFifthManual)
		{

			if (m_manualCount == kFirstManual)
			{
				DrawGraph(0, 0, m_fifthManualHandle, true);
			}
			if (m_manualCount == kSecondManual || m_manualCount == kThirdManual)
			{
				DrawExtendGraph(kUpLeftX, kUpLeftY, kLowRightX, kLowRightY, m_warpEffectHandle, FALSE);
			}
			if (m_manualCount == kFourthManual || m_manualCount == kFifthManual)
			{
				DrawExtendGraph(kUpLeftX, kUpLeftY, kLowRightX, kLowRightY, m_warpMovementHandle, FALSE);
			}
		}
		else if (!m_isSixTutorial)
		{
			if (m_manualCount == kFirstManual)
			{
				DrawGraph(0, 0, m_sixManualHandle, true);
			}
			if (m_manualCount == kSecondManual)
			{
				DrawExtendGraph(kUpLeftX, kUpLeftY, kLowRightX, kLowRightY, m_statusScreenHandle, FALSE);
			}
			if (m_manualCount == kThirdManual||m_manualCount == kFourthManual || m_manualCount == kFifthManual)
			{
				DrawExtendGraph(kUpLeftX, kUpLeftY, kLowRightX, kLowRightY, m_statusUpVideoHandle, FALSE);
			}
		}

		RepeatVideo();
	}

}

//動画を繰り返し流す
void SceneTutorial::RepeatVideo()
{
	if (!m_isSecondTutorial)
	{
		if (m_manualCount == kSecondManual)
		{
			if (m_playVideo == 0)
			{
				PlayMovieToGraph(m_attackVideoHandle);
				m_playVideo++;
			}

			if (GetMovieStateToGraph( m_attackVideoHandle) == kStopMovie)
			{
				SeekMovieToGraph( m_attackVideoHandle, 0);
				PlayMovieToGraph( m_attackVideoHandle);
			}
		}
		if (m_manualCount == kThirdManual)
		{
			if (m_playVideo == 0)
			{
				PlayMovieToGraph(m_magicAttackVideoideoHandle);
				m_playVideo++;
			}

			if (GetMovieStateToGraph(m_magicAttackVideoideoHandle) == kStopMovie)
			{
				SeekMovieToGraph(m_magicAttackVideoideoHandle, 0);
				PlayMovieToGraph(m_magicAttackVideoideoHandle);
			}
		}
		if (m_manualCount == kFourthManual)
		{
			if (m_playVideo == 0)
			{
				PlayMovieToGraph(m_consumeMPVideoHandle);
				m_playVideo++;
			}

			if (GetMovieStateToGraph(m_consumeMPVideoHandle) == kStopMovie)
			{
				SeekMovieToGraph(m_consumeMPVideoHandle, 0);
				PlayMovieToGraph(m_consumeMPVideoHandle);
			}
		}
		if (m_manualCount == kFifthManual)
		{
			if (m_playVideo == 0)
			{
				PlayMovieToGraph(m_mpRecoveryVideoHandle);
				m_playVideo++;
			}

			if (GetMovieStateToGraph(m_mpRecoveryVideoHandle) == kStopMovie)
			{
				SeekMovieToGraph(m_mpRecoveryVideoHandle, 0);
				PlayMovieToGraph(m_mpRecoveryVideoHandle);
			}
		}
	}
	else if (!m_isThirdTutorial)
	{
		if (m_manualCount == kThirdManual)
		{
			if (m_playVideo == 0)
			{
				PlayMovieToGraph(m_exGaugeRiseVideoHandle);
				m_playVideo++;
			}

			if (GetMovieStateToGraph(m_exGaugeRiseVideoHandle) == kStopMovie)
			{
				SeekMovieToGraph(m_exGaugeRiseVideoHandle, 0);
				PlayMovieToGraph(m_exGaugeRiseVideoHandle);
			}
		}
		if (m_manualCount == kFourthManual|| m_manualCount == kFifthManual)
		{
			if (m_playVideo == 0)
			{
				PlayMovieToGraph(m_specialMoveVideoHandle);
				m_playVideo++;
			}

			if (GetMovieStateToGraph(m_specialMoveVideoHandle) == kStopMovie)
			{
				SeekMovieToGraph(m_specialMoveVideoHandle, 0);
				PlayMovieToGraph(m_specialMoveVideoHandle);
			}
		}
	}

	else if (!m_isFourtutorial)
	{
		if (m_manualCount == kSecondManual || m_manualCount == kThirdManual)
		{
			if (m_playVideo == 0)
			{
				PlayMovieToGraph(m_guardVideoHandle);
				m_playVideo++;
			}

			if (GetMovieStateToGraph(m_guardVideoHandle) == kStopMovie)
			{
				SeekMovieToGraph(m_guardVideoHandle, 0);
				PlayMovieToGraph(m_guardVideoHandle);
			}
		}
		if (m_manualCount == kFourthManual || m_manualCount == kFifthManual)
		{
			if (m_playVideo == 0)
			{
				PlayMovieToGraph(m_guardSuccessVideoHandle);
				m_playVideo++;
			}

			if (GetMovieStateToGraph(m_guardSuccessVideoHandle) == kStopMovie)
			{
				SeekMovieToGraph(m_guardSuccessVideoHandle, 0);
				PlayMovieToGraph(m_guardSuccessVideoHandle);
			}
		}
	}
	else if (!m_isFifthManual)
	{
		if (m_manualCount == kSecondManual || m_manualCount == kThirdManual)
		{
			if (m_playVideo == 0)
			{
				PlayMovieToGraph(m_warpEffectHandle);
				m_playVideo++;
			}

			if (GetMovieStateToGraph(m_warpEffectHandle) == kStopMovie)
			{
				SeekMovieToGraph(m_warpEffectHandle, 0);
				PlayMovieToGraph(m_warpEffectHandle);
			}
		}
		if (m_manualCount == kFourthManual || m_manualCount == kFifthManual)
		{
			if (m_playVideo == 0)
			{
				PlayMovieToGraph(m_warpMovementHandle);
				m_playVideo++;
			}

			if (GetMovieStateToGraph(m_warpMovementHandle) == kStopMovie)
			{
				SeekMovieToGraph(m_warpMovementHandle, 0);
				PlayMovieToGraph(m_warpMovementHandle);
			}
		}

	}
	else if (!m_isSixTutorial)
	{
		if (m_manualCount == kThirdManual||m_manualCount == kFourthManual || m_manualCount == kFifthManual)
		{
			if (m_playVideo == 0)
			{
				PlayMovieToGraph(m_statusUpVideoHandle);
				m_playVideo++;
			}

			if (GetMovieStateToGraph(m_statusUpVideoHandle) == kStopMovie)
			{
				SeekMovieToGraph(m_statusUpVideoHandle, 0);
				PlayMovieToGraph(m_statusUpVideoHandle);
			}
		}
	}
}

//チュートリアルに画像を描画する
void SceneTutorial::DrawTutorial()
{
	if (!m_isManual)
	{
		if (!m_isFirstTutorial)
		{
			DrawGraph(0, kTutorialPosY, m_firstTutorialHandle, true);
		}
		else if (!m_isSecondTutorial && m_tutorialKinds == kSecondTutorial)
		{
			DrawGraph(0, kTutorialPosY, m_secondTutorialHandle, true);
		}
		else if(!m_isThirdTutorial && m_tutorialKinds == kThirdTutorial)
		{
			DrawGraph(0, kTutorialPosY, m_thirdTutorialHandle,true);
		}
		else if (!m_isFourtutorial && m_tutorialKinds == kFourTutorial)
		{
			DrawGraph(0, kTutorialPosY, m_fourTutorialHandle, true);
		}
	}
	
}

//チュートリアル中にテキストを描画する
void SceneTutorial::DrawTutorialText()
{
	if (!m_isManual)
	{
		if (!m_isFirstTutorial)
		{
			//第1のチュートリアルのテキスト
			DrawFormatStringFToHandle(kTutorialTextPosX, kTutorialTextPosY, GetColor(0, 0, 0), Font::m_fontHandle[static_cast<int>(Font::FontId::kSize55_4)], "%4d/3", m_walkTimes/ kCountToSecond);
			DrawFormatStringFToHandle(kTutorialTextPosX, kFirstTutorialTextPosY, GetColor(0, 0, 0), Font::m_fontHandle[static_cast<int>(Font::FontId::kSize55_4)], "%4d/3", m_runTimes/ kCountToSecond);
		}
		else if (!m_isSecondTutorial && m_tutorialKinds == kSecondTutorial)
		{
			//第2チュートリアルのテキスト
			DrawFormatStringFToHandle(kTutorialTextPosX, kTutorialTextPosY, GetColor(0, 0, 0), Font::m_fontHandle[static_cast<int>(Font::FontId::kSize55_4)], "%4d/5", m_attackCount);
			DrawFormatStringFToHandle(kTutorialTextPosX, kSecondTutorialTextPosY, GetColor(0, 0, 0), Font::m_fontHandle[static_cast<int>(Font::FontId::kSize55_4)], "%4d/3", m_magicAttackCount);
		}
		else if (!m_isThirdTutorial && m_tutorialKinds == kThirdTutorial)
		{
			//第3チュートリアルのテキスト
			DrawFormatStringFToHandle(kTutorialTextPosX, kThirdTutorialTextPosY, GetColor(0, 0, 0), Font::m_fontHandle[static_cast<int>(Font::FontId::kSize55_4)], "%4d/1", m_specialMoveCount);
		}
		else if (!m_isFourtutorial && m_tutorialKinds == kFourTutorial)
		{
			//第3チュートリアルのテキスト
			DrawFormatStringFToHandle(kTutorialTextPosX, kThirdTutorialTextPosY, GetColor(0, 0, 0), Font::m_fontHandle[static_cast<int>(Font::FontId::kSize55_4)], "%4d/3", m_guardCount);
		}
	}

}

void SceneTutorial::Fade()
{
	//チュートリアルをクリアした時にフェードインする
	if (m_isTutorialClear)
	{
		m_fadeAlpha += kFadeUpDown;

		if (m_fadeAlpha > kFadeValue)
		{
			m_fadeAlpha = kFadeValue;

			m_isTutorialClear = false;
		}
	}
	//チュートリアル1をクリアした時にフェードアウトする
	else if (!m_isTutorialClear && m_tutorialKinds == kTurorialStart && !m_isFirstTutorial)
	{
		m_fadeAlpha -= kFadeUpDown;

		m_isManual = true;

		if (m_fadeAlpha < 0)
		{
			m_tutorialKinds = kFirstTutorial;
			m_fadeAlpha = 0;
		}
	}
	//チュートリアル2をクリアした時にフェードアウトする
	else if (!m_isTutorialClear && m_tutorialKinds == kFirstTutorial && m_isFirstTutorial)
	{
		m_fadeAlpha -= kFadeUpDown;

		m_isManual = true;
		if (m_fadeAlpha < 0)
		{
			m_tutorialKinds = kSecondTutorial;
			m_fadeAlpha = 0;
		}
	}
	//チュートリアル3をクリアした時にフェードアウトする
	else if(!m_isTutorialClear && m_tutorialKinds ==  kSecondTutorial && m_isSecondTutorial)
	{
		m_fadeAlpha -= kFadeUpDown;

		m_isManual = true;

		if (m_fadeAlpha < 0)
		{
			m_tutorialKinds = kThirdTutorial;
			m_fadeAlpha = 0;
		}
	}
	//チュートリアル4をクリアした時にフェードアウトする
	else if (!m_isTutorialClear && m_tutorialKinds == kThirdTutorial && m_isThirdTutorial)
	{
		m_fadeAlpha -= kFadeUpDown;

		m_isManual = true;

		if (m_fadeAlpha < 0)
		{
			m_fadeAlpha = 0;
			m_tutorialKinds = kFourTutorial;
		}
	}
	//チュートリアル5をクリアした時にフェードアウトする
	else if (!m_isTutorialClear && m_tutorialKinds == kFourTutorial && m_isFourtutorial)
	{
		m_fadeAlpha -= kFadeUpDown;

		m_isManual = true;

		if (m_fadeAlpha < 0)
		{
			m_fadeAlpha = 0;
		}
	}
	else if (!m_isTutorialClear)
	{
		m_fadeAlpha -= kFadeUpDown;

		if (m_fadeAlpha < 0)
		{
			m_fadeAlpha = 0;
		}
	}
}
