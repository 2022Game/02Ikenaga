#include "System.h"
#include "CColor.h"
#include <glut.h>

// 画面のクリアカラーを設定（rgba）
void System::SetClearColor(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
}

// 画面のクリアカラーを設定（CColor）
void System::SetClearColor(const CColor& color)
{
	SetClearColor(color.R(), color.G(), color.B(), color.A());
}
