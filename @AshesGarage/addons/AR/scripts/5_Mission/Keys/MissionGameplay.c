modded class MissionGameplay
{
	void MissionGameplay()
	{
		GetRPCManager().AddRPC("Config", "ClientStoreARKeyConfig", this, SingeplayerExecutionType.Client);
		GetRPCManager().SendRPC("Config", "GetARKeyServerConfig", null, true, null);
	}

	void ClientStoreARKeyConfig(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        Param1<ref AR_KeyConfig> data;
        if (type == CallType.Client)
		{
			if (!ctx.Read(data)) return;
			g_Game.SetARKeyConfig( data.param1 );			
		}
    }
};