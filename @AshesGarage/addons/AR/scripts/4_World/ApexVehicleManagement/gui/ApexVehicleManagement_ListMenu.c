#ifndef SERVER
const int MENU_AVM_GARAGEMENU = -551210;

class AVM_GarageMenu : UIScriptedMenu 
{
	protected Widget m_Root;
	protected ref array<ref AVM_StoredCarClient> m_Vehicles;
    protected ref array<int> m_VisibleVehicleIndices;
    protected EditBoxWidget m_SearchBox;
    protected ButtonWidget m_ClearSearch;
    protected TextWidget m_NoResults;
    protected ButtonWidget m_FavoritesFilter;
    protected ButtonWidget m_ReverseOrder;
    protected ButtonWidget m_ResetSelection;
    protected ButtonWidget m_InfoToggle;
    protected TextWidget m_FavoritesLabel;
    protected TextWidget m_OrderLabel;
    protected TextWidget m_DetailPosition;
    protected TextWidget m_InfoMessage;
    protected ItemPreviewWidget m_Vehicle3DPreview;
    protected TextWidget m_PreviewStatus;
    protected EntityAI m_PreviewEntity;
    protected ref array<ItemPreviewWidget> m_PartsPreviews;
    protected ref array<ItemPreviewWidget> m_CargoPreviews;
    protected ref array<TextWidget> m_PartsLabels;
    protected ref array<TextWidget> m_CargoLabels;
    protected ref array<EntityAI> m_TileEntities;
    protected ButtonWidget m_PreviewBack;
    protected bool m_IsDraggingPreview = false;
    protected int m_LastDragX;
    protected float m_PreviewYaw = 0;
    protected float m_PreviewZoom = 1.0;
    protected bool m_InspectingItem = false;

    protected ref array<int> m_FavoriteIds;
    protected bool m_FavoritesOnly = false;
    protected bool m_Reverse = false;
    protected bool m_InfoVisible = true;
    protected ref array<ButtonWidget> m_VehicleButtons;
    protected ref array<Widget> m_RowCards;
    protected ref array<Widget> m_RowAccents;
    protected ref array<TextWidget> m_RowLabels;
    protected ref array<TextWidget> m_RowMetas;
    protected ref array<TextWidget> m_RowIds;
    protected ref array<TextWidget> m_RowExpiries;
    protected ref array<ButtonWidget> m_RowStars;
    protected ref array<ItemPreviewWidget> m_RowPreviews;
    protected ref array<EntityAI> m_RowPreviewEntities;
    protected ref array<TextWidget> m_PartsQtys;
    protected ref array<TextWidget> m_CargoQtys;
    protected ref array<Widget> m_ActionPanels;
    protected ref array<string> m_PartsNames;
    protected ref array<int> m_PartsCounts;
    protected ref array<string> m_CargoNames;
    protected ref array<int> m_CargoCounts;
    protected Widget m_TabFavBg;
    protected Widget m_TabRecentBg;
    protected Widget m_InvTabAttachBg;
    protected Widget m_InvTabCargoBg;
    protected TextWidget m_InvTabAttachLabel;
    protected TextWidget m_InvTabCargoLabel;
    protected ButtonWidget m_InvTabAttach;
    protected ButtonWidget m_InvTabCargo;
    protected Widget m_PartsPanel;
    protected Widget m_CargoPanel;
    protected bool m_ShowCargoTab = false;
    protected Widget m_SearchPlaceholder;
    protected ButtonWidget m_TitleFavorite;
    protected ButtonWidget m_SpawnBtn;
    protected ButtonWidget m_DeleteBtn;
    protected TextWidget m_StoredDate;
    protected TextWidget m_TagOffroad;
    protected TextWidget m_TagPersonal;
    protected ref array<Widget> m_StatFills;
    protected ref array<TextWidget> m_StatValues;
    protected int m_PartsPage = 0;
    protected int m_CargoPage = 0;
    protected ButtonWidget m_PartsPrevious;
    protected ButtonWidget m_PartsNext;
    protected ButtonWidget m_CargoPrevious;
    protected ButtonWidget m_CargoNext;
    protected int m_SelectedVehicleIndex = -1;
    protected int m_Page = 0;
    protected static const int VEHICLES_PER_PAGE = 2;
    protected static const int TILES_PER_PAGE = 6;
    protected ButtonWidget m_PrevPage;
    protected ButtonWidget m_NextPage;
    protected TextWidget m_PageInfo;
    protected TextWidget m_ResultCount;
    protected TextWidget m_DetailTitle;
    protected TextWidget m_DetailOwner;
    protected TextWidget m_DetailId;
    protected TextWidget m_DetailExpiry;
	protected TextWidget m_ParkVehicleList;
	protected vector m_TargetPosition;
	protected vector m_TargetOrientation;
    private ButtonWidget m_BtnClose;
    private ButtonWidget m_BtnConfirm;
    private TextWidget m_ConfirmLabel;
    private ButtonWidget m_BtnRetrieve;
    private ButtonWidget m_BtnPark;
    private TextWidget m_ErrorText;
    private TextWidget m_CarsStoredText;
    private TextWidget m_MaxCarsText;
    private Widget m_MaxCarsPanel;
    private Widget m_ErrorPanel;
	private bool m_RetrieveMode = true;
	private int m_CarsStoredCount = -1;
	private int m_MaxCarsAllowed = -1;
	private bool m_StoreCargoAllowed = false;

	override Widget Init() 
	{
		layoutRoot = GetGame().GetWorkspace().CreateWidgets("AR/gui/list.layout");
		m_Root = layoutRoot;
        // Native PAA background; the image widget is behind all interactive controls.
        ImageWidget avmBackdrop = ImageWidget.Cast(layoutRoot.FindAnyWidget("AVMBackdrop"));
        if (avmBackdrop)
        {
            avmBackdrop.LoadImageFile(0, "AR/gui/textures/ar_background.paa");
            avmBackdrop.SetImage(0);
            // Darken the PAA without changing the original texture.
            avmBackdrop.SetColor(ARGB(255, 65, 75, 88));
        }
		g_Game.SetKeyboardHandle(this);
		m_Vehicles = new array<ref AVM_StoredCarClient>;
        m_FavoriteIds = new array<int>;
        m_VisibleVehicleIndices = new array<int>;
        m_SearchBox = EditBoxWidget.Cast(m_Root.FindAnyWidget("ARSearch"));
        m_ClearSearch = ButtonWidget.Cast(m_Root.FindAnyWidget("ARClearSearch"));
        m_NoResults = TextWidget.Cast(m_Root.FindAnyWidget("ARNoResults"));
        m_FavoritesFilter = ButtonWidget.Cast(m_Root.FindAnyWidget("ARFavoritesFilter"));
        m_ReverseOrder = ButtonWidget.Cast(m_Root.FindAnyWidget("ARReverseOrder"));
        m_ResetSelection = ButtonWidget.Cast(m_Root.FindAnyWidget("ARResetSelection"));
        m_InfoToggle = ButtonWidget.Cast(m_Root.FindAnyWidget("ARInfoToggle"));
        m_FavoritesLabel = TextWidget.Cast(m_Root.FindAnyWidget("ARFavoritesLabel"));
        m_OrderLabel = TextWidget.Cast(m_Root.FindAnyWidget("AROrderLabel"));
        m_DetailPosition = TextWidget.Cast(m_Root.FindAnyWidget("ARDetailPosition"));
        m_InfoMessage = TextWidget.Cast(m_Root.FindAnyWidget("ARInfoMessage"));
        m_Vehicle3DPreview = ItemPreviewWidget.Cast(m_Root.FindAnyWidget("ARVehicle3DPreview"));
        m_PreviewStatus = TextWidget.Cast(m_Root.FindAnyWidget("ARPreviewStatus"));
        m_PreviewBack = ButtonWidget.Cast(m_Root.FindAnyWidget("ARPreviewBack"));
        m_SearchPlaceholder = m_Root.FindAnyWidget("ARSearchPlaceholder");
        m_TitleFavorite = ButtonWidget.Cast(m_Root.FindAnyWidget("ARTitleFavorite"));
        m_SpawnBtn = ButtonWidget.Cast(m_Root.FindAnyWidget("ARSpawnBtn"));
        m_DeleteBtn = ButtonWidget.Cast(m_Root.FindAnyWidget("ARDeleteBtn"));
        m_StoredDate = TextWidget.Cast(m_Root.FindAnyWidget("AVMVehicleStored"));
        m_TagOffroad = TextWidget.Cast(m_Root.FindAnyWidget("ARTagOffroadText"));
        m_TagPersonal = TextWidget.Cast(m_Root.FindAnyWidget("ARTagPersonalText"));
        m_TabFavBg = m_Root.FindAnyWidget("ARTabFavBg");
        m_TabRecentBg = m_Root.FindAnyWidget("ARTabRecentBg");
        m_InvTabAttachBg = m_Root.FindAnyWidget("ARInvTabAttachBg");
        m_InvTabCargoBg = m_Root.FindAnyWidget("ARInvTabCargoBg");
        m_InvTabAttachLabel = TextWidget.Cast(m_Root.FindAnyWidget("ARInvTabAttachLabel"));
        m_InvTabCargoLabel = TextWidget.Cast(m_Root.FindAnyWidget("ARInvTabCargoLabel"));
        m_InvTabAttach = ButtonWidget.Cast(m_Root.FindAnyWidget("ARInvTabAttach"));
        m_InvTabCargo = ButtonWidget.Cast(m_Root.FindAnyWidget("ARInvTabCargo"));
        m_PartsPanel = m_Root.FindAnyWidget("ARPartsPanel");
        m_CargoPanel = m_Root.FindAnyWidget("ARCargoPanel");
        m_PartsPreviews = new array<ItemPreviewWidget>;
        m_CargoPreviews = new array<ItemPreviewWidget>;
        m_PartsLabels = new array<TextWidget>;
        m_CargoLabels = new array<TextWidget>;
        m_PartsQtys = new array<TextWidget>;
        m_CargoQtys = new array<TextWidget>;
        m_TileEntities = new array<EntityAI>;
        m_PartsNames = new array<string>;
        m_PartsCounts = new array<int>;
        m_CargoNames = new array<string>;
        m_CargoCounts = new array<int>;
        m_StatFills = new array<Widget>;
        m_StatValues = new array<TextWidget>;
        for (int si = 0; si < 5; si++)
        {
            m_StatFills.Insert(m_Root.FindAnyWidget("ARStatFill" + si.ToString()));
            m_StatValues.Insert(TextWidget.Cast(m_Root.FindAnyWidget("ARStatValue" + si.ToString())));
        }
        for (int ti = 0; ti < TILES_PER_PAGE; ti++)
        {
            m_PartsPreviews.Insert(ItemPreviewWidget.Cast(m_Root.FindAnyWidget("ARPartsPreview" + ti.ToString())));
            m_CargoPreviews.Insert(ItemPreviewWidget.Cast(m_Root.FindAnyWidget("ARCargoPreview" + ti.ToString())));
            m_PartsLabels.Insert(TextWidget.Cast(m_Root.FindAnyWidget("ARPartsLabel" + ti.ToString())));
            m_CargoLabels.Insert(TextWidget.Cast(m_Root.FindAnyWidget("ARCargoLabel" + ti.ToString())));
            m_PartsQtys.Insert(TextWidget.Cast(m_Root.FindAnyWidget("ARPartsQty" + ti.ToString())));
            m_CargoQtys.Insert(TextWidget.Cast(m_Root.FindAnyWidget("ARCargoQty" + ti.ToString())));
        }

        m_PartsPrevious = ButtonWidget.Cast(m_Root.FindAnyWidget("ARPartsPrevious"));
        m_PartsNext = ButtonWidget.Cast(m_Root.FindAnyWidget("ARPartsNext"));
        m_CargoPrevious = ButtonWidget.Cast(m_Root.FindAnyWidget("ARCargoPrevious"));
        m_CargoNext = ButtonWidget.Cast(m_Root.FindAnyWidget("ARCargoNext"));
        m_VehicleButtons = new array<ButtonWidget>;
        m_RowCards = new array<Widget>;
        m_RowAccents = new array<Widget>;
        m_RowLabels = new array<TextWidget>;
        m_RowMetas = new array<TextWidget>;
        m_RowIds = new array<TextWidget>;
        m_RowExpiries = new array<TextWidget>;
        m_RowStars = new array<ButtonWidget>;
        m_RowPreviews = new array<ItemPreviewWidget>;
        m_RowPreviewEntities = new array<EntityAI>;
        m_ActionPanels = new array<Widget>;
        for (int rowIndex = 0; rowIndex < VEHICLES_PER_PAGE; rowIndex++)
        {
            string rowKey = rowIndex.ToString();
            m_VehicleButtons.Insert(ButtonWidget.Cast(m_Root.FindAnyWidget("VehicleRow" + rowKey)));
            m_RowCards.Insert(m_Root.FindAnyWidget("ARRowCard" + rowKey));
            m_RowAccents.Insert(m_Root.FindAnyWidget("ARRowAccent" + rowKey));
            m_RowLabels.Insert(TextWidget.Cast(m_Root.FindAnyWidget("ARRowLabel" + rowKey)));
            m_RowMetas.Insert(TextWidget.Cast(m_Root.FindAnyWidget("ARRowMeta" + rowKey)));
            m_RowIds.Insert(TextWidget.Cast(m_Root.FindAnyWidget("ARRowId" + rowKey)));
            m_RowExpiries.Insert(TextWidget.Cast(m_Root.FindAnyWidget("ARRowExpiry" + rowKey)));
            m_RowStars.Insert(ButtonWidget.Cast(m_Root.FindAnyWidget("ARRowStar" + rowKey)));
            m_RowPreviews.Insert(ItemPreviewWidget.Cast(m_Root.FindAnyWidget("ARRowPreview" + rowKey)));
        }
        m_ActionPanels.Insert(m_Root.FindAnyWidget("ARImageRetrieveBtn"));
        m_ActionPanels.Insert(m_Root.FindAnyWidget("ARImageParkBtn"));
        m_ActionPanels.Insert(m_Root.FindAnyWidget("ARImageConfirmBtn"));
        m_ActionPanels.Insert(m_Root.FindAnyWidget("ARImagePrevPage"));
        m_ActionPanels.Insert(m_Root.FindAnyWidget("ARImageNextPage"));
        m_PrevPage = ButtonWidget.Cast(m_Root.FindAnyWidget("PrevPage"));
        m_NextPage = ButtonWidget.Cast(m_Root.FindAnyWidget("NextPage"));
        m_PageInfo = TextWidget.Cast(m_Root.FindAnyWidget("PageInfo"));
        m_ResultCount = TextWidget.Cast(m_Root.FindAnyWidget("AVMResultCount"));
        m_DetailTitle = TextWidget.Cast(m_Root.FindAnyWidget("AVMVehicleTitle"));
        m_DetailOwner = TextWidget.Cast(m_Root.FindAnyWidget("AVMVehicleOwner"));
        m_DetailId = TextWidget.Cast(m_Root.FindAnyWidget("AVMVehicleId"));
        m_DetailExpiry = TextWidget.Cast(m_Root.FindAnyWidget("AVMVehicleExpiry"));
		m_ParkVehicleList = TextWidget.Cast(m_Root.FindAnyWidget("ParkList"));
		m_BtnClose = ButtonWidget.Cast(layoutRoot.FindAnyWidget("btn_UIClose"));
        m_BtnConfirm = ButtonWidget.Cast(layoutRoot.FindAnyWidget("ConfirmBtn"));
        m_ConfirmLabel = TextWidget.Cast(layoutRoot.FindAnyWidget("ARLabelConfirmBtn"));
		m_BtnRetrieve = ButtonWidget.Cast(layoutRoot.FindAnyWidget("RetrieveBtn"));
		m_BtnPark = ButtonWidget.Cast(layoutRoot.FindAnyWidget("ParkBtn"));    
		m_ErrorText = TextWidget.Cast(layoutRoot.FindAnyWidget( "ErrorText" ));
		m_ErrorPanel = Widget.Cast(layoutRoot.FindAnyWidget( "ErrorPanel" ));
		m_MaxCarsPanel = Widget.Cast(layoutRoot.FindAnyWidget( "MaxStoredCarsSpacer" ));
		m_CarsStoredText = TextWidget.Cast(layoutRoot.FindAnyWidget( "CurrentCountValue" ));
		m_MaxCarsText = TextWidget.Cast(layoutRoot.FindAnyWidget( "MaxAllowedValue" ));
		m_MaxCarsPanel.Show(false);
		RefreshInventoryTab();
		SwitchPage();
		return layoutRoot;
	}

	// return true; idk why
	override bool OnKeyDown(Widget w, int x, int y, int key) 
	{
		switch (key) 
		{
		case KeyCode.KC_ESCAPE:
			Close();
			return true;
		}

		return super.OnKeyDown(w, x, y, key);
	}

	void SetTarget(vector Position, vector Orientation) 
	{
		m_TargetPosition = Position;
		m_TargetOrientation = Orientation;
	}

	void SetCarsStoredCount(int carsStoredCount) 
	{
		m_CarsStoredCount = carsStoredCount;
		m_MaxCarsAllowed = g_Game.GetApexVehicleManagementSettings().MaxStoredCarsPerPlayer;
		m_StoreCargoAllowed = g_Game.GetApexVehicleManagementSettings().StoreCargo;
		if(m_MaxCarsAllowed > 0)
		{
			m_MaxCarsText.SetText(m_MaxCarsAllowed.ToString());
			m_CarsStoredText.SetText(m_CarsStoredCount.ToString());
			m_MaxCarsPanel.Show(true);
		}
	}

    int ColorIdle() { return ARGB(255, 12, 20, 32); }
    int ColorHover() { return ARGB(255, 28, 48, 70); }
    int ColorActive() { return ARGB(255, 30, 115, 200); }
    int ColorCardIdle() { return ARGB(255, 12, 20, 30); }
    int ColorCardHover() { return ARGB(255, 20, 36, 52); }
    int ColorCardSelected() { return ARGB(255, 22, 48, 72); }
    int ColorAccentOn() { return ARGB(255, 38, 184, 242); }
    int ColorAccentOff() { return ARGB(255, 10, 20, 30); }

    void SetPanelColor(Widget panel, int color)
    {
        if (panel)
            panel.SetColor(color);
    }

    int GetActionIndex(Widget w)
    {
        if (w == m_BtnRetrieve) return 0;
        if (w == m_BtnPark) return 1;
        if (w == m_BtnConfirm) return 2;
        if (w == m_PrevPage) return 3;
        if (w == m_NextPage) return 4;
        return -1;
    }

    void SetActionPanel(int index, int color)
    {
        if (index < 0 || index >= m_ActionPanels.Count()) return;
        SetPanelColor(m_ActionPanels.Get(index), color);
    }

    void RefreshActionTextures()
    {
        for (int i = 0; i < m_ActionPanels.Count(); i++)
            SetActionPanel(i, ColorIdle());
        SetActionPanel(2, ARGB(255, 30, 107, 210));
        if (m_RetrieveMode && !m_FavoritesOnly)
            SetActionPanel(0, ColorActive());
        else if (!m_RetrieveMode)
            SetActionPanel(1, ColorActive());
        SetPanelColor(m_TabFavBg, ColorIdle());
        SetPanelColor(m_TabRecentBg, ColorIdle());
        if (m_FavoritesOnly)
            SetPanelColor(m_TabFavBg, ColorActive());
        if (m_Reverse)
            SetPanelColor(m_TabRecentBg, ColorActive());
    }

    void SetCardState(int row, bool selected, bool hover)
    {
        if (row < 0 || row >= m_RowCards.Count()) return;
        if (selected)
            SetPanelColor(m_RowCards.Get(row), ColorCardSelected());
        else if (hover)
            SetPanelColor(m_RowCards.Get(row), ColorCardHover());
        else
            SetPanelColor(m_RowCards.Get(row), ColorCardIdle());
        if (row < m_RowAccents.Count())
        {
            if (selected)
                SetPanelColor(m_RowAccents.Get(row), ColorAccentOn());
            else
                SetPanelColor(m_RowAccents.Get(row), ColorAccentOff());
        }
    }

    override bool OnMouseEnter(Widget w, int x, int y)
    {
        for (int i = 0; i < m_VehicleButtons.Count(); i++)
        {
            if (w == m_VehicleButtons.Get(i) && GetVehicleIndexForRow(i) != m_SelectedVehicleIndex)
            {
                SetCardState(i, false, true);
                return true;
            }
        }
        int actionIndex = GetActionIndex(w);
        if (actionIndex >= 0)
        {
            SetActionPanel(actionIndex, ColorHover());
            return true;
        }
        return super.OnMouseEnter(w, x, y);
    }

    override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
    {
        for (int i = 0; i < m_VehicleButtons.Count(); i++)
        {
            if (w == m_VehicleButtons.Get(i))
            {
                SetCardState(i, GetVehicleIndexForRow(i) == m_SelectedVehicleIndex, false);
                return true;
            }
        }
        int actionIndex = GetActionIndex(w);
        if (actionIndex >= 0)
        {
            RefreshActionTextures();
            return true;
        }
        return super.OnMouseLeave(w, enterW, x, y);
    }

    override bool OnChange(Widget w, int x, int y, bool finished)
    {
        if (w == m_SearchBox)
        {
            m_Page = 0;
            RebuildSearchResults();
            return true;
        }
        return super.OnChange(w, x, y, finished);
    }

    void RebuildSearchResults()
    {
        m_VisibleVehicleIndices.Clear();
        string query = "";
        if (m_SearchBox)
        {
            query = m_SearchBox.GetText();
            query.ToLower();
        }
        for (int i = 0; i < m_Vehicles.Count(); i++)
        {
            AVM_StoredCarClient car = m_Vehicles.Get(i);
            string name = car.displayName;
            string owner = car.playerName;
            string vehicleId = car.carScriptId.ToString();
            name.ToLower();
            owner.ToLower();
            bool matchesText = query == "" || name.Contains(query) || owner.Contains(query) || vehicleId.Contains(query);
            bool matchesFavorite = !m_FavoritesOnly || m_FavoriteIds.Find(car.carScriptId) != -1;
            if (matchesText && matchesFavorite)
                m_VisibleVehicleIndices.Insert(i);
        }
        if (m_SearchPlaceholder)
            m_SearchPlaceholder.Show(!m_SearchBox || m_SearchBox.GetText() == "");
        RefreshActionTextures();
        RefreshVehicleRows();
    }

    void AddVehicle(AVM_StoredCarClient StoredCar)
    {
        if (!StoredCar) return;
        ref AVM_StoredCarClient copy = new AVM_StoredCarClient;
        copy.className = StoredCar.className;
        copy.displayName = StoredCar.displayName;
        copy.playerName = StoredCar.playerName;
        copy.carScriptId = StoredCar.carScriptId;
        copy.expiryDate = StoredCar.expiryDate;
        copy.lastPosition = StoredCar.lastPosition;
        copy.previewAttachments = new array<string>;
        copy.previewCargo = new array<string>;
        if (StoredCar.previewAttachments)
        {
            foreach (string part : StoredCar.previewAttachments) copy.previewAttachments.Insert(part);
        }
        if (StoredCar.previewCargo)
        {
            foreach (string cargoItem : StoredCar.previewCargo) copy.previewCargo.Insert(cargoItem);
        }
        copy.previewHealth = StoredCar.previewHealth;
        copy.previewFuel = StoredCar.previewFuel;
        copy.storedDate = StoredCar.storedDate;
        for (int existing = 0; existing < m_Vehicles.Count(); existing++)
        {
            if (m_Vehicles.Get(existing).carScriptId == copy.carScriptId)
            {
                Print("[AVM UI] Duplicate vehicle ID in RPC: " + copy.carScriptId.ToString());
                return;
            }
        }
        m_Vehicles.Insert(copy);
        RebuildSearchResults();
        if (m_SelectedVehicleIndex < 0 && m_Vehicles.Count() == 1)
            SelectVehicle(0);
    }

    void RefreshVehicleRows()
    {
        int count = m_VisibleVehicleIndices.Count();
        int maxPage = 0;
        if (count > 0)
            maxPage = (count - 1) / VEHICLES_PER_PAGE;
        if (m_Page > maxPage)
            m_Page = maxPage;

        ClearRowThumbnails();
        for (int i = 0; i < VEHICLES_PER_PAGE; i++)
        {
            int index = m_Page * VEHICLES_PER_PAGE + i;
            ButtonWidget btn = m_VehicleButtons.Get(i);
            Widget card = m_RowCards.Get(i);
            if (index < count && m_RetrieveMode)
            {
                int actualIndex = GetVehicleIndexForRow(i);
                AVM_StoredCarClient car = m_Vehicles.Get(actualIndex);
                if (m_RowLabels.Get(i)) m_RowLabels.Get(i).SetText(car.displayName);
                if (m_RowMetas.Get(i)) m_RowMetas.Get(i).SetText(GetVehicleCategory(car));
                if (m_RowIds.Get(i)) m_RowIds.Get(i).SetText("ID: " + car.carScriptId.ToString());
                if (m_RowExpiries.Get(i)) m_RowExpiries.Get(i).SetText(FormatExpiry(car.expiryDate));
                if (btn) btn.Show(true);
                if (card) card.Show(true);
                if (m_RowStars.Get(i))
                {
                    m_RowStars.Get(i).Show(true);
                    m_RowStars.Get(i).SetText("*");
                    if (m_FavoriteIds.Find(car.carScriptId) != -1)
                        m_RowStars.Get(i).SetColor(ARGB(255, 240, 190, 50));
                    else
                        m_RowStars.Get(i).SetColor(ARGB(0, 0, 0, 0));
                }
                SetCardState(i, actualIndex == m_SelectedVehicleIndex, false);
                SetRowThumbnail(i, car.className);
            }
            else
            {
                if (btn) btn.Show(false);
                if (card) card.Show(false);
                if (m_RowStars.Get(i)) m_RowStars.Get(i).Show(false);
            }
        }
        if (m_PageInfo)
            m_PageInfo.SetText((m_Page + 1).ToString() + " / " + (maxPage + 1).ToString());
        if (m_ResultCount)
            m_ResultCount.SetText(count.ToString() + " Vehicles");
        if (m_NoResults)
            m_NoResults.Show(count == 0 && m_RetrieveMode);
        if (m_PrevPage)
            m_PrevPage.Enable(m_Page > 0);
        if (m_NextPage)
            m_NextPage.Enable(m_Page < maxPage);
    }

    int GetVehicleIndexForRow(int row)
    {
        int resultIndex = m_Page * VEHICLES_PER_PAGE + row;
        if (resultIndex < 0 || resultIndex >= m_VisibleVehicleIndices.Count())
            return -1;
        if (m_Reverse)
            resultIndex = m_VisibleVehicleIndices.Count() - 1 - resultIndex;
        return m_VisibleVehicleIndices.Get(resultIndex);
    }

    string GetVehicleCategory(AVM_StoredCarClient car)
    {
        string n = car.className + " " + car.displayName;
        n.ToLower();
        if (n.Contains("offroad") || n.Contains("jeep") || n.Contains("rubi") || n.Contains("gunter")) return "Offroad";
        if (n.Contains("hatch") || n.Contains("sedan") || n.Contains("olga") || n.Contains("ada")) return "Civilian";
        if (n.Contains("truck") || n.Contains("v3s") || n.Contains("utility")) return "Utility";
        if (n.Contains("boat") || n.Contains("kayak")) return "Watercraft";
        if (n.Contains("heli") || n.Contains("helicopter")) return "Aircraft";
        if (n.Contains("moto") || n.Contains("bike")) return "Motorcycle";
        if (n.Contains("hellcat") || n.Contains("sport")) return "Sports";
        return "Vehicle";
    }

    string FormatExpiry(string expiry)
    {
        if (expiry == "" || expiry == "N/A")
            return "No expiry";
        if (expiry.Contains("Left"))
            return expiry;
        return expiry + " Left";
    }

    string PrettyItemName(string className)
    {
        string name = className;
        name.Replace("  + ", "");
        name.Replace("_", " ");
        return name;
    }

    string NormalizeClassName(string className)
    {
        string name = className;
        name.Replace("  + ", "");
        name.Trim();
        return name;
    }

    void ClearRowThumbnails()
    {
        for (int i = 0; i < m_RowPreviews.Count(); i++)
        {
            if (m_RowPreviews.Get(i))
            {
                m_RowPreviews.Get(i).SetItem(null);
                m_RowPreviews.Get(i).Show(false);
            }
        }
        foreach (EntityAI thumb : m_RowPreviewEntities)
        {
            if (thumb) GetGame().ObjectDelete(thumb);
        }
        m_RowPreviewEntities.Clear();
    }

    void SetRowThumbnail(int row, string vehicleClass)
    {
        if (row < 0 || row >= m_RowPreviews.Count() || vehicleClass == "")
            return;
        ItemPreviewWidget preview = m_RowPreviews.Get(row);
        if (!preview)
            return;
        EntityAI entity = EntityAI.Cast(GetGame().CreateObjectEx(vehicleClass, "0 0 0", ECE_LOCAL | ECE_NOLIFETIME));
        if (!entity)
            return;
        entity.SetAllowDamage(false);
        m_RowPreviewEntities.Insert(entity);
        preview.SetItem(entity);
        preview.SetView(0);
        preview.Show(true);
    }

    void SetStatBar(int index, float fraction, bool known)
    {
        if (index < 0 || index >= m_StatValues.Count())
            return;
        if (!known || fraction < 0)
        {
            if (m_StatValues.Get(index)) m_StatValues.Get(index).SetText("N/A");
            if (m_StatFills.Get(index)) m_StatFills.Get(index).SetSize(0, 1);
            return;
        }
        int pct = Math.Round(Math.Clamp(fraction, 0.0, 1.0) * 100);
        if (m_StatValues.Get(index)) m_StatValues.Get(index).SetText(pct.ToString() + "%");
        if (m_StatFills.Get(index))
            m_StatFills.Get(index).SetSize(Math.Clamp(fraction, 0.0, 1.0), 1);
    }

    void AggregateItems(array<string> source, array<string> names, array<int> counts)
    {
        names.Clear();
        counts.Clear();
        if (!source)
            return;
        foreach (string raw : source)
        {
            string key = NormalizeClassName(raw);
            if (key == "")
                continue;
            int found = names.Find(key);
            if (found == -1)
            {
                names.Insert(key);
                counts.Insert(1);
            }
            else
            {
                counts.Set(found, counts.Get(found) + 1);
            }
        }
    }

    void ToggleFavorite(int vehicleIndex)
    {
        if (vehicleIndex < 0 || vehicleIndex >= m_Vehicles.Count())
            return;
        int id = m_Vehicles.Get(vehicleIndex).carScriptId;
        int favIndex = m_FavoriteIds.Find(id);
        if (favIndex == -1)
            m_FavoriteIds.Insert(id);
        else
            m_FavoriteIds.Remove(favIndex);
        RebuildSearchResults();
        if (vehicleIndex == m_SelectedVehicleIndex)
            SelectVehicle(vehicleIndex);
    }

    // Native DayZ ItemPreviewWidget: avoids changing the player's active camera.
    // The preview entity is client-local and is never stored or sent to the server.
    void Clear3DPreview()
    {
        if (m_Vehicle3DPreview)
        {
            m_Vehicle3DPreview.SetItem(null);
            m_Vehicle3DPreview.Show(false);
        }
        if (m_PreviewEntity)
        {
            GetGame().ObjectDelete(m_PreviewEntity);
            m_PreviewEntity = null;
        }
    }

    void Show3DPreview(string vehicleClass)
    {
        Clear3DPreview();
        if (m_PreviewStatus)
            m_PreviewStatus.SetText("PREVIEW NOT AVAILABLE");
        if (vehicleClass == "" || !m_Vehicle3DPreview)
            return;

        m_PreviewEntity = EntityAI.Cast(GetGame().CreateObjectEx(vehicleClass, "0 0 0", ECE_LOCAL | ECE_NOLIFETIME));
        if (!m_PreviewEntity)
        {
            if (m_PreviewStatus)
                m_PreviewStatus.SetText("MODEL NOT AVAILABLE: " + vehicleClass);
            return;
        }
        m_PreviewEntity.SetAllowDamage(false);
        // Rebuild ONLY direct attachments from the persisted, read-only snapshot.
        // These are local preview parts and cannot be retrieved or saved.
        if (m_SelectedVehicleIndex >= 0 && m_SelectedVehicleIndex < m_Vehicles.Count())
        {
            AVM_StoredCarClient selected = m_Vehicles.Get(m_SelectedVehicleIndex);
            if (selected.previewAttachments)
            {
                foreach (string partClass : selected.previewAttachments)
                {
                    if (partClass != "" && m_PreviewEntity.GetInventory())
                        m_PreviewEntity.GetInventory().CreateAttachment(partClass);
                }
            }
        }
        m_Vehicle3DPreview.SetItem(m_PreviewEntity);
        m_Vehicle3DPreview.SetView(0);
        m_Vehicle3DPreview.Show(true);
        m_PreviewYaw = 0;
        m_PreviewZoom = 1.0;
        m_InspectingItem = false;
        RefreshPreviewOrientation();
        if (m_PreviewStatus)
            m_PreviewStatus.SetText("3D PREVIEW  /  " + vehicleClass);
    }

    void ClearInventoryTiles()
    {
        for (int i = 0; i < m_PartsPreviews.Count(); i++)
        {
            if (m_PartsPreviews.Get(i)) { m_PartsPreviews.Get(i).SetItem(null); m_PartsPreviews.Get(i).Show(false); }
            if (m_CargoPreviews.Get(i)) { m_CargoPreviews.Get(i).SetItem(null); m_CargoPreviews.Get(i).Show(false); }
            if (m_PartsLabels.Get(i)) m_PartsLabels.Get(i).SetText("");
            if (m_CargoLabels.Get(i)) m_CargoLabels.Get(i).SetText("");
            if (m_PartsQtys.Get(i)) m_PartsQtys.Get(i).SetText("");
            if (m_CargoQtys.Get(i)) m_CargoQtys.Get(i).SetText("");
        }
        foreach (EntityAI tileEntity : m_TileEntities)
        {
            if (tileEntity) GetGame().ObjectDelete(tileEntity);
        }
        m_TileEntities.Clear();
    }

    void SetInventoryTile(bool isCargo, int slot, string className)
    {
        ItemPreviewWidget widget;
        TextWidget label;
        if (isCargo) { widget = m_CargoPreviews.Get(slot); label = m_CargoLabels.Get(slot); }
        else { widget = m_PartsPreviews.Get(slot); label = m_PartsLabels.Get(slot); }
        string spawnClass = NormalizeClassName(className);
        if (label) label.SetText(PrettyItemName(spawnClass));
        if (!widget || spawnClass == "") return;
        // Preview entities are client-only; never touch actual vehicle inventory.
        EntityAI entity = EntityAI.Cast(GetGame().CreateObjectEx(spawnClass, "0 0 0", ECE_LOCAL | ECE_NOLIFETIME));
        if (!entity) return;
        entity.SetAllowDamage(false);
        m_TileEntities.Insert(entity);
        if (label) label.SetText(entity.GetDisplayName());
        widget.SetItem(entity);
        widget.SetView(0);
        widget.Show(true);
    }

    void ShowInventoryItem(string className)
    {
        if (className == "") return;
        Clear3DPreview();
        m_InspectingItem = true;
        m_PreviewEntity = EntityAI.Cast(GetGame().CreateObjectEx(className, "0 0 0", ECE_LOCAL | ECE_NOLIFETIME));
        if (!m_PreviewEntity) return;
        m_PreviewEntity.SetAllowDamage(false);
        m_Vehicle3DPreview.SetItem(m_PreviewEntity);
        m_Vehicle3DPreview.SetView(0);
        m_Vehicle3DPreview.Show(true);
        if (m_PreviewStatus) m_PreviewStatus.SetText("ITEM PREVIEW / " + className);
    }

    void RefreshPreviewOrientation()
    {
        if (!m_Vehicle3DPreview || !m_PreviewEntity) return;
        m_Vehicle3DPreview.SetModelOrientation(Vector(0, m_PreviewYaw, 0));
        m_Vehicle3DPreview.SetModelPosition(Vector(0, 0, m_PreviewZoom - 1.0));
    }

    override bool OnMouseButtonDown(Widget w, int x, int y, int button)
    {
        if (w == m_Vehicle3DPreview && button == 0)
        {
            m_IsDraggingPreview = true;
            m_LastDragX = x;
            return true;
        }
        return super.OnMouseButtonDown(w, x, y, button);
    }

    override bool OnMouseButtonUp(Widget w, int x, int y, int button)
    {
        m_IsDraggingPreview = false;
        return super.OnMouseButtonUp(w, x, y, button);
    }

    bool OnMouseMove(Widget w, int x, int y)
    {
        if (m_IsDraggingPreview && w == m_Vehicle3DPreview)
        {
            m_PreviewYaw = m_PreviewYaw + (x - m_LastDragX) * 0.7;
            m_LastDragX = x;
            RefreshPreviewOrientation();
            return true;
        }
        return false;
    }

    override bool OnMouseWheel(Widget w, int x, int y, int wheel)
    {
        if (w == m_Vehicle3DPreview)
        {
            m_PreviewZoom = Math.Clamp(m_PreviewZoom + wheel * 0.10, 0.4, 2.2);
            RefreshPreviewOrientation();
            return true;
        }
        return super.OnMouseWheel(w, x, y, wheel);
    }

    void UpdateInventoryPages()
    {
        ClearInventoryTiles();
        if (m_SelectedVehicleIndex < 0 || m_SelectedVehicleIndex >= m_Vehicles.Count()) return;
        AVM_StoredCarClient car = m_Vehicles.Get(m_SelectedVehicleIndex);
        AggregateItems(car.previewAttachments, m_PartsNames, m_PartsCounts);
        AggregateItems(car.previewCargo, m_CargoNames, m_CargoCounts);
        int partsCount = m_PartsNames.Count();
        int cargoCount = m_CargoNames.Count();
        if (partsCount > 0 && m_PartsPage * TILES_PER_PAGE >= partsCount) m_PartsPage = (partsCount - 1) / TILES_PER_PAGE;
        if (cargoCount > 0 && m_CargoPage * TILES_PER_PAGE >= cargoCount) m_CargoPage = (cargoCount - 1) / TILES_PER_PAGE;
        for (int i = 0; i < TILES_PER_PAGE; i++)
        {
            string part = "";
            string cargo = "";
            int partIndex = m_PartsPage * TILES_PER_PAGE + i;
            int cargoIndex = m_CargoPage * TILES_PER_PAGE + i;
            if (partIndex < partsCount)
            {
                part = m_PartsNames.Get(partIndex);
                SetInventoryTile(false, i, part);
                if (m_PartsQtys.Get(i))
                {
                    if (m_PartsCounts.Get(partIndex) > 1)
                        m_PartsQtys.Get(i).SetText(m_PartsCounts.Get(partIndex).ToString());
                    else
                        m_PartsQtys.Get(i).SetText("EQUIPPED");
                }
            }
            else
            {
                SetInventoryTile(false, i, "");
            }
            if (cargoIndex < cargoCount)
            {
                cargo = m_CargoNames.Get(cargoIndex);
                SetInventoryTile(true, i, cargo);
                if (m_CargoQtys.Get(i)) m_CargoQtys.Get(i).SetText(m_CargoCounts.Get(cargoIndex).ToString());
            }
            else
            {
                SetInventoryTile(true, i, "");
            }
        }
        if (m_PartsPrevious) m_PartsPrevious.Enable(m_PartsPage > 0);
        if (m_PartsNext) m_PartsNext.Enable((m_PartsPage + 1) * TILES_PER_PAGE < partsCount);
        if (m_CargoPrevious) m_CargoPrevious.Enable(m_CargoPage > 0);
        if (m_CargoNext) m_CargoNext.Enable((m_CargoPage + 1) * TILES_PER_PAGE < cargoCount);
        TextWidget partsPageLabel = TextWidget.Cast(m_Root.FindAnyWidget("ARPartsPage"));
        TextWidget cargoPageLabel = TextWidget.Cast(m_Root.FindAnyWidget("ARCargoPage"));
        TextWidget partsCountLabel = TextWidget.Cast(m_Root.FindAnyWidget("ARPartsCount"));
        TextWidget cargoCountLabel = TextWidget.Cast(m_Root.FindAnyWidget("ARCargoCount"));
        int partsPages = 1;
        int cargoPages = 1;
        if (partsCount > 0) partsPages = ((partsCount - 1) / TILES_PER_PAGE) + 1;
        if (cargoCount > 0) cargoPages = ((cargoCount - 1) / TILES_PER_PAGE) + 1;
        if (partsPageLabel) partsPageLabel.SetText((m_PartsPage + 1).ToString() + "/" + partsPages.ToString());
        if (cargoPageLabel) cargoPageLabel.SetText((m_CargoPage + 1).ToString() + "/" + cargoPages.ToString());
        if (partsCountLabel) partsCountLabel.SetText("(" + partsCount.ToString() + ")");
        if (cargoCountLabel) cargoCountLabel.SetText("(" + cargoCount.ToString() + ")");
        if (m_InvTabAttachLabel)
            m_InvTabAttachLabel.SetText("ATTACHMENTS (" + partsCount.ToString() + ")");
        if (m_InvTabCargoLabel)
            m_InvTabCargoLabel.SetText("CARGO (" + cargoCount.ToString() + ")");
        RefreshInventoryTab();
    }

    void RefreshInventoryTab()
    {
        if (m_PartsPanel)
            m_PartsPanel.Show(!m_ShowCargoTab);
        if (m_CargoPanel)
            m_CargoPanel.Show(m_ShowCargoTab);
        SetPanelColor(m_InvTabAttachBg, ColorIdle());
        SetPanelColor(m_InvTabCargoBg, ColorIdle());
        if (m_ShowCargoTab)
            SetPanelColor(m_InvTabCargoBg, ColorActive());
        else
            SetPanelColor(m_InvTabAttachBg, ColorActive());
    }

    string FormatPosition(vector pos)
    {
        string raw = pos.ToString();
        raw.Replace("<", "");
        raw.Replace(">", "");
        return raw;
    }

    void SelectVehicle(int index)
    {
        if (index < 0 || index >= m_Vehicles.Count())
            return;
        m_SelectedVehicleIndex = index;
        AVM_StoredCarClient car = m_Vehicles.Get(index);
        Show3DPreview(car.className);
        Widget inventoryOverlay = m_Root.FindAnyWidget("ARInventoryPanel");
        if (inventoryOverlay)
            inventoryOverlay.Show(false);
        if (m_DetailTitle) m_DetailTitle.SetText(car.displayName);
        if (m_DetailOwner) m_DetailOwner.SetText(car.playerName);
        if (m_DetailId) m_DetailId.SetText(car.carScriptId.ToString());
        if (m_DetailExpiry) m_DetailExpiry.SetText(FormatExpiry(car.expiryDate));
        if (m_StoredDate)
        {
            if (car.storedDate != "")
                m_StoredDate.SetText(car.storedDate);
            else
                m_StoredDate.SetText("N/A");
        }
        if (m_TagOffroad)
            m_TagOffroad.SetText(GetVehicleCategory(car));
        if (m_TagPersonal)
            m_TagPersonal.SetText("PERSONAL VEHICLE");
        if (m_TitleFavorite)
        {
            m_TitleFavorite.SetText("*");
            if (m_FavoriteIds.Find(car.carScriptId) != -1)
                m_TitleFavorite.SetColor(ARGB(255, 240, 190, 50));
            else
                m_TitleFavorite.SetColor(ARGB(0, 0, 0, 0));
        }
        if (m_DetailPosition)
            m_DetailPosition.SetText(FormatPosition(car.lastPosition));
        if (m_InfoMessage)
            m_InfoMessage.SetText("Selected: " + car.displayName + "  |  ID " + car.carScriptId.ToString());
        SetStatBar(0, car.previewHealth, car.previewHealth >= 0);
        SetStatBar(1, -1, false);
        SetStatBar(2, -1, false);
        SetStatBar(3, -1, false);
        SetStatBar(4, car.previewFuel, car.previewFuel >= 0);
        m_PartsPage = 0;
        m_CargoPage = 0;
        UpdateInventoryPages();
        RefreshVehicleRows();
    }

	override bool OnClick(Widget w, int x, int y, int button) 
	{
        if (w == m_PreviewBack)
        {
            if (m_SelectedVehicleIndex >= 0 && m_SelectedVehicleIndex < m_Vehicles.Count())
                Show3DPreview(m_Vehicles.Get(m_SelectedVehicleIndex).className);
            return true;
        }
        for (int slot = 0; slot < TILES_PER_PAGE; slot++)
        {
            int partIndex = m_PartsPage * TILES_PER_PAGE + slot;
            int cargoIndex = m_CargoPage * TILES_PER_PAGE + slot;
            if (w == m_Root.FindAnyWidget("ARPartsSelect" + slot.ToString()) && partIndex < m_PartsNames.Count())
            {
                ShowInventoryItem(m_PartsNames.Get(partIndex));
                return true;
            }
            if (w == m_Root.FindAnyWidget("ARCargoSelect" + slot.ToString()) && cargoIndex < m_CargoNames.Count())
            {
                ShowInventoryItem(m_CargoNames.Get(cargoIndex));
                return true;
            }
        }

        if (w == m_PartsPrevious && m_PartsPage > 0) { m_PartsPage--; UpdateInventoryPages(); return true; }
        if (w == m_PartsNext) { m_PartsPage++; UpdateInventoryPages(); return true; }
        if (w == m_CargoPrevious && m_CargoPage > 0) { m_CargoPage--; UpdateInventoryPages(); return true; }
        if (w == m_CargoNext) { m_CargoPage++; UpdateInventoryPages(); return true; }

		// The close widget has no IDC_CANCEL user ID in list.layout.
        // Handle its actual widget reference before the legacy ID switch.
        if (w == m_BtnClose || w == m_Root.FindAnyWidget("CloseX"))
        {
            Close();
            return true;
        }
        switch (w.GetUserID()) 
		{
		case IDC_CANCEL:
			Close();
			return true;
		}
		
        if (w == m_InvTabAttach)
        {
            m_ShowCargoTab = false;
            RefreshInventoryTab();
            return true;
        }
        if (w == m_InvTabCargo)
        {
            m_ShowCargoTab = true;
            RefreshInventoryTab();
            return true;
        }
        if (w == m_ClearSearch && m_SearchBox)
        {
            m_SearchBox.SetText("");
            m_Page = 0;
            RebuildSearchResults();
            return true;
        }
        if (w == m_FavoritesFilter)
        {
            m_RetrieveMode = true;
            m_FavoritesOnly = !m_FavoritesOnly;
            m_Page = 0;
            SwitchPage();
            RebuildSearchResults();
            return true;
        }
        if (w == m_ReverseOrder)
        {
            m_RetrieveMode = true;
            m_Reverse = !m_Reverse;
            SwitchPage();
            RebuildSearchResults();
            return true;
        }
        if (w == m_TitleFavorite && m_SelectedVehicleIndex >= 0)
        {
            ToggleFavorite(m_SelectedVehicleIndex);
            return true;
        }
        for (int starRow = 0; starRow < m_RowStars.Count(); starRow++)
        {
            if (w == m_RowStars.Get(starRow))
            {
                ToggleFavorite(GetVehicleIndexForRow(starRow));
                return true;
            }
        }
        if (w == m_SpawnBtn)
        {
            if (m_InfoMessage)
                m_InfoMessage.SetText("Spawn is this parking location. Retrieve will spawn the vehicle here.");
            return true;
        }
        if (w == m_DeleteBtn)
        {
            m_ErrorPanel.Show(true);
            m_ErrorText.SetText("Delete is not enabled. Retrieve the vehicle, then dispose of it in-world.");
            return true;
        }

        int pressedAction = GetActionIndex(w);
        if (pressedAction >= 0)
            SetActionPanel(pressedAction, ColorActive());

        if (w == m_PrevPage && m_Page > 0)
        {
            m_Page--;
            RefreshVehicleRows();
            return true;
        }
        if (w == m_NextPage && (m_Page + 1) * VEHICLES_PER_PAGE < m_VisibleVehicleIndices.Count())
        {
            m_Page++;
            RefreshVehicleRows();
            return true;
        }
        for (int i = 0; i < m_VehicleButtons.Count(); i++)
        {
            if (w == m_VehicleButtons.Get(i))
            {
                SelectVehicle(GetVehicleIndexForRow(i));
                return true;
            }
        }

		switch (w)
		{
		case m_BtnPark:
			m_RetrieveMode = false;
            m_FavoritesOnly = false;
			SwitchPage();
			return true;
		case m_BtnRetrieve:
			m_RetrieveMode = true;
            m_FavoritesOnly = false;
			SwitchPage();
			return true;
		case m_BtnConfirm:
				if(m_RetrieveMode)
				{					
					if (m_SelectedVehicleIndex == -1)
					{	
						return true;
					}
					RetrieveCar();
				}
				else
				{
					ParkCar();
				}
			return true;
		}

		return super.OnClick(w, x, y, button);
	}

	void SwitchPage()
	{
		m_ErrorPanel.Show(false);
        m_ParkVehicleList.Show(false);
        RefreshActionTextures();
        if (m_NoResults)
            m_NoResults.Show(false);
		if(m_RetrieveMode)
		{
			//retrieve mode
			m_BtnPark.Enable(true);
            m_BtnRetrieve.Enable(true);
			RefreshVehicleRows();
			m_ParkVehicleList.Show(false);
			m_ConfirmLabel.SetText("RETRIEVE VEHICLE");
		}
		else
		{			
			//parking mode
			m_BtnPark.Enable(true);
            m_BtnRetrieve.Enable(true);
			for (int i = 0; i < m_VehicleButtons.Count(); i++)
                {
                    if (m_VehicleButtons.Get(i)) m_VehicleButtons.Get(i).Show(false);
                    if (m_RowCards.Get(i)) m_RowCards.Get(i).Show(false);
                    if (m_RowStars.Get(i)) m_RowStars.Get(i).Show(false);
                }
            m_ConfirmLabel.SetText("PARK VEHICLE");
			Transport vehicle = AVM_Helper.GetNearbyTransport(m_TargetPosition, 4);
			if(vehicle)
			{
				string vehicleName = vehicle.GetDisplayName();
				m_ParkVehicleList.SetText("Vehicle found: " + vehicleName);
				m_ParkVehicleList.Show(true);
			}
			else
			{
				m_ErrorText.SetText("No vehicle found within 4m of this parking meter.");
				m_ErrorPanel.Show(true);
			}
		}
	}

	void ParkCar()
	{
		Transport vehicle = AVM_Helper.GetNearbyTransport(m_TargetPosition, 4);
		string reason = "";
		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
		if (m_MaxCarsAllowed > 0)
		{
			if(m_CarsStoredCount >= m_MaxCarsAllowed)
			{
				if(player.GetItemInHands() && player.GetItemInHands().IsInherited(MCK_MasterKey))
				{
					
				}
				else
				{
					reason = "#avm_e_fullgarage";
					m_ErrorText.SetText("#avm_error: " +  reason);
					m_ErrorPanel.Show(true);
					return;
				}
			}
		}
		if(!AVM_Helper.CanPark(player, vehicle, reason))
		{
			m_ErrorText.SetText("#avm_error: " +  reason);
			m_ErrorPanel.Show(true);
			return;
		}		
		Param1<vector> rpc_params = new Param1<vector>(m_TargetPosition);
		GetGame().RPCSingleParam(GetGame().GetPlayer(), AVM_RPCs.RPC_PARKCAR, rpc_params, false, GetGame().GetPlayer().GetIdentity());
		Close();
	}

	void RetrieveCar()
	{
		array<Object> nearby_objectsTotal = new array<Object>;		
		array<Object> excluded = new array<Object>;
		GetGame().IsBoxCollidingGeometry(m_TargetPosition, m_TargetOrientation, "2 1 4", ObjIntersectView, ObjIntersectGeom, excluded, nearby_objectsTotal);
		// Print("got nearby_objects total: " + nearby_objectsTotal.Count());
		Object objectNearby;
		bool blocked = false;	
		for (int i = 0; i < nearby_objectsTotal.Count(); i++)
		{
			objectNearby = nearby_objectsTotal.Get(i);
			if(objectNearby && (objectNearby.IsAnyInherited({AVM_ParkingBay, AVM_HND_ParkingBay, AVM_ParkingMeter, AVM_HND_ParkingMeter})))
			{
				continue;
			}
			blocked = true;
			break;
		}
		if(blocked && objectNearby)
		{
			string blockedBy = "#avm_e_blocked: " + objectNearby.GetDisplayName();
			//Do error text
			m_ErrorPanel.Show(true);
			m_ErrorText.SetText("ERROR: " + blockedBy);
			return;
		}

		
        if (m_SelectedVehicleIndex < 0 || m_SelectedVehicleIndex >= m_Vehicles.Count())
            return;
        AVM_StoredCarClient clientData = m_Vehicles.Get(m_SelectedVehicleIndex);
		if(g_Game.GetApexVehicleManagementSettings().AllowRetrieveWithinRadiusOnly)
		{
			float distance = vector.Distance(m_TargetPosition, clientData.lastPosition);
			int radius = g_Game.GetApexVehicleManagementSettings().RadiusRetrieve;
			if(distance > radius)
			{
				string faraway = "You cannot retrieve vehicle more than " + radius + "m away from last position.";
				//Do error text
				m_ErrorPanel.Show(true);
				m_ErrorText.SetText("ERROR: " + faraway);
				return;
			}
		}
		
		Param3<int, vector, vector> rpc_params = new Param3<int, vector, vector>(clientData.carScriptId, m_TargetPosition, m_TargetOrientation);
		GetGame().RPCSingleParam(GetGame().GetPlayer(), AVM_RPCs.RPC_RETRIEVE_PARKEDCAR, rpc_params, false, GetGame().GetPlayer().GetIdentity());
		Close();
	}

	override void OnShow() 
	{
		if (GetGame().GetMission())
			GetGame().GetMission().PlayerControlDisable(INPUT_EXCLUDE_ALL);
		super.OnShow();
	}

	override void OnHide() 
	{
        Clear3DPreview();
        ClearRowThumbnails();
        ClearInventoryTiles();
		super.OnHide();
		if (GetGame().GetMission())
			GetGame().GetMission().PlayerControlEnable(true);
	}
}
#endif