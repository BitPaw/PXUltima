#pragma once

#ifndef PXRuleSetIncluded
#define PXRuleSetIncluded

#include "PXRule.h"
#include <PX/Engine/PXGameEvent.h>
#include <PX/Container/List/PXList.h>

typedef struct PXRuleSet_
{
    PXListT(PXRule) RuleList;
} 
PXRuleSet;

PXPublic PXResult PXAPI PXRuleSetDispatch(PXRuleSet PXREF pxRuleSet, void PXREF owner, const PXGameEvent PXREF pxGameEvent);

#endif