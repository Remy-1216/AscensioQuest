#pragma once
#include "SceneBase.h"

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

private:

	/// <summary>
	/// セレクト画面に行くかどうか
	/// </summary>
	bool m_isSelect;
};

