#pragma once
#include "CTask.h"
#include "CImage.h"
#include "COperationIns.h"
#include "CPlayerStatusUI.h"

class CGameMenu : public CTask
{
public:
	CGameMenu();
	~CGameMenu();

	void Open();
	void Close();
	bool IsOpened() const;

	void Decide(int select);

	void Update() override;
	void Render() override;

private:
	CImage* mpBackground;
	std::vector<CImage*> mMenuItems;
	CImage* mpSelectFrame;
	CImage* mpText;
	CImage* mpStatus;
	CImage* mpTextReturn;

	int mSelectIndex;
	bool mIsOpened;

	COperationIns* mpOperationIns;
	CPlayerStatusUI* mpPlayerStatusUI;
};