class AR_VehicleLock
{
    static void AssignNewKey(Transport transport, int ar_key_id, PlayerBase player)
    {
        CarScript carScript = CarScript.Cast(transport);
		if(carScript)
		{
            carScript.m_CarKeyId = ar_key_id;
            carScript.m_HasARKeyAssigned = true;
            carScript.m_OriginalOwnerName = player.GetIdentity().GetName();
            carScript.m_OriginalOwnerId = player.GetIdentity().GetPlainId();
            carScript.ResetLifetime();
            carScript.SetSynchDirty(); 
            return;
		}

		BoatScript boatScript = BoatScript.Cast(transport);
		if(boatScript)
		{
            boatScript.ar_KeyId = ar_key_id;
            boatScript.ar_OriginalOwnerName = player.GetIdentity().GetName();
            boatScript.ar_OriginalOwnerId = player.GetIdentity().GetPlainId();
            boatScript.AR_ResetLifetime();
            boatScript.SetSynchDirty(); 
            return;
		}
    }

    
    static void AssignNewData(Transport transport, int carScriptId, int carKeyId, string originalOwnerId, string originalOwnerName)
    {
        CarScript carScript = CarScript.Cast(transport);
		if(carScript)
		{
            carScript.m_CarKeyId = carKeyId;
            carScript.m_CarScriptId = carScriptId;
            carScript.SetLastInteractedWithTimeToNow();
            carScript.m_OriginalOwnerId = originalOwnerId;
            carScript.m_OriginalOwnerName = originalOwnerName; 
            carScript.m_HadPlayerInteraction = true;
            carScript.m_IsARLocked = false;
            carScript.m_HasARKeyAssigned = true;
            carScript.ResetLifetime();
            return;
		}

		BoatScript boatScript = BoatScript.Cast(transport);
		if(boatScript)
		{
            boatScript.ar_KeyId = carKeyId;
            boatScript.ar_ObjectId = carScriptId;
            boatScript.SetLastInteractedWithTimeToNow();
            boatScript.ar_OriginalOwnerId = originalOwnerId;
            boatScript.ar_OriginalOwnerName = originalOwnerName; 
            boatScript.ar_HadPlayerInteraction = true;
            boatScript.ar_IsLocked = false;
            boatScript.AR_ResetLifetime();
            return;
		}
    }

    static bool IsEngineOn(Transport transport)
    {
        CarScript carScript = CarScript.Cast(transport);
		if(carScript)
		{
            return carScript.EngineIsOn();
		}

		BoatScript boatScript = BoatScript.Cast(transport);
		if(boatScript)
		{
            return boatScript.EngineIsOn();
		}
        return false;
    }

    static void GetFluidData(Transport transport, int fluid, out float fluidFraction, out float fluidCapacity)
    {
        CarScript carScript = CarScript.Cast(transport);
		if(carScript)
		{
            fluidFraction = carScript.GetFluidFraction(fluid);
            fluidCapacity = carScript.GetFluidCapacity(fluid);
            return;
		}

		BoatScript boatScript = BoatScript.Cast(transport);
		if(boatScript)
		{
            fluidFraction = boatScript.GetFluidFraction(fluid);
            fluidCapacity = boatScript.GetFluidCapacity(fluid);
            return;
		}
    }

    static void FillFluid(Transport transport, int fluid, float amount, bool maxFluid = false)
    {
        CarScript carScript = CarScript.Cast(transport);
		if(carScript)
		{
            if(maxFluid)
            {
                carScript.Fill(fluid, carScript.GetFluidCapacity(fluid));
            }
            else
            {
                carScript.Fill(fluid, amount);
            }
            return;
		}

		BoatScript boatScript = BoatScript.Cast(transport);
		if(boatScript)
		{
            if(maxFluid)
            {
                boatScript.Fill(fluid, boatScript.GetFluidCapacity(fluid));
            }
            else
            {
                boatScript.Fill(fluid, amount);
            }
            return;
		}
    }

    static bool HasKeyAssigned(Transport transport)
    {
        CarScript carScript = CarScript.Cast(transport);
		if(carScript)
		{
            return carScript.AR_HasKeyAssigned();
		}

		BoatScript boatScript = BoatScript.Cast(transport);
		if(boatScript)
		{
            return boatScript.AR_HasKeyAssigned();
		}
        return false;
    }
    
    static bool IsLocked(Transport transport)
    {
        CarScript carScript = CarScript.Cast(transport);
		if(carScript)
		{
            return carScript.m_IsARLocked;
		}

		BoatScript boatScript = BoatScript.Cast(transport);
		if(boatScript)
		{
            return boatScript.ar_IsLocked;
		}
        return false;
    }
    
    static int GetTransportKeyId(Transport transport)
    {
        CarScript carScript = CarScript.Cast(transport);
		if(carScript)
		{
            return carScript.m_CarKeyId;
		}

		BoatScript boatScript = BoatScript.Cast(transport);
		if(boatScript)
		{
            return boatScript.ar_KeyId;
		}
        return -1;
    }
    
    static int GetTransportId(Transport transport)
    {
        CarScript carScript = CarScript.Cast(transport);
		if(carScript)
		{
            return carScript.m_CarScriptId;
		}

		BoatScript boatScript = BoatScript.Cast(transport);
		if(boatScript)
		{
            return boatScript.ar_ObjectId;
		}
        return -1;
    }
    
    static string GetOriginalOwnerName(Transport transport)
    {
        CarScript carScript = CarScript.Cast(transport);
		if(carScript)
		{
            return carScript.m_OriginalOwnerName;
		}

		BoatScript boatScript = BoatScript.Cast(transport);
		if(boatScript)
		{
            return boatScript.ar_OriginalOwnerName;
		}
        return "";
    }
    
    static string GetOriginalOwnerId(Transport transport)
    {
        CarScript carScript = CarScript.Cast(transport);
		if(carScript)
		{
            return carScript.m_OriginalOwnerId;
		}

		BoatScript boatScript = BoatScript.Cast(transport);
		if(boatScript)
		{
            return boatScript.ar_OriginalOwnerId;
		}
        return "";
    }
    
    static int GetLastInteractedWithTime(Transport transport)
    {
        CarScript carScript = CarScript.Cast(transport);
		if(carScript)
		{
            return carScript.GetLastInteractedWithTime();
		}

		BoatScript boatScript = BoatScript.Cast(transport);
		if(boatScript)
		{
            return boatScript.GetLastInteractedWithTime();
		}
        return -1;
    }
    
    static int GetRemainingTimeTilDespawnTransport(Transport transport)
    {
        int elapsedTime = -1;
        CarScript carScript = CarScript.Cast(transport);
		if(carScript)
		{
            elapsedTime = AR_VehicleLock.GetElapsedTimeSinceInteraction(carScript.m_LastInteractedWithUnixTime);
            return AR_VehicleLock.GetRemainingTimeTilDespawn(carScript, elapsedTime, carScript.m_HadPlayerInteraction);
		}

		BoatScript boatScript = BoatScript.Cast(transport);
		if(boatScript)
		{
            elapsedTime = AR_VehicleLock.GetElapsedTimeSinceInteraction(boatScript.ar_LastInteractedUnixTime);
            return AR_VehicleLock.GetRemainingTimeTilDespawn(boatScript, elapsedTime, boatScript.ar_HadPlayerInteraction);
		}
        return -1;
    }
    
    static void SetDeletionByMod(Transport transport)
    {
        CarScript carScript = CarScript.Cast(transport);
		if(carScript)
		{
            carScript.m_DeletionInitialisedByMod = true;
            return;
		}

		BoatScript boatScript = BoatScript.Cast(transport);
		if(boatScript)
		{
            boatScript.ar_DeletionByMod = true;
            return;
		}
    }

    static void UnlockTransport(Transport transport)
    {
        CarScript carScript = CarScript.Cast(transport);
		if(carScript)
		{
            carScript.UnlockVehicle();
            return;
		}

		BoatScript boatScript = BoatScript.Cast(transport);
		if(boatScript)
		{
            boatScript.AR_Unlock();
            return;
		}
    }
    
    static void LockTransport(Transport transport)
    {
        CarScript carScript = CarScript.Cast(transport);
		if(carScript)
		{
            carScript.LockVehicle();
            return;
		}

		BoatScript boatScript = BoatScript.Cast(transport);
		if(boatScript)
		{
            boatScript.AR_Lock();
            return;
		}
    }

    static void ToggleLockTransport(Transport transport)
    {
        if(AR_VehicleLock.IsLocked(transport))
        {
            AR_VehicleLock.UnlockTransport(transport);
        }
        else
        {
            AR_VehicleLock.LockTransport(transport);
        }
    }
    
    static bool CanDoAction(Transport transport)
    {
        CarScript carScript = CarScript.Cast(transport);
		if(carScript)
		{
            return carScript.CanDoAction();
		}

		BoatScript boatScript = BoatScript.Cast(transport);
		if(boatScript)
		{ 
            return g_Game.GetARKeyConfig().CanLockBoats;
		}
        return false;
    }

    static void ResetTransport(Transport transport)
    {
        CarScript carScript = CarScript.Cast(transport);
		if(carScript)
		{
            carScript.ResetVehicle();
            return;
		}

		BoatScript boatScript = BoatScript.Cast(transport);
		if(boatScript)
		{
            boatScript.AR_Reset();
            return;
		}
    }
	
	static int GetElapsedTimeSinceInteractionFromTransport(Transport transport)
	{
		return CF_Date.Now(false).GetTimestamp() - AR_VehicleLock.GetLastInteractedWithTime(transport);
	}

	static int GetElapsedTimeSinceInteraction(int lastInteractedWithTime)
	{
		return CF_Date.Now(false).GetTimestamp() - lastInteractedWithTime;
	}

    static void HandleLifetime(Transport transport, int elapsedTimeSinceInteraction, bool hadPlayerInteraction)
	{
		if(g_Game.GetARKeyConfig().IsLifetimeMaintenanceEnabled())
		{
			bool expiredDueToNoPlayerInteraction = elapsedTimeSinceInteraction > g_Game.GetARKeyConfig().Get_MaxLifetimeWithoutAnyPlayerInteraction() && !hadPlayerInteraction;
			bool expiredDueToMaxLifetimeRunningOut = elapsedTimeSinceInteraction > g_Game.GetARKeyConfig().Get_MaxLifetime();
			if(expiredDueToNoPlayerInteraction || expiredDueToMaxLifetimeRunningOut)
			{
				AR_LogTransportActivity(transport, "Time elapsed since last interaction was " + MF_Helper.SecondsToDays(elapsedTimeSinceInteraction) + ". Garage is deleting car due to no player interaction. Last time refreshed: " + AR_TimestampToString(AR_VehicleLock.GetLastInteractedWithTime(transport)));
				transport.Delete();
			}
		}
	}    

	static int GetRemainingTimeTilDespawn(Transport transport, int elapsedTimeSinceInteraction, bool hadPlayerInteraction)
	{
		if(g_Game.GetARKeyConfig().IsLifetimeMaintenanceEnabled())
		{
			int remainingTime = g_Game.GetARKeyConfig().Get_MaxLifetimeWithoutAnyPlayerInteraction();
			if(hadPlayerInteraction)
			{
				remainingTime = g_Game.GetARKeyConfig().Get_MaxLifetime();
			}
			remainingTime = remainingTime - elapsedTimeSinceInteraction;
			return remainingTime;
		}
		return transport.GetLifetime();
	}

    static void PlayLockUnlockSound_Server(Transport transport)
	{
		if(GetGame().IsServer() && transport)
		{
            CarScript carScript = CarScript.Cast(transport);
            if(carScript)
            {
                carScript.ar_PlaySound_LockUnlock = true;
            }

            BoatScript boatScript = BoatScript.Cast(transport);
            if(boatScript)
            {
                boatScript.ar_PlaySound_LockUnlock = true;
            }
			
			transport.SetSynchDirty();
		}
	}

	static void PlayLockUnlockSound_Client(Transport transport, string lockSoundset, string unlockSoundset)
	{
		if(GetGame().IsServer())
		{
			return;
		}

		string soundset = lockSoundset;
		if(AR_VehicleLock.IsLocked(transport))
		{
			soundset = unlockSoundset;
		}

		EffectSound sound = SEffectManager.PlaySoundOnObject(soundset, transport);		
		if( sound )
		{
			sound.SetAutodestroy(true);
		}
	}

    static void EEKilledLog(Transport transport, Object killer)
	{
		if (GetGame().IsServer())
		{
            PlayerBase player = PlayerBase.Cast(killer);
            string killerString = "";
            if(player)
            {
                killerString = player.GetIdentity().GetName() + " steam64id=" + player.GetIdentity().GetPlainId();
            }
            else
            {
                EntityAI entity = EntityAI.Cast(killer);
                if(entity && entity.GetHierarchyRootPlayer())
                {
                    player = PlayerBase.Cast(entity.GetHierarchyRootPlayer());
                    killerString = player.GetIdentity().GetName() + " steam64id=" + player.GetIdentity().GetPlainId() + " with " + entity.ToString();
                }
                else
                {
                    killerString = entity.ToString();
                }
            }
            AR_LogTransportActivity(transport, "has been destroyed by: " + killerString);
		}
	}

    
	static void EEDeleteLog(Transport transport)
	{
		if (GetGame().IsServer())
		{
			AR_LogTransportActivity(transport, "is being deleted. This log doesn't mean that the garage deleted it.");
			if(transport.GetLifetime() <= 0)
			{
				AR_LogTransportActivity(transport, "Remaining DB lifetime: " + MF_Helper.SecondsToDays(transport.GetLifetime()) + ". This happened due to lifetime from types running out or something done by DayZ.");
			}
		}
	}
};