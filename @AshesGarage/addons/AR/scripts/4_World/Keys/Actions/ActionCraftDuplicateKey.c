class ActionCraftDuplicateKeyCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime(5);
	}
};

class ActionCraftDuplicateKey: ActionLockUnlockCar
{	
	void ActionCraftDuplicateKey()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_INTERACT;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
        m_CallbackClass = ActionCraftDuplicateKeyCB;
		m_LockTargetOnUse 	= false;
		m_Text = "#ar_key_action_duplicate";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if(!target) 
			return false;

		Object targetObject = target.GetObject();
		if (!targetObject)
		{			
			targetObject = target.GetParent();
			if (!target.GetParent())
			{
				return false;
			}
		}

		bool HasCorrectTarget = false;
		if(targetObject.IsInherited(AR_KeyGrinder)) 
		{
			HasCorrectTarget = true;
		}
		if(g_Game.GetARKeyConfig().CanUseAnyGrinder && !HasCorrectTarget)
		{
			if(targetObject.IsKindOf("StaticObj_Furniture_grinder")  || HasGrinderProxy(target)) 
			{
				HasCorrectTarget = true;
			}
		}		

		if(!HasCorrectTarget)
		{
			return false;
		}
		
        AR_CarKey_Base carKey = AR_CarKey_Base.Cast(item);
		if (carKey)
		{
			m_Text = "#ar_key_action_duplicate";
			if(!carKey.IsKeyAssigned())
			{
				m_Text = "#ar_key_action_duplicate_error";
				if(GetGame().IsServer())
				{
					return false;
				}
				else
				{
					return true;
				}
			}
			
			return true;            
		}

        return false;
	}
	
	override void OnFinishProgressServer( ActionData action_data )
	{	
		PlayerBase player = action_data.m_Player;
		AR_CarKey_Base carKey = AR_CarKey_Base.Cast(action_data.m_MainItem);            
		if(carKey)
		{
			int ar_key_id = carKey.m_ARKeyId;
			string copiedName = carKey.GetAVMVehicleName();
			AR_CarKey_Base duplicateKey = AR_CarKey_Base.Cast( GetGame().CreateObject(carKey.GetType(), player.GetPosition()) );			
			duplicateKey.SetNewARKeyId(ar_key_id);
			if (copiedName != "")
				duplicateKey.SetAVMVehicleName(copiedName);
			AR_LogKeyActivity(player, "duplicated key (ID: " + ar_key_id + " )");
		}
	}    
};