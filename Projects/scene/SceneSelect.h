#pragma once
#include "SceneBase.h"

class Stage;
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
	/// タイトル画面に戻る
	/// </summary>
	void BackToTitle(const Pad& pad);

	/// <summary>
	///	矢印を描画する
	/// </summary>
	void DrawArrow();

	/// <summary>
	/// ステージを選ぶ際の処理
	/// </summary>
	void StageSelect(const Pad& pad);

	/// <summary>
	/// 説明文を描画する
	/// </summary>
	void DrawExplanation();

	/// <summary>
	/// メニュー画面を描画する
	/// </summary>
	void DrawMenu();

	/// <summary>
	/// 選択ステージによって、再生する動画を変更する
	/// </summary>
	void DrawStageMovie();

	/// <summary>
	/// 動画を繰り返し流す
	/// </summary>
	void RepeatVideo();

	/// <summary>
	/// カーソルの位置によって遷移するシーンを変更する
	/// </summary>
	std::shared_ptr<SceneBase>SceneChange(const Pad& pad);

	/// <summary>
	/// フェードイン、フェードアウト
	/// </summary>
	void Fade();

private:

	std::shared_ptr<Stage>m_pStage;

	//セレクト画面のUIハンドル
	int m_uiHandle;

	//操作説明画面
	int m_operationInstructionsHandle;

	//メニュー画面
	int m_menuHandle;

	//ステージ1の動画
	int m_firstStageVideoHandle;

	//ステージ2の動画
	int m_secondStageVideoHandle;

	/// <summary>
	/// カウント
	/// </summary>
	int m_count;

	/// <summary>
	/// スティックをどれだけの時間倒しているか
	/// </summary>
	int m_cursorCount;

	//カーソル位置
	float m_cursorPosY;

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

	/// <summary>
	/// スティックを上に倒したかどうか
	/// </summary>
	bool m_isPressUp;

	/// <summary>
	/// スティックを下に倒したかどうか
	/// </summary>
	bool m_isPressDown;


	/// <summary>
	/// Aボタンを一時的に無効化するかどうか
	/// </summary>
	bool m_ignoreNextAInput;

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

