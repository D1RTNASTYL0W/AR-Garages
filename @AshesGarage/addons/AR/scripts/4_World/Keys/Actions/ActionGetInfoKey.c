class ActionGetInfoKeyCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime(1);
	}
};

class ActionGetInfoKey: ActionLockUnlockCar
{	
	void ActionGetInfoKey()
	{
		ActionLockUnlockCar();
        m_CallbackClass = ActionGetInfoKeyCB;        
	}
		
	override string GetText()
	{
		return "#ar_key_action_getinfo";
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

        AR_InfoKey infoKey = AR_InfoKey.Cast(item);
		if (infoKey && transport)
		{
			return true;         
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
			AR_InfoKey infoKey = AR_InfoKey.Cast(action_data.m_MainItem);            
            if(infoKey)
            {          
                DisplayReport(action_data.m_Player, transport);
            }
		}

	}

	override void OnFinishProgressClient( ActionData action_data )
	{	
		PlayerBase player = action_data.m_Player;
		Transport transport = Transport.Cast(action_data.m_Target.GetObject());
		if(!transport)
		{
			transport = Transport.Cast(action_data.m_Target.GetParent());
		}
		if( transport )
		{
			AR_InfoKey infoKey = AR_InfoKey.Cast(action_data.m_MainItem);            
            if(infoKey)
            {          
				string clipboard = AR_VehicleLock.GetTransportId(transport).ToString();
				GetGame().CopyToClipboard(clipboard);
            }
		}
	}

	void DisplayReport(PlayerBase player, Transport transport)
	{
		SendARKeyMessageToClient(player, "Vehicle Report:");
		SendARKeyMessageToClient(player, "Vehicle: " + transport);
		SendARKeyMessageToClient(player, "Vehicle key id: " + AR_VehicleLock.GetTransportKeyId(transport));
		SendARKeyMessageToClient(player, "Vehicle id: " + AR_VehicleLock.GetTransportId(transport));
		SendARKeyMessageToClient(player, "Vehicle original owner: " + AR_VehicleLock.GetOriginalOwnerName(transport));
		SendARKeyMessageToClient(player, "Vehicle original owner id: " + AR_VehicleLock.GetOriginalOwnerId(transport));
		SendARKeyMessageToClient(player, "Last time interacted: " + AR_TimestampToString(AR_VehicleLock.GetLastInteractedWithTime(transport)));
		SendARKeyMessageToClient(player, "Remaining lifetime: " + MF_Helper.SecondsToDays(AR_VehicleLock.GetRemainingTimeTilDespawnTransport(transport)));
		//SendARKeyMessageToClient(player, "Max lifetime from types: " + MF_Helper.SecondsToDays(transport.GetLifetimeMax()));
	}
	
	void SendARKeyMessageToClient(Object reciever, string message)
	{
		//Printing to server too
		AR_LogKeyLine(message);
		PlayerBase man;
		if (GetGame().IsServer() && Class.CastTo(man, reciever) && reciever.IsAlive() && message != "")
		{
			Param1<string> reportParam = new Param1<string>("");
			reportParam.param1 = message;
			GetGame().RPCSingleParam(man, ERPCs.RPC_USER_ACTION_MESSAGE, reportParam, true, man.GetIdentity());
		}
	}
};