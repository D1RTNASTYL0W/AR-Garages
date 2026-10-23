class ActionLockCarInside: ActionLockUnlockCar
{	
	void ActionLockCarInside()
	{
		m_FullBody = false;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_STARTENGINE;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ALL;
	}

	override void CreateConditionComponents()
	{
		super.CreateConditionComponents();

		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTNonRuined(0.8);
	}

	override bool CanBeUsedInVehicle()
	{
		return true;
	}
		
	override string GetText()
	{
		return "#ar_key_action_lock";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if(!target) 
			return false;

		CarScript carScript;
		if (Class.CastTo(carScript, target.GetParent()))
		{
			if (carScript.AR_HasKeyAssigned() && !carScript.AR_IsLocked() && player.IsInVehicle() && !carScript.CheckOpenedDoors())
				return true;
		}

        return false;
	}
	
	override void OnFinishProgressServer( ActionData action_data )
	{	
		PlayerBase player = action_data.m_Player;
		CarScript carScript = CarScript.Cast(action_data.m_Target.GetParent());
		if( carScript )
		{
            carScript.LockVehicle();			
			AR_LogKeyActivity(player, "locked vehicle " + carScript.GetDisplayName() + " (ID: " + AR_VehicleLock.GetTransportId(carScript) + ") from inside");
		}
	}
};