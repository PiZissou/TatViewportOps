# TatViewportOps

Copyright (c) TATProductions.

DESCRIPTION: Utility Plugin who extends 3dsmax viewport methods
AUTHOR: Pierre Lasbignes
DATE: 2021

# API

This Api can be used in 3dsmax with maxscript scripting language.

## File Path 
	.\TatViewportOps\TatViewportopsOps.cpp

## MAXSCRIPT

	(bool)		TatViewportOps.IsGuideDisplayed (int)  viewID 
	(name)		TatViewportOps.GetGuideType (int) viewID
	(bool)		TatViewportOps.SetGuideType (int) viewID  (name)  #ruleOfThird | #cross | #centerCross | #goldenRatio | #goldenTriangle 
	(bool)		TatViewportOps.DisplayGuide (int) viewID  (bool) display  type: #ruleOfThird | #cross | #centerCross | #goldenRatio | #goldenTriangle 
	(bool)		TatViewportOps.ToggleGuideDisplay  (int)  viewID   type: #ruleOfThird | #cross | #centerCross | #goldenRatio | #goldenTriangle 
	(TatGuideDisplay)	TatViewportOps.GetGuide  (int)  viewID 

## EXAMPLES
	TatViewportOps.GetGuideType viewport.activeViewportID
	TatViewportOps.DisplayGuide viewport.activeViewportID true type:#ruleOfThird

