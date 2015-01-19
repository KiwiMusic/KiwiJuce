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

#ifndef __DEF_KIWI_ATTRIBUTENUMBERCOMPONENT__
#define __DEF_KIWI_ATTRIBUTENUMBERCOMPONENT__

#include "../JuceLibraryCode/JuceHeader.h"
#include "AttributeComponent.h"
#include "../Components/NumberBoxComponent.h"

namespace Kiwi
{
	//==============================================================================
	/**
	 An AttributeComponent that shows its value as a draggable/editable number.
	 
	 @see AttributeComponent, AttributeLong, AttributeDouble
	 */
	class AttributeNumberComponent  : public AttributeComponent, public ChangeListener
	{
	public:
		//==============================================================================
		/** Creates the property component.
		 
		 The ranges, interval and skew factor are passed to the Slider component.
		 
		 If you need to customise the slider in other ways, your constructor can
		 access the slider member variable and change it directly.
		 */
		AttributeNumberComponent (sAttr attribute, Attr::sManager attrsManager,
								  double rangeMin,
								  double rangeMax,
								  double interval);
		
		/** Destructor. */
		~AttributeNumberComponent();
		
		//==============================================================================
		/** Called when the user moves the slider to change its value.
		 
		 Your subclass must use this method to update whatever item this property
		 represents.
		 */
		virtual void setValue (double newValue);
		
		/** Returns the value that the slider should show. */
		virtual double getValue() const;
		
		//==============================================================================
		/** @internal */
		void refresh();
		/** @internal */
		void changeListenerCallback (ChangeBroadcaster*) override;
		
	protected:
		/** The slider component being used in this component.
		 Your subclass has access to this in case it needs to customise it in some way.
		 */
		//Slider slider;
		NumberBoxComponent m_numberbox;
		
	private:
		bool m_flonum;
		//==============================================================================
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AttributeNumberComponent)
	};	
}

#endif