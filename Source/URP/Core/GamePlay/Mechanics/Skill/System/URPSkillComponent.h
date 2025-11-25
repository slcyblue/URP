#pragma once
#include "CoreMinimal.h"
#include "Types/URPSkillTypes.h"
#include "Components/ActorComponent.h"
#include "URPSkillComponent.generated.h"

class UURPSkillBase;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class URP_API UURPSkillComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UURPSkillComponent();

    /** 플레이어 직업에 맞는 스킬만 로드 */
    void InitializeSkills(EURPClassType ClassType);

    /** SkillId 기반으로 스킬 실행 */
    void ExecuteSkill(int32 SkillId, float AdjustTime);

    /** 스킬 등록 */
    void RegisterSkill(int32 SkillId, UURPSkillBase* Skill);

    /** 스킬 찾기 */
    UURPSkillBase* GetSkill(int32 SkillId) const;

    float GetRemainingCooldown(int32 SkillId) const;

    float GetCooldown(int32 SkillId) const;

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY()
    TMap<int32, UURPSkillBase*> SkillMap;
};
