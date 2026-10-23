modded class ActionPushObject
{
    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
		if (target)
		{
            Transport transport = Transport.Cast(target.GetObject());
			if(!transport)
				transport = Transport.Cast(target.GetParent());
			if(transport)
			{
				if (AR_VehicleLock.IsLocked(transport))
					return false;
			}
		}

        return super.ActionCondition(player, target, item);
    }
    
    //extended  logs
	override void OnEndServer(ActionData action_data)
	{
        super.OnEndServer(action_data);
		if (action_data.m_Player && action_data.m_Player.GetIdentity())
		{
			ActionPushObjectData data = ActionPushObjectData.Cast(action_data);
			Transport transport = Transport.Cast(data.m_Object);
            if (transport && g_Game.GetARKeyConfig().AreExtendedLogsActivated())
            {				
				AR_LogKeyLine("Player " + action_data.m_Player.GetIdentity().GetName() + " (" + action_data.m_Player.GetPosition() + " steam64id=" + action_data.m_Player.GetIdentity().GetPlainId() + ") pushed " + transport.GetDisplayName() + " (ID: " + AR_VehicleLock.GetTransportId(transport) + ")");				
			}
		}
    }
};