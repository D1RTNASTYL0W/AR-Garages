modded class DayZGame 
{
	ref ApexVehicleManagementSettings m_ApexVehicleManagementSettings;
	
	ApexVehicleManagementSettings GetApexVehicleManagementSettings() 
	{
		#ifdef SERVER
		if(!m_ApexVehicleManagementSettings)
		{
			m_ApexVehicleManagementSettings = new ApexVehicleManagementSettings;
			m_ApexVehicleManagementSettings.Init();
		}
		#endif
		return m_ApexVehicleManagementSettings;
	}

    void SetApexVehicleManagementSettings(ApexVehicleManagementSettings config)
	{
		m_ApexVehicleManagementSettings = config;
	}
};
