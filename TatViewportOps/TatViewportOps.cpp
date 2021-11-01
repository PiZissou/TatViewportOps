#include "TatViewportCore.h"

#include <maxscript\macros\define_instantiation_functions.h>
#	include "namedefs.h"
#	include "TatViewportOps.h"

/*
TatViewportOps.GetGuideType viewport.activeViewportID
TatViewportOps.DisplayGuide viewport.activeViewportID true type:#ruleOfThird
*/

Value* DisplayGuide_cf(Value** arg_list, int count)
{
	check_arg_count_with_keys(TatViewportOps.DisplayGuide, 2, count);
	int viewId = arg_list[0]->to_int() - 1;
	BOOL onOff = arg_list[1]->to_bool();

	Value* displayType = key_arg(guideType);
	
	TatViewportCore* vCore = TatViewportCore::GetInstance();

	def_guideTypes();
	int index = ConvertValueToID(guideTypes, elements(guideTypes), displayType);
	vCore->DisplayGuide( viewId, onOff, index);

	CharStream* out = thread_local(current_stdout);
	out->printf(_T("gType = %s, n_ruleOfThird = %s, index = %i, isEqual = %i\n"),displayType->to_mstr(), n_ruleOfThird->to_mstr(), index , displayType == n_ruleOfThird);

	return bool_result(TRUE);
}

Value* IsGuideDisplayed_cf(Value** arg_list, int count)
{
	check_arg_count(TatViewportOps.IsGuideDisplayed, 1, count);
	int viewId = arg_list[0]->to_int() - 1;

	TatViewportCore* vCore = TatViewportCore::GetInstance();
	BOOL res = vCore->IsGuideDisplayed(viewId);

	return bool_result(res);
}

Value* ToggleGuideDisplay_cf(Value** arg_list, int count)
{
	check_arg_count_with_keys(TatViewportOps.ToggleGuideDisplay, 1, count);
	int viewId = arg_list[0]->to_int() - 1;

	Value* displayType = key_arg(guideType);

	def_guideTypes();
	int index = ConvertValueToID(guideTypes, elements(guideTypes), displayType);

	TatViewportCore* vCore = TatViewportCore::GetInstance();
	vCore->ToggleDisplayGuides(viewId , index);

	return bool_result(TRUE);
}

Value* SetGuideType_cf(Value** arg_list, int count)
{
	check_arg_count(TatViewportOps.SetGuideType, 2, count);
	int viewId = arg_list[0]->to_int() - 1;
	Value* displayType = arg_list[1];

	def_guideTypes();
	int index = ConvertValueToID(guideTypes, elements(guideTypes), displayType);

	TatViewportCore* vCore = TatViewportCore::GetInstance();
	vCore->SetGuideType(viewId, index);

	return bool_result(TRUE);
}

Value* GetGuideType_cf(Value** arg_list, int count)
{
	check_arg_count(TatViewportOps.GetGuideType, 1, count);
	int viewId = arg_list[0]->to_int() - 1;

	TatViewportCore* vCore = TatViewportCore::GetInstance();
	def_guideTypes();
	return ConvertIDToValue( guideTypes, elements(guideTypes),vCore->GetGuideType( viewId));
}

Value* GetGuide_cf(Value** arg_list, int count)
{
	check_arg_count(TatViewportOps.GetGuide, 1, count);
	int viewId = arg_list[0]->to_int() - 1;

	TatViewportCore* vCore = TatViewportCore::GetInstance();
	vCore->GetGuide(viewId);

	return bool_result(TRUE);
}

#include <maxscript\macros\local_implementations.h>
void dlxclass_init()
{
	def_name(guideType);

	def_name(ruleOfThird);
	def_name(cross);
	def_name(centerCross);
	def_name(goldenRatio);
	def_name(goldenTriangle);
}