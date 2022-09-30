//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Player.h"
#include "Task_Map2D.h"

namespace  Player
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->player = DG::Image::Create("./data/image/chara.png");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		this->player.reset();
		return true;
	}
	//-------------------------------------------------------------------
	//�u�������v�^�X�N�������ɂP�񂾂��s������
	bool  Object::Initialize()
	{
		//�X�[�p�[�N���X������
		__super::Initialize(defGroupName, defName, true);
		//���\�[�X�N���X����or���\�[�X���L
		this->res = Resource::Create();

		//���f�[�^������
		this->hitBase = ML::Box2D(-8, -16, 16, 32);
		this->footBase = ML::Box2D(hitBase.x, hitBase.h + hitBase.y, hitBase.w, 1);
		this->headBase = ML::Box2D(hitBase.x, hitBase.y - 1, hitBase.w, 1);
		this->jumpPow = -6.0f;
		this->state = State::Normal;

		//���^�X�N�̐���

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���


		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//�������p���^�X�N�̐���
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		auto inp = ge->in1->GetState();
		
		//����
		ML::Vec2  est = { 0, 0 };
		if (this->state == State::Normal)
		{
			if (inp.LStick.BR.on) { est.x += 2; animDir = DIR::RIGHT; }//D�������ꂽ��E�ɐi��
			if (inp.LStick.BL.on) { est.x -= 2; animDir = DIR::LEFT; }//A�������ꂽ��E�ɐi��
			if (inp.S0.down || inp.LStick.BU.down)
			{
				if (true == hitFlag)//���n���̂݃W�����v�J�n�ł��� 
				{
					this->fallSpeed = this->jumpPow;
				}
			}
		}
		
		est.y += this->fallSpeed;
		Chara_CheckMove(est);
		if (this->pos.x <= 10)
		{
			this->pos.x = 10;
		}
		if (this->pos.x >= ge->screen2DWidth - 10)
		{
			this->pos.x = ge->screen2DWidth - 10;
		}

		//���㔻��
		if (this->fallSpeed < 0)//�㏸��
		{
			if (Chara_CheckHead())
			{
				this->fallSpeed = 0;//�㏸�͂𖳌��ɂ���
			}
		}

		//�����ڐG����
		this->hitFlag = Chara_CheckFoot();
		if (true == this->hitFlag) {
			this->fallSpeed = 0;//�������x0
		}
		else {
			this->fallSpeed += ML::Gravity(32) * 3;//�d�͉���
		}

		//�A�j��
		int AnimFrame = 12;//1�R�}�\������t���[����
		int AnimMax = 4;//�A�j���̍ő�R�}��
		//1�R�}�\������t���[�����𒴂���
		if (this->animCnt >= AnimFrame)
		{
			this->animIndex++;//�A�j����1�R�}�i�߂�
			this->animCnt = 0;//�J�E���g��0�ɖ߂�
			//�A�j���ő�R�}���𒴂���
			if (this->animIndex >= AnimMax)
			{
				this->animIndex = 0;//�\���R�}����0�ɂ���
			}
		}
		this->animCnt++;

		auto map = ge->GetTask< Map2D::Object>("�}�b�v2D", "NoName");
		map->Map_CheckHit(this->hitBase);
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		int animTbl[3][4] =
		{
			{0,1,2,1},//CENTER
			{3,4,5,4},//LEFT
			{6,7,8,7},//RIGHT
		};
		int animX = this->animIndex;   //�e�[�u��x�C���f�b�N�X
		int animY = (int)this->animDir;//�e�[�u��y�C���f�b�N�X
		int animNo = animTbl[animY][animX];//�\������A�j��No

		int srcX = (animNo % 3) * 32; //�摜���ǂ̈ʒu����\�����n�߂邩
		int srcY = (animNo / 3) * 32;

		ML::Box2D draw(-16, -16, 32, 32);
		draw.Offset(this->pos);
		ML::Box2D src(srcX, srcY, 32, 32);
		this->res->player->Draw(draw, src);
	}

	//������������������������������������������������������������������������������������
	//�ȉ��͊�{�I�ɕύX�s�v�ȃ��\�b�h
	//������������������������������������������������������������������������������������
	//-------------------------------------------------------------------
	//�^�X�N��������
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//�Q�[���G���W���ɓo�^
				
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//�C�j�V�����C�Y�Ɏ��s������Kill
			}
			return  ob;
		}
		return nullptr;
	}
	//-------------------------------------------------------------------
	bool  Object::B_Initialize()
	{
		return  this->Initialize();
	}
	//-------------------------------------------------------------------
	Object::~Object() { this->B_Finalize(); }
	bool  Object::B_Finalize()
	{
		auto  rtv = this->Finalize();
		return  rtv;
	}
	//-------------------------------------------------------------------
	Object::Object() {	}
	//-------------------------------------------------------------------
	//���\�[�X�N���X�̐���
	Resource::SP  Resource::Create()
	{
		if (auto sp = instance.lock()) {
			return sp;
		}
		else {
			sp = Resource::SP(new  Resource());
			if (sp) {
				sp->Initialize();
				instance = sp;
			}
			return sp;
		}
	}
	//-------------------------------------------------------------------
	Resource::Resource() {}
	//-------------------------------------------------------------------
	Resource::~Resource() { this->Finalize(); }
}