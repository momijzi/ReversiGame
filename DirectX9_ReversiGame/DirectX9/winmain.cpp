
#include <Windows.h>
#pragma comment(lib,"winmm.lib")

#include <tchar.h>

#include"Direct3D.h"

#include "Sprite.h"
#include "Texture.h"

#include "DirectInput.h"

#include"time.h"

#define screenWidth 960
#define screenHeight 960
#define Pixel 72

//ウィンドウプロシージャ
LRESULT CALLBACK WndPrc
(
	HWND hWnd,	//ウィンドウのハンドル
	UINT msg,	//メッセージ
	WPARAM wParam,
	LPARAM lParam
)
{
	//メッセージ処理のための関数
	//DispatchMessageによって呼び出される

	//ウィンドウクラスの登録時に
	//各ウィンドウに設定される

	//アプリケーションがメッセージを
	//取得したら呼び出される

	//メッセージの例

	//WM_DESTROY ウィンドウが破棄された
	//			 ×ボタンが押された時など
	//WM_MOVE    ウィンドウが移動
	//WM_SIZE    ウィンドウサイズの変更
	//等

	//特別な処理を必要とする場合
	//または自分で独自の処理を定義したい場合
	//ここでメッセージの種類に応じた
	//処理を行う

	//取りあえず WM_DESTROYが来た時の
	//終了処理のみ定義

	switch (msg)
	{
		case WM_DESTROY:

			PostQuitMessage(0);

			//メッセージキューに
			//新しくWM_QUITメッセージを送る

			//いずれメッセージキューから
			//取り出され
			//メインループが終了する
			//(メインループの終了条件を
			//そのように作る)
			break;
	}

	//独自の処理を行ったら
	//デフォルトのウィンドウプロシージャを実行

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

//ウィンドウ設定の名前
static const TCHAR* WC_BASIC = _T("Basic");

//作りたいウィンドウの設定を
//登録する関数
HRESULT RegistClassEx(HINSTANCE hInstance)
{
	//ウィンドウの設定を保存する構造体
	WNDCLASSEX wcex;
	
	//wcexの先頭アドレスから
	//WNDCLASSEXの長さ分のメモリを
	//全て0で埋める
	memset(&wcex, 0, sizeof(WNDCLASSEX));
	//つまりはwcexが使用しているメモリ領域を
	//全て0で初期化する処理

	//ウィンドウの設定

	//構造体のサイズ
	wcex.cbSize = sizeof(WNDCLASSEX);

	//ウィンドウの設定情報に追加する捕捉を付けないので
	//0で固定
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;

	//Windowのスタイル
	//それぞれがビット単位のフラグであるため
	//オア演算で合成したものがスタイルの最終的な設定
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;

	//CS_HREDRAW 横サイズが変わったときウィンドウ全体の再描画
	//   VREDRAW 縦サイズが変わったときウィンドウ全体の再描画
	//   DBLCLKS ダブルクリックが発生したことをウィンドウに通知

	//WinMainのインスタンスハンドル
	wcex.hInstance = hInstance;

	//ウィンドウプロシージャのアドレス
	//関数ポインタの設定
	//ウィンドウプロシージャをOSが呼び出せるようにする
	wcex.lpfnWndProc = (WNDPROC)WndPrc;

	//背景色
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

	//メニュー　なし
	wcex.lpszMenuName = NULL;

	//ウィンドウクラスの名前　このウィンドウ設定の名前
	wcex.lpszClassName = WC_BASIC;

	//アイコン　デフォルトのを使う
	wcex.hIcon = (HICON)LoadImage(NULL,
		MAKEINTRESOURCE(IDI_APPLICATION),
		IMAGE_ICON,
		0, 
		0, 
		LR_DEFAULTSIZE | LR_SHARED);
	wcex.hIconSm = NULL;

	//カーソル デフォルトのものを使う
	wcex.hCursor= (HCURSOR)LoadImage(NULL,
		MAKEINTRESOURCE(IDC_ARROW),
		IMAGE_CURSOR,
		0,
		0,
		LR_DEFAULTSIZE | LR_SHARED);

	//設定したウィンドウクラスを登録
	//登録したウィンドウ設定でないと
	//ウィンドウを作ることができない
	return RegisterClassEx(&wcex); 
	//HRESULT型の実行結果が戻される
}


//登録した設定を基にウィンドウを作成する
HRESULT MakeWindow
(HINSTANCE hInstance,//WinMainの識別子
	HWND &refHWnd,	//ウィンドウの識別子
				   //正しくウィンドウの作成ができたら
				   //この変数に識別子を代入する
	int width = screenWidth,	//クライアント領域の幅
	int height = screenHeight)	//クライアント領域の高さ
	//クライアント領域はウィンドウ全体から
	//外枠やメニューの部分を除いた物と今は思っておけばOK
{
	//クライアント領域のおおきさを決める
	RECT rect;
	rect.top = 0;
	rect.left = 0;
	rect.right = width;
	rect.bottom = height;

	//クライアント領域を保持するのに必要な
	//ウィンドウのおおきさを計算
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE/*TRUE*/);
	//正しく実行されるとrectの値が更新される

	//WS_OVERLAPPEDWINDOWはウィンドウのスタイルの一つ

	//ウィンドウのハンドル
	//ウィンドウの識別に使用する
	HWND hWnd;

	hWnd = CreateWindowEx
	(	0,						//ウィンドウ拡張スタイル
		WC_BASIC,				//作りたいウィンドウクラス
								//あらかじめ登録されたもの
		_T("タイトル"),			//ウィンドウのタイトル
		WS_OVERLAPPEDWINDOW,	//ウィンドウのスタイル
		CW_USEDEFAULT,			//位置x座標 デフォルトの値
		CW_USEDEFAULT,			//位置y座標 デフォルトの値
		rect.right - rect.left,	//ウィンドウ全体の幅
		rect.bottom - rect.top, //ウィンドウ全体の高さ
		NULL,					//親ウィンドウのハンドル
		NULL,					//メニューのハンドル
		hInstance,				//WinMainのハンドル

		NULL);					//ウィンドウ作成データ
								//ウィンドウに好きな32ビットのデータを
								//付属させることができる
								//追加情報構造体へのポインタなど
								//を付けるのが無難な使い方か
								//今回は使わない

	//ウィンドウが正しく作れたかをチェック
	//正しく作れたらhWndにNULL以外が入るはず
	if (hWnd == NULL)
	{
		return S_FALSE;//HRESULT型　失敗を表す識別子の一つ
	}

	//ウィンドウをアクティブにして表示
	//hWndでウィンドウの識別をしていることに注目
	ShowWindow(hWnd, SW_SHOWNORMAL);

	//クライアント領域の更新
	UpdateWindow(hWnd);

	//参照渡しになっている引数に
	//ウィンドウの識別ハンドルをコピーする
	//これによって今作ったウィンドウの識別ハンドルを
	//呼び出し元のプログラムで得ることができる
	refHWnd = hWnd;

	//HRESULT型　成功を表す識別子の一つ
	return S_OK;
}

//スプライトのインスタンスを作成
//パラメータは適当で
Sprite sprite;
Sprite sprite2;
Texture textureBoardColor;
Texture textureWhiteColt;
Texture textureDarkColt;
Texture textureRedColt;
Texture textureStart;
Texture textureOver;
Texture textureClear;



//エントリーポイント
//プログラムの開始関数

int _stdcall WinMain
(HINSTANCE hInstance,		//プログラムを識別するハンドル
	HINSTANCE hPrevInstance,//アプリケーションの前のインスタンスハンドル
							//win32だと常にNULL
	LPSTR lpCmdLine,		//コマンドライン引数
	int nCmdShow)			//ウィンドウの表示状態
{
	//変数の宣言-------------------------------------
	const int ReversiXY = 8;//おける場所　８×８
	int ReversiFlag[ReversiXY][ReversiXY];		//ひっくりかえせる場所
	int MousePositionX, MousePositionY;			//マウスの位置
	int Mx, My;

	srand((unsigned int)time(NULL));//乱数の初期値設定

	//現在のプロセス
	enum GameMode { ZERO, START,PROCESSING, PLAY, OVER };
	GameMode game = ZERO;

	//白の手番からスタート
	enum PlayerTurn {PLAYERWHITE,PLAYERBLACK};
	PlayerTurn PlayerTurn = PLAYERWHITE;

	//盤面の情報
	enum ReversiMap {NONE,WHITE,BLACK};
	ReversiMap ReversiMap[ReversiXY][ReversiXY];

	if (FAILED(RegistClassEx(hInstance)))
	{
		MessageBox(NULL,
			TEXT("ウィンドウクラス設定失敗"),
			TEXT("テスト-タイトル"),
			MB_OK);
		return 0;
	}
	HWND hWnd = NULL;
	/*正しくウィンドウが作成されれば
	hWndにウィンドウの識別ハンドルが入る*/
	if (FAILED(MakeWindow(hInstance, hWnd)))
	{
		MessageBox(NULL,
			TEXT("ウィンドウ作成失敗"),
			TEXT("テスト-タイトル"),
			MB_OK);
		return 0;
	}
	//Direct3Dを管理するクラス(シングルトン)への参照を取得
	Direct3D& d3d = Direct3D::GetInstance();
	//Direct3DDeviceの作成を試みる
	if (d3d.TryCreate(hWnd))
	{ }
	//レンダーステートの設定  αブレンド
	d3d.SetRenderState(RENDERSTATE::RENDER_ALPHABLEND);


	sprite.SetAlpha(0.1);						//透明度の設定
	sprite.SetSize(Pixel, Pixel);				//画像の大きさ
	sprite.SetAngle(0);							//画像の回転

	//テクスチャのインスタンスを作成
	textureBoardColor.Load(_T("Texture/ReversiBoard.png"));
	textureDarkColt.Load(_T("Texture/ReversiDark.png"));
	textureWhiteColt.Load(_T("Texture/ReversiWhite.png"));
	textureRedColt.Load(_T("Texture/ReversiRedFlag.png"));

	
	DirectInput * pDi = DirectInput::GetInstance();
	pDi->Init(hWnd);

	//メインループ
	//メッセージループ

	MSG msg = {};

	
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			pDi->Update();//キー状態の更新
			if (pDi->KeyJustPressed(DIK_A))
			{
				MessageBox(NULL,
					TEXT("キー入力確認"),
					TEXT("テスト-タイトル"),
					MB_OK);
			}

			if (pDi->MouseButtonJustPressed(MOUSE_BUTTON_LEFT))
				Vector2<int> vec = pDi->MousePosition();
			{
				int breakpoint = 0;
			}

			//sprite.SetAngle(sprite.GetAngle_Rad()+0.0f);//回転ここのfを大きい値にするほど回転速度が上がる

			static int dir = 1;//α
			sprite.SetAlpha(sprite.GetAlpha() + (dir*0.01));
			switch (dir)
			{
				case 1:
					if (sprite.GetAlpha() >= 1)
					{
						dir = -1;
					}
					break;
				case -1:
					if (sprite.GetAlpha() <= 0)
					{
						dir = 1;
					}
					break;
			}
			//メインプログラム//------------------------------------------------------------------------------------------
			switch (game)
			{
				case ZERO:
					d3d.ClearScreen();
					MousePositionX = screenWidth / 2, MousePositionY = screenHeight / 2;

					for (int y = 0; y < ReversiXY; y++)
					{
						for (int x = 0; x < ReversiXY; x++)
						{
							//0で初期化
							ReversiFlag[x][y] = 0;
						}
					}
					//配列初期化　最初の状態に
					for (int y = 0; y < ReversiXY; y++)
					{
						for (int x = 0; x < ReversiXY; x++)
						{
							if ((x == 3 && y == 3) || (x == 4 && y == 4))
							{
								ReversiMap[x][y] = WHITE;//1Pで白
							}
							else if ((x == 4 && y == 3) || (x == 3 && y == 4))
							{
								ReversiMap[x][y] = BLACK;//2Pで黒
							}
							else
							{
								ReversiMap[x][y] = NONE;//何も置かれていない
							}
						}
					}
					game = START;
					break;
				case START:
					//エンター押したときPLAYへ
					/*if (pDi->KeyJustPressed(DIK_RETURN))
					{*/
					game = PROCESSING;
					//}
					break;
				case PROCESSING:
					game = PLAY;
					//まずおける場所を判断するために配列を初期化
					for (int y = 0; y < ReversiXY; y++)
					{
						for (int x = 0; x < ReversiXY; x++)
						{
							//0で初期化
							ReversiFlag[x][y] = 0;
						}
					}
					switch (PlayerTurn)
					{
						//白のターンの時における場所判断
						case PLAYERWHITE:
							for (int y = 0; y < ReversiXY; y++)
							{
								for (int x = 0; x < ReversiXY; x++)
								{
									if (ReversiMap[x][y] == WHITE)
									{
										//白の駒から周りを見る
										for (int dy = -1; dy < 2; dy++)
										{
											for (int dx = -1; dx < 2; dx++)
											{
												//白の先に黒があったことを示す
												if (ReversiMap[x + dx][y + dy] == BLACK)
												{
													for (int Count = 2; Count < 8; Count++)
													{
														if (x + (dx * Count) < 8 && y + (dy * Count) < 8
															&& x + (dx * Count) > -1 && y + (dy * Count) > -1)
														{
															//黒が並んでおり終着点に何もなかった場合
															if (ReversiMap[x + (dx * Count)][y + (dy * Count)] == NONE)
															{
																//このターンおける場所を格納
																ReversiFlag[x + (dx * Count)][y + (dy * Count)] = 1;
																break;
															}
															else if (ReversiMap[x + (dx * Count)][y + (dy * Count)] == WHITE)
															{
																break;
															}
														}
													}
												}
											}
										}
									}
								}
							}
							break;
							//黒のターンの時における場所判断
						case PLAYERBLACK:
							for (int y = 0; y < ReversiXY; y++)
							{
								for (int x = 0; x < ReversiXY; x++)
								{
									if (ReversiMap[x][y] == BLACK)
									{
										//黒の駒から周りを見る
										for (int dy = -1; dy < 2; dy++)
										{
											for (int dx = -1; dx < 2; dx++)
											{
												//黒の先に白があったことを示す
												if (ReversiMap[x + dx][y + dy] == WHITE)
												{
													for (int Count = 2; Count < 8; Count++)
													{
														if (x + (dx * Count) < 8 && y + (dy * Count) < 8
															&& x + (dx * Count) > -1 && y + (dy * Count) > -1)
														{
															//黒が並んでおり終着点に何もなかった場合
															if (ReversiMap[x + (dx * Count)][y + (dy * Count)] == NONE)
															{
																//このターンおける場所を格納
																ReversiFlag[x + (dx * Count)][y + (dy * Count)] = 1;
																break;
															}
															else if (ReversiMap[x + (dx * Count)][y + (dy * Count)] == BLACK)
															{
																break;
															}
														}
													}
												}
											}
										}
									}
								}
							}
							break;
					}
					break;
				case PLAY:
				{
					Vector2<int> mousepos = pDi->MousePosition();
					MousePositionX = mousepos.X();
					MousePositionY = mousepos.Y();
					Mx = (MousePositionX) / Pixel;
					My = (MousePositionY) / Pixel;

					//マウス座標が取れた　そしてプレイヤーがおける場所を押したとき
					if (pDi->MouseButtonJustPressed(MOUSE_BUTTON_LEFT))
					{
						if (ReversiFlag[Mx][My] == 1)
						{
							if (PlayerTurn != PLAYERBLACK)
							{
								//白を置く
								ReversiMap[Mx][My] = WHITE;
								//ひっくり返す
								for (int dy = -1; dy < 2; dy++)
								{
									for (int dx = -1; dx < 2; dx++)
									{
										//置いた先に黒があったことを示す
										if (ReversiMap[Mx + dx][My + dy] == BLACK)
										{
											//先を見るためのループ
											for (int Count = 2; Count < 8; Count++)
											{
												//ここで配列外を除外
												if (Mx + (dx * Count) < 8 && My + (dy * Count) < 8
													&& Mx + (dx * Count) > -1 && My + (dy * Count) > -1)
												{
													//白に辿りついた　返す
													if (ReversiMap[Mx + (dx * Count)][My + (dy * Count)] == WHITE)
													{
														for (int Change = Count - 1; Change > 0; Change--)
														{
															ReversiMap[Mx + (dx * Change)][My + (dy * Change)] = WHITE;
														}
													}
												}
											}
										}
									}
								}
								PlayerTurn = PLAYERBLACK;
							}
							else
							{
								//黒を置く
								ReversiMap[Mx][My] = BLACK;
								//ひっくり返す
								for (int dy = -1; dy < 2; dy++)
								{
									for (int dx = -1; dx < 2; dx++)
									{
										//置いた先に白があったことを示す
										if (ReversiMap[Mx + dx][My + dy] == WHITE)
										{
											//先を見るためのループ
											for (int Count = 2; Count < 8; Count++)
											{
												//ここで配列外を除外
												if (Mx + (dx * Count) < 8 && My + (dy * Count) < 8
													&& Mx + (dx * Count) > -1 && My + (dy * Count) > -1)
												{
													//黒に辿りついた　返す
													if (ReversiMap[Mx + (dx * Count)][My + (dy * Count)] == BLACK)
													{
														for (int Change = Count - 1; Change > 0; Change--)
														{
															ReversiMap[Mx + (dx * Change)][My + (dy * Change)] = BLACK;
														}
													}
												}
											}
										}
									}
								}
								PlayerTurn = PLAYERWHITE;
							}
							game = PROCESSING;
						}
					}
				}
					break;
				case OVER:

					
					break;
			}
					//まず描画 
					d3d.BeginScene();//描画開始
					d3d.ClearScreen();//描画初期化

					//ボードの描画　//大きさを変える気がないならば固定でやればよい
					for (int y = 0; y < ReversiXY; y++)
					{
						for (int x = 0; x < ReversiXY; x++)
						{
							sprite.SetPos(Pixel * x + Pixel / 2, Pixel * y + Pixel / 2);
							sprite.Draw(textureBoardColor);
						}
					}
					//駒 と　おける場所の描画
					for (int y = 0; y < ReversiXY; y++)
					{
						for (int x = 0; x < ReversiXY; x++)
						{
							//白駒描画
							if (ReversiMap[x][y] == WHITE)		//1P 白
							{
								sprite.SetPos(Pixel * x + Pixel / 2 , Pixel * y + Pixel / 2);
								sprite.Draw(textureWhiteColt);
							}
							//黒駒描画
							else if (ReversiMap[x][y] == BLACK)	//2P 黒
							{
								sprite.SetPos(Pixel * x + Pixel / 2, Pixel * y + Pixel / 2);
								sprite.Draw(textureDarkColt);
							}
							//おける場所描画
							else if (ReversiFlag[x][y] == 1 )
							{
								sprite.SetPos(Pixel * x + Pixel / 2, Pixel * y + Pixel / 2);
								sprite.Draw(textureRedColt);
							}
						}
					}
					//描画終了の合図//--------------------------------------------------------------------------------------------

					d3d.EndScene();

					//バックバッファをフロントへ反映
					d3d.Present();			
		}
	}
	return 0;
}
