// Fill out your copyright notice in the Description page of Project Settings.


#include "GeoBigFEnce.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "UObject/ConstructorHelpers.h"

AGeoBigFEnce::AGeoBigFEnce()
{
    // �W�I���g���R���N�V�����A�Z�b�g��ݒ�
    static const ConstructorHelpers::FObjectFinder<UGeometryCollection> GeoCollection(TEXT("GeometryCollection'/Game/0102/Enemy/MyBigFence_GeometryCollection.MyBigFence_GeometryCollection_GeometryCollection'"));
    if (GeoCollection.Succeeded())                                                         /*"GeometryCollection'/Game/0102/Enemy/Poal_GeometryCollection_GeometryCollection.Poal_GeometryCollection_GeometryCollection'"));*/
    {
        GetGeometryCollectionComponent()->RestCollection = GeoCollection.Object;
    }

}