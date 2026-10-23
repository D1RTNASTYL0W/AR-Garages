class CfgPatches
{
	class AshesGarage
	{
		requiredAddons[]=
		{
			"MuchFramework",
			"UniversalApi",
			"UAPIBase",
			"DZ_Data",
			"DZ_Scripts"
		};
		units[]=
		{
			"AVM_ParkingBay_Kit",
			"AVM_ParkingBay",
			"AVM_ParkingMeter_Kit",
			"AVM_ParkingMeter",
			"AVM_ParkingKey",
			"AR_KeyGrinder",
			"AR_CarKey_Base",
			"AR_CarKey_Blue",
			"AR_CarKey_Green",
			"AR_CarKey_Red",
			"AR_CarKey_White",
			"AR_CarKey_Yellow",
			"AR_CarKey_Purple",
			"AR_CarKey_Pink",
			"AR_CarKeyTag_Base",
			"AR_CarKeyTag_OlgaBlack",
			"AR_CarKeyTag_OlgaWhite",
			"AR_CarKeyTag_OlgaWine",
			"AR_CarKeyTag_AdaBlue",
			"AR_CarKeyTag_AdaWhite",
			"AR_CarKeyTag_AdaGreen",
			"AR_CarKeyTag_GunterBlue",
			"AR_CarKeyTag_GunterBlack",
			"AR_CarKeyTag_GunterRed",
			"AR_CarKeyTag_SarkaGrey",
			"AR_CarKeyTag_SarkaRed",
			"AR_CarKeyTag_SarkaYellow",
			"AR_CarKeyTag_Black",
			"AR_CarKeyTag_Blue",
			"AR_CarKeyTag_Green",
			"AR_CarKeyTag_Grey",
			"AR_CarKeyTag_Red",
			"AR_CarKeyTag_White",
			"AR_CarKeyTag_Yellow",
			"AR_CarKeyTag_Purple",
			"AR_CarKeyTag_Pink",
			"AR_MasterKey",
			"AR_ResetKey",
			"AR_InfoKey",
			"AR_KeyPouch",
			"AR_KeyPouch_Admin",
		};
		weapons[]={};
	};
	class ApexVehicleManagement: AshesGarage
	{
	};
};
class CfgMods
{
	class AshesGarage
	{
		dir="AshesGarage";
		picture="";
		action="";
		hideName=1;
		hidePicture=1;
		name="Ashes Reborn Garage";
		credits="Ashes Reborn";
		author="Ashes Reborn";
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
		defines[]=
		{
			"UNIVERSALAPI"
		};
		class defs
		{
			class gameScriptModule
			{
				files[]=
				{
					"AshesReborn/scripts/3_Game"
				};
			};
			class worldScriptModule
			{
				files[]=
				{
					"AshesReborn/scripts/4_World"
				};
			};
			class missionScriptModule
			{
				files[]=
				{
					"AshesReborn/scripts/5_Mission"
				};
			};
		};
	};
	class ApexVehicleManagement
	{
		dir="AshesGarage";
		name="Ashes Reborn Garage";
		hideName=1;
		hidePicture=1;
		type="mod";
	};
};
class CfgVehicles
{
	class HouseNoDestruct;
	class Msp_Kit;
	class Msp_Item;
	class Inventory_Base;
	class Container_Base;
	class AVM_HND_ParkingBay: HouseNoDestruct
	{
		scope=1;
		displayName="#avm_parkingbay_name";
		model="AshesReborn\data\parkingbay\parkingbay.p3d";
	};
	class AVM_ParkingBay_Kit: Msp_Kit
	{
		scope=2;
		displayName="#avm_parkingbay_namekit";
		hiddenSelectionsTextures[]=
		{
			"AshesReborn\data\kitbox\kitbox_parkingbay_co.paa"
		};
		hiddenSelectionsMaterials[]=
		{
			"AshesReborn\data\kitbox\kitbox_parkingbay.rvmat"
		};
	};
	class AVM_ParkingBay: Msp_Item
	{
		scope=2;
		displayName="#avm_parkingbay_name";
		descriptionShort="#avm_parkingbay_desc";
		model="\AshesReborn\data\parkingbay\parkingbay.p3d";
		alignHologramToTerain=0;
	};
	class AVM_ParkingMeter_Kit: AVM_ParkingBay_Kit
	{
		scope=2;
		displayName="#avm_parkingmeter_namekit";
		hiddenSelectionsTextures[]=
		{
			"AshesReborn\data\kitbox\kitbox_parkingmeter_co.paa"
		};
	};
	class AVM_ParkingMeter: AVM_ParkingBay
	{
		scope=2;
		displayName="#avm_parkingmeter_name";
		model="\AshesReborn\data\parkingbay\parkingmeter.p3d";
		physLayer="item_small";
	};
	class AVM_HND_ParkingMeter: HouseNoDestruct
	{
		scope=1;
		displayName="#avm_parkingmeter_name";
		model="AshesReborn\data\parkingbay\parkingmeter.p3d";
		physLayer="item_small";
	};
	class AR_KeyGrinder: HouseNoDestruct
	{
		scope=1;
		model="\DZ\structures\furniture\various\grinder.p3d";
	};
	class AR_CarKey_Base: Inventory_Base
	{
		scope=2;
		displayName="#ar_key_name_carkey";
		descriptionShort="#ar_key_description_carkey";
		model="\AshesReborn\key\carkey.p3d";
		rotationFlags=8;
		weight=100;
		itemSize[]={1,2};
		fragility=0.0099999998;
		attachments[]=
		{
			"CarKeyTag"
		};
		hiddenSelections[]=
		{
			"metal",
			"key_plastic"
		};
		hiddenSelectionsTextures[]=
		{
			"AshesReborn\key\carkey_co.paa",
			"AshesReborn\key\carkey_co.paa"
		};
		hiddenSelectionsMaterials[]=
		{
			"AshesReborn\key\carkey_metal.rvmat",
			"AshesReborn\key\carkey_tag.rvmat"
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=100;
					healthLevels[]=
					{
						
						{
							1,
							
							{
								"AshesReborn\key\carkey_metal.rvmat",
								"AshesReborn\key\carkey_tag.rvmat"
							}
						},
						
						{
							0.69999999,
							
							{
								"AshesReborn\key\carkey_metal.rvmat",
								"AshesReborn\key\carkey_tag.rvmat"
							}
						},
						
						{
							0.5,
							
							{
								"AshesReborn\key\carkey_damage.rvmat",
								"AshesReborn\key\carkey_damage.rvmat"
							}
						},
						
						{
							0.30000001,
							
							{
								"AshesReborn\key\carkey_damage.rvmat",
								"AshesReborn\key\carkey_damage.rvmat"
							}
						},
						
						{
							0,
							
							{
								"AshesReborn\key\carkey_destruct.rvmat",
								"AshesReborn\key\carkey_destruct.rvmat"
							}
						}
					};
				};
			};
		};
	};
	class AR_CarKey_Blue: AR_CarKey_Base
	{
		scope=2;
		displayName="Blue #ar_key_name_carkey";
		hiddenSelectionsTextures[]=
		{
			"AshesReborn\key\carkey_co.paa",
			"AshesReborn\key\key_variants\key_blue_co.paa"
		};
	};
	class AR_CarKey_Green: AR_CarKey_Base
	{
		scope=2;
		displayName="Green #ar_key_name_carkey";
		hiddenSelectionsTextures[]=
		{
			"AshesReborn\key\carkey_co.paa",
			"AshesReborn\key\key_variants\key_green_co.paa"
		};
	};
	class AR_CarKey_Red: AR_CarKey_Base
	{
		scope=2;
		displayName="Red #ar_key_name_carkey";
		hiddenSelectionsTextures[]=
		{
			"AshesReborn\key\carkey_co.paa",
			"AshesReborn\key\key_variants\key_red_co.paa"
		};
	};
	class AR_CarKey_White: AR_CarKey_Base
	{
		scope=2;
		displayName="White #ar_key_name_carkey";
		hiddenSelectionsTextures[]=
		{
			"AshesReborn\key\carkey_co.paa",
			"AshesReborn\key\key_variants\key_white_co.paa"
		};
	};
	class AR_CarKey_Yellow: AR_CarKey_Base
	{
		scope=2;
		displayName="Yellow #ar_key_name_carkey";
		hiddenSelectionsTextures[]=
		{
			"AshesReborn\key\carkey_co.paa",
			"AshesReborn\key\key_variants\key_yellow_co.paa"
		};
	};
	class AR_CarKey_Purple: AR_CarKey_Base
	{
		scope=2;
		displayName="Purple #ar_key_name_carkey";
		hiddenSelectionsTextures[]=
		{
			"AshesReborn\key\carkey_co.paa",
			"AshesReborn\key\key_variants\key_purple_co.paa"
		};
	};
	class AR_CarKey_Pink: AR_CarKey_Base
	{
		scope=2;
		displayName="Pink #ar_key_name_carkey";
		hiddenSelectionsTextures[]=
		{
			"AshesReborn\key\carkey_co.paa",
			"AshesReborn\key\key_variants\key_pink_co.paa"
		};
	};
	class AR_CarKeyTag_Base: Inventory_Base
	{
		scope=2;
		displayName="#ar_key_name_carkeytag";
		descriptionShort="#ar_key_description_carkeytag";
		model="\AshesReborn\key\carkey_tag.p3d";
		rotationFlags=17;
		weight=100;
		itemSize[]={1,2};
		inventorySlot[]=
		{
			"CarKeyTag"
		};
		fragility=0.0099999998;
		hiddenSelections[]=
		{
			"leather_tag"
		};
		hiddenSelectionsTextures[]=
		{
			"AshesReborn\key\carkey_co.paa"
		};
		hiddenSelectionsMaterials[]=
		{
			"AshesReborn\key\carkey_tag.rvmat"
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=100;
					healthLevels[]=
					{
						
						{
							1,
							
							{
								"AshesReborn\key\carkey_tag.rvmat"
							}
						},
						
						{
							0.69999999,
							
							{
								"AshesReborn\key\carkey_tag.rvmat"
							}
						},
						
						{
							0.5,
							
							{
								"AshesReborn\key\carkey_damage.rvmat"
							}
						},
						
						{
							0.30000001,
							
							{
								"AshesReborn\key\carkey_damage.rvmat"
							}
						},
						
						{
							0,
							
							{
								"AshesReborn\key\carkey_destruct.rvmat"
							}
						}
					};
				};
			};
		};
	};
	class AR_CarKeyTag_OlgaBlack: AR_CarKeyTag_Base
	{
		scope=2;
		displayName="Olga 24 Black Tag";
		hiddenSelectionsTextures[]=
		{
			"AshesReborn\key\tag_variants\tag_olgablack_co.paa"
		};
	};
	class AR_CarKeyTag_OlgaWhite: AR_CarKeyTag_Base
	{
		scope=2;
		displayName="Olga 24 White Tag";
		hiddenSelectionsTextures[]=
		{
			"AshesReborn\key\tag_variants\tag_olgawhite_co.paa"
		};
	};
	class AR_CarKeyTag_OlgaWine: AR_CarKeyTag_Base
	{
		scope=2;
		displayName="Olga 24 Wine Tag";
		hiddenSelectionsTextures[]=
		{
			"AshesReborn\key\tag_variants\tag_olgawine_co.paa"
		};
	};
	class AR_CarKeyTag_AdaBlue: AR_CarKeyTag_Base
	{
		scope=2;
		displayName="Ada 4x4 Blue Tag";
		hiddenSelectionsTextures[]=
		{
			"AshesReborn\key\tag_variants\tag_adablue_co.paa"
		};
	};
	class AR_CarKeyTag_AdaWhite: AR_CarKeyTag_Base
	{
		scope=2;
		displayName="Ada 4x4 White Tag";
		hiddenSelectionsTextures[]=
		{
			"AshesReborn\key\tag_variants\tag_adawhite_co.paa"
		};
	};
	class AR_CarKeyTag_AdaGreen: AR_CarKeyTag_Base
	{
		scope=2;
		displayName="Ada 4x4 Green Tag";
		hiddenSelectionsTextures[]=
		{
			"AshesReborn\key\tag_variants\tag_adagreen_co.paa"
		};
	};
	class AR_CarKeyTag_GunterBlue: AR_CarKeyTag_Base
	{
		scope=2;
		displayName="Gunter 2 Blue Tag";
		hiddenSelectionsTextures[]=
		{
			"AshesReborn\key\tag_variants\tag_gunterblue_co.paa"
		};
	};
	class AR_CarKeyTag_GunterBlack: AR_CarKeyTag_Base
	{
		scope=2;
		displayName="Gunter 2 Black Tag";
		hiddenSelectionsTextures[]=
		{
			"AshesReborn\key\tag_variants\tag_gunterblack_co.paa"
		};
	};
	class AR_CarKeyTag_GunterRed: AR_CarKeyTag_Base
	{
		scope=2;
		displayName="Gunter 2 Red Tag";
		hiddenSelectionsTextures[]=
		{
			"AshesReborn\key\tag_variants\tag_gunterred_co.paa"
		};
	};
	class AR_CarKeyTag_SarkaGrey: AR_CarKeyTag_Base
	{
		scope=2;
		displayName="Sarka 120 Grey Tag";
		hiddenSelectionsTextures[]=
		{
			"AshesReborn\key\tag_variants\tag_sarkagrey_co.paa"
		};
	};
	class AR_CarKeyTag_SarkaRed: AR_CarKeyTag_Base
	{
		scope=2;
		displayName="Sarka 120 Red Tag";
		hiddenSelectionsTextures[]=
		{
			"AshesReborn\key\tag_variants\tag_sarkared_co.paa"
		};
	};
	class AR_CarKeyTag_SarkaYellow: AR_CarKeyTag_Base
	{
		scope=2;
		displayName="Sarka 120 Yellow Tag";
		hiddenSelectionsTextures[]=
		{
			"AshesReborn\key\tag_variants\tag_sarkayellow_co.paa"
		};
	};
	class AR_CarKeyTag_Black: AR_CarKeyTag_Base
	{
		scope=2;
		displayName="Black Tag";
		hiddenSelectionsTextures[]=
		{
			"AshesReborn\key\tag_variants\tag_black_co.paa"
		};
	};
	class AR_CarKeyTag_Blue: AR_CarKeyTag_Base
	{
		scope=2;
		displayName="Blue Tag";
		hiddenSelectionsTextures[]=
		{
			"AshesReborn\key\tag_variants\tag_blue_co.paa"
		};
	};
	class AR_CarKeyTag_Green: AR_CarKeyTag_Base
	{
		scope=2;
		displayName="Green Tag";
		hiddenSelectionsTextures[]=
		{
			"AshesReborn\key\tag_variants\tag_green_co.paa"
		};
	};
	class AR_CarKeyTag_Grey: AR_CarKeyTag_Base
	{
		scope=2;
		displayName="Grey Tag";
		hiddenSelectionsTextures[]=
		{
			"AshesReborn\key\tag_variants\tag_grey_co.paa"
		};
	};
	class AR_CarKeyTag_Red: AR_CarKeyTag_Base
	{
		scope=2;
		displayName="Red Tag";
		hiddenSelectionsTextures[]=
		{
			"AshesReborn\key\tag_variants\tag_red_co.paa"
		};
	};
	class AR_CarKeyTag_White: AR_CarKeyTag_Base
	{
		scope=2;
		displayName="White Tag";
		hiddenSelectionsTextures[]=
		{
			"AshesReborn\key\tag_variants\tag_white_co.paa"
		};
	};
	class AR_CarKeyTag_Yellow: AR_CarKeyTag_Base
	{
		scope=2;
		displayName="Yellow Tag";
		hiddenSelectionsTextures[]=
		{
			"AshesReborn\key\tag_variants\tag_yellow_co.paa"
		};
	};
	class AR_CarKeyTag_Purple: AR_CarKeyTag_Base
	{
		scope=2;
		displayName="Purple Tag";
		hiddenSelectionsTextures[]=
		{
			"AshesReborn\key\key_variants\key_purple_co.paa"
		};
	};
	class AR_CarKeyTag_Pink: AR_CarKeyTag_Base
	{
		scope=2;
		displayName="Pink Tag";
		hiddenSelectionsTextures[]=
		{
			"AshesReborn\key\key_variants\key_pink_co.paa"
		};
	};
	class AR_MasterKey: Inventory_Base
	{
		scope=2;
		displayName="#ar_key_name_masterkey";
		descriptionShort="#ar_key_description_masterkey";
		model="\AshesReborn\key\fullcarkey.p3d";
		rotationFlags=17;
		weight=100;
		itemSize[]={1,2};
		fragility=0.0099999998;
		hiddenSelections[]=
		{
			"key_plastic",
			"leather_tag"
		};
		hiddenSelectionsTextures[]=
		{
			"AshesReborn\key\carkey_masterkey_co.paa",
			"AshesReborn\key\carkey_masterkey_co.paa"
		};
		hiddenSelectionsMaterials[]=
		{
			"AshesReborn\key\carkey_masterkey_tag.rvmat",
			"AshesReborn\key\carkey_masterkey_tag.rvmat"
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=1000000;
				};
			};
		};
	};
	class AR_ResetKey: AR_MasterKey
	{
		scope=2;
		displayName="#ar_key_name_resetkey";
		descriptionShort="#ar_key_description_resetkey";
		hiddenSelectionsTextures[]=
		{
			"AshesReborn\key\carkey_resetkey_co.paa",
			"AshesReborn\key\carkey_resetkey_co.paa"
		};
		hiddenSelectionsMaterials[]=
		{
			"AshesReborn\key\carkey_resetkey_tag.rvmat",
			"AshesReborn\key\carkey_resetkey_tag.rvmat"
		};
	};
	class AR_InfoKey: AR_MasterKey
	{
		scope=2;
		displayName="#ar_key_name_infokey";
		descriptionShort="#ar_key_description_infokey";
		hiddenSelectionsTextures[]=
		{
			"AshesReborn\key\carkey_infokey_co.paa",
			"AshesReborn\key\carkey_infokey_co.paa"
		};
		hiddenSelectionsMaterials[]=
		{
			"AshesReborn\key\carkey_tag.rvmat",
			"AshesReborn\key\carkey_tag.rvmat"
		};
	};
	class AR_KeyPouch: Container_Base
	{
		scope=2;
		displayName="#ar_key_name_keypouch";
		descriptionShort="#ar_key_description_keypouch";
		model="\AshesReborn\key\key_pouch\keypouch.p3d";
		rotationFlags=1;
		weight=130;
		itemSize[]={3,2};
		itemsCargoSize[]={4,3};
		canBeDigged=0;
		allowOwnedCargoManipulation=1;
		hiddenSelections[]=
		{
			"all"
		};
		hiddenSelectionsTextures[]=
		{
			"AshesReborn\key\key_pouch\keypouch_co.paa"
		};
		hiddenSelectionsMaterials[]=
		{
			"AshesReborn\key\key_pouch\keypouch.rvmat"
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=100;
					healthLevels[]=
					{
						
						{
							1,
							
							{
								"AshesReborn\key\key_pouch\keypouch.rvmat"
							}
						},
						
						{
							0.69999999,
							
							{
								"AshesReborn\key\key_pouch\keypouch.rvmat"
							}
						},
						
						{
							0.5,
							
							{
								"AshesReborn\key\key_pouch\keypouch_damage.rvmat"
							}
						},
						
						{
							0.30000001,
							
							{
								"AshesReborn\key\key_pouch\keypouch_damage.rvmat"
							}
						},
						
						{
							0,
							
							{
								"AshesReborn\key\key_pouch\keypouch_destruct.rvmat"
							}
						}
					};
				};
			};
		};
	};
	class AR_KeyPouch_Admin: AR_KeyPouch
	{
		scope=2;
		displayName="ADMIN #ar_key_name_keypouch";
		descriptionShort="#ar_key_description_keypouch This pouch is meant for admins to store their admin keys.";
		hiddenSelectionsTextures[]=
		{
			"AshesReborn\key\key_pouch\keypouch_red_co.paa"
		};
	};
	class AVM_ParkingKey: AR_MasterKey
	{
		scope=2;
		displayName="Parking Vehicle Key";
		descriptionShort="Only admins should have this. Can park any vehicle without a meter.";
		hiddenSelectionsTextures[]=
		{
			"AshesReborn\key\carkey_co.paa",
			"AshesReborn\key\carkey_co.paa"
		};
		hiddenSelectionsMaterials[]=
		{
			"AshesReborn\key\carkey_tag.rvmat",
			"AshesReborn\key\carkey_tag.rvmat"
		};
	};
};
class CfgSlots
{
	class Slot_CarKeytag
	{
		name="CarKeyTag";
		displayName="Car Key Tag";
		ghostIcon="missing";
	};
};
class CfgNonAIVehicles
{
	class ProxyAttachment;
	class Proxycarkey_tag: ProxyAttachment
	{
		scope=2;
		inventorySlot[]=
		{
			"CarKeyTag"
		};
		model="\AshesReborn\key\carkey_tag.p3d";
	};
};
class CfgSoundShaders
{
	class AR_base_SoundShaderFlies
	{
		range=20;
		rangeCurve="defaultLFECurve";
		volume=3;
	};
	class AR_UnlockVehicle_SoundShader: AR_base_SoundShaderFlies
	{
		samples[]=
		{
			
			{
				"\AshesReborn\key\sounds\unlock",
				1
			}
		};
	};
	class AR_LockVehicle_SoundShader: AR_base_SoundShaderFlies
	{
		samples[]=
		{
			
			{
				"\AshesReborn\key\sounds\lock",
				1
			}
		};
	};

};
class CfgSoundSets
{
	class AR_base_SoundSet
	{
		sound3DProcessingType="defaultLFECurve";
		spatial=1;
		doppler=0;
		loop=0;
		volume=30;
	};
	class AR_UnlockVehicle_SoundSet: AR_base_SoundSet
	{
		soundShaders[]=
		{
			"AR_UnlockVehicle_SoundShader"
		};
	};
	class AR_LockVehicle_SoundSet: AR_base_SoundSet
	{
		soundShaders[]=
		{
			"AR_LockVehicle_SoundShader"
		};
	};

};
