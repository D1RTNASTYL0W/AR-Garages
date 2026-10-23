class ApexVehicleManagementVehicle 
{
	[NonSerialized()]
	protected Transport m_Vehicle;

	string ClassName;
	string DisplayName;
	int CarScriptId = -1;
	int CarKeyId = -1;
	string ParkedPlayerId;
	string ParkedPlayerName;
	string OriginalOwnerId;
	string OriginalOwnerName;
	vector LastPosition;

	ref array<float> fluids = new array<float>;


	ref MF_Inventory inventory;

	void ApexVehicleManagementVehicle(Transport vehicle = null) 
	{
		m_Vehicle = vehicle;
	}

	protected string GetFileName(int id) 
	{
		return ApexVehicleManagementSettings.VEHICLES_FOLDER + id.ToString() + ".json";
	}

	void Load(int id) 
	{
		JsonFileLoader<ApexVehicleManagementVehicle>.JsonLoadFile(GetFileName(id), this);
	}

	void Save(int id) 
	{
		if (!FileExist(ApexVehicleManagementSettings.VEHICLES_FOLDER))
			MakeDirectory(ApexVehicleManagementSettings.VEHICLES_FOLDER);
		JsonFileLoader<ApexVehicleManagementVehicle>.JsonSaveFile(GetFileName(id), this);
	}

	void Delete(int id)
	{
		string fileName = GetFileName(id);
		if (FileExist(fileName))
		{
			DeleteFile(fileName);
		}
	}

	bool StoreVehicle(string playerID, string playerName) 
	{
		if (!m_Vehicle)
			return false;

		ClassName = m_Vehicle.GetType();
		GetGame().ObjectGetDisplayName(m_Vehicle, DisplayName);
		LastPosition = m_Vehicle.GetPosition();
		CarKeyId = AR_VehicleLock.GetTransportKeyId(m_Vehicle);
		CarScriptId = AR_VehicleLock.GetTransportId(m_Vehicle);
		OriginalOwnerId = AR_VehicleLock.GetOriginalOwnerId(m_Vehicle);
		OriginalOwnerName = AR_VehicleLock.GetOriginalOwnerName(m_Vehicle);
		ParkedPlayerId = playerID;
		ParkedPlayerName = playerName;
		int fluidSize = EnumTools.GetEnumSize(CarFluid);
		if(m_Vehicle.IsInherited(BoatScript))
		{
			fluidSize = EnumTools.GetEnumSize(BoatFluid);
		}
		for (int i = 0; i < fluidSize; ++i)
		{
			float fluidFraction = -1; 
			float fluidCapacity = -1; 
			AR_VehicleLock.GetFluidData(m_Vehicle, i, fluidFraction, fluidCapacity);
			fluids.Insert(fluidFraction * fluidCapacity);
		}
		inventory = new MF_Inventory(g_Game.GetApexVehicleManagementSettings().StoreCargo);
		return inventory.StoreEntity(m_Vehicle);
	}

	bool RestoreVehicle(vector position, vector orientation, PlayerBase player) 
	{
		Object obj = Object.Cast(GetGame().CreateObjectEx(ClassName, position, ECE_LOCAL | ECE_CREATEPHYSICS | ECE_TRACE));
		Class.CastTo(m_Vehicle, obj);
		if(!m_Vehicle)
		{
			return false;
		}
		m_Vehicle.SetPosition(position);
		m_Vehicle.SetOrientation(orientation);
		m_Vehicle.SetDirection(m_Vehicle.GetDirection());
		AR_VehicleLock.AssignNewData(m_Vehicle, CarScriptId, CarKeyId, OriginalOwnerId, OriginalOwnerName);
		// Sync happens after RestoreInventory()
		int i = 0;
		int fluidSize = EnumTools.GetEnumSize(CarFluid);		
		if(m_Vehicle.IsInherited(BoatScript))
		{
			fluidSize = EnumTools.GetEnumSize(BoatFluid);
		}

		bool refuelCars = g_Game.GetApexVehicleManagementSettings().RefuelCars;

		for (i = 0; i < fluidSize; ++i)
		{
			AR_VehicleLock.FillFluid(m_Vehicle, i, fluids[i], refuelCars);
		}

		GetGame().RemoteObjectCreate(m_Vehicle);
		if(!inventory.RestoreParentEntity(m_Vehicle, player, g_Game.GetApexVehicleManagementSettings().HealCars))
		{			
			GetGame().RPCSingleParam(player, MUCH_RPC.RPC_CLIENT_SHOWWARNINGUI, null, true, player.GetIdentity());
		}
		m_Vehicle.SetSynchDirty();
		return true;
	}
}