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

#ifndef __DEF_KIWI_ATTRIBUTECOLORCOMPONENT__
#define __DEF_KIWI_ATTRIBUTECOLORCOMPONENT__

#include "../JuceLibraryCode/JuceHeader.h"
#include "AttributeComponent.h"

namespace Kiwi
{
	class PopupColourSelector   : public Component,
	public ChangeListener,
	public Value::Listener
	{
	public:
		PopupColourSelector (const Value& colour)
		: colourValue (colour)
		{
			addAndMakeVisible (selector);
			selector.setName ("Colour");
			selector.setCurrentColour (getColour());
			selector.addChangeListener (this);
			
			colourValue.addListener (this);
			setSize (300, 400);
		}
		
		void resized()
		{
			selector.setBounds (getLocalBounds());
		}
		
		Colour getColour() const
		{
			if (colourValue.toString().isEmpty())
				return Colour();
			
			return Colour::fromString (colourValue.toString());
		}
		
		void setColour (Colour newColour)
		{
			if (getColour() != newColour)
			{
				colourValue = newColour.toDisplayString (true);
			}
		}
	
		void changeListenerCallback (ChangeBroadcaster*) override
		{
			if (selector.getCurrentColour() != getColour())
				setColour (selector.getCurrentColour());
		}

		void valueChanged (Value&) override
		{
			selector.setCurrentColour (getColour());
		}

	private:
		ColourSelector selector;
		Value colourValue;
	};

	//==============================================================================

	/** A AttributeComponent that shows a colour swatch with hex ARGB value, and which pops up
	 a colour selector when you click it.
	 @see AttributeComponent
	 */
	class AttributeColorComponent : public AttributeComponent, public Value::Listener
	{
	public:
		//==============================================================================
		/** Creates a button component.
		 
		 If you use this constructor, you must override the getState() and setState()
		 methods.
		 */
		AttributeColorComponent (sAttr attribute, Attr::sManager attrsManager);

		/** Destructor. */
		~AttributeColorComponent();
		
		virtual Colour getColour() const;
		
		void valueChanged(Value&) override;
		
		//==============================================================================
		/** @internal */
		void mouseDown(const MouseEvent&) override;
		/** @internal */
		void mouseUp(const MouseEvent&) override;
		/** @internal */
		void paint (Graphics&) override;
		/** @internal */
		void refresh() override;
		
	private:
		Value			m_color;
		Colour			m_lastcolor;
		
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AttributeColorComponent)
	};
}

#endif