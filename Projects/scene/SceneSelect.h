#pragma once
#include "SceneBase.h"

class SoundManager;
class SceneSelect :public SceneBase
{
public:
	SceneSelect();
	virtual ~SceneSelect();

	virtual void Init();

	virtual std::shared_ptr<SceneBase> Update(const Pad& pad);

	virtual void Draw();

	virtual void End();

private:

	/// <summary>
	/// カーソルの動き
	/// </summary>
	/// <param name="pad"></param>
	void CursorMotion(const Pad& pad);

	/// <summary>
	/// 選択できる文字を描画する
	/// </summary>
	void DrawCharacter();

	/// <summary>
	/// 選択肢が動く
	/// </summary>
	void MoveChatracter();

	/// <summary>
	/// ステージの説明を描画する
	/// </summary>
	void DrawStageExplanation();

	/// <summary>
	/// 説明文を描画する
	/// </summary>
	void DrawExplanation();
	
	/// <summary>
	/// カーソルの位置によって遷移するシーンを変更する
	/// </summary>
	/// <param name="pad"></param>
	/// <returns></returns>
	std::shared_ptr<SceneBase>SceneChange(const Pad& pad);

	/// <summary>
	/// フェードイン、フェードアウト
	/// </summary>
	void Fade();

private:

	//ステージ1文字のハンドル
	int m_stage1Handle;

	//ステージ2文字のハンドル
	int m_stage2Handle;

	//ステータス文字のハンドル
	int m_statusHandle;

	//操作方法文字のハンドル
	int m_operationHandle;

	//ゲーム終了文字のハンドル
	int m_gameEndHandle;

	//ステージ1の説明付き画面のハンドル
	int m_stage1DescriptionHandle;

	//ステージ2の説明付き画面のハンドル
	int m_stage2DescriptionHandle;

	//ステータスの説明付き画面のハンドル
	int m_statusDescriptionHandle;

	//操作説明の説明付き画面のハンドル
	int m_operationDescriptionHandle;

	//操作説明画面
	int m_operationInstructionsHandle;

	//注意書き画面
	int m_cautionaryNoteHandle;

	/// <summary>
	/// カウント
	/// </summary>
	int m_count;

	/// <summary>
	/// スティックをどれだけの時間倒しているか
	/// </summary>
	int m_cursorCount;

	/// <summary>
	/// バージョンがデバッグかどうか
	/// </summary>
	bool m_isDebug;

	/// <summary>
	/// ステージ1に遷移するかどうか
	/// </summary>
	bool m_isStage1;

	/// <summary>
	/// ステージ2に遷移するかどうか
	/// </summary>
	bool m_isStage2;

	/// <summary>
	/// ステータス画面に遷移するかどうか
	/// </summary>
	bool m_isStatus;

	/// <summary>
	/// 操作説明画面を描画するかどうか
	/// </summary>
	bool m_isExplanation;

	/// <summary>
	/// 注意書き画面を描画するかどうか
	/// </summary>
	bool m_isCautionaryNote;

	/// <summary>
	/// ボタンを押したかどうか
	/// </summary>
	bool m_isPishAButton;

	/// <summary>
	/// スティックを倒しているかどうか
	/// </summary>
	bool m_isPressPad;

	/// <summary>
	/// スティックを一定時間倒した
	/// </summary>
	bool m_isPressPadTime;

	//カーソルの位置
	VECTOR m_cursorPos;

	//ステージ1の説明文の位置
	VECTOR m_stage1DescriptionPos;

	//ステージ2の説明文の位置
	VECTOR m_stage2DescriptionPos;

	//ステータスの説明文の位置
	VECTOR m_statusDescriptionPos;

	//操作説明の説明文の位置
	VECTOR m_operationDescriptionPos;

	//ステージ1の文字位置
	VECTOR m_stage1Pos;

	//ステージ2の文字位置
	VECTOR m_stage2Pos;

	//ステータスの文字位置
	VECTOR m_statusPos;

	//操作説明の文字位置
	VECTOR m_explanationPos;

	//ゲーム終了の文字位置
	VECTOR m_gameEndPos;
};

