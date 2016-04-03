// Fill out your copyright notice in the Description page of Project Settings.

#include "kinect2ArrayProject.h"
#include "k2Array.h"
#include <math.h>


//General Log
DEFINE_LOG_CATEGORY(logK2Array);


// Sets default values
Ak2Array::Ak2Array()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Texture = nullptr;
	UE_LOG(logK2Array, Warning, TEXT("Constructor >> Hello World!")); //temp msg
}


// Called when the game starts or when spawned
void Ak2Array::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void Ak2Array::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	// schedule a task on the render thread
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(
		UpdateTextureData,
		UTexture2D*, Texture, Texture,
		TArray<FColor>&, Array, TextureData,
		{
			UpdateTextureData(Texture, Array);
		}
	);
}


// save the texture in a var (must be a BlueprintCallable UFUNCTION)
void Ak2Array::SetTexture(UTexture2D* Texture)
{
	this->Texture = Texture;
}


// update the array reading the texture pixels on render thread
void Ak2Array::UpdateTextureData(UTexture2D* Texture, TArray<FColor>& TextureData)
{
	if (!Texture || !Texture->IsValidLowLevel()) return;

	// the amount of pixels in the texture
	const int32 NumPixels = Texture->GetSizeX() * Texture->GetSizeY();

	// lock the texture memory
	FColor* TextureBuffer = static_cast<FColor*>(Texture->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_ONLY));

	// fastest way (I think) of copying it to the array
	TextureData.SetNum(NumPixels); // make sure there's the right amount of memory reserved on this array
	FMemory::Memcpy(TextureData.GetData(), TextureBuffer, sizeof(FColor) * NumPixels);

	/* ^^^ It may be that the data from the Kinect camera (via the texture buffer) has been configured or optimized in some unexpected way.
	This function gets good results with basic texture settings (no mipMaps, and basic compression - for eg. try 'T_radialGradient1_D').
	Using any of the Kinect camera's as an input texture returns a constant gray value.
	This is the same gray value you'd get running the function without a Kinect even connected.
	It may be possible to copy the texture buffer data into a temporary texture, and then reconfigure the texture to make it readable.
	The settings might take some experimentation; but, 'no mipMap' and 'VectorDisplacementmap (RGBA8)' seem like a good place to start.
	What do you think? */

	// unlock the texture (very important!!)
	Texture->PlatformData->Mips[0].BulkData.Unlock();
}


// the pixels are in a linear sequence. This function just makes it easier to get them from x and y coordinates
FColor Ak2Array::SampleTextureAt(int32 X, int32 Y)
{
	if (!Texture || !Texture->IsValidLowLevel()) return FColor();

	const int32 SizeX = Texture->GetSizeX(),
		SizeY = Texture->GetSizeY();

	// check for valid coordinates
	if (X >= 0 && X < SizeX && Y >= 0 && Y < SizeY)
	{
		return TextureData[Y * SizeX + X];
	}

	// coordinates were invalid!
	return FColor();
}


/* moves objects in array according to pixel values - must be a fast as possible.
I guess speed will come, and in the meantime if it woks... ;) */
void Ak2Array::transform_Array(TArray<FColor> txtrData, TArray<UStaticMeshComponent*> objectArray, bool bVerbose, int32 pxPerUnit=8, int32 depthScalar=100)
{

	// Local Variables
	UStaticMeshComponent* object;	//object to be moved - as a pointer
	FVector location;	//object's current location - xyz
	int32 txtrSize;	//texture's x&y dimension - assumes square texture 
	int32 pxIndx = 0;	//pixel array index
	FVector pxVctr;	//pixel colour as vector - xyz
	//const int32 depthScalar = 125;	//scale vector y axis ### depthScalar will become a blueprint property

	if (txtrData.Num()) {	// warning, pixel array may still be empty shortly after initialization
		txtrSize = sqrt(txtrData.Num()); // texture's x&y dimension - assumes square texture 
		for (int32 i = 0; i < objectArray.Num(); i++)	//iterate over all objects in objectArray
		{
			object = objectArray[i];	//get object
			location = object->GetComponentLocation();	//get object location
			pxIndx = (i * pxPerUnit % txtrSize) + (i * pxPerUnit / txtrSize * txtrSize * pxPerUnit);	//calculate corresponding pixel index
			pxVctr = FVector(txtrData[pxIndx]);	//get pixel colour as a vector - xyz
			object->SetWorldLocation(FVector(location[0], pxVctr[1] * depthScalar, location[2]));	//use vector (*depthScalar) to update object's y-location (one RGBA channel) 
			if (bVerbose == 1) { UE_LOG(logK2Array, Warning, TEXT("#### >> transform_Array >> %f, %f, %f"), pxVctr[0], pxVctr[1], pxVctr[2]); } //log message
		}
	}
}


