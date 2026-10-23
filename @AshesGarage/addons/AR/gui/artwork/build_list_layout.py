from pathlib import Path

def img(name, path, pos, size, extra=""):
    return f'''        ImageWidgetClass {name} {{
         ignorepointer 1
         position {pos}
         size {size}
         image0 "AshesReborn/gui/textures/{path}.paa"
         mode blend
         "src alpha" 1
         "stretch mode" stretch_w_h
{extra}        }}
'''

def txt(name, pos, size, text, font="gui/fonts/metronbook16", color="0.96 0.97 1 1", extra=""):
    return f'''        TextWidgetClass {name} {{
         ignorepointer 1
         position {pos}
         size {size}
         text "{text}"
         font "{font}"
         "text color" {color}
{extra}        }}
'''

def btn(name, pos, size, visible=""):
    vis = f"         visible {visible}\n" if visible != "" else ""
    return f'''        ButtonWidgetClass {name} {{
{vis}         position {pos}
         size {size}
         text ""
         color 0 0 0 0
         style Empty
        }}
'''

rows = []
for i in range(6):
    y = 0.175 + i * 0.108
    rows.append(f'''      PanelWidgetClass ARRowCard{i} {{
       color 0 0 0 0
       position 0.04 {y:.3f}
       size 0.92 0.100
       {{
        ImageWidgetClass ARRowImage{i} {{ ignorepointer 1 position 0 0 size 1 1 image0 "AshesReborn/gui/textures/ar_row_idle.paa" mode blend "src alpha" 1 "stretch mode" stretch_w_h }}
        PanelWidgetClass ARRowAccent{i} {{ ignorepointer 1 position 0.012 0.18 size 0.012 0.64 color 0 0.75 1 0 }}
        ItemPreviewWidgetClass ARRowPreview{i} {{ ignorepointer 1 keepsafezone 0 position 0.03 0.10 size 0.18 0.80 }}
        TextWidgetClass ARRowLabel{i} {{ ignorepointer 1 position 0.23 0.08 size 0.62 0.38 text "" font "gui/fonts/metronbook-bold16" "text color" 0.96 0.97 1 1 }}
        TextWidgetClass ARRowMeta{i} {{ ignorepointer 1 position 0.23 0.46 size 0.40 0.24 text "" font "gui/fonts/metronbook14" "text color" 0.70 0.79 0.86 1 }}
        TextWidgetClass ARRowId{i} {{ ignorepointer 1 position 0.23 0.70 size 0.40 0.22 text "" font "gui/fonts/metronbook14" "text color" 0.51 0.61 0.70 1 }}
        TextWidgetClass ARRowExpiry{i} {{ ignorepointer 1 position 0.58 0.70 size 0.28 0.22 text "" font "gui/fonts/metronbook14" "text color" 0.00 0.90 0.36 1 }}
        ButtonWidgetClass ARRowStar{i} {{ position 0.86 0.22 size 0.11 0.56 text "*" font "gui/fonts/metronbook-bold18" "text color" 0.70 0.79 0.86 1 color 0 0 0 0 style Empty }}
        ButtonWidgetClass VehicleRow{i} {{ position 0 0 size 0.86 1 text "" color 0 0 0 0 style Empty }}
       }}
      }}''')

def tiles(prefix, y0):
    parts = []
    for i in range(6):
        x = 0.02 + i * 0.162
        parts.append(f'''       PanelWidgetClass {prefix}Slot{i} {{
        color 0 0 0 0
        position {x:.3f} {y0}
        size 0.150 0.78
        {{
         ImageWidgetClass {prefix}Bg{i} {{ ignorepointer 1 position 0 0 size 1 1 image0 "AshesReborn/gui/textures/ar_tile_idle.paa" mode blend "src alpha" 1 "stretch mode" stretch_w_h }}
         ItemPreviewWidgetClass {prefix}Preview{i} {{ ignorepointer 1 keepsafezone 0 position 0.12 0.06 size 0.76 0.52 }}
         TextWidgetClass {prefix}Label{i} {{ ignorepointer 1 position 0.04 0.60 size 0.92 0.20 text "" font "gui/fonts/metronbook14" "text color" 0.96 0.97 1 1 "text halign" center }}
         TextWidgetClass {prefix}Qty{i} {{ ignorepointer 1 position 0.04 0.80 size 0.92 0.16 text "" font "gui/fonts/metronbook14" "text color" 0.00 0.90 0.36 1 "text halign" center }}
         ButtonWidgetClass {prefix}Select{i} {{ position 0 0 size 1 1 text "" color 0 0 0 0 style Empty }}
        }}
       }}''')
    return "\n".join(parts)

stats = []
labels = ["HEALTH", "ENGINE", "GEARBOX", "TIRES", "FUEL"]
for i, lab in enumerate(labels):
    y = 0.16 + i * 0.16
    stats.append(f'''       TextWidgetClass ARStatLabel{i} {{ ignorepointer 1 position 0.08 {y:.3f} size 0.50 0.055 text "{lab}" font "gui/fonts/metronbook14" "text color" 0.70 0.79 0.86 1 }}
       TextWidgetClass ARStatValue{i} {{ ignorepointer 1 position 0.58 {y:.3f} size 0.34 0.055 text "N/A" font "gui/fonts/metronbook14" "text color" 0.96 0.97 1 1 "text halign" right }}
       PanelWidgetClass ARStatTrack{i} {{
        color 0.15 0.23 0.30 1
        position 0.08 {y+0.055:.3f}
        size 0.84 0.035
        style UIDefaultPanel
        {{
         PanelWidgetClass ARStatFill{i} {{ ignorepointer 1 color 0.00 0.90 0.46 1 position 0 0 size 0 1 style UIDefaultPanel }}
        }}
       }}''')

layout = f'''FrameWidgetClass ContainerWidget {{
 clipchildren 0
 position 0 0
 size 1 1
 hexactpos 0
 vexactpos 0
 hexactsize 0
 vexactsize 0
 {{
  PanelWidgetClass AVMDimmer {{
   ignorepointer 1
   color 0.02 0.04 0.07 0.45
   position 0 0
   size 1 1
   style UIDefaultPanel
  }}
  ImageWidgetClass AVMChrome {{
   ignorepointer 1
   position 0.035 0.025
   size 0.93 0.95
   image0 "AshesReborn/gui/textures/ar_dash_bg.paa"
   mode blend
   "src alpha" 1
   "stretch mode" stretch_w_h
  }}
  ImageWidgetClass AVMBackdrop {{ visible 0 ignorepointer 1 position 0 0 size 0.01 0.01 }}
  PanelWidgetClass AVMBorder {{ visible 0 ignorepointer 1 position 0 0 size 0.01 0.01 }}
  FrameWidgetClass MainWindowWidget {{
   clipchildren 0
   position 0.035 0.025
   size 0.93 0.95
   hexactpos 0
   vexactpos 0
   hexactsize 0
   vexactsize 0
   {{
    ImageWidgetClass HeaderPanel {{
     ignorepointer 1
     position 0.016 0.018
     size 0.968 0.100
     image0 "AshesReborn/gui/textures/ar_header.paa"
     mode blend
     "src alpha" 1
     "stretch mode" stretch_w_h
    }}
    TextWidgetClass ApexBrandAR {{
     ignorepointer 1
     position 0.028 0.028
     size 0.06 0.048
     text "AR"
     font "gui/fonts/metronbook-bold22"
     "text color" 0.96 0.97 1 1
    }}
    TextWidgetClass ApexTitle {{
     ignorepointer 1
     position 0.082 0.028
     size 0.28 0.048
     text "/  ASHES REBORN"
     font "gui/fonts/metronbook-bold22"
     "text color" 0.15 0.78 1 1
    }}
    TextWidgetClass ApexSubtitle {{
     ignorepointer 1
     position 0.028 0.078
     size 0.32 0.028
     text "VEHICLE MANAGEMENT / CROSS-SERVER GARAGE"
     font "gui/fonts/metronbook14"
     "text color" 0.65 0.76 0.85 1
    }}
    ImageWidgetClass ARImageRetrieveBtn {{
     ignorepointer 1
     position 0.36 0.028
     size 0.12 0.078
     image0 "AshesReborn/gui/textures/ar_tab_idle.paa"
     mode blend
     "src alpha" 1
     "stretch mode" stretch_w_h
    }}
    TextWidgetClass ARLabelRetrieveBtn {{
     ignorepointer 1
     position 0.36 0.028
     size 0.12 0.078
     text "MY VEHICLES"
     font "gui/fonts/metronbook-bold16"
     "text color" 0.89 0.93 0.96 1
     "text halign" center
     "text valign" center
    }}
    ButtonWidgetClass RetrieveBtn {{
     position 0.36 0.028
     size 0.12 0.078
     text ""
     color 0 0 0 0
     style Empty
    }}
    ImageWidgetClass ARImageParkBtn {{
     ignorepointer 1
     position 0.485 0.028
     size 0.12 0.078
     image0 "AshesReborn/gui/textures/ar_tab_idle.paa"
     mode blend
     "src alpha" 1
     "stretch mode" stretch_w_h
    }}
    TextWidgetClass ARLabelParkBtn {{
     ignorepointer 1
     position 0.485 0.028
     size 0.12 0.078
     text "PARK VEHICLE"
     font "gui/fonts/metronbook-bold16"
     "text color" 0.89 0.93 0.96 1
     "text halign" center
     "text valign" center
    }}
    ButtonWidgetClass ParkBtn {{
     position 0.485 0.028
     size 0.12 0.078
     text ""
     color 0 0 0 0
     style Empty
    }}
    ImageWidgetClass ARTabFavBg {{
     ignorepointer 1
     position 0.610 0.028
     size 0.12 0.078
     image0 "AshesReborn/gui/textures/ar_tab_idle.paa"
     mode blend
     "src alpha" 1
     "stretch mode" stretch_w_h
    }}
    TextWidgetClass ARFavoritesLabel {{
     ignorepointer 1
     position 0.610 0.028
     size 0.12 0.078
     text "FAVORITES"
     font "gui/fonts/metronbook-bold16"
     "text color" 0.89 0.93 0.96 1
     "text halign" center
     "text valign" center
    }}
    ButtonWidgetClass ARFavoritesFilter {{
     position 0.610 0.028
     size 0.12 0.078
     text ""
     color 0 0 0 0
     style Empty
    }}
    ImageWidgetClass ARTabRecentBg {{
     ignorepointer 1
     position 0.735 0.028
     size 0.12 0.078
     image0 "AshesReborn/gui/textures/ar_tab_idle.paa"
     mode blend
     "src alpha" 1
     "stretch mode" stretch_w_h
    }}
    TextWidgetClass AROrderLabel {{
     ignorepointer 1
     position 0.735 0.028
     size 0.12 0.078
     text "RECENT"
     font "gui/fonts/metronbook-bold16"
     "text color" 0.89 0.93 0.96 1
     "text halign" center
     "text valign" center
    }}
    ButtonWidgetClass ARReverseOrder {{
     position 0.735 0.028
     size 0.12 0.078
     text ""
     color 0 0 0 0
     style Empty
    }}
    ImageWidgetClass ARDotGreen {{
     ignorepointer 1
     position 0.862 0.048
     size 0.014 0.028
     image0 "AshesReborn/gui/textures/ar_dot_green.paa"
     mode blend
     "src alpha" 1
     "stretch mode" stretch_w_h
    }}
    TextWidgetClass MongoLabel {{
     ignorepointer 1
     position 0.880 0.044
     size 0.070 0.036
     text "ONLINE"
     font "gui/fonts/metronbook14"
     "text color" 0.00 0.90 0.36 1
    }}
    ImageWidgetClass CloseBg {{
     ignorepointer 1
     position 0.952 0.038
     size 0.028 0.054
     image0 "AshesReborn/gui/textures/ar_close.paa"
     mode blend
     "src alpha" 1
     "stretch mode" stretch_w_h
    }}
    TextWidgetClass CloseLabel {{
     ignorepointer 1
     position 0.952 0.038
     size 0.028 0.054
     text "X"
     font "gui/fonts/metronbook-bold18"
     "text color" 0.96 0.97 1 1
     "text halign" center
     "text valign" center
    }}
    ButtonWidgetClass btn_UIClose {{
     position 0.952 0.038
     size 0.028 0.054
     text ""
     color 0 0 0 0
     style Empty
    }}

    ImageWidgetClass VehicleListPanel {{
     ignorepointer 1
     position 0.016 0.130
     size 0.258 0.710
     image0 "AshesReborn/gui/textures/ar_panel.paa"
     mode blend
     "src alpha" 1
     "stretch mode" stretch_w_h
    }}
    FrameWidgetClass VehicleListFrame {{
     clipchildren 0
     position 0.016 0.130
     size 0.258 0.710
     {{
      TextWidgetClass ListHeading {{
       ignorepointer 1
       position 0.05 0.018
       size 0.70 0.045
       text "MY VEHICLES"
       font "gui/fonts/metronbook-bold16"
       "text color" 0.15 0.78 1 1
      }}
      ImageWidgetClass SearchFrame {{
       ignorepointer 1
       position 0.04 0.075
       size 0.92 0.072
       image0 "AshesReborn/gui/textures/ar_search.paa"
       mode blend
       "src alpha" 1
       "stretch mode" stretch_w_h
      }}
      TextWidgetClass ARSearchPlaceholder {{
       ignorepointer 1
       position 0.07 0.088
       size 0.70 0.048
       text "Search vehicle / owner / ID"
       font "gui/fonts/metronbook16"
       "text color" 0.61 0.70 0.78 1
      }}
      EditBoxWidgetClass ARSearch {{
       position 0.06 0.088
       size 0.72 0.048
       text ""
       font "gui/fonts/metronbook16"
       style ServerBrowser
       "text color" 0.96 0.97 1 1
      }}
      ImageWidgetClass SearchClearBg {{
       ignorepointer 1
       position 0.82 0.088
       size 0.12 0.048
       image0 "AshesReborn/gui/textures/ar_close.paa"
       mode blend
       "src alpha" 1
       "stretch mode" stretch_w_h
      }}
      TextWidgetClass SearchClearLabel {{
       ignorepointer 1
       position 0.82 0.088
       size 0.12 0.048
       text "X"
       font "gui/fonts/metronbook16"
       "text color" 0.89 0.93 0.96 1
       "text halign" center
       "text valign" center
      }}
      ButtonWidgetClass ARClearSearch {{
       position 0.82 0.088
       size 0.12 0.048
       text ""
       color 0 0 0 0
       style Empty
      }}
      TextWidgetClass ARNoResults {{
       ignorepointer 1
       visible 0
       position 0.08 0.30
       size 0.84 0.08
       text "NO MATCHING VEHICLES"
       font "gui/fonts/metronbook16"
       "text color" 0.70 0.79 0.86 1
      }}
{chr(10).join(rows)}
      TextWidgetClass ParkList {{
       ignorepointer 1
       position 0.08 0.30
       size 0.84 0.30
       text ""
       font "gui/fonts/metronbook18"
       "text color" 0.85 0.97 1 1
      }}
      TextWidgetClass AVMResultCount {{
       ignorepointer 1
       position 0.05 0.930
       size 0.40 0.045
       text ""
       font "gui/fonts/metronbook14"
       "text color" 0.70 0.79 0.86 1
      }}
      ImageWidgetClass ARImagePrevPage {{
       ignorepointer 1
       position 0.46 0.925
       size 0.12 0.050
       image0 "AshesReborn/gui/textures/ar_button_idle.paa"
       mode blend
       "src alpha" 1
       "stretch mode" stretch_w_h
      }}
      ButtonWidgetClass PrevPage {{
       position 0.46 0.925
       size 0.12 0.050
       text "<"
       font "gui/fonts/metronbook-bold18"
       "text color" 0.89 0.93 0.96 1
       color 0 0 0 0
       style Empty
      }}
      TextWidgetClass PageInfo {{
       ignorepointer 1
       position 0.58 0.925
       size 0.20 0.050
       text ""
       font "gui/fonts/metronbook14"
       "text color" 0.70 0.79 0.86 1
       "text halign" center
       "text valign" center
      }}
      ImageWidgetClass ARImageNextPage {{
       ignorepointer 1
       position 0.79 0.925
       size 0.12 0.050
       image0 "AshesReborn/gui/textures/ar_button_idle.paa"
       mode blend
       "src alpha" 1
       "stretch mode" stretch_w_h
      }}
      ButtonWidgetClass NextPage {{
       position 0.79 0.925
       size 0.12 0.050
       text ">"
       font "gui/fonts/metronbook-bold18"
       "text color" 0.89 0.93 0.96 1
       color 0 0 0 0
       style Empty
      }}
     }}
    }}

    FrameWidgetClass DetailsPanel {{
     clipchildren 0
     position 0.282 0.130
     size 0.702 0.710
     {{
      TextWidgetClass AVMVehicleTitle {{
       ignorepointer 1
       position 0.01 0.000
       size 0.62 0.055
       text ""
       font "gui/fonts/metronbook-bold22"
       "text color" 0.96 0.97 1 1
      }}
      ButtonWidgetClass ARTitleFavorite {{
       position 0.64 0.006
       size 0.05 0.045
       text "*"
       font "gui/fonts/metronbook-bold22"
       "text color" 0.70 0.79 0.86 1
       color 0 0 0 0
       style Empty
      }}
      ImageWidgetClass ARTagOffroadBg {{
       ignorepointer 1
       position 0.01 0.058
       size 0.14 0.038
       image0 "AshesReborn/gui/textures/ar_badge.paa"
       mode blend
       "src alpha" 1
       "stretch mode" stretch_w_h
      }}
      TextWidgetClass ARTagOffroadText {{
       ignorepointer 1
       position 0.01 0.058
       size 0.14 0.038
       text ""
       font "gui/fonts/metronbook14"
       "text color" 0.89 0.93 0.96 1
       "text halign" center
       "text valign" center
      }}
      ImageWidgetClass ARTagPersonalBg {{
       ignorepointer 1
       position 0.16 0.058
       size 0.18 0.038
       image0 "AshesReborn/gui/textures/ar_badge.paa"
       mode blend
       "src alpha" 1
       "stretch mode" stretch_w_h
      }}
      TextWidgetClass ARTagPersonalText {{
       ignorepointer 1
       position 0.16 0.058
       size 0.18 0.038
       text ""
       font "gui/fonts/metronbook14"
       "text color" 0.89 0.93 0.96 1
       "text halign" center
       "text valign" center
      }}

      ImageWidgetClass ARStatPanelBg {{
       ignorepointer 1
       position 0.00 0.110
       size 0.195 0.430
       image0 "AshesReborn/gui/textures/ar_stat_panel.paa"
       mode blend
       "src alpha" 1
       "stretch mode" stretch_w_h
      }}
      FrameWidgetClass ARStatsFrame {{
       position 0.00 0.110
       size 0.195 0.430
       {{
        TextWidgetClass ARStatsHeading {{
         ignorepointer 1
         position 0.08 0.04
         size 0.84 0.08
         text "VEHICLE STATS"
         font "gui/fonts/metronbook-bold16"
         "text color" 0.15 0.78 1 1
        }}
{chr(10).join(stats)}
       }}
      }}

      ImageWidgetClass PreviewFrame {{
       ignorepointer 1
       position 0.205 0.110
       size 0.545 0.430
       image0 "AshesReborn/gui/textures/ar_preview_well.paa"
       mode blend
       "src alpha" 1
       "stretch mode" stretch_w_h
      }}
      ItemPreviewWidgetClass ARVehicle3DPreview {{
       clipchildren 0
       ignorepointer 1
       keepsafezone 0
       position 0.215 0.125
       size 0.525 0.400
      }}
      TextWidgetClass ARPreviewStatus {{
       ignorepointer 1
       position 0.215 0.290
       size 0.525 0.06
       text "VEHICLE PREVIEW"
       font "gui/fonts/metronbook16"
       "text color" 0.40 0.62 0.80 1
       "text halign" center
      }}
      ButtonWidgetClass PreviewDrag {{
       position 0.205 0.110
       size 0.545 0.430
       text ""
       color 0 0 0 0
       style Empty
      }}
      ButtonWidgetClass ARPreviewBack {{
       visible 0
       position 0 0
       size 0.01 0.01
       text ""
       color 0 0 0 0
       style Empty
      }}

      ImageWidgetClass ARInfoPanelBg {{
       ignorepointer 1
       position 0.760 0.110
       size 0.240 0.430
       image0 "AshesReborn/gui/textures/ar_info_panel.paa"
       mode blend
       "src alpha" 1
       "stretch mode" stretch_w_h
      }}
      FrameWidgetClass ARInfoFrame {{
       position 0.760 0.110
       size 0.240 0.430
       {{
        TextWidgetClass ARInfoIdLab {{ ignorepointer 1 position 0.08 0.04 size 0.84 0.05 text "VEHICLE ID" font "gui/fonts/metronbook14" "text color" 0.51 0.61 0.70 1 }}
        TextWidgetClass AVMVehicleId {{ ignorepointer 1 position 0.08 0.09 size 0.84 0.07 text "" font "gui/fonts/metronbook-bold16" "text color" 0.96 0.97 1 1 }}
        TextWidgetClass ARInfoOwnLab {{ ignorepointer 1 position 0.08 0.20 size 0.84 0.05 text "OWNER" font "gui/fonts/metronbook14" "text color" 0.51 0.61 0.70 1 }}
        TextWidgetClass AVMVehicleOwner {{ ignorepointer 1 position 0.08 0.25 size 0.84 0.07 text "" font "gui/fonts/metronbook-bold16" "text color" 0.96 0.97 1 1 }}
        TextWidgetClass ARInfoStoreLab {{ ignorepointer 1 position 0.08 0.36 size 0.84 0.05 text "STORED ON" font "gui/fonts/metronbook14" "text color" 0.51 0.61 0.70 1 }}
        TextWidgetClass AVMVehicleStored {{ ignorepointer 1 position 0.08 0.41 size 0.84 0.07 text "" font "gui/fonts/metronbook16" "text color" 0.96 0.97 1 1 }}
        TextWidgetClass ARInfoPosLab {{ ignorepointer 1 position 0.08 0.52 size 0.84 0.05 text "LAST LOCATION" font "gui/fonts/metronbook14" "text color" 0.51 0.61 0.70 1 }}
        TextWidgetClass ARDetailPosition {{ ignorepointer 1 position 0.08 0.57 size 0.84 0.10 text "" font "gui/fonts/metronbook14" "text color" 0.96 0.97 1 1 }}
        TextWidgetClass ARInfoExpLab {{ ignorepointer 1 position 0.08 0.72 size 0.84 0.05 text "TIME REMAINING" font "gui/fonts/metronbook14" "text color" 0.51 0.61 0.70 1 }}
        TextWidgetClass AVMVehicleExpiry {{ ignorepointer 1 position 0.08 0.78 size 0.84 0.10 text "" font "gui/fonts/metronbook-bold16" "text color" 0.00 0.90 0.36 1 }}
       }}
      }}

      PanelWidgetClass ARInventoryPanel {{
       color 0 0 0 0
       position 0.00 0.555
       size 1.00 0.445
       {{
        ImageWidgetClass ARInvTabAttachBg {{
         ignorepointer 1
         position 0.00 0.00
         size 0.18 0.10
         image0 "AshesReborn/gui/textures/ar_tab_active.paa"
         mode blend
         "src alpha" 1
         "stretch mode" stretch_w_h
        }}
        TextWidgetClass ARInvTabAttachLabel {{
         ignorepointer 1
         position 0.00 0.00
         size 0.18 0.10
         text "ATTACHMENTS"
         font "gui/fonts/metronbook-bold16"
         "text color" 0.15 0.78 1 1
         "text halign" center
         "text valign" center
        }}
        ButtonWidgetClass ARInvTabAttach {{
         position 0.00 0.00
         size 0.18 0.10
         text ""
         color 0 0 0 0
         style Empty
        }}
        ImageWidgetClass ARInvTabCargoBg {{
         ignorepointer 1
         position 0.19 0.00
         size 0.14 0.10
         image0 "AshesReborn/gui/textures/ar_tab_idle.paa"
         mode blend
         "src alpha" 1
         "stretch mode" stretch_w_h
        }}
        TextWidgetClass ARInvTabCargoLabel {{
         ignorepointer 1
         position 0.19 0.00
         size 0.14 0.10
         text "CARGO"
         font "gui/fonts/metronbook-bold16"
         "text color" 0.70 0.79 0.86 1
         "text halign" center
         "text valign" center
        }}
        ButtonWidgetClass ARInvTabCargo {{
         position 0.19 0.00
         size 0.14 0.10
         text ""
         color 0 0 0 0
         style Empty
        }}
        TextWidgetClass ARPartsCount {{ ignorepointer 1 position 0.34 0.00 size 0.20 0.10 text "" font "gui/fonts/metronbook14" "text color" 0.70 0.79 0.86 1 "text valign" center }}
        TextWidgetClass ARCargoCount {{ ignorepointer 1 position 0.52 0.00 size 0.20 0.10 text "" font "gui/fonts/metronbook14" "text color" 0.70 0.79 0.86 1 "text valign" center }}
        ImageWidgetClass ARImagePartsPrev {{ ignorepointer 1 position 0.74 0.015 size 0.08 0.07 image0 "AshesReborn/gui/textures/ar_button_idle.paa" mode blend "src alpha" 1 "stretch mode" stretch_w_h }}
        ButtonWidgetClass ARPartsPrevious {{ position 0.74 0.015 size 0.08 0.07 text "<" font "gui/fonts/metronbook-bold16" "text color" 0.89 0.93 0.96 1 color 0 0 0 0 style Empty }}
        TextWidgetClass ARPartsPage {{ ignorepointer 1 position 0.82 0.015 size 0.08 0.07 text "" font "gui/fonts/metronbook14" "text color" 0.70 0.79 0.86 1 "text halign" center "text valign" center }}
        ImageWidgetClass ARImagePartsNext {{ ignorepointer 1 position 0.90 0.015 size 0.08 0.07 image0 "AshesReborn/gui/textures/ar_button_idle.paa" mode blend "src alpha" 1 "stretch mode" stretch_w_h }}
        ButtonWidgetClass ARPartsNext {{ position 0.90 0.015 size 0.08 0.07 text ">" font "gui/fonts/metronbook-bold16" "text color" 0.89 0.93 0.96 1 color 0 0 0 0 style Empty }}

        PanelWidgetClass ARPartsPanel {{
         color 0 0 0 0
         position 0.00 0.11
         size 1.00 0.42
         {{
{tiles("ARParts", "0.08")}
         }}
        }}
        PanelWidgetClass ARCargoPanel {{
         color 0 0 0 0
         position 0.00 0.54
         size 1.00 0.46
         {{
          TextWidgetClass ARCargoHeading {{
           ignorepointer 1
           position 0.00 0.00
           size 0.30 0.16
           text "CARGO"
           font "gui/fonts/metronbook-bold16"
           "text color" 0.15 0.78 1 1
          }}
          ImageWidgetClass ARImageCargoPrev {{ ignorepointer 1 position 0.74 0.00 size 0.08 0.16 image0 "AshesReborn/gui/textures/ar_button_idle.paa" mode blend "src alpha" 1 "stretch mode" stretch_w_h }}
          ButtonWidgetClass ARCargoPrevious {{ position 0.74 0.00 size 0.08 0.16 text "<" font "gui/fonts/metronbook-bold16" "text color" 0.89 0.93 0.96 1 color 0 0 0 0 style Empty }}
          TextWidgetClass ARCargoPage {{ ignorepointer 1 position 0.82 0.00 size 0.08 0.16 text "" font "gui/fonts/metronbook14" "text color" 0.70 0.79 0.86 1 "text halign" center "text valign" center }}
          ImageWidgetClass ARImageCargoNext {{ ignorepointer 1 position 0.90 0.00 size 0.08 0.16 image0 "AshesReborn/gui/textures/ar_button_idle.paa" mode blend "src alpha" 1 "stretch mode" stretch_w_h }}
          ButtonWidgetClass ARCargoNext {{ position 0.90 0.00 size 0.08 0.16 text ">" font "gui/fonts/metronbook-bold16" "text color" 0.89 0.93 0.96 1 color 0 0 0 0 style Empty }}
{tiles("ARCargo", "0.20")}
         }}
        }}
       }}
      }}
     }}
    }}

    ImageWidgetClass ARImageConfirmBtn {{
     ignorepointer 1
     position 0.016 0.855
     size 0.300 0.115
     image0 "AshesReborn/gui/textures/button_blue.paa"
     mode blend
     "src alpha" 1
     "stretch mode" stretch_w_h
    }}
    TextWidgetClass ARLabelConfirmBtn {{
     ignorepointer 1
     position 0.016 0.855
     size 0.300 0.070
     text "RETRIEVE VEHICLE"
     font "gui/fonts/metronbook-bold18"
     "text color" 0.98 0.99 1 1
     "text halign" center
     "text valign" center
    }}
    TextWidgetClass ARConfirmSub {{
     ignorepointer 1
     position 0.016 0.915
     size 0.300 0.040
     text "Spawn at this location"
     font "gui/fonts/metronbook14"
     "text color" 0.85 0.93 1 1
     "text halign" center
    }}
    ButtonWidgetClass ConfirmBtn {{
     position 0.016 0.855
     size 0.300 0.115
     text ""
     color 0 0 0 0
     style Empty
    }}
    ImageWidgetClass ARImageSpawnBtn {{
     ignorepointer 1
     position 0.350 0.855
     size 0.300 0.115
     image0 "AshesReborn/gui/textures/button_dark.paa"
     mode blend
     "src alpha" 1
     "stretch mode" stretch_w_h
    }}
    TextWidgetClass ARLabelSpawnBtn {{
     ignorepointer 1
     position 0.350 0.855
     size 0.300 0.070
     text "SET SPAWN LOCATION"
     font "gui/fonts/metronbook-bold18"
     "text color" 0.96 0.97 1 1
     "text halign" center
     "text valign" center
    }}
    TextWidgetClass ARSpawnSub {{
     ignorepointer 1
     position 0.350 0.915
     size 0.300 0.040
     text "Set where the vehicle will spawn"
     font "gui/fonts/metronbook14"
     "text color" 0.70 0.79 0.86 1
     "text halign" center
    }}
    ButtonWidgetClass ARSpawnBtn {{
     position 0.350 0.855
     size 0.300 0.115
     text ""
     color 0 0 0 0
     style Empty
    }}
    ImageWidgetClass ARImageDeleteBtn {{
     ignorepointer 1
     position 0.684 0.855
     size 0.300 0.115
     image0 "AshesReborn/gui/textures/ar_btn_red.paa"
     mode blend
     "src alpha" 1
     "stretch mode" stretch_w_h
    }}
    TextWidgetClass ARLabelDeleteBtn {{
     ignorepointer 1
     position 0.684 0.855
     size 0.300 0.070
     text "DELETE VEHICLE"
     font "gui/fonts/metronbook-bold18"
     "text color" 0.96 0.97 1 1
     "text halign" center
     "text valign" center
    }}
    TextWidgetClass ARDeleteSub {{
     ignorepointer 1
     position 0.684 0.915
     size 0.300 0.040
     text "Permanently remove from garage"
     font "gui/fonts/metronbook14"
     "text color" 0.90 0.75 0.75 1
     "text halign" center
    }}
    ButtonWidgetClass ARDeleteBtn {{
     position 0.684 0.855
     size 0.300 0.115
     text ""
     color 0 0 0 0
     style Empty
    }}

    ImageWidgetClass ARItemTooltip {{
     visible 0
     ignorepointer 1
     position 0.55 0.55
     size 0.22 0.12
     image0 "AshesReborn/gui/textures/ar_tooltip.paa"
     mode blend
     "src alpha" 1
     "stretch mode" stretch_w_h
    }}
    TextWidgetClass ARInfoMessage {{
     visible 0
     ignorepointer 1
     position 0.55 0.56
     size 0.20 0.10
     text ""
     font "gui/fonts/metronbook14"
     "text color" 0.96 0.97 1 1
    }}

    PanelWidgetClass FooterPanel {{ visible 0 position 0 0 size 0.01 0.01 }}
    PanelWidgetClass ErrorPanel {{
     color 0 0 0 0
     position 0.282 0.100
     size 0.50 0.028
     {{
      TextWidgetClass ErrorText {{
       position 0 0
       size 1 1
       text ""
       font "gui/fonts/metronbook14"
       "text color" 1 0.40 0.40 1
      }}
     }}
    }}
    PanelWidgetClass MaxStoredCarsSpacer {{
     visible 0
     position 0 0
     size 0.01 0.01
     {{
      TextWidgetClass CurrentCountValue {{ position 0 0 size 1 1 text "" }}
      TextWidgetClass MaxAllowedValue {{ position 0 0 size 1 1 text "" }}
     }}
    }}
    PanelWidgetClass HiddenDump {{
     visible 0
     position 0 0
     size 0.01 0.01
     {{
      TextWidgetClass LogoText {{ position 0 0 size 0.01 0.01 text "AR" }}
      ButtonWidgetClass ARResetSelection {{ position 0 0 size 0.01 0.01 text "" color 0 0 0 0 style Empty }}
      ButtonWidgetClass ARInfoToggle {{ position 0 0 size 0.01 0.01 text "" color 0 0 0 0 style Empty }}
      ButtonWidgetClass CloseX {{ position 0 0 size 0.01 0.01 text "" color 0 0 0 0 style Empty }}
      TextWidgetClass OverviewHeading {{ position 0 0 size 0.01 0.01 text "" }}
     }}
    }}
   }}
  }}
 }}
}}
'''

Path(r"C:\Users\Michael\Desktop\@AR\src\AR\gui\list.layout").write_text(layout, encoding="utf-8")
print("wrote list.layout", len(layout))
