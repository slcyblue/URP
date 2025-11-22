// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "URPCameraManager.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class URP_API UURPCameraManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	UURPCameraManager();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    void InitializeCamera(class USpringArmComponent* InBoom, class UCameraComponent* InCamera);

    UPROPERTY(EditAnywhere, Category = "Camera")
    float DefaultArmLength = 1600.f;

    UPROPERTY(EditAnywhere, Category = "Camera")
    float MinArmLength = 400.f;

    UPROPERTY(EditAnywhere, Category = "Camera")
    float CollisionOffset = 15.f;

    UPROPERTY(EditAnywhere, Category = "Camera")
    float LengthInterpSpeed = 7.f;

    UPROPERTY()
    UCameraComponent* FollowCamera;
private:
    
    class USpringArmComponent* SpringArm;
    class ACharacter* OwnerCharacter;

    // 투명 처리된 오브젝트 목록
    TArray<UPrimitiveComponent*> TransparentObjects;

    void HandleCameraCollision(float DeltaTime);
    void UpdateOcclusionTransparency();
    void RestoreTransparentObjects();
};
