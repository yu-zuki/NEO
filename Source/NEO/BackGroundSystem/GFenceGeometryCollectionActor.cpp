// Fill out your copyright notice in the Description page of Project Settings.

#include "GFenceGeometryCollectionActor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "UObject/ConstructorHelpers.h"
AGFenceGeometryCollectionActor::AGFenceGeometryCollectionActor()
{
    // �W�I���g���R���N�V�����A�Z�b�g��ݒ�
    static const ConstructorHelpers::FObjectFinder<UGeometryCollection> GeoCollection(TEXT("GeometryCollection'/Game/0102/Enemy/Gunman/MyGunFence_GeometryCollection.MyGunFence_GeometryCollection'"));
    if (GeoCollection.Succeeded())
    {
        GetGeometryCollectionComponent()->RestCollection = GeoCollection.Object;
    }
}