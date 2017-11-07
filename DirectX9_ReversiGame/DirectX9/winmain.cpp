
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
Sprite sprite3;

Texture textureBoardColor;
Texture textureWhiteColt;
Texture textureDarkColt;
Texture textureRedColt;

Texture Number;
Texture Score;

Texture WinW;
Texture WinB;
Texture Draw;



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
	int Mx, My;									//判断するための変数
	int TurnY;									//どちらのターンか判断
	bool Turnflag;								//パスなどのためフラグ追加
	int PassCount;								//パスした回数　２連続で続いたら終わり(黒白置くところがない状態)
	bool PlayerHelp;							//おける場所の補佐ありか無しか


	//スコア
	int WscoreL, WscoreR,BscoreL,BscoreR;		//スコアのカウント用　まあ描画にも使うけども


	srand((unsigned int)time(NULL));//乱数の初期値設定

	//現在のプロセス
	enum GameMode { ZERO, START, PROCESSING, PLAY, OVER };
	GameMode game = ZERO;

	//黒の手番からスタート
	enum PlayerTurn { PLAYERWHITE, PLAYERBLACK };
	PlayerTurn PlayerTurn = PLAYERBLACK;

	//盤面の情報
	enum ReversiMap { NONE, WHITE, BLACK };
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
	{
	}
	//レンダーステートの設定  αブレンド
	d3d.SetRenderState(RENDERSTATE::RENDER_ALPHABLEND);


	sprite.SetAlpha(0.1);						//透明度の設定
	sprite.SetSize(Pixel, Pixel);				//画像の大きさ
	sprite.SetAngle(0);							//画像の回転

	sprite2.SetAlpha(0.1);						//透明度の設定
	sprite2.SetSize(64*2, 128*2);					//画像の大きさ
	sprite2.SetAngle(0);						//画像の回転

	sprite3.SetAlpha(0.1);						//透明度の設定
	sprite3.SetSize(352, 128);					//画像の大きさ
	sprite3.SetAngle(0);						//画像の回転

	//テクスチャのインスタンスを作成
	textureBoardColor.Load(_T("Texture/ReversiBoard.png"));
	textureDarkColt.Load(_T("Texture/ReversiDark.png"));
	textureWhiteColt.Load(_T("Texture/ReversiWhite.png"));
	textureRedColt.Load(_T("Texture/ReversiRedFlag.png"));
	Score.Load(_T("Texture/Score.png"));
	Number.Load(_T("Texture/number.png"));
	WinW.Load(_T("Texture/WinW.png"));
	WinB.Load(_T("Texture/WinB.png"));
	Draw.Load(_T("Texture/Draw.png"));

	//分割処理（画像）
	Number.SetDivide(10, 0);

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
					TurnY = -Pixel / 1.55;
					PassCount = 0; Turnflag = false;
					MousePositionX = screenWidth / 2, MousePositionY = screenHeight / 2;
					WscoreL = WscoreR = BscoreL = BscoreR = 0;
					PlayerTurn = PLAYERBLACK;
					PlayerHelp = true;

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
					game = PROCESSING;
					break;
				case PROCESSING:
				{
					WscoreL = WscoreR = BscoreL = BscoreR = 0;
					Turnflag = false;
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
										WscoreR++; //第一の位
										if (WscoreR > 9)
										{
											WscoreR = 0;
											WscoreL++;
										}
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
																Turnflag = true;
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
									else if (ReversiMap[x][y] == BLACK)
									{
										BscoreR++; //第一の位
										if (BscoreR > 9)
										{
											BscoreR = 0;
											BscoreL++;
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
										BscoreR++; //第一の位
										if (BscoreR > 9)
										{
											BscoreR = 0;
											BscoreL++;
										}
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
																Turnflag = true;
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
									else if (ReversiMap[x][y] == WHITE)
									{
										WscoreR++; //第一の位
										if (WscoreR > 9)
										{
											WscoreR = 0;
											WscoreL++;
										}
									}
								}
							}
							break;
					}
					//ゲームの終わるかどうかの判定
					if (Turnflag != true)
					{
						PassCount++;
						if (PassCount != 2)
						{
							if (PlayerTurn != PLAYERBLACK)
							{
								PlayerTurn = PLAYERBLACK;
							}
							else
							{
								PlayerTurn = PLAYERWHITE;
							}
						}
						else
						{
							game = OVER;
						}
					}
					else if (Turnflag == true)
					{
						if (PassCount != 1)
						{
							TurnY = TurnY * -1;
						}
						PassCount = 0;
						game = PLAY;
					}
				}
					break;
				case PLAY:
				{
					Vector2<int> mousepos = pDi->MousePosition();
					MousePositionX = mousepos.X();
					MousePositionY = mousepos.Y();
					Mx = (MousePositionX) / Pixel;
					My = (MousePositionY) / Pixel;

					if (pDi->KeyJustPressed(DIK_W))
					{
						//プレイヤーがおける場所,ヘルプを外したいとき
						if (PlayerHelp != false)
						{
							PlayerHelp = false;
						}
						else
						{
							PlayerHelp = true;
						}
					}
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
													if (ReversiMap[Mx + (dx * Count)][My + (dy * Count)] == NONE)
													{
														break;
													}
													else if (ReversiMap[Mx + (dx * Count)][My + (dy * Count)] == WHITE)
													{
														for (int Change = Count - 1; Change > 0; Change--)
														{
															ReversiMap[Mx + (dx * Change)][My + (dy * Change)] = WHITE;
														}
														break;
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
													if (ReversiMap[Mx + (dx * Count)][My + (dy * Count)] == NONE)
													{
														break;
													}
													//黒に辿りついた　返す
													else if (ReversiMap[Mx + (dx * Count)][My + (dy * Count)] == BLACK)
													{
														for (int Change = Count - 1; Change > 0; Change--)
														{
															ReversiMap[Mx + (dx * Change)][My + (dy * Change)] = BLACK;
														}
														break;
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
					if (pDi->KeyJustPressed(DIK_RETURN))
					{
						game = ZERO;
					}
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
						sprite.SetPos(Pixel * x + Pixel / 2, Pixel * y + Pixel / 2);
						sprite.Draw(textureWhiteColt);
					}
					//黒駒描画
					else if (ReversiMap[x][y] == BLACK)	//2P 黒
					{
						sprite.SetPos(Pixel * x + Pixel / 2, Pixel * y + Pixel / 2);
						sprite.Draw(textureDarkColt);
					}
					//おける場所描画
					else if (ReversiFlag[x][y] == 1 && PlayerHelp == true)
					{
						sprite.SetPos(Pixel * x + Pixel / 2, Pixel * y + Pixel / 2);
						sprite.Draw(textureRedColt);
					}
				}
			}
			
			//スコア関連描画
			//赤玉
			sprite.SetPos(screenWidth / 10 * 6 + Pixel / 2, screenHeight / 2 - TurnY + Pixel / 2 + 64);
			sprite.Draw(textureRedColt);
			//黒白の文字
			sprite2.SetPos(screenWidth / 10 * 7 + Pixel / 2, screenHeight / 2 + Pixel / 2 + 72);
			sprite2.Draw(Score);
			//現在の黒白の数
			//黒左
			if (BscoreL > 0)
			{
				sprite.SetPos(screenWidth / 5 * 4 + Pixel / 2, screenHeight / 2 + Pixel / 2);
				Number.SetNum(BscoreL, 0);
				sprite.Draw(Number);
			}
			//黒右
			sprite.SetPos(screenWidth / 5 * 4 + Pixel/2 + 64, screenHeight / 2 + Pixel / 2);
			Number.SetNum(BscoreR, 0);
			sprite.Draw(Number);
			//白左
			if (WscoreL > 0)
			{
				sprite.SetPos(screenWidth / 5 * 4 + Pixel / 2, screenHeight / 2 + Pixel / 2 + 128);
				Number.SetNum(WscoreL, 0);
				sprite.Draw(Number);
			}
			//白右
			sprite.SetPos(screenWidth / 5 * 4 + Pixel / 2 + 64, screenHeight / 2 + Pixel / 2 + 128);
			Number.SetNum(WscoreR, 0);
			sprite.Draw(Number);

			//勝利者を讃えたまえ
			if (game == OVER)
			{
				sprite3.SetPos(screenWidth / 9 * 7 + Pixel / 2, screenHeight / 10 * 2 + Pixel / 2 );
				if (WscoreL * 10 + WscoreR > BscoreL * 10 + BscoreR)
				{
					sprite3.Draw(WinW);
				}
				else if(WscoreL * 10 + WscoreR < BscoreL * 10 + BscoreR)
				{
					sprite3.Draw(WinB);
				}
				else
				{
					sprite3.Draw(Draw);
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
