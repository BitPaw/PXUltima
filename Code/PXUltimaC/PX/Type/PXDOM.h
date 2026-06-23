#pragma once

#ifndef PXDOMIncluded
#define PXDOMIncluded

#include <PX/Type/PXType.h>
#include <PX/Engine/ECS/PXECS.h>
#include <PX/Container/List/PXList.h>
#include <PX/Compiler/PXCompiler.h>

typedef PXI16U PXDOMIndex;

// Keys are seperate namespace entrys.
typedef struct PXDOMKey_
{
    PXCompilerSymbolEntry Symbol;
}
PXDOMKey;

typedef struct PXDOMKeyStack_
{
    PXDOMIndex PartIndexFirst; // index into g_keyParts[]
    PXDOMIndex PartIndexCount; // number of parts  
}
PXDOMKeyStack;

// KV entry: flattened key (via indices) + value slice
typedef struct PXDOMProperty_
{
    PXCompilerSymbolEntry Symbol; // slice into file buffer

    PXDOMKeyStack KeyStack;

    PXDOMIndex ParentObjectIndex;
}
PXDOMProperty;

// Image object
typedef struct PXDOMClass_
{
    // Namespace build from this
    PXDOMKeyStack KeyStack;
}
PXDOMClass;

typedef struct PXDOMObject_
{
    PXDOMIndex InstanceID;

    PXDOMIndex ClassIndex;

    PXDOMIndex PropertyIndexStart;        // first KV index
    PXDOMIndex PropertyIndexCount;          // last KV index

    PXDOMIndex ObjectIndexParent;         // index of parent object (0xFFFF = none)
    PXDOMIndex ObjectIndexSibling;    // index of next object at same depth
    PXDOMIndex ObjectIndexChild;     // index of first child object
}
PXDOMObject;






// Document Object Model
// A container for nested key-value pairs.
typedef struct PXDOM_ PXDOM;






PXPublic PXResult PXAPI PXDOMCreate(PXDOM** pxDOMREF);
PXPublic PXResult PXAPI PXDOMRelease(PXDOM PXREF pxDOM);


//---------------------------------------------------------
// KEY
//---------------------------------------------------------
// Add a key-part slice to global pool
PXPublic PXSize PXAPI PXDOMKeyAmount(const PXDOM PXREF pxDOM);
PXPublic PXDOMKey* PXAPI PXDOMKeyGet(const PXDOM PXREF pxDOM, const PXDOMIndex index);
PXPublic PXDOMIndex PXAPI PXDOMKeyAdd(PXDOM PXREF pxDOM, PXCompilerSymbolEntry PXREF pxCompilerSymbolEntry);
PXPublic PXBool PXAPI PXDOMKeyCompare(const PXDOMKey PXREF pxDOMClassA, const PXCompilerSymbolEntry PXREF pxCompilerSymbolEntry);
PXPublic PXSize PXAPI PXDOMKeyFindFirst(const PXDOM PXREF pxDOM, const PXCompilerSymbolEntry PXREF pxCompilerSymbolEntry);
PXPublic PXSize PXAPI PXDOMKeyFindLast(const PXDOM PXREF pxDOM, const PXCompilerSymbolEntry PXREF pxCompilerSymbolEntry);
PXPublic PXResult PXAPI PXDOMKeyToString
(
    PXDOM PXREF pxDOM,
    PXText PXREF pxText,
    const PXSize index
);
//---------------------------------------------------------



//---------------------------------------------------------
// Property
//---------------------------------------------------------
PXPublic PXDOMIndex PXAPI PXDOMPropertyAmount(const PXDOM PXREF pxDOM);
PXPublic PXDOMProperty* PXAPI PXDOMPropertyGet(PXDOM PXREF pxDOM, const PXDOMIndex index);
PXPublic void PXAPI PXDOMPropertyAdd
(
    PXDOM PXREF pxDOM,
    PXDOMPrefixStack PXREF pxDOMPrefixStack,
    PXCompilerSymbolEntry PXREF pxCompilerSymbolEntryKey,
    PXCompilerSymbolEntry PXREF pxCompilerSymbolEntryValue
);
PXPublic PXResult PXAPI PXDOMPropertyToString
(
    PXDOM PXREF pxDOM,
    PXDOMProperty PXREF pxDOMProperty,
    PXText PXREF pxTextKey,
    PXText PXREF pxTextValue,
    const PXBool fullName
);
//---------------------------------------------------------


PXPublic void PXAPI PXDOMStackStore
(
    PXDOM PXREF pxDOM,
    const PXDOMPrefixStack PXREF pxDOMPrefixStack,
    PXDOMKeyStack PXREF pxDOMKeyStack
);


//---------------------------------------------------------
// Object
//---------------------------------------------------------
PXPublic PXDOMIndex PXAPI PXDOMObjectAmount(const PXDOM PXREF pxDOM);
PXPublic PXDOMObject* PXAPI PXDOMObjectGet(const PXDOM PXREF pxDOM, const PXDOMIndex index);
PXPublic PXDOMIndex PXAPI PXDOMObjectAdd(PXDOM PXREF pxDOM, PXDOMPrefixStack PXREF pxDOMPrefixStack, PXCompilerSymbolEntry PXREF pxCompilerSymbolEntry);
PXPublic PXResult PXAPI PXDOMObjectToString
(
    PXDOM PXREF pxDOM,
    PXDOMObject PXREF pxDOMClass,
    PXText PXREF pxText,
    const PXBool fullName
);
//---------------------------------------------------------



//---------------------------------------------------------
// Class
//---------------------------------------------------------
PXPublic PXSize PXAPI PXDOMClassAmount(const PXDOM PXREF pxDOM);
PXPublic PXDOMClass* PXAPI PXDOMClassGet(const PXDOM PXREF pxDOM, const PXSize index);
PXPublic PXDOMIndex PXAPI PXDOMClassAdd(PXDOM PXREF pxDOM, PXDOMPrefixStack PXREF pxDOMPrefixStack, PXCompilerSymbolEntry PXREF pxCompilerSymbolEntry);
PXPublic PXSize PXAPI PXDOMClassFind(const PXDOM PXREF pxDOM, PXDOMPrefixStack PXREF pxDOMPrefixStack);
PXPublic PXResult PXAPI PXDOMClassToString
(
    PXDOM PXREF pxDOM,
    PXDOMClass PXREF pxDOMClass,
    PXText PXREF pxText,
    const PXBool fullName
);
//---------------------------------------------------------



//---------------------------------------------------------
// Move
//---------------------------------------------------------
typedef struct PXDOMCursor_
{
    PXDOMIndex current; // or loop all roots
    int indent;

    PXDOMObject* obj;
    PXDOMClass* cls;
    PXDOMKey className;
    PXDOMProperty* prop;
    PXDOMKey propName;

    // Property counter
    PXDOMIndex k;
    PXSize index; 
}
PXDOMCursor;

PXPublic void PXAPI PXDOMCursorStart(PXDOM PXREF pxDOM, PXDOMCursor PXREF pxDOMCursor);
PXPublic void PXAPI PXDOMCursorNext(PXDOM PXREF pxDOM, PXDOMCursor PXREF pxDOMCursor);
PXPublic PXBool PXAPI PXDOMCursorUpdate(PXDOM PXREF pxDOM, PXDOMCursor PXREF pxDOMCursor);

PXPublic PXSize PXAPI PXDOMCursorKeyCompare
(
    PXDOM PXREF pxDOM,
    PXDOMCursor PXREF pxDOMCursor,
    const char** keyList,
    const PXSize keyAmount
);
PXPublic PXSize PXAPI PXDOMCursorPropertyCompare
(
    PXDOM PXREF pxDOM,
    PXDOMCursor PXREF pxDOMCursor,
    const char** keyList,
    const PXSize keyAmount
);


PXPublic PXBool PXAPI PXDOMCursorPropertyUpdate(PXDOM PXREF pxDOM, PXDOMCursor PXREF pxDOMCursor);
PXPublic void PXAPI PXDOMCursorPropertyNext(PXDOM PXREF pxDOM, PXDOMCursor PXREF pxDOMCursor);

PXPublic void PXAPI PXDOMPrintTreeIterative(PXDOM PXREF pxDOM);
//---------------------------------------------------------


PXPublic PXDOMIndex PXAPI PXDOMPrefixIndentCurrent(const PXDOMPrefixStack PXREF pxDOMPrefixStack);

#endif