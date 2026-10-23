modded class ActionOpenCarDoors
{
    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        Transport transport = Transport.Cast(target.GetParent());
        if(transport)
        {
            if (AR_VehicleLock.IsLocked(transport))
                return false;
        }

        return super.ActionCondition(player, target, item);
    }
};