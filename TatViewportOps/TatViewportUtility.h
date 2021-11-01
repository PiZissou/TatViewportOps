#pragma once
#include "TatViewportCore.h"

#define TatViewportUtility_CLASS_ID Class_ID(0xd95a333c, 0x460d70cf)

class TatViewportUtility : public UtilityObj
{
public:
	// Constructor/Destructor
	TatViewportUtility();
	virtual ~TatViewportUtility();

	virtual void DeleteThis() override {}

	virtual void BeginEditParams(Interface* ip, IUtil* iu) override;
	virtual void EndEditParams(Interface* ip, IUtil* iu) override;

	virtual void Init(HWND hWnd);
	virtual void Destroy(HWND hWnd);

	void OnCurrentViewportChanged();
	// Singleton access
	static TatViewportUtility* GetInstance() {
		static TatViewportUtility theTatViewportOps;
		return &theTatViewportOps;
	}

private:
	static INT_PTR CALLBACK DlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	HWND hPanel;
	IUtil* iu;
};


class TatViewportUtilityClassDesc : public ClassDesc2
{
public:
	virtual int           IsPublic() override { return TRUE; }
	virtual void* Create(BOOL /*loading = FALSE*/) override { return TatViewportUtility::GetInstance(); }
	virtual const TCHAR* ClassName() override { return GetString(IDS_CLASS_NAME); }
	virtual SClass_ID     SuperClassID() override { return UTILITY_CLASS_ID; }
	virtual Class_ID      ClassID() override { return TatViewportUtility_CLASS_ID; }
	virtual const TCHAR* Category() override { return GetString(IDS_CATEGORY); }

	virtual const TCHAR* InternalName() override { return _T("TatViewportUtility"); } // Returns fixed parsable name (scripter-visible name)
	virtual HINSTANCE     HInstance() override { return hInstance; } // Returns owning module handle


};