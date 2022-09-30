//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Map2D.h"

namespace  Map2D
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->MapChip = DG::Image::Create("./data/image/mapchip.png");
		this->TimeLimit = DG::Image::Create("./data/image/Time.png");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
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

			//マップのゼロクリア
		for (int y = 0; y < 9; ++y) {
			for (int x = 0; x < 15; ++x) {
				arr[y][x] = 0;
			}
		}

		//マップチップ情報の初期化
		for (int c = 0; c < 4 * 4; ++c)
		{
			chip[c] = ML::Box2D((c % 4) * 32, (c / 4) * 32, 32, 32);
		}
		//ファイルパスを作る

		string fliePath = "./data/map/Map" + to_string(ge->stage_num) + ".txt";
		//ファイルを開く（読み込み）
		ifstream fin(fliePath);
		if (!fin) { return false; }//読み込み失敗
		//配列にデータを読み込む
		for (int y = 0; y < 9; ++y)
		{
			for (int x = 0; x < 15; ++x)
			{
				fin >> arr[y][x];
			}
		}
		//ファイルを閉じる
		fin.close();
		return true;

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
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		for (int y = 0; y < 9; ++y)
		{
			for (int x = 0; x < 15; ++x)
			{
				if (arr[y][x] != -1) {
					ML::Box2D  draw(x * 32, y * 32, 32, 32);
					int  num = arr[y][x];	//現在のマスのチップ番号を取得
					ML::Box2D  src = chip[num];	//チップ番号から転送元画像範囲を得る
				//draw.Offset(x * 32, y * 32);
					this->res->MapChip->Draw(draw, src);
				}
			}
		}

		//タイム描画
		//1の位
		ML::Box2D draw(438, 32, 16, 32);
		ML::Box2D src(16 * One, 0, 16, 32);
		this->res->TimeLimit->Draw(draw, src);
		//10の位
		ML::Box2D draw10(420, 32, 16, 32);
		ML::Box2D src10(16 * Ten, 0, 16, 32);
		this->res->TimeLimit->Draw(draw10, src10);
	}

	//-------------------------------------------------------------------
//マップの当たり判定
	bool Object::Map_CheckHit(const ML::Box2D& h_)
	{
		ML::Rect r = { h_.x, h_.y, h_.x + h_.w, h_.y + h_.h };
		//矩形がマップ外に出た時の対応
		//未実装

		//ループ範囲調整
		int sx, sy, ex, ey;
		sx = r.left / 32;
		sy = r.top / 32;
		ex = (r.right - 1) / 32;
		ey = (r.bottom - 1) / 32;

		//範囲内の障害物を探す
		for (int y = sy; y <= ey; ++y) {
			for (int x = sx; x <= ex; ++x) {
				if (0 < arr[y][x]) {
					return true;
				}
			}
		}
		return false;
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