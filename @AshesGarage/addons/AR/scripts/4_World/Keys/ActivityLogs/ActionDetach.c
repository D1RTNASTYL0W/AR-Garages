#ifdef SERVER	
modded class ActionDetach: ActionInteractBase
{
	override void OnExecuteServer( ActionData action_data )
	{
		super.OnExecuteServer(action_data);
		if (action_data.m_Player && action_data.m_Player.GetIdentity())
		{
			PlayerBase player = PlayerBase.Cast(action_data.m_Player);
			Object target = Object.Cast(action_data.m_Target.GetObject());
			Transport transport = Transport.Cast(action_data.m_Target.GetParent());
            if(transport && g_Game.GetARKeyConfig().AreExtendedLogsActivated())
            {
				AR_LogKeyActivity(player, "detached item=" + Object.GetDebugName(target) + " from vehicle " + transport.GetDisplayName() + " (ID: " + AR_VehicleLock.GetTransportId(transport) + ")");
			}				
		}
	}
};
#endif