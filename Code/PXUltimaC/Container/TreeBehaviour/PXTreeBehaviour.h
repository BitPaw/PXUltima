#ifndef PXTreeBehaviourDEFINED
#define PXTreeBehaviourDEFINED

typedef (PXAPI* PXTreeBehaviourNodeCallBack)(void* owner);

#define PXTreeBehaviourNodeFlagMask
#define PXTreeBehaviourNodeFlagActive // If not, node will be ignored and skipped
#define PXTreeBehaviourNodeFlagCanAbord // Can an action me canceled before or while it is running? Will not consume object
#define PXTreeBehaviourNodeFlagImportant // Node shall be executed if possible. Useful when we want to swap threads or aboud, so we still execute this node 


typedef struct PXTreeBehaviourNode_
{
    
    PXTreeBehaviourNodeCallBack CallBack;
}
PXTreeBehaviourNode;


typedef struct PXTreeBehaviour_
{
    int dummy;
}
PXTreeBehaviour;

#endif
