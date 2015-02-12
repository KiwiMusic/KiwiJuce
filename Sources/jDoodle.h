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

#include "jDefs.h"

namespace Kiwi
{
    template<typename type> static inline juce::Point<type> toJuce(Kiwi::Point const& pt)
    {
        return juce::Point<type>((type)pt.x(), (type)pt.y());
    }
    
    template<typename type> static inline Kiwi::Point toKiwi(juce::Point<type> const& rect)
    {
        return Kiwi::Point(rect.getX(), rect.getY());
    }
    
    template<typename type> static inline juce::Rectangle<type> toJuce(Kiwi::Rectangle const& rect)
    {
        return juce::Rectangle<type>((type)rect.x(), (type)rect.y(), (type)rect.width(), (type)rect.height());
    }
    
    template<typename type> static inline Kiwi::Rectangle toKiwi(juce::Rectangle<type> const& rect)
    {
        return Kiwi::Rectangle(rect.getX(), rect.getY(), rect.getWidth(), rect.getHeight());
    }
    
    static inline juce::Colour toJuce(Kiwi::Color const& color)
    {
        return Colour::fromFloatRGBA(color.red(), color.green(), color.blue(), color.alpha());
    }
    
    static inline Kiwi::Color toKiwi(juce::Colour const& color)
    {
        return Kiwi::Color(color.getFloatRed(), color.getFloatGreen(), color.getFloatBlue(), color.getFloatAlpha());
    }
    
    class JDoodle : public Kiwi::Gui::Doodle
    {
    private:
        Graphics &g;
        Kiwi::Rectangle bounds;
    public:
        
        //! Constructor.
        /** The function initialize the juce graphics and the bounds.
         */
        JDoodle(Graphics& _g, juce::Rectangle<int> b);
		
		//! Constructor.
		/** The function initialize the juce graphics and the bounds.
		 */
		JDoodle(Graphics& _g, Kiwi::Rectangle b);
		
        //! Destructor.
        /** The function does nothing.
         */
        ~JDoodle();
		
        //! Set the color.
        /** The sets the color that now will be used by the doodle.
         @param colot The color.
         */
        void setColor(Kiwi::Color const& color) override;
        
        //! Set the font.
        /** The sets the font that now will be used by the doodle.
         @param font The font.
         */
        void setFont(Kiwi::Font const& font) override;
        
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
        void drawText(string const& text, double x, double y, double w, double h, Kiwi::Font::Justification j, bool truncated = false) override;
        
        //! Draw a line of text within a rectangle.
        /** The function draws a line of text within a rectangle.
         @param text The text.
         @param rect The rectangle.
         @param j The justification.
         @param truncated If the text should be truncated if it goes out the boundaries.
         */
        void drawText(string const& text, Kiwi::Rectangle const& rect, Kiwi::Font::Justification j, bool wrap = false) override;
		
		//! Tries to draw a text string inside a given rectangle.
		/** The function tries to draw a text string inside a given space.
		 @see drawFittedText
		 */
		void drawFittedText(string const& text, const double x, const double y, const double width, const double height, Kiwi::Font::Justification j, const long maximumNumberOfLines, const double minimumHorizontalScale) override;
		
		//! Tries to draw a text string inside a given rectangle.
		/** The function tries to draw a text string inside a given space.
		 @see drawFittedText
		 */
		void drawFittedText(string const& text, Kiwi::Rectangle const& rect, Kiwi::Font::Justification j, const long maximumNumberOfLines, const double minimumHorizontalScale) override;
		
		//! Draws text across multiple lines.
		/** Draws text across multiple lines. This will break the text onto a new line
		 where there's a new-line or carriage-return character, or at a word-boundary when the text becomes wider
		 than the size specified by the maximumLineWidth parameter.
		 @see setFont, drawFittedText
		 */
		void drawMultiLineText(wstring const& text, const long startX, const long baselineY, const long maximumLineWidth) const override;
        
        //! Fill a path.
        /** The function fills a patcher.
         @param path The path.
         */
        void fillPath(Kiwi::Path const& path) override;
        
        //! Draw a path.
        /** The function draws a patcher.
         @param path The path.
         @param thickness The thickness of the parth.
         */
        void drawPath(const Kiwi::Path& path, double const thickness) override;
        
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
        inline Kiwi::Point getPosition() override
        {
			return bounds.position();
        }

        //! Retrieve the size.
        /** The function retrieves the size.
         @return The size.
         */
        inline Kiwi::Point getSize() override
        {
            return bounds.size();
        }

        //! Retrieve the bounds.
        /** The function retrieves the bounds.
         @return The bounds.
         */
        inline Kiwi::Rectangle getBounds() override
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

        inline void drawRectangle(Kiwi::Rectangle const& rect, double thickness, double rounded = 0.) override
        {
			g.drawRoundedRectangle(rect.x(), rect.y(), rect.width(), rect.height(), rounded, thickness);
        }

        inline void fillRectangle(double x, double y, double w, double h, double rounded = 0.) override
        {
			g.fillRoundedRectangle(x, y, w, h, rounded);
        }

        inline void fillRectangle(Kiwi::Rectangle const& rect, double rounded = 0.) override
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
