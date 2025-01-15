#pragma once

#include "DxLib.h"

class Player;
class Camera
{
public:
	Camera();
	virtual ~Camera();

    void Init();

	void Update(VECTOR mplayerPos);

    void Draw();

    //カメラのアングルを取得する
    float GetCameraAngle() const noexcept { return m_cameraAngle; }
private:

    //ジョイパッドの出力
    DINPUT_JOYSTATE m_input;

    //ジョイパッドの出力の保持
    int m_pad;

    //カメラアングルの位置
    float m_cameraAngle;

    //カメラの座標
    VECTOR m_cameraPos;

    //カメラの注視点
    VECTOR m_cameraTarget;

    //プレイヤーの座標
    VECTOR m_playerTarget;

  
};

