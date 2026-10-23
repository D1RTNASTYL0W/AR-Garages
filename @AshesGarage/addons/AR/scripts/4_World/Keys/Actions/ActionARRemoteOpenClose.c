class ActionARRemoteOpenClose: ActionSingleUseBase
{
	private int m_LastActionCheck = 0;
	private int m_LastActionCheckTimer = 3000; //miliseconds

	void ActionARRemoteOpenClose()
	{
        m_Text = "Remote lock/unlock vehicle";
	}
	
	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNone;
	}

	override bool HasTarget()
	{
		return false;
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		AR_CarKey_Base carKey = AR_CarKey_Base.Cast(item);
		if (carKey)
		{
            int currentTime = GetGame().GetTime();
            if(currentTime - m_LastActionCheck > m_LastActionCheckTimer)
            {
			    return carKey.IsKeyAssigned();
            }
		}
		return false;
	}
	
	override void OnExecuteServer( ActionData action_data )
	{
		//find car nearby with id
        PlayerBase player = PlayerBase.Cast(action_data.m_Player);
        AR_CarKey_Base carKey = AR_CarKey_Base.Cast(action_data.m_MainItem);
        if(player && carKey)
        {
            Transport transport = carKey.FindNearbyTransport(player);
            if(!transport)
            {
                return;
            }
            AR_VehicleLock.PlayLockUnlockSound_Server(transport);
            CarScript carScript = CarScript.Cast(transport);
            if(carScript)
            {
                if(carScript.AR_IsLocked())
                {
                    carScript.AR_SetHeadlights(false);
                    carScript.AR_SetHeadlights(true);
                    GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(carScript.AR_SetHeadlights, 200, false, false);
                    GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(carScript.AR_SetHeadlights, 300, false, true);
                    GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(carScript.AR_SetHeadlights, 500, false, false);
                }
                else
                {
                    carScript.AR_SetHeadlights(false);
                    carScript.AR_SetHeadlights(true);
                    GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(carScript.AR_SetHeadlights, 500, false, false);
                }
            }
            if(AR_VehicleLock.IsLocked(transport))
            {
                AR_LogKeyActivity(player, "locked vehicle " + transport.GetDisplayName() + " (ID: " + AR_VehicleLock.GetTransportId(transport) + ")");
            }
            else
            {
                AR_LogKeyActivity(player, "unlocked vehicle " + transport.GetDisplayName() + " (ID: " + AR_VehicleLock.GetTransportId(transport) + ")");
            }
            
            AR_VehicleLock.ToggleLockTransport(transport);		
        }
	}

    override void OnExecuteClient( ActionData action_data )
	{
        int currentTime = GetGame().GetTime();
        if(currentTime - m_LastActionCheck > m_LastActionCheckTimer)
        {
            m_LastActionCheck = currentTime;
        }
    }
};