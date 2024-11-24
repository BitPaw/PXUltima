#ifndef PXTreeBehaviourDEFINED
#define PXTreeBehaviourDEFINED

#include <Media/PXType.h>

typedef void (PXAPI* PXTreeBehaviourNodeCallBack)(void* owner);

#define PXTreeBehaviourNodeFlagMask            0b00001111
#define PXTreeBehaviourNodeFlagActive          0b00000001 // If not, node will be ignored and thus skipped
#define PXTreeBehaviourNodeFlagCanAbord        0b00000010 // Can an action me canceled before or while it is running? Will not consume object
#define PXTreeBehaviourNodeFlagImportant       0b00000100 // Node shall be executed if possible. Useful when we want to swap threads or aboud, so we still execute this node 
#define PXTreeBehaviourNodeFlagRepeatOnFinish  0b00001000 // When the behaviour executed all nodes, should it start from the top?

typedef enum PXTreeBehaviourNodeType_
{
    PXTreeBehaviourNodeInvalid,
    PXTreeBehaviourNodeTaskSequence,
    PXTreeBehaviourNodeTaskSet,
    PXTreeBehaviourNodeSelector
}
PXTreeBehaviourNodeType;

typedef struct PXTreeBehaviourNode_
{
    // Info

    // Type
    void* CallBackOwner;
    PXTreeBehaviourNodeCallBack CallBack;

    PXTreeBehaviourNodeType Type;
}
PXTreeBehaviourNode;


typedef struct PXTreeBehaviour_
{
    // Hiracy of behaviour nodes
    

    // The amount of updates we successfully did in one update loop.
    // If we didn't do any updates for a whole iteration, lets sleep
    int UpdatesThisCycle; 
    int dummy;
}
PXTreeBehaviour;

#endif
