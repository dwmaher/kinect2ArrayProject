name:
kinect2Array (k2Array)

authors:
rVillani
dMaher

requirements:
UE4 4.10.0 - epic
kinect hardware - ms
kinect SDK - ms
kinect4Unreal (K4U) - opaque.media

path:
%\Documents\Unreal Projects\

description:
The intention of k2Array is to transmit a point-cloud array from Kinect into UE4.
As far as I know, there is no existing method to do this.
Such an array could be used to create sculptural visual effects in real-time.

includes:
k2Array.cpp - C++ class for reading kinect data directly from the texture buffer(?).
BP_k2Array_dynamicTest_actor.uasset - blueprint class for testing k2Array.cpp with kinect dynamic texture stream >> not working
BP_k2Array_staticTest_actor.uasset - blueprint class for testing k2Array.cpp with static UE4 textures >> working!

problem:
So far, k2Array only half works... :)
It works with simple textures - no mipMap's and basic compression (like 'VectorDisplacementmap (RGBA8)')
Possibly the stream pulled from kinect could be reconfigured,  using texture class methods, before it is assigned to the array.
The other challenge with k2Array is the arrays can be quite large (almost 220,000 items for the depthMap).
Iterating over the entire array each frame, to update object positions for instance, can be time consuming.
But there are likely many optimization possible.
Any thoughts?

dMaher



