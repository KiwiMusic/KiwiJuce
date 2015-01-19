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

#ifndef __DEF_KIWI_ATTRIBUTEMENUCOMPONENT__
#define __DEF_KIWI_ATTRIBUTEMENUCOMPONENT__

#include "../JuceLibraryCode/JuceHeader.h"
#include "AttributeComponent.h"

namespace Kiwi
{
	//==============================================================================
	/**
	 An AttributeComponent that shows its value as a combo box.
	 
	 This type of attribute component contains a list of values and has a
	 combo box to choose one.
	 
	 @see AttributeComponent, AttributePanel, Attr, AttrEnum
	 */
	class JUCE_API  AttributeMenuComponent : public AttributeComponent, private ComboBox::Listener
	{
	public:
		/** Creates the component.
		 Your subclass's constructor must add a list of options to the choices member variable.
		 */
		AttributeMenuComponent (sAttr attribute, Attr::sManager attrsManager);
		
		/** Destructor. */
		~AttributeMenuComponent();
		
		//==============================================================================
		/** @internal */
		void refresh();
		
	private:
		ComboBox m_combo_box;
		void createComboBox();
		void comboBoxChanged (ComboBox*);
		
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AttributeMenuComponent)
	};	
}

#endif