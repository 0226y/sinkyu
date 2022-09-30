//-----------------------------------------------------------------------------
//キャラクタ汎用スーパークラス
//-----------------------------------------------------------------------------
#include "BChara.h"
#include  "MyPG.h"
#include "Task_Map2D.h"

ML::Vec2 BChara::MoveSet(int key)
{
	//0, 1, 2,  3,  4,  5,  6, 7, 8, 9,10,
	int table[] = { -1,90,270,-1,180,135,225,-1, 0,45,315 };
	float spd = 10.5f;
	ML::Vec2 est;
	//0は特殊条件
	if (key == 0)
	{
		est.x = 0.f;
		est.y = 0.f;
	}
	else
	{
		est.x = (float)cos(table[key] * D3DX_PI / 180.f) * spd;
		est.y = (float)-sin(table[key] * D3DX_PI / 180.f) * spd;
	}
	return est;
}
//-------------------------------------------------------------------
void  BChara::Chara_CheckMove(ML::Vec2& e_)
{
	hitFlag = false;	//移動判定の機能とは関係ない
	auto map = ge->GetTask< Map2D::Object>("マップ2D", "NoName");

	//横軸に対する移動
	while (e_.x != 0) {
		float  preX = pos.x;
		if (e_.x >= 1) { pos.x += 1;		e_.x -= 1; }
		else if (e_.x <= -1) { pos.x -= 1;		e_.x += 1; }
		else { pos.x += e_.x;	e_.x = 0; }
		ML::Box2D  hit = hitBase.OffsetCopy(pos.x, pos.y);
		if (true == map->Map_CheckHit(hit)) {
			pos.x = preX;		//移動をキャンセル
			hitFlag = true;
			break;
		}
	}
	//縦軸に対する移動
	while (e_.y != 0) {
		float  preY = pos.y;
		if (e_.y >= 1) { pos.y += 1;		e_.y -= 1; }
		else if (e_.y <= -1) { pos.y -= 1;		e_.y += 1; }
		else { pos.y += e_.y;	e_.y = 0; }
		ML::Box2D  hit = hitBase.OffsetCopy(pos.x, pos.y);
		if (true == map->Map_CheckHit(hit)) {
			pos.y = preY;		//移動をキャンセル
			break;
		}
	}
}

//-------------------------------------------------------------------
bool  BChara::Chara_CheckFoot()
{
	ML::Box2D  hit = footBase.OffsetCopy(pos.x, pos.y);
	auto map = ge->GetTask< Map2D::Object>("マップ2D", "NoName");
	return  map->Map_CheckHit(hit);
}

//-------------------------------------------------------------------
bool	BChara::Chara_CheckHead()
{
	ML::Box2D  hit = headBase.OffsetCopy(pos.x, pos.y);
	auto map = ge->GetTask< Map2D::Object>("マップ2D", "NoName");
	return  map->Map_CheckHit(hit);
}