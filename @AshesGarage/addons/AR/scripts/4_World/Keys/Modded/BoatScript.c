modded class BoatScript
{
	bool ar_IsLocked = false;
	int ar_KeyId = -1;
	int ar_ObjectId = -1;
	int ar_LastInteractedUnixTime = -1;	
	bool ar_HadPlayerInteraction = false;
	string ar_OriginalOwnerId =  "";
	string ar_OriginalOwnerName = "";
	private bool ar_Initialised = false;//client only
	bool ar_DeletionByMod = false;

	private static const string	AR_LOCK_SOUND = "AR_LockVehicle_SoundSet";
	private static const string	AR_UNLOCK_SOUND = "AR_UnlockVehicle_SoundSet";
	bool ar_PlaySound_LockUnlock = false;

	void BoatScript()
	{
		RegisterNetSyncVariableBool("ar_IsLocked");
		RegisterNetSyncVariableBoolSignal("ar_PlaySound_LockUnlock");
		RegisterNetSyncVariableInt("ar_KeyId", -1, int.MAX - 1);
		RegisterNetSyncVariableInt("ar_ObjectId", -1, int.MAX - 1);
	} 

	bool AR_IsLocked()
	{
		return ar_IsLocked;
	}

	void AR_Reset()
	{
		ar_KeyId = -1;
		ar_IsLocked = false;
		ar_HadPlayerInteraction = false;
		ar_OriginalOwnerName = "";
		ar_OriginalOwnerId =  "";
		SetLastInteractedWithTimeToNow();
		AR_SetLifetime();
		SetSynchDirty();
	}

	void AR_ResetLifetime()
	{
		SetLastInteractedWithTimeToNow();
		ar_HadPlayerInteraction = true;
		AR_SetLifetime();
	}

	void AR_Unlock()
	{
		ar_IsLocked = false;
		SetSynchDirty();
		AR_ResetLifetime();
	}

	void AR_Lock()
	{
		ar_IsLocked = true;
		SetSynchDirty();
		AR_ResetLifetime();
	}

	bool AR_HasKeyAssigned()
	{
		return ar_KeyId > -1;
	}

	void AR_SetObjectID(int newID)
	{
		ar_ObjectId = newID;
		SetSynchDirty();
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
		AR_SetLifetime();
	}

	override void EEKilled(Object killer)
	{
		if (GetGame().IsServer())
		{			
			AR_VehicleLock.EEKilledLog(this, killer);
		}
		super.EEKilled(killer);
	}

	override void EEDelete(EntityAI parent)
	{
		super.EEDelete(parent);
		if (GetGame().IsServer())
		{
			if(ar_DeletionByMod)
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
		if(ar_Initialised)
		{
			return;
		}			
		AR_SetLifetime();
		if (ar_ObjectId == -1)
		{
			ar_ObjectId = g_Game.GetARKeyConfig().GetNextVehicleStoreID();			
			SetLastInteractedWithTimeToNow();
		}
		int remainingTime = AR_VehicleLock.GetRemainingTimeTilDespawn(this, AR_VehicleLock.GetElapsedTimeSinceInteraction(ar_LastInteractedUnixTime), ar_HadPlayerInteraction);
		AR_LogTransportActivity(this,  "initialized." + " Remaining lifetime: " + MF_Helper.SecondsToDays(remainingTime));
		SetSynchDirty();
		ar_Initialised = true;
	}

	override bool NameOverride(out string output)
	{
		if(ar_OriginalOwnerName != "")
		{
			string DisplayName;
			GetGame().ObjectGetDisplayName(this, DisplayName);	
			output = ar_OriginalOwnerName + "'s " + DisplayName;
			return true;
		}
		return super.NameOverride(output);
	}

	override void OnStoreSave(ParamsWriteContext ctx)
	{
		super.OnStoreSave(ctx);

		ctx.Write(ar_IsLocked);
		ctx.Write(ar_KeyId);
		ctx.Write(ar_ObjectId);

		ctx.Write(ar_LastInteractedUnixTime);
		ctx.Write(ar_HadPlayerInteraction);
		ctx.Write(ar_OriginalOwnerName);
		ctx.Write(ar_OriginalOwnerId);
	}

	override bool OnStoreLoad(ParamsReadContext ctx, int version)
	{
		if (!super.OnStoreLoad(ctx, version))
			return false;

		if (!ctx.Read(ar_IsLocked))
			ar_IsLocked = false;
		if (!ctx.Read(ar_KeyId))
			ar_KeyId = -1;
		if (!ctx.Read(ar_ObjectId))
			ar_ObjectId = -1;
		if (!ctx.Read(ar_LastInteractedUnixTime))
			SetLastInteractedWithTimeToNow();
		if (!ctx.Read(ar_HadPlayerInteraction))
			ar_HadPlayerInteraction = false;
		if (!ctx.Read(ar_OriginalOwnerName))
			ar_OriginalOwnerName = "";
		if (!ctx.Read(ar_OriginalOwnerId))
			ar_OriginalOwnerId = "";

		SetSynchDirty();

		return true;
	}

	override void AfterStoreLoad()
	{
		super.AfterStoreLoad();
		DoARKeyInitialize();
	}

	override void EEOnCECreate()
	{
		super.EEOnCECreate();
		DoARKeyInitialize();
	}

	override void OnCEUpdate()
	{
		super.OnCEUpdate();
		if(!ar_Initialised)
		{
			return;
		}	
		AR_VehicleLock.HandleLifetime(this, AR_VehicleLock.GetElapsedTimeSinceInteraction(ar_LastInteractedUnixTime), ar_HadPlayerInteraction);
	}

	void AR_SetLifetime()
	{		
		if(g_Game.GetARKeyConfig().IsLifetimeMaintenanceEnabled())
		{			
			SetLifetime(g_Game.GetARKeyConfig().Get_MaxLifetime());
		}
	}

	int GetLastInteractedWithTime()
	{
		if(ar_LastInteractedUnixTime <= 0)
		{
			SetLastInteractedWithTimeToNow();
		}
		return ar_LastInteractedUnixTime;
	}
	
	void SetLastInteractedWithTimeToNow()
	{
		ar_LastInteractedUnixTime = CF_Date.Now(false).GetTimestamp();
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

	override bool IsInventoryVisible()
	{
		if(!super.IsInventoryVisible())
		{
			return false;
		}
		return !AR_IsLocked();
	}
};