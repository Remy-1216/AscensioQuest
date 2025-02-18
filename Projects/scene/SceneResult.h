#pragma once
#include "SceneBase.h"
class SceneResult :public SceneBase
{
public:
	SceneResult();
	~SceneResult();

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
	/// タイトルに戻るかどうか
	/// </summary>
	bool m_isTitle;


};

