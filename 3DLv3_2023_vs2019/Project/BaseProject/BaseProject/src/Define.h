#pragma once
#define WINDOW_WIDTH 1280	//ウィンドウの幅
#define WINDOW_HEIGHT 720	//ウィンドウの高さ
//#define FULL_SCREEN	//フルスクリーンにする場合は定義する
#define GAME3D	//3Dゲームの場合は定義する

//配列のサイズ取得をマクロ化
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

//リソースファイルフォルダ
#define RES_DIR "res\\"
//領域解放をマクロ化
#define SAFE_DELETE(del) {if(del != nullptr) delete del; del = nullptr;}
//動的配列の領域解放をマクロ化
#define SAFE_DELETE_ARRAY(ary) {if(ary != nullptr) delete[] ary; ary = nullptr;}



#include "Time.h"
#include "CDebugPrint.h"
#include "CDebugProfiler.h"