class AVM_Helper
{    
	static Transport GetNearbyTransport(vector position, float radius) 
	{
		array<Object> nearby_objects = new array<Object>;
		GetGame().GetObjectsAtPosition3D(position, radius, nearby_objects, null);
		for (int i = 0; i < nearby_objects.Count(); i++)
		{
			Transport transport = Transport.Cast(nearby_objects.Get(i));
			if(transport)
			{
				return transport;
			}
		}

		return null;
	}

	static bool CanPark(PlayerBase player, Transport transport, out string reason) 
	{
		if (!player)
		{
            reason = "Player is invalid?!";
			return false;
		}
		
		if (!transport)
		{
            reason = "#avm_e_novehicle";
			return false;
		}

		if(MF_Helper.IsAnyKindOf(transport, g_Game.GetApexVehicleManagementSettings().VehicleBlacklist))
		{
            reason = "This vehicle type is not allowed to be stored.";
			return false;
		}

		int i;
		if(!g_Game.GetApexVehicleManagementSettings().StoreCargo)
		{			
			bool hasCount = false;
			string itemReason = "";
			if(!CheckEntityInventoryCount(transport, hasCount, itemReason))
			{
				if(hasCount)
				{					
					reason = "#avm_e_inventoryfull";
					return false;
				}
				
				reason = "#avm_e_inventoryblacklist" + " " + itemReason;
				return false;
			}
		}
		else
		{
			string item = "";
			if(!MF_Helper.CheckItemInventory(transport, g_Game.GetApexVehicleManagementSettings().ItemsBlacklist, item))
			{				
				reason = "#avm_e_inventoryblacklist" + " " + item;
				return false;
			}
		}

		if (MCK_Transport.IsEngineOn(transport))
		{	
			reason = "#avm_e_engineon";
			return false;
		}

		if (transport.IsRuined())
		{	
			reason = "Ruined car";
			return false;
		}

		// Don't allow parking if the vehicle has crew
		for (i = 0; i < transport.CrewSize(); ++i)
		{
			if (transport.CrewMember(i))
			{	
				reason = "#avm_e_playerinside";
				return false;
			}
		}


		if (!MCK_Transport.HasMCKAssigned(transport))
		{	
			reason = "#avm_e_keynotassigned";
			return false;
		}
		
		if(player.GetItemInHands() && player.GetItemInHands().IsInherited(MCK_MasterKey))
		{
			return true;
		}

		//search inventory for key
		if (!MCK_Helper.MCK_PlayerHasCorrectKeyInInventory(player, MCK_Transport.GetTransportKeyId(transport)))
		{	
			reason = "#avm_e_nokey";
			return false;
		}

		// Don't allow parking if key is not in hand
		MCK_CarKey_Base carKey = MCK_CarKey_Base.Cast(player.GetItemInHands());
		if (carKey && carKey.m_MCKId != MCK_Transport.GetTransportKeyId(transport))
		{	
			reason = "#avm_e_keymismatch";
			return false;
		}

		return true;
	}

	static bool CheckEntityInventoryCount(EntityAI entity, out bool hasCount, out string reasonItem)
	{
		if(!entity.GetInventory())
		{	
			return true;
		}
		if(entity.GetInventory().GetCargo())
		{	
			if (entity.GetInventory().GetCargo().GetItemCount() > 0)
			{	
				hasCount = true;
				return false;
			}
		}

		int attCount = entity.GetInventory().AttachmentCount();
		for (int i = 0; i < attCount; ++i) 
		{
			EntityAI att = entity.GetInventory().GetAttachmentFromIndex(i);
			if (!att)
				continue;

			if(!MF_Helper.CheckBlacklistedItem(att, g_Game.GetApexVehicleManagementSettings().ItemsBlacklist, reasonItem))
			{
				hasCount = false;
				return false;
			}
			
			if(!CheckEntityInventoryCount(att, hasCount, reasonItem))
			{					
				return false;
			}			
		}	
		return true;	
	}

};
