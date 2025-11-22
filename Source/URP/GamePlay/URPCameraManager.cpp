#include "URPCameraManager.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

UURPCameraManager::UURPCameraManager()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UURPCameraManager::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (!OwnerCharacter) return;

	SpringArm = OwnerCharacter->FindComponentByClass<USpringArmComponent>();
}


// Called every frame
void UURPCameraManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!SpringArm || !OwnerCharacter) return;

	HandleCameraCollision(DeltaTime);
	UpdateOcclusionTransparency();
}

void UURPCameraManager::InitializeCamera(USpringArmComponent* InBoom, UCameraComponent* InCamera)
{
    SpringArm = InBoom;
    FollowCamera = InCamera;

    // SpringArm 충돌 OFF → 커스텀 방식 사용
    if (SpringArm)
        SpringArm->bDoCollisionTest = false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// 1) 카메라 충돌 처리 → SpringArm 위치를 부드럽게 보정
//////////////////////////////////////////////////////////////////////////////////////////////////////

void UURPCameraManager::HandleCameraCollision(float DeltaTime)
{
    FVector Start = OwnerCharacter->GetActorLocation() + FVector(0, 0, 70.f);
    FVector End = SpringArm->GetComponentLocation();

    FHitResult Hit;
    bool bHit = GetWorld()->LineTraceSingleByChannel(
        Hit, Start, End, ECC_Visibility
    );

    float TargetLength = DefaultArmLength;

    if (bHit)
    {
        float Dist = (Hit.Location - Start).Size() - CollisionOffset;
        TargetLength = FMath::Clamp(Dist, MinArmLength, DefaultArmLength);
    }

    SpringArm->TargetArmLength =
        FMath::FInterpTo(SpringArm->TargetArmLength, TargetLength, DeltaTime, LengthInterpSpeed);
}

/////////////////////////////////////////////////////////////////////////
// 2) 카메라 가리는 오브젝트 투명화 처리
/////////////////////////////////////////////////////////////////////////

void UURPCameraManager::UpdateOcclusionTransparency()
{
    RestoreTransparentObjects();

    ACharacter* OwnerChar = Cast<ACharacter>(GetOwner());
    if (!OwnerChar) return;

    FVector CharPos = OwnerChar->GetActorLocation() + FVector(0, 0, 70.f);
    FVector CamPos = FollowCamera->GetComponentLocation();

    // 카메라가 너무 벽에 붙었을 때를 보정
    CamPos -= FollowCamera->GetForwardVector() * 15.f;

    TArray<FHitResult> Hits;
    GetWorld()->LineTraceMultiByChannel(
        Hits, CamPos, CharPos, ECC_Visibility
    );

    for (auto& Hit : Hits)
    {
        UPrimitiveComponent* Comp = Hit.GetComponent();

        if (!Comp) continue;
        if (Comp->GetOwner() == OwnerChar) continue; // 자기 자신은 제외

        Comp->SetRenderCustomDepth(true);
        TransparentObjects.Add(Comp);
    }
}

/////////////////////////////////////////////////////////////////////////
// 3) 이전 프레임의 투명 처리 복구
/////////////////////////////////////////////////////////////////////////

void UURPCameraManager::RestoreTransparentObjects()
{
    for (auto* Comp : TransparentObjects)
    {
        if (Comp)
            Comp->SetRenderCustomDepth(false);
    }
    TransparentObjects.Empty();
}