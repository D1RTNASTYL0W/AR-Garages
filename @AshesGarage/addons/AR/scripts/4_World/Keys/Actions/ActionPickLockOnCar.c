class ActionPickLockOnCarCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        float raidTime = g_Game.GetARKeyConfig().Get_TimeToPickLock();
        if (raidTime > 0)
        {
            m_ActionData.m_ActionComponent = new CAContinuousRepeat(raidTime);
        }
        else
        {
            m_ActionData.m_ActionComponent = new CAContinuousTime(100);
        }
    }
}

class ActionPickLockOnCar : ActionLockUnlockCar
{
    void ActionPickLockOnCar()
    {
        m_CallbackClass = ActionPickLockOnCarCB;
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_INTERACT;
        m_FullBody = true;
        m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
    }

    override void CreateConditionComponents()
    {
        m_ConditionItem = new CCINonRuined;
        m_ConditionTarget = new CCTNone;
    }

    override string GetText()
    {
        return "#ar_key_action_raid";
    }

    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
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
                
        array<string> raidTools = g_Game.GetARKeyConfig().Get_RaidTools();
        if(raidTools.Find(item.GetType()) == -1)
            return false;

        bool canPickCarLocks = g_Game.GetARKeyConfig().Get_CanPickCarLocks();
        if (transport && canPickCarLocks && AR_VehicleLock.HasKeyAssigned(transport) && AR_VehicleLock.IsLocked(transport))
            return true;

        return false;
    }

    override void OnFinishProgressServer(ActionData action_data)
    {
        PlayerBase player = PlayerBase.Cast(action_data.m_Player);      
        int toolDamage = g_Game.GetARKeyConfig().Get_ToolDamage();
        float chanceToPickLock = g_Game.GetARKeyConfig().Get_ChanceToPickLock();
        Transport transport = Transport.Cast(action_data.m_Target.GetObject());
		if(!transport)
		{
			transport = Transport.Cast(action_data.m_Target.GetParent());
		}

		if( transport )
		{
            int roll = Math.RandomInt(0, 100);
            action_data.m_MainItem.DecreaseHealth(toolDamage, false);
            if (chanceToPickLock == -1 || roll < chanceToPickLock)
            {
                AR_VehicleLock.UnlockTransport(transport);
                if(g_Game.GetARKeyConfig().Get_ShouldResetKeyOnRaid())
                {
                    AR_VehicleLock.ResetTransport(transport);
                }
                
	            AR_LogKeyActivity(player, "picked lock for vehicle " + transport.GetDisplayName() + " (ID: " + AR_VehicleLock.GetTransportId(transport) + ")");
            }
        }
    }
};