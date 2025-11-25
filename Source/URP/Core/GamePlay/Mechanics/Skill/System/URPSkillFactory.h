#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Types/URPSkillTypes.h"
#include "URPSkillFactory.generated.h"

class UURPSkillBase;
class UURPProjectileBase;
class UURPSingleHitBase;
class UURPAOEBase;
class UURPDashBase;
class UURPDOTBase;
class UURPBuffBase;
class UURPDebuffBase;

/**
 * FURPSkillRow -> UURPSkillBase 자식 생성기
 * - SkillType에 따라 적절한 스킬 클래스를 생성하고
 * - Row에 있는 값을 스킬 인스턴스에 매핑해준다.
 */
UCLASS()
class URP_API UURPSkillFactory : public UObject
{
    GENERATED_BODY()

public:
    /**
     * 메인 엔트리: Row.SkillType 기반으로 적절한 스킬 생성
     * @param Row   : GameDataSubsystem에서 가져온 스킬 데이터
     * @param Outer : 스킬의 Outer (보통 SkillComponent 같은 Owner 컴포넌트)
     */
    UFUNCTION()
    UURPSkillBase* CreateSkill(const FURPSkillRow& Row, UObject* Outer);

private:
    UURPSkillBase* CreateProjectileSkill(const FURPSkillRow& Row, UObject* Outer);
    UURPSkillBase* CreateSingleHitSkill(const FURPSkillRow& Row, UObject* Outer);
    UURPSkillBase* CreateAOESkill(const FURPSkillRow& Row, UObject* Outer);
    UURPSkillBase* CreateDashSkill(const FURPSkillRow& Row, UObject* Outer);
    UURPSkillBase* CreateDOTSkill(const FURPSkillRow& Row, UObject* Outer);
    UURPSkillBase* CreateBuffSkill(const FURPSkillRow& Row, UObject* Outer);
    UURPSkillBase* CreateDebuffSkill(const FURPSkillRow& Row, UObject* Outer);

    /** 공통 필드 셋업 (SkillId, Cooldown, etc.) */
    void ApplyCommonData(UURPSkillBase* Skill, const FURPSkillRow& Row);
};
