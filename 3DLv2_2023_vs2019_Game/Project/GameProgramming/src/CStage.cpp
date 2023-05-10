#include"CStage.h"

void CStage::Collision()
{

}

CStage::CStage(CModel* model, const CVector& position, const CVector& rotation, const CVector& scale)
	//: mCollider1(this, &mMatrix, CVector(0.0f, 5.0f, 0.0f), 0.8f)
	//, mCollider2(this, &mMatrix, CVector(0.0f, 5.0f, 20.0f), 0.8f)
	//, mCollider3(this, &mMatrix, CVector(0.0f, 5.0f, -20.0f), 0.8f)
{
	//ƒ‚ƒfƒ‹,ˆÊ’u,‰ñ“],Šgk‚ğİ’è‚·‚é
	mpModel = model;  //ƒ‚ƒfƒ‹‚Ìİ’è
	mPosition = position;  //ˆÊ’u‚Ìİ’è
	mRotation = rotation;  //‰ñ“]‚Ìİ’è
	mScale = scale;  //Šgk‚Ìİ’è
}

//XVˆ—
void CStage::Update()
{

}