class ActionCraftCarKeyCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime(5);
	}
};

class ActionCraftCarKey: ActionLockUnlockCar
{	
	
	ref array<string> m_AR_Keys = { "AR_CarKey_Base", "AR_CarKey_Blue", "AR_CarKey_Green", "AR_CarKey_Red",  
	"AR_CarKey_White", "AR_CarKey_Yellow", "AR_CarKey_Purple", "AR_CarKey_Pink" };
	void ActionCraftCarKey()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_INTERACT;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
		m_LockTargetOnUse 	= false;
        m_CallbackClass = ActionCraftCarKeyCB;        
	}
		
	override string GetText()
	{
		return "#ar_key_action_craft";
	}
	
	// override bool HasTarget()
	// {
	// 	return false;
	// }

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

		if(!g_Game.GetARKeyConfig().Get_CanCraftKey())
			return false;

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

		Wrench wrench = Wrench.Cast(item);
		if (wrench)
		{
			return true;
		}
		

        return false;
	}
	
	override void OnFinishProgressServer( ActionData action_data )
	{	
		PlayerBase player = action_data.m_Player;
		Wrench wrench = Wrench.Cast(action_data.m_MainItem);            
		if(wrench)
		{
			AR_CarKey_Base duplicateKey = AR_CarKey_Base.Cast( GetGame().CreateObject(m_AR_Keys.GetRandomElement(), player.GetPosition()) );			
			wrench.DeleteSafe();
			AR_LogKeyActivity(player, "crafted new key.");
		}
	}    
};