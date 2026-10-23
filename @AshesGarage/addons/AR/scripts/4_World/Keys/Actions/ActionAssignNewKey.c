class ActionAssignNewKeyCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime(5);
	}
};

class ActionAssignNewKey: ActionLockUnlockCar
{	
	void ActionAssignNewKey()
	{
		ActionLockUnlockCar();
        m_CallbackClass = ActionAssignNewKeyCB;        
	}
		
	override string GetText()
	{
		return "#ar_key_action_assign";
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

        AR_CarKey_Base carKey = AR_CarKey_Base.Cast(item);
		if (carKey && carKey.GetARKeyId() <= 0)
		{
			if(transport)
			{
				if(AR_VehicleLock.HasKeyAssigned(transport))
					return false;

				if(AR_VehicleLock.CanDoAction(transport))
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
			AR_CarKey_Base carKey = AR_CarKey_Base.Cast(action_data.m_MainItem);            
            if(carKey)
            {
                int ar_key_id = carKey.GenerateNewID();
				carKey.SetNewARKeyId(ar_key_id);
				AR_VehicleLock.AssignNewKey(transport, ar_key_id, player);
				carKey.SetAVMVehicleName(transport.GetDisplayName());
				AR_LogKeyActivity(player, " assigned new owner and key (ID: " + ar_key_id + " ) to vehicle " + transport.GetDisplayName() + " (ID: " + AR_VehicleLock.GetTransportId(transport) + ")");
            }
		}
	}    
};