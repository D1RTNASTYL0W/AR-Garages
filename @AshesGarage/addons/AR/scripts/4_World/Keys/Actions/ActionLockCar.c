class ActionLockCar: ActionLockUnlockCar
{			
	override string GetText()
	{
		return "#ar_key_action_lock";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if(!target)
			return false;

		if (!target.GetObject())
			return false;

		Transport transport = Transport.Cast(target.GetObject());
		if(!transport)
		{
			transport = Transport.Cast(target.GetParent());
			if(!transport)
			{
				return false;
			}
		}

		AR_MasterKey masterKey = AR_MasterKey.Cast(item);
		if (transport && masterKey)
		{
			if (!AR_VehicleLock.IsLocked(transport) && AR_VehicleLock.CanDoAction(transport))
				return true;
		}

        AR_CarKey_Base carKey = AR_CarKey_Base.Cast(item);
		if (transport && carKey)
		{
			if (AR_VehicleLock.HasKeyAssigned(transport) && AR_VehicleLock.CanDoAction(transport) && carKey.m_ARKeyId == AR_VehicleLock.GetTransportKeyId(transport))
				return true;
		}

        return false;
	}
	
	override void OnFinishProgressServer( ActionData action_data )
	{	
		PlayerBase player = action_data.m_Player;
		Transport transport = Transport.Cast(action_data.m_Target.GetObject());
		if(!transport)
		{
			transport = Transport.Cast(action_data.m_Target.GetParent());
		}
		if( transport )
		{
            AR_VehicleLock.LockTransport(transport);		
			AR_LogKeyActivity(player, "locked vehicle " + transport.GetDisplayName() + " (ID: " + AR_VehicleLock.GetTransportId(transport) + ")");		
		}
	}
};