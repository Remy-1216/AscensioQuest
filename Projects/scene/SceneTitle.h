#pragma once
#include "SceneBase.h"

class TitlePlayer;
class PlayerStatus;
class Stage;
class SceneTitle : public SceneBase
{
public:
	SceneTitle();
	virtual ~SceneTitle();

	virtual void Init();

	virtual std::shared_ptr<SceneBase> Update(const Pad& pad);

	virtual void Draw();

	virtual void End();
private:

	/// <summary>
	/// フェードイン、フェードアウト
	/// </summary>
	void Fade();

	/// <summary>
	/// カーソルの動き
	/// </summary>
	/// <param name="pad">パッド</param>
	void CursorMotion(const Pad& pad);

	/// <summary>
	/// タイトル画面に文字を表示する
	/// </summary>
	void DrawTitle();

private:

	std::shared_ptr<PlayerStatus> m_pPlayerStatus;

	std::shared_ptr<TitlePlayer> m_pTitlePlayer;

	std::shared_ptr<Stage>m_pStage;

	/// <summary>
	/// タイトルロゴのハンドル
	/// </summary>
	int m_logoHandle;

	/// <summary>
	/// カウント
	/// </summary>
	int m_count;

	/// <summary>
	/// スティックをどれだけの時間倒しているか
	/// </summary>
	int m_cursorCount;

	/// <summary>
	/// セレクト画面に行くかどうか
	/// </summary>
	bool m_isSelect;

	/// <summary>
	/// 新しくゲームを始めるかどうか
	/// </summary>
	bool m_isNewGame;

	/// <summary>
	/// ゲームをプレイするかどうか
	/// </summary>
	bool m_isPlayGame;

	/// <summary>
	/// スティックを倒しているかどうか
	/// </summary>
	bool m_isPressPad;

	/// <summary>
	/// スティックを一定時間倒した
	/// </summary>
	bool m_isPressPadTime;

	/// <summary>
	/// カーソルの位置
	/// </summary>
	VECTOR m_cursorPos;

	/// <summary>/// <summary>
	/// タイトル画面に表示する文字の位置
	/// </summary>
	VECTOR m_newGamePos;

	/// <summary>
	/// タイトル画面に表示する文字の位置
	/// </summary>
	VECTOR m_loadGamePos;

	/// <summary>
	/// タイトル画面に表示する文字の位置
	/// </summary>
	VECTOR m_exitGamePos;

};