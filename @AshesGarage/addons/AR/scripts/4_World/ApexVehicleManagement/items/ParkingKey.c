class AVM_ParkingKey : AR_MasterKey 
{
    override void SetActions()
	{
		super.SetActions();
        AddAction(ActionAVMParkWithKey);
	}
};