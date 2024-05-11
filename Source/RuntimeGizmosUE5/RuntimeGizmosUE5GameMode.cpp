// Copyright Epic Games, Inc. All Rights Reserved.

#include "RuntimeGizmosUE5GameMode.h"
#include "RuntimeGizmosUE5Character.h"
#include "UObject/ConstructorHelpers.h"
#include "DrawDebugHelpers.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/Material.h"
#include <assimp/Importer.hpp>
#include <Runtime/Engine/Classes/Engine/Texture2D.h>
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "types.h"
#include "HAL/FileManager.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"
#include "ImageUtils.h"
#include "UnrealEd/Public/UnrealEdMisc.h"
#include "IImageWrapperModule.h"
#include "IImageWrapper.h"
#include <DDSLoader.h>
#include "Engine/Texture2D.h"
#include "Engine/Texture2DDynamic.h"
#include "AssetRegistry/AssetRegistryModule.h"


ARuntimeGizmosUE5GameMode::ARuntimeGizmosUE5GameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
