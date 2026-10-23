class ApexVehicleManagementSettings 
{
	[NonSerialized()]
	static const string FOLDER = "$profile:ApexVehicleManagement";
	[NonSerialized()]
	static const string VEHICLES_FOLDER = FOLDER + "\\Vehicles\\";
	[NonSerialized()]
	static const string SETTINGS = FOLDER + "\\settings.json";
	[NonSerialized()]
	static const string STORED_CARS = FOLDER + "\\StoredCars.json";

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

		// Save the settings if they don't exist
		if (!FileExist(SETTINGS))
			JsonFileLoader<ApexVehicleManagementSettings>.JsonSaveFile(SETTINGS, this);

		JsonFileLoader<ApexVehicleManagementSettings>.JsonLoadFile(SETTINGS, this);
		if (version != wantedVersion) 
		{
			version = wantedVersion;
			JsonFileLoader<ApexVehicleManagementSettings>.JsonSaveFile(SETTINGS, this);
		}
	}
	#endif
	/******************************************************************
	 * Settings
	 ******************************************************************/

	// JSON version for modifications
	[NonSerialized()]
	int wantedVersion = 4;
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
	bool StoreCargo = false;
	ref array<string> ItemsBlacklist = new array<string>;
	bool AllowRetrieveWithinRadiusOnly = false;
	int RadiusRetrieve = 100;
	ref array<string> VehicleBlacklist = new array<string>;
}