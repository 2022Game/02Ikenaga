#include"CStage.h"

void CStage::Collision()
{

}

CStage::CStage(CModel* model, const CVector& position, const CVector& rotation, const CVector& scale)
	//: mCollider1(this, &mMatrix, CVector(0.0f, 5.0f, 0.0f), 0.8f)
	//, mCollider2(this, &mMatrix, CVector(0.0f, 5.0f, 20.0f), 0.8f)
	//, mCollider3(this, &mMatrix, CVector(0.0f, 5.0f, -20.0f), 0.8f)
{
	//モデル,位置,回転,拡縮を設定する
	mpModel = model;  //モデルの設定
	mPosition = position;  //位置の設定
	mRotation = rotation;  //回転の設定
	mScale = scale;  //拡縮の設定
}

//更新処理
void CStage::Update()
{

}