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
	//リソースの初期化
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
	//リソースの解放
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
	//「初期化」タスク生成時に１回だけ行う処理
	bool  Object::Initialize()
	{
		//スーパークラス初期化
		__super::Initialize(defGroupName, defName, true);
		//リソースクラス生成orリソース共有
		this->res = Resource::Create();

		//★データ初期化
		this->frame = 0;
		
		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		ge->KillAll_G("ステージロゴ");

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			Game::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		this->frame++;
		if (this->frame == 60)
			
		{
			this->frame = 61;
			ge->StartCounter("test", 45); //フェードは90フレームなので半分の45で切り替え
			ge->CreateEffect(98, ML::Vec2(0, 0));
		}
		if (ge->getCounterFlag("test") == ge->LIMIT) {
			this->Kill();
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		//背景
		ML::Box2D draw_b(0, 0, ge->screen2DWidth, ge->screen2DHeight);
		ML::Box2D src_b(0, 0, 480, 270);
		this->res->back->Draw(draw_b, src_b);

		//ステージ表示
		ML::Box2D draw_stage(50, 50, 277, 84);
		ML::Box2D src_stage(0, 0, 277, 84);
		this->res->stage->Draw(draw_stage, src_stage);

		//ストック表示
		ML::Box2D draw_stock(50, 150, 285, 84);
		ML::Box2D src_stock(0, 0, 285, 84);
		this->res->stock->Draw(draw_stock, src_stock);

		//0,1,2表示
		ML::Box2D stage_draw_num(335, 50, 56, 84);
		ML::Box2D stock_draw_num(335, 150, 56, 84);
		ML::Box2D src_num(0, 0, 56, 84);

		//現在のステージナンバー
		this->res->number[ge->stage_num]->Draw(stage_draw_num, src_num);
		//現在のストック数
		this->res->number[ge->stock_num -1]->Draw(stock_draw_num, src_num);
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