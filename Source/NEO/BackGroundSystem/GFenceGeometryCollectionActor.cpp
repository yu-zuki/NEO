// Fill out your copyright notice in the Description page of Project Settings.

#include "GFenceGeometryCollectionActor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "UObject/ConstructorHelpers.h"
AGFenceGeometryCollectionActor::AGFenceGeometryCollectionActor()
{
    // �W�I���g���R���N�V�����A�Z�b�g��ݒ�
    static const ConstructorHelpers::FObjectFinder<UGeometryCollection> GeoCollection(TEXT("GeometryCollection'/Game/ALL/Content/0102/Enemy/GunMan/MyGunFence_GeometryCollection.MyGunFence_GeometryCollection'"));
    if (GeoCollection.Succeeded())
    {
        GetGeometryCollectionComponent()->RestCollection = GeoCollection.Object;
    }
}