#include "../AliveLibCommon/stdafx_common.h"
#include "TypesCollection.hpp"
#include "AOTlvs.hpp"
#include "AETlvs.hpp"
#include "../AliveLibAO/SwitchStates.hpp"
#include "../AliveLibAE/PathData.hpp"
#include "../AliveLibAE/SwitchStates.hpp"
#include "magic_enum/include/magic_enum.hpp"

#define ADD_BASIC_TYPE(name, type) AddBasicType<type>(name, std::numeric_limits<type>::min(), std::numeric_limits<type>::max());

TypesCollection::TypesCollection(Game gameType) 
    : mGameType(gameType)
{
    ADD_BASIC_TYPE("Byte", BYTE);
    ADD_BASIC_TYPE("UInt16", unsigned short);
    ADD_BASIC_TYPE("Uint32", unsigned int);
    ADD_BASIC_TYPE("SInt16", signed short);
    ADD_BASIC_TYPE("SInt32", signed int);


    if (mGameType == Game::AO)
    {
        AddAOTypes();
    }
    else
    {
        AddAETypes();
    }
}

#define REGISTER_TYPE_AO(TlvWrapperType) mTlvFactoryAO.DoRegisterType<TlvWrapperType>(*this)
#define REGISTER_TYPE_AE(TlvWrapperType) mTlvFactoryAE.DoRegisterType<TlvWrapperType>(*this)

void TypesCollection::AddAOTypes()
{
    REGISTER_TYPE_AO(AOTlvs::Path_Hoist);
    REGISTER_TYPE_AO(AOTlvs::Path_ContinuePoint);
    REGISTER_TYPE_AO(AOTlvs::Path_Door);
    REGISTER_TYPE_AO(AOTlvs::Path_Change);
    REGISTER_TYPE_AO(AOTlvs::Path_Switch);
    REGISTER_TYPE_AO(AOTlvs::Path_LightEffect);
    REGISTER_TYPE_AO(AOTlvs::Path_ElectricWall);
    REGISTER_TYPE_AO(AOTlvs::Path_ContinueZone);
    REGISTER_TYPE_AO(AOTlvs::Path_StartController);
    REGISTER_TYPE_AO(AOTlvs::Path_Edge);
    REGISTER_TYPE_AO(AOTlvs::Path_WellLocal);
    REGISTER_TYPE_AO(AOTlvs::Path_WellExpress);
    REGISTER_TYPE_AO(AOTlvs::Path_InvisibleZone);
    REGISTER_TYPE_AO(AOTlvs::Path_EnemyStopper);
    REGISTER_TYPE_AO(AOTlvs::Path_Slig);
    REGISTER_TYPE_AO(AOTlvs::Path_DeathDrop);
    REGISTER_TYPE_AO(AOTlvs::Path_SligLeftBound);
    REGISTER_TYPE_AO(AOTlvs::Path_SligRightBound);
    REGISTER_TYPE_AO(AOTlvs::Path_SligPersist);
    REGISTER_TYPE_AO(AOTlvs::Path_SecurityOrb);
    REGISTER_TYPE_AO(AOTlvs::Path_FallingItem);
    REGISTER_TYPE_AO(AOTlvs::Path_Mine);
    REGISTER_TYPE_AO(AOTlvs::Path_Dove);
    REGISTER_TYPE_AO(AOTlvs::Path_UXB);
    REGISTER_TYPE_AO(AOTlvs::Path_HintFly);
    REGISTER_TYPE_AO(AOTlvs::Path_Bat);
    REGISTER_TYPE_AO(AOTlvs::Path_ShadowZone);
    REGISTER_TYPE_AO(AOTlvs::Path_BellHammer);
    REGISTER_TYPE_AO(AOTlvs::Path_IdSplitter);
    REGISTER_TYPE_AO(AOTlvs::Path_PullRingRope);
    REGISTER_TYPE_AO(AOTlvs::Path_MusicTrigger);
    REGISTER_TYPE_AO(AOTlvs::Path_ElumPathTrans);
    REGISTER_TYPE_AO(AOTlvs::Path_ElumStart);
    REGISTER_TYPE_AO(AOTlvs::Path_ElumWall);
    REGISTER_TYPE_AO(AOTlvs::Path_LiftPoint);
    REGISTER_TYPE_AO(AOTlvs::Path_MovingBomb);
    REGISTER_TYPE_AO(AOTlvs::Path_MovingBombStopper);
    REGISTER_TYPE_AO(AOTlvs::Path_RingMudokon);
    REGISTER_TYPE_AO(AOTlvs::Path_RingCancel);
    REGISTER_TYPE_AO(AOTlvs::Path_MeatSaw);
    REGISTER_TYPE_AO(AOTlvs::Path_LCDScreen);
    REGISTER_TYPE_AO(AOTlvs::Path_InvisibleSwitch);
    REGISTER_TYPE_AO(AOTlvs::Path_TrapDoor);
    REGISTER_TYPE_AO(AOTlvs::Path_BirdPortal);
    REGISTER_TYPE_AO(AOTlvs::Path_BoomMachine);
    REGISTER_TYPE_AO(AOTlvs::Path_Mudokon);
    REGISTER_TYPE_AO(AOTlvs::Path_BirdPortalExit);
    REGISTER_TYPE_AO(AOTlvs::Path_Slog);
    REGISTER_TYPE_AO(AOTlvs::Path_ChimeLock);
    REGISTER_TYPE_AO(AOTlvs::Path_FlintLockFire);
    REGISTER_TYPE_AO(AOTlvs::Path_LiftMover);
    REGISTER_TYPE_AO(AOTlvs::Path_Scrab);
    REGISTER_TYPE_AO(AOTlvs::Path_SlogSpawner);
    REGISTER_TYPE_AO(AOTlvs::Path_RockSack);
    REGISTER_TYPE_AO(AOTlvs::Path_SlogHut);
    REGISTER_TYPE_AO(AOTlvs::Path_SecurityClaw);
    REGISTER_TYPE_AO(AOTlvs::Path_SecurityDoor);
    REGISTER_TYPE_AO(AOTlvs::Path_TimedMine);
    REGISTER_TYPE_AO(AOTlvs::Path_SligSpawner);
    REGISTER_TYPE_AO(AOTlvs::Path_MotionDetector);
    REGISTER_TYPE_AO(AOTlvs::Path_BackgroundAnimation);
    REGISTER_TYPE_AO(AOTlvs::Path_LCDStatusBoard);
    REGISTER_TYPE_AO(AOTlvs::Path_Preloader);
    REGISTER_TYPE_AO(AOTlvs::Path_Pulley);
    REGISTER_TYPE_AO(AOTlvs::Path_SoftLanding);
    REGISTER_TYPE_AO(AOTlvs::Path_MudokonPathTrans);
    REGISTER_TYPE_AO(AOTlvs::Path_AbeStart);
    REGISTER_TYPE_AO(AOTlvs::Path_ZSligCover);
    REGISTER_TYPE_AO(AOTlvs::Path_ScrabLeftBound);
    REGISTER_TYPE_AO(AOTlvs::Path_ScrabRightBound);
    REGISTER_TYPE_AO(AOTlvs::Path_ScrabNoFall);
    REGISTER_TYPE_AO(AOTlvs::Path_LiftMudokon);
    REGISTER_TYPE_AO(AOTlvs::Path_HoneySack);
    REGISTER_TYPE_AO(AOTlvs::Path_SlingMudokon);
    REGISTER_TYPE_AO(AOTlvs::Path_BeeSwarmHole);
    REGISTER_TYPE_AO(AOTlvs::Path_MeatSack);
    REGISTER_TYPE_AO(AOTlvs::Path_RollingBall);
    REGISTER_TYPE_AO(AOTlvs::Path_RollingBallStopper);
    REGISTER_TYPE_AO(AOTlvs::Path_Bees);
    REGISTER_TYPE_AO(AOTlvs::Path_ZBall);
    REGISTER_TYPE_AO(AOTlvs::Path_FootSwitch);
    REGISTER_TYPE_AO(AOTlvs::Path_Paramite);
    REGISTER_TYPE_AO(AOTlvs::Path_Honey);
    REGISTER_TYPE_AO(AOTlvs::Path_HoneyDripTarget);
    REGISTER_TYPE_AO(AOTlvs::Path_HandStone);
    REGISTER_TYPE_AO(AOTlvs::Path_BellsongStone);
    REGISTER_TYPE_AO(AOTlvs::Path_MovieStone);

    AddEnum<AO::SwitchOp>("Enum_SwitchOp",
        {
             { AO::SwitchOp::eSetTrue_0, "SetTrue" },
             { AO::SwitchOp::eSetFalse_1, "SetFalse" },
             { AO::SwitchOp::eToggle_2, "Toggle" },
             { AO::SwitchOp::eIncrement_3, "Increment" },
             { AO::SwitchOp::eDecrement_4, "Decrement" },
        });

    AddEnum<AO::LevelIds>("Enum_LevelIds",
        {
            {AO::LevelIds::eNone, "None"},
            {AO::LevelIds::eMenu_0, "Menu"},
            {AO::LevelIds::eRuptureFarms_1, "RuptureFarms"},
            {AO::LevelIds::eLines_2, "Lines"},
            {AO::LevelIds::eForest_3, "Forest"},
            {AO::LevelIds::eForestTemple_4, "ForestTemple"},
            {AO::LevelIds::eStockYards_5, "StockYards"},
            {AO::LevelIds::eStockYardsReturn_6, "StockYardsReturn"},
            {AO::LevelIds::eRemoved_7, "Removed7"},
            {AO::LevelIds::eDesert_8, "Desert"},
            {AO::LevelIds::eDesertTemple_9, "DesertTemple"},
            {AO::LevelIds::eCredits_10, "Credits"},
            {AO::LevelIds::eRemoved_11, "Removed11"},
            {AO::LevelIds::eBoardRoom_12, "BoardRoom"},
            {AO::LevelIds::eRuptureFarmsReturn_13, "RuptureFarmsReturn"},
            {AO::LevelIds::eForestChase, "ForestChase"},
            {AO::LevelIds::eDesertEscape, "DesertEscape"},
        });

    AddEnum<Choice_short>("Enum_Choice_short",
        {
             { Choice_short::eNo_0, "No" },
             { Choice_short::eYes_1, "Yes" },
        });

    AddEnum<Scale_short>("Enum_Scale_short",
        {
             { Scale_short::eFull_0, "Full" },
             { Scale_short::eHalf_1, "Half" },
        });

    AddEnum<XDirection_short>("Enum_XDirection_short",
        {
             { XDirection_short::eLeft_0, "Left" },
             { XDirection_short::eRight_1, "Right" },
        });
}

void TypesCollection::AddAETypes()
{
    REGISTER_TYPE_AE(AETlvs::Path_Hoist);
    REGISTER_TYPE_AE(AETlvs::Path_Switch);
    REGISTER_TYPE_AE(AETlvs::Path_Door);
    REGISTER_TYPE_AE(AETlvs::Path_LCDStatusBoard);
    REGISTER_TYPE_AE(AETlvs::Path_BirdPortal);
    REGISTER_TYPE_AE(AETlvs::Path_Mudokon);
    REGISTER_TYPE_AE(AETlvs::Path_ElectricWall);
    REGISTER_TYPE_AE(AETlvs::Path_BoomMachine);
    REGISTER_TYPE_AE(AETlvs::Path_Slig);
    REGISTER_TYPE_AE(AETlvs::Path_Fleech);
    REGISTER_TYPE_AE(AETlvs::Path_EnemyStopper);
    REGISTER_TYPE_AE(AETlvs::Path_Teleporter);
    REGISTER_TYPE_AE(AETlvs::Path_UXB);
    REGISTER_TYPE_AE(AETlvs::Path_LCDScreen);
    REGISTER_TYPE_AE(AETlvs::Path_Edge);
    REGISTER_TYPE_AE(AETlvs::Path_StatusLight);
    REGISTER_TYPE_AE(AETlvs::Path_ShadowZone);
    REGISTER_TYPE_AE(AETlvs::Path_WorkWheel);
    REGISTER_TYPE_AE(AETlvs::Path_MusicTrigger);
    REGISTER_TYPE_AE(AETlvs::Path_AbeStart);
    REGISTER_TYPE_AE(AETlvs::Path_SoftLanding);
    REGISTER_TYPE_AE(AETlvs::Path_WellExpress);
    REGISTER_TYPE_AE(AETlvs::Path_SlamDoor);
    REGISTER_TYPE_AE(AETlvs::Path_HandStone);
    REGISTER_TYPE_AE(AETlvs::Path_LaughingGas);
    REGISTER_TYPE_AE(AETlvs::Path_InvisibleSwitch);
    REGISTER_TYPE_AE(AETlvs::Path_Water);
    REGISTER_TYPE_AE(AETlvs::Path_GasEmitter);
    REGISTER_TYPE_AE(AETlvs::Path_BackgroundAnimation);
    REGISTER_TYPE_AE(AETlvs::Path_LiftPoint);
    REGISTER_TYPE_AE(AETlvs::Path_PullRingRope);
    REGISTER_TYPE_AE(AETlvs::Path_MultiSwitchController);
    REGISTER_TYPE_AE(AETlvs::Path_SecurityOrb);
    REGISTER_TYPE_AE(AETlvs::Path_InvisibleZone);
    REGISTER_TYPE_AE(AETlvs::Path_ContinuePoint);
    REGISTER_TYPE_AE(AETlvs::Path_WheelSyncer);
    REGISTER_TYPE_AE(AETlvs::Path_LevelLoader);
    REGISTER_TYPE_AE(AETlvs::Path_Pulley);
    REGISTER_TYPE_AE(AETlvs::Path_FlyingSlig);
    REGISTER_TYPE_AE(AETlvs::Path_FlyingSligSpawner);
    REGISTER_TYPE_AE(AETlvs::Path_DeathDrop);
    REGISTER_TYPE_AE(AETlvs::Path_SligSpawner);
    REGISTER_TYPE_AE(AETlvs::Path_SligLeftBound);
    REGISTER_TYPE_AE(AETlvs::Path_SligRightBound);
    REGISTER_TYPE_AE(AETlvs::Path_SligPersist);
    REGISTER_TYPE_AE(AETlvs::Path_ZSligCover);
    REGISTER_TYPE_AE(AETlvs::Path_WellLocal);
    REGISTER_TYPE_AE(AETlvs::Path_FartMachine);
    REGISTER_TYPE_AE(AETlvs::Path_Grinder);
    REGISTER_TYPE_AE(AETlvs::Path_Mine);
    REGISTER_TYPE_AE(AETlvs::Path_Slog);
    REGISTER_TYPE_AE(AETlvs::Path_ResetSwitchRange);
    REGISTER_TYPE_AE(AETlvs::Path_TrapDoor);
    REGISTER_TYPE_AE(AETlvs::Path_PathTransition);
    REGISTER_TYPE_AE(AETlvs::Path_LiftMover);
    REGISTER_TYPE_AE(AETlvs::Path_RockSack);
    REGISTER_TYPE_AE(AETlvs::Path_TimerTrigger);
    REGISTER_TYPE_AE(AETlvs::Path_MotionDetector);
    REGISTER_TYPE_AE(AETlvs::Path_MineCar);
    REGISTER_TYPE_AE(AETlvs::Path_ExplosionSet);
    REGISTER_TYPE_AE(AETlvs::Path_ColourfulMeter);
    REGISTER_TYPE_AE(AETlvs::Path_Alarm);
    REGISTER_TYPE_AE(AETlvs::Path_DemoSpawnPoint);
    REGISTER_TYPE_AE(AETlvs::Path_SlapLock);
    REGISTER_TYPE_AE(AETlvs::Path_Slurg);
    REGISTER_TYPE_AE(AETlvs::Path_DoorBlocker);
    REGISTER_TYPE_AE(AETlvs::Path_Dove);
    REGISTER_TYPE_AE(AETlvs::Path_BirdPortalExit);
    REGISTER_TYPE_AE(AETlvs::Path_DoorFlame);
    REGISTER_TYPE_AE(AETlvs::Path_TrainDoor);
    REGISTER_TYPE_AE(AETlvs::Path_Greeter);
    REGISTER_TYPE_AE(AETlvs::Path_ScrabLeftBound);
    REGISTER_TYPE_AE(AETlvs::Path_ScrabRightBound);
    REGISTER_TYPE_AE(AETlvs::Path_CreditsController);
    REGISTER_TYPE_AE(AETlvs::Path_MovieHandstone);
    REGISTER_TYPE_AE(AETlvs::Path_MovingBomb);
    REGISTER_TYPE_AE(AETlvs::Path_SecurityDoor);
    REGISTER_TYPE_AE(AETlvs::Path_NakedSlig);
    REGISTER_TYPE_AE(AETlvs::Path_SligGetPants);
    REGISTER_TYPE_AE(AETlvs::Path_SligGetWings);
    REGISTER_TYPE_AE(AETlvs::Path_NakedSligButton);
    REGISTER_TYPE_AE(AETlvs::Path_Glukkon);
    REGISTER_TYPE_AE(AETlvs::Path_GlukkonSwitch);
    REGISTER_TYPE_AE(AETlvs::Path_GasCountDown);
    REGISTER_TYPE_AE(AETlvs::Path_FallingItem);
    REGISTER_TYPE_AE(AETlvs::Path_BoneBag);
    REGISTER_TYPE_AE(AETlvs::Path_SecurityClaw);
    REGISTER_TYPE_AE(AETlvs::Path_FootSwitch);
    REGISTER_TYPE_AE(AETlvs::Path_SlogHut);
    REGISTER_TYPE_AE(AETlvs::Path_SlogSpawner);
    REGISTER_TYPE_AE(AETlvs::Path_MainMenuController);
    REGISTER_TYPE_AE(AETlvs::Path_Null_63);
    REGISTER_TYPE_AE(AETlvs::Path_Scrab);
    REGISTER_TYPE_AE(AETlvs::Path_ScrabSpawner);
    REGISTER_TYPE_AE(AETlvs::Path_SlurgSpawner);
    REGISTER_TYPE_AE(AETlvs::Path_Paramite);
    REGISTER_TYPE_AE(AETlvs::Path_ParamiteWebLine);
    REGISTER_TYPE_AE(AETlvs::Path_MeatSack);
    REGISTER_TYPE_AE(AETlvs::Path_TorturedMudokon);

    AddEnum<CameraSwapEffects>("Enum_CameraSwapEffects",
        {
             { CameraSwapEffects::eEffect0_InstantChange, "Instant Change" },
             { CameraSwapEffects::eEffect1_LeftToRight, "Left To Right" },
             { CameraSwapEffects::eEffect2_RightToLeft, "Right To Left" },
             { CameraSwapEffects::eEffect3_TopToBottom, "Top To Bottom" },
             { CameraSwapEffects::eEffect4_BottomToTop, "Bottom To Top" },
             { CameraSwapEffects::eEffect5_1_FMV, "1 FMV" },
             { CameraSwapEffects::eEffect6_VerticalSplit, "Left To Vertical Split" },
             { CameraSwapEffects::eEffect7_HorizontalSplit, "Horizontal Split" },
             { CameraSwapEffects::eEffect8_BoxOut, "Box Out" },
             { CameraSwapEffects::eEffect9_2_FMV, "2 FMV" },
             { CameraSwapEffects::eEffect10_3_FMV, "3 FMV" },
             { CameraSwapEffects::eEffect11_Unknown, "Unknown" },
        });

    AddEnum<Choice_short>("Enum_Choice_short",
        {
             { Choice_short::eNo_0, "No" },
             { Choice_short::eYes_1, "Yes" },
        });

    AddEnum<XDirection_short>("Enum_XDirection_short",
        {
             { XDirection_short::eLeft_0, "Left" },
             { XDirection_short::eRight_1, "Right" },
        });

    AddEnum<Scale_short>("Enum_Scale_short",
        {
             { Scale_short::eFull_0, "Full" },
             { Scale_short::eHalf_1, "Half" },
        });

    AddEnum<Scale_int>("Enum_Scale_int",
        {
             { Scale_int::eFull_0, "Full" },
             { Scale_int::eHalf_1, "Half" },
        });

    AddEnum<SwitchOp>("Enum_SwitchOp",
        {
             { SwitchOp::eSetTrue_0, "SetTrue" },
             { SwitchOp::eSetFalse_1, "SetFalse" },
             { SwitchOp::eToggle_2, "Toggle" },
             { SwitchOp::eIncrement_3, "Increment" },
             { SwitchOp::eDecrement_4, "Decrement" },
        });

    AddEnum<LevelIds>("Enum_LevelIds",
        {
            {LevelIds::eNone, "None"},
            {LevelIds::eMenu_0, "Menu"},
            {LevelIds::eMines_1, "Mines"},
            {LevelIds::eNecrum_2, "Necrum"},
            {LevelIds::eMudomoVault_3, "MudomoVault"},
            {LevelIds::eMudancheeVault_4, "MudancheeVault"},
            {LevelIds::eFeeCoDepot_5, "FeeCoDepot"},
            {LevelIds::eBarracks_6, "Barracks"},
            {LevelIds::eMudancheeVault_Ender_7, "MudancheeVault_Ender"},
            {LevelIds::eBonewerkz_8, "Bonewerkz"},
            {LevelIds::eBrewery_9, "Brewery"},
            {LevelIds::eBrewery_Ender_10, "Brewery_Ender"},
            {LevelIds::eMudomoVault_Ender_11, "MudomoVault_Ender"},
            {LevelIds::eFeeCoDepot_Ender_12, "FeeCoDepot_Ender"},
            {LevelIds::eBarracks_Ender_13, "Barracks_Ender"},
            {LevelIds::eBonewerkz_Ender_14, "Bonewerkz_Ender"},
            {LevelIds::eNotUsed_15, "NotUsed"},
            {LevelIds::eCredits_16, "Credits"},
        });
}

void TypesCollection::AddTlvsToJsonArray(jsonxx::Array& array)
{
    if (mGameType == Game::AO)
    {
        mTlvFactoryAO.AddTlvsToJsonArray(*this, array);
    }
    else
    {
        mTlvFactoryAE.AddTlvsToJsonArray(*this, array);
    }
}


std::unique_ptr<TlvObjectBase> TypesCollection::MakeTlvAE(TlvTypes tlvType, Path_TLV* pTlv, int instanceCount)
{
    return mTlvFactoryAE.MakeTlvByEnum(*this, tlvType, pTlv, instanceCount);
}

std::unique_ptr<TlvObjectBase> TypesCollection::MakeTlvAO(AO::TlvTypes tlvType, AO::Path_TLV* pTlv, int instanceCount)
{
    return mTlvFactoryAO.MakeTlvByEnum(*this, tlvType, pTlv, instanceCount);
}

std::unique_ptr<TlvObjectBase> TypesCollection::MakeTlvAE(const std::string& tlvTypeName, Path_TLV* pTlv)
{
    return mTlvFactoryAE.MakeTlvByName(*this, tlvTypeName, pTlv);
}

std::unique_ptr<TlvObjectBase> TypesCollection::MakeTlvAO(const std::string& tlvTypeName, AO::Path_TLV* pTlv)
{
    return mTlvFactoryAO.MakeTlvByName(*this, tlvTypeName, pTlv);
}
