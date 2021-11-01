#include "TatViewportUtility.h"

ClassDesc2* GetTatViewportUtilityDesc()
{
	static TatViewportUtilityClassDesc TatViewportOpsDesc;
	return &TatViewportOpsDesc;
}


TatViewportUtility::TatViewportUtility()
	: iu(nullptr)
	, hPanel(nullptr)
{

}

TatViewportUtility::~TatViewportUtility()
{

}

void TatViewportUtility::BeginEditParams(Interface* ip, IUtil* iu)
{
	this->iu = iu;

	hPanel = ip->AddRollupPage(
		hInstance,
		MAKEINTRESOURCE(IDD_CAMERA_GUIDES),
		DlgProc,
		GetString(IDS_PARAMS),
		0);
}

void TatViewportUtility::EndEditParams(Interface* ip, IUtil*)
{
	this->iu = nullptr;
	ip->DeleteRollupPage(hPanel);
	hPanel = nullptr;
}

void TatViewportUtility::Init(HWND /*handle*/)
{

}

void TatViewportUtility::Destroy(HWND /*handle*/)
{

}

void TatViewportUtility::OnCurrentViewportChanged()
{
	if (hPanel != nullptr)
	{
		DlgProc(hPanel, WM_INITDIALOG, 0, 0);
	}
}

INT_PTR CALLBACK TatViewportUtility::DlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	ICustButton* iButDisplayGuide;
	TatViewportCore* vCore = TatViewportCore::GetInstance();

	Interface19* it = GetCOREInterface19();
	int currentViewId = it->GetActiveViewExp().GetViewID();

	switch (msg)
	{
	case WM_INITDIALOG:
		TatViewportUtility::GetInstance()->Init(hWnd);
		iButDisplayGuide = GetICustButton(GetDlgItem(hWnd, IDC_BUTTON_GUIDES));
		iButDisplayGuide->SetType(CBT_CHECK);
		iButDisplayGuide->SetCheck( vCore->IsGuideDisplayed(currentViewId));
		ReleaseICustButton(iButDisplayGuide);

		switch (vCore->GetGuideType(currentViewId))
		{
			case gType::p_ruleOfThird:
			{
				CheckRadioButton(hWnd, IDC_RAD_RULEOFTHIRD, IDC_RAD_GOLDENTRIANGLE, IDC_RAD_RULEOFTHIRD);
				break;
			}
			case gType::p_cross:
			{
				CheckRadioButton(hWnd, IDC_RAD_RULEOFTHIRD, IDC_RAD_GOLDENTRIANGLE, IDC_RAD_CROSS);
				break;
			}
			case gType::p_centerCross:
			{
				CheckRadioButton(hWnd, IDC_RAD_RULEOFTHIRD, IDC_RAD_GOLDENTRIANGLE, IDC_RAD_CENTERCROSS);
				break;
			}
			case gType::p_goldenRatio:
			{
				CheckRadioButton(hWnd, IDC_RAD_RULEOFTHIRD, IDC_RAD_GOLDENTRIANGLE, IDC_RAD_GOLDENRATIO);
				break;
			}
			case gType::p_goldenTriangle:
			{
				CheckRadioButton(hWnd, IDC_RAD_RULEOFTHIRD, IDC_RAD_GOLDENTRIANGLE, IDC_RAD_GOLDENTRIANGLE);
				break;
			}
		}
		
		break;

	case WM_DESTROY:
		TatViewportUtility::GetInstance()->Destroy(hWnd);
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
			case IDC_BUTTON_GUIDES:
			{
				iButDisplayGuide = GetICustButton(GetDlgItem(hWnd, IDC_BUTTON_GUIDES));
				BOOL onOff = iButDisplayGuide->IsChecked();

				vCore->DisplayGuide(currentViewId, onOff);

				break;
			}
			case IDC_RAD_RULEOFTHIRD:
			{
				vCore->SetGuideType(currentViewId,gType::p_ruleOfThird);
				break;
			}
			case IDC_RAD_CROSS:
			{
				vCore->SetGuideType(currentViewId,gType::p_cross);
				break;
			}
			case IDC_RAD_CENTERCROSS:
			{
				vCore->SetGuideType(currentViewId,gType::p_centerCross);
				break;
			}
			case IDC_RAD_GOLDENRATIO:
			{
				vCore->SetGuideType(currentViewId,gType::p_goldenRatio);
				break;
			}
			case IDC_RAD_GOLDENTRIANGLE:
			{
				vCore->SetGuideType(currentViewId, gType::p_goldenTriangle);
				break;
			}
		}
		break;

	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_MOUSEMOVE:
		GetCOREInterface()->RollupMouseMessage(hWnd, msg, wParam, lParam);
		break;

	default:
		return 0;
	}
	return 1;
}