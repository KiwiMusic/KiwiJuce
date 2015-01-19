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

#ifndef __DEF_KIWI_ATTRIBUTEBOOLCOMPONENT__
#define __DEF_KIWI_ATTRIBUTEBOOLCOMPONENT__

#include "../JuceLibraryCode/JuceHeader.h"
#include "AttributeComponent.h"

namespace Kiwi
{
	//==============================================================================
	/**
	 An AttributeComponent that contains an on/off toggle button.
	 
	 This type of attribute component can be used if you have a boolean value to toggle on/off.
	 
	 @see AttributeComponent
	 */
	class AttributeBoolComponent : public AttributeComponent, private Button::Listener
	{
	public:
		//==============================================================================
		/** Creates a button component.
		 
		 If you use this constructor, you must override the getState() and setState()
		 methods.
		 */
		AttributeBoolComponent (sAttr attribute, Attr::sManager attrsManager);

		/** Destructor. */
		~AttributeBoolComponent();
		
		//==============================================================================
		/** Called to change the state of the boolean value. */
		virtual void setState (bool newState);
		
		/** Must return the current value of the attribute. */
		virtual bool getState() const;
		
		//==============================================================================
		/** A set of colour IDs to use to change the colour of various aspects of the component.
		 
		 These constants can be used either via the Component::setColour(), or LookAndFeel::setColour()
		 methods.
		 
		 @see Component::setColour, Component::findColour, LookAndFeel::setColour, LookAndFeel::findColour
		 */
		enum ColourIds
		{
			backgroundColourId          = 0x100e801,    /**< The colour to fill the background of the button area. */
			outlineColourId             = 0x100e803,    /**< The colour to use to draw an outline around the text area. */
		};
		
		//==============================================================================
		/** @internal */
		void paint (Graphics&) override;
		/** @internal */
		void refresh() override;
		/** @internal */
		void buttonClicked (Button*) override;
		
	private:
		ToggleButton button;
		
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AttributeBoolComponent)
	};
}

#endif