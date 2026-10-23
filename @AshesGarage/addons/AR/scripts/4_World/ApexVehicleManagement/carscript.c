#ifdef SERVER
modded class BoatScript 
{
	override void OnStoreSave(ParamsWriteContext ctx) 
	{
		ApexVehicleManagementManager.OnStoreSave();
		super.OnStoreSave(ctx);
	}
};
	
modded class CarScript 
{
	override void OnStoreSave(ParamsWriteContext ctx) 
	{
		ApexVehicleManagementManager.OnStoreSave();
		super.OnStoreSave(ctx);
	}

#ifdef DIAG_DEVELOPER
	override void SpawnUniversalParts()
	{
		GetInventory().CreateInInventory("HeadlightH7");
		GetInventory().CreateInInventory("HeadlightH7");

		if (IsVitalCarBattery())
		{
			GetInventory().CreateInInventory("CarBattery");
		}

		if (IsVitalTruckBattery())
		{
			GetInventory().CreateInInventory("TruckBattery");
		}

		if (IsVitalRadiator())
		{
			GetInventory().CreateInInventory("CarRadiator");
		}

		if (IsVitalSparkPlug())
		{
			GetInventory().CreateInInventory("SparkPlug");
		}

		if (IsVitalGlowPlug())
		{
			GetInventory().CreateInInventory("GlowPlug");
		}
	}

	override void SpawnAdditionalItems()
	{
		array<string> keys = {"MCK_CarKey_Blue", "MCK_CarKey_Green", "MCK_CarKey_Red", "MCK_CarKey_White", "MCK_CarKey_Yellow", "MCK_CarKey_Base"};
		GetInventory().CreateInInventory(keys.GetRandomElement());
	}
#endif
}
#endif