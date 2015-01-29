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
	// ================================================================================ //
	//                                  JBOX CONTROLER                                  //
	// ================================================================================ //
	
	//! The jBox is the juce implementation of the box::controller.
	/**
	 The jBox overrides the redraw and paint method and wraps the mouse and keyboard events.
	 */
	class jTextField : public Gui::TextField, public Label
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
	
		jTextField() noexcept
		{
			Label::setText("teststring", dontSendNotification);
			
			setEditable(false, true, false);
						
			Font font(13);
			font.setTypefaceName("Menelo");
			setFont(font);
			
			setMinimumHorizontalScale(1);
			setJustificationType(Justification::topLeft);
			setBorderSize(BorderSize<int>(3, 6, 3, 6));
			setColour(Label::ColourIds::backgroundWhenEditingColourId, Colours::transparentWhite);
			//m_textfield->setColour(Label::ColourIds::backgroundColourId, Colours::palegoldenrod);
			setEditable(false, true);
			//m_textfield->setEditable(false);
			//m_textfield->addListener(this);
		}
	
		~jTextField()
		{
			;
		}
	
		void setText(wstring const& newtext) override
		{
			Label::setText(newtext.c_str(), dontSendNotification);
		}
	};

	typedef shared_ptr<jTextField>		 sjTextField;
	typedef weak_ptr<jTextField>         wjTextField;
	typedef shared_ptr<const jTextField> scjTextField;
	typedef weak_ptr<const jTextField>   wcjTextField;
}

#endif
