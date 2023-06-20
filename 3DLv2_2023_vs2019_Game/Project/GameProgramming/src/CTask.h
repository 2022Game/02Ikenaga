#pragma once
#ifndef CTASK_H
#define CTASK_H
class CTaskManager;
/*
タスククラス
タスクリストの要素
*/

class CCollisionManager;
class CTask {
	friend CCollisionManager;
	friend CTaskManager;
public:
	//衝突処理	
	virtual void Collision() {}
	//デフォルトコンストラクタ
	CTask();
	//デストラクタ　virtualにしないと子クラスのデストラクタが呼ばれない
	virtual~CTask(){}
	//更新
	virtual void Update() {}
	//描画
	virtual void Render(){}

	void SetHidden(bool isRender);
	//描画するかどうか
	bool IsHidden()const;
protected:
	int mPriority; //優先度
	bool mEnabled; //有効フラグ
	bool mIsHidden;
private:
	CTask* mpNext;//次のポインタ
	CTask* mpPrev;//前のポインタ
};

#endif 