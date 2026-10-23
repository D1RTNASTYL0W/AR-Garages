class AR_KeyHelper
{
	static bool AR_PlayerHasCorrectKeyInInventory(PlayerBase player, int keyId)
	{
		array<EntityAI> entities = new array<EntityAI>;
		
		player.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, entities);
		
		for (int i = 0; i < entities.Count(); i++)
		{
			EntityAI item = entities.Get(i);
			if (item)
			{
				if(item.IsInherited(AR_MasterKey))
				{
					return true;
				}
				AR_CarKey_Base carKey = AR_CarKey_Base.Cast(item);
				if (carKey)
				{
					if (carKey.m_ARKeyId == keyId)
					{	
						return true;
					}
				}
			}
		}
		return false;
	}
};