#pragma once
#include "BaseSystem/CObjectBase.h"

class CWeapon : public CObjectBase
{
public:
	CWeapon();
	virtual ~CWeapon();

	void SetAttachMtx(const CMatrix* mtx);

protected:
	const CMatrix* mpAttachMtx;
};
