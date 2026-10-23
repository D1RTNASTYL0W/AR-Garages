class AR_CarKey_Base : ItemBase
{
    int m_ARKeyId = -1;
	protected string m_AVMVehicleName;
	protected bool m_AVMNameRequested;

    void AR_CarKey_Base()
    {        
        RegisterNetSyncVariableInt( "m_ARKeyId", -1, int.MAX - 1);
    }

	void ResetKey()
	{
		m_ARKeyId = -1;
		SetAVMVehicleName("");
		m_AVMNameRequested = false;
        Synchronize();
	}

    override void OnStoreSave( ParamsWriteContext ctx )
	{   
		super.OnStoreSave( ctx );
        ctx.Write(m_ARKeyId);
		ctx.Write(m_AVMVehicleName);
	}
	
	override bool OnStoreLoad( ParamsReadContext ctx, int version )
	{
		if ( !super.OnStoreLoad( ctx, version ) )
			return false;

        if (!ctx.Read(m_ARKeyId))
            m_ARKeyId = -1;
		if (!ctx.Read(m_AVMVehicleName))
			m_AVMVehicleName = "";

        Synchronize();
		ScheduleNameSync();
		
		return true;
	}

	override void AfterStoreLoad()
	{
		super.AfterStoreLoad();
		Synchronize();
		ScheduleNameSync();
	}

	override void EEInit()
	{
		super.EEInit();
		if (GetGame().IsServer() && m_ARKeyId > 0)
			ScheduleNameSync();
	}

	override void OnInventoryEnter(Man player)
	{
		super.OnInventoryEnter(player);
		if (GetGame().IsServer() && m_ARKeyId > 0)
			ScheduleNameSync();
	}

	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();
		if (GetGame().IsServer())
			return;
		if (m_ARKeyId > 0 && m_AVMVehicleName == "")
		{
			m_AVMNameRequested = false;
			RequestAVMVehicleName();
		}
	}

	void ScheduleNameSync()
	{
		if (!GetGame().IsServer())
			return;
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(AR_SyncVehicleNameToClients, 500, false);
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(AR_SyncVehicleNameToClients, 2500, false);
		if (m_ARKeyId > 0 && m_AVMVehicleName == "")
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(AVMTryBindNameFromGarage, 2500, false);
	}

	string GetAVMVehicleName()
	{
		return m_AVMVehicleName;
	}

	void SetAVMVehicleName(string name)
	{
		if (m_AVMVehicleName == name)
			return;
		m_AVMVehicleName = name;
		AR_SyncVehicleNameToClients();
	}

	void AR_SyncVehicleNameToClients()
	{
		if (!GetGame().IsServer())
			return;
		Param1<string> data = new Param1<string>(m_AVMVehicleName);
		PlayerBase owner = PlayerBase.Cast(GetHierarchyRootPlayer());
		if (owner && owner.GetIdentity())
			GetGame().RPCSingleParam(this, AVM_RPCs.RPC_SYNC_KEY_NAME, data, true, owner.GetIdentity());
		else
			GetGame().RPCSingleParam(this, AVM_RPCs.RPC_SYNC_KEY_NAME, data, true, NULL);
	}

	void AVMTryBindNameFromGarage()
	{
		#ifdef SERVER
		if (!GetGame().IsServer() || m_ARKeyId <= 0 || m_AVMVehicleName != "")
			return;
		string found = ApexVehicleManagementManager.GetDisplayNameForKeyId(m_ARKeyId);
		if (found != "")
			SetAVMVehicleName(found);
		#endif
	}

	void RequestAVMVehicleName()
	{
		if (m_AVMNameRequested || GetGame().IsServer() || m_ARKeyId <= 0)
			return;
		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
		if (!player)
			return;
		m_AVMNameRequested = true;
		Param1<int> data = new Param1<int>(m_ARKeyId);
		GetGame().RPCSingleParam(player, AVM_RPCs.RPC_REQUEST_KEY_NAME, data, true, player.GetIdentity());
	}

	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
		super.OnRPC(sender, rpc_type, ctx);
		if (rpc_type != AVM_RPCs.RPC_SYNC_KEY_NAME)
			return;
		Param1<string> data = new Param1<string>("");
		if (ctx.Read(data))
			m_AVMVehicleName = data.param1;
	}
	
	override bool NameOverride(out string output)
	{
		if(m_ARKeyId > 0)
		{
			string DisplayName;
			GetGame().ObjectGetDisplayName(this, DisplayName);
			if (m_AVMVehicleName != "")
				output = DisplayName + " [" + m_AVMVehicleName + "]";
			else
			{
				output = DisplayName + " [Assigned]";
				RequestAVMVehicleName();
			}
			return true;
		}
		return super.NameOverride(output);
	}

	override bool DescriptionOverride(out string output)
	{
		if (m_ARKeyId > 0 && m_AVMVehicleName != "")
		{
			output = "Assigned to " + m_AVMVehicleName;
			return true;
		}
		return super.DescriptionOverride(output);
	}

    int GenerateNewID()
    {      
		if(g_Game.GetARKeyConfig().GetVersion() >= 2)
		{
			return g_Game.GetARKeyConfig().GetNextCarKeyStoreID();
		}
		else
		{
        	return Math.RandomIntInclusive(1, int.MAX - 1);
		}
    }

    void SetNewARKeyId(int newid)
    {
        m_ARKeyId = newid;
        Synchronize();
    }

    int GetARKeyId()
    {
        return m_ARKeyId;
    }
	
	bool IsKeyAssigned()
	{
		return m_ARKeyId > 0;
	}

    protected void Synchronize()
	{
		if (GetGame().IsServer())
			SetSynchDirty();
    }
    
	override int GetViewIndex()
	{
		if( MemoryPointExists( "invView2" ) )
		{
			AR_CarKeyTag_Base tag = AR_CarKeyTag_Base.Cast(FindAttachmentBySlotName("CarKeyTag"));
			if(tag)
                return 0;
            else
                return 1;
		}
		return 0;
	} 
	
    Transport FindNearbyTransport(PlayerBase player)
    {
        AR_RemoteKeySettings settings = g_Game.GetARKeyConfig().GetRemoteKeySettings();
        if(!settings)
        {
            return null;
        }
        float radius = settings.Range;
        array<Object> objectsInVicinity = new array<Object>();
        array<CargoBase> cargo = new array<CargoBase>();
                     
        vector playerPos = player.GetPosition();
        GetGame().GetObjectsAtPosition3D(playerPos, radius, objectsInVicinity, cargo);
        foreach (Object actorInRadius : objectsInVicinity)
        {
            Transport transport = Transport.Cast(actorInRadius);
            if (!transport)
            {
                continue;
            }
            if(!AR_VehicleLock.HasKeyAssigned(transport))
            {
                continue;
            }
            if (m_ARKeyId == AR_VehicleLock.GetTransportKeyId(transport))
            {	   
                foreach(string classname : settings.VehiclesWithRemoteKey)
                {
                    if(transport.IsKindOf(classname))
                    {
                        return transport;
                    }
                }
            }
        }
        return null;
    }

    override void SetActions()
	{
		super.SetActions();

		AddAction(ActionUnlockCar);
        AddAction(ActionLockCar);
        AddAction(ActionResetKeyId);
        AddAction(ActionAssignNewKey);
        AddAction(ActionCraftDuplicateKey);
		
		
        AR_RemoteKeySettings settings = g_Game.GetARKeyConfig().GetRemoteKeySettings();
        if(!settings)
        {
            return;
        }
		if(settings.CanUnlockRemotely && settings.VehiclesWithRemoteKey && settings.VehiclesWithRemoteKey.Count() > 0)
		{
			AddAction(ActionARRemoteOpenClose);
		}
	}

    #ifdef UNIVERSALAPI
	override void OnUApiSave(UApiEntityStore data){
		super.OnUApiSave(data);
		data.Write("m_ARKeyId", m_ARKeyId);
		data.Write("m_AVMVehicleName", m_AVMVehicleName);
	}
	
	override void OnUApiLoad(UApiEntityStore data){
		super.OnUApiLoad(data);
		data.Read("m_ARKeyId", m_ARKeyId);
		if (!data.Read("m_AVMVehicleName", m_AVMVehicleName))
			m_AVMVehicleName = "";
        Synchronize();
		ScheduleNameSync();
	}
    #endif
};
class AR_CarKey_Blue: AR_CarKey_Base{};
class AR_CarKey_Green: AR_CarKey_Base{};
class AR_CarKey_Red: AR_CarKey_Base{};
class AR_CarKey_White: AR_CarKey_Base{};
class AR_CarKey_Yellow: AR_CarKey_Base{};
class AR_CarKey_Purple: AR_CarKey_Base{};
class AR_CarKey_Pink: AR_CarKey_Base{};

class AR_MasterKey : ItemBase 
{
    override void SetActions()
	{
		super.SetActions();

		AddAction(ActionUnlockCar);
        AddAction(ActionLockCar);
        AddAction(ActionResetKeyId);
	}
};

class AR_ResetKey : ItemBase 
{
    override void SetActions()
	{
		super.SetActions();
        AddAction(ActionResetKeyId);
	}
};

class AR_InfoKey : ItemBase 
{
    override void SetActions()
	{
		super.SetActions();
        AddAction(ActionGetInfoKey);
	}
};

