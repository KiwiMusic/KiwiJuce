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

#ifndef __DEF_KIWI_ATTRIBUTETEXTCOMPONENT__
#define __DEF_KIWI_ATTRIBUTETEXTCOMPONENT__

#include "../JuceLibraryCode/JuceHeader.h"
#include "AttributeComponent.h"

namespace Kiwi
{
	
	//==============================================================================
	/**
	 An AttributeComponent that shows its value as an editable text label.
	 
	 @see AttributeComponent
	 */
	class AttributeTextComponent  : public AttributeComponent
	{
	public:
		//==============================================================================
		/** Creates a text property component.
		 
		 The maxNumChars is used to set the length of string allowable, and isMultiLine
		 sets whether the text editor allows carriage returns.
		 
		 @see TextEditor
		 */
		AttributeTextComponent (sAttr attribute, Attr::sManager attrsManager,
								int maxNumChars,
								bool isMultiLine);
		
		/** Destructor. */
		~AttributeTextComponent();
		
		//==============================================================================
		/** Called when the user edits the text.
		 
		 Your subclass must use this callback to change the value of whatever item
		 this property component represents.
		 */
		virtual void setText (const String& newText);
		
		/** Returns the text that should be shown in the text editor. */
		virtual String getText() const;
		
		//==============================================================================
		/** A set of colour IDs to use to change the colour of various aspects of the component.
		 
		 These constants can be used either via the Component::setColour(), or LookAndFeel::setColour()
		 methods.
		 
		 @see Component::setColour, Component::findColour, LookAndFeel::setColour, LookAndFeel::findColour
		 */
		enum ColourIds
		{
			backgroundColourId          = 0x100e401,    /**< The colour to fill the background of the text area. */
			textColourId                = 0x100e402,    /**< The colour to use for the editable text. */
			outlineColourId             = 0x100e403,    /**< The colour to use to draw an outline around the text area. */
		};
		
		//==============================================================================
		/** @internal */
		void refresh() override;
		/** @internal */
		void enablementChanged() override;
		
	private:
		ScopedPointer<Label> textEditor;
		
		class LabelComp;
		friend class LabelComp;
		
		void textWasEdited();
		void createEditor (int maxNumChars, bool isMultiLine);
		
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AttributeTextComponent)
	};
	
}

#endif