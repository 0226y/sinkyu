#pragma once
#pragma warning(disable:4996)
#pragma once
//-----------------------------------------------------------------------------
//キャラクタ汎用スーパークラス
//-----------------------------------------------------------------------------
#include "GameEngine_Ver3_83.h"

class BChara : public BTask
{
	//変更不可◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆
public:
	typedef shared_ptr<BChara>		SP;
	typedef weak_ptr<BChara>		WP;
public:
	//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
	//キャラクタ共通メンバ変数
	enum class DIR {
		CENTER, //真ん中 
		LEFT,   //左
		RIGHT   //右
	};
	enum class State {
		Live,  //生きてる
		DeaD,  //死んでる
		Normal,//通常
		Non,   //いない
	};
	State state;
	//向き（2D視点）
	float angle;
	ML::Vec2    pos;		//キャラクタ位置
	ML::Box2D   hitBase;	//あたり判定範囲
	ML::Vec2	moveVec;	//移動ベクトル
	int			moveCnt;	//行動カウンタ
	int frame;				//フレーム管理
	int Ten;				//10の位管理
	int One;				//1の位管理
	int time;               //秒管理        
	int	logoTimes;          //stageLogo_Taskにいった回数を格納
	bool hitFlag;
	ML::Box2D footBase;
	ML::Box2D headBase;
	float fallSpeed;
	float jumpPow;
	DIR animDir;//キャラクタの向き
	int animCnt;//アニメーション処理用カウンタ
	int animIndex;

	//メンバ変数に最低限の初期化を行う
	//★★メンバ変数を追加したら必ず初期化も追加する事★★
	BChara()
		: pos(0, 0)
		, hitBase(0, 0, 0, 0)
		, moveVec(0, 0)
		, moveCnt(0)
		, angle(0.f)
		, animDir(DIR::CENTER)
		, frame(0)
		, time(0)
		, One(0)
		, Ten(0)
		, logoTimes(0)
		, hitFlag(false)
		, footBase(0, 0, 0, 0)
		, headBase(0, 0, 0, 0)
		, fallSpeed(0)
		, jumpPow(0)
		, animCnt(0)//アニメーション処理用カウンタ
		, animIndex(0)
	{
	}
	virtual  ~BChara() {}

	void  Chara_CheckMove(ML::Vec2& e_);
	bool  Chara_CheckFoot();
	bool Chara_CheckHead();

protected:
	ML::Vec2 MoveSet(int key);
};
