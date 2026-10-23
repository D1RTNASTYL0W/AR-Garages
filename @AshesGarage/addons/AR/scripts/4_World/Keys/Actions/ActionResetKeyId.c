class ActionResetKeyIdCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime(10);
	}
};

class ActionResetKeyId: ActionLockUnlockCar
{	
	void ActionResetKeyId()
	{
        m_CallbackClass = ActionResetKeyIdCB;        
	}
		
	override string GetText()
	{
		return "#ar_key_action_resetkey";
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
		
		if (transport)
		{
        	AR_ResetKey resetKey = AR_ResetKey.Cast(item);
			if(resetKey)
			{
				return true;
			}
			
        	AR_CarKey_Base carKey = AR_CarKey_Base.Cast(item);
			if(carKey && carKey.m_ARKeyId == AR_VehicleLock.GetTransportKeyId(transport))
			{
				if(!AR_VehicleLock.HasKeyAssigned(transport) || !g_Game.GetARKeyConfig().Get_CanPlayersResetKey())
				{
					return false;
				}
				return true;   
			}
         
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
			AR_VehicleLock.ResetTransport(transport);
			AR_CarKey_Base carKey = AR_CarKey_Base.Cast(action_data.m_MainItem);            
            if(carKey)
            {
				carKey.ResetKey();
			}
			AR_LogKeyActivity(player, "reset key for vehicle " + transport.GetDisplayName() + " (ID: " + AR_VehicleLock.GetTransportId(transport) + ")");
		}
	}    
};