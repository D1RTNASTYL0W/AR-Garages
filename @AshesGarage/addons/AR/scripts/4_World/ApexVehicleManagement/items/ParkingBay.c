class AVM_ParkingBay_Kit : Msp_Kit 
{    
	override string Get_ItemName()
	{
		return "AVM_ParkingBay";
	}
};
class AVM_ParkingBay : Msp_Item 
{    
	override string Get_KitName()
	{
		return "AVM_ParkingBay_Kit";
	}

	override void SetActions()
	{
		super.SetActions();
		AddAction(ActionAVMGarageMenu);
	}
};

class AVM_ParkingMeter_Kit : AVM_ParkingBay_Kit 
{    
	override string Get_ItemName()
	{
		return "AVM_ParkingMeter";
	}
};
class AVM_ParkingMeter : AVM_ParkingBay 
{    
	override string Get_KitName()
	{
		return "AVM_ParkingMeter_Kit";
	}
};

class AVM_HND_ParkingBay : BuildingBase 
{
	override void SetActions()
	{
		super.SetActions();
		AddAction(ActionAVMGarageMenu);
	}
};

class AVM_HND_ParkingMeter : AVM_HND_ParkingBay {};