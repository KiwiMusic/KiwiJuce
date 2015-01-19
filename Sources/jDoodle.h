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

#ifndef __DEF_KIWI_JDOODLE__
#define __DEF_KIWI_JDOODLE__

#include "Wrapper.h"

namespace Kiwi
{
    class JDoodle : public Kiwi::Gui::Doodle
    {
    private:
        Graphics &g;
        Gui::Rectangle bounds;
    public:
        
        //! Constructor.
        /** The function initialize the juce graphics and the bounds.
         */
        JDoodle(Graphics& _g, juce::Rectangle<int> b);
        
        //! Destructor.
        /** The function does nothing.
         */
        ~JDoodle();
		
        //! Set the color.
        /** The sets the color that now will be used by the doodle.
         @param colot The color.
         */
        void setColor(Gui::Color const& color) override;
        
        //! Set the font.
        /** The sets the font that now will be used by the doodle.
         @param font The font.
         */
        void setFont(Gui::Font const& font) override;
        
        //! Fill the doodle with a color.
        /** The function fills the entire doodle with a color.
         */
        void fillAll() override;
        
        //! Draw a line of text within a rectangle.
        /** The function draws a line of text within a rectangle.
         @param text The text.
         @param x The abscissa of the rectangle.
         @param y The ordinate of the rectangle.
         @param w The width of the rectangle.
         @param h The height of the rectangle.
         @param j The justification.
         @param truncated If the text should be truncated if it goes out the boundaries.
         */
        void drawText(string const& text, double x, double y, double w, double h, Gui::Font::Justification j, bool truncated = false) override;
        
        //! Draw a line of text within a rectangle.
        /** The function draws a line of text within a rectangle.
         @param text The text.
         @param rect The rectangle.
         @param j The justification.
         @param truncated If the text should be truncated if it goes out the boundaries.
         */
        void drawText(string const& text, Gui::Rectangle const& rect, Gui::Font::Justification j, bool wrap = false) override;
        
        //! Fill a path.
        /** The function fills a page.
         @param path The path.
         */
        void fillPath(Gui::Path const& path) override;
        
        //! Draw a path.
        /** The function draws a page.
         @param path The path.
         @param thickness The thickness of the parth.
         */
        void drawPath(const Gui::Path& path, double const thickness) override;
        
        //! Retrieve the abscissa.
        /** The function retrieves the abscissa.
         @return The abscissa.
         */
        inline double getX() override
        {
            return bounds.x();
        }
    
        //! Retrieve the ordinate.
        /** The function retrieves the ordinate.
         @return The ordinate.
         */
        inline double getY() override
        {
            return bounds.y();
        }

        //! Retrieve the width.
        /** The function retrieves the width.
         @return The width.
         */
        inline double getWidth() override
        {
            return bounds.width();
        }

        //! Retrieve the height.
        /** The function retrieves the height.
         @return The height.
         */
        inline double getHeight() override
        {
            return bounds.height();
        }

        //! Retrieve the position.
        /** The function retrieves the position.
         @return The position.
         */
        inline Gui::Point getPosition() override
        {
			return bounds.position();
        }

        //! Retrieve the size.
        /** The function retrieves the size.
         @return The size.
         */
        inline Gui::Point getSize() override
        {
            return bounds.size();
        }

        //! Retrieve the bounds.
        /** The function retrieves the bounds.
         @return The bounds.
         */
        inline Gui::Rectangle getBounds() override
        {
			return bounds;
        }

        inline void drawLine(double x1, double y1, double x2, double y2, double thickness) override
        {
			g.drawLine(x1, y1, x2, y2, thickness);
        }

        inline void drawRectangle(double x, double y, double w, double h, double thickness, double rounded = 0.) override
        {
			g.drawRoundedRectangle(x, y, w, h, rounded, thickness);
        }

        inline void drawRectangle(Gui::Rectangle const& rect, double thickness, double rounded = 0.) override
        {
			g.drawRoundedRectangle(rect.x(), rect.y(), rect.width(), rect.height(), rounded, thickness);
        }

        inline void fillRectangle(double x, double y, double w, double h, double rounded = 0.) override
        {
			g.fillRoundedRectangle(x, y, w, h, rounded);
        }

        inline void fillRectangle(Gui::Rectangle const& rect, double rounded = 0.) override
        {
			g.fillRoundedRectangle(rect.x(), rect.y(), rect.width(), rect.height(), rounded);
        }

        inline void drawEllipse(double x, double y, double width, double height, double thickness = 0.) override
        {
            g.drawEllipse(x, y, width, height, thickness);
        }

        inline void fillEllipse(double x, double y, double width, double height) override
        {
            g.fillEllipse(x, y, width, height);
        }
    };
}

#endif
