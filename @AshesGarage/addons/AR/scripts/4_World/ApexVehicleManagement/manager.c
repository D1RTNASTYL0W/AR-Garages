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
    float previewHealth = -1;
    float previewFuel = -1;
};

// Read-only DTO for the EXISTING MuchFramework inventory JSON. Never write this DTO
// back to MongoDB: the original Payload remains the authoritative restore source.
class AVM_PreviewNode
{
    string typeName;
    float health;
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
    float previewHealth = -1;
    float previewFuel = -1;
    string storedDate;
};
class AVM_TransferData
{
	vector Position;
	vector Orientation;
	int CarsStoredCount = -1;
	ref array<ref AVM_StoredCarClient> StoredCars;
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
	
	AVM_TransferData FilterByKeyIdOrPlayerId(string PlayerId, int CarKeyId)
	{
		ref array<ref AVM_StoredCarClient> filtered = new array<ref AVM_StoredCarClient>;
		for(int i = 0; i < StoredCars.Count(); i++)
		{
			ApexVehicleManagementStoredCar storedCar = StoredCars.Get(i);
			if(storedCar.playerId == PlayerId || storedCar.carKeyId == CarKeyId)
			{
				ref AVM_StoredCarClient clientInfo = new ref AVM_StoredCarClient;
				clientInfo.className = storedCar.className;
                clientInfo.displayName = storedCar.displayName;
				clientInfo.playerName = storedCar.playerName;
				clientInfo.carScriptId = storedCar.carScriptId;
				clientInfo.lastPosition = storedCar.lastPosition;
                clientInfo.previewAttachments = storedCar.previewAttachments;
                clientInfo.previewCargo = storedCar.previewCargo;
                clientInfo.previewHealth = storedCar.previewHealth;
                clientInfo.previewFuel = storedCar.previewFuel;
                clientInfo.storedDate = storedCar.date;
				if(g_Game.GetApexVehicleManagementSettings().DeleteExpiredCars)
				{
					clientInfo.expiryDate = MF_Helper.SecondsToDays(storedCar.timestamp + g_Game.GetApexVehicleManagementSettings().StoredCarLifetime - MF_Helper.GetUnixTime());
				}
				else
				{
					clientInfo.expiryDate = "N/A";
				}
				
				filtered.Insert(clientInfo);
			}
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
			{
				ref AVM_StoredCarClient clientInfo = new ref AVM_StoredCarClient;
				clientInfo.className = storedCar.className;
                clientInfo.className = storedCar.className;
                clientInfo.displayName = storedCar.displayName;
				clientInfo.playerName = storedCar.playerName;
				clientInfo.carScriptId = storedCar.carScriptId;
				if(g_Game.GetApexVehicleManagementSettings().DeleteExpiredCars)
				{
					clientInfo.expiryDate = MF_Helper.SecondsToDays(storedCar.timestamp + g_Game.GetApexVehicleManagementSettings().StoredCarLifetime - MF_Helper.GetUnixTime());
				}
				else
				{
					clientInfo.expiryDate = "N/A";
				}
				
				filtered.Insert(clientInfo);
			}
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

/***********************
 * Manager */
class ApexVehicleManagementManager 
{
	protected static ref ApexVehicleManagementStoredData m_StoredCars = new ApexVehicleManagementStoredData;
	protected static ref array<int> m_VehiclesToRemove = new array<int>;
	protected static ref array<int> m_VehiclesFilesToRemove = new array<int>;
	protected static int m_LastStoreTime;
	protected static bool m_StoreIsDirty = false;
	protected static PluginMCKLogs m_MCKLogger;
    protected static ref UApiDBHandler<AVM_MongoDocument> m_DB;
    protected static ref ApexVehicleManagementMongoCallbacks m_Callbacks;
    protected static ref array<ref AVM_MongoDocument> m_Records = new array<ref AVM_MongoDocument>;
    protected static ref map<int, ref AVM_MongoDocument> m_PendingPark = new map<int, ref AVM_MongoDocument>;
    protected static ref map<int, PlayerIdentity> m_PendingRetrieve = new map<int, PlayerIdentity>;
    protected static ref map<int, vector> m_PendingPosition = new map<int, vector>;
    protected static ref map<int, vector> m_PendingOrientation = new map<int, vector>;
    protected static bool m_IndexReady = false;
    protected static bool m_IndexBusy = false;

	/**
	 * Initializes the ApexVehicleManagement manager by creating the necessary directory if it does not exist,
	 * loading stored cars, and registering inbound and outbound RPC endpoints.
	 */
	static void Init() 
	{
		if (!FileExist(ApexVehicleManagementSettings.FOLDER))
			MakeDirectory(ApexVehicleManagementSettings.FOLDER);		
		m_MCKLogger = PluginMCKLogs.Cast(GetPlugin(PluginMCKLogs));
		m_DB = new UApiDBHandler<AVM_MongoDocument>("ApexVehicleManagement", OBJECT_DB);
        m_Callbacks = new ApexVehicleManagementMongoCallbacks();
        RefreshIndex();
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
        if (m_IndexBusy) return;
        m_IndexBusy = true;
        m_IndexReady = false;
        int callId = m_DB.Query("{}", m_Callbacks, "OnIndex");
        if (callId == -1) m_IndexBusy = false;
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
        meta.previewHealth = -1;
        string payloadJson = JsonFileLoader<ApexVehicleManagementVehicle>.JsonMakeData(doc.Payload);
        ref AVM_PreviewPayload view = new AVM_PreviewPayload;
        JsonFileLoader<AVM_PreviewPayload>.JsonLoadData(payloadJson, view);
        if (!view || !view.inventory) return;
        // Absolute item health is not a percentage; avoid misleading condition bars.
        if (view.inventory.attachments)
        {
            foreach (AVM_PreviewNode part : view.inventory.attachments)
            {
                if (part && part.typeName != "") meta.previewAttachments.Insert(part.typeName);
            }
        }
        if (view.inventory.cargo)
        {
            foreach (AVM_PreviewNode item : view.inventory.cargo)
            {
                if (item && item.typeName != "")
                {
                    meta.previewCargo.Insert(item.typeName);
                    // Indicate nested contents without treating them as direct vehicle cargo.
                    if (item.attachments)
                    {
                        foreach (AVM_PreviewNode nested : item.attachments)
                        {
                            if (nested && nested.typeName != "")
                                meta.previewCargo.Insert("  + " + nested.typeName);
                        }
                    }
                }
            }
        }
    }

    // Universal API queries may deliver only a subset of documents per refresh.
    // Keep previously observed stored vehicles, keyed by immutable carScriptId,
    // rather than replacing the entire index with the latest partial response.
    // Explicit retrieval removes an entry; returned non-stored records also remove it.
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
            Error("[AVM] MongoDB index query failed; retaining last known index");
            return;
        }
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
                {
                    RemoveIndexedCar(vehicleId);
                    continue;
                }
                BuildPreviewFromPayload(stableDoc);
                RemoveIndexedCar(vehicleId);
                m_Records.Insert(stableDoc);
                m_StoredCars.StoredCars.Insert(stableDoc.Meta);
                Print("[AVM INDEX] id=" + vehicleId.ToString() + " class=" + stableDoc.Meta.className + " name=" + stableDoc.Meta.displayName);
            }
        }
        m_IndexReady = true;
        Print("[AVM INDEX] query documents=" + received.ToString() + " cached stored=" + m_StoredCars.StoredCars.Count().ToString());
    }

    static void ParkResult(int id, int status)
    {
        AVM_MongoDocument doc;
        if (!m_PendingPark.Find(id, doc)) return;
        m_PendingPark.Remove(id);
        if (status != UAPI_SUCCESS) {
            Error("[AVM] MongoDB save failed for vehicle " + id + "; physical vehicle was NOT deleted");
            return;
        }
        // Only delete the world vehicle after the backend acknowledges the save.
        Transport vehicle = AVM_Helper.GetNearbyTransport(doc.Meta.lastPosition, 5);
        if (!vehicle || MCK_Transport.GetTransportId(vehicle) != id) {
            Error("[AVM] Saved car is no longer at original location; manual recovery needed: " + id);
            return;
        }
        MCK_Transport.SetDeletionByMod(vehicle);
        GetGame().ObjectDelete(vehicle);
        RefreshIndex();
    }

    static void RetrieveResult(int id, int status, AVM_MongoDocument doc)
    {
        PlayerIdentity identity;
        if (!m_PendingRetrieve.Find(id, identity)) return;
        if (status != UAPI_SUCCESS || !doc || !doc.Payload || doc.Status != "stored") {
            m_PendingRetrieve.Remove(id);
            return;
        }
        PlayerBase player = PlayerBase.Cast(identity.GetPlayer());
        if (!player || (doc.Meta.playerId != identity.GetPlainId() && KeyInHand(player) != doc.Meta.carKeyId)) {
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
            RefreshIndex();
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
				if(player.GetItemInHands() && player.GetItemInHands().IsInherited(MCK_MasterKey))
				{
					
				}
				else
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
        storedCar.playerId = playerID;
        storedCar.playerName = playerName;
        storedCar.timestamp = MF_Helper.GetUnixTime();
        storedCar.date = MF_Helper.GetDate();
        storedCar.lastPosition = v.LastPosition;
        // Snapshot actual attached parts and direct cargo BEFORE deleting the vehicle.
        // Never reconstruct or retrieve the stored vehicle just to preview it.
        storedCar.previewAttachments = new array<string>;
        storedCar.previewCargo = new array<string>;
        storedCar.previewHealth = transport.GetHealth01("", "");
        storedCar.previewFuel = -1;
        CarScript parkedCar = CarScript.Cast(transport);
        if (parkedCar)
            storedCar.previewFuel = parkedCar.GetFluidFraction(CarFluid.FUEL);
        if (transport.GetInventory())
        {
            for (int attachmentIndex = 0; attachmentIndex < transport.GetInventory().AttachmentCount(); attachmentIndex++)
            {
                EntityAI part = transport.GetInventory().GetAttachmentFromIndex(attachmentIndex);
                if (part) storedCar.previewAttachments.Insert(part.GetType());
            }
            CargoBase cargo = transport.GetInventory().GetCargo();
            if (cargo)
            {
                for (int cargoIndex = 0; cargoIndex < cargo.GetItemCount(); cargoIndex++)
                {
                    EntityAI cargoItem = cargo.GetItem(cargoIndex);
                    if (cargoItem) storedCar.previewCargo.Insert(cargoItem.GetType());
                }
            }
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

	/**
	 * Returns the MCK ID of the car key in the player's hands, if any.
	 *
	 * @param player The player whose hands to check for a car key.
	 * @return The MCK ID of the car key in the player's hands, or 0 if no car key is found.
	 */
	static protected int KeyInHand(notnull PlayerBase player) 
	{
		MCK_CarKey_Base carKey = MCK_CarKey_Base.Cast(player.GetItemInHands());
		if (!carKey)
			return -1;

		return carKey.m_MCKId;
	}

	protected static void FindVehicle(string fileName)
	{
		//m_MCKLogger.LogMCKActivity("[ApexVehicleManagement] Found file " + fileName);
		TStringArray strs = new TStringArray;
		fileName.Split(".", strs);		
		if(strs.Count() > 2 || strs.Count() == 0)
		{
			return;
		}
		int fileId = strs[0].ToInt();
		//m_MCKLogger.LogMCKActivity("[ApexVehicleManagement] Found fileId " + fileId);
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
				//m_MCKLogger.LogMCKActivity("[ApexVehicleManagement] Not Found fileId " + fileId);
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
			m_MCKLogger.LogMCKActivity("[ApexVehicleManagement] Deleting old vehicle file " + filename);
			DeleteFile(filename);
		}
	}

	/**
	 * Deletes expired vehicles from the ApexVehicleManagement stored cars map.
	 * A vehicle is considered expired if its timestamp is older than the stored car lifetime setting.
	 * The function iterates through all stored cars and removes the expired ones from the map.
	 * It also sets the store as dirty to ensure it gets saved to the database.
	 */
    protected static void DeleteExpiredVehicles() { /* MongoDB expiry requires coordinated deletion; disabled in MVP. */ }

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
		if (!m_IndexReady) {
            MF_Helper.SendMessageToClient(sender, "Garage database loading. Please try again.");
            if (!m_IndexBusy) RefreshIndex();
            return;
        }
        int inHand = KeyInHand(sender);
		AVM_TransferData filtered = m_StoredCars.FilterByKeyIdOrPlayerId(sender.GetIdentity().GetPlainId(), inHand);		
		vector targetPos = target.GetMemoryPointPos("parking_pos");
		targetPos = GetGame().ObjectModelToWorld(target, targetPos);		
		vector targetOri = target.GetOrientation();
		if(target.IsInherited(AVM_ParkingMeter))
		{
			targetOri = targetOri + "90 0 0";
		}
		else if(target.IsInherited(AVM_HND_ParkingMeter))
		{				
			targetOri = targetOri + "270 0 0";
		}
		else if(target.IsInherited(AVM_HND_ParkingBay))
		{
			targetOri = targetOri + "180 0 0";
		}
		filtered.Position = targetPos;
		filtered.Orientation = targetOri;
		filtered.CarsStoredCount = m_StoredCars.GetCountOfStoredCarPerPlayerId(sender.GetIdentity().GetPlainId());
		Param1<AVM_TransferData> param = new Param1<AVM_TransferData>( filtered );
		GetGame().RPCSingleParam(sender, AVM_RPCs.RPC_SEND_PARKEDCARS, param, true, sender.GetIdentity());
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
        if (car.playerId != sender.GetPlainId() && car.carKeyId != KeyInHand(player)) return false;
        m_PendingRetrieve.Set(args.param1, sender);
        m_PendingPosition.Set(args.param1, args.param2);
        m_PendingOrientation.Set(args.param1, args.param3);
        if (m_DB.Load(args.param1.ToString(), m_Callbacks, "OnRetrieveLoaded") == -1) {
            m_PendingRetrieve.Remove(args.param1);
            return false;
        }
        return true;
    }

	/**
	 * This function is called when the server initiates a save so we can save at the same time.
	 */
	static void OnStoreSave() 
	{
        // MongoDB writes happen per vehicle, immediately, via asynchronous callbacks.
        if (!m_IndexBusy) RefreshIndex();
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
}
#endif
