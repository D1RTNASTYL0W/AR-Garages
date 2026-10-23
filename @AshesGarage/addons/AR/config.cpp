class CfgPatches
{
	class ApexVehicleManagement
	{
		requiredAddons[]=
		{
			"MuchFramework",
			"MuchCarKey",
            "UniversalApi",
			"DZ_Data",
			"DZ_Scripts"
		};
		units[]=
		{
			"AVM_ParkingBay_Kit",
			"AVM_ParkingBay",
			"AVM_ParkingMeter_Kit",
			"AVM_ParkingMeter",
			"AVM_ParkingKey"
		};
		weapons[]={};
	};
};
class CfgMods
{
	class ApexVehicleManagement
	{
		dir="AR";
		picture="";
		action="";
		hideName=1;
		hidePicture=1;
		name="AR";
		credits="Original source contributors: antihax and Helkhiana; modified project";
		author="Your Community (modified version)";
		authorID="";
		version="0.1.0";
		extra=0;
		type="mod";
		dependencies[]=
		{
			"GameLib",
			"Game",
			"World",
			"Mission"
		};
		class defs
		{
			class gameScriptModule
			{
				files[]=
				{
					"AR/scripts/3_Game"
				};
			};
			class worldScriptModule
			{
				files[]=
				{
					"AR/scripts/4_World"
				};
			};
			class missionScriptModule
			{
				files[]=
				{
					"AR/scripts/5_Mission"
				};
			};
		};
	};
};
class CfgVehicles
{
	class HouseNoDestruct;
	class Msp_Kit;
	class Msp_Item;
	class AVM_HND_ParkingBay: HouseNoDestruct
	{
		scope=1;
		displayName="#avm_parkingbay_name";
		model="AR\data\parkingbay\parkingbay.p3d";
	};
	class AVM_ParkingBay_Kit: Msp_Kit
	{
		scope=2;
		displayName="#avm_parkingbay_namekit";
		hiddenSelectionsTextures[]=
		{
			"AR\data\kitbox\kitbox_parkingbay_co.paa"
		};
		hiddenSelectionsMaterials[]=
		{
			"AR\data\kitbox\kitbox_parkingbay.rvmat"
		};
	};
	class AVM_ParkingBay: Msp_Item
	{
		scope=2;
		displayName="#avm_parkingbay_name";
		descriptionShort="#avm_parkingbay_desc";
		model="\AR\data\parkingbay\parkingbay.p3d";
		alignHologramToTerain=0;
	};
	class AVM_ParkingMeter_Kit: AVM_ParkingBay_Kit
	{
		scope=2;
		displayName="#avm_parkingmeter_namekit";
		hiddenSelectionsTextures[]=
		{
			"AR\data\kitbox\kitbox_parkingmeter_co.paa"
		};
	};
	class AVM_ParkingMeter: AVM_ParkingBay
	{
		scope=2;
		displayName="#avm_parkingmeter_name";
		model="\AR\data\parkingbay\parkingmeter.p3d";
		physLayer="item_small";
	};
	class AVM_HND_ParkingMeter: HouseNoDestruct
	{
		scope=1;
		displayName="#avm_parkingmeter_name";
		model="AR\data\parkingbay\parkingmeter.p3d";
		physLayer="item_small";
	};
	class MCK_MasterKey;
	class AVM_ParkingKey: MCK_MasterKey
	{
		scope=2;
		displayName="Parking Vehicle Key";
		descriptionShort="Only admins should have this. Can park any vehicle without a meter.";
		hiddenSelectionsTextures[]=
		{
			"MuchCarKey\data\carkey_co.paa",
			"MuchCarKey\data\carkey_co.paa"
		};
		hiddenSelectionsMaterials[]=
		{
			"MuchCarKey\data\carkey_tag.rvmat",
			"MuchCarKey\data\carkey_tag.rvmat"
		};
	};
};
