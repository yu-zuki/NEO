// �X�V���F2023/7/7		�X�V�ҁF��		CharaInfoAPI�̍쐬
// 
// ��
// �g�p���@�FCharaInfoAPI���p�����āAGetCharacterInfo���I�[�o�[���C�h����
// GetCharacterInfo�̕Ԃ�l�́A�L�����N�^�[���AHP�AMP�̏��ŕԂ�

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CharacterInfo_UI_API.generated.h"

/**
 * 
 */
UCLASS()
class NEO_API UCharacterInfo_UI_API : public UObject
{
	GENERATED_BODY()

public:
	virtual	 TTuple<FName, int, int> GetCharacterInfo();	//First is character name, second is character Hp
	
	UFUNCTION(BlueprintCallable)
	void SetUI_Enemy();
};
