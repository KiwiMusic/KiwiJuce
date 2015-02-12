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

#ifndef __DEF_KIWI_JTEXTLAYOUT__
#define __DEF_KIWI_JTEXTLAYOUT__

#include "jDefs.h"

namespace Kiwi
{
    class jTextLayout : public Kiwi::TextLayout
    {
    private:
		GlyphArrangement layout;
		
    public:
        
        //! Constructor.
        /** The function initialize the juce textlayout.
         */
        jTextLayout();
		
        //! Destructor.
        /** The function does nothing.
         */
        ~jTextLayout();
		
		//! Set the text and attributes.
		/**	This function sets the text and attributes of the textfield.
		 @param	text			The text to render.
		 @param	font			The font with which to render the text.
		 @param	bounds			The text is placed within the specified rect.
		 @param	justification	How to justify the text within the rect.
		 @param	wordwrap		enable/disable word wrapping.
		 @param	ellipsis		show/don't show ... if a line doesn't fit bounds (implies enabled word wrapping).
		 */
		void set(wstring const& text,
				Kiwi::Font const& font,
				Kiwi::Rectangle const& bounds,
				Kiwi::Font::Justification justification,
				const bool wordwrap = false,
				const bool ellipsis = false) override;
    };
}

#endif
