// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "TPS_GAS/TPS_GASGameMode.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeTPS_GASGameMode() {}
// Cross Module References
	ENGINE_API UClass* Z_Construct_UClass_AGameModeBase();
	TPS_GAS_API UClass* Z_Construct_UClass_ATPS_GASGameMode();
	TPS_GAS_API UClass* Z_Construct_UClass_ATPS_GASGameMode_NoRegister();
	UPackage* Z_Construct_UPackage__Script_TPS_GAS();
// End Cross Module References
	void ATPS_GASGameMode::StaticRegisterNativesATPS_GASGameMode()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(ATPS_GASGameMode);
	UClass* Z_Construct_UClass_ATPS_GASGameMode_NoRegister()
	{
		return ATPS_GASGameMode::StaticClass();
	}
	struct Z_Construct_UClass_ATPS_GASGameMode_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_ATPS_GASGameMode_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AGameModeBase,
		(UObject* (*)())Z_Construct_UPackage__Script_TPS_GAS,
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_ATPS_GASGameMode_Statics::DependentSingletons) < 16);
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ATPS_GASGameMode_Statics::Class_MetaDataParams[] = {
		{ "HideCategories", "Info Rendering MovementReplication Replication Actor Input Movement Collision Rendering HLOD WorldPartition DataLayers Transformation" },
		{ "IncludePath", "TPS_GASGameMode.h" },
		{ "ModuleRelativePath", "TPS_GASGameMode.h" },
		{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_ATPS_GASGameMode_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ATPS_GASGameMode>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_ATPS_GASGameMode_Statics::ClassParams = {
		&ATPS_GASGameMode::StaticClass,
		"Game",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x008802ACu,
		METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_ATPS_GASGameMode_Statics::Class_MetaDataParams), Z_Construct_UClass_ATPS_GASGameMode_Statics::Class_MetaDataParams)
	};
	UClass* Z_Construct_UClass_ATPS_GASGameMode()
	{
		if (!Z_Registration_Info_UClass_ATPS_GASGameMode.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_ATPS_GASGameMode.OuterSingleton, Z_Construct_UClass_ATPS_GASGameMode_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_ATPS_GASGameMode.OuterSingleton;
	}
	template<> TPS_GAS_API UClass* StaticClass<ATPS_GASGameMode>()
	{
		return ATPS_GASGameMode::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(ATPS_GASGameMode);
	ATPS_GASGameMode::~ATPS_GASGameMode() {}
	struct Z_CompiledInDeferFile_FID_TPS_GAS_Source_TPS_GAS_TPS_GASGameMode_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_TPS_GAS_Source_TPS_GAS_TPS_GASGameMode_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_ATPS_GASGameMode, ATPS_GASGameMode::StaticClass, TEXT("ATPS_GASGameMode"), &Z_Registration_Info_UClass_ATPS_GASGameMode, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(ATPS_GASGameMode), 3349592444U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_TPS_GAS_Source_TPS_GAS_TPS_GASGameMode_h_1564978441(TEXT("/Script/TPS_GAS"),
		Z_CompiledInDeferFile_FID_TPS_GAS_Source_TPS_GAS_TPS_GASGameMode_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_TPS_GAS_Source_TPS_GAS_TPS_GASGameMode_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
