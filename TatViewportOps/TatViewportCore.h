#pragma once

#include "3dsmaxsdk_preinclude.h"
#include "resource.h"
#include <istdplug.h>
#include <iparamb2.h>
#include <iparamm2.h>
#include <maxtypes.h>
//SIMPLE TYPE

#include <utilapi.h>

#include <maxscript/maxscript.h>
#include <maxscript/foundation/numbers.h>
#include <maxscript/foundation/3dmath.h>
#include <maxscript/maxwrapper/mxsobjects.h>
#include <maxscript/foundation/strings.h>
#include <maxscript/foundation/arrays.h>
#include <maxscript/foundation/hashtable.h>
#include <maxscript/maxwrapper/mxsmaterial.h>
#include <maxscript/maxwrapper/maxclasses.h>
#include <maxscript/foundation/colors.h>
#include <maxscript/compiler/parser.h>
#include <maxscript/util/listener.h>
#include <maxscript/foundation/structs.h>
#include <maxscript/mxsplugin/mxsplugin.h>
#include <maxscript/mxsplugin/mxsCustomAttributes.h>
#include <maxscript/foundation/functions.h>
#include <maxscript/compiler/thunks.h>
#include <maxscript/maxwrapper/bitmaps.h>
#include <maxscript/util/mxsZipPackage.h>
#include <maxscript/foundation/DataPair.h>
#include <maxscript/UI/rollouts.h>
#include <trig.h>
#include <vector>

#ifdef ScripterExport
#undef ScripterExport
#endif
#define ScripterExport __declspec( dllexport )

void dlxclass_init();

enum gType {
	p_ruleOfThird,
	p_cross,
	p_centerCross,
	p_goldenRatio,
	p_goldenTriangle
};

#define NUM_GUIDETYPES  5 	

#define def_guideTypes() \
	mxs_value_id_pair guideTypes[] = { \
		{gType::p_ruleOfThird, n_ruleOfThird}, \
		{gType::p_cross,  n_cross}, \
		{gType::p_centerCross,  n_centerCross}, \
		{gType::p_goldenRatio,  n_goldenRatio}, \
		{gType::p_goldenTriangle,  n_goldenTriangle}, \
};

#define elements(array)			(_countof(array))

inline	BOOL StrEqual(TCHAR* str1, TCHAR* str2) {
	return (_tcsicmp(str1, str2) == 0);
}

extern TCHAR* GetString(int id);
extern HINSTANCE hInstance;

class TatViewportDisplayCallback :public ViewportDisplayCallback {

private:
	void Display(TimeValue t, ViewExp* vpt, int flags);
	void GetViewportRect(TimeValue t, ViewExp* vpt, Rect* rect);
	BOOL Foreground();

};

struct TatGuideDisplay
{
	int	viewId;			
	int type = gType::p_ruleOfThird;
	BOOL isEnabled = FALSE;
};

class TatViewportCore  {
public:
	// Constructor/Destructor
	TatViewportCore();
	virtual ~TatViewportCore();

	void SetGuideType(int viewId, int displayType);
	int  GetGuideType(int viewId);
	
	void DisplayGuide(int viewId, BOOL onOff, int displayType = -1 );
	BOOL IsGuideDisplayed(int viewId);
	void ToggleDisplayGuides(int viewId, int displayType = -1);

	void DisplayGuideCallBack(ViewExp* vpt);
	TatGuideDisplay* GetGuide(int viewId , BOOL autoCreate = TRUE, BOOL debug = TRUE);

	// Singleton access
	static TatViewportCore* GetInstance() {
		static TatViewportCore theTatViewportCore;
		return &theTatViewportCore;
	}

private:
	int _currentViewId = 0;
	int _numGuides = 0;
	TatGuideDisplay* _guides;
	Color	_displayColor = Color(1.f, 0.f, 0.f);
	float	_goldenRatio = 1.61803f;
	GraphicsWindow* gw = NULL;
	TatViewportDisplayCallback _vpDispCB;

	void RegisterViewportGuide(BOOL onOff);
	void SetRuleOfThird(float w, float h, float refH);
	void SetCross(float w, float h, float refH);
	void SetCenterCross(float w, float h, float refH);
	void SetGoldenRatio(float w, float h, float refH);
	void SetGoldenTriangle(float w, float h, float refH);
};


