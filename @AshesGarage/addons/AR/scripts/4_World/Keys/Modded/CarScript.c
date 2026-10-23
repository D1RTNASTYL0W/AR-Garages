modded class CarScript
{
	bool m_IsARLocked;
	bool m_HasARKeyAssigned;
	int m_CarKeyId;
	int m_CarScriptId;
	int m_LastInteractedWithUnixTime;	
	bool m_HadPlayerInteraction;
	string m_OriginalOwnerId;
	string m_OriginalOwnerName;
	private bool m_Initialised;//client only
	bool m_DeletionInitialisedByMod;

	private static const string	AR_LOCK_SOUND = "AR_LockVehicle_SoundSet";
	private static const string	AR_UNLOCK_SOUND = "AR_UnlockVehicle_SoundSet";
	bool ar_PlaySound_LockUnlock = false;

	void CarScript()
	{
		m_IsARLocked = false;
		m_CarKeyId = -1;
		m_CarScriptId = -1;
		m_LastInteractedWithUnixTime = -1;
		m_HadPlayerInteraction = false;
		m_DeletionInitialisedByMod = false;
		m_OriginalOwnerId =  "";
		m_OriginalOwnerName = "";
		m_Initialised = false;
		RegisterNetSyncVariableBool("m_IsARLocked");
		RegisterNetSyncVariableBoolSignal("ar_PlaySound_LockUnlock");
		RegisterNetSyncVariableInt("m_CarKeyId", int.MIN, int.MAX - 1);
		RegisterNetSyncVariableInt("m_CarScriptId", int.MIN, int.MAX - 1);
	}  


	bool AR_IsLocked()
	{
		return m_IsARLocked;
	}

	void ResetVehicle()
	{
		m_CarKeyId = -1;
		m_IsARLocked = false;
		m_HadPlayerInteraction = false;
		m_OriginalOwnerName = "";
		m_OriginalOwnerId =  "";
		SetLastInteractedWithTimeToNow();
		SetARKeyLifetime();
		SetSynchDirty();
	}

	void ResetLifetime()
	{
		SetLastInteractedWithTimeToNow();
		m_HadPlayerInteraction = true;
		SetARKeyLifetime();
	}

	void UnlockVehicle()
	{
		m_IsARLocked = false;
		SetSynchDirty();
		ResetLifetime();
	}

	bool AR_HasKeyAssigned()
	{
		return m_CarKeyId > -1;
	}

	void LockVehicle()
	{
		m_IsARLocked = true;
		SetSynchDirty();
		ResetLifetime();
	}
	
	void AR_SetHeadlights(bool headlightsOn)
	{
		m_HeadlightsOn = headlightsOn;
		SetSynchDirty();

		UpdateLights();
	}
	
	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();
		
		if (ar_PlaySound_LockUnlock)
		{		
			AR_VehicleLock.PlayLockUnlockSound_Client(this, AR_LOCK_SOUND, AR_UNLOCK_SOUND);
		}
	}
	override void OnEngineStart()
	{
		super.OnEngineStart();
		SetLastInteractedWithTimeToNow();
		SetARKeyLifetime();
	}

	override void EEKilled(Object killer)
	{
		if (GetGame().IsServer())
		{
			if(!m_EngineDestroyed)
			{
				AR_VehicleLock.EEKilledLog(this, killer);
			}
		}
		super.EEKilled(killer);
	}

	override void EEDelete(EntityAI parent)
	{
		super.EEDelete(parent);
		if (GetGame().IsServer())
		{
			if(m_DeletionInitialisedByMod)
			{
				return;
			}
			AR_VehicleLock.EEDeleteLog(this);
		}
	}

	override void DeferredInit()
	{
		super.DeferredInit();	
		DoARKeyInitialize();
	}

	void DoARKeyInitialize()
	{					
		if (!GetGame().IsServer())
			return;
		if(m_Initialised)
		{
			return;
		}			
		SetARKeyLifetime();
		if (m_CarScriptId < 0)
		{
			m_CarScriptId = g_Game.GetARKeyConfig().GetNextVehicleStoreID();
			SetLastInteractedWithTimeToNow();
		}
		int remainingTime = AR_VehicleLock.GetRemainingTimeTilDespawn(this, AR_VehicleLock.GetElapsedTimeSinceInteraction(m_LastInteractedWithUnixTime), m_HadPlayerInteraction);
		AR_LogKeyLine(GetDisplayName() + " (ID: " + m_CarScriptId + " - pos " + GetPosition() + ") initialized." + " Remaining lifetime: " + MF_Helper.SecondsToDays(remainingTime));
		SetSynchDirty();
		m_Initialised = true;
	}

	override bool NameOverride(out string output)
	{
		if(m_OriginalOwnerName != "")
		{
			string DisplayName;
			GetGame().ObjectGetDisplayName(this, DisplayName);	
			output = m_OriginalOwnerName + "'s " + DisplayName;
			return true;
		}
		return super.NameOverride(output);
	}

	override void OnStoreSave(ParamsWriteContext ctx)
	{
		super.OnStoreSave(ctx);

		Param4<bool, bool, int, int> ar_CarScriptData = new Param4<bool, bool, int, int>(m_IsARLocked, m_HasARKeyAssigned, m_CarKeyId, m_CarScriptId);
		ctx.Write(ar_CarScriptData);
		ctx.Write(m_LastInteractedWithUnixTime);
		ctx.Write(m_HadPlayerInteraction);
		ctx.Write(m_OriginalOwnerName);
		ctx.Write(m_OriginalOwnerId);
	}

	override bool OnStoreLoad(ParamsReadContext ctx, int version)
	{
		if (!super.OnStoreLoad(ctx, version))
			return false;

		Param4<bool, bool, int, int> ar_CarScriptData = new Param4<bool, bool, int, int>(false, false, 0, 0);
		if (ctx.Read(ar_CarScriptData))
		{
			m_IsARLocked = ar_CarScriptData.param1;
			m_HasARKeyAssigned = ar_CarScriptData.param2;
			m_CarKeyId = ar_CarScriptData.param3;
			m_CarScriptId = ar_CarScriptData.param4;
		}

		if (!ctx.Read(m_LastInteractedWithUnixTime))
			SetLastInteractedWithTimeToNow();
		if (!ctx.Read(m_HadPlayerInteraction))
			m_HadPlayerInteraction = false;
		if (!ctx.Read(m_OriginalOwnerName))
			m_OriginalOwnerName = "";
		if (!ctx.Read(m_OriginalOwnerId))
			m_OriginalOwnerId = "";
		
		SetSynchDirty();

		return true;
	}

	override void AfterStoreLoad()
	{
		super.AfterStoreLoad();
		DoARKeyInitialize();
	}

#ifdef UNIVERSALAPI
	override void OnUApiSave(UApiEntityStore data)
	{
		super.OnUApiSave(data);
		data.Write("m_IsARLocked", m_IsARLocked);
		data.Write("m_HasARKeyAssigned", m_HasARKeyAssigned);
		data.Write("m_CarKeyId", m_CarKeyId);
		data.Write("m_CarScriptId", m_CarScriptId);
		data.Write("m_LastInteractedWithUnixTime", m_LastInteractedWithUnixTime);
		data.Write("m_HadPlayerInteraction", m_HadPlayerInteraction);
		data.Write("m_OriginalOwnerName", m_OriginalOwnerName);
		data.Write("m_OriginalOwnerId", m_OriginalOwnerId);
	}

	override void OnUApiLoad(UApiEntityStore data)
	{
		super.OnUApiLoad(data);
		data.Read("m_IsARLocked", m_IsARLocked);
		data.Read("m_HasARKeyAssigned", m_HasARKeyAssigned);
		data.Read("m_CarKeyId", m_CarKeyId);
		data.Read("m_CarScriptId", m_CarScriptId);
		data.Read("m_LastInteractedWithUnixTime", m_LastInteractedWithUnixTime);
		data.Read("m_HadPlayerInteraction", m_HadPlayerInteraction);
		data.Read("m_OriginalOwnerName", m_OriginalOwnerName);
		data.Read("m_OriginalOwnerId", m_OriginalOwnerId);
		SetSynchDirty();
	}
#endif
	
	override void EEOnCECreate()
	{
		super.EEOnCECreate();
		DoARKeyInitialize();
	}

	override void OnCEUpdate()
	{
		super.OnCEUpdate();		
		if(!m_Initialised)
		{
			return;
		}	
		AR_VehicleLock.HandleLifetime(this, AR_VehicleLock.GetElapsedTimeSinceInteraction(m_LastInteractedWithUnixTime), m_HadPlayerInteraction);
	}

	void SetARKeyLifetime()
	{		
		if(g_Game.GetARKeyConfig().IsLifetimeMaintenanceEnabled())
		{			
			SetLifetime(g_Game.GetARKeyConfig().Get_MaxLifetime());
		}
	}

	int GetLastInteractedWithTime()
	{
		if(m_LastInteractedWithUnixTime <= 0)
		{
			SetLastInteractedWithTimeToNow();
		}
		return m_LastInteractedWithUnixTime;
	}

	void SetLastInteractedWithTimeToNow()
	{
		m_LastInteractedWithUnixTime = CF_Date.Now(false).GetTimestamp();
	}

	override bool CanDisplayAttachmentSlot(int slot_id)
	{
		if (!super.CanDisplayAttachmentSlot(slot_id))
			return false;

		return !AR_IsLocked();
	}

	override bool CanDisplayAttachmentCategory(string category_name)
	{
		if (!super.CanDisplayAttachmentCategory(category_name))
			return false;

		return !AR_IsLocked();
	}
	
	//Don't use this. Use SetSynchDirty(); directly
	void SynchronizeValues()
	{
		if (GetGame().IsServer())
			SetSynchDirty();
	}

	bool CheckOpenedDoors()
	{
		TStringArray attachmentNames = new TStringArray;
		ConfigGetTextArray("attachments", attachmentNames);
		foreach (string att : attachmentNames)
		{
			if (CarDoor.Cast(FindAttachmentBySlotName(att)))
			{
				int doorState = GetCarDoorsState(att);
				if (doorState == CarDoorState.DOORS_OPEN || doorState == CarDoorState.DOORS_MISSING)
					return true;
			}
			
		}

		return false;
	}

	bool HasDoors()
	{
		TStringArray attachmentNames = new TStringArray;
		ConfigGetTextArray("attachments", attachmentNames);
		foreach (string att : attachmentNames)
		{
			if (CarDoor.Cast(FindAttachmentBySlotName(att)))
				return true;
		}

		return false;
	}

	bool CanDoAction()
	{			
		if (HasDoors())
			return !AR_IsLocked() && !CheckOpenedDoors();
		else
			return !AR_IsLocked();

		return false;
	}

	bool ShouldShowInv()
	{
		if (HasDoors())
			return !AR_IsLocked() && CheckOpenedDoors();
		else
			return !AR_IsLocked();

		return false;
	}

	override bool IsInventoryVisible()
	{
		bool superIsVisible = super.IsInventoryVisible();
		if(!g_Game.GetARKeyConfig().Get_HideInventory())
		{
			if(!superIsVisible)
			{
				return false;
			}
			return !AR_IsLocked();
		}
		return ShouldShowInv() && superIsVisible;
	}

#ifdef DIAG_DEVELOPER
	override void SpawnUniversalParts()
	{
		GetInventory().CreateInInventory("HeadlightH7");
		GetInventory().CreateInInventory("HeadlightH7");

		if (IsVitalCarBattery())
		{
			GetInventory().CreateInInventory("CarBattery");
		}

		if (IsVitalTruckBattery())
		{
			GetInventory().CreateInInventory("TruckBattery");
		}

		if (IsVitalRadiator())
		{
			GetInventory().CreateInInventory("CarRadiator");
		}

		if (IsVitalSparkPlug())
		{
			GetInventory().CreateInInventory("SparkPlug");
		}

		if (IsVitalGlowPlug())
		{
			GetInventory().CreateInInventory("GlowPlug");
		}
	}

	override void SpawnAdditionalItems()
	{
		array<string> keys = {"AR_CarKey_Blue", "AR_CarKey_Green", "AR_CarKey_Red", "AR_CarKey_White", "AR_CarKey_Yellow", "AR_CarKey_Base"};
		GetInventory().CreateInInventory(keys.GetRandomElement());
	}
#endif
};