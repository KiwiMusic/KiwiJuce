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

#ifdef __KIWI_JUCE_WRAPPER__

#include "KiwiGuiJuceDevice.h"

namespace Kiwi
{
    KiwiJuceGuiDeviceManager::KiwiJuceGuiDeviceManager()
    {
        ;
    }
    
    KiwiJuceGuiDeviceManager::~KiwiJuceGuiDeviceManager()
    {
        ;
    }
    
    sGuiView KiwiJuceGuiDeviceManager::createView(sGuiController ctrl) noexcept
    {
        if(ctrl->wantKeyboard() && !ctrl->wantActions())
        {
            //return make_shared<jViewTextInput>(shared_from_this(), ctrl);
        }
        return make_shared<jView>(shared_from_this(), ctrl);
    }
    
    Point KiwiJuceGuiDeviceManager::getMousePosition() const noexcept
    {
        return toKiwi(Desktop::getMousePosition());
    }
    
    Rectangle KiwiJuceGuiDeviceManager::getScreenBounds(Point const& pt) const noexcept
    {
        return toKiwi(Desktop::getInstance().getDisplays().getDisplayContaining(toJuce<int>(pt)).userArea);
    }
    
    vector<Font> KiwiJuceGuiDeviceManager::getSystemFonts() const noexcept
    {
        vector<Font>        fonts;
        StringArray names = juce::Font::findAllTypefaceNames();
        for(int i = 0; i < names.size(); i++)
        {
            fonts.push_back(Kiwi::Font(unique_ptr<jInternalFont>(new jInternalFont(names[i].toStdString(), 12., Kiwi::Font::Regular))));
        }
        return fonts;
    }
    
    Kiwi::Font KiwiJuceGuiDeviceManager::getSystemDefaultFont() const noexcept
    {
        juce::Font font;
        return Kiwi::Font(unique_ptr<jInternalFont>(new jInternalFont(font.getTypefaceName().toStdString(), 12., Kiwi::Font::Regular)));
    }
            
    double jInternalFont::getCharacterWidth(char const& c) const noexcept
    {
        return double(juce::Font::getStringWidthFloat(juce::String(c))) - juce::Font::getExtraKerningFactor();
    }
    
    double jInternalFont::getCharacterWidth(wchar_t const& c) const noexcept
    {
        Array<int> newGlyphs;
        Array<float> xOffsets;
        juce::Font::getGlyphPositions(String(c), newGlyphs, xOffsets);
        return double(xOffsets.getUnchecked(1) - juce::Font::getExtraKerningFactor());
    }
    
    double jInternalFont::getLineWidth(string const& line) const noexcept
    {
        return double(juce::Font::getStringWidthFloat(juce::String(line)));
    }
    
    double jInternalFont::getLineWidth(wstring const& line) const noexcept
    {
        return double(juce::Font::getStringWidthFloat(juce::String(line.c_str())));
    }
    
    Size jInternalFont::getTextSize(string const& text, const double width) const noexcept
    {
        juce::GlyphArrangement glypher;
        glypher.addJustifiedText(juce::Font(*this), String(text.c_str()), 0., 0., width > 0. ? width : numeric_limits<float>::max(), juce::Justification::topLeft);
        const juce::Rectangle<float> bounds = glypher.getBoundingBox(0, -1, true);
        if(text[text.size()-1ul] == '\n')
        {
            return Size(bounds.getWidth(), bounds.getHeight() + getHeight());
        }
        else
        {
            return Size(bounds.getWidth(), bounds.getHeight());
        }
    }
    
    Size jInternalFont::getTextSize(wstring const& text, const double width) const noexcept
    {
        juce::GlyphArrangement glypher;
        glypher.addJustifiedText(juce::Font(*this), String(text.c_str()), 0., 0., width > 0. ? width : numeric_limits<float>::max(), juce::Justification::topLeft);
        const juce::Rectangle<float> bounds = glypher.getBoundingBox(0, -1, true);
        if(text[text.size()-1ul] == L'\n')
        {
            return Size(bounds.getWidth(), bounds.getHeight() + getHeight());
        }
        else
        {
            return Size(bounds.getWidth(), bounds.getHeight());
        }
    }
}

#endif

