class ActionLockUnlockCarCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime(1);
	}
};

class ActionLockUnlockCar: ActionContinuousBase
{	
	void ActionLockUnlockCar()
	{
		m_CallbackClass = ActionLockUnlockCarCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_INTERACT;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
	}

	override void CreateConditionComponents()
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNone;
	}

	override bool CanBeUsedInVehicle()
	{
		return false;
	}

	protected bool IsCarDoorHood(ActionTarget target)
	{
		CarDoor carDoor = CarDoor.Cast(target.GetObject());

		CarScript car;
		if (Class.CastTo(car, target.GetParent()) && carDoor)
		{
			array<string> selections = new array<string>();
			carDoor.GetActionComponentNameList(target.GetComponentIndex(), selections);
					
			for (int i = 0; i < selections.Count(); i++)
			{
				string m_AnimSource = car.GetAnimSourceFromSelection(selections[i]);
				if(m_AnimSource == "DoorsHood")
					return true;
			}
		}

		return false;
	}

	

	bool HasGrinderProxy(ActionTarget actionTarget)
	{
		//"Land_Repair_Center"

		House building;
		if (!Class.CastTo(building, actionTarget.GetObject()))
		{
			if (!Class.CastTo(building, actionTarget.GetParent()))
			{
				return false;
			}
		}
		LOD lod = building.GetLODByName(LOD.NAME_GEOMETRY);
		if(!lod)
		{
			return false;
		}
		array<Selection> selections	= new array<Selection>();
		if(!lod.GetSelections(selections))
			return false;
		
		for (int i = 0; i < selections.Count(); ++i)
		{	
			string selectionName = selections[i].GetName();
			selectionName.ToLower();				
			if(selectionName.Contains("proxy") && selectionName.Contains("grinder"))
			{	
				vector hitPosition = actionTarget.GetCursorHitPos();
				vector proxyPosition = building.ModelToWorld(building.GetSelectionPositionLS( selections[i].GetName()));
				float distance = vector.Distance(hitPosition, proxyPosition);
				if(distance < 0.8)
				{
					return true;
				}
				else
				{
					return false;
				}
			}
		}
		return false;
	}
};