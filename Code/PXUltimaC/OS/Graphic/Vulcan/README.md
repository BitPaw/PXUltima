# Vulcan
## Info
This is a modern cross platform graphics API. It reduces CPU bound "driver guessing" on how to handle interactions.<br>
Beeing more strait forward decreases complexity, thus allowing for faster execution, as the CPU does less work overall.<br>
This also implies, that this API will not improve performance if the system is bottlenecked by the GPU (graphics card).<br>
Inlcuding the complexity of using Vulcan and the lack of current global experience, using this API is not recommended. Nevertheless, Vulcan showed promissing performance increase on HI-End perfroming applications that are limited by multible hardware components.

## Usage
To use Vulcan, you need to link manually with the system librarys, if accessable.<br>
To do this, same as OpenGL, you need to open the Library and fetch the functions via name.

There is a library that does this as a wrapper for you, named "[vk-bootstrap](https://github.com/charles-lunarg/vk-bootstrap)".
