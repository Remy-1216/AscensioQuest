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

	//ステージ1の説明付き画面のハンドル
	int m_stage1Handle;

	//ステージ2の説明付き画面のハンドル
	int m_stage2Handle;

	//ステータスの説明付き画面のハンドル
	int m_statusHandle;

	//ゲーム終了の説明付き画面のハンドル
	int m_gameEndHandle;

	//操作説明の説明付き画面のはハンドル
	int m_explanationHandle;

	//カーソルのハンドル
	int m_cursorHandle;

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

	//カーソルの位置
	VECTOR m_cursorPos;

};

