#include "CBullet.h"
//#define VELOCITY CVector(0.0f,0.0f,1.0f)//移動速度

CBullet::CBullet()
	:mLife(50)
	,mCollider(this,&mMatrix,CVector(0.0f,0.0f,0.0f),0.1)
{}

//幅と奥行きの設定
//Set(幅,奥行)
void CBullet::Set(float w, float d) {
	//スケール設定
	mScale = CVector(1.0f, 1.0f, 1.0f);
	//三角形の頂点設定
	mT.Vertex(CVector(w, 0.0f, 0.0f),CVector(0.0f, 0.0f, -d),CVector(-w, 0.0f, 0.0f));
	//三角形の法線設定
	mT.Normal(CVector(0.0f, 1.0f, 0.0f));
}

//更新
void CBullet::Update() {
	//生存時間の判定
	if (mLife-- > 0) {
		CTransform::Update();
		mPosition = mPosition + CVector(0.0f, 0.0f, 1.0f) * mMatrixRotate;;
	}
	else {
		//無効にする
		mEnabled = false;
	}
}

//描画
void CBullet::Render() {
	//DIFFUSE黄色設定
	float c[] = { 1.0f,1.0f,0.0f,1.0f };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, c);
	//三角形の描画
	mT.Render(mMatrix);
	//CCharacter3::Render();
	//mCollider.Render();
}