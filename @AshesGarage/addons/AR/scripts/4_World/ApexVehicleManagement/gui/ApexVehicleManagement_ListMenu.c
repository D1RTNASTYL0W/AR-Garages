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
    protected Widget m_PreviewFrame;
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
    protected ref array<ImageWidget> m_RowImages;
    protected ref array<ImageWidget> m_ActionImages;
    protected ref array<Widget> m_RowAccents;
    protected ButtonWidget m_PreviewDrag;
    protected ref array<TextWidget> m_RowLabels;
    protected ref array<TextWidget> m_RowMetas;
    protected ref array<TextWidget> m_RowIds;
    protected ref array<TextWidget> m_RowExpiries;
    protected ref array<ButtonWidget> m_RowStars;
    protected ref array<ImageWidget> m_RowStarImages;
    protected ref array<ItemPreviewWidget> m_RowPreviews;
    protected ref array<EntityAI> m_RowPreviewEntities;
    protected ref array<TextWidget> m_PartsQtys;
    protected ref array<TextWidget> m_CargoQtys;
    protected ref array<Widget> m_ActionPanels;
    protected ref array<string> m_PartsNames;
    protected ref array<int> m_PartsCounts;
    protected ref array<float> m_PartsHealths;
    protected ref array<string> m_CargoNames;
    protected ref array<int> m_CargoCounts;
    protected ref array<float> m_CargoHealths;
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
    protected Widget m_ConfirmDeletePanel;
    protected ButtonWidget m_ConfirmDeleteCancel;
    protected ButtonWidget m_ConfirmDeleteOk;
    protected TextWidget m_ConfirmDeleteBody;
    protected TextWidget m_MongoLabel;
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
	protected static const int VEHICLES_PER_PAGE = 6;
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
	protected ref AVM_StoredCarClient m_ParkCandidate;
	protected ref AVM_StoredCarClient m_ServerParkCandidate;
	private int m_CarsStoredCount = -1;
	private int m_MaxCarsAllowed = -1;
	private bool m_StoreCargoAllowed = false;

	override Widget Init() 
	{
		layoutRoot = GetGame().GetWorkspace().CreateWidgets("AshesReborn/gui/list.layout");
		m_Root = layoutRoot;
        m_PreviewFrame = m_Root.FindAnyWidget("PreviewFrame");
        m_PreviewDrag = ButtonWidget.Cast(m_Root.FindAnyWidget("PreviewDrag"));
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
        if (m_Vehicle3DPreview)
        {
            m_Vehicle3DPreview.SetSort(0);
            m_Vehicle3DPreview.Show(true);
        }
        m_PreviewStatus = TextWidget.Cast(m_Root.FindAnyWidget("ARPreviewStatus"));
        m_PreviewBack = ButtonWidget.Cast(m_Root.FindAnyWidget("ARPreviewBack"));
        m_SearchPlaceholder = m_Root.FindAnyWidget("ARSearchPlaceholder");
        m_TitleFavorite = ButtonWidget.Cast(m_Root.FindAnyWidget("ARTitleFavorite"));
        m_SpawnBtn = ButtonWidget.Cast(m_Root.FindAnyWidget("ARSpawnBtn"));
        m_DeleteBtn = ButtonWidget.Cast(m_Root.FindAnyWidget("ARDeleteBtn"));
        m_ConfirmDeletePanel = m_Root.FindAnyWidget("ARConfirmDeletePanel");
        m_ConfirmDeleteCancel = ButtonWidget.Cast(m_Root.FindAnyWidget("ARConfirmDeleteCancel"));
        m_ConfirmDeleteOk = ButtonWidget.Cast(m_Root.FindAnyWidget("ARConfirmDeleteOk"));
        m_ConfirmDeleteBody = TextWidget.Cast(m_Root.FindAnyWidget("ARConfirmDeleteBody"));
        m_MongoLabel = TextWidget.Cast(m_Root.FindAnyWidget("MongoLabel"));
        ApplyStorageModeLabel();
        if (m_ConfirmDeletePanel)
        {
            m_ConfirmDeletePanel.SetSort(200);
            m_ConfirmDeletePanel.Show(false);
        }
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
        m_PartsHealths = new array<float>;
        m_CargoNames = new array<string>;
        m_CargoCounts = new array<int>;
        m_CargoHealths = new array<float>;
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
        m_RowImages = new array<ImageWidget>;
        m_ActionImages = new array<ImageWidget>;
        m_RowAccents = new array<Widget>;
        m_RowLabels = new array<TextWidget>;
        m_RowMetas = new array<TextWidget>;
        m_RowIds = new array<TextWidget>;
        m_RowExpiries = new array<TextWidget>;
        m_RowStars = new array<ButtonWidget>;
        m_RowStarImages = new array<ImageWidget>;
        m_RowPreviews = new array<ItemPreviewWidget>;
        m_RowPreviewEntities = new array<EntityAI>;
        m_ActionPanels = new array<Widget>;
        for (int rowIndex = 0; rowIndex < VEHICLES_PER_PAGE; rowIndex++)
        {
            string rowKey = rowIndex.ToString();
            m_VehicleButtons.Insert(ButtonWidget.Cast(m_Root.FindAnyWidget("VehicleRow" + rowKey)));
            m_RowCards.Insert(m_Root.FindAnyWidget("ARRowCard" + rowKey));
            ImageWidget rowImage = ImageWidget.Cast(m_Root.FindAnyWidget("ARRowImage" + rowKey));
            m_RowImages.Insert(rowImage);
            LoadARTexture(rowImage, "ar_row_idle");
            m_RowAccents.Insert(m_Root.FindAnyWidget("ARRowAccent" + rowKey));
            m_RowLabels.Insert(TextWidget.Cast(m_Root.FindAnyWidget("ARRowLabel" + rowKey)));
            m_RowMetas.Insert(TextWidget.Cast(m_Root.FindAnyWidget("ARRowMeta" + rowKey)));
            m_RowIds.Insert(TextWidget.Cast(m_Root.FindAnyWidget("ARRowId" + rowKey)));
            m_RowExpiries.Insert(TextWidget.Cast(m_Root.FindAnyWidget("ARRowExpiry" + rowKey)));
            m_RowStars.Insert(ButtonWidget.Cast(m_Root.FindAnyWidget("ARRowStar" + rowKey)));
            m_RowStarImages.Insert(ImageWidget.Cast(m_Root.FindAnyWidget("ARRowStarIcon" + rowKey)));
            m_RowPreviews.Insert(ItemPreviewWidget.Cast(m_Root.FindAnyWidget("ARRowPreview" + rowKey)));
        }
        m_ActionPanels.Insert(m_Root.FindAnyWidget("ARImageRetrieveBtn"));
        m_ActionPanels.Insert(m_Root.FindAnyWidget("ARImageParkBtn"));
        m_ActionPanels.Insert(m_Root.FindAnyWidget("ARImageConfirmBtn"));
        m_ActionPanels.Insert(m_Root.FindAnyWidget("ARImagePrevPage"));
        m_ActionPanels.Insert(m_Root.FindAnyWidget("ARImageNextPage"));
        ref array<string> actionNames = {"RetrieveBtn", "ParkBtn", "ConfirmBtn", "PrevPage", "NextPage"};
        for (int ai = 0; ai < 5; ai++)
        {
            ImageWidget actionImage = ImageWidget.Cast(m_Root.FindAnyWidget("ARImage" + actionNames[ai]));
            m_ActionImages.Insert(actionImage);
            LoadARTexture(actionImage, "ar_button_idle");
        }
        LoadARTexture(ImageWidget.Cast(m_Root.FindAnyWidget("ARImageConfirmBtn")), "button_blue");
        LoadARTexture(ImageWidget.Cast(m_Root.FindAnyWidget("CloseBg")), "ar_close_idle");
        LoadARTexture(ImageWidget.Cast(m_Root.FindAnyWidget("SearchClearBg")), "ar_icon_close");
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
		Widget inventoryOverlay = m_Root.FindAnyWidget("ARInventoryPanel");
		if (inventoryOverlay)
			inventoryOverlay.Show(true);
        LoadARTexture(ImageWidget.Cast(m_Root.FindAnyWidget("ARImageSpawnBtn")), "button_dark");
        LoadARTexture(ImageWidget.Cast(m_Root.FindAnyWidget("ARImageDeleteBtn")), "ar_btn_red");
        LoadARTexture(ImageWidget.Cast(m_Root.FindAnyWidget("CloseBg")), "ar_close");
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

	void ApplyStorageModeLabel()
	{
		if (!m_MongoLabel)
			return;
		ApexVehicleManagementSettings settings = g_Game.GetApexVehicleManagementSettings();
		if (settings && !settings.UseMongoDB)
			m_MongoLabel.SetText("JSON STORAGE");
		else
			m_MongoLabel.SetText("MONGODB CONNECTED");
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
    int ColorCardIdle() { return ARGB(255, 22, 40, 62); }
    int ColorCardHover() { return ARGB(255, 30, 70, 110); }
    int ColorCardSelected() { return ARGB(255, 37, 122, 214); }
    int ColorAccentOn() { return ARGB(255, 37, 122, 214); }
    int ColorAccentOff() { return ARGB(255, 22, 40, 62); }

    string ResolveARTexture(string textureName)
    {
        if (textureName == "ar_tab_idle") return "ar_nav_idle";
        if (textureName == "ar_tab_hover") return "ar_nav_hover";
        if (textureName == "ar_tab_active") return "ar_nav_active";
        if (textureName == "ar_row_idle") return "ar_vehicle_idle";
        if (textureName == "ar_row_hover") return "ar_vehicle_hover";
        if (textureName == "ar_row_selected") return "ar_vehicle_selected";
        if (textureName == "ar_tile_idle") return "ar_item_idle";
        if (textureName == "ar_tile_hover") return "ar_item_hover";
        if (textureName == "button_blue") return "ar_button_blue_idle";
        if (textureName == "button_dark") return "ar_button_dark_idle";
        if (textureName == "ar_btn_red") return "ar_button_red_idle";
        if (textureName == "ar_btn_blue_hover") return "ar_button_blue_hover";
        if (textureName == "ar_btn_dark_hover") return "ar_button_dark_hover";
        if (textureName == "ar_btn_red_hover") return "ar_button_red_hover";
        if (textureName == "ar_close") return "ar_close_idle";
        if (textureName == "ar_button_idle") return "ar_button_dark_idle";
        if (textureName == "ar_button_hover") return "ar_button_dark_hover";
        if (textureName == "ar_dash_bg") return "ar_main_background";
        if (textureName == "ar_panel") return "ar_section_backdrop";
        if (textureName == "ar_search") return "ar_search_idle";
        if (textureName == "ar_preview_well") return "ar_vehicle_preview";
        if (textureName == "ar_stat_panel") return "ar_vehicle_stats";
        if (textureName == "ar_info_panel") return "ar_vehicle_information";
        return textureName;
    }

    void LoadARTexture(ImageWidget imageWidget, string textureName)
    {
        if (!imageWidget)
            return;
        imageWidget.LoadImageFile(0, "AshesReborn/gui/textures/" + ResolveARTexture(textureName) + ".paa");
        imageWidget.SetImage(0);
    }

    string FavoriteTexture(bool isOn, bool hover, bool pressed)
    {
        if (pressed)
            return "ar_icon_favorite_pressed";
        if (hover)
            return "ar_icon_favorite_hover";
        if (isOn)
            return "ar_icon_favorite_on";
        return "ar_icon_favorite_off";
    }

    bool VehicleIsFavorite(int vehicleIndex)
    {
        if (vehicleIndex < 0 || vehicleIndex >= m_Vehicles.Count())
            return false;
        return m_FavoriteIds.Find(m_Vehicles.Get(vehicleIndex).carScriptId) != -1;
    }

    void SetRowFavoriteIcon(int row, bool hover, bool pressed)
    {
        if (!m_RowStarImages || row < 0 || row >= m_RowStarImages.Count())
            return;
        LoadARTexture(m_RowStarImages.Get(row), FavoriteTexture(VehicleIsFavorite(GetVehicleIndexForRow(row)), hover, pressed));
    }

    void SetTitleFavoriteIcon(bool hover, bool pressed)
    {
        LoadARTexture(ImageWidget.Cast(m_Root.FindAnyWidget("ARTitleFavIcon")), FavoriteTexture(VehicleIsFavorite(m_SelectedVehicleIndex), hover, pressed));
    }

    void RefreshTabIcons()
    {
        string vehiclesIcon = "ar_icon_car";
        if (m_RetrieveMode && !m_FavoritesOnly && !m_Reverse)
            vehiclesIcon = "ar_icon_car_cyan";
        LoadARTexture(ImageWidget.Cast(m_Root.FindAnyWidget("ARIconVehicles")), vehiclesIcon);

        string parkIcon = "ar_icon_garage";
        if (!m_RetrieveMode)
            parkIcon = "ar_icon_garage_cyan";
        LoadARTexture(ImageWidget.Cast(m_Root.FindAnyWidget("ARIconPark")), parkIcon);

        string favIcon = "ar_icon_favorite_off";
        if (m_FavoritesOnly)
            favIcon = "ar_icon_favorite_on";
        LoadARTexture(ImageWidget.Cast(m_Root.FindAnyWidget("ARIconFav")), favIcon);

        string recentIcon = "ar_icon_clock";
        if (m_Reverse)
            recentIcon = "ar_icon_clock_cyan";
        LoadARTexture(ImageWidget.Cast(m_Root.FindAnyWidget("ARIconRecent")), recentIcon);

        string attachIcon = "ar_icon_wrench";
        if (!m_ShowCargoTab)
            attachIcon = "ar_icon_wrench_cyan";
        LoadARTexture(ImageWidget.Cast(m_Root.FindAnyWidget("ARIconAttach")), attachIcon);

        string cargoIcon = "ar_icon_box";
        if (m_ShowCargoTab)
            cargoIcon = "ar_icon_box_cyan";
        LoadARTexture(ImageWidget.Cast(m_Root.FindAnyWidget("ARIconCargoTab")), cargoIcon);
    }

    void SetPanelColor(Widget panel, int color)
    {
        if (panel)
            panel.SetColor(color);
    }

    void SetActionTexture(int index, string texture)
    {
        if (index < 0 || index >= m_ActionImages.Count()) return;
        LoadARTexture(m_ActionImages.Get(index), texture);
    }

    int GetActionIndex(Widget w)
    {
        if (w == m_BtnRetrieve) return 0;
        if (w == m_BtnPark) return 1;
        if (w == m_BtnConfirm) return 2;
        if (w == m_PrevPage) return 3;
        if (w == m_NextPage) return 4;
        if (w == m_SpawnBtn) return 5;
        if (w == m_DeleteBtn) return 6;
        if (w == m_FavoritesFilter) return 7;
        if (w == m_ReverseOrder) return 8;
        if (w == m_BtnClose) return 9;
        return -1;
    }

    void SetActionPanel(int index, int color)
    {
        if (index < 0 || index >= m_ActionPanels.Count()) return;
        SetPanelColor(m_ActionPanels.Get(index), color);
    }

    bool TopTabIsVehicles()
    {
        return m_RetrieveMode && !m_FavoritesOnly && !m_Reverse;
    }

    int ActiveTopTab()
    {
        if (!m_RetrieveMode)
            return 1;
        if (m_FavoritesOnly)
            return 2;
        if (m_Reverse)
            return 3;
        return 0;
    }

    void SetTopTabLabel(string widgetName, bool active)
    {
        TextWidget label = TextWidget.Cast(m_Root.FindAnyWidget(widgetName));
        if (!label)
            return;
        if (active)
            label.SetColor(ARGB(255, 245, 250, 255));
        else
            label.SetColor(ARGB(255, 160, 180, 198));
    }

    void SelectTopTab(int tab)
    {
        m_RetrieveMode = true;
        m_FavoritesOnly = false;
        m_Reverse = false;
        if (tab == 1)
            m_RetrieveMode = false;
        else if (tab == 2)
            m_FavoritesOnly = true;
        else if (tab == 3)
            m_Reverse = true;
        m_Page = 0;
        SwitchPage();
        if (m_RetrieveMode)
            RebuildSearchResults();
    }

    void ClearActionTints()
    {
        int white = ARGB(255, 255, 255, 255);
        int clear = ARGB(0, 0, 0, 0);
        if (m_ActionPanels)
        {
            for (int i = 0; i < m_ActionPanels.Count(); i++)
                SetPanelColor(m_ActionPanels.Get(i), white);
        }
        SetPanelColor(m_Root.FindAnyWidget("ARTabFavBg"), white);
        SetPanelColor(m_Root.FindAnyWidget("ARTabRecentBg"), white);
        SetPanelColor(m_Root.FindAnyWidget("ARImageSpawnBtn"), white);
        SetPanelColor(m_Root.FindAnyWidget("ARImageDeleteBtn"), white);
        SetPanelColor(m_BtnRetrieve, clear);
        SetPanelColor(m_BtnPark, clear);
        SetPanelColor(m_FavoritesFilter, clear);
        SetPanelColor(m_ReverseOrder, clear);
        SetPanelColor(m_BtnConfirm, clear);
        SetPanelColor(m_PrevPage, clear);
        SetPanelColor(m_NextPage, clear);
        SetPanelColor(m_SpawnBtn, clear);
        SetPanelColor(m_DeleteBtn, clear);
    }

    void RefreshActionTextures()
    {
        ClearActionTints();
        LoadARTexture(ImageWidget.Cast(m_Root.FindAnyWidget("ARImageRetrieveBtn")), "ar_tab_idle");
        LoadARTexture(ImageWidget.Cast(m_Root.FindAnyWidget("ARImageParkBtn")), "ar_tab_idle");
        LoadARTexture(ImageWidget.Cast(m_Root.FindAnyWidget("ARTabFavBg")), "ar_tab_idle");
        LoadARTexture(ImageWidget.Cast(m_Root.FindAnyWidget("ARTabRecentBg")), "ar_tab_idle");
        int tab = ActiveTopTab();
        if (tab == 1)
            LoadARTexture(ImageWidget.Cast(m_Root.FindAnyWidget("ARImageParkBtn")), "ar_tab_active");
        else if (tab == 2)
            LoadARTexture(ImageWidget.Cast(m_Root.FindAnyWidget("ARTabFavBg")), "ar_tab_active");
        else if (tab == 3)
            LoadARTexture(ImageWidget.Cast(m_Root.FindAnyWidget("ARTabRecentBg")), "ar_tab_active");
        else
            LoadARTexture(ImageWidget.Cast(m_Root.FindAnyWidget("ARImageRetrieveBtn")), "ar_tab_active");
        SetTopTabLabel("ARLabelRetrieveBtn", tab == 0);
        SetTopTabLabel("ARLabelParkBtn", tab == 1);
        SetTopTabLabel("ARFavoritesLabel", tab == 2);
        SetTopTabLabel("AROrderLabel", tab == 3);
        LoadARTexture(ImageWidget.Cast(m_Root.FindAnyWidget("ARImageConfirmBtn")), "button_blue");
        LoadARTexture(ImageWidget.Cast(m_Root.FindAnyWidget("ARImageSpawnBtn")), "button_dark");
        LoadARTexture(ImageWidget.Cast(m_Root.FindAnyWidget("ARImageDeleteBtn")), "ar_btn_red");
        LoadARTexture(ImageWidget.Cast(m_Root.FindAnyWidget("ARImagePrevPage")), "ar_button_idle");
        LoadARTexture(ImageWidget.Cast(m_Root.FindAnyWidget("ARImageNextPage")), "ar_button_idle");
        LoadARTexture(ImageWidget.Cast(m_Root.FindAnyWidget("CloseBg")), "ar_close");
        if (m_InvTabAttachBg)
            LoadARTexture(ImageWidget.Cast(m_InvTabAttachBg), "ar_inventory_tab_normal");
        if (m_InvTabCargoBg)
            LoadARTexture(ImageWidget.Cast(m_InvTabCargoBg), "ar_inventory_tab_normal");
        if (m_ShowCargoTab)
            LoadARTexture(ImageWidget.Cast(m_InvTabCargoBg), "ar_inventory_tab_active");
        else
            LoadARTexture(ImageWidget.Cast(m_InvTabAttachBg), "ar_inventory_tab_active");
        RefreshTabIcons();
    }

    AVM_StoredCarClient CopyClientCar(AVM_StoredCarClient StoredCar)
    {
        if (!StoredCar)
            return null;
        ref AVM_StoredCarClient copy = new AVM_StoredCarClient;
        copy.className = StoredCar.className;
        copy.displayName = StoredCar.displayName;
        copy.playerName = StoredCar.playerName;
        copy.carScriptId = StoredCar.carScriptId;
        copy.expiryDate = StoredCar.expiryDate;
        copy.lastPosition = StoredCar.lastPosition;
        copy.previewAttachments = new array<string>;
        copy.previewCargo = new array<string>;
        copy.previewAttachmentHealth = new array<float>;
        copy.previewCargoHealth = new array<float>;
        if (StoredCar.previewAttachments)
        {
            foreach (string part : StoredCar.previewAttachments) copy.previewAttachments.Insert(part);
        }
        if (StoredCar.previewCargo)
        {
            foreach (string cargoItem : StoredCar.previewCargo) copy.previewCargo.Insert(cargoItem);
        }
        if (StoredCar.previewAttachmentHealth)
        {
            foreach (float partHealth : StoredCar.previewAttachmentHealth) copy.previewAttachmentHealth.Insert(partHealth);
        }
        if (StoredCar.previewCargoHealth)
        {
            foreach (float cargoHealth : StoredCar.previewCargoHealth) copy.previewCargoHealth.Insert(cargoHealth);
        }
        copy.previewHealth = StoredCar.previewHealth;
        copy.previewFuel = StoredCar.previewFuel;
        copy.previewEngine = StoredCar.previewEngine;
        copy.previewTransmission = StoredCar.previewTransmission;
        copy.previewTires = StoredCar.previewTires;
        copy.storedDate = StoredCar.storedDate;
        return copy;
    }

    void SetNearbyParkCar(AVM_StoredCarClient car)
    {
        m_ServerParkCandidate = CopyClientCar(car);
    }

    float ReadHealth01(EntityAI entity, string zone)
    {
        if (!entity)
            return -1;
        float health = entity.GetHealth01(zone, "Health");
        if (health > 0)
            return health;
        float maxHealth = entity.GetMaxHealth(zone, "Health");
        if (maxHealth > 0)
            return Math.Clamp(entity.GetHealth(zone, "Health") / maxHealth, 0.0, 1.0);
        health = entity.GetHealth01(zone, "");
        if (health > 0)
            return health;
        return -1;
    }

    float Health01IfZone(EntityAI entity, string zone)
    {
        if (!entity || zone == "")
            return -1;
        string path = "CfgVehicles " + entity.GetType() + " DamageSystem DamageZones " + zone;
        if (!GetGame().ConfigIsExisting(path))
            return -1;
        return ReadHealth01(entity, zone);
    }

    AVM_StoredCarClient GetDisplayedCar()
    {
        if (!m_RetrieveMode && m_ParkCandidate)
            return m_ParkCandidate;
        if (m_SelectedVehicleIndex < 0 || m_SelectedVehicleIndex >= m_Vehicles.Count())
            return null;
        return m_Vehicles.Get(m_SelectedVehicleIndex);
    }

    AVM_StoredCarClient BuildParkCandidateFromTransport(Transport transport)
    {
        AVM_StoredCarClient car = new AVM_StoredCarClient;
        if (!transport)
            return car;
        car.className = transport.GetType();
        car.displayName = transport.GetDisplayName();
        car.playerName = AR_VehicleLock.GetOriginalOwnerName(transport);
        car.carScriptId = AR_VehicleLock.GetTransportId(transport);
        car.lastPosition = transport.GetPosition();
        car.expiryDate = "N/A";
        car.storedDate = "World";
        car.previewAttachments = new array<string>;
        car.previewCargo = new array<string>;
        car.previewAttachmentHealth = new array<float>;
        car.previewCargoHealth = new array<float>;
        car.previewHealth = ReadHealth01(transport, "");
        car.previewEngine = Health01IfZone(transport, "Engine");
        car.previewTransmission = Health01IfZone(transport, "Gearbox");
        if (car.previewTransmission < 0)
            car.previewTransmission = Health01IfZone(transport, "Transmission");
        car.previewFuel = -1;
        float fuelFraction = 0;
        float fuelCapacity = 0;
        AR_VehicleLock.GetFluidData(transport, CarFluid.FUEL, fuelFraction, fuelCapacity);
        if (fuelCapacity > 0)
            car.previewFuel = fuelFraction;
        car.previewTires = -1;
        float tireTotal = 0;
        int tireCount = 0;
        if (transport.GetInventory())
        {
            for (int attachmentIndex = 0; attachmentIndex < transport.GetInventory().AttachmentCount(); attachmentIndex++)
            {
                EntityAI part = transport.GetInventory().GetAttachmentFromIndex(attachmentIndex);
                if (!part)
                    continue;
                car.previewAttachments.Insert(part.GetType());
                float partHealth01 = ReadHealth01(part, "");
                car.previewAttachmentHealth.Insert(partHealth01);
                string partType = part.GetType();
                partType.ToLower();
                if (partType.Contains("wheel") || partType.Contains("tire"))
                {
                    if (partHealth01 >= 0)
                    {
                        tireTotal = tireTotal + partHealth01;
                        tireCount++;
                    }
                }
            }
            AVM_Helper.CollectEntityCargo(transport, car.previewCargo, car.previewCargoHealth, "");
        }
        if (tireCount > 0)
            car.previewTires = tireTotal / tireCount;
        return car;
    }

    void SnapshotNearbyParkVehicle()
    {
        m_ParkCandidate = null;
        Transport vehicle = AVM_Helper.GetNearbyTransport(m_TargetPosition, 4);
        if (!vehicle)
            return;
        int nearbyId = AR_VehicleLock.GetTransportId(vehicle);
        if (m_ServerParkCandidate && (m_ServerParkCandidate.carScriptId == nearbyId || nearbyId <= 0))
        {
            m_ParkCandidate = CopyClientCar(m_ServerParkCandidate);
            return;
        }
        m_ParkCandidate = BuildParkCandidateFromTransport(vehicle);
    }

    void ShowParkCandidateDetails()
    {
        if (!m_ParkCandidate)
            return;
        FillVehicleDetails(m_ParkCandidate);
        RefreshVehicleRows();
    }

    void ClearVehicleDetails()
    {
        Clear3DPreview();
        if (m_DetailTitle) m_DetailTitle.SetText("");
        if (m_DetailOwner) m_DetailOwner.SetText("");
        if (m_DetailId) m_DetailId.SetText("");
        if (m_DetailExpiry) m_DetailExpiry.SetText("");
        if (m_StoredDate) m_StoredDate.SetText("");
        if (m_TagOffroad) m_TagOffroad.SetText("");
        if (m_TagPersonal) m_TagPersonal.SetText("");
        if (m_DetailPosition) m_DetailPosition.SetText("");
        SetStatBar(0, -1, false);
        SetStatBar(1, -1, false);
        SetStatBar(3, -1, false);
        SetStatBar(4, -1, false);
        m_PartsNames.Clear();
        m_PartsCounts.Clear();
        m_PartsHealths.Clear();
        m_CargoNames.Clear();
        m_CargoCounts.Clear();
        m_CargoHealths.Clear();
        UpdateInventoryPages();
    }

    void FillVehicleDetails(AVM_StoredCarClient car)
    {
        if (!car)
            return;
        Show3DPreview(car.className);
        Widget inventoryOverlay = m_Root.FindAnyWidget("ARInventoryPanel");
        if (inventoryOverlay)
            inventoryOverlay.Show(true);
        if (m_DetailTitle) m_DetailTitle.SetText(car.displayName);
        if (m_DetailOwner) m_DetailOwner.SetText(car.playerName);
        if (m_DetailId) m_DetailId.SetText(car.carScriptId.ToString());
        if (m_DetailExpiry)
        {
            m_DetailExpiry.SetText(FormatExpiry(car.expiryDate));
            string expiry = car.expiryDate;
            expiry.ToLower();
            if (expiry.Contains("n/a") || expiry == "")
                m_DetailExpiry.SetColor(ARGB(255, 178, 201, 220));
            else if (expiry.Contains("hour") || expiry.Contains("min") || expiry.Contains("1 day"))
                m_DetailExpiry.SetColor(ARGB(255, 255, 64, 64));
            else if (expiry.Contains("2 day") || expiry.Contains("3 day"))
                m_DetailExpiry.SetColor(ARGB(255, 255, 209, 46));
            else
                m_DetailExpiry.SetColor(ARGB(255, 0, 230, 118));
        }
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
            m_TitleFavorite.SetText("");
        SetTitleFavoriteIcon(false, false);
        if (m_DetailPosition)
            m_DetailPosition.SetText(FormatPosition(car.lastPosition));
        if (m_InfoMessage)
            m_InfoMessage.SetText("Selected: " + car.displayName + "  |  ID " + car.carScriptId.ToString());
        SetStatBar(0, car.previewHealth, car.previewHealth >= 0);
        SetStatBar(1, car.previewEngine, car.previewEngine >= 0);
        SetStatBar(3, car.previewTires, car.previewTires >= 0);
        SetStatBar(4, car.previewFuel, car.previewFuel >= 0);
        m_PartsPage = 0;
        m_CargoPage = 0;
        UpdateInventoryPages();
    }

    void SetCardState(int row, bool selected, bool hover)
    {
        if (row < 0 || row >= m_RowCards.Count()) return;
        if (selected)
            LoadARTexture(m_RowImages.Get(row), "ar_row_selected");
        else if (hover)
            LoadARTexture(m_RowImages.Get(row), "ar_row_hover");
        else
            LoadARTexture(m_RowImages.Get(row), "ar_row_idle");
        SetPanelColor(m_RowCards.Get(row), ARGB(0, 0, 0, 0));
        if (row < m_RowAccents.Count())
            SetPanelColor(m_RowAccents.Get(row), ARGB(0, 0, 0, 0));
    }

    override bool OnMouseEnter(Widget w, int x, int y)
    {
        for (int i = 0; i < m_VehicleButtons.Count(); i++)
        {
            if (w == m_VehicleButtons.Get(i))
            {
                if (!m_RetrieveMode)
                    return true;
                if (GetVehicleIndexForRow(i) != m_SelectedVehicleIndex)
                {
                    SetCardState(i, false, true);
                    return true;
                }
            }
        }
        if (w == m_TitleFavorite)
        {
            SetTitleFavoriteIcon(true, false);
            return true;
        }
        for (int starHover = 0; starHover < m_RowStars.Count(); starHover++)
        {
            if (w == m_RowStars.Get(starHover))
            {
                SetRowFavoriteIcon(starHover, true, false);
                return true;
            }
        }
        int actionIndex = GetActionIndex(w);
        if (actionIndex >= 0)
        {
            int hoverTab = ActiveTopTab();
            if (actionIndex == 0 && hoverTab != 0) LoadARTexture(ImageWidget.Cast(m_Root.FindAnyWidget("ARImageRetrieveBtn")), "ar_nav_hover");
            else if (actionIndex == 1 && hoverTab != 1) LoadARTexture(ImageWidget.Cast(m_Root.FindAnyWidget("ARImageParkBtn")), "ar_nav_hover");
            else if (actionIndex == 2) LoadARTexture(ImageWidget.Cast(m_Root.FindAnyWidget("ARImageConfirmBtn")), "ar_button_blue_hover");
            else if (actionIndex == 3) LoadARTexture(ImageWidget.Cast(m_Root.FindAnyWidget("ARImagePrevPage")), "ar_button_dark_hover");
            else if (actionIndex == 4) LoadARTexture(ImageWidget.Cast(m_Root.FindAnyWidget("ARImageNextPage")), "ar_button_dark_hover");
            else if (actionIndex == 5) LoadARTexture(ImageWidget.Cast(m_Root.FindAnyWidget("ARImageSpawnBtn")), "ar_button_dark_hover");
            else if (actionIndex == 6) LoadARTexture(ImageWidget.Cast(m_Root.FindAnyWidget("ARImageDeleteBtn")), "ar_button_red_hover");
            else if (actionIndex == 7 && hoverTab != 2)
            {
                LoadARTexture(ImageWidget.Cast(m_Root.FindAnyWidget("ARTabFavBg")), "ar_nav_hover");
                LoadARTexture(ImageWidget.Cast(m_Root.FindAnyWidget("ARIconFav")), "ar_icon_favorite_hover");
            }
            else if (actionIndex == 8 && hoverTab != 3) LoadARTexture(ImageWidget.Cast(m_Root.FindAnyWidget("ARTabRecentBg")), "ar_nav_hover");
            else if (actionIndex == 9) LoadARTexture(ImageWidget.Cast(m_Root.FindAnyWidget("CloseBg")), "ar_close_hover");
            return true;
        }
        for (int slot = 0; slot < TILES_PER_PAGE; slot++)
        {
            if (w == m_Root.FindAnyWidget("ARPartsSelect" + slot.ToString()))
            {
                LoadARTexture(ImageWidget.Cast(m_Root.FindAnyWidget("ARPartsBg" + slot.ToString())), "ar_item_hover");
                int partHover = m_PartsPage * TILES_PER_PAGE + slot;
                if (m_InfoMessage && partHover < m_PartsNames.Count())
                {
                    float hoverHealth = -1;
                    if (partHover < m_PartsHealths.Count())
                        hoverHealth = m_PartsHealths.Get(partHover);
                    string state = ConditionLabel(hoverHealth);
                    m_InfoMessage.Show(true);
                    if (state != "")
                        m_InfoMessage.SetText(PrettyItemName(m_PartsNames.Get(partHover)) + "  |  " + state);
                    else
                        m_InfoMessage.SetText(PrettyItemName(m_PartsNames.Get(partHover)));
                }
                return true;
            }
            if (w == m_Root.FindAnyWidget("ARCargoSelect" + slot.ToString()))
            {
                LoadARTexture(ImageWidget.Cast(m_Root.FindAnyWidget("ARCargoBg" + slot.ToString())), "ar_item_hover");
                int cargoHover = m_CargoPage * TILES_PER_PAGE + slot;
                if (m_InfoMessage && cargoHover < m_CargoNames.Count())
                {
                    float hoverCargoHealth = -1;
                    if (cargoHover < m_CargoHealths.Count())
                        hoverCargoHealth = m_CargoHealths.Get(cargoHover);
                    string cargoState = ConditionLabel(hoverCargoHealth);
                    m_InfoMessage.Show(true);
                    if (cargoState != "")
                        m_InfoMessage.SetText(PrettyItemName(m_CargoNames.Get(cargoHover)) + "  |  " + cargoState);
                    else
                        m_InfoMessage.SetText(PrettyItemName(m_CargoNames.Get(cargoHover)));
                }
                return true;
            }
        }
        return super.OnMouseEnter(w, x, y);
    }

    override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
    {
        for (int i = 0; i < m_VehicleButtons.Count(); i++)
        {
            if (w == m_VehicleButtons.Get(i))
            {
                if (!m_RetrieveMode)
                {
                    SetCardState(i, i == 0 && m_ParkCandidate != null, false);
                    return true;
                }
                SetCardState(i, GetVehicleIndexForRow(i) == m_SelectedVehicleIndex, false);
                return true;
            }
        }
        if (w == m_TitleFavorite)
        {
            SetTitleFavoriteIcon(false, false);
            return true;
        }
        for (int starLeave = 0; starLeave < m_RowStars.Count(); starLeave++)
        {
            if (w == m_RowStars.Get(starLeave))
            {
                SetRowFavoriteIcon(starLeave, false, false);
                return true;
            }
        }
        int actionIndex = GetActionIndex(w);
        if (actionIndex >= 0)
        {
            RefreshActionTextures();
            return true;
        }
        for (int slot = 0; slot < TILES_PER_PAGE; slot++)
        {
            if (w == m_Root.FindAnyWidget("ARPartsSelect" + slot.ToString()) || w == m_Root.FindAnyWidget("ARCargoSelect" + slot.ToString()))
            {
                LoadARTexture(ImageWidget.Cast(m_Root.FindAnyWidget("ARPartsBg" + slot.ToString())), "ar_item_idle");
                LoadARTexture(ImageWidget.Cast(m_Root.FindAnyWidget("ARCargoBg" + slot.ToString())), "ar_item_idle");
                if (m_InfoMessage)
                    m_InfoMessage.Show(false);
                return true;
            }
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
        ref AVM_StoredCarClient copy = CopyClientCar(StoredCar);
        if (!copy)
            return;
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
        if (!m_RetrieveMode)
        {
            for (int parkRow = 0; parkRow < VEHICLES_PER_PAGE; parkRow++)
            {
                ButtonWidget parkBtn = m_VehicleButtons.Get(parkRow);
                Widget parkCard = m_RowCards.Get(parkRow);
                bool showPark = parkRow == 0 && m_ParkCandidate != null;
                if (showPark)
                {
                    if (m_RowLabels.Get(parkRow)) m_RowLabels.Get(parkRow).SetText(m_ParkCandidate.displayName);
                    if (m_RowMetas.Get(parkRow)) m_RowMetas.Get(parkRow).SetText(GetVehicleCategory(m_ParkCandidate));
                    if (m_RowIds.Get(parkRow)) m_RowIds.Get(parkRow).SetText("ID  " + m_ParkCandidate.carScriptId.ToString());
                    if (m_RowExpiries.Get(parkRow)) m_RowExpiries.Get(parkRow).SetText("Nearby");
                    if (parkBtn) parkBtn.Show(true);
                    if (parkCard) parkCard.Show(true);
                    if (m_RowStars.Get(parkRow)) m_RowStars.Get(parkRow).Show(false);
                    if (m_RowStarImages && parkRow < m_RowStarImages.Count() && m_RowStarImages.Get(parkRow)) m_RowStarImages.Get(parkRow).Show(false);
                    SetRowThumbnail(parkRow, m_ParkCandidate.className, m_ParkCandidate.previewAttachments);
                    SetCardState(parkRow, true, false);
                }
                else
                {
                    if (parkBtn) parkBtn.Show(false);
                    if (parkCard) parkCard.Show(false);
                    if (m_RowStars.Get(parkRow)) m_RowStars.Get(parkRow).Show(false);
                    if (m_RowStarImages && parkRow < m_RowStarImages.Count() && m_RowStarImages.Get(parkRow)) m_RowStarImages.Get(parkRow).Show(false);
                }
            }
            if (m_PageInfo)
                m_PageInfo.SetText("1 / 1");
            if (m_ResultCount)
            {
                if (m_ParkCandidate)
                    m_ResultCount.SetText("1 Vehicle");
                else
                    m_ResultCount.SetText("0 Vehicles");
            }
            if (m_NoResults)
                m_NoResults.Show(!m_ParkCandidate);
            if (m_PrevPage)
                m_PrevPage.Enable(false);
            if (m_NextPage)
                m_NextPage.Enable(false);
            LoadARTexture(ImageWidget.Cast(m_Root.FindAnyWidget("ARImagePrevPage")), "ar_button_dark_disabled");
            LoadARTexture(ImageWidget.Cast(m_Root.FindAnyWidget("ARImageNextPage")), "ar_button_dark_disabled");
            return;
        }
        for (int i = 0; i < VEHICLES_PER_PAGE; i++)
        {
            int index = m_Page * VEHICLES_PER_PAGE + i;
            ButtonWidget btn = m_VehicleButtons.Get(i);
            Widget card = m_RowCards.Get(i);
            if (index < count)
            {
                int actualIndex = GetVehicleIndexForRow(i);
                AVM_StoredCarClient car = m_Vehicles.Get(actualIndex);
                if (m_RowLabels.Get(i)) m_RowLabels.Get(i).SetText(car.displayName);
                if (m_RowMetas.Get(i)) m_RowMetas.Get(i).SetText(GetVehicleCategory(car));
                if (m_RowIds.Get(i)) m_RowIds.Get(i).SetText("ID  " + car.carScriptId.ToString());
                if (m_RowExpiries.Get(i)) m_RowExpiries.Get(i).SetText(FormatExpiry(car.expiryDate));
                if (btn) btn.Show(true);
                if (card) card.Show(true);
                if (m_RowStars.Get(i))
                    m_RowStars.Get(i).Show(true);
                if (m_RowStarImages.Get(i))
                    m_RowStarImages.Get(i).Show(true);
                SetRowFavoriteIcon(i, false, false);
                SetRowThumbnail(i, car.className, car.previewAttachments);
                SetCardState(i, actualIndex == m_SelectedVehicleIndex, false);
            }
            else
            {
                if (btn) btn.Show(false);
                if (card) card.Show(false);
                if (m_RowStars.Get(i)) m_RowStars.Get(i).Show(false);
                if (m_RowStarImages && i < m_RowStarImages.Count() && m_RowStarImages.Get(i)) m_RowStarImages.Get(i).Show(false);
            }
        }
        if (m_PageInfo)
            m_PageInfo.SetText((m_Page + 1).ToString() + " / " + (maxPage + 1).ToString());
        if (m_ResultCount)
            m_ResultCount.SetText(count.ToString() + " Vehicles");
        if (m_NoResults)
            m_NoResults.Show(count == 0);
        if (m_PrevPage)
            m_PrevPage.Enable(m_Page > 0);
        if (m_NextPage)
            m_NextPage.Enable(m_Page < maxPage);
        if (m_Page > 0)
            LoadARTexture(ImageWidget.Cast(m_Root.FindAnyWidget("ARImagePrevPage")), "ar_button_dark_idle");
        else
            LoadARTexture(ImageWidget.Cast(m_Root.FindAnyWidget("ARImagePrevPage")), "ar_button_dark_disabled");
        if (m_Page < maxPage)
            LoadARTexture(ImageWidget.Cast(m_Root.FindAnyWidget("ARImageNextPage")), "ar_button_dark_idle");
        else
            LoadARTexture(ImageWidget.Cast(m_Root.FindAnyWidget("ARImageNextPage")), "ar_button_dark_disabled");
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

    void SetRowThumbnail(int row, string vehicleClass, array<string> storedParts)
    {
        if (row < 0 || row >= m_RowPreviews.Count() || vehicleClass == "")
            return;
        ItemPreviewWidget preview = m_RowPreviews.Get(row);
        if (!preview)
            return;
        EntityAI entity = SpawnLocalPreview(vehicleClass);
        if (!entity)
            return;
        entity.OnDebugSpawn();
        AssemblePreviewVehicle(entity, storedParts);
        FreezePreviewHierarchy(entity);
        m_RowPreviewEntities.Insert(entity);
        preview.SetForceFlipEnable(true);
        preview.SetForceFlip(false);
        preview.SetItem(entity);
        preview.SetView(entity.GetViewIndex());
        preview.SetModelOrientation(Vector(0, 40, 0));
        preview.SetModelPosition(Vector(0, 0, 0));
        preview.Show(true);
    }

    void SetStatBar(int index, float fraction, bool known)
    {
        if (index < 0 || index >= m_StatValues.Count())
            return;
        if (!known || fraction < 0)
        {
            if (m_StatValues.Get(index)) m_StatValues.Get(index).SetText("N/A");
            if (m_StatFills.Get(index))
                m_StatFills.Get(index).SetSize(0, 1);
            return;
        }
        float clamped = Math.Clamp(fraction, 0.0, 1.0);
        int pct = Math.Round(clamped * 100);
        if (m_StatValues.Get(index)) m_StatValues.Get(index).SetText(pct.ToString() + "%");
        string fillTex = "ar_stat_green";
        if (index == 4)
            fillTex = "ar_stat_cyan";
        else if (clamped < 0.40)
            fillTex = "ar_stat_red";
        else if (clamped < 0.55)
            fillTex = "ar_stat_orange";
        else if (clamped < 0.75)
            fillTex = "ar_stat_yellow";
        LoadARTexture(ImageWidget.Cast(m_Root.FindAnyWidget("ARStatTrackImg" + index.ToString())), "ar_stat_track");
        LoadARTexture(ImageWidget.Cast(m_StatFills.Get(index)), fillTex);
        if (m_StatFills.Get(index))
            m_StatFills.Get(index).SetSize(clamped, 1);
    }

    void AggregateItems(array<string> source, array<float> sourceHealth, array<string> names, array<int> counts, array<float> healths)
    {
        names.Clear();
        counts.Clear();
        healths.Clear();
        if (!source)
            return;
        for (int i = 0; i < source.Count(); i++)
        {
            string key = NormalizeClassName(source.Get(i));
            if (key == "")
                continue;
            float health01 = -1;
            if (sourceHealth && i < sourceHealth.Count())
                health01 = sourceHealth.Get(i);
            int found = names.Find(key);
            if (found == -1)
            {
                names.Insert(key);
                counts.Insert(1);
                healths.Insert(health01);
            }
            else
            {
                counts.Set(found, counts.Get(found) + 1);
                float existing = healths.Get(found);
                if (health01 >= 0 && (existing < 0 || health01 < existing))
                    healths.Set(found, health01);
            }
        }
    }

    string ConditionLabel(float fraction)
    {
        if (fraction < 0)
            return "";
        if (fraction > 0.70)
            return "PRISTINE";
        if (fraction > 0.50)
            return "WORN";
        if (fraction > 0.30)
            return "DAMAGED";
        if (fraction > 0.0)
            return "BADLY DAMAGED";
        return "RUINED";
    }

    int ConditionColor(float fraction)
    {
        if (fraction < 0)
            return ARGB(255, 178, 201, 220);
        if (fraction > 0.70)
            return ARGB(255, 0, 230, 118);
        if (fraction > 0.50)
            return ARGB(255, 255, 209, 47);
        if (fraction > 0.30)
            return ARGB(255, 255, 138, 36);
        if (fraction > 0.0)
            return ARGB(255, 255, 64, 64);
        return ARGB(255, 156, 45, 45);
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

    // Client-local copy only. Same spawn/attach/freeze order as COT inventory preview.
    // Never ECE_SETUP (that parks a real car) and never a world staticcamera.
    void Clear3DPreview()
    {
        GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(FinishPreviewAssemble);
        if (m_Vehicle3DPreview)
            m_Vehicle3DPreview.SetItem(null);
        if (m_PreviewEntity)
        {
            GetGame().ObjectDelete(m_PreviewEntity);
            m_PreviewEntity = null;
        }
        if (m_PreviewStatus)
        {
            m_PreviewStatus.Show(true);
            m_PreviewStatus.SetText("VEHICLE PREVIEW");
        }
    }

    EntityAI SpawnLocalPreview(string typeName)
    {
        if (typeName == "")
            return null;

        vector pos = vector.Zero;
        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        if (player)
        {
            pos = player.GetPosition();
            pos[0] = pos[0] + 400;
            pos[1] = pos[1] + 80;
            pos[2] = pos[2] + 400;
        }

        EntityAI ent = EntityAI.Cast(GetGame().CreateObjectEx(typeName, pos, ECE_LOCAL | ECE_NOLIFETIME | ECE_CREATEPHYSICS));
        if (!ent)
            ent = EntityAI.Cast(GetGame().CreateObject(typeName, pos, true, false, true));
        if (ent)
        {
            ent.SetAllowDamage(false);
            if (ent.HasSelection("antiwater"))
                ent.HideSelection("antiwater");
        }
        return ent;
    }

    void FreezePreviewHierarchy(EntityAI entity)
    {
        if (!entity)
            return;
        entity.SetAllowDamage(false);
        Transport transport;
        if (Class.CastTo(transport, entity))
            transport.DisableSimulation(true);
        else
            entity.DisableSimulation(true);
        if (dBodyIsSet(entity))
        {
            dBodyActive(entity, ActiveState.INACTIVE);
            dBodyDynamic(entity, false);
        }
        GameInventory inventory = entity.GetInventory();
        if (!inventory)
            return;
        for (int i = 0; i < inventory.AttachmentCount(); i++)
            FreezePreviewHierarchy(inventory.GetAttachmentFromIndex(i));
    }

    void BindPreviewWidget(ItemPreviewWidget preview, EntityAI ent)
    {
        if (!preview || !ent)
            return;
        preview.SetSort(0);
        preview.SetForceFlipEnable(true);
        preview.SetForceFlip(false);
        preview.SetItem(null);
        preview.SetItem(ent);
        int viewIndex = ent.GetViewIndex();
        if (viewIndex < 0)
            viewIndex = 0;
        preview.SetView(viewIndex);
        preview.SetModelOrientation(Vector(0, m_PreviewYaw, 0));
        preview.SetModelPosition(Vector(0, 0, m_PreviewZoom));
        preview.Show(true);
    }

    void RefreshBoundPreview()
    {
        if (m_Vehicle3DPreview && m_PreviewEntity)
            BindPreviewWidget(m_Vehicle3DPreview, m_PreviewEntity);
    }

    void Show3DPreview(string vehicleClass)
    {
        Clear3DPreview();
        m_PreviewYaw = 35;
        m_PreviewZoom = 0.0;
        m_IsDraggingPreview = false;
        m_InspectingItem = false;
        if (vehicleClass == "")
            return;

        m_PreviewEntity = SpawnLocalPreview(vehicleClass);
        if (!m_PreviewEntity)
            return;

        BindPreviewWidget(m_Vehicle3DPreview, m_PreviewEntity);
        GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(FinishPreviewAssemble, 80, false);
        GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(FinishPreviewAssemble, 220, false);
        if (m_PreviewStatus)
            m_PreviewStatus.Show(false);
    }

    void FinishPreviewAssemble()
    {
        if (!m_PreviewEntity)
            return;

        array<string> storedParts;
        AVM_StoredCarClient displayed = GetDisplayedCar();
        if (displayed)
            storedParts = displayed.previewAttachments;
        AssemblePreviewVehicle(m_PreviewEntity, storedParts);
        FreezePreviewHierarchy(m_PreviewEntity);
        BindPreviewWidget(m_Vehicle3DPreview, m_PreviewEntity);
    }

    int PreviewAttachmentCount(EntityAI vehicle)
    {
        if (!vehicle || !vehicle.GetInventory())
            return 0;
        return vehicle.GetInventory().AttachmentCount();
    }

    void AssemblePreviewVehicle(EntityAI vehicle, array<string> storedParts)
    {
        if (!vehicle)
            return;

        if (PreviewAttachmentCount(vehicle) == 0)
            vehicle.OnDebugSpawn();

        if (PreviewAttachmentCount(vehicle) == 0 && storedParts)
        {
            foreach (string partClass : storedParts)
                AttachPreviewPart(vehicle, partClass);
        }

        StripPreviewCargo(vehicle);
    }

    bool AttachPreviewPart(EntityAI vehicle, string partClass)
    {
        if (partClass == "" || partClass.Contains("  + ") || !vehicle.GetInventory())
            return false;
        if (PreviewHasPart(vehicle, partClass))
            return true;
        EntityAI part = vehicle.GetInventory().CreateAttachment(partClass);
        if (!part)
        {
            EntityAI spawned = EntityAI.Cast(GetGame().CreateObjectEx(partClass, vehicle.GetPosition(), ECE_LOCAL | ECE_NOLIFETIME));
            if (spawned)
            {
                bool taken = vehicle.LocalTakeEntityToTargetAttachment(vehicle, spawned);
                if (!taken)
                {
                    GetGame().ObjectDelete(spawned);
                    return false;
                }
                part = spawned;
            }
        }
        return part != null;
    }

    bool PreviewHasPart(EntityAI vehicle, string partClass)
    {
        if (!vehicle || !vehicle.GetInventory())
            return false;
        for (int i = 0; i < vehicle.GetInventory().AttachmentCount(); i++)
        {
            EntityAI part = vehicle.GetInventory().GetAttachmentFromIndex(i);
            if (part && part.GetType() == partClass)
                return true;
        }
        return false;
    }

    void StripPreviewCargo(EntityAI vehicle)
    {
        if (!vehicle || !vehicle.GetInventory())
            return;
        CargoBase cargo = vehicle.GetInventory().GetCargo();
        if (!cargo)
            return;
        for (int i = cargo.GetItemCount() - 1; i >= 0; i--)
        {
            EntityAI item = cargo.GetItem(i);
            if (item)
                GetGame().ObjectDelete(item);
        }
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
        if (!widget) return;
        string tileBg = "ARPartsBg" + slot.ToString();
        if (isCargo) tileBg = "ARCargoBg" + slot.ToString();
        if (spawnClass == "")
        {
            LoadARTexture(ImageWidget.Cast(m_Root.FindAnyWidget(tileBg)), "ar_item_disabled");
            return;
        }
        LoadARTexture(ImageWidget.Cast(m_Root.FindAnyWidget(tileBg)), "ar_item_idle");
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
        m_PreviewEntity = SpawnLocalPreview(className);
        if (!m_PreviewEntity) return;
        FreezePreviewHierarchy(m_PreviewEntity);
        BindPreviewWidget(m_Vehicle3DPreview, m_PreviewEntity);
        if (m_PreviewStatus) m_PreviewStatus.SetText("ITEM PREVIEW / " + className);
    }

    void RefreshPreviewOrientation()
    {
        if (m_Vehicle3DPreview && m_PreviewEntity)
        {
            m_Vehicle3DPreview.SetModelOrientation(Vector(0, m_PreviewYaw, 0));
            m_Vehicle3DPreview.SetModelPosition(Vector(0, 0, m_PreviewZoom));
        }
    }

    void StopPreviewDrag()
    {
        m_IsDraggingPreview = false;
    }

    override void Update(float timeslice)
    {
        super.Update(timeslice);
        if (!m_IsDraggingPreview)
            return;
        if (GetMouseState(MouseState.LEFT) == 0)
        {
            StopPreviewDrag();
            return;
        }
        if (m_Vehicle3DPreview && m_PreviewEntity)
        {
            int mx;
            int my;
            GetMousePos(mx, my);
            float dx = mx - m_LastDragX;
            m_LastDragX = mx;
            if (dx != 0)
            {
                m_PreviewYaw = m_PreviewYaw + (dx * 0.55);
                RefreshPreviewOrientation();
            }
        }
    }

    override bool OnMouseButtonDown(Widget w, int x, int y, int button)
    {
        if (button == 0 && (w == m_PreviewDrag || w == m_PreviewFrame || w == m_Vehicle3DPreview))
        {
            int mx;
            int my;
            GetMousePos(mx, my);
            m_LastDragX = mx;
            m_IsDraggingPreview = true;
            return true;
        }
        if (button == 0 && w == m_TitleFavorite)
        {
            SetTitleFavoriteIcon(true, true);
            return super.OnMouseButtonDown(w, x, y, button);
        }
        for (int starPress = 0; starPress < m_RowStars.Count(); starPress++)
        {
            if (button == 0 && w == m_RowStars.Get(starPress))
            {
                SetRowFavoriteIcon(starPress, true, true);
                return super.OnMouseButtonDown(w, x, y, button);
            }
        }
        if (button == 0)
        {
            int pressedAction = GetActionIndex(w);
            if (pressedAction == 2) LoadARTexture(ImageWidget.Cast(m_Root.FindAnyWidget("ARImageConfirmBtn")), "ar_button_blue_pressed");
            else if (pressedAction == 5) LoadARTexture(ImageWidget.Cast(m_Root.FindAnyWidget("ARImageSpawnBtn")), "ar_button_dark_pressed");
            else if (pressedAction == 6) LoadARTexture(ImageWidget.Cast(m_Root.FindAnyWidget("ARImageDeleteBtn")), "ar_button_red_pressed");
        }
        return super.OnMouseButtonDown(w, x, y, button);
    }

    override bool OnMouseButtonUp(Widget w, int x, int y, int button)
    {
        if (button == 0)
            StopPreviewDrag();
        RefreshActionTextures();
        SetTitleFavoriteIcon(false, false);
        for (int starUp = 0; starUp < m_RowStarImages.Count(); starUp++)
            SetRowFavoriteIcon(starUp, false, false);
        return super.OnMouseButtonUp(w, x, y, button);
    }

    bool IsUnderWidget(Widget w, Widget ancestor)
    {
        if (!w || !ancestor)
            return false;
        Widget cur = w;
        while (cur)
        {
            if (cur == ancestor)
                return true;
            cur = cur.GetParent();
        }
        return false;
    }

    int InventoryMaxPage(int itemCount)
    {
        if (itemCount <= 0)
            return 0;
        return (itemCount - 1) / TILES_PER_PAGE;
    }

    void ScrollInventory(int dir)
    {
        if (m_ShowCargoTab)
        {
            int cargoMax = InventoryMaxPage(m_CargoNames.Count());
            m_CargoPage = m_CargoPage + dir;
            if (m_CargoPage < 0)
                m_CargoPage = 0;
            if (m_CargoPage > cargoMax)
                m_CargoPage = cargoMax;
        }
        else
        {
            int partsMax = InventoryMaxPage(m_PartsNames.Count());
            m_PartsPage = m_PartsPage + dir;
            if (m_PartsPage < 0)
                m_PartsPage = 0;
            if (m_PartsPage > partsMax)
                m_PartsPage = partsMax;
        }
        UpdateInventoryPages();
    }

    override bool OnMouseWheel(Widget w, int x, int y, int wheel)
    {
        if (w == m_PreviewDrag || w == m_PreviewFrame)
        {
            m_PreviewZoom = Math.Clamp(m_PreviewZoom - (wheel * 0.04), -0.08, 0.12);
            RefreshPreviewOrientation();
            return true;
        }
        Widget inventoryPanel = m_Root.FindAnyWidget("ARInventoryPanel");
        if (IsUnderWidget(w, inventoryPanel) || IsUnderWidget(w, m_PartsPanel) || IsUnderWidget(w, m_CargoPanel))
        {
            int dir = 1;
            if (wheel > 0)
                dir = -1;
            ScrollInventory(dir);
            return true;
        }
        return super.OnMouseWheel(w, x, y, wheel);
    }

    override bool OnFocus(Widget w, int x, int y)
    {
        if (w == m_SearchBox)
            LoadARTexture(ImageWidget.Cast(m_Root.FindAnyWidget("SearchFrame")), "ar_search_focus");
        return super.OnFocus(w, x, y);
    }

    override bool OnFocusLost(Widget w, int x, int y)
    {
        if (w == m_SearchBox)
            LoadARTexture(ImageWidget.Cast(m_Root.FindAnyWidget("SearchFrame")), "ar_search_idle");
        return super.OnFocusLost(w, x, y);
    }

    void UpdateInventoryPages()
    {
        ClearInventoryTiles();
        AVM_StoredCarClient car = GetDisplayedCar();
        if (!car) return;
        AggregateItems(car.previewAttachments, car.previewAttachmentHealth, m_PartsNames, m_PartsCounts, m_PartsHealths);
        AggregateItems(car.previewCargo, car.previewCargoHealth, m_CargoNames, m_CargoCounts, m_CargoHealths);
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
                    float partHealth = -1;
                    if (partIndex < m_PartsHealths.Count())
                        partHealth = m_PartsHealths.Get(partIndex);
                    string partState = ConditionLabel(partHealth);
                    if (partState == "")
                    {
                        if (m_PartsCounts.Get(partIndex) > 1)
                            m_PartsQtys.Get(i).SetText("x" + m_PartsCounts.Get(partIndex).ToString());
                        else
                            m_PartsQtys.Get(i).SetText("EQUIPPED");
                        m_PartsQtys.Get(i).SetColor(ARGB(255, 178, 201, 220));
                    }
                    else
                    {
                        if (m_PartsCounts.Get(partIndex) > 1)
                            m_PartsQtys.Get(i).SetText(partState + "  x" + m_PartsCounts.Get(partIndex).ToString());
                        else
                            m_PartsQtys.Get(i).SetText(partState);
                        m_PartsQtys.Get(i).SetColor(ConditionColor(partHealth));
                    }
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
                if (m_CargoQtys.Get(i))
                {
                    float cargoHealth = -1;
                    if (cargoIndex < m_CargoHealths.Count())
                        cargoHealth = m_CargoHealths.Get(cargoIndex);
                    string cargoState = ConditionLabel(cargoHealth);
                    string cargoQty = "x" + m_CargoCounts.Get(cargoIndex).ToString();
                    if (cargoState == "")
                    {
                        m_CargoQtys.Get(i).SetText(cargoQty);
                        m_CargoQtys.Get(i).SetColor(ARGB(255, 178, 201, 220));
                    }
                    else
                    {
                        m_CargoQtys.Get(i).SetText(cargoState + "  " + cargoQty);
                        m_CargoQtys.Get(i).SetColor(ConditionColor(cargoHealth));
                    }
                }
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

    void ShowNamed(string widgetName, bool show)
    {
        Widget w = m_Root.FindAnyWidget(widgetName);
        if (w)
            w.Show(show);
    }

    void RefreshInventoryTab()
    {
        bool showCargo = m_ShowCargoTab;
        if (m_PartsPanel)
            m_PartsPanel.Show(!showCargo);
        if (m_CargoPanel)
            m_CargoPanel.Show(showCargo);
        ShowNamed("ARImagePartsPrev", false);
        ShowNamed("ARPartsPrevious", false);
        ShowNamed("ARPartsPage", false);
        ShowNamed("ARImagePartsNext", false);
        ShowNamed("ARPartsNext", false);
        ShowNamed("ARImageCargoPrev", false);
        ShowNamed("ARCargoPrevious", false);
        ShowNamed("ARCargoPage", false);
        ShowNamed("ARImageCargoNext", false);
        ShowNamed("ARCargoNext", false);
        LoadARTexture(ImageWidget.Cast(m_InvTabAttachBg), "ar_inventory_tab_normal");
        LoadARTexture(ImageWidget.Cast(m_InvTabCargoBg), "ar_inventory_tab_normal");
        if (showCargo)
        {
            LoadARTexture(ImageWidget.Cast(m_InvTabCargoBg), "ar_inventory_tab_active");
            if (m_InvTabAttachLabel)
                m_InvTabAttachLabel.SetColor(ARGB(255, 178, 201, 220));
            if (m_InvTabCargoLabel)
                m_InvTabCargoLabel.SetColor(ARGB(255, 38, 199, 255));
        }
        else
        {
            LoadARTexture(ImageWidget.Cast(m_InvTabAttachBg), "ar_inventory_tab_active");
            if (m_InvTabAttachLabel)
                m_InvTabAttachLabel.SetColor(ARGB(255, 38, 199, 255));
            if (m_InvTabCargoLabel)
                m_InvTabCargoLabel.SetColor(ARGB(255, 178, 201, 220));
        }
        RefreshTabIcons();
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
        FillVehicleDetails(m_Vehicles.Get(index));
        RefreshVehicleRows();
    }

	override bool OnClick(Widget w, int x, int y, int button) 
	{
        if (w == m_PreviewBack)
        {
            AVM_StoredCarClient previewCar = GetDisplayedCar();
            if (previewCar)
                Show3DPreview(previewCar.className);
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
            SelectTopTab(2);
            return true;
        }
        if (w == m_ReverseOrder)
        {
            SelectTopTab(3);
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
        if (w == m_ConfirmDeleteCancel || w == m_Root.FindAnyWidget("ARConfirmDeleteBlocker"))
        {
            HideDeleteConfirm();
            return true;
        }
        if (w == m_ConfirmDeleteOk)
        {
            ExecuteDeleteVehicle();
            return true;
        }
        if (w == m_DeleteBtn)
        {
            ShowDeleteConfirm();
            return true;
        }

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
                if (!m_RetrieveMode)
                {
                    ShowParkCandidateDetails();
                    return true;
                }
                SelectVehicle(GetVehicleIndexForRow(i));
                return true;
            }
        }

		switch (w)
		{
		case m_BtnPark:
			SelectTopTab(1);
			return true;
		case m_BtnRetrieve:
			SelectTopTab(0);
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
			m_BtnPark.Enable(true);
            m_BtnRetrieve.Enable(true);
			m_ParkCandidate = null;
			RefreshVehicleRows();
			m_ParkVehicleList.Show(false);
			m_ConfirmLabel.SetText("RETRIEVE VEHICLE");
            TextWidget confirmSub = TextWidget.Cast(m_Root.FindAnyWidget("ARConfirmSub"));
            if (confirmSub) confirmSub.SetText("Spawn at this location");
            Widget details = m_Root.FindAnyWidget("DetailsPanel");
            if (details) details.Show(true);
            if (m_SelectedVehicleIndex >= 0 && m_SelectedVehicleIndex < m_Vehicles.Count())
                FillVehicleDetails(m_Vehicles.Get(m_SelectedVehicleIndex));
		}
		else
		{			
			m_BtnPark.Enable(true);
            m_BtnRetrieve.Enable(true);
            m_ConfirmLabel.SetText("PARK VEHICLE");
            TextWidget parkSub = TextWidget.Cast(m_Root.FindAnyWidget("ARConfirmSub"));
            if (parkSub) parkSub.SetText("Store nearby vehicle in this garage");
            Widget parkDetails = m_Root.FindAnyWidget("DetailsPanel");
            if (parkDetails) parkDetails.Show(true);
            SnapshotNearbyParkVehicle();
            RefreshVehicleRows();
			if(m_ParkCandidate)
			{
                ShowParkCandidateDetails();
			}
			else
			{
                ClearVehicleDetails();
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
				if(!AVM_Helper.PlayerHasMasterKey(player))
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

    void ShowDeleteConfirm()
    {
        if (m_SelectedVehicleIndex < 0 || m_SelectedVehicleIndex >= m_Vehicles.Count())
        {
            m_ErrorPanel.Show(true);
            m_ErrorText.SetText("Select a vehicle first.");
            return;
        }
        AVM_StoredCarClient car = m_Vehicles.Get(m_SelectedVehicleIndex);
        if (m_ConfirmDeleteBody)
        {
            string storageName = "MongoDB";
            ApexVehicleManagementSettings settings = g_Game.GetApexVehicleManagementSettings();
            if (settings && !settings.UseMongoDB)
                storageName = "local storage";
            m_ConfirmDeleteBody.SetText("Permanently remove " + car.displayName + " (ID " + car.carScriptId.ToString() + ") from " + storageName + ". This cannot be undone.");
        }
        if (m_Vehicle3DPreview)
            m_Vehicle3DPreview.Show(false);
        if (m_ConfirmDeletePanel)
        {
            m_ConfirmDeletePanel.SetSort(200);
            m_ConfirmDeletePanel.Show(true);
        }
    }

    void HideDeleteConfirm()
    {
        if (m_ConfirmDeletePanel)
            m_ConfirmDeletePanel.Show(false);
        if (m_Vehicle3DPreview && m_PreviewEntity)
            BindPreviewWidget(m_Vehicle3DPreview, m_PreviewEntity);
    }

    void ExecuteDeleteVehicle()
    {
        if (m_SelectedVehicleIndex < 0 || m_SelectedVehicleIndex >= m_Vehicles.Count())
            return;
        AVM_StoredCarClient car = m_Vehicles.Get(m_SelectedVehicleIndex);
        Param1<int> rpc_params = new Param1<int>(car.carScriptId);
        GetGame().RPCSingleParam(GetGame().GetPlayer(), AVM_RPCs.RPC_DELETECAR, rpc_params, true, GetGame().GetPlayer().GetIdentity());
        m_Vehicles.Remove(m_SelectedVehicleIndex);
        m_SelectedVehicleIndex = -1;
        HideDeleteConfirm();
        Clear3DPreview();
        RebuildSearchResults();
        m_ErrorPanel.Show(true);
        m_ErrorText.SetText("Vehicle removed from garage.");
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
		super.OnShow();
		GetGame().GetUIManager().ShowCursor(true);
		if (GetGame().GetMission())
		{
			GetGame().GetMission().PlayerControlDisable(INPUT_EXCLUDE_ALL);
			GetGame().GetMission().GetHud().Show(false);
		}
	}

	override void OnHide() 
	{
        m_IsDraggingPreview = false;
        Clear3DPreview();
        ClearRowThumbnails();
        ClearInventoryTiles();
		super.OnHide();
		PPEffects.SetBlurMenu(0);
		if (GetGame().GetMission())
		{
			GetGame().GetMission().PlayerControlEnable(true);
			GetGame().GetMission().GetHud().Show(true);
		}
		GetGame().GetUIManager().ShowCursor(false);
	}
}
#endif