#ifdef SERVER
modded class MissionServer 
{
	private ref ApexVehicleManagementSettings m_ApexVehicleManagementSettings;
    private ref AR_KeyConfig m_ARKeyConfig;

    void MissionServer()
    {
        GetRPCManager().AddRPC("Config", "GetARKeyServerConfig", this, SingeplayerExecutionType.Server);
    }

	override void OnInit() 
	{
		super.OnInit();		
		m_ApexVehicleManagementSettings = new ApexVehicleManagementSettings;
		m_ApexVehicleManagementSettings.Init();		
        g_Game.SetApexVehicleManagementSettings(m_ApexVehicleManagementSettings);
		ApexVehicleManagementManager.Init();
        m_ARKeyConfig = new AR_KeyConfig;
        g_Game.SetARKeyConfig(m_ARKeyConfig);
	}

	override void InvokeOnConnect(PlayerBase player, PlayerIdentity identity)
    {
        super.InvokeOnConnect(player, identity);		
		auto avmSettings = new Param1<ApexVehicleManagementSettings>(m_ApexVehicleManagementSettings);
        GetGame().RPCSingleParam(player, AVM_RPCs.RPC_SEND_SETTINGS_TO_CLIENT, avmSettings, true, identity);
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(ApexVehicleManagementManager.BindAllPlayerKeyNames, 2000, false, player);
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(AR_SyncConnectedPlayerKeys, 2500, false, player);
	}

	void AR_SyncConnectedPlayerKeys(PlayerBase player)
	{
		if (!player || !player.GetInventory())
			return;
		array<EntityAI> entities = new array<EntityAI>;
		player.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, entities);
		for (int i = 0; i < entities.Count(); i++)
		{
			AR_CarKey_Base carKey = AR_CarKey_Base.Cast(entities.Get(i));
			if (carKey && carKey.GetARKeyId() > 0)
				carKey.AR_SyncVehicleNameToClients();
		}
	}

    void GetARKeyServerConfig(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        if (type == CallType.Server && sender != null)                    
            GetRPCManager().SendRPC("Config", "ClientStoreARKeyConfig", new Param1<ref AR_KeyConfig>( m_ARKeyConfig ), true, sender);
    }
}
#endif
