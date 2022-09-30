//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Map2D.h"

namespace  Map2D
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->MapChip = DG::Image::Create("./data/image/mapchip.png");
		this->TimeLimit = DG::Image::Create("./data/image/Time.png");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
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

			//�}�b�v�̃[���N���A
		for (int y = 0; y < 9; ++y) {
			for (int x = 0; x < 15; ++x) {
				arr[y][x] = 0;
			}
		}

		//�}�b�v�`�b�v���̏�����
		for (int c = 0; c < 4 * 4; ++c)
		{
			chip[c] = ML::Box2D((c % 4) * 32, (c / 4) * 32, 32, 32);
		}
		//�t�@�C���p�X�����

		string fliePath = "./data/map/Map" + to_string(ge->stage_num) + ".txt";
		//�t�@�C�����J���i�ǂݍ��݁j
		ifstream fin(fliePath);
		if (!fin) { return false; }//�ǂݍ��ݎ��s
		//�z��Ƀf�[�^��ǂݍ���
		for (int y = 0; y < 9; ++y)
		{
			for (int x = 0; x < 15; ++x)
			{
				fin >> arr[y][x];
			}
		}
		//�t�@�C�������
		fin.close();
		return true;

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
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		for (int y = 0; y < 9; ++y)
		{
			for (int x = 0; x < 15; ++x)
			{
				if (arr[y][x] != -1) {
					ML::Box2D  draw(x * 32, y * 32, 32, 32);
					int  num = arr[y][x];	//���݂̃}�X�̃`�b�v�ԍ����擾
					ML::Box2D  src = chip[num];	//�`�b�v�ԍ�����]�����摜�͈͂𓾂�
				//draw.Offset(x * 32, y * 32);
					this->res->MapChip->Draw(draw, src);
				}
			}
		}

		//�^�C���`��
		//1�̈�
		ML::Box2D draw(438, 32, 16, 32);
		ML::Box2D src(16 * One, 0, 16, 32);
		this->res->TimeLimit->Draw(draw, src);
		//10�̈�
		ML::Box2D draw10(420, 32, 16, 32);
		ML::Box2D src10(16 * Ten, 0, 16, 32);
		this->res->TimeLimit->Draw(draw10, src10);
	}

	//-------------------------------------------------------------------
//�}�b�v�̓����蔻��
	bool Object::Map_CheckHit(const ML::Box2D& h_)
	{
		ML::Rect r = { h_.x, h_.y, h_.x + h_.w, h_.y + h_.h };
		//��`���}�b�v�O�ɏo�����̑Ή�
		//������

		//���[�v�͈͒���
		int sx, sy, ex, ey;
		sx = r.left / 32;
		sy = r.top / 32;
		ex = (r.right - 1) / 32;
		ey = (r.bottom - 1) / 32;

		//�͈͓��̏�Q����T��
		for (int y = sy; y <= ey; ++y) {
			for (int x = sx; x <= ex; ++x) {
				if (0 < arr[y][x]) {
					return true;
				}
			}
		}
		return false;
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