# User Interface (UI)

## Windows
Common used elements, like Buttons, Text and others, are defined in __WinUser.h__ from the __USER32.DLL__.<br>
These elements, as stupid as it is, are created with the [__CreateWindow()__](https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindoww) function by passing a spesific name as a parameter.
The result is to be used closely like an actual window and has the same intended interaction behind it.


After we created our UI element, what now?<br>
We can send events, like a normal window, over the [__SendMessage()__](https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-sendmessage) function. 
These events will be stored in the event queue to be fetched form the function [__WindowProc()__](https://learn.microsoft.com/en-us/windows/win32/api/winuser/nc-winuser-wndproc) at some point.
Here, the events are parsed depending on the ID of the given event. 

Sadly, its not that easy, as it is not quite clear what events can be used with what element. Although documentation exists, its chaotic.

Note: 
- Strings used in message might usee the first 4 Bytes as the lengh. I don't know why.
- Some events are not send in different threads. so single threaded might be required?
- Coloring objects does not work as one might think. You can only do it in the event loop. 

### Examples
- [How to create a Button](https://learn.microsoft.com/en-us/windows/win32/controls/create-a-button)

### Elements
|Element|#define|Allowed messages|
|:-|:-:|:-:|
| Animation | yyyyyyyyy | yyyyyyyyyy| 
| Button | yyyyyyyyy | yyyyyyyyyy| 
| ComboBox | yyyyyyyyy | yyyyyyyyyy| 
| ComboBoxEx | yyyyyyyyy | yyyyyyyyyy| 
| Date and Time Picker | yyyyyyyyy | yyyyyyyyyy| 
| Edit | yyyyyyyyy | yyyyyyyyyy| 
| Flat Scroll Bar | yyyyyyyyy | yyyyyyyyyy| 
| Header Control | yyyyyyyyy | yyyyyyyyyy| 
| Hot Key | yyyyyyyyy | yyyyyyyyyy| 
| Image List | yyyyyyyyy | yyyyyyyyyy| 
| IP Address Control | yyyyyyyyy | yyyyyyyyyy| 
| List Box | yyyyyyyyy | yyyyyyyyyy| 
| List View | yyyyyyyyy | yyyyyyyyyy| 
| Month Calendar | yyyyyyyyy | yyyyyyyyyy| 
| Pager | yyyyyyyyy | yyyyyyyyyy| 
| Progress Bar | yyyyyyyyy | yyyyyyyyyy| 
| Property Sheet | yyyyyyyyy | yyyyyyyyyy| 
| Rebar | yyyyyyyyy | yyyyyyyyyy| 
| Rich Edit | yyyyyyyyy | yyyyyyyyyy| 
| Scroll Bar | yyyyyyyyy | yyyyyyyyyy| 
| Static Control | yyyyyyyyy | yyyyyyyyyy| 
| Status Bar | yyyyyyyyy | yyyyyyyyyy| 
| SysLink | yyyyyyyyy | yyyyyyyyyy| 
| Tab | yyyyyyyyy | yyyyyyyyyy| 
| Task Dialog | yyyyyyyyy | yyyyyyyyyy| 
| Toolbar | yyyyyyyyy | yyyyyyyyyy| 
| Tooltip | yyyyyyyyy | yyyyyyyyyy| 
| Trackbar | yyyyyyyyy | yyyyyyyyyy| 
| Tree View | yyyyyyyyy | yyyyyyyyyy| 
| Up-Down Control | yyyyyyyyy | yyyyyyyyyy| 


---

## Linux (X-System)
> ToDo