class ApexVehicleManagementStoredCar
{
	string className;
	string displayName;
	int carScriptId;
	int carKeyId;
	string playerId;
	string playerName;
	int timestamp;
	string date;
	vector lastPosition;
    // Read-only snapshot captured when a vehicle is parked (new records only).
    ref array<string> previewAttachments;
    ref array<string> previewCargo;
    ref array<float> previewAttachmentHealth;
    ref array<float> previewCargoHealth;
    float previewHealth = -1;
    float previewFuel = -1;
    float previewEngine = -1;
    float previewTransmission = -1;
    float previewTires = -1;
};

// Read-only DTO for the stored inventory JSON. Never write this DTO
// back to MongoDB: the original Payload remains the authoritative restore source.
class AVM_PreviewNode
{
    string typeName;
    float health;
    ref map<string, float> damageZones;
    ref array<ref AVM_PreviewNode> attachments;
    ref array<ref AVM_PreviewNode> cargo;
};
class AVM_PreviewPayload
{
    ref AVM_PreviewNode inventory;
};

class AVM_MongoDocument
{
    string RecordId;
    string Status = "stored";
    ref ApexVehicleManagementStoredCar Meta;
    ref ApexVehicleManagementVehicle Payload;
};

class AVM_StoredCarClient
{
    string className; // Vehicle type for client-side 3D preview
	string displayName;
	string playerName;
	int carScriptId;
	string expiryDate;
	vector lastPosition;
    ref array<string> previewAttachments;
    ref array<string> previewCargo;
    ref array<float> previewAttachmentHealth;
    ref array<float> previewCargoHealth;
    float previewHealth = -1;
    float previewFuel = -1;
    float previewEngine = -1;
    float previewTransmission = -1;
    float previewTires = -1;
    string storedDate;
};
class AVM_TransferData
{
	vector Position;
	vector Orientation;
	int CarsStoredCount = -1;
	ref array<ref AVM_StoredCarClient> StoredCars;
	ref AVM_StoredCarClient NearbyParkCar;
};

#ifdef SERVER
/***********************
 * Storage Structures */

class ApexVehicleManagementStoredData 
{
	int version = 1;
	ref array<ref ApexVehicleManagementStoredCar> StoredCars = new array<ref ApexVehicleManagementStoredCar>;

	int FindCar(int CarScriptId)
	{
		for(int i = 0; i < StoredCars.Count(); i++)
		{
			if(StoredCars.Get(i).carScriptId == CarScriptId)
			{
				return i;
			}
		}
		return -1;
	}

	void RemoveStoredCar(int CarScriptId)
	{
		int foundIndex = FindCar(CarScriptId);
		if(foundIndex > -1)
		{
			StoredCars.Remove(foundIndex);
		}
	}
	
    AVM_StoredCarClient ToClient(ApexVehicleManagementStoredCar storedCar)
    {
        ref AVM_StoredCarClient clientInfo = new AVM_StoredCarClient;
        if (!storedCar)
            return clientInfo;
        clientInfo.className = storedCar.className;
        clientInfo.displayName = storedCar.displayName;
        clientInfo.playerName = storedCar.playerName;
        clientInfo.carScriptId = storedCar.carScriptId;
        clientInfo.lastPosition = storedCar.lastPosition;
        clientInfo.previewAttachments = storedCar.previewAttachments;
        clientInfo.previewCargo = storedCar.previewCargo;
        clientInfo.previewAttachmentHealth = storedCar.previewAttachmentHealth;
        clientInfo.previewCargoHealth = storedCar.previewCargoHealth;
        clientInfo.previewHealth = storedCar.previewHealth;
        clientInfo.previewFuel = storedCar.previewFuel;
        clientInfo.previewEngine = storedCar.previewEngine;
        clientInfo.previewTransmission = storedCar.previewTransmission;
        clientInfo.previewTires = storedCar.previewTires;
        clientInfo.storedDate = storedCar.date;
        if (g_Game.GetApexVehicleManagementSettings().DeleteExpiredCars)
            clientInfo.expiryDate = MF_Helper.SecondsToDays(storedCar.timestamp + g_Game.GetApexVehicleManagementSettings().StoredCarLifetime - MF_Helper.GetUnixTime());
        else
            clientInfo.expiryDate = "N/A";
        return clientInfo;
    }

	AVM_TransferData FilterByKeyIdOrPlayerId(string PlayerId, array<int> carKeyIds)
	{
		ref array<ref AVM_StoredCarClient> filtered = new array<ref AVM_StoredCarClient>;
		ref map<int, bool> keyMap = new map<int, bool>;
		if (carKeyIds)
		{
			for (int k = 0; k < carKeyIds.Count(); k++)
				keyMap.Set(carKeyIds.Get(k), true);
		}
		for(int i = 0; i < StoredCars.Count(); i++)
		{
			ApexVehicleManagementStoredCar storedCar = StoredCars.Get(i);
			bool hasKey = false;
			if (storedCar.carKeyId > 0 && keyMap.Contains(storedCar.carKeyId))
				hasKey = true;
			if(storedCar.playerId == PlayerId || hasKey)
				filtered.Insert(ToClient(storedCar));
		}
		AVM_TransferData filteredData = new AVM_TransferData();
		filteredData.StoredCars = filtered;
		return filteredData;
	}

	AVM_TransferData FilterByPlayerId(string PlayerId)
	{
		ref array<ref AVM_StoredCarClient> filtered = new array<ref AVM_StoredCarClient>;
		for(int i = 0; i < StoredCars.Count(); i++)
		{
			ApexVehicleManagementStoredCar storedCar = StoredCars.Get(i);
			if(storedCar.playerId == PlayerId)
				filtered.Insert(ToClient(storedCar));
		}
		AVM_TransferData filteredData = new AVM_TransferData();
		filteredData.StoredCars = filtered;
		return filteredData;
	}
	
	int GetCountOfStoredCarPerPlayerId(string PlayerId)
	{
		int count = 0;
		for(int i = 0; i < StoredCars.Count(); i++)
		{
			ApexVehicleManagementStoredCar storedCar = StoredCars.Get(i);
			if(storedCar.playerId == PlayerId)
			{
				count++;
			}
		}
		return count;
	}
};

class AVM_PendingGarageOpen
{
	PlayerIdentity Identity;
	vector Position;
	vector Orientation;
	ref array<string> Queries;
	int QueryIndex;
};

/***********************
 * Manager */
class ApexVehicleManagementManager 
{
	protected static ref ApexVehicleManagementStoredData m_StoredCars = new ApexVehicleManagementStoredData;
	protected static ref array<int> m_VehiclesToRemove = new array<int>;
	protected static ref array<int> m_VehiclesFilesToRemove = new array<int>;
	protected static int m_LastStoreTime;
	protected static bool m_StoreIsDirty = false;
	protected static PluginARKeyLogs m_ARKeyLogger;
    protected static ref UApiDBHandler<AVM_MongoDocument> m_DB;
    protected static ref ApexVehicleManagementMongoCallbacks m_Callbacks;
    protected static ref array<ref AVM_MongoDocument> m_Records = new array<ref AVM_MongoDocument>;
    protected static ref map<int, ref AVM_MongoDocument> m_PendingPark = new map<int, ref AVM_MongoDocument>;
    protected static ref map<int, PlayerIdentity> m_PendingRetrieve = new map<int, PlayerIdentity>;
    protected static ref map<int, PlayerIdentity> m_PendingDelete = new map<int, PlayerIdentity>;
    protected static ref map<int, vector> m_PendingPosition = new map<int, vector>;
    protected static ref map<int, vector> m_PendingOrientation = new map<int, vector>;
    protected static bool m_IndexReady = false;
    protected static bool m_IndexBusy = false;
    protected static int m_LastIndexRefreshMs = 0;
    protected static ref map<int, string> m_KeyNameById = new map<int, string>;
    protected static ref array<ref AVM_PendingGarageOpen> m_GarageOpenQueue = new array<ref AVM_PendingGarageOpen>;
    protected static ref AVM_PendingGarageOpen m_ActiveGarageOpen;

	/**
	 * Initializes the ApexVehicleManagement manager by creating the necessary directory if it does not exist,
	 * loading stored cars, and registering inbound and outbound RPC endpoints.
	 */
	static bool UseMongoDB()
	{
		ApexVehicleManagementSettings settings = g_Game.GetApexVehicleManagementSettings();
		if (!settings)
			return true;
		return settings.UseMongoDB;
	}

	static void Init() 
	{
		if (!FileExist(ApexVehicleManagementSettings.FOLDER))
			MakeDirectory(ApexVehicleManagementSettings.FOLDER);		
		m_ARKeyLogger = PluginARKeyLogs.Cast(GetPlugin(PluginARKeyLogs));
		if (UseMongoDB())
		{
			m_DB = new UApiDBHandler<AVM_MongoDocument>("ApexVehicleManagement", OBJECT_DB);
			m_Callbacks = new ApexVehicleManagementMongoCallbacks();
			m_IndexReady = true;
			Print("[AVM] Mongo garage ready. Cars load when a player opens a garage.");
			return;
		}
		LoadJsonIndex();
		ResetTimer();
	}

	static void SaveJsonIndex()
	{
		JsonFileLoader<ApexVehicleManagementStoredData>.JsonSaveFile(ApexVehicleManagementSettings.STORED_CARS, m_StoredCars);
	}

	static void LoadJsonIndex()
	{
		m_IndexBusy = false;
		if (FileExist(ApexVehicleManagementSettings.STORED_CARS))
			JsonFileLoader<ApexVehicleManagementStoredData>.JsonLoadFile(ApexVehicleManagementSettings.STORED_CARS, m_StoredCars);
		if (!m_StoredCars)
			m_StoredCars = new ApexVehicleManagementStoredData;
		if (!m_StoredCars.StoredCars)
			m_StoredCars.StoredCars = new array<ref ApexVehicleManagementStoredCar>;
		for (int i = 0; i < m_StoredCars.StoredCars.Count(); i++)
		{
			ApexVehicleManagementStoredCar meta = m_StoredCars.StoredCars.Get(i);
			if (!meta || meta.carScriptId <= 0)
				continue;
			if (meta.previewAttachments && meta.previewAttachments.Count() > 0)
				continue;
			ApexVehicleManagementVehicle payload = new ApexVehicleManagementVehicle;
			payload.Load(meta.carScriptId);
			if (payload.ClassName != "")
			{
				AVM_MongoDocument doc = new AVM_MongoDocument;
				doc.Meta = meta;
				doc.Payload = payload;
				BuildPreviewFromPayload(doc);
			}
		}
		m_IndexReady = true;
		RebuildKeyNameIndex();
		Print("[AVM] JSON garage loaded stored=" + m_StoredCars.StoredCars.Count().ToString());
	}

	/**
	 * @brief Resets the timer for saving the game.
	 * This function runs the save timer with the interval specified in the ApexVehicleManagementSettings.
	 */
	protected static void ResetTimer() 
	{
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(OnStoreSave, g_Game.GetApexVehicleManagementSettings().StoreSaveInterval * 1000, true);
	}

	/**
	 * Loads stored cars from a JSON file.
	 * If the file does not exist, the function returns without doing anything.
	 */
    static void RefreshIndex()
    {
		RefreshIndexEx(false);
    }

    static void RefreshIndexForced()
    {
		RefreshIndexEx(true);
    }

    static void RefreshIndexEx(bool force)
    {
		if (!UseMongoDB())
		{
			LoadJsonIndex();
			return;
		}
		StartNextGarageQuery();
    }

	static array<string> BuildPlayerGarageQueries(string steamId, array<int> keyIds)
	{
		array<string> queries = new array<string>;
		queries.Insert("{\"Status\":\"stored\"}");
		return queries;
	}

	static void QueueGarageOpen(PlayerIdentity identity, vector position, vector orientation)
	{
		if (!identity)
			return;
		string steamId = identity.GetPlainId();
		for (int i = 0; i < m_GarageOpenQueue.Count(); i++)
		{
			AVM_PendingGarageOpen queued = m_GarageOpenQueue.Get(i);
			if (queued && queued.Identity && queued.Identity.GetPlainId() == steamId)
			{
				queued.Position = position;
				queued.Orientation = orientation;
				StartNextGarageQuery();
				return;
			}
		}
		AVM_PendingGarageOpen pending = new AVM_PendingGarageOpen;
		pending.Identity = identity;
		pending.Position = position;
		pending.Orientation = orientation;
		m_GarageOpenQueue.Insert(pending);
		StartNextGarageQuery();
	}

	static void StartNextGarageQuery()
	{
		if (!UseMongoDB() || m_IndexBusy)
			return;
		if (m_ActiveGarageOpen && m_ActiveGarageOpen.Queries && m_ActiveGarageOpen.QueryIndex < m_ActiveGarageOpen.Queries.Count())
		{
			FireGarageQuery(m_ActiveGarageOpen.Queries.Get(m_ActiveGarageOpen.QueryIndex));
			m_ActiveGarageOpen.QueryIndex = m_ActiveGarageOpen.QueryIndex + 1;
			return;
		}
		if (m_GarageOpenQueue.Count() == 0)
			return;
		m_ActiveGarageOpen = m_GarageOpenQueue.Get(0);
		m_GarageOpenQueue.Remove(0);
		if (!m_ActiveGarageOpen || !m_ActiveGarageOpen.Identity)
		{
			m_ActiveGarageOpen = null;
			StartNextGarageQuery();
			return;
		}
		PlayerBase player = PlayerBase.Cast(m_ActiveGarageOpen.Identity.GetPlayer());
		if (!player)
		{
			m_ActiveGarageOpen = null;
			StartNextGarageQuery();
			return;
		}
		array<int> inventoryKeys = new array<int>;
		bool hasMaster;
		AVM_Helper.CollectInventoryKeys(player, inventoryKeys, hasMaster);
		m_ActiveGarageOpen.Queries = BuildPlayerGarageQueries(m_ActiveGarageOpen.Identity.GetPlainId(), inventoryKeys);
		m_ActiveGarageOpen.QueryIndex = 0;
		StartNextGarageQuery();
	}

	static void FireGarageQuery(string query)
	{
		Print("[AVM] player garage query " + query);
		m_IndexBusy = true;
		int callId = m_DB.Query(query, m_Callbacks, "OnIndex");
		if (callId != -1)
			return;
		m_IndexBusy = false;
		FinishActiveGarageOpen();
	}

	static void FinishActiveGarageOpen()
	{
		AVM_PendingGarageOpen pending = m_ActiveGarageOpen;
		m_ActiveGarageOpen = null;
		if (pending)
			SendParkedListReady(pending);
		StartNextGarageQuery();
	}

    static void RebuildKeyNameIndex()
    {
		m_KeyNameById = new map<int, string>;
		if (!m_StoredCars || !m_StoredCars.StoredCars)
			return;
		for (int i = 0; i < m_StoredCars.StoredCars.Count(); i++)
		{
			ApexVehicleManagementStoredCar storedCar = m_StoredCars.StoredCars.Get(i);
			if (storedCar && storedCar.carKeyId > 0 && storedCar.displayName != "")
				m_KeyNameById.Set(storedCar.carKeyId, storedCar.displayName);
		}
    }

    static float ConfigHitpoints(string typeName)
    {
        if (typeName == "")
            return 0;
        float maxHealth = GetGame().ConfigGetFloat("CfgVehicles " + typeName + " DamageSystem GlobalHealth Health hitpoints");
        if (maxHealth <= 0)
            maxHealth = GetGame().ConfigGetFloat("CfgVehicles " + typeName + " DamageSystem GlobalHealth hitpoints");
        return maxHealth;
    }

    static float ConfigZoneHitpoints(string vehicleType, string zone)
    {
        if (vehicleType == "" || zone == "")
            return 0;
        return GetGame().ConfigGetFloat("CfgVehicles " + vehicleType + " DamageSystem DamageZones " + zone + " Health hitpoints");
    }

    static float Health01FromAbsolute(string typeName, float health)
    {
        float maxHealth = ConfigHitpoints(typeName);
        if (maxHealth <= 0 || health < 0)
            return -1;
        return Math.Clamp(health / maxHealth, 0.0, 1.0);
    }

    static float Zone01(string vehicleType, map<string, float> zones, string zone)
    {
        if (!zones || !zones.Contains(zone))
            return -1;
        float maxHealth = ConfigZoneHitpoints(vehicleType, zone);
        if (maxHealth <= 0)
            return -1;
        return Math.Clamp(zones.Get(zone) / maxHealth, 0.0, 1.0);
    }

    static void CaptureTransportPreview(Transport transport, ApexVehicleManagementStoredCar storedCar)
    {
        if (!transport || !storedCar)
            return;
        if (storedCar.className == "")
            storedCar.className = transport.GetType();
        if (storedCar.displayName == "")
            storedCar.displayName = transport.GetDisplayName();
        if (storedCar.carScriptId <= 0)
            storedCar.carScriptId = AR_VehicleLock.GetTransportId(transport);
        storedCar.lastPosition = transport.GetPosition();
        storedCar.previewAttachments = new array<string>;
        storedCar.previewCargo = new array<string>;
        storedCar.previewAttachmentHealth = new array<float>;
        storedCar.previewCargoHealth = new array<float>;
        storedCar.previewHealth = transport.GetHealth01("", "");
        storedCar.previewEngine = -1;
        storedCar.previewTransmission = -1;
        if (ConfigZoneHitpoints(storedCar.className, "Engine") > 0)
            storedCar.previewEngine = transport.GetHealth01("Engine", "");
        if (ConfigZoneHitpoints(storedCar.className, "Gearbox") > 0)
            storedCar.previewTransmission = transport.GetHealth01("Gearbox", "");
        else if (ConfigZoneHitpoints(storedCar.className, "Transmission") > 0)
            storedCar.previewTransmission = transport.GetHealth01("Transmission", "");
        storedCar.previewFuel = -1;
        storedCar.previewTires = -1;
        CarScript parkedCar = CarScript.Cast(transport);
        if (parkedCar)
            storedCar.previewFuel = parkedCar.GetFluidFraction(CarFluid.FUEL);
        else
        {
            float fuelFraction = 0;
            float fuelCapacity = 0;
            AR_VehicleLock.GetFluidData(transport, CarFluid.FUEL, fuelFraction, fuelCapacity);
            if (fuelCapacity > 0)
                storedCar.previewFuel = fuelFraction;
        }
        float tireTotal = 0;
        int tireCount = 0;
        if (transport.GetInventory())
        {
            for (int attachmentIndex = 0; attachmentIndex < transport.GetInventory().AttachmentCount(); attachmentIndex++)
            {
                EntityAI part = transport.GetInventory().GetAttachmentFromIndex(attachmentIndex);
                if (!part)
                    continue;
                storedCar.previewAttachments.Insert(part.GetType());
                storedCar.previewAttachmentHealth.Insert(part.GetHealth01("", ""));
                string partType = part.GetType();
                partType.ToLower();
                if (partType.Contains("wheel") || partType.Contains("tire"))
                {
                    tireTotal = tireTotal + part.GetHealth01("", "");
                    tireCount++;
                }
            }
            AVM_Helper.CollectEntityCargo(transport, storedCar.previewCargo, storedCar.previewCargoHealth, "");
        }
        if (tireCount > 0)
            storedCar.previewTires = tireTotal / tireCount;
    }

    static void CollectPreviewCargoNode(AVM_PreviewNode node, array<string> dest, array<float> healths, string prefix)
    {
        if (!node || !dest)
            return;
        if (AVM_Helper.IsGarageContainerType(node.typeName))
        {
            if (node.cargo)
            {
                foreach (AVM_PreviewNode containerCargo : node.cargo)
                    CollectPreviewCargoNode(containerCargo, dest, healths, prefix);
            }
            if (node.attachments)
            {
                foreach (AVM_PreviewNode containerAtt : node.attachments)
                    CollectPreviewCargoFromNode(containerAtt, dest, healths, prefix);
            }
            return;
        }
        if (node.typeName != "")
        {
            dest.Insert(prefix + node.typeName);
            if (healths)
                healths.Insert(Health01FromAbsolute(node.typeName, node.health));
        }
        string nestedPrefix = prefix + "  + ";
        if (node.cargo)
        {
            foreach (AVM_PreviewNode cargoChild : node.cargo)
                CollectPreviewCargoNode(cargoChild, dest, healths, nestedPrefix);
        }
    }

    static void CollectPreviewCargoFromNode(AVM_PreviewNode node, array<string> dest, array<float> healths, string prefix)
    {
        if (!node)
            return;
        if (node.cargo)
        {
            foreach (AVM_PreviewNode cargoChild : node.cargo)
                CollectPreviewCargoNode(cargoChild, dest, healths, prefix);
        }
        if (node.attachments)
        {
            foreach (AVM_PreviewNode attChild : node.attachments)
                CollectPreviewCargoFromNode(attChild, dest, healths, prefix);
        }
    }

    static void CollectPreviewParts(AVM_PreviewNode node, array<string> dest, array<float> healths)
    {
        if (!node || !dest)
            return;
        if (node.typeName != "")
        {
            dest.Insert(node.typeName);
            if (healths)
                healths.Insert(Health01FromAbsolute(node.typeName, node.health));
        }
        if (node.attachments)
        {
            foreach (AVM_PreviewNode child : node.attachments)
                CollectPreviewParts(child, dest, healths);
        }
    }

    // Builds display data from each MongoDB document, including OLD stored cars.
    // Never changes saved inventory or calls RestoreVehicle.
    static void BuildPreviewFromPayload(AVM_MongoDocument doc)
    {
        if (!doc || !doc.Meta || !doc.Payload) return;
        ApexVehicleManagementStoredCar meta = doc.Meta;
        // Payload is authoritative for identity, independent of index order.
        if (doc.Payload.ClassName != "") meta.className = doc.Payload.ClassName;
        if (doc.Payload.DisplayName != "") meta.displayName = doc.Payload.DisplayName;
        meta.carScriptId = doc.Payload.CarScriptId;
        meta.previewAttachments = new array<string>;
        meta.previewCargo = new array<string>;
        meta.previewAttachmentHealth = new array<float>;
        meta.previewCargoHealth = new array<float>;
        string payloadJson = JsonFileLoader<ApexVehicleManagementVehicle>.JsonMakeData(doc.Payload);
        ref AVM_PreviewPayload view = new AVM_PreviewPayload;
        JsonFileLoader<AVM_PreviewPayload>.JsonLoadData(payloadJson, view);
        if (!view || !view.inventory) return;

        float bodyHealth = Health01FromAbsolute(meta.className, view.inventory.health);
        if (bodyHealth < 0 && view.inventory.damageZones)
            bodyHealth = Zone01(meta.className, view.inventory.damageZones, "Chassis");
        if (bodyHealth >= 0)
            meta.previewHealth = bodyHealth;

        if (view.inventory.damageZones)
        {
            float engine = Zone01(meta.className, view.inventory.damageZones, "Engine");
            if (engine >= 0)
                meta.previewEngine = engine;
            float gearbox = Zone01(meta.className, view.inventory.damageZones, "Gearbox");
            if (gearbox < 0)
                gearbox = Zone01(meta.className, view.inventory.damageZones, "Transmission");
            if (gearbox >= 0)
                meta.previewTransmission = gearbox;
        }

        if (meta.previewFuel < 0 && doc.Payload.fluids && doc.Payload.fluids.Count() > 0)
        {
            float capacity = GetGame().ConfigGetFloat("CfgVehicles " + meta.className + " fuelCapacity");
            if (capacity > 0)
                meta.previewFuel = Math.Clamp(doc.Payload.fluids.Get(0) / capacity, 0.0, 1.0);
        }

        float tireTotal = 0;
        int tireCount = 0;
        if (view.inventory.attachments)
        {
            foreach (AVM_PreviewNode part : view.inventory.attachments)
            {
                CollectPreviewParts(part, meta.previewAttachments, meta.previewAttachmentHealth);
                CollectPreviewCargoFromNode(part, meta.previewCargo, meta.previewCargoHealth, "");
                if (part && part.typeName != "")
                {
                    string partType = part.typeName;
                    partType.ToLower();
                    if (partType.Contains("wheel") || partType.Contains("tire"))
                    {
                        float tireHealth = Health01FromAbsolute(part.typeName, part.health);
                        if (tireHealth >= 0)
                        {
                            tireTotal = tireTotal + tireHealth;
                            tireCount++;
                        }
                    }
                }
            }
        }
        if (tireCount > 0)
            meta.previewTires = tireTotal / tireCount;

        if (view.inventory.cargo)
        {
            foreach (AVM_PreviewNode item : view.inventory.cargo)
                CollectPreviewCargoNode(item, meta.previewCargo, meta.previewCargoHealth, "");
        }
    }

    // Universal API queries may deliver only a subset of documents per refresh.
    // Keep previously observed stored vehicles, keyed by immutable carScriptId,
    // rather than replacing the entire index with the latest partial response.
    // Explicit retrieval removes an entry; returned non-stored records also remove it.
    static string GetDisplayNameForKeyId(int keyId)
    {
        string name;
        if (keyId <= 0 || !m_KeyNameById)
            return "";
        if (m_KeyNameById.Find(keyId, name))
            return name;
        return "";
    }

    static void BindNameOntoPlayerKeys(PlayerBase player, int keyId, string name)
    {
        if (!player || !player.GetInventory() || keyId <= 0 || name == "")
            return;
        array<EntityAI> entities = new array<EntityAI>;
        player.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, entities);
        for (int i = 0; i < entities.Count(); i++)
        {
            AR_CarKey_Base carKey = AR_CarKey_Base.Cast(entities.Get(i));
            if (carKey && carKey.GetARKeyId() == keyId)
                carKey.SetAVMVehicleName(name);
        }
    }

    static void BindAllPlayerKeyNames(PlayerBase player)
    {
        if (!player || !player.GetInventory())
            return;
        array<EntityAI> entities = new array<EntityAI>;
        player.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, entities);
        for (int i = 0; i < entities.Count(); i++)
        {
            AR_CarKey_Base carKey = AR_CarKey_Base.Cast(entities.Get(i));
            if (!carKey || carKey.GetARKeyId() <= 0)
                continue;
            string name = GetDisplayNameForKeyId(carKey.GetARKeyId());
            if (name != "")
                carKey.SetAVMVehicleName(name);
        }
    }

    static void RPC_RequestKeyName(PlayerIdentity sender, ParamsReadContext ctx)
    {
        Param1<int> args = new Param1<int>(-1);
        if (!ctx.Read(args) || !sender)
            return;
        PlayerBase player = PlayerBase.Cast(sender.GetPlayer());
        if (!player)
            return;
        BindAllPlayerKeyNames(player);
        string name = GetDisplayNameForKeyId(args.param1);
        if (name != "")
            BindNameOntoPlayerKeys(player, args.param1, name);
        if (!player.GetInventory())
            return;
        array<EntityAI> entities = new array<EntityAI>;
        player.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, entities);
        for (int i = 0; i < entities.Count(); i++)
        {
            AR_CarKey_Base carKey = AR_CarKey_Base.Cast(entities.Get(i));
            if (carKey && carKey.GetARKeyId() == args.param1)
                carKey.AR_SyncVehicleNameToClients();
        }
    }

    static void RemoveIndexedCar(int id)
    {
        m_StoredCars.RemoveStoredCar(id);
        for (int j = m_Records.Count() - 1; j >= 0; j--)
        {
            if (m_Records.Get(j) && m_Records.Get(j).Meta && m_Records.Get(j).Meta.carScriptId == id)
                m_Records.Remove(j);
        }
    }

    static void IndexResult(int status, UApiQueryResult<AVM_MongoDocument> result)
    {
        m_IndexBusy = false;
        if (status != UAPI_SUCCESS && status != UAPI_EMPTY)
        {
            Print("[AVM] MongoDB garage query failed status=" + status.ToString() + "; using last known cars");
        }
        else
        {
            m_Records.Clear();
            if (m_StoredCars && m_StoredCars.StoredCars)
                m_StoredCars.StoredCars.Clear();
            else
                m_StoredCars = new ApexVehicleManagementStoredData;
            int received = 0;
            if (result && result.GetResults())
            {
                foreach (AVM_MongoDocument document : result.GetResults())
                {
                    if (!document || !document.Meta) continue;
                    received++;
                    string serializedDoc = JsonFileLoader<AVM_MongoDocument>.JsonMakeData(document);
                    ref AVM_MongoDocument stableDoc = new AVM_MongoDocument;
                    JsonFileLoader<AVM_MongoDocument>.JsonLoadData(serializedDoc, stableDoc);
                    if (!stableDoc || !stableDoc.Meta || !stableDoc.Payload) continue;
                    int vehicleId = stableDoc.Payload.CarScriptId;
                    if (vehicleId <= 0) continue;
                    if (stableDoc.Status != "stored")
                        continue;
                    BuildPreviewFromPayload(stableDoc);
                    m_Records.Insert(stableDoc);
                    m_StoredCars.StoredCars.Insert(stableDoc.Meta);
                    Print("[AVM INDEX] id=" + vehicleId.ToString() + " class=" + stableDoc.Meta.className + " name=" + stableDoc.Meta.displayName);
                }
            }
            Print("[AVM INDEX] stored query documents=" + received.ToString() + " cached stored=" + m_StoredCars.StoredCars.Count().ToString());
        }
        m_IndexReady = true;
        RebuildKeyNameIndex();
        if (m_ActiveGarageOpen && m_ActiveGarageOpen.Queries && m_ActiveGarageOpen.QueryIndex < m_ActiveGarageOpen.Queries.Count())
        {
            StartNextGarageQuery();
            return;
        }
        FinishActiveGarageOpen();
    }

    static void ParkResult(int id, int status)
    {
        AVM_MongoDocument doc;
        if (!m_PendingPark.Find(id, doc)) return;
        m_PendingPark.Remove(id);
        if (status != UAPI_SUCCESS) {
            Error("[AVM] MongoDB save failed for vehicle " + id + " status=" + status.ToString() + "; physical vehicle was NOT deleted");
            return;
        }
        Print("[AVM] Mongo save ok id=" + id.ToString() + " player=" + doc.Meta.playerId + " key=" + doc.Meta.carKeyId.ToString());
        Transport vehicle = AVM_Helper.GetNearbyTransport(doc.Meta.lastPosition, 5);
        if (!vehicle || AR_VehicleLock.GetTransportId(vehicle) != id) {
            Error("[AVM] Saved car is no longer at original location; mongo record kept id=" + id.ToString());
            RemoveIndexedCar(id);
            if (doc.Meta)
                m_StoredCars.StoredCars.Insert(doc.Meta);
            m_Records.Insert(doc);
            RebuildKeyNameIndex();
            return;
        }
        AR_VehicleLock.SetDeletionByMod(vehicle);
        GetGame().ObjectDelete(vehicle);
        RemoveIndexedCar(id);
        if (doc.Meta)
            m_StoredCars.StoredCars.Insert(doc.Meta);
        m_Records.Insert(doc);
        RebuildKeyNameIndex();
    }

    static void RetrieveResult(int id, int status, AVM_MongoDocument doc)
    {
        PlayerIdentity identity;
        if (!m_PendingRetrieve.Find(id, identity)) return;
        if (status != UAPI_SUCCESS || !doc || !doc.Payload || doc.Status != "stored") {
            string docStatus = "";
            if (doc)
                docStatus = doc.Status;
            Print("[AVM] retrieve ignored id=" + id.ToString() + " mongoStatus=" + docStatus);
            m_PendingRetrieve.Remove(id);
            RemoveIndexedCar(id);
            RebuildKeyNameIndex();
            return;
        }
        PlayerBase player = PlayerBase.Cast(identity.GetPlayer());
        if (!player || (doc.Meta.playerId != identity.GetPlainId() && !AVM_Helper.PlayerHasKeyId(player, doc.Meta.carKeyId))) {
            m_PendingRetrieve.Remove(id);
            return;
        }
        // Claim by conditional query. The backend update is atomic; verification is still
        // required before spawning and is handled by the subsequent load callback.
        string claim = identity.GetPlainId() + "_" + MF_Helper.GetUnixTime().ToString() + "_" + id.ToString();
        m_RetrieveClaims.Set(id, claim);
        UApiDBQuery query = new UApiDBQuery("{\"RecordId\":\"" + doc.RecordId + "\",\"Status\":\"stored\"}");
        UApi().db(OBJECT_DB).QueryUpdate(query, "ApexVehicleManagement", "Status", "claimed:" + claim, UpdateOpts.SET, m_Callbacks, "OnClaim");
    }

    protected static ref map<int, string> m_RetrieveClaims = new map<int, string>;

    static void ClaimResult(int status, string oid, string response)
    {
        // The backend reports matched Count; verify the exact token before spawning.
        if (status != UAPI_SUCCESS) return;
        UApiQueryUpdateResponse claimUpdateResult = new UApiQueryUpdateResponse;
        JsonFileLoader<UApiQueryUpdateResponse>.JsonLoadData(response, claimUpdateResult);
        if (!claimUpdateResult || claimUpdateResult.Count != 1) return;
        foreach (int id, string claim : m_RetrieveClaims) {
            m_DB.Load(id.ToString(), m_Callbacks, "OnVerifyClaim");
        }
    }

    static void VerifyClaim(int id, int status, AVM_MongoDocument doc)
    {
        string claim;
        PlayerIdentity identity;
        if (!m_RetrieveClaims.Find(id, claim) || !m_PendingRetrieve.Find(id, identity)) return;
        if (status != UAPI_SUCCESS || !doc || doc.Status != "claimed:" + claim) {
            m_RetrieveClaims.Remove(id);
            m_PendingRetrieve.Remove(id);
            return;
        }
        PlayerBase player = PlayerBase.Cast(identity.GetPlayer());
        vector position, orientation;
        if (!player || !m_PendingPosition.Find(id, position) || !m_PendingOrientation.Find(id, orientation)) return;
        if (doc.Payload.RestoreVehicle(position, orientation, player)) {
            m_DB.Update(id.ToString(), "Status", "retrieved", UpdateOpts.SET);
            RemoveIndexedCar(id);
            RebuildKeyNameIndex();
        } else {
            // Failed spawn: restore claim to stored, allowing retry.
            m_DB.Update(id.ToString(), "Status", "stored", UpdateOpts.SET);
        }
        m_RetrieveClaims.Remove(id);
        m_PendingRetrieve.Remove(id);
        m_PendingPosition.Remove(id);
        m_PendingOrientation.Remove(id);
    }

	static bool RPC_ParkVehicle(PlayerIdentity sender, ParamsReadContext ctx) 
	{
		Param1<vector> rpc_params = new Param1<vector>("0 0 0");
		ctx.Read(rpc_params);
		return ParkVehicle(sender, rpc_params.param1);
	}

	static bool ParkVehicle(PlayerIdentity sender, vector position) 
	{
		Transport transport = AVM_Helper.GetNearbyTransport(position, 4);
		string reason = "";
		PlayerBase player = PlayerBase.Cast(sender.GetPlayer());
		string playerID = sender.GetPlainId();
		string playerName = sender.GetName();
		int maxStoredCars = g_Game.GetApexVehicleManagementSettings().MaxStoredCarsPerPlayer;
		if (maxStoredCars > 0)
		{//refactor this in helper CanPark
			if(m_StoredCars.GetCountOfStoredCarPerPlayerId(playerID) >= maxStoredCars)
			{
				if(!AVM_Helper.PlayerHasMasterKey(player))
				{
					reason = "#avm_e_fullgarage";
					MF_Helper.SendMessageToClient(player, reason);
					return false;
				}
			}
		}
		if(!AVM_Helper.CanPark(player, transport, reason))
		{
			MF_Helper.SendMessageToClient(player, reason);
			return false;
		}
		ApexVehicleManagementVehicle v = new ApexVehicleManagementVehicle(transport);
		if (!v.StoreVehicle(playerID, playerName)) 
		{
			Error("ApexVehicleManagement: Failed to store vehicle");
			return false;
		}
        if (!m_IndexReady || m_StoredCars.FindCar(v.CarScriptId) != -1 || m_PendingPark.Contains(v.CarScriptId)) {
            MF_Helper.SendMessageToClient(player, "Garage database is loading, or this vehicle is already stored.");
            return false;
        }
        ref ApexVehicleManagementStoredCar storedCar = new ApexVehicleManagementStoredCar;
        storedCar.className = v.ClassName;
        storedCar.displayName = v.DisplayName;
        storedCar.carScriptId = v.CarScriptId;
        storedCar.carKeyId = v.CarKeyId;
        BindNameOntoPlayerKeys(player, v.CarKeyId, v.DisplayName);
        storedCar.playerId = playerID;
        storedCar.playerName = playerName;
        storedCar.timestamp = MF_Helper.GetUnixTime();
        storedCar.date = MF_Helper.GetDate();
        storedCar.lastPosition = v.LastPosition;
        CaptureTransportPreview(transport, storedCar);
		if (!UseMongoDB())
		{
			v.Save(v.CarScriptId);
			m_StoredCars.StoredCars.Insert(storedCar);
			RebuildKeyNameIndex();
			SaveJsonIndex();
			AR_VehicleLock.SetDeletionByMod(transport);
			GetGame().ObjectDelete(transport);
			MF_Helper.SendMessageToClient(player, "Vehicle stored.");
			return true;
		}
        ref AVM_MongoDocument doc = new AVM_MongoDocument;
        doc.RecordId = v.CarScriptId.ToString();
        doc.Meta = storedCar;
        doc.Payload = v;
        m_PendingPark.Set(v.CarScriptId, doc);
        if (m_DB.Save(doc.RecordId, doc, m_Callbacks, "OnParkSaved") == -1) {
            m_PendingPark.Remove(v.CarScriptId);
            return false;
        }
        MF_Helper.SendMessageToClient(player, "Saving vehicle to shared garage...");
        return true;
	}

	static protected bool PlayerOwnsOrHasKey(PlayerBase player, string playerId, int carKeyId)
	{
		if (!player)
			return false;
		if (player.GetIdentity() && player.GetIdentity().GetPlainId() == playerId)
			return true;
		return AVM_Helper.PlayerHasKeyId(player, carKeyId);
	}

	protected static void FindVehicle(string fileName)
	{
		//m_ARKeyLogger.LogARKeyActivity("[ApexVehicleManagement] Found file " + fileName);
		TStringArray strs = new TStringArray;
		fileName.Split(".", strs);		
		if(strs.Count() > 2 || strs.Count() == 0)
		{
			return;
		}
		int fileId = strs[0].ToInt();
		//m_ARKeyLogger.LogARKeyActivity("[ApexVehicleManagement] Found fileId " + fileId);
		bool found = false;
		foreach (ApexVehicleManagementStoredCar storedCar : m_StoredCars.StoredCars) 
		{
			if (storedCar.carScriptId == fileId) 
			{				
				found = true;
				return;
			}
		}
		if(!found)
		{
			if(m_VehiclesFilesToRemove.Find(fileId) == -1)
			{
				//m_ARKeyLogger.LogARKeyActivity("[ApexVehicleManagement] Not Found fileId " + fileId);
				m_VehiclesFilesToRemove.Insert(fileId);
			}
		}
	}

	protected static void DeleteOldVehicleFiles()
	{
		string fileName;
		FileAttr fileAttr;
		FindFileHandle vehicles = FindFile(ApexVehicleManagementSettings.VEHICLES_FOLDER + "/*.json", fileName, fileAttr, 0);
		m_VehiclesFilesToRemove.Clear();
		if (!vehicles)
			return;

		FindVehicle(fileName);

		while(FindNextFile(vehicles, fileName, fileAttr))
		{
			FindVehicle(fileName);
		}
		for(int i = 0; i < m_VehiclesFilesToRemove.Count(); i++)
		{
			string filename = ApexVehicleManagementSettings.VEHICLES_FOLDER + m_VehiclesFilesToRemove[i].ToString() + ".json";
			m_ARKeyLogger.LogARKeyActivity("[ApexVehicleManagement] Deleting old vehicle file " + filename);
			DeleteFile(filename);
		}
	}

	/**
	 * Deletes expired vehicles from the ApexVehicleManagement stored cars map.
	 * A vehicle is considered expired if its timestamp is older than the stored car lifetime setting.
	 * The function iterates through all stored cars and removes the expired ones from the map.
	 * It also sets the store as dirty to ensure it gets saved to the database.
	 */
    protected static void DeleteExpiredVehicles()
	{
		if (UseMongoDB())
			return;
		if (!g_Game.GetApexVehicleManagementSettings().DeleteExpiredCars)
			return;
		int now = MF_Helper.GetUnixTime();
		int lifetime = g_Game.GetApexVehicleManagementSettings().StoredCarLifetime;
		for (int i = m_StoredCars.StoredCars.Count() - 1; i >= 0; i--)
		{
			ApexVehicleManagementStoredCar car = m_StoredCars.StoredCars.Get(i);
			if (!car)
				continue;
			if (car.timestamp + lifetime >= now)
				continue;
			ApexVehicleManagementVehicle payload = new ApexVehicleManagementVehicle;
			payload.Delete(car.carScriptId);
			m_StoredCars.StoredCars.Remove(i);
			m_StoreIsDirty = true;
			m_ARKeyLogger.LogARKeyActivity("[ApexVehicleManagement] Expired stored vehicle " + car.carScriptId.ToString());
		}
	}

	/**
	 * Sends a list of parked cars to a target player. The list includes the IDs and class names of all cars
	 * owned by the sending player, as well as the ID and class name of any car the sending player is holding
	 * the key for.
	 *
	 * @param sender The player sending the parked car list.
	 * @param target The object the player interacted with.
	 */
	static void SendParkedList(notnull PlayerBase sender, Object target) 
	{
		if (!sender || !sender.GetIdentity() || !target)
			return;
		vector targetPos = target.GetMemoryPointPos("parking_pos");
		targetPos = GetGame().ObjectModelToWorld(target, targetPos);
		vector targetOri = target.GetOrientation();
		if(target.IsInherited(AVM_ParkingMeter))
			targetOri = targetOri + "90 0 0";
		else if(target.IsInherited(AVM_HND_ParkingMeter))
			targetOri = targetOri + "270 0 0";
		else if(target.IsInherited(AVM_HND_ParkingBay))
			targetOri = targetOri + "180 0 0";
		if (!UseMongoDB())
		{
			AVM_PendingGarageOpen localOpen = new AVM_PendingGarageOpen;
			localOpen.Identity = sender.GetIdentity();
			localOpen.Position = targetPos;
			localOpen.Orientation = targetOri;
			SendParkedListReady(localOpen);
			return;
		}
		QueueGarageOpen(sender.GetIdentity(), targetPos, targetOri);
	}

	static void SendParkedListReady(AVM_PendingGarageOpen pending)
	{
		if (!pending || !pending.Identity)
			return;
		PlayerBase sender = PlayerBase.Cast(pending.Identity.GetPlayer());
		if (!sender)
			return;
		array<int> inventoryKeys = new array<int>;
		bool hasMaster;
		AVM_Helper.CollectInventoryKeys(sender, inventoryKeys, hasMaster);
		BindAllPlayerKeyNames(sender);
		AVM_TransferData filtered = m_StoredCars.FilterByKeyIdOrPlayerId(pending.Identity.GetPlainId(), inventoryKeys);
		filtered.Position = pending.Position;
		filtered.Orientation = pending.Orientation;
		filtered.CarsStoredCount = m_StoredCars.GetCountOfStoredCarPerPlayerId(pending.Identity.GetPlainId());
		Transport nearby = AVM_Helper.GetNearbyTransport(pending.Position, 4);
		if (nearby)
		{
			ref ApexVehicleManagementStoredCar liveCar = new ApexVehicleManagementStoredCar;
			liveCar.playerName = AR_VehicleLock.GetOriginalOwnerName(nearby);
			liveCar.date = "World";
			CaptureTransportPreview(nearby, liveCar);
			filtered.NearbyParkCar = m_StoredCars.ToClient(liveCar);
			if (filtered.NearbyParkCar)
			{
				filtered.NearbyParkCar.storedDate = "World";
				filtered.NearbyParkCar.expiryDate = "N/A";
			}
		}
		Param1<AVM_TransferData> param = new Param1<AVM_TransferData>(filtered);
		GetGame().RPCSingleParam(sender, AVM_RPCs.RPC_SEND_PARKEDCARS, param, true, pending.Identity);
	}

	/**
	 * RPC_GetVehicle - Retrieves a vehicle from storage and spawns it in the world
	 *
	 * @param sender: The identity of the player requesting the vehicle
	 * @param ctx: The context of the parameters being passed in
	 *
	 * @return: Returns true if the vehicle was successfully retrieved and spawned, false otherwise
	 */
    static bool RPC_GetVehicle(PlayerIdentity sender, ParamsReadContext ctx)
    {
        Param3<int, vector, vector> args = new Param3<int, vector, vector>(-1, "0 0 0", "0 0 0");
        if (!ctx.Read(args) || !m_IndexReady || m_StoredCars.FindCar(args.param1) == -1) return false;
        if (m_PendingRetrieve.Contains(args.param1)) return false;
        PlayerBase player = PlayerBase.Cast(sender.GetPlayer());
        if (!player) return false;
        int idx = m_StoredCars.FindCar(args.param1);
        ApexVehicleManagementStoredCar car = m_StoredCars.StoredCars[idx];
        if (!PlayerOwnsOrHasKey(player, car.playerId, car.carKeyId)) return false;
		if (!UseMongoDB())
		{
			ApexVehicleManagementVehicle payload = new ApexVehicleManagementVehicle;
			payload.Load(args.param1);
			if (payload.ClassName == "")
				return false;
			if (!payload.RestoreVehicle(args.param2, args.param3, player))
				return false;
			payload.Delete(args.param1);
			RemoveIndexedCar(args.param1);
			RebuildKeyNameIndex();
			SaveJsonIndex();
			return true;
		}
        m_PendingRetrieve.Set(args.param1, sender);
        m_PendingPosition.Set(args.param1, args.param2);
        m_PendingOrientation.Set(args.param1, args.param3);
        if (m_DB.Load(args.param1.ToString(), m_Callbacks, "OnRetrieveLoaded") == -1) {
            m_PendingRetrieve.Remove(args.param1);
            return false;
        }
        return true;
    }

    static bool RPC_DeleteVehicle(PlayerIdentity sender, ParamsReadContext ctx)
    {
        Param1<int> args = new Param1<int>(-1);
        if (!ctx.Read(args) || !m_IndexReady)
            return false;
        int id = args.param1;
        if (m_PendingDelete.Contains(id))
            return false;
        PlayerBase player = PlayerBase.Cast(sender.GetPlayer());
        if (!player)
            return false;
        int idx = m_StoredCars.FindCar(id);
        if (idx == -1)
            return false;
        ApexVehicleManagementStoredCar car = m_StoredCars.StoredCars[idx];
        if (!PlayerOwnsOrHasKey(player, car.playerId, car.carKeyId))
            return false;
		if (!UseMongoDB())
		{
			ApexVehicleManagementVehicle payload = new ApexVehicleManagementVehicle;
			payload.Delete(id);
			RemoveIndexedCar(id);
			RebuildKeyNameIndex();
			SaveJsonIndex();
			return true;
		}
        m_PendingDelete.Set(id, sender);
        if (m_DB.Load(id.ToString(), m_Callbacks, "OnDeleteLoaded") == -1)
        {
            m_PendingDelete.Remove(id);
            return false;
        }
        return true;
    }

    static void DeleteLoaded(int id, int status, AVM_MongoDocument doc)
    {
        PlayerIdentity identity;
        if (!m_PendingDelete.Find(id, identity))
            return;
        m_PendingDelete.Remove(id);
        if (status != UAPI_SUCCESS || !doc || !doc.Meta)
            return;
        PlayerBase player = PlayerBase.Cast(identity.GetPlayer());
        if (!player)
            return;
        if (!PlayerOwnsOrHasKey(player, doc.Meta.playerId, doc.Meta.carKeyId))
            return;
        m_DB.Update(id.ToString(), "Status", "deleted", UpdateOpts.SET);
        RemoveIndexedCar(id);
        RebuildKeyNameIndex();
        Print("[AVM] deleted vehicle " + id.ToString() + " from MongoDB");
    }

	/**
	 * This function is called when the server initiates a save so we can save at the same time.
	 */
	static void OnStoreSave() 
	{
		if (!UseMongoDB())
		{
			DeleteExpiredVehicles();
			if (m_StoreIsDirty)
			{
				SaveJsonIndex();
				DeleteOldVehicleFiles();
				m_StoreIsDirty = false;
			}
			return;
		}
	}
}

#endif
#ifdef SERVER
class ApexVehicleManagementMongoCallbacks
{
    void OnIndex(int cid, int status, string oid, UApiQueryResult<AVM_MongoDocument> result)
    {
        ApexVehicleManagementManager.IndexResult(status, result);
    }
    void OnParkSaved(int cid, int status, string oid, AVM_MongoDocument data)
    {
        ApexVehicleManagementManager.ParkResult(oid.ToInt(), status);
    }
    void OnRetrieveLoaded(int cid, int status, string oid, AVM_MongoDocument data)
    {
        ApexVehicleManagementManager.RetrieveResult(oid.ToInt(), status, data);
    }
    void OnClaim(int cid, int status, string oid, string data)
    {
        ApexVehicleManagementManager.ClaimResult(status, oid, data);
    }
    void OnVerifyClaim(int cid, int status, string oid, AVM_MongoDocument data)
    {
        ApexVehicleManagementManager.VerifyClaim(oid.ToInt(), status, data);
    }
    void OnDeleteLoaded(int cid, int status, string oid, AVM_MongoDocument data)
    {
        ApexVehicleManagementManager.DeleteLoaded(oid.ToInt(), status, data);
    }
}
#endif
