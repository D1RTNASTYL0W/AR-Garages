modded class ActionDetach
{
    override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
    {
		if (target)
		{
            Transport transport = Transport.Cast(target.GetParent());
            if(transport)
            {
                if (AR_VehicleLock.IsLocked(transport))
                    return false;
            }
		}

        return super.ActionCondition(player, target, item);
    }
};