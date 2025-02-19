#include "SceneStatus.h"
#include "PlayerStatus.h"
#include "SceneSelect.h"
#include "SoundManager.h"
#include "Pad.h"
#include "Game.h"
#include "Font.h"

namespace
{
	//カーソルの移動量
	constexpr float kCarsorMove = 130.0f;

	//カーソル位置
	constexpr float kHpUpPos = 0.0f;
	constexpr float kAttackUpPos = 130.0f;
	constexpr float kMagicAttackUpPos = 260.0f;
	constexpr float kDefenseOowerUpPos = 390.0f;
	constexpr float kUpdatePos = 520.0f;
	constexpr float kGameSelectPos = 650.0f;

	//フェードイン、フェードアウトの数値
	constexpr int kFadeValue = 255;

	//フェード値の増減
	constexpr int kFadeUpDown = 8;

	//描画する文字の位置
	constexpr float kPosX =600.0f;
	constexpr float kPosY = 100.0f;


	//強化回数
	constexpr int kReinforcementTimes1 = 1;
	constexpr int kReinforcementTimes2 = 2;
	constexpr int kReinforcementTimes3 = 3;
	constexpr int kReinforcementTimes4 = 4;
	constexpr int kReinforcementTimes5 = 5;
	constexpr int kReinforcementTimes6 = 6;
	constexpr int kReinforcementTimes7= 7;
	constexpr int kReinforcementTimes8= 8;
	constexpr int kReinforcementTimes9 = 9;
	constexpr int kReinforcementTimes10 = 10;

	//ズレを修正
	constexpr float kEdit = -4.0f;
}

SceneStatus::SceneStatus():m_statusUpBgHandle(-1), m_cursorHandle(-1), m_releasedGaugeHandle1(-1), m_releasedGaugeHandle2(-1), m_releasedGaugeHandle3(-1), m_releasedGaugeHandle4(-1), m_releasedGaugeHandle5(-1),
m_releasedGaugeHandle6(-1), m_releasedGaugeHandle7(-1), m_releasedGaugeHandle8(-1), m_releasedGaugeHandle9(-1), m_releasedGaugeHandle10(-1), m_isSelect(false), m_cursorPos(VGet(0.0f, 0.0f, 0.0f))
{
}

SceneStatus::~SceneStatus()
{
	//画像の削除
	DeleteGraph(m_bgHandle);
	DeleteGraph(m_statusUpBgHandle);
	DeleteGraph(m_cursorHandle);
	DeleteGraph(m_releasedGaugeHandle1);
	DeleteGraph(m_releasedGaugeHandle2);
	DeleteGraph(m_releasedGaugeHandle3);
	DeleteGraph(m_releasedGaugeHandle4);
	DeleteGraph(m_releasedGaugeHandle5);
	DeleteGraph(m_releasedGaugeHandle6);
	DeleteGraph(m_releasedGaugeHandle7);
	DeleteGraph(m_releasedGaugeHandle8);
	DeleteGraph(m_releasedGaugeHandle9);
	DeleteGraph(m_releasedGaugeHandle10);
	DeleteGraph(m_customBonusHandle);



}

void SceneStatus::Init()
{
	m_pPlayerStatus = std::make_shared<PlayerStatus>();
	m_pPlayerStatus->Init();

	m_pSoundManager->StatusBGM();

	//背景をロード
	m_bgHandle = LoadGraph("data/BG/StatusScene.png");
	
	//能力強化する位置にカーソルがあった場合の画像をロード
	m_statusUpBgHandle = LoadGraph("data/BG/StatusUp.png");

	//カーソルをロード
	m_cursorHandle = LoadGraph("data/UI/StatusCursor.png");

	//解放済みのゲージをロード
	m_releasedGaugeHandle1 = LoadGraph("data/UI/released.png");
	m_releasedGaugeHandle2 = LoadGraph("data/UI/released2.png");
	m_releasedGaugeHandle3 = LoadGraph("data/UI/released3.png");
	m_releasedGaugeHandle4 = LoadGraph("data/UI/released4.png");
	m_releasedGaugeHandle5 = LoadGraph("data/UI/released5.png");
	m_releasedGaugeHandle6 = LoadGraph("data/UI/released6.png");
	m_releasedGaugeHandle7 = LoadGraph("data/UI/released7.png");
	m_releasedGaugeHandle8 = LoadGraph("data/UI/released8.png");
	m_releasedGaugeHandle9 = LoadGraph("data/UI/released9.png");
	m_releasedGaugeHandle10 = LoadGraph("data/UI/released10.png");

	//カスタムボーナスの説明をロード
	m_customBonusHandle = LoadGraph("data/BG/custombonus.png");

	//フルカスタムボーナスの説明をロード
	m_fullCustomBonusHandle = LoadGraph("data/BG/fullCustomBonus.png");


	m_fadeAlpha = kFadeValue;

	m_isSelect = false;

	//カーソル位置の初期化
	m_cursorPos = VGet(0.0f, 0.0f, 0.0f);
}

std::shared_ptr<SceneBase> SceneStatus::Update(const Pad& pad)
{
	CursorMotion(pad);

	m_pPlayerStatus->CustomBonus(pad);

	return UpdateCursorPos(pad);
}

void SceneStatus::Draw()
{
	Fade();

	//背景の変更
	BackgroundChange();
	
	//カーソルの描画
	DrawGraph(0.0f, m_cursorPos.y, m_cursorHandle, true);

	//debug時に強化回数を描画する
	m_pPlayerStatus->Draw();

	//ゲージの描画
	ReleasedGaugeDraw();

	//スキルポイントのびょうが　
	DrawPoint();

	//カスタムボーナス説明の描画
	DrawCustomBonus();

	//フェードの描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha); //半透明で表示
	DrawBox(0, 0, Game::kScreenWindidth, Game::kScreenHeight, GetColor(0, 0, 0), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); //不透明に戻しておく
}

void SceneStatus::End()
{
}

void SceneStatus::Fade()
{
	//フェードイン、フェードアウト
	if (m_isSelect)
	{
		m_fadeAlpha += kFadeUpDown;
		if (m_fadeAlpha > kFadeValue)
		{
			m_fadeAlpha = kFadeValue;
		}
	}
	else
	{
		m_fadeAlpha -= kFadeUpDown;
		if (m_fadeAlpha < 0)
		{
			m_fadeAlpha = 0;
		}
	}
}

//カーソル移動の処理
void SceneStatus::CursorMotion(const Pad& pad)
{
	//カーソルの移動
	if (pad.IsTrigger("up"))
	{
		m_cursorPos.y -= kCarsorMove;
		
		//SEの再生
		m_pSoundManager->MoveCursorSE();
	}
	if (pad.IsTrigger("down"))
	{
		m_cursorPos.y += kCarsorMove;

		//SEの再生
		m_pSoundManager->MoveCursorSE();
	}

	//上限を超える一番下の項目に飛ぶ
	if (m_cursorPos.y < kHpUpPos)
	{
		m_cursorPos.y = kGameSelectPos;
	}

	//上限を超えると、一番上の項目に飛ぶ
	if (m_cursorPos.y > kGameSelectPos)
	{
		m_cursorPos.y = kHpUpPos;
	}
}

//強化段階ごとにゲージを増やす
void SceneStatus::ReleasedGaugeDraw()
{
	//HPUPの改造度
	if (m_pPlayerStatus->GetHpUpTimes() == kReinforcementTimes1)
	{
		DrawGraph(0.0f, 0.0f, m_releasedGaugeHandle1, true);
	}
	else if (m_pPlayerStatus->GetHpUpTimes() == kReinforcementTimes2)
	{
		DrawGraph(0.0f, 0.0f, m_releasedGaugeHandle2, true);
	}
	else if (m_pPlayerStatus->GetHpUpTimes() == kReinforcementTimes3)
	{
		DrawGraph(0.0f, 0.0f, m_releasedGaugeHandle3, true);
	}
	else if (m_pPlayerStatus->GetHpUpTimes() == kReinforcementTimes4)
	{
		DrawGraph(0.0f, 0.0f, m_releasedGaugeHandle4, true);
	}
	else if (m_pPlayerStatus->GetHpUpTimes() == kReinforcementTimes5)
	{
		DrawGraph(0.0f, 0.0f, m_releasedGaugeHandle5, true);
	}
	else if (m_pPlayerStatus->GetHpUpTimes() == kReinforcementTimes6)
	{
		DrawGraph(0.0f, 0.0f, m_releasedGaugeHandle6, true);
	}
	else if (m_pPlayerStatus->GetHpUpTimes() == kReinforcementTimes7)
	{
		DrawGraph(0.0f, 0.0f, m_releasedGaugeHandle7, true);
	}
	else if (m_pPlayerStatus->GetHpUpTimes() == kReinforcementTimes8)
	{
		DrawGraph(0.0f, 0.0f, m_releasedGaugeHandle8, true);
	}
	else if (m_pPlayerStatus->GetHpUpTimes() == kReinforcementTimes9)
	{
		DrawGraph(0.0f, 0.0f, m_releasedGaugeHandle9, true);
	}
	else if (m_pPlayerStatus->GetHpUpTimes() == kReinforcementTimes10)
	{
		DrawGraph(0.0f, 0.0f, m_releasedGaugeHandle10, true);
	}

	//攻撃力の改造度
	if (m_pPlayerStatus->GetAttackUpTimes() == kReinforcementTimes1)
	{
		DrawGraph(0.0f, kAttackUpPos- kEdit, m_releasedGaugeHandle1, true);
	}
	else if (m_pPlayerStatus->GetAttackUpTimes() == kReinforcementTimes2)
	{
		DrawGraph(0.0f, kAttackUpPos- kEdit, m_releasedGaugeHandle2, true);
	}
	else if (m_pPlayerStatus->GetAttackUpTimes() == kReinforcementTimes3)
	{
		DrawGraph(0.0f, kAttackUpPos- kEdit, m_releasedGaugeHandle3, true);
	}
	else if (m_pPlayerStatus->GetAttackUpTimes() == kReinforcementTimes4)
	{
		DrawGraph(0.0f, kAttackUpPos- kEdit, m_releasedGaugeHandle4, true);
	}
	else if (m_pPlayerStatus->GetAttackUpTimes() == kReinforcementTimes5)
	{
		DrawGraph(0.0f, kAttackUpPos-kEdit, m_releasedGaugeHandle5, true);
	}
	else if (m_pPlayerStatus->GetAttackUpTimes() == kReinforcementTimes6)
	{
		DrawGraph(0.0f, kAttackUpPos - kEdit, m_releasedGaugeHandle6, true);
	}
	else if (m_pPlayerStatus->GetAttackUpTimes() == kReinforcementTimes7)
	{
		DrawGraph(0.0f, kAttackUpPos - kEdit, m_releasedGaugeHandle7, true);
	}
	else if (m_pPlayerStatus->GetAttackUpTimes() == kReinforcementTimes8)
	{
		DrawGraph(0.0f, kAttackUpPos - kEdit, m_releasedGaugeHandle8, true);
	}
	else if (m_pPlayerStatus->GetAttackUpTimes() == kReinforcementTimes9)
	{
		DrawGraph(0.0f, kAttackUpPos - kEdit, m_releasedGaugeHandle9, true);
	}
	else if (m_pPlayerStatus->GetAttackUpTimes() == kReinforcementTimes10)
	{
		DrawGraph(0.0f, kAttackUpPos - kEdit, m_releasedGaugeHandle10, true);
	}

	//魔法攻撃の改造度
	if (m_pPlayerStatus->GetMagicAttackUpTimes() == kReinforcementTimes1)
	{
		DrawGraph(0.0f, kMagicAttackUpPos-kEdit, m_releasedGaugeHandle1, true);
	}
	else if (m_pPlayerStatus->GetMagicAttackUpTimes() == kReinforcementTimes2)
	{
		DrawGraph(0.0f, kMagicAttackUpPos-kEdit, m_releasedGaugeHandle2, true);
	}
	else if (m_pPlayerStatus->GetMagicAttackUpTimes() == kReinforcementTimes3)
	{
		DrawGraph(0.0f, kMagicAttackUpPos-kEdit, m_releasedGaugeHandle3, true);
	}
	else if (m_pPlayerStatus->GetMagicAttackUpTimes() == kReinforcementTimes4)
	{
		DrawGraph(0.0f, kMagicAttackUpPos-kEdit, m_releasedGaugeHandle4, true);
	}
	else if (m_pPlayerStatus->GetMagicAttackUpTimes() == kReinforcementTimes5)
	{
		DrawGraph(0.0f, kMagicAttackUpPos-kEdit, m_releasedGaugeHandle5, true);
	}
	else if (m_pPlayerStatus->GetMagicAttackUpTimes() == kReinforcementTimes6)
	{
		DrawGraph(0.0f, kMagicAttackUpPos - kEdit, m_releasedGaugeHandle6, true);
	}
	else if (m_pPlayerStatus->GetMagicAttackUpTimes() == kReinforcementTimes7)
	{
		DrawGraph(0.0f, kMagicAttackUpPos - kEdit, m_releasedGaugeHandle7, true);
	}
	else if (m_pPlayerStatus->GetMagicAttackUpTimes() == kReinforcementTimes8)
	{
		DrawGraph(0.0f, kMagicAttackUpPos - kEdit, m_releasedGaugeHandle8, true);
	}
	else if (m_pPlayerStatus->GetMagicAttackUpTimes() == kReinforcementTimes9)
	{
		DrawGraph(0.0f, kMagicAttackUpPos - kEdit, m_releasedGaugeHandle9, true);
	}
	else if (m_pPlayerStatus->GetMagicAttackUpTimes() == kReinforcementTimes10)
	{
		DrawGraph(0.0f, kMagicAttackUpPos - kEdit, m_releasedGaugeHandle10, true);
	}
	//	防御力の改造度
	if (m_pPlayerStatus->GetDefensePowerUpTimes() == kReinforcementTimes1)
	{
		DrawGraph(0.0f, kDefenseOowerUpPos-kEdit, m_releasedGaugeHandle1, true);
	}
	else if (m_pPlayerStatus->GetDefensePowerUpTimes() == kReinforcementTimes2)
	{
		DrawGraph(0.0f, kDefenseOowerUpPos-kEdit, m_releasedGaugeHandle2, true);
	}
	else if (m_pPlayerStatus->GetDefensePowerUpTimes() == kReinforcementTimes3)
	{
		DrawGraph(0.0f, kDefenseOowerUpPos-kEdit, m_releasedGaugeHandle3, true);
	}
	else if (m_pPlayerStatus->GetDefensePowerUpTimes() == kReinforcementTimes4)
	{
		DrawGraph(0.0f, kDefenseOowerUpPos-kEdit, m_releasedGaugeHandle4, true);
	}
	else if (m_pPlayerStatus->GetDefensePowerUpTimes() == kReinforcementTimes5)
	{
		DrawGraph(0.0f, kDefenseOowerUpPos-kEdit, m_releasedGaugeHandle5, true);
	}
	else if (m_pPlayerStatus->GetDefensePowerUpTimes() == kReinforcementTimes6)
	{
		DrawGraph(0.0f, kDefenseOowerUpPos - kEdit, m_releasedGaugeHandle6, true);
	}
	else if (m_pPlayerStatus->GetDefensePowerUpTimes() == kReinforcementTimes7)
	{
		DrawGraph(0.0f, kDefenseOowerUpPos - kEdit, m_releasedGaugeHandle7, true);
	}
	else if (m_pPlayerStatus->GetDefensePowerUpTimes() == kReinforcementTimes8)
	{
		DrawGraph(0.0f, kDefenseOowerUpPos - kEdit, m_releasedGaugeHandle8, true);
	}
	else if (m_pPlayerStatus->GetDefensePowerUpTimes() == kReinforcementTimes9)
	{
		DrawGraph(0.0f, kDefenseOowerUpPos - kEdit, m_releasedGaugeHandle9, true);
	}
	else if (m_pPlayerStatus->GetDefensePowerUpTimes() == kReinforcementTimes10)
	{
		DrawGraph(0.0f, kDefenseOowerUpPos - kEdit, m_releasedGaugeHandle10, true);
	}

}

//カーソル位置によって背景を変更する
void SceneStatus::BackgroundChange()
{
	if(m_cursorPos.y  >= kHpUpPos && m_cursorPos.y <= kDefenseOowerUpPos)
	{ 
		DrawGraph(0.0f, 0.0f, m_statusUpBgHandle, true);
	}
	else if (m_cursorPos.y >= kUpdatePos && m_cursorPos.y <= kGameSelectPos)
	{
		DrawGraph(0.0f, 0.0f, m_bgHandle, true);
	}
}

//ポイントの数値や、上限UPできるかを描画する
void SceneStatus::DrawPoint()
{
	DrawFormatStringFToHandle(kPosX, kPosY, GetColor(0, 0, 0), Font::m_fontHandle[static_cast<int>(Font::FontId::kSize64_4)], "ステータスポイント数:%4d", m_pPlayerStatus->GetStatusPoint());
}

//カスタムボーナス説明を描画する
void SceneStatus::DrawCustomBonus()
{
	if(m_pPlayerStatus->GetIsCustomBonus())
	{
		DrawGraph(0, 0, m_customBonusHandle, true);
	}


	if (m_pPlayerStatus->GetIsFullCustomBonus())
	{
		DrawGraph(0, 0, m_fullCustomBonusHandle, true);
	}
}

//カーソル位置で動きが変わります
std::shared_ptr<SceneBase>  SceneStatus::UpdateCursorPos(const Pad& pad)
{
	if (m_cursorPos.y == kHpUpPos && pad.IsTrigger("A"))
	{
		m_pPlayerStatus->HpUp();
	}
	if (m_cursorPos.y == kAttackUpPos && pad.IsTrigger("A"))
	{
		m_pPlayerStatus->AttackPowerUp();
	}
	if (m_cursorPos.y == kMagicAttackUpPos && pad.IsTrigger("A"))
	{
		m_pPlayerStatus->MagicAttackPowerUp();
	}
	if (m_cursorPos.y == kDefenseOowerUpPos && pad.IsTrigger("A"))
	{
		m_pPlayerStatus->DefensePowerUp();
	}
	if (m_cursorPos.y == kUpdatePos && pad.IsTrigger("A"))
	{
		m_pSoundManager->DeterminationSE();

		m_pPlayerStatus->StatOutput();
	}
	if (m_cursorPos.y == kGameSelectPos && pad.IsTrigger("A"))
	{
		m_isSelect = true;
	}
	if (m_isSelect && m_fadeAlpha >= kFadeValue)
	{
		return std::make_shared<SceneSelect>();
	}

	return shared_from_this();
}
