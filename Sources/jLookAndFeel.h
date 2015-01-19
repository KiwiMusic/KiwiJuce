/*
 ==============================================================================
 
 This file is part of the KIWI library.
 Copyright (c) 2014 Pierre Guillot & Eliott Paris.
 
 Permission is granted to use this software under the terms of either:
 a) the GPL v2 (or any later version)
 b) the Affero GPL v3
 
 Details of these licenses can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 To release a closed-source product which uses KIWI, contact : guillotpierre6@gmail.com
 
 ==============================================================================
 */

#ifndef __DEF_KIWI_JLOOKANDFEEL__
#define __DEF_KIWI_JLOOKANDFEEL__

#include "../JuceLibraryCode/JuceHeader.h"

class KiwiLookAndFeel   : public LookAndFeel_V3
{
public:
	KiwiLookAndFeel();
	
	/** Draw any border */
	void drawTextEditorOutline(Graphics& g, int width, int height, TextEditor& textEditor) override {}

    void drawTabButton(TabBarButton& button, Graphics&, bool isMouseOver, bool isMouseDown) override;
    void drawTabAreaBehindFrontButton(TabbedButtonBar&, Graphics&, int, int) override {}

    int getTabButtonBestWidth(TabBarButton&, int tabDepth) override;
	
	//==============================================================================
	
	void drawTableHeaderBackground(Graphics& g, TableHeaderComponent& header) override;
	
	void drawScrollbar(Graphics& g, ScrollBar& scrollbar,
					   int x, int y, int width, int height,
					   bool isScrollbarVertical, int thumbStartPosition, int thumbSize,
					   bool isMouseOver, bool isMouseDown) override;
	
	void drawPopupMenuBackground(Graphics &, int width, int height) override;
};

enum ColourIds
{
	mainBackgroundColourId          = 0x2340000,
};



#endif
