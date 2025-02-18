#include "TitlePlayer.h"
#include "LoadCsv.h"

namespace
{
	//モデルのサイズ変更
	constexpr float kSize = 1.25f;

	//モデルの初期位置
	constexpr float kPosX = 60.0f;
	constexpr float kPosY = 180.0f;
	constexpr float kPosZ = 60.0f;

	//はじめから始める位置
	constexpr float kStartPos = 0.0f;

	//続きから始める位置
	constexpr float kContinuePos = 135.0f;

	//ゲーム終了する位置
	constexpr float kGameEndPos = 270.0f;

}

TitlePlayer::TitlePlayer() :CharacterBase(m_handle), m_changeFistPumpAnimCount(0), m_changeRaiseYourHandCount(0), m_changeWaveHandAnimCount(0)
{
	m_handle = MV1LoadModel("data/model/player/TitlePlayer.mv1");



	//アニメーションのデータを読み込む
	LoadCsv::GetInstance().LoadPlayerAnimData(m_animData);

}

TitlePlayer::~TitlePlayer()
{
}

void TitlePlayer::Init()
{
	// モデルのサイズを変更
	MV1SetScale(m_handle, VGet(kSize, kSize, kSize));

	//モデルの位置
	m_pos = VGet(kPosX, kPosY, kPosZ);
}

void TitlePlayer::Update(VECTOR cursorPos)
{
	//アニメーションの更新
	UpdateAnim();

	//カーソルの位置によってアニメーションを変更
	ChangeAnimation(cursorPos);

	//モデルの位置
	MV1SetPosition(m_handle, m_pos);
}

void TitlePlayer::Draw()
{
	//モデルの表示
	MV1DrawModel(m_handle);
}

void TitlePlayer::ChangeAnimation(VECTOR cursorPos)
{
	//カーソルの位置によってアニメーションを変更
	if (cursorPos.y == kStartPos)
	{
		m_changeWaveHandAnimCount = 0;
		m_changeRaiseYourHandCount = 0;

		if (m_changeFistPumpAnimCount == 0)
		{
			ChangeAnim("FistPump");
			m_changeFistPumpAnimCount++;
		}
		
	}

	if ( cursorPos.y == kContinuePos)
	{
		m_changeFistPumpAnimCount = 0;
		m_changeWaveHandAnimCount = 0;

		if (m_changeRaiseYourHandCount == 0)
		{
			ChangeAnim("RaiseYourHand");
			m_changeRaiseYourHandCount++;
		}
		
	}


	if (cursorPos.y == kGameEndPos)
	{
		m_changeFistPumpAnimCount = 0;
		m_changeRaiseYourHandCount = 0;

		if (m_changeWaveHandAnimCount == 0)
		{
			ChangeAnim("WaveHand");
			m_changeWaveHandAnimCount++;
		}
	}
}
