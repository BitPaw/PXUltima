#include "PXRuleSet.h"

PXResult PXAPI PXRuleSetDispatch(PXRuleSet PXREF pxRuleSet, void PXREF owner, const PXGameEvent PXREF pxGameEvent)
{
	PXSize amount = PXListItemAmount(&pxRuleSet->RuleList);

	for(PXSize i = 0; i < amount; i++)
	{
		PXRule PXREF pxRule = PXListItemAtIndexGetT(PXRule, &pxRuleSet->RuleList, i);
		PXRuleDefinition PXREF pxRuleDefinition = &pxRule->Definition;

		// If fetching did not work
		if(!pxRule)
		{
			continue;
		}

		// Trigger when this event is the one we want and we have a callback to handle
		PXBool isOK =
			(pxGameEvent->EventID == pxRuleDefinition->EventID) && pxRuleDefinition->HandlerCondition;

		// Go next if event does not match
		if(!isOK)
		{
			continue;
		}
		
		PXBool doesApply = pxRuleDefinition->HandlerCondition(owner, pxRule->Payload);

		// Go next if condition is not met
		if(!doesApply)
		{
			continue;
		}

		PXResult pxResult = pxRuleDefinition->HandlerAction(owner, pxRule->Payload);


	}

	return PXResultOK;
}