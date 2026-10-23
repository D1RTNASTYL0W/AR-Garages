modded class PluginManager
{
	override void Init()
	{
		super.Init();
		#ifdef SERVER	
			RegisterPlugin( "PluginARKeyLogs", false, true);
		#endif
	}
};