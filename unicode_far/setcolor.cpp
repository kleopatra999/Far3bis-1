/*
setcolor.cpp

��������� ��������� ������
*/
/*
Copyright � 1996 Eugene Roshal
Copyright � 2000 Far Group
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:
1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
3. The name of the authors may not be used to endorse or promote products
   derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "headers.hpp"
#pragma hdrstop

#include "setcolor.hpp"
#include "keys.hpp"
#include "lang.hpp"
#include "colors.hpp"
#include "vmenu.hpp"
#include "dialog.hpp"
#include "filepanels.hpp"
#include "ctrlobj.hpp"
#include "savescr.hpp"
#include "scrbuf.hpp"
#include "panel.hpp"
#include "chgmmode.hpp"
#include "interf.hpp"
#include "palette.hpp"
#include "config.hpp"

static void SetItemColors(MenuDataEx *Items,int *PaletteItems,int Size,int TypeSub, VMenu* MenuToRedraw1);

void GetColor(int PaletteIndex, VMenu* MenuToRedraw1, VMenu* MenuToRedraw2, VMenu* MenuToRedraw3)
{
	ChangeMacroMode chgMacroMode(MACRO_MENU);
	WORD NewColor=Palette[PaletteIndex-COL_FIRSTPALETTECOLOR];

	if (GetColorDialog(NewColor))
	{
		Palette[PaletteIndex-COL_FIRSTPALETTECOLOR]=static_cast<BYTE>(NewColor);
		ScrBuf.Lock(); // �������� ������ ����������
		CtrlObject->Cp()->LeftPanel->Update(UPDATE_KEEP_SELECTION);
		CtrlObject->Cp()->LeftPanel->Redraw();
		CtrlObject->Cp()->RightPanel->Update(UPDATE_KEEP_SELECTION);
		CtrlObject->Cp()->RightPanel->Redraw();

		if (MenuToRedraw3)
			MenuToRedraw3->Hide();

		MenuToRedraw2->Hide(); // �����
		MenuToRedraw1->Hide();
		FrameManager->RefreshFrame(); // ��������
		FrameManager->PluginCommit(); // ��������.
		MenuToRedraw1->SetColors();
		MenuToRedraw1->Show(); // �����
		MenuToRedraw2->SetColors();
		MenuToRedraw2->Show();

		if (MenuToRedraw3)
		{
			MenuToRedraw3->SetColors();
			MenuToRedraw3->Show();
		}

		if (Opt.Clock)
			ShowTime(1);

		ScrBuf.Unlock(); // ��������� ����������
		FrameManager->PluginCommit(); // ��������.
	}
}


void SetColors()
{
	MenuDataEx Groups[]=
	{
		MSG(MSetColorPanel),LIF_SELECTED,0,
		MSG(MSetColorDialog),0,0,
		MSG(MSetColorWarning),0,0,
		MSG(MSetColorMenu),0,0,
		MSG(MSetColorHMenu),0,0,
		MSG(MSetColorKeyBar),0,0,
		MSG(MSetColorCommandLine),0,0,
		MSG(MSetColorClock),0,0,
		MSG(MSetColorViewer),0,0,
		MSG(MSetColorEditor),0,0,
		MSG(MSetColorHelp),0,0,
		L"",LIF_SEPARATOR,0,
		MSG(MSetDefaultColors),0,0,
		MSG(MSetBW),0,0,
	};
	MenuDataEx PanelItems[]=
	{
		MSG(MSetColorPanelNormal),LIF_SELECTED,0,
		MSG(MSetColorPanelSelected),0,0,
		MSG(MSetColorPanelHighlightedInfo),0,0,
		MSG(MSetColorPanelDragging),0,0,
		MSG(MSetColorPanelBox),0,0,
		MSG(MSetColorPanelNormalCursor),0,0,
		MSG(MSetColorPanelSelectedCursor),0,0,
		MSG(MSetColorPanelNormalTitle),0,0,
		MSG(MSetColorPanelSelectedTitle),0,0,
		MSG(MSetColorPanelColumnTitle),0,0,
		MSG(MSetColorPanelTotalInfo),0,0,
		MSG(MSetColorPanelSelectedInfo),0,0,
		MSG(MSetColorPanelScrollbar),0,0,
		MSG(MSetColorPanelScreensNumber),0,0,
	};
	int PanelPaletteItems[]=
	{
		COL_PANELTEXT,COL_PANELSELECTEDTEXT,COL_PANELINFOTEXT,
		COL_PANELDRAGTEXT,COL_PANELBOX,COL_PANELCURSOR,COL_PANELSELECTEDCURSOR,
		COL_PANELTITLE,COL_PANELSELECTEDTITLE,COL_PANELCOLUMNTITLE,
		COL_PANELTOTALINFO,COL_PANELSELECTEDINFO,COL_PANELSCROLLBAR,
		COL_PANELSCREENSNUMBER
	};
	MenuDataEx DialogItems[]=
	{
		MSG(MSetColorDialogNormal),LIF_SELECTED,0,
		MSG(MSetColorDialogHighlighted),0,0,
		MSG(MSetColorDialogDisabled),0,0,
		MSG(MSetColorDialogBox),0,0,
		MSG(MSetColorDialogBoxTitle),0,0,
		MSG(MSetColorDialogHighlightedBoxTitle),0,0,
		MSG(MSetColorDialogTextInput),0,0,
		MSG(MSetColorDialogUnchangedTextInput),0,0,
		MSG(MSetColorDialogSelectedTextInput),0,0,
		MSG(MSetColorDialogEditDisabled),0,0,
		MSG(MSetColorDialogButtons),0,0,
		MSG(MSetColorDialogSelectedButtons),0,0,
		MSG(MSetColorDialogHighlightedButtons),0,0,
		MSG(MSetColorDialogSelectedHighlightedButtons),0,0,
		MSG(MSetColorDialogDefaultButton),0,0,
		MSG(MSetColorDialogSelectedDefaultButton),0,0,
		MSG(MSetColorDialogHighlightedDefaultButton),0,0,
		MSG(MSetColorDialogSelectedHighlightedDefaultButton),0,0,
		MSG(MSetColorDialogListBoxControl),0,0,
		MSG(MSetColorDialogComboBoxControl),0,0,
	};
	int DialogPaletteItems[]=
	{
		COL_DIALOGTEXT,
		COL_DIALOGHIGHLIGHTTEXT,
		COL_DIALOGDISABLED,
		COL_DIALOGBOX,
		COL_DIALOGBOXTITLE,
		COL_DIALOGHIGHLIGHTBOXTITLE,
		COL_DIALOGEDIT,
		COL_DIALOGEDITUNCHANGED,
		COL_DIALOGEDITSELECTED,
		COL_DIALOGEDITDISABLED,
		COL_DIALOGBUTTON,
		COL_DIALOGSELECTEDBUTTON,
		COL_DIALOGHIGHLIGHTBUTTON,
		COL_DIALOGHIGHLIGHTSELECTEDBUTTON,
		COL_DIALOGDEFAULTBUTTON,
		COL_DIALOGSELECTEDDEFAULTBUTTON,
		COL_DIALOGHIGHLIGHTDEFAULTBUTTON,
		COL_DIALOGHIGHLIGHTSELECTEDDEFAULTBUTTON,
		0,
		2,
	};
	MenuDataEx WarnDialogItems[]=
	{
		MSG(MSetColorDialogNormal),LIF_SELECTED,0,
		MSG(MSetColorDialogHighlighted),0,0,
		MSG(MSetColorDialogDisabled),0,0,
		MSG(MSetColorDialogBox),0,0,
		MSG(MSetColorDialogBoxTitle),0,0,
		MSG(MSetColorDialogHighlightedBoxTitle),0,0,
		MSG(MSetColorDialogTextInput),0,0,
		MSG(MSetColorDialogUnchangedTextInput),0,0,
		MSG(MSetColorDialogSelectedTextInput),0,0,
		MSG(MSetColorDialogEditDisabled),0,0,
		MSG(MSetColorDialogButtons),0,0,
		MSG(MSetColorDialogSelectedButtons),0,0,
		MSG(MSetColorDialogHighlightedButtons),0,0,
		MSG(MSetColorDialogSelectedHighlightedButtons),0,0,
		MSG(MSetColorDialogDefaultButton),0,0,
		MSG(MSetColorDialogSelectedDefaultButton),0,0,
		MSG(MSetColorDialogHighlightedDefaultButton),0,0,
		MSG(MSetColorDialogSelectedHighlightedDefaultButton),0,0,
		MSG(MSetColorDialogListBoxControl),0,0,
		MSG(MSetColorDialogComboBoxControl),0,0,
	};
	int WarnDialogPaletteItems[]=
	{
		COL_WARNDIALOGTEXT,
		COL_WARNDIALOGHIGHLIGHTTEXT,
		COL_WARNDIALOGDISABLED,
		COL_WARNDIALOGBOX,
		COL_WARNDIALOGBOXTITLE,
		COL_WARNDIALOGHIGHLIGHTBOXTITLE,
		COL_WARNDIALOGEDIT,
		COL_WARNDIALOGEDITUNCHANGED,
		COL_WARNDIALOGEDITSELECTED,
		COL_WARNDIALOGEDITDISABLED,
		COL_WARNDIALOGBUTTON,
		COL_WARNDIALOGSELECTEDBUTTON,
		COL_WARNDIALOGHIGHLIGHTBUTTON,
		COL_WARNDIALOGHIGHLIGHTSELECTEDBUTTON,
		COL_WARNDIALOGDEFAULTBUTTON,
		COL_WARNDIALOGSELECTEDDEFAULTBUTTON,
		COL_WARNDIALOGHIGHLIGHTDEFAULTBUTTON,
		COL_WARNDIALOGHIGHLIGHTSELECTEDDEFAULTBUTTON,
		1,
		3,
	};
	MenuDataEx MenuItems[]=
	{
		MSG(MSetColorMenuNormal),LIF_SELECTED,0,
		MSG(MSetColorMenuSelected),0,0,
		MSG(MSetColorMenuHighlighted),0,0,
		MSG(MSetColorMenuSelectedHighlighted),0,0,
		MSG(MSetColorMenuDisabled),0,0,
		MSG(MSetColorMenuBox),0,0,
		MSG(MSetColorMenuTitle),0,0,
		MSG(MSetColorMenuScrollBar),0,0,
		MSG(MSetColorMenuArrows),0,0,
		MSG(MSetColorMenuArrowsSelected),0,0,
		MSG(MSetColorMenuArrowsDisabled),0,0,
		MSG(MSetColorMenuGrayed),0,0,
		MSG(MSetColorMenuSelectedGrayed),0,0,
	};
	int MenuPaletteItems[]=
	{
		COL_MENUTEXT,COL_MENUSELECTEDTEXT,COL_MENUHIGHLIGHT,
		COL_MENUSELECTEDHIGHLIGHT,COL_MENUDISABLEDTEXT,
		COL_MENUBOX,COL_MENUTITLE,COL_MENUSCROLLBAR,
		COL_MENUARROWS,                             // Arrow
		COL_MENUARROWSSELECTED,                     // ��������� - Arrow
		COL_MENUARROWSDISABLED,
		COL_MENUGRAYTEXT,                          // "�����"
		COL_MENUSELECTEDGRAYTEXT,                  // ��������� "�����"
	};
	MenuDataEx HMenuItems[]=
	{
		MSG(MSetColorHMenuNormal),LIF_SELECTED,0,
		MSG(MSetColorHMenuSelected),0,0,
		MSG(MSetColorHMenuHighlighted),0,0,
		MSG(MSetColorHMenuSelectedHighlighted),0,0,
	};
	int HMenuPaletteItems[]=
	{
		COL_HMENUTEXT,COL_HMENUSELECTEDTEXT,COL_HMENUHIGHLIGHT,
		COL_HMENUSELECTEDHIGHLIGHT
	};
	MenuDataEx KeyBarItems[]=
	{
		MSG(MSetColorKeyBarNumbers),LIF_SELECTED,0,
		MSG(MSetColorKeyBarNames),0,0,
		MSG(MSetColorKeyBarBackground),0,0,
	};
	int KeyBarPaletteItems[]=
	{
		COL_KEYBARNUM,COL_KEYBARTEXT,COL_KEYBARBACKGROUND
	};
	MenuDataEx CommandLineItems[]=
	{
		MSG(MSetColorCommandLineNormal),LIF_SELECTED,0,
		MSG(MSetColorCommandLineSelected),0,0,
		MSG(MSetColorCommandLinePrefix),0,0,
		MSG(MSetColorCommandLineUserScreen),0,0,
	};
	int CommandLinePaletteItems[]=
	{
		COL_COMMANDLINE,COL_COMMANDLINESELECTED,COL_COMMANDLINEPREFIX,COL_COMMANDLINEUSERSCREEN
	};
	MenuDataEx ClockItems[]=
	{
		MSG(MSetColorClockNormal),LIF_SELECTED,0,
		MSG(MSetColorClockNormalEditor),0,0,
		MSG(MSetColorClockNormalViewer),0,0,
	};
	int ClockPaletteItems[]=
	{
		COL_CLOCK,
		COL_EDITORCLOCK,COL_VIEWERCLOCK,
	};
	MenuDataEx ViewerItems[]=
	{
		MSG(MSetColorViewerNormal),LIF_SELECTED,0,
		MSG(MSetColorViewerSelected),0,0,
		MSG(MSetColorViewerStatus),0,0,
		MSG(MSetColorViewerArrows),0,0,
		MSG(MSetColorViewerScrollbar),0,0
	};
	int ViewerPaletteItems[]=
	{
		COL_VIEWERTEXT,COL_VIEWERSELECTEDTEXT,COL_VIEWERSTATUS,COL_VIEWERARROWS,COL_VIEWERSCROLLBAR
	};
	MenuDataEx EditorItems[]=
	{
		MSG(MSetColorEditorNormal),LIF_SELECTED,0,
		MSG(MSetColorEditorSelected),0,0,
		MSG(MSetColorEditorStatus),0,0,
		MSG(MSetColorEditorScrollbar),0,0,
	};
	int EditorPaletteItems[]=
	{
		COL_EDITORTEXT,COL_EDITORSELECTEDTEXT,COL_EDITORSTATUS,COL_EDITORSCROLLBAR
	};
	MenuDataEx HelpItems[]=
	{
		MSG(MSetColorHelpNormal),LIF_SELECTED,0,
		MSG(MSetColorHelpHighlighted),0,0,
		MSG(MSetColorHelpReference),0,0,
		MSG(MSetColorHelpSelectedReference),0,0,
		MSG(MSetColorHelpBox),0,0,
		MSG(MSetColorHelpBoxTitle),0,0,
		MSG(MSetColorHelpScrollbar),0,0,
	};
	int HelpPaletteItems[]=
	{
		COL_HELPTEXT,COL_HELPHIGHLIGHTTEXT,COL_HELPTOPIC,COL_HELPSELECTEDTOPIC,
		COL_HELPBOX,COL_HELPBOXTITLE,COL_HELPSCROLLBAR
	};
	{
		int GroupsCode;
		VMenu GroupsMenu(MSG(MSetColorGroupsTitle),Groups,ARRAYSIZE(Groups),0);
		VMenu *MenuToRedraw1=&GroupsMenu;
		for (;;)
		{
			GroupsMenu.SetPosition(2,1,0,0);
			GroupsMenu.SetFlags(VMENU_WRAPMODE|VMENU_NOTCHANGE);
			GroupsMenu.ClearDone();
			GroupsMenu.Process();

			if ((GroupsCode=GroupsMenu.Modal::GetExitCode())<0)
				break;

			if (GroupsCode==12)
			{
				//                   ���� sizeof(Palette)
				memcpy(Palette,DefaultPalette,SizeArrayPalette);
				break;
			}

			if (GroupsCode==13)
			{
				memcpy(Palette,BlackPalette,SizeArrayPalette);
				break;
			}

			switch (GroupsCode)
			{
				case 0:
					SetItemColors(PanelItems,PanelPaletteItems,ARRAYSIZE(PanelItems),0,MenuToRedraw1);
					break;
				case 1:
					SetItemColors(DialogItems,DialogPaletteItems,ARRAYSIZE(DialogItems),1,MenuToRedraw1);
					break;
				case 2:
					SetItemColors(WarnDialogItems,WarnDialogPaletteItems,ARRAYSIZE(WarnDialogItems),1,MenuToRedraw1);
					break;
				case 3:
					SetItemColors(MenuItems,MenuPaletteItems,ARRAYSIZE(MenuItems),0,MenuToRedraw1);
					break;
				case 4:
					SetItemColors(HMenuItems,HMenuPaletteItems,ARRAYSIZE(HMenuItems),0,MenuToRedraw1);
					break;
				case 5:
					SetItemColors(KeyBarItems,KeyBarPaletteItems,ARRAYSIZE(KeyBarItems),0,MenuToRedraw1);
					break;
				case 6:
					SetItemColors(CommandLineItems,CommandLinePaletteItems,ARRAYSIZE(CommandLineItems),0,MenuToRedraw1);
					break;
				case 7:
					SetItemColors(ClockItems,ClockPaletteItems,ARRAYSIZE(ClockItems),0,MenuToRedraw1);
					break;
				case 8:
					SetItemColors(ViewerItems,ViewerPaletteItems,ARRAYSIZE(ViewerItems),0,MenuToRedraw1);
					break;
				case 9:
					SetItemColors(EditorItems,EditorPaletteItems,ARRAYSIZE(EditorItems),0,MenuToRedraw1);
					break;
				case 10:
					SetItemColors(HelpItems,HelpPaletteItems,ARRAYSIZE(HelpItems),0,MenuToRedraw1);
					break;
			}
		}
	}
	CtrlObject->Cp()->SetScreenPosition();
	CtrlObject->Cp()->LeftPanel->Update(UPDATE_KEEP_SELECTION);
	CtrlObject->Cp()->LeftPanel->Redraw();
	CtrlObject->Cp()->RightPanel->Update(UPDATE_KEEP_SELECTION);
	CtrlObject->Cp()->RightPanel->Redraw();
}


static void SetItemColors(MenuDataEx *Items,int *PaletteItems,int Size,int TypeSub, VMenu* MenuToRedraw1)
{
	int ItemsCode;
	VMenu ItemsMenu(MSG(MSetColorItemsTitle),Items,Size,0);
	VMenu* MenuToRedraw2 = nullptr;
	VMenu* MenuToRedraw3 = nullptr;
	if (TypeSub == 2)
		MenuToRedraw3=&ItemsMenu;
	else
		MenuToRedraw2=&ItemsMenu;

	for (;;)
	{
		ItemsMenu.SetPosition(17-(TypeSub == 2?7:0),5+(TypeSub == 2?2:0),0,0);
		ItemsMenu.SetFlags(VMENU_WRAPMODE|VMENU_NOTCHANGE);
		ItemsMenu.ClearDone();
		ItemsMenu.Process();

		if ((ItemsCode=ItemsMenu.Modal::GetExitCode())<0)
			break;

// 0,1 - dialog,warn List
// 2,3 - dialog,warn Combobox
		if (TypeSub == 1 && PaletteItems[ItemsCode] < 4)
		{
			MenuDataEx ListItems[]=
			{
				MSG(MSetColorDialogListText),LIF_SELECTED,0,
				MSG(MSetColorDialogListHighLight),0,0,
				MSG(MSetColorDialogListSelectedText),0,0,
				MSG(MSetColorDialogListSelectedHighLight),0,0,
				MSG(MSetColorDialogListDisabled),0,0,
				MSG(MSetColorDialogListBox),0,0,
				MSG(MSetColorDialogListTitle),0,0,
				MSG(MSetColorDialogListScrollBar),0,0,
				MSG(MSetColorDialogListArrows),0,0,
				MSG(MSetColorDialogListArrowsSelected),0,0,
				MSG(MSetColorDialogListArrowsDisabled),0,0,
				MSG(MSetColorDialogListGrayed),0,0,
				MSG(MSetColorDialogSelectedListGrayed),0,0,
			};

			// 0,1 - dialog,warn List
			// 2,3 - dialog,warn Combobox
			int ListPaletteItems[4][13]=
			{
				// Listbox
				{
					// normal
					COL_DIALOGLISTTEXT,
					COL_DIALOGLISTHIGHLIGHT,
					COL_DIALOGLISTSELECTEDTEXT,
					COL_DIALOGLISTSELECTEDHIGHLIGHT,
					COL_DIALOGLISTDISABLED,
					COL_DIALOGLISTBOX,
					COL_DIALOGLISTTITLE,
					COL_DIALOGLISTSCROLLBAR,
					COL_DIALOGLISTARROWS,             // Arrow
					COL_DIALOGLISTARROWSSELECTED,     // ��������� - Arrow
					COL_DIALOGLISTARROWSDISABLED,     // Arrow disabled
					COL_DIALOGLISTGRAY,               // "�����"
					COL_DIALOGLISTSELECTEDGRAYTEXT,   // ��������� "�����"
				},
				{
					// warn
					COL_WARNDIALOGLISTTEXT,
					COL_WARNDIALOGLISTHIGHLIGHT,
					COL_WARNDIALOGLISTSELECTEDTEXT,
					COL_WARNDIALOGLISTSELECTEDHIGHLIGHT,
					COL_WARNDIALOGLISTDISABLED,
					COL_WARNDIALOGLISTBOX,
					COL_WARNDIALOGLISTTITLE,
					COL_WARNDIALOGLISTSCROLLBAR,
					COL_WARNDIALOGLISTARROWS,            // Arrow
					COL_WARNDIALOGLISTARROWSSELECTED,    // ��������� - Arrow
					COL_WARNDIALOGLISTARROWSDISABLED,    // Arrow disabled
					COL_WARNDIALOGLISTGRAY,              // "�����"
					COL_WARNDIALOGLISTSELECTEDGRAYTEXT,  // ��������� "�����"
				},
				// Combobox
				{
					// normal
					COL_DIALOGCOMBOTEXT,
					COL_DIALOGCOMBOHIGHLIGHT,
					COL_DIALOGCOMBOSELECTEDTEXT,
					COL_DIALOGCOMBOSELECTEDHIGHLIGHT,
					COL_DIALOGCOMBODISABLED,
					COL_DIALOGCOMBOBOX,
					COL_DIALOGCOMBOTITLE,
					COL_DIALOGCOMBOSCROLLBAR,
					COL_DIALOGCOMBOARROWS,               // Arrow
					COL_DIALOGCOMBOARROWSSELECTED,       // ��������� - Arrow
					COL_DIALOGCOMBOARROWSDISABLED,       // Arrow disabled
					COL_DIALOGCOMBOGRAY,                 // "�����"
					COL_DIALOGCOMBOSELECTEDGRAYTEXT,     // ��������� "�����"
				},
				{
					// warn
					COL_WARNDIALOGCOMBOTEXT,
					COL_WARNDIALOGCOMBOHIGHLIGHT,
					COL_WARNDIALOGCOMBOSELECTEDTEXT,
					COL_WARNDIALOGCOMBOSELECTEDHIGHLIGHT,
					COL_WARNDIALOGCOMBODISABLED,
					COL_WARNDIALOGCOMBOBOX,
					COL_WARNDIALOGCOMBOTITLE,
					COL_WARNDIALOGCOMBOSCROLLBAR,
					COL_WARNDIALOGCOMBOARROWS,            // Arrow
					COL_WARNDIALOGCOMBOARROWSSELECTED,    // ��������� - Arrow
					COL_WARNDIALOGCOMBOARROWSDISABLED,    // Arrow disabled
					COL_WARNDIALOGCOMBOGRAY,              // "�����"
					COL_WARNDIALOGCOMBOSELECTEDGRAYTEXT,  // ��������� "�����"
				},
			};

			SetItemColors(ListItems,ListPaletteItems[PaletteItems[ItemsCode]],ARRAYSIZE(ListItems),2,MenuToRedraw1);
			MenuToRedraw3=nullptr;
		}
		else
			GetColor(PaletteItems[ItemsCode], MenuToRedraw1, MenuToRedraw2, MenuToRedraw3);
	}
}

static INT_PTR WINAPI GetColorDlgProc(HANDLE hDlg, int Msg, int Param1, INT_PTR Param2)
{
	switch (Msg)
	{
		case DN_CTLCOLORDLGITEM:

			if (Param1 >= 37 && Param1 <= 39)
			{
				int *CurColor=(int *)SendDlgMessage(hDlg,DM_GETDLGDATA,0,0);
				return (Param2&0xFFFFFF00U)|((*CurColor)&0xFF);
			}

			break;
		case DN_BTNCLICK:

			if (Param1 >= 2 && Param1 <= 34)
			{
				int NewColor;
				int *CurColor = (int *) SendDlgMessage(hDlg, DM_GETDLGDATA, 0, 0);
				FarDialogItem *DlgItem = (FarDialogItem *)xf_malloc(SendDlgMessage(hDlg, DM_GETDLGITEM, Param1, 0));
				SendDlgMessage(hDlg, DM_GETDLGITEM, Param1, (INT_PTR)DlgItem);
				NewColor=*CurColor;

				if (Param1 >= 2 && Param1 <= 17) // Fore
				{
					NewColor&=~0x0F;
					NewColor|=(DlgItem->Flags & B_MASK)>>4;
				}

				if (Param1 >= 19 && Param1 <= 34) // Back
				{
					NewColor&=~0xF0;
					NewColor|=DlgItem->Flags & B_MASK;
				}

				if (NewColor!=*CurColor)
					*CurColor=NewColor;

				xf_free(DlgItem);
				return TRUE;
			}

			break;
		default:
			break;
	}

	return DefDlgProc(hDlg, Msg, Param1, Param2);
}


int GetColorDialog(WORD& Color,bool bCentered,bool bAddTransparent)
{
	DialogDataEx ColorDlgData[]=
	{
		/*   0 */ DI_DOUBLEBOX,   3, 1,35,13, 0,nullptr,nullptr,0,MSG(MSetColorTitle),
		/*   1 */ DI_SINGLEBOX,   5, 2,18, 7, 0,nullptr,nullptr,0,MSG(MSetColorForeground),
		/*   2 */ DI_RADIOBUTTON, 6, 3, 0, 3, 0,nullptr,nullptr,F_LIGHTGRAY|B_BLACK|DIF_GROUP|DIF_SETCOLOR|DIF_MOVESELECT,L"",
		/*   3 */ DI_RADIOBUTTON, 6, 4, 0, 4, 0,nullptr,nullptr,F_BLACK|B_RED|DIF_SETCOLOR|DIF_MOVESELECT,L"",
		/*   4 */ DI_RADIOBUTTON, 6, 5, 0, 5, 0,nullptr,nullptr,F_LIGHTGRAY|B_DARKGRAY|DIF_SETCOLOR|DIF_MOVESELECT,L"",
		/*   5 */ DI_RADIOBUTTON, 6, 6, 0, 6, 0,nullptr,nullptr,F_BLACK|B_LIGHTRED|DIF_SETCOLOR|DIF_MOVESELECT,L"",
		/*   6 */ DI_RADIOBUTTON, 9, 3, 0, 3, 0,nullptr,nullptr,F_LIGHTGRAY|B_BLUE|DIF_SETCOLOR|DIF_MOVESELECT,L"",
		/*   7 */ DI_RADIOBUTTON, 9, 4, 0, 4, 0,nullptr,nullptr,F_BLACK|B_MAGENTA|DIF_SETCOLOR|DIF_MOVESELECT,L"",
		/*   8 */ DI_RADIOBUTTON, 9, 5, 0, 5, 0,nullptr,nullptr,F_BLACK|B_LIGHTBLUE|DIF_SETCOLOR|DIF_MOVESELECT,L"",
		/*   9 */ DI_RADIOBUTTON, 9, 6, 0, 6, 0,nullptr,nullptr,F_BLACK|B_LIGHTMAGENTA|DIF_SETCOLOR|DIF_MOVESELECT,L"",
		/*  10 */ DI_RADIOBUTTON,12, 3, 0, 3, 0,nullptr,nullptr,F_BLACK|B_GREEN|DIF_SETCOLOR|DIF_MOVESELECT,L"",
		/*  11 */ DI_RADIOBUTTON,12, 4, 0, 4, 0,nullptr,nullptr,F_BLACK|B_BROWN|DIF_SETCOLOR|DIF_MOVESELECT,L"",
		/*  12 */ DI_RADIOBUTTON,12, 5, 0, 5, 0,nullptr,nullptr,F_BLACK|B_LIGHTGREEN|DIF_SETCOLOR|DIF_MOVESELECT,L"",
		/*  13 */ DI_RADIOBUTTON,12, 6, 0, 6, 0,nullptr,nullptr,F_BLACK|B_YELLOW|DIF_SETCOLOR|DIF_MOVESELECT,L"",
		/*  14 */ DI_RADIOBUTTON,15, 3, 0, 3, 0,nullptr,nullptr,F_BLACK|B_CYAN|DIF_SETCOLOR|DIF_MOVESELECT,L"",
		/*  15 */ DI_RADIOBUTTON,15, 4, 0, 4, 0,nullptr,nullptr,F_BLACK|B_LIGHTGRAY|DIF_SETCOLOR|DIF_MOVESELECT,L"",
		/*  16 */ DI_RADIOBUTTON,15, 5, 0, 5, 0,nullptr,nullptr,F_BLACK|B_LIGHTCYAN|DIF_SETCOLOR|DIF_MOVESELECT,L"",
		/*  17 */ DI_RADIOBUTTON,15, 6, 0, 6, 0,nullptr,nullptr,F_BLACK|B_WHITE|DIF_SETCOLOR|DIF_MOVESELECT,L"",
		/*  18 */ DI_SINGLEBOX,  20, 2,33, 7, 0,nullptr,nullptr,0,MSG(MSetColorBackground),
		/*  19 */ DI_RADIOBUTTON,21, 3, 0, 3, 0,nullptr,nullptr,F_LIGHTGRAY|B_BLACK|DIF_GROUP|DIF_SETCOLOR|DIF_MOVESELECT,L"",
		/*  20 */ DI_RADIOBUTTON,21, 4, 0, 4, 0,nullptr,nullptr,F_BLACK|B_RED|DIF_SETCOLOR|DIF_MOVESELECT,L"",
		/*  21 */ DI_RADIOBUTTON,21, 5, 0, 5, 0,nullptr,nullptr,F_LIGHTGRAY|B_DARKGRAY|DIF_SETCOLOR|DIF_MOVESELECT,L"",
		/*  22 */ DI_RADIOBUTTON,21, 6, 0, 6, 0,nullptr,nullptr,F_BLACK|B_LIGHTRED|DIF_SETCOLOR|DIF_MOVESELECT,L"",
		/*  23 */ DI_RADIOBUTTON,24, 3, 0, 3, 0,nullptr,nullptr,F_LIGHTGRAY|B_BLUE|DIF_SETCOLOR|DIF_MOVESELECT,L"",
		/*  24 */ DI_RADIOBUTTON,24, 4, 0, 4, 0,nullptr,nullptr,F_BLACK|B_MAGENTA|DIF_SETCOLOR|DIF_MOVESELECT,L"",
		/*  25 */ DI_RADIOBUTTON,24, 5, 0, 5, 0,nullptr,nullptr,F_BLACK|B_LIGHTBLUE|DIF_SETCOLOR|DIF_MOVESELECT,L"",
		/*  26 */ DI_RADIOBUTTON,24, 6, 0, 6, 0,nullptr,nullptr,F_BLACK|B_LIGHTMAGENTA|DIF_SETCOLOR|DIF_MOVESELECT,L"",
		/*  27 */ DI_RADIOBUTTON,27, 3, 0, 3, 0,nullptr,nullptr,F_BLACK|B_GREEN|DIF_SETCOLOR|DIF_MOVESELECT,L"",
		/*  28 */ DI_RADIOBUTTON,27, 4, 0, 4, 0,nullptr,nullptr,F_BLACK|B_BROWN|DIF_SETCOLOR|DIF_MOVESELECT,L"",
		/*  29 */ DI_RADIOBUTTON,27, 5, 0, 5, 0,nullptr,nullptr,F_BLACK|B_LIGHTGREEN|DIF_SETCOLOR|DIF_MOVESELECT,L"",
		/*  30 */ DI_RADIOBUTTON,27, 6, 0, 6, 0,nullptr,nullptr,F_BLACK|B_YELLOW|DIF_SETCOLOR|DIF_MOVESELECT,L"",
		/*  31 */ DI_RADIOBUTTON,30, 3, 0, 3, 0,nullptr,nullptr,F_BLACK|B_CYAN|DIF_SETCOLOR|DIF_MOVESELECT,L"",
		/*  32 */ DI_RADIOBUTTON,30, 4, 0, 4, 0,nullptr,nullptr,F_BLACK|B_LIGHTGRAY|DIF_SETCOLOR|DIF_MOVESELECT,L"",
		/*  33 */ DI_RADIOBUTTON,30, 5, 0, 5, 0,nullptr,nullptr,F_BLACK|B_LIGHTCYAN|DIF_SETCOLOR|DIF_MOVESELECT,L"",
		/*  34 */ DI_RADIOBUTTON,30, 6, 0, 6, 0,nullptr,nullptr,F_BLACK|B_WHITE|DIF_SETCOLOR|DIF_MOVESELECT,L"",

		/*  35 */ DI_CHECKBOX,    5, 10,0, 10,0,nullptr,nullptr,0,MSG(MSetColorForeTransparent),
		/*  36 */ DI_CHECKBOX,   22, 10,0, 10,0,nullptr,nullptr,0,MSG(MSetColorBackTransparent),

		/*  37 */ DI_TEXT,        5, 8, 33,8, 0,nullptr,nullptr,DIF_SETCOLOR,MSG(MSetColorSample),
		/*  38 */ DI_TEXT,        5, 9, 33,9, 0,nullptr,nullptr,DIF_SETCOLOR,MSG(MSetColorSample),
		/*  39 */ DI_TEXT,        5,10, 33,10,0,nullptr,nullptr,DIF_SETCOLOR,MSG(MSetColorSample),
		/*  40 */ DI_TEXT,        0,11, 0, 11,0,nullptr,nullptr,DIF_SEPARATOR,L"",
		/*  41 */ DI_BUTTON,      0,12, 0, 12,0,nullptr,nullptr,DIF_DEFAULTBUTTON|DIF_CENTERGROUP,MSG(MSetColorSet),
		/*  42 */ DI_BUTTON,      0,12, 0, 12,0,nullptr,nullptr,DIF_CENTERGROUP,MSG(MSetColorCancel),

	};
	MakeDialogItemsEx(ColorDlgData,ColorDlg);
	int ExitCode;
	WORD CurColor=Color;

	for (size_t i=2; i<18; i++)
	{
		if (static_cast<WORD>((ColorDlg[i].Flags & B_MASK)>>4)==(Color & F_MASK))
		{
			ColorDlg[i].Selected=1;
			ColorDlg[i].Flags|=DIF_FOCUS;
			break;
		}
	}

	for (size_t i=19; i<35; i++)
	{
		if (static_cast<WORD>(ColorDlg[i].Flags & B_MASK)==(Color & B_MASK))
		{
			ColorDlg[i].Selected=1;
			break;
		}
	}

	for (size_t i=37; i<40; i++)
	{
		ColorDlg[i].Flags=(ColorDlg[i].Flags & ~DIF_COLORMASK) | Color;
	}

	if (bAddTransparent)
	{
		ColorDlg[0].Y2++;

		for (size_t i=37; i<=42; i++)
		{
			ColorDlg[i].Y1+=3;
			ColorDlg[i].Y2+=3;
		}

		ColorDlg[0].X2+=4;
		ColorDlg[0].Y2+=2;
		ColorDlg[1].X2+=2;
		ColorDlg[1].Y2+=2;
		ColorDlg[18].X1+=2;
		ColorDlg[18].X2+=4;
		ColorDlg[18].Y2+=2;

		for (size_t i=2; i<=17; i++)
		{
			ColorDlg[i].X1+=1;
			ColorDlg[i].Y1+=1;
			ColorDlg[i].Y2+=1;
		}

		for (size_t i=19; i<=34; i++)
		{
			ColorDlg[i].X1+=3;
			ColorDlg[i].Y1+=1;
			ColorDlg[i].Y2+=1;
		}

		for (size_t i=37; i<=39; i++)
		{
			ColorDlg[i].X2+=4;
		}

		ColorDlg[35].Selected=(Color&0x0F00?1:0);
		ColorDlg[36].Selected=(Color&0xF000?1:0);
	}
	else
	{
		ColorDlg[35].Flags|=DIF_HIDDEN;
		ColorDlg[36].Flags|=DIF_HIDDEN;
	}

	{
		Dialog Dlg(ColorDlg,ARRAYSIZE(ColorDlg), GetColorDlgProc, (INT_PTR) &CurColor);

		if (bCentered)
			Dlg.SetPosition(-1,-1,39+(bAddTransparent?4:0),15+(bAddTransparent?3:0));
		else
			Dlg.SetPosition(37,2,75+(bAddTransparent?4:0),16+(bAddTransparent?3:0));

		Dlg.Process();
		ExitCode=Dlg.GetExitCode();
	}

	if (ExitCode==41)
	{
		Color=CurColor;

		if (ColorDlg[35].Selected)
			Color|=0x0F00;
		else
			Color&=0xF0FF;

		if (ColorDlg[36].Selected)
			Color|=0xF000;
		else
			Color&=0x0FFF;

		return TRUE;
	}

	return FALSE;
}
