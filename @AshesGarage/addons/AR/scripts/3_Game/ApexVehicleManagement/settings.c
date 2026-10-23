class ApexVehicleManagementSettings 
{
	[NonSerialized()]
	static const string FOLDER = "$profile:AR";
	[NonSerialized()]
	static const string CONFIG_FOLDER = FOLDER;
	[NonSerialized()]
	static const string VEHICLES_FOLDER = FOLDER + "\\Vehicles\\";
	[NonSerialized()]
	static const string SETTINGS = FOLDER + "\\AR_VehicleManagementConfig.json";
	[NonSerialized()]
	static const string STORED_CARS = FOLDER + "\\StoredCars.json";
	[NonSerialized()]
	static const string LEGACY_FOLDER = "$profile:ApexVehicleManagement";
	[NonSerialized()]
	static const string LEGACY_SETTINGS = LEGACY_FOLDER + "\\settings.json";
	[NonSerialized()]
	static const string LEGACY_AR_SETTINGS = LEGACY_FOLDER + "\\AR_VehicleManagementConfig.json";
	[NonSerialized()]
	static const string LEGACY_STORED_CARS = LEGACY_FOLDER + "\\StoredCars.json";
	[NonSerialized()]
	static const string LEGACY_VEHICLES_FOLDER = LEGACY_FOLDER + "\\Vehicles\\";

	#ifdef SERVER
	/**
	 * Initializes and load ApexVehicleManagement settings with default values if the settings file does not exist.
	 */
	void Init() 
	{
		if (!FileExist(FOLDER))
			MakeDirectory(FOLDER);
		if (!FileExist(VEHICLES_FOLDER))
			MakeDirectory(VEHICLES_FOLDER);

		CopyIfMissing(LEGACY_SETTINGS, SETTINGS);
		CopyIfMissing(LEGACY_AR_SETTINGS, SETTINGS);
		CopyIfMissing(LEGACY_STORED_CARS, STORED_CARS);
		CopyLegacyVehicleFiles();

		if (!FileExist(SETTINGS))
			JsonFileLoader<ApexVehicleManagementSettings>.JsonSaveFile(SETTINGS, this);

		JsonFileLoader<ApexVehicleManagementSettings>.JsonLoadFile(SETTINGS, this);
		if (version != wantedVersion) 
		{
			if (version < 5)
				UseMongoDB = true;
			if (version < 6)
				StoreCargo = true;
			version = wantedVersion;
			JsonFileLoader<ApexVehicleManagementSettings>.JsonSaveFile(SETTINGS, this);
		}
	}

	protected void CopyIfMissing(string source, string destination)
	{
		if (FileExist(destination) || !FileExist(source))
			return;
		CopyFile(source, destination);
	}

	protected void CopyLegacyVehicleFiles()
	{
		if (!FileExist(LEGACY_VEHICLES_FOLDER))
			return;
		if (!FileExist(VEHICLES_FOLDER))
			MakeDirectory(VEHICLES_FOLDER);

		string fileName;
		FileAttr fileAttr;
		FindFileHandle vehicles = FindFile(LEGACY_VEHICLES_FOLDER + "*.json", fileName, fileAttr, 0);
		if (!vehicles)
			return;

		CopyLegacyVehicleFile(fileName);
		while (FindNextFile(vehicles, fileName, fileAttr))
		{
			CopyLegacyVehicleFile(fileName);
		}
	}

	protected void CopyLegacyVehicleFile(string fileName)
	{
		if (fileName == "")
			return;
		string destination = VEHICLES_FOLDER + fileName;
		if (FileExist(destination))
			return;
		CopyFile(LEGACY_VEHICLES_FOLDER + fileName, destination);
	}
	#endif
	/******************************************************************
	 * Settings
	 ******************************************************************/

	// JSON version for modifications
	[NonSerialized()]
	int wantedVersion = 6;
	int version;

	// Delete expired cars
	bool DeleteExpiredCars;

	// The amount of time in seconds that a vehicle will be stored before it is deleted.
	int StoredCarLifetime = 60 * 60 * 24 * 31; // 31 days

	// The amount of time in seconds that vehicles will be stored if the server does not perform the save.
	float StoreSaveInterval = 60;
	bool RefuelCars = false;
	bool HealCars = false;
	//-1 or 0 will have infinite
	int MaxStoredCarsPerPlayer = -1;
	bool StoreCargo = true;
	// true = Universal API / MongoDB shared garage. false = original local JSON files.
	bool UseMongoDB = true;
	ref array<string> ItemsBlacklist = new array<string>;
	bool AllowRetrieveWithinRadiusOnly = false;
	int RadiusRetrieve = 100;
	ref array<string> VehicleBlacklist = new array<string>;
}
