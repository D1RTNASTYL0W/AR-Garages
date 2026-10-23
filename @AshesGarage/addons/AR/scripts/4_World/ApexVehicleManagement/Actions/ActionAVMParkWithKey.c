class ActionAVMParkWithKey: ActionLockUnlockCar
{		
    void ActionAVMParkWithKey()
    {
        m_Text = "Park vehicle (Admin)";
    }

    
	override string GetText() 
	{
		return m_Text;
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

		AVM_ParkingKey parkingKey = AVM_ParkingKey.Cast(item);
		if (transport && parkingKey)
		{
            string reason = "";
            if (AR_VehicleLock.HasKeyAssigned(transport) && AVM_Helper.CanPark(player, transport, reason))
            {
                m_Text = "Park vehicle";
			    return true;
            }
            else
            {           
        		m_Text = "Cannot park: " + reason;
				if(GetGame().IsServer())
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
            string reason = "";
			if(AR_VehicleLock.HasKeyAssigned(transport) && AVM_Helper.CanPark(player, transport, reason))
			{
	            #ifdef SERVER
	            if(ApexVehicleManagementManager.ParkVehicle(player.GetIdentity(), transport.GetPosition()))
	            {
	                AR_LogKeyActivity(player, "(Admin) parked vehicle " + transport.GetDisplayName() + " (ID: " + AR_VehicleLock.GetTransportId(transport) + ")");
	            }
	            #endif	
			}
		}
	}
};