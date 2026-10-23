class AR_KeyPouch : Container_Base 
{    
	ref array<string> m_AllowedCargo = { "AR_CarKey_Base", "AR_CarKeyTag_Base" };

	override bool CanReceiveItemIntoCargo (EntityAI item)
	{
        if(!super.CanReceiveItemIntoCargo(item))
        {
            return false;
        }		
		
		return AR_IsAnyKindOfAllowed(item);
	}
	
	override bool CanSwapItemInCargo (EntityAI child_entity, EntityAI new_entity)
	{
        if(!super.CanSwapItemInCargo(child_entity,new_entity))
        {
            return false;
        }		

		return AR_IsAnyKindOfAllowed(child_entity) && AR_IsAnyKindOfAllowed(new_entity);		
	}

	override bool AreChildrenAccessible()
	{
		return true;
	}

    bool AR_IsAnyKindOfAllowed(EntityAI item)
    {
        foreach( string allowedCargo : m_AllowedCargo )
		{
			if(item.IsKindOf(allowedCargo))
			{
				return true;
			}
		}
        return false;
    }
};

class AR_KeyPouch_Admin : AR_KeyPouch 
{
    void AR_KeyPouch_Admin()
    {
        m_AllowedCargo = { "AR_CarKey_Base", "AR_CarKeyTag_Base", "AR_MasterKey",  "AR_ResetKey", "AR_InfoKey" };
    }
};