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

#include "KiwiLookAndFeel.h"

//==============================================================================
KiwiLookAndFeel::KiwiLookAndFeel()
{
	setUsingNativeAlertWindows(true);
    setColour(mainBackgroundColourId, Colour::greyLevel(0.8f));
	setColour(PopupMenu::backgroundColourId, Colours::white.withAlpha(0.95f));
	setColour(PopupMenu::highlightedBackgroundColourId, Colour(76, 153, 249));
}

int KiwiLookAndFeel::getTabButtonBestWidth(TabBarButton&, int)
{
	return 120;
}

static Colour getTabBackgroundColour(TabBarButton& button)
{
    const Colour bkg(button.findColour(mainBackgroundColourId).contrasting(0.15f));

    if (button.isFrontTab())
        return bkg.overlaidWith (Colours::yellow.withAlpha (0.5f));

    return bkg;
}

void KiwiLookAndFeel::drawTabButton(TabBarButton& button, Graphics& g, bool isMouseOver, bool isMouseDown)
{
    const Rectangle<int> activeArea(button.getActiveArea());

    const Colour bkg(getTabBackgroundColour(button));

    g.setGradientFill(ColourGradient(bkg.brighter (0.1f), 0, (float)activeArea.getY(),
									 bkg.darker (0.1f), 0, (float)activeArea.getBottom(), false));
    g.fillRect(activeArea);

    g.setColour(button.findColour(mainBackgroundColourId).darker(0.3f));
    g.drawRect(activeArea);

    const float alpha = button.isEnabled() ? ((isMouseOver || isMouseDown) ? 1.0f : 0.8f) : 0.3f;
    const Colour col(bkg.contrasting().withMultipliedAlpha(alpha));

    TextLayout textLayout;
    LookAndFeel_V3::createTabTextLayout (button, (float)activeArea.getWidth(), (float)activeArea.getHeight(), col, textLayout);

    textLayout.draw(g, button.getTextArea().toFloat());
}

void KiwiLookAndFeel::drawTableHeaderBackground(Graphics& g, TableHeaderComponent& header)
{
	Rectangle<int> r (header.getLocalBounds());
	Rectangle<int> c = header.getColumnPosition(0);
	
	g.fillAll(Colour(0xff444444).withAlpha(0.2f));
	
	g.setColour (Colours::black.withAlpha(0.25f));
	g.drawHorizontalLine(r.getHeight()-1, 0, r.getWidth());
	
	int numColumns = header.getNumColumns(true);
	float left = 0, width = 0;
	
	for (int i = 0; i < numColumns; i++)
	{
		width = header.getColumnWidth(header.getColumnIdOfIndex(i, true));
		
		g.setColour (Colours::black.withAlpha (0.15f));
		g.drawLine(left + width, 0, left + width, r.getHeight());
		
		left += width;
	}
}

void KiwiLookAndFeel::drawScrollbar(Graphics& g, ScrollBar& scrollbar,
									int x, int y, int width, int height,
									bool isScrollbarVertical, int thumbStartPosition, int thumbSize,
									bool isMouseOver, bool isMouseDown)
{
	if (thumbSize <= 0) return;
	
	Path thumbPath;
	
	if (isScrollbarVertical)
		thumbPath.addRectangle(x, thumbStartPosition, width, thumbSize);
	else
		thumbPath.addRectangle(thumbStartPosition, y, thumbSize, height);
	
	Colour thumbCol(scrollbar.findColour(ScrollBar::thumbColourId, true));
	
	if (isMouseOver || isMouseDown)
		thumbCol = thumbCol.withMultipliedAlpha (2.0f);
	
	g.setColour(thumbCol);
	g.fillPath(thumbPath);
	
	g.setColour(thumbCol.contrasting((isMouseOver  || isMouseDown) ? 0.2f : 0.1f));
	g.strokePath(thumbPath, PathStrokeType (1.0f));
}

void KiwiLookAndFeel::drawPopupMenuBackground (Graphics& g, int width, int height)
{
	g.setColour (findColour (PopupMenu::backgroundColourId));
	g.fillRoundedRectangle(0, 0, width, height, 4);
	
	g.setColour (findColour (PopupMenu::backgroundColourId).contrasting(0.2));
	g.drawRoundedRectangle(0, 0, width, height, 4, 1);

#if ! JUCE_MAC
	g.setColour (findColour (PopupMenu::textColourId).withAlpha (0.6f));
	g.drawRect (0, 0, width, height);
#endif
}


