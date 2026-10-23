#ifndef SERVER
modded class MissionBase 
{
	override UIScriptedMenu CreateScriptedMenu(int id) 
	{
		if (id == MENU_AVM_GARAGEMENU) 
		{
			UIScriptedMenu menu = new AVM_GarageMenu;
			if (menu)
				menu.SetID(id);
			return menu;
		}
		return super.CreateScriptedMenu(id);
	}
};
#endif