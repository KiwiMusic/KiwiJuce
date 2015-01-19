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

#ifndef __DEF_KIWI_ATTRIBUTECOMPONENT__
#define __DEF_KIWI_ATTRIBUTECOMPONENT__

#include "../JuceLibraryCode/JuceHeader.h"
#include "Kiwi.h"

namespace Kiwi
{
	
	//==============================================================================
	/**
	 A base class for a component that goes in an AttributePanel and displays one of
	 an item's attibute.
	 
	 Subclasses of this are used to display an attribute in various forms, e.g. an
	 AttributeMenuComponent shows its value as a combo box; an AttributeNumberComponent
	 shows its value as draggable/editable number box; an AttributeTextComponent as a text box, etc.
	 
	 A subclass must implement the refresh() method which will be called to tell the
	 component to update itself, and is also responsible for calling it when the
	 item that it refers to is changed.
	 
	 @see Attribute, AttributePanel, AttributeTextComponent, AttributeNumberComponent,
	 AttributeMenuComponent, AttributeBoolComponent
	 */
	class AttributeComponent  : public Component, public SettableTooltipClient,
								public Attr::Listener,
								public enable_shared_from_this<AttributeComponent>
	{
	public:
		//==============================================================================
		/** Creates an AttributeComponent.
		 
		 @param attributeName     the name is stored as this component's name, and is
		 used as the name displayed next to this component in
		 a attribute panel
		 @param preferredHeight  the height that the component should be given - some
		 items may need to be larger than a normal row height.
		 This value can also be set if a subclass changes the
		 preferredHeight member variable.
		 */
		AttributeComponent (sAttr attribute, Attr::sManager attrsManager, int preferredHeight = 25);
		
		/** Destructor. */
		~AttributeComponent();
		
		//! Attribute component creator.
		/** This static method creates and returns a shared pointer of an AttributeComponent and bind it to an Attr::Manager.
		 @param attrsManager A shared pointer of an Attribute::Manager to watch.
		 @return    A shared pointer of an AttributeComponent.
		 */
		template<class compClass, class ...Args> static shared_ptr<compClass> create(Args&& ...arguments)
		{
			shared_ptr<compClass> cp = make_shared<compClass>(forward<Args>(arguments)...);
			if(cp)
			{
                if(cp->m_manager)
                {
                    cp->m_manager->bind(cp);
                }
				cp->refresh();
			}
			return cp;
		}
		
		//! Receive the notification that an attribute has changed.
		/** Receive notifications when an attribute is added or removed, or when its value, appearance or behavior changes.
		 @param manager		The Attribute::Manager that manages the attribute.
		 @param attr		The attribute that has been modified.
		 @type type			The type of notification
		 */
		void notify(Attr::sManager manager, sAttr attr, Attr::Notification type) override;
		
		//==============================================================================
		/** Returns this item's preferred height.
		 
		 This value is specified either in the constructor or by a subclass changing the
		 preferredHeight member variable.
		 */
		int getPreferredHeight() const noexcept                 { return m_preferred_height; }
		
		void setPreferredHeight (int newHeight) noexcept        { m_preferred_height = newHeight; }
		
		/** Retrieves the area of the attribute value component
		 */
		juce::Rectangle<int> getAttributeContentComponentArea();
		
		//==============================================================================
		/** Updates the attribute component if the item it refers to has changed.
		 
		 A subclass must implement this method, and other objects may call it to
		 force it to refresh itself.
		 
		 The subclass should be economical in the amount of work is done, so for
		 example it should check whether it really needs to do a repaint rather than
		 just doing one every time this method is called, as it may be called when
		 the value being displayed hasn't actually changed.
		 */
		virtual void refresh() = 0;
		
		/** Sets the attribute value
		 */
		void getAttributeValue(ElemVector& elements);
		
		/** Sets the attribute value
		 */
		void setAttributeValue(ElemVector const& elements);
		
		/** The default paint method fills the background and draws a label for the
		 item's name.
		 
		 @see LookAndFeel::drawAttributeComponentBackground(), LookAndFeel::drawAttributeComponentLabel()
		 */
		void paint(Graphics&) override;
		
		/** The default paint method fills the background and draws a label for the
		 item's name.
		 
		 @see LookAndFeel::drawAttributeComponentBackground(), LookAndFeel::drawAttributeComponentLabel()
		 */
		void paintOverChildren(Graphics&) override;
		
		/** The default resize method positions any child component to the right of this
		 one, based on the look and feel's default label size.
		 */
		void resized() override;
		
		/** By default, this just repaints the component. */
		virtual void enablementChanged() override;
		
		//==============================================================================
		/** A set of colour IDs to use to change the colour of various aspects of attribute component.
		 
		 These constants can be used either via the Component::setColour(), or LookAndFeel::setColour()
		 methods.
		 
		 To change the colours of the menu that pops up
		 
		 @see Component::setColour, Component::findColour, LookAndFeel::setColour, LookAndFeel::findColour
		 */
		enum ColourIds
		{
			backgroundColourId	= 0x1008300,    /**< The background colour to fill the component with. */
			textColourId		= 0x1008301,    /**< The colour for the attribute's label text. */
		};
		
	protected:
		/** Used by the AttributePanel to determine how high this component needs to be.
		 A subclass can update this value in its constructor but shouldn't alter it later
		 as changes won't necessarily be picked up.
		 */
		int				m_preferred_height;
		
		sAttr			m_attr;
		Attr::sManager	m_manager;

	private:
		
		const String getAttrName() const noexcept;
		const String getAttrLabel() const noexcept;
	};	
}

#endif