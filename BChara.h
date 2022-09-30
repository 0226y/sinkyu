#pragma once
#pragma warning(disable:4996)
#pragma once
//-----------------------------------------------------------------------------
//�L�����N�^�ėp�X�[�p�[�N���X
//-----------------------------------------------------------------------------
#include "GameEngine_Ver3_83.h"

class BChara : public BTask
{
	//�ύX�s����������������������������������������������������
public:
	typedef shared_ptr<BChara>		SP;
	typedef weak_ptr<BChara>		WP;
public:
	//�ύX������������������������������������������������������
	//�L�����N�^���ʃ����o�ϐ�
	enum class DIR {
		CENTER, //�^�� 
		LEFT,   //��
		RIGHT   //�E
	};
	enum class State {
		Live,  //�����Ă�
		DeaD,  //����ł�
		Normal,//�ʏ�
		Non,   //���Ȃ�
	};
	State state;
	//�����i2D���_�j
	float angle;
	ML::Vec2    pos;		//�L�����N�^�ʒu
	ML::Box2D   hitBase;	//�����蔻��͈�
	ML::Vec2	moveVec;	//�ړ��x�N�g��
	int			moveCnt;	//�s���J�E���^
	int frame;				//�t���[���Ǘ�
	int Ten;				//10�̈ʊǗ�
	int One;				//1�̈ʊǗ�
	int time;               //�b�Ǘ�        
	int	logoTimes;          //stageLogo_Task�ɂ������񐔂��i�[
	bool hitFlag;
	ML::Box2D footBase;
	ML::Box2D headBase;
	float fallSpeed;
	float jumpPow;
	DIR animDir;//�L�����N�^�̌���
	int animCnt;//�A�j���[�V���������p�J�E���^
	int animIndex;

	//�����o�ϐ��ɍŒ���̏��������s��
	//���������o�ϐ���ǉ�������K�����������ǉ����鎖����
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
		, animCnt(0)//�A�j���[�V���������p�J�E���^
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
