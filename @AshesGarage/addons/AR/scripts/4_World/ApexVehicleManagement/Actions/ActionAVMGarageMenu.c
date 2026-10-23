class ActionAVMGarageMenu : ActionInteractBase
{
	void ActionAVMGarageMenu()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_OPENDOORFW;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_HUDCursorIcon = CursorIcons.OpenDoors;
	}

	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTNone;
	}

	override string GetText() 
	{
		return "#avm_garageaction";
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item) 
	{
		if(!target.GetObject())
		{
			return false;
		}
		string selection = target.GetObject().GetActionComponentName(target.GetComponentIndex());
		if(selection && selection != "parking_meter")
		{
			return false;
		}
		if (target.GetObject().MemoryPointExists("parking_pos"))
		{
			return true;
		}
		return false;
	}

#ifdef SERVER
	override void OnEndServer(ActionData action_data) 
	{
		ApexVehicleManagementManager.SendParkedList(action_data.m_Player, action_data.m_Target.GetObject());
	}
#endif
}
