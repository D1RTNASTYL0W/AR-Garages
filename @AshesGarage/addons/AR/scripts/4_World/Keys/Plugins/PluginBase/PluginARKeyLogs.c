
class PluginARKeyLogs extends PluginBase
{
	bool			m_LogEnabled	= false;
	bool			m_DoFileLogs	= false;
	FileHandle		m_ARKeyLogFile;
	string			m_profileFolder = "$profile:AR";
	string			m_logFolder = "$profile:AR\\Logs";
	string			m_ARKeyLogName	= "ARKeyActivity";
	
	int year, month, day, hour, minute, second;
	string sYear, sMonth, sDay, sHour, sMinute, sSecond, currentDateTime, currentTime;

#ifdef SERVER
	override void OnInit()
	{
		if (GetGame().IsServer())
		{		
			Print("Init: PluginARKeyLogs");
			m_DoFileLogs = g_Game.GetARKeyConfig().DoFileLogs;
			if(m_DoFileLogs)
			{
				if (!FileExist(m_profileFolder))
					MakeDirectory(m_profileFolder);
				if (!FileExist(m_logFolder))
					MakeDirectory(m_logFolder);
				SetCurrentTime();
				string currentFileName = m_logFolder + "\\" + m_ARKeyLogName + currentDateTime + ".log";
			
				// Create New Log
				if (CreateNewLogFile(currentFileName))
				{				
					m_LogEnabled = true;
				}				
			}
		}
	}

	override void OnDestroy()
	{
		if (GetGame().IsServer())
		{
			// Close Log File
			Print("~PluginARKeyLogs Closed");
			if(m_DoFileLogs && m_LogEnabled)
			{
				CloseFile(m_ARKeyLogFile);
			}
		}
	}
	
	bool CreateNewLogFile(string logFilePath)
	{
		if (GetGame().IsServer())
		{
			SetCurrentTime();		
			m_ARKeyLogFile = OpenFile(logFilePath, FileMode.WRITE);
			if (m_ARKeyLogFile != 0)
			{
				FPrintln(m_ARKeyLogFile, "Log Created: " + currentDateTime);
				return true;
			}
			Print("Unable to create" + logFilePath + " file in Profile.");
			return false;
		}
		return false;
	}

	void SetCurrentTime()
	{
		// Setting Time Variables for new file name
		GetYearMonthDay(year, month, day);
		GetHourMinuteSecond(hour, minute, second);
		
		// Setting String for Time Variables for new file name
		sYear = year.ToString();
		
		sMonth = month.ToString();
		if (sMonth.Length() == 1)
		{
			sMonth = "0" + sMonth;
		}
		
		sDay = day.ToString();
		if (sDay.Length() == 1)
		{
			sDay = "0" + sDay;
		}
		
		sHour = hour.ToString();
		if (sHour.Length() == 1)
		{
			sHour = "0" + sHour;
		}
		
		sMinute = minute.ToString();
		if (sMinute.Length() == 1)
		{
			sMinute = "0" + sMinute;
		}
		
		sSecond = second.ToString();
		if (sSecond.Length() == 1)
		{
			sSecond = "0" + sSecond;
		}
		
		currentDateTime = "_" + sYear + "-" + sMonth + "-" + sDay + "_" + sHour + "-" + sMinute + "-" + sSecond;
		currentTime = sHour + ":" + sMinute + ":" + sSecond + " | ";
	}

#endif	
	void LogARKeyActivity(string text)
	{
		#ifdef SERVER
		if (GetGame().IsServer() && m_LogEnabled)
		{
			SetCurrentTime();			
			FPrintln(m_ARKeyLogFile, currentTime + text);
		}
		#endif	
	}
};

static void AR_LogKeyLine(string message)
{	
	#ifdef SERVER
	PluginARKeyLogs m_ARKeyLogger = PluginARKeyLogs.Cast(GetPlugin(PluginARKeyLogs));
	if(m_ARKeyLogger)
	{					
		m_ARKeyLogger.LogARKeyActivity(message);
	}
	#endif	
}


static void AR_LogTransportActivity(Transport transport, string message)
{	
	#ifdef SERVER
	PluginARKeyLogs m_ARKeyLogger = PluginARKeyLogs.Cast(GetPlugin(PluginARKeyLogs));
	if(m_ARKeyLogger)
	{
		string prefix = transport.GetDisplayName() + " (ID: " + AR_VehicleLock.GetTransportId(transport) + " - pos " + transport.GetPosition() + ") ";
		m_ARKeyLogger.LogARKeyActivity(prefix + message);
	}
	#endif	
}

static void AR_LogKeyActivity(PlayerBase player, string message)
{	
	#ifdef SERVER
	PluginARKeyLogs m_ARKeyLogger = PluginARKeyLogs.Cast(GetPlugin(PluginARKeyLogs));
	if(m_ARKeyLogger)
	{
		string prefix = "Player " + player.GetIdentity().GetName() + " (" + player.GetPosition() + " steam64id=" + player.GetIdentity().GetPlainId() + ") ";
		m_ARKeyLogger.LogARKeyActivity(prefix + message);
	}
	#endif	
}