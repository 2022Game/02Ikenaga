#include "CWeapon.h"

CWeapon::CWeapon()
	: CObjectBase(ETag::eItem, ETaskPriority::eWeapon)
	, mpAttachMtx(nullptr)
{
}

CWeapon::~CWeapon()
{
}

void CWeapon::SetAttachMtx(const CMatrix* mtx)
{
	mpAttachMtx = mtx;
}
