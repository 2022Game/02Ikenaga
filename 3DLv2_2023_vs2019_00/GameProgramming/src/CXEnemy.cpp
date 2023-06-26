#include "CXEnemy.h"

void CXEnemy::Init(CModelX* model)
{
	CXCharacter::Init(model);
	//‡¬s—ñ‚Ìİ’è
	//“ª
	mColSphereHead.Matrix(&mpCombinedMatrix[11]);
	//Œ•
	mColSphereSword.Matrix(&mpCombinedMatrix[21]);
	//‘Ì
	mColSphereBody.Matrix(&mpCombinedMatrix[8]);
}