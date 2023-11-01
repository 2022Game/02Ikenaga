#pragma once

namespace Time
{
	// 目標フレームレートを取得
	int TargetFPS();
	// 前回のフレームのFPSを取得
	float FPS();
	// 前回のフレームの経過時間を取得
	float DeltaTime();
}