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

#ifndef __DEF_KIWI_JTEXTFIELD__
#define __DEF_KIWI_JTEXTFIELD__

#include "jDefs.h"

namespace Kiwi
{
	class jTextField : public Gui::TextField, public TextEditor
    {
    private:
		
		//! Text Input filter class.
		/** Text Input filter that is applied to the text field to notify the box that a new character has been entered.
		 Boxes that owns a text field can implement the virtual textEntered function
		 */
		class TextFilter : public TextEditor::InputFilter
		{
		public:
			TextFilter(Gui::TextField::sOwner owner) {}
			~TextFilter() {}
			
			/** This method is called whenever text is entered into the editor.
			 An implementation of this class should should check the input string,
			 and return an edited version of it that should be used.
			 */
			String filterNewText(TextEditor& editor, const String& newInput) override
			{
				return newInput;
			}
		};
		
    public:
		
		jTextField() noexcept;
		
        ~jTextField()
        {
            ;
        }
    };
}

#endif
