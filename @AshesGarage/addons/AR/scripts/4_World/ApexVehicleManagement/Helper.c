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
			if(!CheckBlacklistedNested(transport, item))
			{
				reason = "#avm_e_inventoryblacklist" + " " + item;
				return false;
			}
		}

		if (AR_VehicleLock.IsEngineOn(transport))
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


		if (!AR_VehicleLock.HasKeyAssigned(transport))
		{	
			reason = "#avm_e_keynotassigned";
			return false;
		}

		if (!PlayerHasCorrectKey(player, AR_VehicleLock.GetTransportKeyId(transport)))
		{	
			reason = "#avm_e_nokey";
			return false;
		}

		return true;
	}

	static void CollectInventoryKeys(PlayerBase player, array<int> keyIds, out bool hasMaster)
	{
		hasMaster = false;
		if (!player || !player.GetInventory() || !keyIds)
			return;

		array<EntityAI> entities = new array<EntityAI>;
		player.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, entities);
		for (int i = 0; i < entities.Count(); i++)
		{
			EntityAI item = entities.Get(i);
			if (!item)
				continue;
			if (item.IsInherited(AR_MasterKey))
			{
				hasMaster = true;
				continue;
			}
			AR_CarKey_Base carKey = AR_CarKey_Base.Cast(item);
			if (carKey && carKey.m_ARKeyId > 0 && keyIds.Find(carKey.m_ARKeyId) == -1)
				keyIds.Insert(carKey.m_ARKeyId);
		}
	}

	static bool PlayerHasMasterKey(PlayerBase player)
	{
		array<int> keyIds = new array<int>;
		bool hasMaster;
		CollectInventoryKeys(player, keyIds, hasMaster);
		return hasMaster;
	}

	static bool PlayerHasCorrectKey(PlayerBase player, int keyId)
	{
		return PlayerHasKeyId(player, keyId);
	}

	static bool PlayerHasKeyId(PlayerBase player, int keyId)
	{
		if (!player)
			return false;
		array<int> keyIds = new array<int>;
		bool hasMaster;
		CollectInventoryKeys(player, keyIds, hasMaster);
		if (hasMaster)
			return true;
		if (keyId <= 0)
			return false;
		return keyIds.Find(keyId) != -1;
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

	static bool CheckBlacklistedNested(EntityAI entity, out string reasonItem)
	{
		if (!entity || !entity.GetInventory())
			return true;

		CargoBase cargo = entity.GetInventory().GetCargo();
		if (cargo)
		{
			for (int cargoIndex = 0; cargoIndex < cargo.GetItemCount(); cargoIndex++)
			{
				EntityAI cargoItem = cargo.GetItem(cargoIndex);
				if (!cargoItem)
					continue;
				if (!MF_Helper.CheckBlacklistedItem(cargoItem, g_Game.GetApexVehicleManagementSettings().ItemsBlacklist, reasonItem))
					return false;
				if (!CheckBlacklistedNested(cargoItem, reasonItem))
					return false;
			}
		}

		int attCount = entity.GetInventory().AttachmentCount();
		for (int attIndex = 0; attIndex < attCount; attIndex++)
		{
			EntityAI att = entity.GetInventory().GetAttachmentFromIndex(attIndex);
			if (!att)
				continue;
			if (!MF_Helper.CheckBlacklistedItem(att, g_Game.GetApexVehicleManagementSettings().ItemsBlacklist, reasonItem))
				return false;
			if (!CheckBlacklistedNested(att, reasonItem))
				return false;
		}
		return true;
	}

	static bool IsGarageContainer(EntityAI item)
	{
		if (!item)
			return false;
		if (item.IsKindOf("Barrel_ColorBase"))
			return true;
		if (item.IsKindOf("WoodenCrate"))
			return true;
		if (item.IsKindOf("SeaChest"))
			return true;
		return false;
	}

	static bool IsGarageContainerType(string typeName)
	{
		if (typeName == "")
			return false;
		if (GetGame().IsKindOf(typeName, "Barrel_ColorBase"))
			return true;
		if (GetGame().IsKindOf(typeName, "WoodenCrate"))
			return true;
		if (GetGame().IsKindOf(typeName, "SeaChest"))
			return true;
		return false;
	}

	static void CollectEntityCargo(EntityAI entity, array<string> names, array<float> healths, string prefix)
	{
		if (!entity || !entity.GetInventory())
			return;

		CargoBase cargo = entity.GetInventory().GetCargo();
		if (cargo)
		{
			for (int cargoIndex = 0; cargoIndex < cargo.GetItemCount(); cargoIndex++)
			{
				EntityAI cargoItem = cargo.GetItem(cargoIndex);
				if (!cargoItem)
					continue;
				if (IsGarageContainer(cargoItem))
				{
					CollectEntityCargo(cargoItem, names, healths, prefix);
					continue;
				}
				if (names)
					names.Insert(prefix + cargoItem.GetType());
				if (healths)
					healths.Insert(cargoItem.GetHealth01("", ""));
				CollectEntityCargo(cargoItem, names, healths, prefix + "  + ");
			}
		}

		int attCount = entity.GetInventory().AttachmentCount();
		for (int attIndex = 0; attIndex < attCount; attIndex++)
		{
			EntityAI att = entity.GetInventory().GetAttachmentFromIndex(attIndex);
			if (!att)
				continue;
			CollectEntityCargo(att, names, healths, prefix);
		}
	}

};
