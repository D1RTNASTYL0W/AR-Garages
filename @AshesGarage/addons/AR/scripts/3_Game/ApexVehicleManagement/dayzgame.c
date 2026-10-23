modded class DayZGame 
{
	ref ApexVehicleManagementSettings m_ApexVehicleManagementSettings;
    private ref AR_KeyConfig m_ARKeyConfig;
	
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

    AR_KeyConfig GetARKeyConfig()
    {
        if (!m_ARKeyConfig)
        {
            SetARKeyConfig(new AR_KeyConfig);
        }
	    return m_ARKeyConfig;
    }

    void SetARKeyConfig(AR_KeyConfig config)
	{
		m_ARKeyConfig = config;
	}
};
