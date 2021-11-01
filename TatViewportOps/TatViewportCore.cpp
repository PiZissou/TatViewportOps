#include "TatViewportCore.h"
#include "TatViewportUtility.h"

#include <maxscript\macros\define_external_functions.h>
#	include "namedefs.h"

void ToMaxScript(Animatable* a, const wchar_t* varName)
{
	if (a) {
		AnimHandle h = a->GetHandleByAnim(a);
		TSTR strScript;
		strScript.printf(_T("%s = GetAnimByHandle %i"), varName, h);
		ExecuteMAXScriptScript(strScript.data());
	}
}

void ToMaxScript(const wchar_t* msg)
{
	CharStream* out = thread_local(current_stdout);
	out->printf(msg);
}

TatViewportCore::TatViewportCore()
{
	dlxclass_init();
}

TatViewportCore::~TatViewportCore()
{

}

TatGuideDisplay* TatViewportCore::GetGuide(int viewId, BOOL autoCreate, BOOL debug)
{
	CharStream* out = thread_local(current_stdout);

	for (int i = 0; i < _numGuides; i++)
	{
		if (_guides[i].viewId == viewId)
		{
			if (debug) {
				DebugPrint(_T("_guides[%i] %i == %i \n"), i, _guides[i].viewId, viewId);
			}
			return &_guides[i];
		}
	}

	if (autoCreate)
	{
		TatGuideDisplay* tmp = new TatGuideDisplay[_numGuides + 1];
		for (int i = 0; i < _numGuides; i++)
		{
			tmp[i] = _guides[i];
		}
		_numGuides += 1;
		tmp[_numGuides - 1] = TatGuideDisplay{ viewId };
		_guides = tmp;
		if (debug)
		{
			DebugPrint(_T("Add new guide at index %i, TatGuideDisplay{ %i , gType::p_ruleOfThird, FALSE } \n"), _numGuides - 1, viewId);
		}
		return &_guides[_numGuides - 1];
	}
	return NULL;
}

void TatViewportCore::DisplayGuideCallBack(ViewExp* vpt)
{
	if (!vpt || !vpt->IsAlive())
	{
		DbgAssert(!"Doing Display() on inactive viewport!");
		ToMaxScript(_T("Doing Display() on inactive viewport!"));
		return;
	}
	gw = vpt->getGW();
	if (gw != NULL)
	{
		int viewId = vpt->GetViewID();
		BOOL notifyChangedViewport = _currentViewId != viewId;
		if (notifyChangedViewport)
		{
			_currentViewId = viewId;
			TatViewportUtility::GetInstance()->OnCurrentViewportChanged();
		}
		TatGuideDisplay* guide = GetGuide(viewId, FALSE, FALSE);
		if (guide != NULL && guide->isEnabled)
		{
			CharStream* out = thread_local(current_stdout);
			if (notifyChangedViewport)
			{
				DebugPrint(_T("Guide Enabled on viewId : %i, typeId : %i\n"), guide->viewId, guide->type);
			}
			gw->setTransform(Matrix3(1));
			gw->setColor(LINE_COLOR, _displayColor);

			int renderHeight = GetCOREInterface19()->GetRendHeight();
			int renderWidth = GetCOREInterface19()->GetRendWidth();
			float w = gw->getWinSizeX();
			float h = gw->getWinSizeX() * renderHeight / (float)renderWidth;
			float refH = (gw->getWinSizeY() - h) / 2;

			if (guide->type == gType::p_ruleOfThird)
			{
				SetRuleOfThird(w, h, refH);
			}
			else if (guide->type == gType::p_cross)
			{
				SetCross(w, h, refH);
			}
			else if (guide->type == gType::p_centerCross)
			{
				SetCenterCross(w, h, refH);
			}
			else if (guide->type == gType::p_goldenRatio)
			{
				SetGoldenRatio(w, h, refH);
			}
			else if (guide->type == gType::p_goldenTriangle)
			{
				SetGoldenTriangle(w, h, refH);
			}
			gw->updateScreen();
		}
	}
}

void TatViewportCore::RegisterViewportGuide(BOOL onOff)
{
	Interface19* it = GetCOREInterface19();

	if (onOff)
	{
		it->RegisterViewportDisplayCallback(FALSE, &_vpDispCB);
	}
	else
	{
		BOOL unregister = TRUE;
		for (int i = 0; i < _numGuides; i++)
		{
			if (_guides[i].isEnabled)
			{
				unregister = FALSE;
			}
		}
		if (unregister)
		{
			it->UnRegisterViewportDisplayCallback(FALSE, &_vpDispCB);
		}
	}
	it->ForceCompleteRedraw();
}

void TatViewportCore::DisplayGuide(int viewId, BOOL onOff, int displayType )
{
	TatGuideDisplay* guide = GetGuide(viewId);
	if (guide != NULL)
	{
		guide->viewId = viewId;
		guide->isEnabled = onOff;
		if (displayType >= 0 && displayType < NUM_GUIDETYPES)
		{
			guide->type = displayType;
		}

		RegisterViewportGuide(guide->isEnabled);
	}
}

void TatViewportCore::ToggleDisplayGuides(int viewId, int displayType)
{
	BOOL onOff = !IsGuideDisplayed(viewId);
	DisplayGuide(viewId, onOff, displayType);
}

void TatViewportCore::SetGuideType(int viewId, int displayType)
{
	if (displayType >= 0 && displayType < NUM_GUIDETYPES)
	{
		TatGuideDisplay* guide = GetGuide(viewId);
		if (guide != NULL)
		{
			guide->type = displayType;
			GetCOREInterface19()->ForceCompleteRedraw();
		}
	}
}

int TatViewportCore::GetGuideType(int viewId)
{
	TatGuideDisplay* guide = GetGuide(viewId);
	if (guide != NULL)
	{
		return guide->type;
	}
	return -1;
}

BOOL TatViewportCore::IsGuideDisplayed(int viewId)
{
	TatGuideDisplay* guide = GetGuide(viewId);
	if (guide != NULL)
	{
		return guide->isEnabled;
	}
	return FALSE;
}

void TatViewportCore::SetRuleOfThird(float w, float h, float refH)
{
	float ph, pw;
	ph = refH + h / 3;
	IPoint3 line1[2];
	line1[0] = IPoint3(0, ph, 0);
	line1[1] = IPoint3(w, ph, 0);
	gw->hPolyline(2, line1, NULL, NULL, FALSE, NULL);

	ph = refH + 2 * h / 3;
	IPoint3 line2[2];
	line2[0] = IPoint3(0, ph, 0);
	line2[1] = IPoint3(w, ph, 0);
	gw->hPolyline(2, line2, NULL, NULL, FALSE, NULL);

	pw = w / 3;
	IPoint3 line3[2];
	line3[0] = IPoint3(pw, refH, 0);
	line3[1] = IPoint3(pw, refH + h, 0);
	gw->hPolyline(2, line3, NULL, NULL, FALSE, NULL);

	pw = 2 * w / 3;
	IPoint3 line4[2];
	line4[0] = IPoint3(pw, refH, 0);
	line4[1] = IPoint3(pw, refH + h, 0);
	gw->hPolyline(2, line4, NULL, NULL, FALSE, NULL);
}

void TatViewportCore::SetCross(float w, float h, float refH)
{
	IPoint3 line1[2];
	line1[0] = IPoint3(0, refH, 0);
	line1[1] = IPoint3(w, refH + h, 0);
	gw->hPolyline(2, line1, NULL, NULL, FALSE, NULL);

	IPoint3 line2[2];
	line2[0] = IPoint3(0, refH + h, 0);
	line2[1] = IPoint3(w, refH, 0);
	gw->hPolyline(2, line2, NULL, NULL, FALSE, NULL);
}

void TatViewportCore::SetCenterCross(float w, float h, float refH)
{
	float ph, pw;
	pw = w / 2;
	IPoint3 line1[2];
	line1[0] = IPoint3(pw, refH, 0);
	line1[1] = IPoint3(pw, refH + h, 0);
	gw->hPolyline(2, line1, NULL, NULL, FALSE, NULL);

	ph = refH + h / 2;
	IPoint3 line2[2];
	line2[0] = IPoint3(0, ph, 0);
	line2[1] = IPoint3(w, ph, 0);
	gw->hPolyline(2, line2, NULL, NULL, FALSE, NULL);
}

void TatViewportCore::SetGoldenRatio(float w, float h, float refH)
{
	float ph, pw;
	ph = refH + h / _goldenRatio;
	IPoint3 line1[2];
	line1[0] = IPoint3(0, ph, 0);
	line1[1] = IPoint3(w, ph, 0);
	gw->hPolyline(2, line1, NULL, NULL, FALSE, NULL);

	ph = refH + h - h / _goldenRatio;
	IPoint3 line2[2];
	line2[0] = IPoint3(0, ph, 0);
	line2[1] = IPoint3(w, ph, 0);
	gw->hPolyline(2, line2, NULL, NULL, FALSE, NULL);

	pw = w / _goldenRatio;
	IPoint3 line3[2];
	line3[0] = IPoint3(pw, refH, 0);
	line3[1] = IPoint3(pw, refH + h, 0);
	gw->hPolyline(2, line3, NULL, NULL, FALSE, NULL);

	pw = w - w / _goldenRatio;
	IPoint3 line4[2];
	line4[0] = IPoint3(pw, refH, 0);
	line4[1] = IPoint3(pw, refH + h, 0);
	gw->hPolyline(2, line4, NULL, NULL, FALSE, NULL);
}

void TatViewportCore::SetGoldenTriangle(float w, float h, float refH)
{
	IPoint3 line1[2];
	line1[0] = IPoint3(0, refH, 0);
	line1[1] = IPoint3(w, refH + h, 0);
	gw->hPolyline(2, line1, NULL, NULL, FALSE, NULL);

	IPoint3 line2[2];
	line2[0] = IPoint3(0, refH + h, 0);
	line2[1] = IPoint3(w - w / _goldenRatio, refH + h - h / _goldenRatio, 0);
	gw->hPolyline(2, line2, NULL, NULL, FALSE, NULL);

	IPoint3 line3[2];
	line3[0] = IPoint3(w, refH, 0);
	line3[1] = IPoint3(w / _goldenRatio, refH + h / _goldenRatio, 0);
	gw->hPolyline(2, line3, NULL, NULL, FALSE, NULL);
}


void TatViewportDisplayCallback::Display(TimeValue t, ViewExp* vpt, int flags)
{
	TatViewportCore* vCore = TatViewportCore::GetInstance();
	vCore->DisplayGuideCallBack(vpt);
}

void TatViewportDisplayCallback::GetViewportRect(TimeValue t, ViewExp* vpt, Rect* rect)
{

	if (!vpt || !vpt->IsAlive())
	{
		DbgAssert(!_T("Doing GetViewportRect() on inactive viewport!"));
		return;
	}
	//DebugPrint(_T("ViewportDisplayCallback::GetViewportRect\n"));
}

BOOL TatViewportDisplayCallback::Foreground()
{
	//DebugPrint(_T("ViewportDisplayCallback::Foreground\n")); 
	return FALSE;
}


