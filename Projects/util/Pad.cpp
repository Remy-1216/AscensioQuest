#include "Pad.h"
#include "DxLib.h"
namespace
{
	//前のフレームのパッド押し下げ状態
	int lastPad = 0;
	//このフレームのパッド押し下げ状態
	int nowPad = 0;
}


void Pad::Update()
{
	// 直前の入力をコピー
	m_lastInputData = m_inputData;

	/*ハードウェア入力チェック*/
	// パッド情報の取得
	int padstate = GetJoypadInputState(DX_INPUT_PAD1);
	// 現在のキーボード入力を取得
	char keystate[256];
	GetHitKeyStateAll(keystate);

	// m_inputDataの内容を更新
	for (const auto& cmd : m_commandTable)
	{
		auto& input = m_inputData[cmd.first];	// コマンドの名前から入力データを作る
		for (const auto& hardIO : cmd.second)
		{
			// キーボードのチェック
			input = false;
			if (hardIO.first == InputType::keybd)
			{
				if (keystate[hardIO.second])
				{
					input = true;
					break;
				}
			}
			// パッドのチェック
			else if (hardIO.first == InputType::pad)
			{
				if (padstate & hardIO.second)
				{
					input = true;
					break;
				}
			}
		}
	}
}

/// <summary>
/// 今押しているかを取得
/// </summary>
/// <param name="command">コマンド文字列</param>
/// <returns>true:押されている</returns>
bool Pad::IsPress(const char* command)const
{
	auto it = m_inputData.find(command);

	// 要素がない場合
	if (it == m_inputData.end())
	{
		return false;
	}

	return m_inputData.at(command);
}

/// <summary>
/// 押された瞬間を取得
/// </summary>
/// <param name="command">コマンド文字列</param>
/// <returns>ture:押された瞬間</returns>
bool Pad::IsTrigger(const char* command)const
{
	auto it = m_inputData.find(command);

	// 要素がない場合
	if (it == m_inputData.end())
	{
		return false;
	}

	return(m_inputData.at(command) && !m_lastInputData.at(command));
}

/// <summary>
/// 離された瞬間を取得
/// </summary>
/// <param name="command">コマンド文字列</param>
/// <returns>true:離された瞬間</returns>
bool Pad::IsRelase(const char* command)const
{
	auto it = m_inputData.find(command);

	// 要素がない場合
	if (it == m_inputData.end())
	{
		return false;
	}

	return(!m_inputData.at(command) && m_lastInputData.at(command));
}
/// <summary>
/// コマンドの種類
/// </summary>
Pad::Pad()
{

	//キーボードと、コントローラーのコマンドの種類

	m_commandTable["A"] = { {InputType::keybd, KEY_INPUT_RETURN},
							{InputType::pad, PAD_INPUT_A} };

	m_commandTable["B"] = { {InputType::keybd, KEY_INPUT_B},
							{InputType::pad, PAD_INPUT_B} };

	m_commandTable["X"] = { {InputType::keybd, KEY_INPUT_X},
							{InputType::pad, PAD_INPUT_C} };

	m_commandTable["Y"] = { {InputType::keybd, KEY_INPUT_Y},
							{InputType::pad, PAD_INPUT_X} };

	m_commandTable["RB"] = { { InputType::keybd, KEY_INPUT_P },
									{InputType::pad, PAD_INPUT_Z } },

	m_commandTable["LB"] = { { InputType::keybd, KEY_INPUT_Q },
									{InputType::pad, PAD_INPUT_Y } },

	m_commandTable["RT"] = { { InputType::keybd, KEY_INPUT_O },
										{InputType::pad, PAD_INPUT_R } },	

	m_commandTable["LT"] = { { InputType::keybd, KEY_INPUT_U },
										{InputType::pad, PAD_INPUT_L } },

	m_commandTable["START"] = { {InputType::keybd, KEY_INPUT_SPACE},
									{InputType::pad, PAD_INPUT_8} };
	
	//キーボードと、コントローラーのスティックのコマンドの種類
	m_commandTable["up"] = { {InputType::keybd, KEY_INPUT_UP},
								 {InputType::pad, PAD_INPUT_UP} };

	m_commandTable["down"] = { {InputType::keybd, KEY_INPUT_DOWN},
							   {InputType::pad, PAD_INPUT_DOWN} };

	m_commandTable["left"] = { {InputType::keybd, KEY_INPUT_LEFT},
							   {InputType::pad, PAD_INPUT_LEFT} };

	m_commandTable["right"] = { {InputType::keybd, KEY_INPUT_RIGHT},
								{InputType::pad, PAD_INPUT_RIGHT} };
}
