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

#ifndef __DEF_KIWI_GUI_JUCE_DEVICE__
#define __DEF_KIWI_GUI_JUCE_DEVICE__

#include "KiwiGuiJuceView.h"

namespace Kiwi
{
    
    class KiwiJuceGuiDeviceManager :    public GuiDeviceManager,
                                        public ApplicationCommandManager,
                                        public enable_shared_from_this<KiwiJuceGuiDeviceManager>
    {
    public:
        
        //! Constructor
        /**
         */
        KiwiJuceGuiDeviceManager();
        
        //! Destructor
        /**
         */
        ~KiwiJuceGuiDeviceManager();
        
        //! Create a view.
        /** The function creates a view for a controller.
         @param ctrl The controller linked with the view.
         @return The view.
         */
        sGuiView createView(sGuiController ctrl) noexcept override;
        
        //! Retrieves the mouse absolute position.
        /** The function retrieves the mouse absolute position.
         @return The mouse absolute position.
         */
        Point getMousePosition() const noexcept override;
        
        //! Retrieves the screen bounds.
        /** The function retrieves the screen bounds for a point. Since there can be several screens, the point determines wichs screen to select.
         @param pt The point.
         @return The screen bounds.
         */
        Rectangle getScreenBounds(Point const& pt) const noexcept override;
        
        //! Retrieves all the fonts from the system.
        /** The function retrieves all the fonts from the system.
         @return A vector of fonts.
         */
        vector<Font> getSystemFonts() const noexcept override;
        
        //! Retrieves the default system's font.
        /** The function retrieves the default system's font.
         @return A default font.
         */
        Font getSystemDefaultFont() const noexcept override;
        
    };
    
    class jInternalFont : public Kiwi::Font::Intern, private juce::Font
    {
    public:
        
        //! Font constructor.
        /** Initializes a intern font with a name, size and style.
         @param name    The name of the font.
         @param height  The height of the font.
         @param style   The style of the font.
         */
        inline jInternalFont(string const& name, double height, Kiwi::Font::Style style) noexcept :
        Kiwi::Font::Intern(name, height, style),
        juce::Font(juce::String(name), float(height), int(style)) {}
        
        //! Destructor.
        /** The function does nothing.
         */
        inline ~jInternalFont() noexcept {};
    
        //! Retrieves if the font is available in the system.
        /** The function retrieves if the font is available in the system.
         @return true if the font is available otherwise false.
         */
        inline bool isValid() const noexcept override
        {
            return true;
        }
        
        //! Retrieves the font name.
        /** The function retrieves the name of the font.
         @return The name of the font.
         */
        inline unique_ptr<Intern> getNewReference() const noexcept override
        {
            return unique_ptr<jInternalFont>(new jInternalFont(getName(), getHeight(), Kiwi::Font::Style(getStyle())));
        }
    
        //! Retrieves the font height.
        /** The function retrieves the height of the font.
         @return The height of the font.
         */
        inline double getHeight() const noexcept override
        {
            return juce::Font::getHeight();
        }
        
        //! Retrieves the font style.
        /** The function retrieves the style of the font.
         @return true if the font is valid, otherwise false.
         */
        inline unsigned getStyle() const noexcept override
        {
            return unsigned(juce::Font::getStyleFlags());
        }
        
        //! Sets the font height.
        /** The function sets the height of the font.
         @param size The height of the font.
         */
        inline void setHeight(const double size) override
        {
            juce::Font::setHeight(float(size));
        }
        
        //! Sets the font style.
        /** The function sets the style of the font.
         @param style The style of the font as a set flags.
         */
        inline void setStyle(const Kiwi::Font::Style style) override
        {
            juce::Font::setStyleFlags(int(style));
        }
        
        //! Retrieves the width of a character.
        /** The function retreives the width of a character for the font.
         @param c The character.
         @return The width of the character.
         */
        double getCharacterWidth(char const& c) const noexcept override;
        
        //! Retrieves the width of a character.
        /** The function retreives the width of a character for the font.
         @param c The character.
         @return The width of the character.
         */
        double getCharacterWidth(wchar_t const& c) const noexcept override;
        
        //! Retrieves the width of a line.
        /** The function retreives the width of a line for the font.
         @param line The line.
         @return The width of the line.
         */
        double getLineWidth(string const& line) const noexcept override;
        
        //! Retrieves the width of a line.
        /** The function retreives the width of a line for the font.
         @param line The line.
         @return The width of the line.
         */
        double getLineWidth(wstring const& line) const noexcept override;
        
        //! Retrieves the size of a text.
        /** The function the size of a text depending for the font.
         @param text The text.
         @param width The width limit of the text, zero means no limits.
         @return The width of the text.
         */
        Size getTextSize(string const& text, const double width = 0.) const noexcept override;
        
        //! Retrieves the size of a text.
        /** The function the width of a text depending for the font.
         @param text The text.
         @param width The width limit of the text, zero means no limits.
         @return The width of the text.
         */
        Size getTextSize(wstring const& text, const double width = 0.) const noexcept override;
        
    };
}

#endif

#endif


