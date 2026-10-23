#ifdef SERVER
modded class MissionServer 
{
	private ref ApexVehicleManagementSettings m_ApexVehicleManagementSettings;
	override void OnInit() 
	{
		super.OnInit();		
		m_ApexVehicleManagementSettings = new ApexVehicleManagementSettings;
		m_ApexVehicleManagementSettings.Init();		
        g_Game.SetApexVehicleManagementSettings(m_ApexVehicleManagementSettings);
		ApexVehicleManagementManager.Init();
	}

	override void InvokeOnConnect(PlayerBase player, PlayerIdentity identity)
    {
        super.InvokeOnConnect(player, identity);		
		auto avmSettings = new Param1<ApexVehicleManagementSettings>(m_ApexVehicleManagementSettings);
        GetGame().RPCSingleParam(player, AVM_RPCs.RPC_SEND_SETTINGS_TO_CLIENT, avmSettings, true, identity);
	}
}
#endif