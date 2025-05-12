#include "SceneSelect.h"
#include "SceneStatus.h"
#include "SoundManager.h"
#include "SceneFirstStage.h"
#include "SceneSecondStage.h"
#include "SceneTitle.h"
#include "Stage.h"
#include "Pad.h"
#include "Font.h"
#include "Game.h"

namespace
{
	//描画する三角のX位置
	constexpr int  kTrianglePos1X = 650;
	constexpr int  kTrianglePos2X = 250;
	constexpr int  kTrianglePos3X = 1050;

	//上を示す三角のY位置
	constexpr int  kUpTrianglePos1Y = 10;
	constexpr int  kUpTrianglePos2Y = 120;
	constexpr int  kUpTrianglePos3Y = 120;

	//下を示す三角のY位置
	constexpr int kDownTrianglePos1Y = 1070;
	constexpr int kDownTrianglePos2Y = 960;
	constexpr int kDownTrianglePos3Y = 960;

	//カメラの座標
	constexpr float kCameraPosX = 200.0f;
	constexpr float kCameraPosY = 280.0f;
	constexpr float kCameraPosZ = -350.0f;

	//カメラターゲットの座標
	constexpr  float kCameraTargetPosX = -350.0f;
	constexpr  float kCameraTargetPosY = 140.0f;

	//カーソルの位置によって、ステージの種類を変更する
	constexpr float kStage1PosY = 0.0f;
	constexpr float kStage2PosY = 1.0f;

	//ステージの説明文の位置
	constexpr float kStageDescriptionPosX = 170.0f;
	constexpr float kStageDescriptionPosY = 700.0f;

	//ステージの難易度を描画する位置
	constexpr float kStageLevelX = 750.0f;
	constexpr float kStageLevelY = 850.0f;

	//動画の左上の座標
	constexpr int kUpLeftX = 150;
	constexpr int kUpLeftY = 200;

	//動画の右下の座標
	constexpr int kLowRightX =1120;
	constexpr int kLowRightY = 670;

	//フェードイン、フェードアウトの数値
	constexpr int kFadeValue = 255;

	//フェード値の増減
	constexpr int kFadeUpDown = 8;

	//カーソルが動く時間
	constexpr int kCursorMoveTime = 25;

	//ステージの説明文の移動量
	constexpr float kStageDescriptionMove = 20.0f;

	//カーソルの移動量
	constexpr float kCursorMove = 1.0f;

	//セレクト画面に戻るカーソルの位置
	constexpr float kBackToSelect = 1.0f;

	//はいの文字位置
	constexpr float kDecisionCharacterPosX = 850.0f;
	constexpr float kDecisionCharacterPosY = 450.0f;

	//いいえ文字位置
	constexpr float kCancelCharacterPosX = 800.0f;
	constexpr float kCancelCharacterPosY = 700.0f;

	//動画が止まっているか
	constexpr int kStopMovie = 0;
}

SceneSelect::SceneSelect() :m_uiHandle(-1), m_operationInstructionsHandle(-1), m_menuHandle(-1),m_count(0), m_cursorCount(0), m_cursorPosY(0.0f),
m_isStage1(false), m_isStage2(false), m_isStatus(false), m_isExplanation(false),  m_isPishAButton(false), m_isPressPad(false), m_isPressUp(false), m_isPressDown(false), m_ignoreNextAInput(false),m_cursorPos(VGet(0.0f, 0.0f, 0.0f)), m_stage1DescriptionPos(VGet(0.0f, 0.0f, 0.0f)),
m_stage2DescriptionPos(VGet(0.0f, 0.0f, 0.0f)), m_statusDescriptionPos(VGet(0.0f, 0.0f, 0.0f)), m_operationDescriptionPos(VGet(0.0f, 0.0f, 0.0f)), m_stage1Pos(VGet(0.0f, 0.0f, 0.0f)), m_stage2Pos(VGet(0.0f, 0.0f, 0.0f)),
m_statusPos(VGet(0.0f, 0.0f, 0.0f)), m_explanationPos(VGet(0.0f, 0.0f, 0.0f)), m_gameEndPos(VGet(0.0f, 0.0f, 0.0f))
{
}

SceneSelect::~SceneSelect()
{
	m_pSoundManager.reset();
	m_pSoundManager = nullptr;

	m_pStage.reset();
	m_pStage = nullptr;

	//画像の削除
	DeleteGraph(m_bgHandle);
	DeleteGraph(m_operationInstructionsHandle);
	DeleteGraph(m_menuHandle);
	DeleteGraph(m_firstStageVideoHandle);
	DeleteGraph(m_secondStageVideoHandle);
}

void SceneSelect::Init()
{
	//初期化
	m_playVideo = 0;

	//画像のロード
	m_uiHandle = LoadGraph("data/UI/NewSelectScene.png");

	//操作説明の画像をロード
	m_operationInstructionsHandle = LoadGraph("data/BG/OperationInstructions.png");

	//メニュー画面の画像をロード
	m_menuHandle = LoadGraph("data/UI/selectmenu.png");

	//ステージ1の動画をロード
	m_firstStageVideoHandle = LoadGraph("data/mp4/stage1.mp4");

	//ステージ2の動画をロード
	m_secondStageVideoHandle = LoadGraph("data/mp4/stage2.mp4");

	//ステージ
	m_pStage = std::make_shared<Stage>(Select);
	m_pStage->Init();

	//カーソルの位置初期化
	m_cursorPos = VGet(0.0f, 0.0f, 0.0f);

	//ステージの説明文の位置初期化
	m_stage1DescriptionPos = VGet(kStageDescriptionPosX, 0.0f, 0.0f);
	m_stage2DescriptionPos = VGet(kStageDescriptionPosX, 0.0f, 0.0f);
	m_statusDescriptionPos = VGet(kStageDescriptionPosX, 0.0f, 0.0f);

	//フェード値の初期設定
	m_fadeAlpha = kFadeValue;

	//BGMを再生する
	m_pSoundManager->SelectBGM();

	//カメラ座標
	m_cameraPos = VAdd(VGet(0.0f, 0.0f, 0.0f), VGet(kCameraPosX, kCameraPosY, kCameraPosZ));

	// 注視点
	m_cameraTarget = VAdd(VGet(0.0f, 0.0f, 0.0f), VGet(kCameraTargetPosX, kCameraTargetPosY, 0.0f));

	//初期化
	m_isStage1 = false;
	m_isStage2 = false;
	m_isStatus = false;
	m_isExplanation = false;
	m_isPishAButton = false;

	m_cursorCount = kCursorMoveTime;
}

std::shared_ptr<SceneBase> SceneSelect::Update(const Pad& pad)
{
	//フェード関係
	Fade();

	//ステージを選ぶ際の処理
	StageSelect(pad);

	//タイトル画面に戻る際の処理
	BackToTitle(pad);

	//カメラの位置設定
	SetCameraPositionAndTarget_UpVecY(m_cameraPos, m_cameraTarget);

	//どのシーンに遷移するかを返す
	return SceneChange(pad);
}


//描画
void SceneSelect::Draw()
{
	//ステージの描画
	m_pStage->Draw();

	//矢印を描画する
	DrawArrow();

	//UIの描画
	DrawGraph(0, 0, m_uiHandle, true);

	//動画を描画する
	DrawStageMovie();

	//説明文の描画
	DrawExplanation();

	//メニュー画面の描画
	DrawMenu();

	if (m_isExplanation)
	{
		DrawGraph(0, 0, m_operationInstructionsHandle, true);
	}
	
	
	//フェードの描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha); //半透明で表示
	DrawBox(0, 0, Game::kScreenWindidth, Game::kScreenHeight, GetColor(0, 0, 0), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); //不透明に戻しておく
}

void SceneSelect::End()
{

}

//タイトルに戻る
void SceneSelect::BackToTitle(const Pad& pad)
{
	//メニューを開いていた場合
	if (m_isOpenMenu)
	{
		if (pad.IsTrigger("down"))
		{
			m_cursorPosY += kCursorMove;
		}
		if (pad.IsTrigger("up"))
		{
			m_cursorPosY -= kCursorMove;
		}

		if (pad.IsTrigger("A"))
		{
			if (m_cursorPosY == 0.0f)
			{
				m_isBackToTheTitle = true;
			}
			if (m_cursorPosY == kBackToSelect)
			{
				m_isOpenMenu = false;
				m_ignoreNextAInput = true;
				
			}
		}
	}

	if (m_cursorPosY < 0.0f)
	{
		m_cursorPosY = 0.0f;
	}
	if (m_cursorPosY > kBackToSelect)
	{
		m_cursorPosY = kBackToSelect;
	}
}

//矢印を描画する
void SceneSelect::DrawArrow()
{
	//上を示す三角を描画
	if (!m_isPressUp)
	{
		DrawTriangle(kTrianglePos1X, kUpTrianglePos1Y, kTrianglePos2X, kUpTrianglePos2Y, kTrianglePos3X, kUpTrianglePos3Y, GetColor(0, 0, 0), TRUE);
	}
	else if (m_isPressUp)
	{
		DrawTriangle(kTrianglePos1X, kUpTrianglePos1Y, kTrianglePos2X, kUpTrianglePos2Y, kTrianglePos3X, kUpTrianglePos3Y, GetColor(100, 100, 100), TRUE);
	}

	//下を示す三角を描画する
	if (!m_isPressDown)
	{
		DrawTriangle(kTrianglePos1X, kDownTrianglePos1Y, kTrianglePos2X, kDownTrianglePos2Y, kTrianglePos3X, kDownTrianglePos3Y, GetColor(0, 0, 0), TRUE);
	}
	else if (m_isPressDown)
	{
		DrawTriangle(kTrianglePos1X, kDownTrianglePos1Y, kTrianglePos2X, kDownTrianglePos2Y, kTrianglePos3X, kDownTrianglePos3Y, GetColor(100, 100, 100), TRUE);
	}
}

//ステージを選ぶ際の処理
void SceneSelect::StageSelect(const Pad& pad)
{
	if (!m_isExplanation && !m_isOpenMenu)
	{
		//カーソルの移動
		if (pad.IsPress("up"))
		{
			m_isPressPad = true;
		}
		if (pad.IsPress("down"))
		{
			m_isPressPad = true;
		}

		//カーソル移動が行われているとき
		if (m_isPressPad)
		{
			//一定時間スティックを倒した場合
			if (m_cursorCount >= kCursorMoveTime)
			{
				if (pad.IsPress("up"))
				{
					m_cursorPos.y -= kCursorMove;

					m_isPressUp = true;

					//SEを流す
					m_pSoundManager->MoveCursorSE();

				}
				else if (pad.IsPress("down"))
				{
					m_cursorPos.y += kCursorMove;

					m_isPressDown = true;

					//SEを流す
					m_pSoundManager->MoveCursorSE();
				}

				m_cursorCount = 0;
			}

			if (pad.IsRelase("up") || pad.IsRelase("down"))
			{
				m_isPressPad = false;
				m_isPressUp = false;
				m_isPressDown = false;
			}

			m_cursorCount++;
		}

		else
		{
			m_cursorCount = kCursorMoveTime;
		}

		//上限を超える一番下の項目に飛ぶ
		if (m_cursorPos.y < kStage1PosY)
		{
			m_cursorPos.y = kStage2PosY;
		}

		//上限を超えると、一番上の項目に飛ぶ
		if (m_cursorPos.y > kStage2PosY)
		{
			m_cursorPos.y = kStage1PosY;
		}
	}
}

//選択している項目別の説明文を描画する
void SceneSelect::DrawExplanation()
{
	//ステージ1
	if (m_cursorPos.y == kStage1PosY)
	{
		//ステージの説明を描画
		DrawFormatStringFToHandle(kStageDescriptionPosX, kStageDescriptionPosY, GetColor(255, 255, 255), Font::m_fontHandle[static_cast<int>(Font::FontId::kSize55_4)], "ステージ1\n敵を倒して,ポイントを稼ぎましょう！");

		//難易度を描画する
		DrawFormatStringFToHandle(kStageLevelX, kStageLevelY, GetColor(255, 255, 255), Font::m_fontHandle[static_cast<int>(Font::FontId::kSize55_4)], "難易度 D\n");
	}
	//ステージ2
	if (m_cursorPos.y == kStage2PosY)
	{
		//ステージの説明を描画する
		DrawFormatStringFToHandle(kStageDescriptionPosX, kStageDescriptionPosY, GetColor(255, 255, 255), Font::m_fontHandle[static_cast<int>(Font::FontId::kSize55_4)], "ボス戦\nステータスを上げて挑戦しよう！");

		//難易度を描画する
		DrawFormatStringFToHandle(kStageLevelX, kStageLevelY, GetColor(255, 255, 255), Font::m_fontHandle[static_cast<int>(Font::FontId::kSize55_4)], "難易度 B\n");
	}

}

//メニュー画面を描画する
void SceneSelect::DrawMenu()
{
	//メニュー画面を表示する場合の処理
	if (m_isOpenMenu)
	{
		DrawGraph(0, 0, m_menuHandle, true);


		//カーソルの位置によって文字の色が変わる
		if (m_cursorPosY == 0.0f)
		{
			DrawFormatStringFToHandle(kDecisionCharacterPosX, kDecisionCharacterPosY, GetColor(150, 150, 150), Font::m_fontHandle[static_cast<int>(Font::FontId::kSize96_4)], "はい\n");
		}
		else
		{
			DrawFormatStringFToHandle(kDecisionCharacterPosX, kDecisionCharacterPosY, GetColor(0, 0, 0), Font::m_fontHandle[static_cast<int>(Font::FontId::kSize96_4)], "はい\n");
		}
		if (m_cursorPosY == kBackToSelect)
		{
			DrawFormatStringFToHandle(kCancelCharacterPosX, kCancelCharacterPosY, GetColor(150, 150, 150), Font::m_fontHandle[static_cast<int>(Font::FontId::kSize96_4)], "いいえ\n");
		}
		else
		{
			DrawFormatStringFToHandle(kCancelCharacterPosX, kCancelCharacterPosY, GetColor(0, 0, 0), Font::m_fontHandle[static_cast<int>(Font::FontId::kSize96_4)], "いいえ\n");
		}
	}
}

//選択しているステージによって、再生する動画を変更する
void SceneSelect::DrawStageMovie()
{
	//ステージ1
	if (m_cursorPos.y == kStage1PosY)
	{
		DrawExtendGraph(kUpLeftX, kUpLeftY, kLowRightX, kLowRightY, m_firstStageVideoHandle, FALSE);
	}
	//ステージ2
	if (m_cursorPos.y == kStage2PosY)
	{
		DrawExtendGraph(kUpLeftX, kUpLeftY, kLowRightX, kLowRightY, m_secondStageVideoHandle, FALSE);
	}

	//動画を繰り返し流す
	RepeatVideo();

}

//動画を繰り返し流す
void SceneSelect::RepeatVideo()
{
	if (m_cursorPos.y == kStage1PosY)
	{
		if (m_playVideo == 0)
		{
			PlayMovieToGraph(m_firstStageVideoHandle);
			m_playVideo++;
		}

		if (GetMovieStateToGraph(m_firstStageVideoHandle) == kStopMovie)
		{
			SeekMovieToGraph(m_firstStageVideoHandle, 0);
			PlayMovieToGraph(m_firstStageVideoHandle);
		}
	}
	if (m_cursorPos.y == kStage2PosY)
	{
		if (m_playVideo == 0)
		{
			PlayMovieToGraph(m_secondStageVideoHandle);
			m_playVideo++;
		}

		if (GetMovieStateToGraph(m_secondStageVideoHandle) == kStopMovie)
		{
			SeekMovieToGraph(m_secondStageVideoHandle, 0);
			PlayMovieToGraph(m_secondStageVideoHandle);
		}
	}
}

//選択した項目によって、遷移するシーンを変更する
std::shared_ptr<SceneBase> SceneSelect::SceneChange(const Pad& pad)
{
	//ステージの遷移する
	if (!m_isExplanation && !m_isOpenMenu && !m_ignoreNextAInput)
	{
		if (pad.IsTrigger("A"))
		{
			if (m_cursorPos.y == 0.0f)
			{
				//SEの再生
				m_pSoundManager->DeterminationSE();
				m_isStage1 = true;
			}
			if (m_cursorPos.y == 1.0f)
			{

				//SEの再生
				m_pSoundManager->DeterminationSE();
				m_isStage2 = true;
			}
		}
		if (m_isStage1 && m_fadeAlpha >= kFadeValue)
		{
			return std::make_shared<SceneFirstStage>();
		}
		if (m_isStage2 && m_fadeAlpha >= kFadeValue)
		{
			return std::make_shared<SceneSecondStage>();
		}

		//ステータスに遷移する
		if (pad.IsTrigger("X"))
		{
			//SEの再生
			m_pSoundManager->DeterminationSE();
			m_isStatus = true;
		}
		
		//操作説明画面を表示する
		if (pad.IsTrigger("Y"))
		{
			m_isExplanation = true;
		}

		if (pad.IsTrigger("B"))
		{
			m_isOpenMenu = true;
		}
	}

	if (m_ignoreNextAInput)
	{
		if (!pad.IsPress("A"))  // 押されてない状態になるまで待つ
		{
			m_ignoreNextAInput = false; // 押されてないので通常処理再開
		}
		return shared_from_this(); // それ以外は何もしない
	}

	if (m_isStatus && m_fadeAlpha >= kFadeValue)
	{
		return std::make_shared<SceneStatus>();
	}
	if (m_isBackToTheTitle && m_fadeAlpha >= kFadeValue)
	{
		return std::make_shared<SceneTitle>();
	}

	//操作説明を描画するか
	if (m_isExplanation && pad.IsRelase("Y"))
	{
		m_isPishAButton = true;
	}

	if (m_isPishAButton && pad.IsTrigger("B"))
	{
		m_isExplanation = false;
		m_isPishAButton = false;
	}


	return shared_from_this();
}

//フェードイン、フェードアウト
void SceneSelect::Fade()
{
	if (!m_isStage1 && !m_isStage2 && !m_isStatus &&!m_isBackToTheTitle)
	{
		m_fadeAlpha -= kFadeUpDown;
		if (m_fadeAlpha < 0)
		{
			m_fadeAlpha = 0;
		}
	}
	if (m_isStage1)
	{
		m_fadeAlpha += kFadeUpDown;
		if (m_fadeAlpha > kFadeValue)
		{
			m_fadeAlpha = kFadeValue;
		}
	}
	if (m_isStage2)
	{
		m_fadeAlpha += kFadeUpDown;
		if (m_fadeAlpha > kFadeValue)
		{
			m_fadeAlpha = kFadeValue;
		}
	}
	if (m_isStatus)
	{
		m_fadeAlpha += kFadeUpDown;
		if (m_fadeAlpha > kFadeValue)
		{
			m_fadeAlpha = kFadeValue;
		}
	}
	if (m_isBackToTheTitle)
	{
		m_fadeAlpha += kFadeUpDown;
		if (m_fadeAlpha > kFadeValue)
		{
			m_fadeAlpha = kFadeValue;
		}
	}

}


