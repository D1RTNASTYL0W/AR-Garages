modded class PlayerBase 
{
	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
		super.OnRPC(sender, rpc_type, ctx);

		switch (rpc_type)
		{
		#ifndef SERVER
		case AVM_RPCs.RPC_SEND_PARKEDCARS:
			RPC_ParkedList(ctx);
			break;
		case AVM_RPCs.RPC_SEND_SETTINGS_TO_CLIENT:
			Param1<ApexVehicleManagementSettings> param;
			ctx.Read(param);
			if(param.param1)
			{
				g_Game.SetApexVehicleManagementSettings(param.param1);
			}
			break;
		#endif
		#ifdef SERVER
		case AVM_RPCs.RPC_RETRIEVE_PARKEDCAR:
			ApexVehicleManagementManager.RPC_GetVehicle(sender, ctx);
			break;
		case AVM_RPCs.RPC_PARKCAR:
			ApexVehicleManagementManager.RPC_ParkVehicle(sender, ctx);
			break;
		case AVM_RPCs.RPC_DELETECAR:
			ApexVehicleManagementManager.RPC_DeleteVehicle(sender, ctx);
			break;
		case AVM_RPCs.RPC_REQUEST_KEY_NAME:
			ApexVehicleManagementManager.RPC_RequestKeyName(sender, ctx);
			break;
		#endif
		default:
			break;
		}
	}

#ifndef SERVER
	void RPC_ParkedList(ParamsReadContext ctx) 
	{
		Param1<AVM_TransferData> param;
		if (!ctx.Read(param) || !param || !param.param1)
			return;

		AVM_GarageMenu menu = AVM_GarageMenu.Cast(GetGame().GetUIManager().EnterScriptedMenu(MENU_AVM_GARAGEMENU, NULL));
		if (!menu)
			return;

		menu.SetTarget(param.param1.Position, param.param1.Orientation);
		menu.SetCarsStoredCount(param.param1.CarsStoredCount);
		menu.SetNearbyParkCar(param.param1.NearbyParkCar);
		if (!param.param1.StoredCars)
			return;
		foreach(AVM_StoredCarClient StoredCar : param.param1.StoredCars)
		{
			if (StoredCar)
				menu.AddVehicle(StoredCar);
		}
	}
#endif
};