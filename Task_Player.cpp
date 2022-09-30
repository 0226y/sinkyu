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
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->player = DG::Image::Create("./data/image/chara.png");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		this->player.reset();
		return true;
	}
	//-------------------------------------------------------------------
	//「初期化」タスク生成時に１回だけ行う処理
	bool  Object::Initialize()
	{
		//スーパークラス初期化
		__super::Initialize(defGroupName, defName, true);
		//リソースクラス生成orリソース共有
		this->res = Resource::Create();

		//★データ初期化
		this->hitBase = ML::Box2D(-8, -16, 16, 32);
		this->footBase = ML::Box2D(hitBase.x, hitBase.h + hitBase.y, hitBase.w, 1);
		this->headBase = ML::Box2D(hitBase.x, hitBase.y - 1, hitBase.w, 1);
		this->jumpPow = -6.0f;
		this->state = State::Normal;

		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放


		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		auto inp = ge->in1->GetState();
		
		//挙動
		ML::Vec2  est = { 0, 0 };
		if (this->state == State::Normal)
		{
			if (inp.LStick.BR.on) { est.x += 2; animDir = DIR::RIGHT; }//Dを押されたら右に進む
			if (inp.LStick.BL.on) { est.x -= 2; animDir = DIR::LEFT; }//Aを押されたら右に進む
			if (inp.S0.down || inp.LStick.BU.down)
			{
				if (true == hitFlag)//着地中のみジャンプ開始できる 
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

		//頭上判定
		if (this->fallSpeed < 0)//上昇中
		{
			if (Chara_CheckHead())
			{
				this->fallSpeed = 0;//上昇力を無効にする
			}
		}

		//足元接触判定
		this->hitFlag = Chara_CheckFoot();
		if (true == this->hitFlag) {
			this->fallSpeed = 0;//落下速度0
		}
		else {
			this->fallSpeed += ML::Gravity(32) * 3;//重力加速
		}

		//アニメ
		int AnimFrame = 12;//1コマ表示するフレーム数
		int AnimMax = 4;//アニメの最大コマ数
		//1コマ表示するフレーム数を超えた
		if (this->animCnt >= AnimFrame)
		{
			this->animIndex++;//アニメを1コマ進める
			this->animCnt = 0;//カウントは0に戻す
			//アニメ最大コマ数を超えた
			if (this->animIndex >= AnimMax)
			{
				this->animIndex = 0;//表示コマ数を0にする
			}
		}
		this->animCnt++;

		auto map = ge->GetTask< Map2D::Object>("マップ2D", "NoName");
		map->Map_CheckHit(this->hitBase);
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		int animTbl[3][4] =
		{
			{0,1,2,1},//CENTER
			{3,4,5,4},//LEFT
			{6,7,8,7},//RIGHT
		};
		int animX = this->animIndex;   //テーブルxインデックス
		int animY = (int)this->animDir;//テーブルyインデックス
		int animNo = animTbl[animY][animX];//表示するアニメNo

		int srcX = (animNo % 3) * 32; //画像をどの位置から表示を始めるか
		int srcY = (animNo / 3) * 32;

		ML::Box2D draw(-16, -16, 32, 32);
		draw.Offset(this->pos);
		ML::Box2D src(srcX, srcY, 32, 32);
		this->res->player->Draw(draw, src);
	}

	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//以下は基本的に変更不要なメソッド
	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//-------------------------------------------------------------------
	//タスク生成窓口
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//ゲームエンジンに登録
				
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//イニシャライズに失敗したらKill
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
	//リソースクラスの生成
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