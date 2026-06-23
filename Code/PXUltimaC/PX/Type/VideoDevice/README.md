# Video

## Info
When we have rendered our image from our graphics card, whatever it is, we want to send it to someone as fast as we can.<br>
Sending this image data "as-is" our bandwidth would need to be ridiculously strong.

|FPS|Bytes| MB/s| MBit/s|
|:-:|:-:|-:|-:|
|1|6.220.800 |6.22 |49.78|
|30|186.624.000|186.62|1492.99|
|60|373.248.000|373.25|2985.98|
|120|746.496.000|746.50|5971.97|
|144|895.680.000|895.68|7166.36|

Because of this, it would be impossible for a normal internet connection to display any video at all. 
But we know it works, so how do we solve this? With encoding. We change the format, only send data that changes instead of "just everything".
That will allow us to reduce the amount needed of bandwidth by so much that we can watch videos smoothly with a fraction of it.

## SDKs for encoding & decoding Video 

|Name|Header|Static Lib|Dynamic Lib|Description|
|:-:|:-|:-:|:-:|:-:|
|Windows Media Foundation (WMF)|	mfapi.h, mftransform.h<br>Built into Windows, starting with Vista	| mfplat.lib |mfplat.dll|	vendor-agnostic<br>Only supports predefined formats|
|Intel Quick Sync (QSV)|	mfxvideo.h<br>(Media SDK)<br>Intel oneAPI Video Processing Library	|libmfx.lib		|	libmfxhw64.dll			|	Only for Intel hardware, usefull for the many Intel CPUs with iGPU. |
|NVIDIA NVENC	|nvEncodeAPI.h<br>NVIDIA Video Codec SDK	|---	|nvEncodeAPI64.dll (loaded at runtime)	|Only for NVIDIA GPUs; low-latency encoding; |
|AMD AMF	|AMF/core/AMF.h<br>AMF GitHub Repository|		amf.lib		|		amfrt64.dll			|	Only for AMD GPUs; supports both encoding and decoding; open-source; Might be harder to use in C as it is a C++ lib|

## Links
[Windows Media Foundation](https://en.wikipedia.org/wiki/Media_Foundation)
