#include "SceneSelect.h"
#include "SceneStage.h"
#include "SceneStatus.h"
#include "SoundManager.h"
#include "Pad.h"
#include "Font.h"
#include "Game.h"

namespace
{
	//カーソルの移動量
	constexpr float kCarsorMove = 200.0f;

	//カーソル位置
	constexpr float kStage1Pos = 0.0f;
	constexpr float kStage2Pos = 200.0f;
	constexpr float kStatusPos = 400.0f;
	constexpr float kExplanationPos = 600.0f;
	constexpr float kGameEndPos = 800.0f;

	//説明文の位置
	constexpr float kExplanatoryTextPosY = 950.0f;

	//ステージの説明文の位置
	constexpr float kStageDescriptionPosX = 650.0f;

	//フェードイン、フェードアウトの数値
	constexpr int kFadeValue = 255;

	//フェード値の増減
	constexpr int kFadeUpDown = 8;

	//カーソルが動く時間
	constexpr int kCursorMoveTime = 25;

	//上下の最大移動量
	constexpr int kMove =60;

	//ステージの説明文の移動量
	constexpr float kStageDescriptionMove = 20.0f;
}

SceneSelect::SceneSelect() :m_stage1Handle(-1), m_stage2Handle(-1), m_statusHandle(-1), m_operationHandle(-1), m_gameEndHandle(-1),
m_stage1DescriptionHandle(-1), m_stage2DescriptionHandle(-1),m_operationInstructionsHandle(-1), m_statusDescriptionHandle(-1), m_count(0), m_cursorCount(0), m_isDebug(false),
m_isStage1(false), m_isStage2(false),m_isStatus(false), m_isExplanation(false), m_isCautionaryNote(false), m_isPishAButton(false), m_isPressPad(false), m_cursorPos(VGet(0.0f,0.0f,0.0f)), m_stage1DescriptionPos(VGet(0.0f,0.0f,0.0f)),
m_stage2DescriptionPos(VGet(0.0f,0.0f,0.0f)), m_statusDescriptionPos(VGet(0.0f,0.0f,0.0f)), m_operationDescriptionPos(VGet(0.0f,0.0f,0.0f)),m_stage1Pos(VGet(0.0f, 0.0f, 0.0f)), m_stage2Pos(VGet(0.0f, 0.0f, 0.0f)),
m_statusPos(VGet(0.0f, 0.0f, 0.0f)),m_explanationPos(VGet(0.0f, 0.0f, 0.0f)), m_gameEndPos(VGet(0.0f, 0.0f, 0.0f))
{
#ifdef _DEBUG

	m_isDebug = true;
#endif
}

SceneSelect::~SceneSelect()
{
	m_pSoundManager.reset();
	m_pSoundManager = nullptr;

	//画像の削除
	DeleteGraph(m_bgHandle);
	DeleteGraph(m_stage1Handle);
	DeleteGraph(m_stage2Handle);
	DeleteGraph(m_statusHandle);
	DeleteGraph(m_operationHandle);
	DeleteGraph(m_gameEndHandle);
	DeleteGraph(m_stage1DescriptionHandle);
	DeleteGraph(m_stage2DescriptionHandle);
	DeleteGraph(m_operationInstructionsHandle);
	DeleteGraph(m_cautionaryNoteHandle);
	
}

void SceneSelect::Init()
{
	//画像のロード
	m_bgHandle = LoadGraph("data/BG/SelectBG.png");
	m_stage1Handle = LoadGraph("data/UI/Stage1.png");
	m_stage2Handle = LoadGraph("data/UI/Stage2.png");
	m_statusHandle = LoadGraph("data/UI/status.png");
	m_operationHandle = LoadGraph("data/UI/operationInstructions.png");
	m_gameEndHandle = LoadGraph("data/UI/gameover.png");
	m_stage1DescriptionHandle = LoadGraph("data/BG/Stage1.png");
	m_stage2DescriptionHandle = LoadGraph("data/BG/Stage2.png");
	m_statusDescriptionHandle = LoadGraph("data/BG/Status.png");
	m_operationDescriptionHandle = LoadGraph("data/BG/Operation.png");
	m_operationInstructionsHandle = LoadGraph("data/BG/OperationInstructions.png");

	if (!m_isDebug)
	{
		m_cautionaryNoteHandle = LoadGraph("data/BG/CautionaryNote.png");
	}

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
	
	//カーソルの動き
	CursorMotion(pad);

	//選択肢の動き
	MoveChatracter();

	//どのシーンに遷移するかを返す
	return SceneChange(pad);
}


//描画
void SceneSelect::Draw()
{
	//背景の描画
	DrawGraph(0, 0, m_bgHandle, true);

	//文字の描画
	DrawCharacter();

	//説明文の描画
	DrawExplanation();

	//ステージの説明文の描画
	DrawStageExplanation();

	if (m_isExplanation)
	{
		DrawGraph(0, 0, m_operationInstructionsHandle, true);
	}
	if (m_isCautionaryNote)
	{
		DrawGraph(0, 0, m_cautionaryNoteHandle, true);
	}
	//フェードの描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha); //半透明で表示
	DrawBox(0, 0, Game::kScreenWindidth, Game::kScreenHeight, GetColor(0, 0, 0), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); //不透明に戻しておく

}

void SceneSelect::End()
{

}

//カーソルの移動
void SceneSelect::CursorMotion(const Pad& pad)
{
	if (!m_isExplanation)
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
			if (m_cursorCount >= kCursorMoveTime)
			{
				if (pad.IsPress("up"))
				{
					m_cursorPos.y -= kCarsorMove;

					//SEを流す
					m_pSoundManager->MoveCursorSE();

				}
				else if (pad.IsPress("down"))
				{
					m_cursorPos.y += kCarsorMove;

					//SEを流す
					m_pSoundManager->MoveCursorSE();
				}

				m_cursorCount = 0;
			}

			if (pad.IsRelase("up") || pad.IsRelase("down"))
			{
				m_isPressPad = false;
			}

			m_cursorCount++;
		}
		else
		{
			m_cursorCount = kCursorMoveTime;
		}

		//上限を超える一番下の項目に飛ぶ
		if (m_cursorPos.y < kStage1Pos)
		{
			m_cursorPos.y = kGameEndPos;
		}

		//上限を超えると、一番上の項目に飛ぶ
		if (m_cursorPos.y > kGameEndPos)
		{
			m_cursorPos.y = kStage1Pos;
		}
	}


}

//選択できる文字を描画する
void SceneSelect::DrawCharacter()
{
	//ステージ1の文字描画
	DrawGraph(m_stage1Pos.x, m_stage1Pos.y, m_stage1Handle, true);

	//ステージ2の文字描画
	DrawGraph(m_stage2Pos.x, m_stage2Pos.y, m_stage2Handle, true);

	//ステータスの文字描画
	DrawGraph(m_statusPos.x, m_statusPos.y, m_statusHandle, true);

	//操作方法の文字描画
	DrawGraph(m_explanationPos.x, m_explanationPos.y, m_operationHandle, true);

	//ゲーム終了の文字描画
	DrawGraph(m_gameEndPos.x, m_gameEndPos.y, m_gameEndHandle, true);
}

//選択できる位置の文字を動かす
void SceneSelect::MoveChatracter()
{
	//ステージ1の文字の動き
	if (m_cursorPos.y == kStage1Pos)
	{
		m_stage1Pos = VAdd(m_stage1Pos,VGet(0.0f, sin(DX_PI_F * 2 / 60 * m_count) * 2, 0.0f));
		m_stage2Pos = VGet(0.0f, 0.0f, 0.0f);
		m_statusPos = VGet(0.0f, 0.0f, 0.0f);
		m_explanationPos = VGet(0.0f, 0.0f, 0.0f);
		m_gameEndPos = VGet(0.0f, 0.0f, 0.0f);
	}
	else
	{
		m_stage1Pos = VGet(0.0f, 0.0f, 0.0f);
	}

	if(m_cursorPos.y == kStage2Pos)
	{
		m_stage1Pos = VGet(0.0f, 0.0f, 0.0f);
		m_stage2Pos = VAdd(m_stage2Pos,VGet(0.0f, sin(DX_PI_F * 2 / 60 * m_count) * 2, 0.0f));
		m_statusPos = VGet(0.0f, 0.0f, 0.0f);
		m_explanationPos = VGet(0.0f, 0.0f, 0.0f);
		m_gameEndPos = VGet(0.0f, 0.0f, 0.0f);
	}
	else 
	{ 
		m_stage2Pos = VGet(0.0f, 0.0f, 0.0f);
	}

	if (m_cursorPos.y == kStatusPos)
	{
		m_stage1Pos = VGet(0.0f, 0.0f, 0.0f);
		m_stage2Pos = VGet(0.0f, 0.0f, 0.0f);
		m_statusPos = VAdd(m_statusPos,VGet(0.0f, sin(DX_PI_F * 2 / 60 * m_count) * 2, 0.0f));
		m_explanationPos = VGet(0.0f, 0.0f, 0.0f);
		m_gameEndPos = VGet(0.0f, 0.0f, 0.0f);
	}
	else
	{
		m_statusPos = VGet(0.0f, 0.0f, 0.0f);
	}

	if (m_cursorPos.y == kExplanationPos)
	{
		m_stage1Pos = VGet(0.0f, 0.0f, 0.0f);
		m_stage2Pos = VGet(0.0f, 0.0f, 0.0f);
		m_statusPos = VGet(0.0f, 0.0f, 0.0f);
		m_explanationPos = VAdd(m_explanationPos, VGet(0.0f, sin(DX_PI_F * 2 / 60 * m_count) * 2, 0.0f));
		m_gameEndPos = VGet(0.0f, 0.0f, 0.0f);
	}
	else
	{
		m_explanationPos = VGet(0.0f, 0.0f, 0.0f);
	}
	if (m_cursorPos.y == kGameEndPos)
	{
		m_stage1Pos = VGet(0.0f, 0.0f, 0.0f);
		m_stage2Pos = VGet(0.0f, 0.0f, 0.0f);
		m_statusPos = VGet(0.0f, 0.0f, 0.0f);
		m_explanationPos = VGet(0.0f, 0.0f, 0.0f);
		m_gameEndPos = VAdd(m_gameEndPos, VGet(0.0f, sin(DX_PI_F * 2 / 60 * m_count) * 2, 0.0f));
	}
	else
	{
		m_gameEndPos = VGet(0.0f, 0.0f, 0.0f);
	}


	m_count++;

	if (m_count > kMove)
	{
		m_count = 0;
	}

}

//選択しているステージの説明文を描画する
void SceneSelect::DrawStageExplanation()
{
	//ステージ1の説明文の描画
	if (m_cursorPos.y == kStage1Pos)
	{
		DrawGraph(m_stage1DescriptionPos.x, m_stage1DescriptionPos.y, m_stage1DescriptionHandle, true);

		m_stage1DescriptionPos.x -= kStageDescriptionMove;

		if (m_stage1DescriptionPos.x <= 0)
		{
			m_stage1DescriptionPos.x = 0;
		}
	}
	else
	{
		m_stage1DescriptionPos.x = kStageDescriptionPosX;
	}

	//ステージ2の説明文の描画
	if (m_cursorPos.y == kStage2Pos)
	{
		DrawGraph(m_stage2DescriptionPos.x, m_stage2DescriptionPos.y, m_stage2DescriptionHandle, true);

		m_stage2DescriptionPos.x -= kStageDescriptionMove;

		if (m_stage2DescriptionPos.x <= 0)
		{
			m_stage2DescriptionPos.x = 0;
		}
	}
	else
	{
		m_stage2DescriptionPos.x = kStageDescriptionPosX;
	}
	
	//ステータスの説明文の描画
	if (m_cursorPos.y == kStatusPos)
	{
		DrawGraph(m_statusDescriptionPos.x, m_statusDescriptionPos.y, m_statusDescriptionHandle, true);

		m_statusDescriptionPos.x -= kStageDescriptionMove;

		if (m_statusDescriptionPos.x <= 0)
		{
			m_statusDescriptionPos.x = 0;
		}
	}
	else
	{
		m_statusDescriptionPos.x = kStageDescriptionPosX;
	}
	//説明画面の説明文の描画
	if (m_cursorPos.y == kExplanationPos)
	{
		DrawGraph(m_operationDescriptionPos.x, m_operationDescriptionPos.y, m_operationDescriptionHandle, true);

		m_operationDescriptionPos.x -= kStageDescriptionMove;

		if (m_operationDescriptionPos.x <= 0)
		{
			m_operationDescriptionPos.x = 0;
		}
	}
	else
	{
		m_operationDescriptionPos.x = kStageDescriptionPosX;
	}

}

//選択している項目別の説明文を描画する
void SceneSelect::DrawExplanation()
{
	if (m_cursorPos.y == kStage1Pos)
	{
		DrawFormatStringFToHandle(0.0f, kExplanatoryTextPosY, GetColor(255, 255, 255),
			Font::m_fontHandle[static_cast<int>(Font::FontId::kSize55_4)], "初心者におすすめ。敵を倒してステータスポイントを獲得しよう！！\n");
	}

	if (m_cursorPos.y == kStage2Pos)
	{
		DrawFormatStringFToHandle(0.0f, kExplanatoryTextPosY, GetColor(255, 255, 255),
			Font::m_fontHandle[static_cast<int>(Font::FontId::kSize55_4)], "ステータスを上げて挑戦しよう！！\n");
	}

	if (m_cursorPos.y == kStatusPos)
	{
		DrawFormatStringFToHandle(0.0f, kExplanatoryTextPosY, GetColor(255, 255, 255),
			Font::m_fontHandle[static_cast<int>(Font::FontId::kSize55_4)], "敵を倒して獲得したポイントでステータスを強化しよう！\n");
	}

	if(m_cursorPos.y == kExplanationPos)
	{ 
		DrawFormatStringFToHandle(0.0f, kExplanatoryTextPosY, GetColor(255, 255, 255),
			Font::m_fontHandle[static_cast<int>(Font::FontId::kSize55_4)], "ゲームプレイ中の操作説明が書かれているよ！\n");
	}

	if (m_cursorPos.y == kGameEndPos)
	{
		DrawFormatStringFToHandle(0.0f, kExplanatoryTextPosY, GetColor(255, 255, 255),
			Font::m_fontHandle[static_cast<int>(Font::FontId::kSize55_4)], "ゲーム終了するよ！\n");
	}


}

//選択した項目によって、遷移するシーンを変更する
std::shared_ptr<SceneBase> SceneSelect::SceneChange(const Pad& pad)
{
	//カーソルの位置によって、遷移するシーンを変更する
	//ステージ1に遷移する
	if (m_cursorPos.y == kStage1Pos && pad.IsTrigger("A") )
	{
		//SEの再生
		m_pSoundManager->DeterminationSE();
		m_isStage1 = true;
	}
	if(m_isStage1 && m_fadeAlpha >= kFadeValue)
	{
		return std::make_shared<SceneStage>(Stage1);
	}
	
	//buildのバージョンによって動作を変更している
	//Debug版の場合
	if (m_isDebug)
	{
		//ステージ2に遷移する
		if (m_cursorPos.y == kStage2Pos && pad.IsTrigger("A"))
		{
			//SEの再生
			m_pSoundManager->DeterminationSE();
			m_isStage2 = true;
		}
		if (m_isStage2 && m_fadeAlpha >= kFadeValue)
		{
			return std::make_shared<SceneStage>(Stage2);
		}
	}
	//Release版の場合
	else
	{
		//ステージ2に遷移する
		if (!m_isPishAButton && m_cursorPos.y == kStage2Pos && pad.IsTrigger("A"))
		{
			//SEの再生
			m_pSoundManager->FailureSE();
			m_isCautionaryNote = true;
		}
	}


	//ステータスに遷移する
	if (m_cursorPos.y == kStatusPos && pad.IsTrigger("A"))
	{
		//SEの再生
		m_pSoundManager->DeterminationSE();
		m_isStatus = true;
	}
	if (m_isStatus && m_fadeAlpha >= kFadeValue)
	{
		return std::make_shared<SceneStatus>();
	}

	//操作説明画面を表示する
	if (!m_isPishAButton && m_cursorPos.y == kExplanationPos && pad.IsTrigger("A"))
	{
		m_isExplanation = true;
	}

	//ゲームを終了する
	if (m_cursorPos.y == kGameEndPos && pad.IsTrigger("A"))
	{
		DxLib_End();

		return 0;
	}

	//操作説明を描画するか
	if (m_isExplanation  && pad.IsRelase("A"))
	{
		m_isPishAButton = true;
	}

	//注意書き画面を描画するか
	if (m_isCautionaryNote && pad.IsRelase("A"))
	{
		m_isPishAButton = true;
	}

	if (m_isPishAButton && pad.IsTrigger("A"))
	{
		m_isCautionaryNote = false;
		m_isExplanation = false;
		m_isPishAButton = false;
	}

	return shared_from_this();
}

//フェードイン、フェードアウト
void SceneSelect::Fade()
{
	if (!m_isStage1 && !m_isStage2 && !m_isStatus)
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
	
}


