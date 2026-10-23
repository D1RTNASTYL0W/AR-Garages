class AR_EngineStartSettings
{
	bool CanStartUnassignedVehiclesWithoutKey = true;
	bool CanStartWithoutKey = true;
	bool CanStartWithKeyInHandOnly = false;
	bool CanStartWithKeyInInventory = false;
};

class AR_RemoteKeySettings
{
	bool CanUnlockRemotely = true;
	float Range = 15.0;
	ref array<string> VehiclesWithRemoteKey = { "Transport" };
};

class AR_KeyConfig
{	
	static const string PROFILE_ROOT = "$profile:AR/";
	static const string ID_FOLDER = "$profile:AR/VehicleID/";
	static const string LEGACY_ROOT = "$profile:AshesReborn/";
    static const string FULLPATH = "$profile:AR/AR_KeyConfig.json";
	static const string LEGACY_FULLPATH = "$profile:AshesReborn/AR_KeyConfig.json";

	private int version = 6;
    private bool CanCraftKey = true;
    bool CanUseAnyGrinder = true;
    private bool CanPickCarLocks = false;
	private ref array<string> RaidTools = { "Lockpick" };
	private int ToolDamage = 50;
	private float ChanceToPickLock = -1;
	private float TimeToPickLock = 200;
	private bool RaidResetsKeyAssigned = true;
	private bool LifetimeMaintenanceEnabled = false;
	private int MaxLifetime = 1296000;
	private int MaxLifetimeWithoutAnyPlayerInteraction = 432000;
    private bool CanPlayersResetKey = false;
    private bool HideInventoryWhenDoorsClosed = false;
    bool DoFileLogs = true;
    private bool ActivateExtendedLogs = true;
    bool CanLockBoats = true;
	private ref AR_EngineStartSettings EngineStartSettings = new AR_EngineStartSettings;
	private ref AR_RemoteKeySettings RemoteKeySettings = new AR_RemoteKeySettings;

	void AR_KeyConfig()
	{
		if (!GetGame().IsServer()) return;
		
		if (!FileExist(PROFILE_ROOT))
        {
            Print("[AR_KeyConfig] '" + PROFILE_ROOT + "' does NOT exist, creating directory!");
            MakeDirectory(PROFILE_ROOT);
        }
		if (!FileExist(ID_FOLDER))
		{
			Print("[AR_KeyConfig] '" + ID_FOLDER + "' does NOT exist, creating directory!");
			MakeDirectory(ID_FOLDER);
		}

		if (!FileExist(FULLPATH) && FileExist(LEGACY_FULLPATH))
		{
			Print("[AR_KeyConfig] copying '" + LEGACY_FULLPATH + "' to '" + FULLPATH + "'");
			CopyFile(LEGACY_FULLPATH, FULLPATH);
		}

		CopyIdFileIfMissing("vehicleid.bin");
		CopyIdFileIfMissing("carkeyid.bin");

        if (!FileExist(FULLPATH))
        {
            Print("[AR_KeyConfig] '" + FULLPATH + "' does NOT exist, creating default config!");
            Save();
            return; 
        }
		CanPlayersResetKey = false;
        Load();
    }

	void Load()
    {
		Print("[AR_KeyConfig] '" + FULLPATH + "' found, loading existing config!");
		JsonFileLoader<AR_KeyConfig>.JsonLoadFile(FULLPATH, this);
		VersionChecker();
    }
	
	int DesiredVersion()
	{
		return 6;
	}

	void VersionChecker()
	{
		if(version != DesiredVersion())
		{
			version = DesiredVersion();
			Save();
		}
	}

	protected void Save()
    {
        JsonFileLoader<AR_KeyConfig>.JsonSaveFile(FULLPATH, this);
    }

	int GetVersion()
	{
		return version;
	}

	float Get_ChanceToPickLock()
	{
		return ChanceToPickLock;
	}

	float Get_TimeToPickLock()
	{
		return TimeToPickLock;
	}

	float Get_ToolDamage()
	{
		return ToolDamage;
	}

	bool Get_CanPickCarLocks()
	{
		return CanPickCarLocks;
	}

	bool Get_ShouldResetKeyOnRaid()
	{
		return RaidResetsKeyAssigned;
	}

	bool Get_CanCraftKey()
	{
		return CanCraftKey;
	}

	array<string> Get_RaidTools()
	{
		return RaidTools;
	}

	bool IsLifetimeMaintenanceEnabled()
	{
		return LifetimeMaintenanceEnabled;
	}
	bool AreExtendedLogsActivated()
	{
		return ActivateExtendedLogs;
	}

	int Get_MaxLifetime()
	{
		return MaxLifetime;
	}

	int Get_MaxLifetimeWithoutAnyPlayerInteraction()
	{
		return MaxLifetimeWithoutAnyPlayerInteraction;
	}
	
	bool Get_CanPlayersResetKey()
	{
		return CanPlayersResetKey;
	}
	
	bool Get_HideInventory()
	{
		return HideInventoryWhenDoorsClosed;
	}


	int GetNextVehicleStoreID() 
	{
		int storeId = LoadStoreID("vehicleid.bin");
		storeId++;
		SaveStoreID(storeId, "vehicleid.bin");
		return storeId;
	}
	
	int GetNextCarKeyStoreID() 
	{
		int storeId = LoadStoreID("carkeyid.bin");
		storeId++;
		SaveStoreID(storeId, "carkeyid.bin");
		return storeId;
	}

	void CopyIdFileIfMissing(string fileName)
	{
		string destination = ID_FOLDER + fileName;
		if (FileExist(destination))
			return;
		string source = PROFILE_ROOT + fileName;
		if (!FileExist(source))
			source = LEGACY_ROOT + fileName;
		if (!FileExist(source))
			return;
		Print("[AR_KeyConfig] copying '" + source + "' to '" + destination + "'");
		CopyFile(source, destination);
	}

	void SaveStoreID(int storeID, string fileName) 
	{
		if (!FileExist(ID_FOLDER))
			MakeDirectory(ID_FOLDER);
		FileSerializer fs = new FileSerializer();
		fs.Open(ID_FOLDER + fileName, FileMode.WRITE);
		fs.Write(storeID);
		fs.Close();
	}

	int LoadStoreID(string fileName) 
	{
		int storeID = 1;
		FileSerializer fs = new FileSerializer();
		if (!fs.Open(ID_FOLDER + fileName, FileMode.READ)) 
		{
			SaveStoreID(storeID, fileName);
			return storeID;
		}
		fs.Read(storeID);
		fs.Close();
		return storeID;
	}

	AR_EngineStartSettings GetEngineStartSettings()
	{
		return EngineStartSettings;
	}

	AR_RemoteKeySettings GetRemoteKeySettings()
	{
		return RemoteKeySettings;
	}
};