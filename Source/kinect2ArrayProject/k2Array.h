// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "k2Array.generated.h"

//General Log
DECLARE_LOG_CATEGORY_EXTERN(logK2Array, Log, All);

UCLASS()
class KINECT2ARRAYPROJECT_API Ak2Array : public AActor
{

	GENERATED_BODY()
	
public:	

	// Sets default values for this actor's properties
	Ak2Array();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;



	// -------- Variables --------

	/** local texture variable */
	UTexture2D* Texture;

	/** array to store the texture data (accessible from Blueprint) */
	UPROPERTY(BlueprintReadWrite, Category = "k2Array")
	TArray<FColor> TextureData;



	// -------- Functions --------

	/** save the texture in a var (must be a BlueprintCallable UFUNCTION) */
	UFUNCTION(BlueprintCallable, Category = "k2Array")
	void SetTexture(UTexture2D* Texture);

	/** update the array reading the texture pixels on render thread */
	static void UpdateTextureData(UTexture2D* Texture, TArray<FColor>& TextureData);

	/** the pixels are in a linear sequence. This function just makes it easier to get them from x and y coordinates */
	FColor SampleTextureAt(int32 X, int32 Y);

	/** transforms an array of mesh objects according to an array of colour values */
	UFUNCTION(BlueprintCallable, Category = "k2Array")
	static void transform_Array(TArray<FColor> txtrData, TArray<UStaticMeshComponent*> objectArray, bool bVerbose, int32 pxPerUnit, int32 depthScalar);
	
};
