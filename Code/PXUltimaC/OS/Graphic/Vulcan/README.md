# Vulcan
## Info
This is a modern cross platform graphics API.<br>
Not supported by older hardware as it is a fully different design.<br>
It reduces CPU bound "driver guessing" on how to handle interactions.<br>
Beeing more strait forward decreases complexity, thus allowing for faster execution, as the CPU does less work overall.<br>
This also implies, that this API will not improve performance if the system is bottlenecked by the GPU (graphics card).<br>
Inlcuding the complexity of using Vulcan and the lack of current global experience, using this API is not recommended.<br>
Nevertheless, Vulcan showed promissing performance increase on HI-End perfroming applications that are limited by multible hardware components.

## Usage
To use Vulcan, you need to link manually with the system librarys, if accessable.<br>
To do this, same as OpenGL, you need to open the Library and fetch the functions via name.

There is a library that does this as a wrapper for you, named "[vk-bootstrap](https://github.com/charles-lunarg/vk-bootstrap)".

Extensions contains a 'KHR' suffix, like OpenGL does it too.

## Rendering Setup

1. Open library manually via the OS specific method.
1. Fetch function **'vkGetInstanceProcAddr'** with that method.
1. Call function **'vkGetInstanceProcAddr'** to fetch **'vkCreateInstance'**
1. Call function **'vkCreateInstance'** to create the Vulkan instance.
1. Use the created instance to fetch all other functions with **'vkGetInstanceProcAddr'**
1. List all physical devices with **'vkEnumeratePhysicalDevices'** and select one device
1. Use selected physical device and create a virtial device from it using **'vkCreateDevice'**
1. If you want to render into a window, create new rendering surface with **'vkCreateWin32SurfaceKHR'**
1. If supported, you might scan the displays and select one with **'vkGetPhysicalDeviceDisplayPropertiesKHR'**

## Links
- [Vulkan Tutorial](https://vulkan-tutorial.com/Overview#page_Step-1-Instance-and-physical-device-selection)
- [Vulkan khronos Specification ](https://registry.khronos.org/vulkan/specs/1.3-extensions/pdf/vkspec.pdf)