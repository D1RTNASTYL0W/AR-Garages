modded class ActionStartEngine : ActionContinuousBase
{	
	private bool m_LastInventoryCheckReturnValue = false;
	private int m_LastInventoryCheck = 0;
	private int m_LastInventoryCheckTimer = 5000;

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{	
		if(!super.ActionCondition(player, target, item))
		{
			return false;
		}		

		return AR_StartEngineCondition(player);
	}

	bool AR_StartEngineCondition(PlayerBase player)
	{
		AR_EngineStartSettings settings = g_Game.GetARKeyConfig().GetEngineStartSettings();
		if(settings)
		{
			if (player.GetCommand_Vehicle())
			{
				Transport transport = Transport.Cast(player.GetCommand_Vehicle().GetTransport());
				if (!transport)
				{
					return false;
				}

				if(settings.CanStartWithoutKey || (settings.CanStartUnassignedVehiclesWithoutKey && !AR_VehicleLock.HasKeyAssigned(transport)))
				{
					return true;
				}
				
				return AR_CanPlayerStartEngine(player, transport);
			}
		}
		return true;
	}	

	bool AR_CanPlayerStartEngine(PlayerBase player, Transport transport)
	{
		AR_EngineStartSettings settings = g_Game.GetARKeyConfig().GetEngineStartSettings();
		if(settings)
		{
			if(settings.CanStartWithKeyInHandOnly)
			{
				if(!player.GetItemInHands())
				{
					return false;
				}
				if(player.GetItemInHands().IsInherited(AR_MasterKey))
				{
					return true;
				}
				AR_CarKey_Base carKey = AR_CarKey_Base.Cast(player.GetItemInHands());
				if (transport && carKey)
				{
					if (AR_VehicleLock.HasKeyAssigned(transport) && carKey.m_ARKeyId == AR_VehicleLock.GetTransportKeyId(transport))
					{	
						return true;
					}
				}
				return false;
			}
			if(settings.CanStartWithKeyInInventory)
			{
				int currentTime = GetGame().GetTime();
				if(currentTime - m_LastInventoryCheck > m_LastInventoryCheckTimer)
				{
					m_LastInventoryCheck = currentTime;
					m_LastInventoryCheckReturnValue = false;
					if (AR_VehicleLock.HasKeyAssigned(transport))
					{
						m_LastInventoryCheckReturnValue = AR_KeyHelper.AR_PlayerHasCorrectKeyInInventory(player, AR_VehicleLock.GetTransportKeyId(transport));
					}
					
					return m_LastInventoryCheckReturnValue;
				}
				else
				{
					return m_LastInventoryCheckReturnValue;
				}
			}
		}
		return true;
	}
};
