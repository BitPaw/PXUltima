# Operating system native drawing

## Info
Nativ drawing functions are the direct API calls of the system which are used to draw things like GUI elements.<br>
Internally, those calls will be redirected to the rendering pipline of the graphics card.

## Functions
|Description| Windows| Linux(X11) | Linux(Wayland)|
|:-|:-:|:-:|:-:|
|Create a "window", to render into. If it is global, it will be your normal window. If it is a child of that, it can be a UI element like a button. |CreateWindow|XCreateWindow|-|
|Scrolling effect to move the contents inside a window. This will effectively only provide a mechanism to change values with which you render the offset yourself.|[GetScrollInfo()](https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getscrollinfo) / [SetScrollInfo()](https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getscrollinfo)|-|-|

## Bash calls
### Linux(X11), List all fonts
```Bash
xlsfonts
```
