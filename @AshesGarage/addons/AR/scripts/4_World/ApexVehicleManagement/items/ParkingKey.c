class AVM_ParkingKey : ItemBase 
{
    override void SetActions()
	{
		super.SetActions();
        AddAction(ActionAVMParkWithKey);
	}
};