//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Stage_logo.h"
#include "Task_Game.h"
#include "Task_Map2D.h"
#include "Task_Player.h"

namespace  Stage_logo
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->back = DG::Image::Create("./data/image/LogoBack.png");
		this->number[0] = DG::Image::Create("./data/image/numbers/number_1.png");
		this->number[1] = DG::Image::Create("./data/image/numbers/number_2.png");
		this->number[2] = DG::Image::Create("./data/image/numbers/number_3.png");
		this->stage = DG::Image::Create("./data/image/stage.png");
		this->stock = DG::Image::Create("./data/image/stock.png");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		this->back.reset();
		this->number[0].reset();
		this->number[1].reset();
		this->number[2].reset();
		this->stage.reset();
		this->stock.reset();
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
		this->frame = 0;
		
		//���^�X�N�̐���

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		ge->KillAll_G("�X�e�[�W���S");

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			Game::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		this->frame++;
		if (this->frame == 60)
			
		{
			this->frame = 61;
			ge->StartCounter("test", 45); //�t�F�[�h��90�t���[���Ȃ̂Ŕ�����45�Ő؂�ւ�
			ge->CreateEffect(98, ML::Vec2(0, 0));
		}
		if (ge->getCounterFlag("test") == ge->LIMIT) {
			this->Kill();
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		//�w�i
		ML::Box2D draw_b(0, 0, ge->screen2DWidth, ge->screen2DHeight);
		ML::Box2D src_b(0, 0, 480, 270);
		this->res->back->Draw(draw_b, src_b);

		//�X�e�[�W�\��
		ML::Box2D draw_stage(50, 50, 277, 84);
		ML::Box2D src_stage(0, 0, 277, 84);
		this->res->stage->Draw(draw_stage, src_stage);

		//�X�g�b�N�\��
		ML::Box2D draw_stock(50, 150, 285, 84);
		ML::Box2D src_stock(0, 0, 285, 84);
		this->res->stock->Draw(draw_stock, src_stock);

		//0,1,2�\��
		ML::Box2D stage_draw_num(335, 50, 56, 84);
		ML::Box2D stock_draw_num(335, 150, 56, 84);
		ML::Box2D src_num(0, 0, 56, 84);

		//���݂̃X�e�[�W�i���o�[
		this->res->number[ge->stage_num]->Draw(stage_draw_num, src_num);
		//���݂̃X�g�b�N��
		this->res->number[ge->stock_num -1]->Draw(stock_draw_num, src_num);
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