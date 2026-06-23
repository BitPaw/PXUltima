#include "PXDOM.h"

#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include <PX/OS/Console/PXConsole.h>
#include <PX/OS/Memory/PXMemory.h>
#include <PX/OS/PXOS.h>
#include <PX/Compiler/PXCompiler.h>

#define PXDOMLogDebug 0

const PXDOMIndex PXDOMIndexInvalid = (PXDOMIndex)-1;
//#define PXDOMIndexInvalid (PXDOMIndex)-1

const char PXDOMText[] = "DOM";

typedef struct PXDOM_
{
    PXECSInfo Info;

    PXListT(PXDOMKey) ListKey;
    PXListT(PXDOMProperty) ListProperty;
    PXListT(PXDOMObject) ListObject;
    PXListT(PXDOMClass) ListClass;

    PXSize InstanceCounter;
}
PXDOM;

PXBool PXAPI PXDOMKeyCompare(const PXDOMKey PXREF pxDOMClass, const PXCompilerSymbolEntry PXREF pxCompilerSymbolEntry)
{
    PXBool isMatch = PXMemoryCompare
    (
        pxDOMClass->Symbol.Source,
        pxDOMClass->Symbol.Size,
        pxCompilerSymbolEntry->Source,
        pxCompilerSymbolEntry->Size,
        PXTrue
    );

    return isMatch;
}

PXDOMIndex PXAPI PXDOMKeyAdd(PXDOM PXREF pxDOM, PXCompilerSymbolEntry PXREF pxCompilerSymbolEntry)
{
    PXSize classIndexParent = 0;
    PXSize classIndexCurrent = 0;
    PXDOMKey* pxDOMClassCurrent = PXNull; 

    if(!(pxDOM && pxCompilerSymbolEntry))
    {
        return 0;
    }

    // lets check BEFORE creation if we already know this type, for dublicates.
    classIndexParent = PXDOMKeyFindLast(pxDOM, pxCompilerSymbolEntry);

    // Create new class-key AFTER search.
    // We always create one, dublicates are expected.

    // Get


    
    PXListEntry pxListEntry = PXListAdd(&pxDOM->ListKey, PXNull);
    pxDOMClassCurrent = (PXDOMKey*)pxListEntry.Address;
    classIndexCurrent = pxListEntry.Index;



    // Clear
    //pxDOMClassCurrent->Duplicates = 0;
    //pxDOMClassCurrent->IndexNext = 0;

    // Set
    pxDOMClassCurrent->Symbol = *pxCompilerSymbolEntry;


    PXBool isAlreadyKnown = PXDOMIndexInvalid != classIndexParent;

    if(isAlreadyKnown)
    {
        PXDOMKey PXREF pxDOMClassParent = PXDOMKeyGet(pxDOM, classIndexParent);

        //pxDOMClassParent->IndexNext = classIndexCurrent;
    }

    return classIndexCurrent;
}

PXDOMIndex PXAPI PXDOMObjectAmount(const PXDOM PXREF pxDOM)
{
    if(!pxDOM)
    {
        return 0;
    }

    PXDOMIndex amount = PXListItemAmount(&pxDOM->ListObject);

    return amount;
}

PXDOMObject* PXAPI PXDOMObjectGet(const PXDOM PXREF pxDOM, const PXDOMIndex index)
{
    PXDOMObject* pxDOMObject = (PXDOMObject*)PXListItemAtIndexGet(&pxDOM->ListObject, index);

    return pxDOMObject;
}

PXDOMIndex PXAPI PXDOMObjectAdd(PXDOM PXREF pxDOM, PXDOMPrefixStack PXREF pxDOMPrefixStack, PXCompilerSymbolEntry PXREF pxCompilerSymbolEntry)
{
    // New key, register.
    const PXDOMIndex keyIndex = PXDOMKeyAdd(pxDOM, pxCompilerSymbolEntry);

    // Add to stack 
    pxDOMPrefixStack->prefixStack[pxDOMPrefixStack->prefixDepth++] = keyIndex;

    // Check if we know the class already, keeping it unique
    PXSize classIndex = PXDOMClassAdd(pxDOM, pxDOMPrefixStack, pxCompilerSymbolEntry);

    // New class
    PXListEntry pxListEntry = PXListAdd(&pxDOM->ListObject, PXNull);
    PXDOMObject* pxDOMObject = (PXDOMObject*)pxListEntry.Address;
    pxDOMObject->ClassIndex = classIndex;
    pxDOMObject->InstanceID = pxDOM->InstanceCounter++;
    pxDOMObject->PropertyIndexStart = PXDOMIndexInvalid;
    pxDOMObject->PropertyIndexCount = 0;


    PXDOMIndex objIndex = pxDOMPrefixStack->objectDepth++;

    // 5) Determine depth
    PXDOMIndex depth = pxDOMPrefixStack->prefixDepth;

    // 6) Determine parent
    if(depth > 1)
        pxDOMObject->ObjectIndexParent = pxDOMPrefixStack->objectStack[depth - 2];
    else
        pxDOMObject->ObjectIndexParent = PXDOMIndexInvalid;

    // 7) Insert into parent's child list
    pxDOMObject->ObjectIndexChild = PXDOMIndexInvalid;
    pxDOMObject->ObjectIndexSibling = PXDOMIndexInvalid;

    if(pxDOMObject->ObjectIndexParent != PXDOMIndexInvalid)
    {
        PXDOMObject* parent = PXDOMObjectGet(pxDOM, pxDOMObject->ObjectIndexParent);

        if(parent->ObjectIndexChild == PXDOMIndexInvalid)
        {
            parent->ObjectIndexChild = objIndex;
        }
        else
        {
            PXDOMIndex s = parent->ObjectIndexChild;
            PXDOMObject* parentFirstChild = PXDOMObjectGet(pxDOM, s);

            while(parentFirstChild->ObjectIndexSibling != PXDOMIndexInvalid)
            {
                s = parentFirstChild->ObjectIndexSibling;
                parentFirstChild = PXDOMObjectGet(pxDOM, s);
            }

            parentFirstChild->ObjectIndexSibling = objIndex;
        }
    }

    // 8) Push this object onto object stack
    pxDOMPrefixStack->objectStack[depth - 1] = objIndex;

#if 0
    if(PXDOMIndexInvalid != classIndex)
    {
        // Class already known.

        PXDOMObject PXREF pxDOMClassPrevious = PXDOMObjectGet(pxDOM, classIndex);

        pxDOMClassPrevious->IndexNext = pxListEntry.Index;
    }
#endif

#if PXLogEnable && PXDOMLogDebug
    char buffer[128];
    PXText pxText;
    PXTextFromAdressA(&pxText, buffer, 0, sizeof(buffer));

    PXDOMObjectToString(pxDOM, pxDOMClass, &pxText);

    PXLogPrint
    (
        PXLoggingInfo,
        PXDOMText,
        "Reg",
        "%10s-ID:%2i, Lv:%2i, Key:%-30s (CLASS)",
        "Class", pxListEntry.Index,
        pxDOMPrefixStack->prefixDepth,
        buffer
    );
#endif

    return pxListEntry.Index;
}

PXResult PXAPI PXDOMObjectToString
(
    PXDOM PXREF pxDOM,
    PXDOMObject PXREF pxDOMObject,
    PXText PXREF pxText,
    const PXBool fullName
)
{
    if(!(pxDOM && pxDOMObject && pxText))
    {
        return PXResultRefusedParameterNull;
    }

    PXDOMClass PXREF pxDOMClass = PXDOMClassGet(pxDOM, pxDOMObject->ClassIndex);

    PXDOMClassToString(pxDOM, pxDOMClass, pxText, fullName);

    return PXResultOK;
}

PXSize PXAPI PXDOMClassAmount(const PXDOM PXREF pxDOM)
{
    if(!pxDOM)
    {
        return 0;
    }

    PXSize amount = PXListItemAmount(&pxDOM->ListClass);

    return amount;
}

PXDOMClass* PXAPI PXDOMClassGet(const PXDOM PXREF pxDOM, const PXSize index)
{
    PXDOMClass* pxDOMClass = (PXDOMClass*)PXListItemAtIndexGet(&pxDOM->ListClass, index);

    return pxDOMClass;
}

PXDOMIndex PXAPI PXDOMClassAdd(PXDOM PXREF pxDOM, PXDOMPrefixStack PXREF pxDOMPrefixStack, PXCompilerSymbolEntry PXREF pxCompilerSymbolEntry)
{
    const PXSize classIndex = PXDOMClassFind(pxDOM, pxDOMPrefixStack);

    if(PXDOMIndexInvalid != classIndex)
    {
        return classIndex;
    }

    PXListEntry pxListEntry = PXListAdd(&pxDOM->ListClass, PXNull);
    PXDOMClass* pxDOMClass = (PXDOMClass*)pxListEntry.Address;

    PXDOMStackStore(pxDOM, pxDOMPrefixStack, &pxDOMClass->KeyStack);

    return pxListEntry.Index;
}

PXSize PXAPI PXDOMClassFind(const PXDOM PXREF pxDOM, PXDOMPrefixStack PXREF pxDOMPrefixStack)
{
    const PXSize amount = PXDOMClassAmount(pxDOM);

    PXSize targetClass = PXDOMIndexInvalid;

    for(PXSize classIndex = 0; classIndex < amount; ++classIndex)
    {
        PXSize classIndexInverted = (amount - 1) - classIndex;
        const PXDOMClass PXREF pxDOMClass = PXDOMClassGet(pxDOM, classIndexInverted);

        PXBool sameLength = pxDOMClass->KeyStack.PartIndexCount == pxDOMPrefixStack->prefixDepth;

        if(!sameLength)
        {
            continue;
        }

        PXSize depth = pxDOMPrefixStack->prefixDepth;
        PXBool isEqual = PXFalse;

        for(PXSize depthIndex = 0; depthIndex < depth; depthIndex++)
        {
            PXDOMKey PXREF keyA = PXDOMKeyGet(pxDOM, pxDOMClass->KeyStack.PartIndexFirst + depthIndex);
            PXDOMKey PXREF keyB = PXDOMKeyGet(pxDOM, pxDOMPrefixStack->prefixStack[depthIndex]);

            isEqual = PXMemoryCompare
            (
                keyA->Symbol.Source,
                keyA->Symbol.Size,
                keyB->Symbol.Source,
                keyB->Symbol.Size,
                PXTrue
            );

            if(!isEqual)
            {
                break;
            }
        }

        if(!isEqual)
        {
            continue;
        }

        targetClass = classIndexInverted;
        break;
    }

    return targetClass;
}

PXResult PXAPI PXDOMClassToString
(
    PXDOM PXREF pxDOM, 
    PXDOMClass PXREF pxDOMClass,
    PXText PXREF pxText,
    const PXBool fullName
)
{
    if(!(pxDOM && pxDOMClass && pxText))
    {
        return PXResultRefusedParameterNull;
    }

    const PXDOMIndex partIndexCount = pxDOMClass->KeyStack.PartIndexCount;

    PXSize i = 0;

    if(!fullName)
    {
        i = partIndexCount - 1;
    }

    for(; i < partIndexCount; ++i)
    {
        PXSize index = pxDOMClass->KeyStack.PartIndexFirst + i;
        PXDOMKey PXREF keyParent = PXDOMKeyGet(pxDOM, index);
        PXBool isLastEntry = i == (partIndexCount - 1);

        PXTextAppendA(pxText, keyParent->Symbol.Source, keyParent->Symbol.Size);

        if(!isLastEntry)
        {
            PXTextAppendC(pxText, '.');
        }
    }

    return PXResultOK;
}

void PXAPI PXDOMStackStore
(
    PXDOM PXREF pxDOM,
    const PXDOMPrefixStack PXREF pxDOMPrefixStack,
    PXDOMKeyStack PXREF pxDOMKeyStack
)
{
    // First part index is where we start writing key parts
    PXDOMIndex indexList[128];

    // Copy prefix parts
    for(PXI8U i = 0; i < pxDOMPrefixStack->prefixDepth; i++)
    {
        PXDOMKey* pxDOMKey = PXDOMKeyGet(pxDOM, pxDOMPrefixStack->prefixStack[i]);

        indexList[i] = PXDOMKeyAdd(pxDOM, &pxDOMKey->Symbol);

#if PXLogEnable && PXDOMLogDebug
        char bufferValue[128];
        PXText pxTextValue;
        PXTextFromAdressA(&pxTextValue, bufferValue, 0, sizeof(bufferValue));

        PXTextAppendA(&pxTextValue, pxDOMKey->Symbol.Source, pxDOMKey->Symbol.Size);

        PXLogPrint
        (
            PXLoggingError,
            PXDOMText,
            "Stack",
            "[%i] -> [%i] (%s)",
            i,
            indexList[i],
            bufferValue
        );
#endif
    }

    // Add the final key part
   // pxListEntry = PXListAdd(&pxDOM->g_keyParts, PXNull);
   // PXDOMKey* pxDOMKey = pxListEntry.Address;

   // pxDOMKey->Slice = *key;

    pxDOMKeyStack->PartIndexFirst = indexList[0];
    pxDOMKeyStack->PartIndexCount = pxDOMPrefixStack->prefixDepth; // +1?
}

void PXAPI PXDOMPropertyAdd
(
    PXDOM PXREF pxDOM,
    PXDOMPrefixStack PXREF pxDOMPrefixStack,
    PXCompilerSymbolEntry PXREF pxCompilerSymbolEntryKey,
    PXCompilerSymbolEntry PXREF pxCompilerSymbolEntryValue
)
{
    // Create property
    PXListEntry pxListEntry = PXListAdd(&pxDOM->ListProperty, PXNull);
    PXDOMProperty* pxDOMProperty = (PXDOMProperty*)pxListEntry.Address;

    // Store property symbol value
    pxDOMProperty->Symbol = *pxCompilerSymbolEntryValue;

    // Store key as full propety path
    PXDOMIndex keyIndex = PXDOMKeyAdd(pxDOM, pxCompilerSymbolEntryKey);

    pxDOMPrefixStack->prefixStack[pxDOMPrefixStack->prefixDepth++] = keyIndex;

    PXDOMStackStore(pxDOM, pxDOMPrefixStack, &pxDOMProperty->KeyStack);

    pxDOMPrefixStack->prefixStack[pxDOMPrefixStack->prefixDepth--] = 0;


    // We also need to update our current object
    PXDOMIndex objectIDCurrent = pxDOMPrefixStack->objectStack[pxDOMPrefixStack->prefixDepth-1];
    PXDOMObject PXREF pxDOMObject = PXDOMObjectGet(pxDOM, objectIDCurrent);

    if(pxDOMObject->PropertyIndexCount == 0)
    {
        pxDOMObject->PropertyIndexStart = pxListEntry.Index;
    }

    ++pxDOMObject->PropertyIndexCount;


    pxDOMProperty->ParentObjectIndex = objectIDCurrent;

#if PXDOMLogDebug && 0
    for(size_t i = 0; i < pxDOMPrefixStack->prefixDepth; i++)
    {
        PXDOMIndex obejctID = pxDOMPrefixStack->objectStack[i];
        PXDOMObject* parent = PXDOMObjectGet(pxDOM, i);

        char textBuffer[128];
        PXText pxText;
        PXTextFromAdressA(&pxText, textBuffer, 0, sizeof(textBuffer));

        PXDOMObjectToString(pxDOM, parent, &pxText);

        PXLogPrint
        (
            PXLoggingInfo,
            PXDOMText,
            "Push",
            "Stack : [%i] %s (%i)",
            i,
            pxText.A,
            parent->InstanceID
        );
    }
#endif

#if PXLogEnable && PXDOMLogDebug
    char bufferKey[128];
    PXText pxTextKey;
    char bufferValue[128];
    PXText pxTextValue;
    PXTextFromAdressA(&pxTextKey, bufferKey, 0, sizeof(bufferKey));
    PXTextFromAdressA(&pxTextValue, bufferValue, 0, sizeof(bufferValue));

    PXDOMPropertyToString(pxDOM, pxDOMProperty, &pxTextKey, &pxTextValue);

    PXLogPrint
    (
        PXLoggingInfo,
        PXDOMText,
        "Push",
        "%10s-ID:%2i, Lv:%2i, K:%-30s V:%-15s",
        "Property", pxListEntry.Index,
        pxDOMPrefixStack->prefixDepth,
        pxTextKey.A,
        pxTextValue.A
    );
#endif
}

static inline PXBool same_prefix(PXDOM PXREF pxDOM, const PXDOMProperty* a, const PXDOMProperty* b)
{
    if(a->KeyStack.PartIndexCount < 2 || b->KeyStack.PartIndexCount < 2)
        return PXFalse;

    PXSize prefixLen = a->KeyStack.PartIndexCount - 1;

    if(prefixLen != b->KeyStack.PartIndexCount - 1)
        return PXFalse;

    for(PXSize i = 0; i < prefixLen; i++)
    {
        PXDOMKey* sa = PXDOMKeyGet(pxDOM, a->KeyStack.PartIndexFirst + i);
        PXDOMKey* sb = PXDOMKeyGet(pxDOM, b->KeyStack.PartIndexFirst + i);
        if(!PXDOMKeyCompare(sa, &sb->Symbol))
            return PXFalse;
    }

    return PXTrue;
}

PXSize PXAPI PXDOMKeyFindFirst(const PXDOM PXREF pxDOM, const PXCompilerSymbolEntry PXREF key)
{
    const PXSize keysAmount = PXDOMKeyAmount(pxDOM);
    const PXBool hasEnoughKeys = keysAmount > 0;

    if(!hasEnoughKeys)
    {
        return PXDOMIndexInvalid;
    }

    // Check previous KV for duplicate prefix
    for(PXSize i = 0; i < keysAmount; ++i)
    {
        const PXDOMKey PXREF pxDOMClass = PXDOMKeyGet(pxDOM, i);
        const PXBool isMatch = PXDOMKeyCompare(pxDOMClass, key);

        if(isMatch)
        {
            return i;
        }
    }

    return PXDOMIndexInvalid;
}

PXSize PXAPI PXDOMKeyFindLast(const PXDOM PXREF pxDOM, const PXCompilerSymbolEntry PXREF key)
{
    const PXSize keysAmount = PXDOMKeyAmount(pxDOM);
    const PXBool hasEnoughKeys = keysAmount > 0;

    if(!hasEnoughKeys)
    {
        return PXDOMIndexInvalid;
    }

    // Check previous KV for duplicate prefix
    for(PXSize i = 0; i < keysAmount; ++i)
    {
        const PXSize index = (keysAmount - 1) - i;
        const PXDOMKey PXREF pxDOMClass = PXDOMKeyGet(pxDOM, index);
        const PXBool isMatch = PXDOMKeyCompare(pxDOMClass, key);

        if(isMatch)
        {
            return index;
        }
    }

    return PXDOMIndexInvalid;
}

PXDOMIndex PXAPI PXDOMPropertyAmount(const PXDOM PXREF pxDOM)
{
    PXDOMIndex amount = PXListItemAmount(&pxDOM->ListProperty);

    return amount;
}

PXDOMProperty* PXAPI PXDOMPropertyGet(PXDOM PXREF pxDOM, const PXDOMIndex index)
{
    PXDOMProperty* pxDOMProperty = (PXDOMProperty*)PXListItemAtIndexGet(&pxDOM->ListProperty, index);

    return pxDOMProperty;
}

PXResult PXAPI PXDOMPropertyToString
(
    PXDOM PXREF pxDOM,
    PXDOMProperty PXREF pxDOMProperty,
    PXText PXREF pxText,
    PXText PXREF pxTextValue,
    const PXBool fullName
)
{
    if(!(pxDOM && pxDOMProperty && pxText))
    {
        return PXResultRefusedParameterNull;
    }

    PXTextClear(pxText);

    const PXDOMIndex partIndexCount = pxDOMProperty->KeyStack.PartIndexCount;

    PXSize i = 0;

    if(!fullName)
    {
        i = partIndexCount - 1;
    }

    for(; i < partIndexCount; ++i)
    {
        PXSize index = pxDOMProperty->KeyStack.PartIndexFirst + i;
        PXDOMKey PXREF keyParent = PXDOMKeyGet(pxDOM, index);
        PXBool isLastEntry = i == (partIndexCount - 1);

        PXTextAppendA(pxText, keyParent->Symbol.Source, keyParent->Symbol.Size);

        if(!isLastEntry)
        {
            PXTextAppendC(pxText, '.');
        }
    }

    if(!pxTextValue)
    {
        return PXResultOK;
    }

    PXTextClear(pxTextValue);
    PXCompilerSymbolValueToString(&pxDOMProperty->Symbol, pxTextValue);

    return PXResultOK;
}

PXResult PXAPI PXDOMCreate(PXDOM** pxDOMREF)
{
    PXDOM* pxDOM = PXNull;

    pxDOM = PXMemoryHeapCallocT(PXDOM, 1);

    *pxDOMREF = pxDOM;

    PXListCreate(&pxDOM->ListKey, sizeof(PXDOMKey), 2048);
    PXListCreate(&pxDOM->ListProperty, sizeof(PXDOMProperty), 1024);
    PXListCreate(&pxDOM->ListObject, sizeof(PXDOMObject), 256);
    PXListCreate(&pxDOM->ListClass, sizeof(PXDOMClass), 256);

    return PXResultOK;
}

PXResult PXAPI PXDOMRelease(PXDOM PXREF pxDOM)
{
    return PXResultOK;
}

PXSize PXAPI PXDOMKeyAmount(const PXDOM PXREF pxDOM)
{
    if(!pxDOM)
    {
        return 0;
    }

    PXSize amount = PXListItemAmount(&pxDOM->ListKey);

    return amount;
}

PXDOMKey* PXAPI PXDOMKeyGet(const PXDOM PXREF pxDOM, const PXDOMIndex index)
{
    PXDOMKey* pxDOMKey = (PXDOMKey*)PXListItemAtIndexGet(&pxDOM->ListKey, index);

    return pxDOMKey;
}

PXResult PXAPI PXDOMKeyToString(PXDOM PXREF pxDOM, PXText PXREF pxText, const PXSize index)
{
    const PXDOMKey PXREF key = PXDOMKeyGet(pxDOM, index);

    PXTextAppendA(pxText, key->Symbol.Source, key->Symbol.Size);

    return PXResultOK;
}

void PXAPI PXDOMCursorStart(PXDOM PXREF pxDOM, PXDOMCursor PXREF pxDOMCursor)
{
    PXClear(PXDOMCursor, pxDOMCursor);
}

void PXAPI PXDOMCursorNext(PXDOM PXREF pxDOM, PXDOMCursor PXREF pxDOMCursor)
{
    pxDOMCursor->k = 0;

    // Go deeper if possible
    if(pxDOMCursor->obj->ObjectIndexChild != PXDOMIndexInvalid)
    {
        pxDOMCursor->current = pxDOMCursor->obj->ObjectIndexChild;
        pxDOMCursor->indent++;
        return;
    }

    // Otherwise go sideways or up
    while(pxDOMCursor->current != PXDOMIndexInvalid && pxDOMCursor->obj->ObjectIndexSibling == PXDOMIndexInvalid)
    {
        pxDOMCursor->current = pxDOMCursor->obj->ObjectIndexParent;
        pxDOMCursor->indent--;

        if(pxDOMCursor->current != PXDOMIndexInvalid)
        {
            pxDOMCursor->obj = PXDOMObjectGet(pxDOM, pxDOMCursor->current);
        }
    }

    if(pxDOMCursor->current != PXDOMIndexInvalid)
    {
        pxDOMCursor->current = pxDOMCursor->obj->ObjectIndexSibling;
    }
}

PXBool PXAPI PXDOMCursorUpdate(PXDOM PXREF pxDOM, PXDOMCursor PXREF pxDOMCursor)
{
    if(pxDOMCursor->current == PXDOMIndexInvalid)
    {
        return PXFalse;
    }

    pxDOMCursor->obj = PXDOMObjectGet(pxDOM, pxDOMCursor->current);

    if(!pxDOMCursor->obj)
    {
        return PXFalse;
    }

    pxDOMCursor->cls = PXDOMClassGet(pxDOM, pxDOMCursor->obj->ClassIndex);

    if(!pxDOMCursor->cls)
    {
        return PXFalse;
    }

    const PXDOMIndex pxDOMIndex = pxDOMCursor->cls->KeyStack.PartIndexFirst + pxDOMCursor->cls->KeyStack.PartIndexCount - 1;
    PXDOMKey* pxDOMKey = PXDOMKeyGet(pxDOM, pxDOMIndex);

    if(pxDOMKey)
    {
        pxDOMCursor->className = *pxDOMKey;
    }

    return PXTrue;
}

PXSize PXAPI PXDOMCursorKeyCompare
(
    PXDOM PXREF pxDOM,
    PXDOMCursor PXREF pxDOMCursor, 
    const char** keyList, 
    const PXSize keyAmount
)
{
    PXDOMClass* obj = pxDOMCursor->cls;

    PXDOMKey* pxDOMKey = PXDOMKeyGet(pxDOM, obj->KeyStack.PartIndexFirst + obj->KeyStack.PartIndexCount -1);

    for(size_t i = 0; i < keyAmount; i++)
    {
        const char* key = keyList[i];

        PXBool isMatch = PXTextCompareA
        (
            key, 
            PXTextUnkownLength,
            pxDOMKey->Symbol.Source,
            pxDOMKey->Symbol.Size,
            PXTextCompareRequireSameLength
        );

        if(isMatch)
        {
            return i;
        }
    }    

    return -1;
}

PXSize PXAPI PXDOMCursorPropertyCompare(PXDOM PXREF pxDOM, PXDOMCursor PXREF pxDOMCursor, const char** keyList, const PXSize keyAmount)
{
    PXDOMProperty* pxDOMProperty = pxDOMCursor->prop;

    PXDOMKey* pxDOMKey = PXDOMKeyGet(pxDOM, pxDOMProperty->KeyStack.PartIndexFirst + pxDOMProperty->KeyStack.PartIndexCount - 1);

    for(size_t i = 0; i < keyAmount; i++)
    {
        const char* key = keyList[i];

        PXBool isMatch = PXTextCompareA
        (
            key,
            PXTextUnkownLength,
            pxDOMKey->Symbol.Source,
            pxDOMKey->Symbol.Size,
            PXTextCompareRequireSameLength
        );

        if(isMatch)
        {
            return i;
        }
    }

    return -1;
}

PXBool PXAPI PXDOMCursorPropertyUpdate(PXDOM PXREF pxDOM, PXDOMCursor PXREF pxDOMCursor)
{
    PXDOMCursorUpdate(pxDOM, pxDOMCursor);

    PXBool invalidIndex =
        pxDOMCursor->k == PXDOMIndexInvalid ||
        pxDOMCursor->k >= pxDOMCursor->obj->PropertyIndexCount;

    if(invalidIndex)
    {
        pxDOMCursor->index = PXDOMIndexInvalid;
        pxDOMCursor->prop = PXNull;
        return PXFalse;
    }

    pxDOMCursor->index = pxDOMCursor->obj->PropertyIndexStart + pxDOMCursor->k;
    pxDOMCursor->prop = PXDOMPropertyGet(pxDOM, pxDOMCursor->index);

    pxDOMCursor->propName = *PXDOMKeyGet(pxDOM, pxDOMCursor->prop->KeyStack.PartIndexFirst + pxDOMCursor->prop->KeyStack.PartIndexCount - 1);

    //pxDOMCursor->obj = PXNull;
    //pxDOMCursor->cls = PXNull;

    if(!pxDOMCursor->prop)
    {
        return PXFalse;
    }

    return PXTrue;
}

void PXAPI PXDOMCursorPropertyNext(PXDOM PXREF pxDOM, PXDOMCursor PXREF pxDOMCursor)
{
    ++pxDOMCursor->k;
}

void PXAPI PXDOMPrintTreeIterative(PXDOM PXREF pxDOM)
{
    PXDOMCursor pxDOMCursor;
    PXDOMCursorStart(pxDOM, &pxDOMCursor);

    // Print class name
    char bufferKey[128];
    PXText pxTextKey;
    PXTextFromAdressA(&pxTextKey, bufferKey, 0, sizeof(bufferKey));

    char bufferValue[128];
    PXText pxTextValue;
    PXTextFromAdressA(&pxTextValue, bufferValue, 0, sizeof(bufferValue));

    while(PXDOMCursorUpdate(pxDOM, &pxDOMCursor))
    {
        // Print indentation
        for(int i = 0; i < pxDOMCursor.indent; i++)
            printf("  ");
      
        PXTextClear(&pxTextKey);
        PXDOMClassToString(pxDOM, pxDOMCursor.cls , &pxTextKey, PXFalse);

        printf("[%.*s]\n", pxTextKey.SizeUsed, pxTextKey.A);


#if 1
        while(PXDOMCursorPropertyUpdate(pxDOM, &pxDOMCursor))
        {
            for(int i = 0; i < pxDOMCursor.indent + 1; i++)
                printf("  ");

            PXDOMPropertyToString(pxDOM, pxDOMCursor.prop, &pxTextKey, &pxTextValue, PXFalse);

            printf
            (
                "%.*s: %.*s\n",
                pxTextKey.SizeUsed,
                pxTextKey.A,
                pxTextValue.SizeUsed,
                pxTextValue.A
            );

            PXDOMCursorPropertyNext(pxDOM, &pxDOMCursor);
        }
#endif

        PXDOMCursorNext(pxDOM, &pxDOMCursor);
    }
}

PXDOMIndex PXAPI PXDOMPrefixIndentCurrent(const PXDOMPrefixStack PXREF pxDOMPrefixStack)
{
    return pxDOMPrefixStack->indentStack[pxDOMPrefixStack->indentDepth];
}