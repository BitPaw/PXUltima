#pragma once

#ifndef PXRuleIncluded
#define PXRuleIncluded
#include <PX/Engine/ECS/PXECS.h>
#include <PX/Engine/PXGameEvent.h>

typedef PXBool (PXAPI* PXRuleConditionHandler)(void PXREF owner, void PXREF payload);
typedef PXResult (PXAPI* PXRuleActionHandler)(void PXREF owner, void PXREF payload);

typedef struct PXRuleDefinition_
{
    PXRuleConditionHandler HandlerCondition;
    PXRuleActionHandler HandlerAction;
    PXGameEventID EventID;
}
PXRuleDefinition;

/*
typedef struct PXRuleContext_
{
    PXCardID card;
    PXPlayerID player;
    PXGameEvent event;
    PXZoneID from_zone;
    PXZoneID to_zone;
} 
PXRuleContext;
*/

typedef struct PXRule_
{    
    PXECSInfo Info;

    PXRuleDefinition Definition;

    void* Payload;
} 
PXRule;

#endif