#pragma once
#include <array>
#include <map>
#include<unordered_map>
#include<string>

enum class InputType
{
	keybd,	// キーボード
	pad,	// パッド
	mouse	// マウス
};

//パッドの状態を取得する
class Pad
{
public:

	//コマンドの書類
	Pad();

	//パッドの更新処理　1フレームに１回行う
	void Update();

	//押されているか処理
	bool IsPress(const char* command)const;
	//押された瞬間を取得
	bool IsTrigger(const char* command)const;
	//離した瞬間の処理
	bool IsRelase(const char* command)const;


	// コマンドの種類
	using InputTable_t = std::unordered_map<std::string, std::map<InputType, int>>;

private:
	// コマンド名と入力をペアにしたテーブル
	InputTable_t m_commandTable;

	// コマンドの入力を覚えておく
	std::map<std::string, bool> m_inputData;		// 現在の入力
	std::map<std::string, bool> m_lastInputData;	// 直前の入力

};

