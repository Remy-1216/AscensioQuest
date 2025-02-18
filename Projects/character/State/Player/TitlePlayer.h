#pragma once
#include "CharacterBase.h"
#include "PlayerStateBase.h"
#include "DxLib.h"
#include <memory>

class TitlePlayer :public CharacterBase, public std::enable_shared_from_this<TitlePlayer>
{
public:

	TitlePlayer();

	virtual ~TitlePlayer();

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init();

	void Update(VECTOR cursorPos);

	void Draw();

private:

	/// <summary>
	/// カーソルの位置によってアニメーションを変更
	/// </summary>
	void ChangeAnimation(VECTOR cursorPos);

private:

	/// <summary>
	/// 一回だけアニメーションを変更するためのカウント
	/// </summary>
	int m_changeFistPumpAnimCount;
	int m_changeRaiseYourHandCount;
	int m_changeWaveHandAnimCount;


};

