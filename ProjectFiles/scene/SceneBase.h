#pragma once
#include <memory>
#include "DxLib.h"

class SoundManager;
class Pad;

class SceneBase : public std::enable_shared_from_this<SceneBase>
{
public:
	//ステージの種類
	enum Stagekinds
	{
		Stage1,
		Stage2,
	};
public:
	SceneBase();
	virtual ~SceneBase();

	virtual void Init() = 0;	//シーンに入るときの初期化処理

	//virtual SceneBase* Update() = 0;	//シーンを変更する場合は遷移先のシーンのポインタ
										//シーン変更しない場合は自身のポインタ

	virtual std::shared_ptr<SceneBase>Update(const Pad& pad) = 0;	//シーンを変更する場合は遷移先のシーンのポインタ
	//シーン変更しない場合は自身のポインタ

	virtual void Draw() = 0;	//毎フレーム行う描画処理
	virtual void End() = 0;		//シーンを抜けるときの処理

protected:

	std::shared_ptr<SoundManager> m_pSoundManager;

	//画像のハンドル
	int m_bgHandle;

	//フェードイン、アウト
	int m_fadeAlpha;

	//プレイヤーの座標
	VECTOR m_playerPos;
};

