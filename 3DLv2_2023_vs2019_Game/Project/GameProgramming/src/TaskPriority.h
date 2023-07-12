#pragma once

//Taskの優先順序
enum class TaskPriority
{
	eNone = -1,

	eBackground,  //背景
	eDefault,	//デフォルト
	ePlayer,	//プレイヤー

	eEffect,	//エフェクト

	eUI,		//UI関連
	eSystem,	//システム関連

	Num
};