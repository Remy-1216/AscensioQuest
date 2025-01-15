#pragma once
#include "DxLib.h"
// 矩形の座標を管理するためのクラス
class Rect
{
public:
	Rect();
	virtual ~Rect();

	//四角の描画
	void Draw(unsigned int Color, bool isFill);

	//中心座標と幅高さを指定
	void SetCenter(float x, float y, float z,float width, float height, float depth);

	//矩形の同士の当たり判定
	bool IsCollsion(const Rect& rect);

	//左上の座標
	VECTOR pos1;
	//左下の座標
	VECTOR pos2;
	//右上の座標
	VECTOR pos3;
	//右下の座標
	VECTOR pos4;
	//後ろの左上
	VECTOR pos5;
	//後ろの左下
	VECTOR pos6;
	//後ろの右上
	VECTOR pos7;
	//後ろの右下
	VECTOR pos8;

private:
	//代入するための変数
	float m_x;
	float m_y;
	float m_z;
	float m_xWidth;
	float m_yHeight;
	float m_zDepth;
};
