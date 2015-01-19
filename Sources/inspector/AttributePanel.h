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

#ifndef __DEF_KIWI_ATTRIBUTEPANELCOMPONENT__
#define __DEF_KIWI_ATTRIBUTEPANELCOMPONENT__

#include "../JuceLibraryCode/JuceHeader.h"
#include "MainWindow.h"
#include "AttributeComponent.h"

namespace Kiwi
{
	//==============================================================================
	/**
	 A panel that holds a list of AttributeComponent objects.
	 
	 This panel displays a list of AttributeComponents, and allows them to be organised
	 into collapsible categorys.
	 
	 To use, simply create one of these and add your attributes to it with addAttribute()
	 or addCategory().
	 
	 @see AttributeComponent
	 */
	class AttributePanel  : public Component
	{
	public:
		//==============================================================================
		/** Creates an empty attribute panel. */
		AttributePanel();
		
		/** Creates an empty attribute panel. */
		AttributePanel (const String& name);
		
		/** Destructor. */
		~AttributePanel();
		
		//==============================================================================
		/** Deletes all attribute components from the panel. */
		void clear();
		
		/** Adds a set of attributes components to the panel.
		 The components in the list will be owned by this object and will be automatically
		 deleted later on when no longer needed.
		 These attributes are added without them being inside a named category. If you
		 want them to be kept together in a collapsible category, use addCategory() instead.
		 */
		void addAttributes (const Array <AttributeComponent*>& newAttributeComponents);
		
		/** Adds a set of attribute components to the panel.
		 
		 These components are added at the bottom of the list, under a category heading with
		 a plus/minus button that allows it to be opened and closed.
		 
		 The components in the list will be owned by this object and will be automatically
		 deleted later on when no longer needed.
		 
		 To add attribute components without them being in a category, use addAttributes().
		 */
		void addCategory (const String& categoryName,
						  const vector<shared_ptr<AttributeComponent>>& newAttributeComponents,
						  bool shouldCategoryInitiallyBeOpen = true);
		
		/** Calls the refresh() method of all AttributeComponents in the panel */
		void refreshAll() const;
		
		/** Calls the refresh() method of the AttributeComponent in the panel that is a view of this specific attribute */
		void refreshAttr(sAttr attr) const;
		
		/** Returns true if the panel contains no attribute. */
		bool isEmpty() const;
		
		/** Returns the height that the panel needs in order to display all of its content
		 without scrolling.
		 */
		int getTotalContentHeight() const;
		
		//==============================================================================
		/** Returns a list of all the names of categorys in the panel.
		 These are the categorys that have been added with addCategory().
		 */
		StringArray getCategoryNames() const;
		
		/** Returns true if the category at this index is currently open.
		 The index is from 0 up to the number of items returned by getCategoryNames().
		 */
		bool isCategoryOpen (int categoryIndex) const;
		
		/** Opens or closes one of the categorys.
		 The index is from 0 up to the number of items returned by getCategoryNames().
		 */
		void setCategoryOpen (int categoryIndex, bool shouldBeOpen);
		
		/** Enables or disables one of the categorys.
		 The index is from 0 up to the number of items returned by getCategoryNames().
		 */
		void setCategoryEnabled (int categoryIndex, bool shouldBeEnabled);
		
		//==============================================================================
		/** @internal */
		void paint (Graphics&) override;
		/** @internal */
		void resized() override;
		
	private:
		class CategoryComponent;
		class AttributeHolderComponent;
		
		Viewport										m_viewport;
		AttributeHolderComponent*						m_attribute_holder_comp;
		
		void init();
		void updateAttrHolderLayout() const;
		
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AttributePanel)
	};
}

#endif