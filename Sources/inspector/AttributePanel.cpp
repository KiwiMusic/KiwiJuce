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

#include "AttributePanel.h"

namespace Kiwi
{
	class AttributePanel::CategoryComponent  : public Component
	{
	public:
		CategoryComponent (const String& categoryTitle,
						   const vector<shared_ptr<AttributeComponent>>& newAttributes,
						   const bool categoryIsOpen_)
		: Component (categoryTitle),
		m_title_height (categoryTitle.isNotEmpty() ? 22 : 0),
		m_category_is_open (categoryIsOpen_)
		{
			m_attribute_comps = newAttributes;
			
			for (int i = m_attribute_comps.size(); --i >= 0;)
			{
				addAndMakeVisible (m_attribute_comps[i].get());
				m_attribute_comps[i]->refresh();
			}
		}
		
		~CategoryComponent()
		{
			m_attribute_comps.clear();
		}
		
		void paint (Graphics& g) override
		{
			if (m_title_height > 0)
			{
				const juce::Rectangle<int> titleRect = getLocalBounds().withHeight(m_title_height);
				
				//@see drawPropertyPanelCategoryHeader
				const float buttonSize = m_title_height * 0.75f;
				const float buttonIndent = (m_title_height - buttonSize) * 0.5f;
				
				//@see drawConcertinaPanelHeader
				const Colour bkg (Colours::grey);
				g.setGradientFill (ColourGradient (Colours::white.withAlpha (0.4f), 0, (float) titleRect.getY(),
												   Colours::darkgrey.withAlpha (0.1f), 0, (float) titleRect.getBottom(), false));
				g.fillAll();
				
				g.setColour (bkg.contrasting(0.5).withAlpha (0.1f));
				g.fillRect (titleRect.withHeight (1));
				g.fillRect (titleRect.withTop (titleRect.getBottom() - 1));
				
				const int textX = (int) (buttonIndent * 2.0f + buttonSize + 2.0f);
				
				g.setColour (bkg.contrasting());
				g.setFont (juce::Font(titleRect.getHeight() * 0.6f).boldened());
				g.drawFittedText(getName(), textX, 0, getWidth() - textX - 4, m_title_height, Justification::centredLeft, 1);
				
				//@see drawTreeviewPlusMinusBox
				const juce::Rectangle<float> plusRect = juce::Rectangle<float>(buttonIndent, buttonIndent, buttonSize, buttonSize);
				
                juce::Path p;
				p.addTriangle (0.0f, 0.0f, 1.0f, isOpen() ? 0.0f : 0.5f, isOpen() ? 0.5f : 0.0f, 1.0f);
				g.setColour (Colours::white.contrasting().withAlpha (isMouseOver() ? 0.5f : 0.3f));
				g.fillPath (p, p.getTransformToScaleToFit (plusRect.reduced (2, plusRect.getHeight() / 4), true));
			}
		}
		
		void resized() override
		{
			int y = m_title_height;
			
			for (int i = 0; i < m_attribute_comps.size(); ++i)
			{
				shared_ptr<AttributeComponent> const comp = m_attribute_comps[i];
				comp->setBounds (1, y, getWidth() - 2, comp->getPreferredHeight());
				y = comp->getBottom();
			}
		}
		
		int getPreferredHeight() const
		{
			int y = m_title_height;
			
			if (isOpen())
			{
				for (int i = m_attribute_comps.size(); --i >= 0;)
					y += m_attribute_comps[i]->getPreferredHeight();
			}
			
			return y;
		}
		
		void setOpen (const bool open)
		{
			if (m_category_is_open != open)
			{
				m_category_is_open = open;
				
				for (int i = m_attribute_comps.size(); --i >= 0;)
					m_attribute_comps[i]->setVisible (open);
				
				if (AttributePanel* const pp = findParentComponentOfClass<AttributePanel>())
					pp->resized();
			}
		}
		
		bool isOpen() const
		{
			return m_category_is_open;
		}
		
		void refreshAll() const
		{
			for (int i = m_attribute_comps.size(); --i >= 0;)
				m_attribute_comps[i]->refresh();
		}
		
		void mouseUp (const MouseEvent& e) override
		{
			if (e.getMouseDownX() < m_title_height
				&& e.x < m_title_height
				&& e.y < m_title_height
				&& e.getNumberOfClicks() != 2)
			{
				setOpen (! isOpen());
			}
		}
		
		void mouseDoubleClick (const MouseEvent& e) override
		{
			if (e.y < m_title_height)
				setOpen (! isOpen());
		}
		
	private:
		vector<shared_ptr<AttributeComponent>>	m_attribute_comps;
		int										m_title_height;
		bool									m_category_is_open;
		
		JUCE_DECLARE_NON_COPYABLE (CategoryComponent)
	};
	
	//==============================================================================
	class AttributePanel::AttributeHolderComponent  : public Component
	{
	public:
		AttributeHolderComponent() {}
		
		~AttributeHolderComponent()
		{
			clear();
		}
		
		void paint (Graphics&) override {}
		
		void updateLayout (int width)
		{
			int y = 0;
			
			for (int i = 0; i < m_categories.size(); ++i)
			{
				shared_ptr<CategoryComponent> const category = m_categories[i];
				
				category->setBounds (0, y, width, category->getPreferredHeight());
				y = category->getBottom();
			}
			
			setSize (width, y);
			repaint();
		}
		
		void refreshAll() const
		{
			for (int i = 0; i < m_categories.size(); ++i)
				m_categories[i]->refreshAll();
		}
		
		void clear()
		{
			m_categories.clear();
		}
		
		void addCategory (shared_ptr<CategoryComponent> newCategory)
		{
			m_categories.push_back(newCategory);
			addAndMakeVisible(newCategory.get(), 0);
		}
		
		int getNumCategories() const noexcept								{ return m_categories.size(); }
		shared_ptr<CategoryComponent> getCategory (const int index) const	{ return m_categories[index]; }
		
	private:
		
		vector<shared_ptr<CategoryComponent>>					m_categories;
		
		JUCE_DECLARE_NON_COPYABLE (AttributeHolderComponent)
	};
	
	
	//==============================================================================
	AttributePanel::AttributePanel()
	{
		init();
	}
	
	AttributePanel::AttributePanel (const String& name)  : Component (name)
	{
		init();
	}
	
	void AttributePanel::init()
	{
		m_viewport.setScrollBarThickness(12.);
		addAndMakeVisible (m_viewport);
		m_viewport.setViewedComponent (m_attribute_holder_comp = new AttributeHolderComponent());
		m_viewport.setFocusContainer (true);
	}
	
	AttributePanel::~AttributePanel()
	{
		clear();
	}
	
	//==============================================================================
	void AttributePanel::paint (Graphics& g)
	{
	}
	
	void AttributePanel::resized()
	{
		m_viewport.setBounds (getLocalBounds());
		updateAttrHolderLayout();
	}
	
	//==============================================================================
	void AttributePanel::clear()
	{
		if (! isEmpty())
		{
			m_attribute_holder_comp->clear();
			updateAttrHolderLayout();
		}
	}
	
	bool AttributePanel::isEmpty() const
	{
		return m_attribute_holder_comp->getNumCategories() == 0;
	}
	
	int AttributePanel::getTotalContentHeight() const
	{
		return m_attribute_holder_comp->getHeight();
	}
	
	void AttributePanel::addCategory(const String& categoryName,
									 const vector<shared_ptr<AttributeComponent>>& newAttributes,
									 bool shouldBeOpen)
	{
		if (isEmpty())
			repaint();
		
		m_attribute_holder_comp->addCategory (make_shared<CategoryComponent>(categoryName, newAttributes, shouldBeOpen));

		//m_attribute_holder_comp->addCategory (new CategoryComponent (categoryName, newAttributes, shouldBeOpen));
		updateAttrHolderLayout();
	}
	
	void AttributePanel::updateAttrHolderLayout() const
	{
		const int maxWidth = m_viewport.getMaximumVisibleWidth();
		m_attribute_holder_comp->updateLayout (maxWidth);
		
		const int newMaxWidth = m_viewport.getMaximumVisibleWidth();
		if (maxWidth != newMaxWidth)
		{
			// need to do this twice because of scrollbars changing the size, etc.
			m_attribute_holder_comp->updateLayout (newMaxWidth);
		}
	}
	
	void AttributePanel::refreshAll() const
	{
		m_attribute_holder_comp->refreshAll();
	}
	
	void AttributePanel::refreshAttr(sAttr attr) const
	{
		
	}
	
	//==============================================================================
	StringArray AttributePanel::getCategoryNames() const
	{
		StringArray s;
		
		for (int i = 0; i < m_attribute_holder_comp->getNumCategories(); ++i)
		{
			shared_ptr<CategoryComponent> const category = m_attribute_holder_comp->getCategory (i);
			
			if (category->getName().isNotEmpty())
				s.add (category->getName());
		}
		
		return s;
	}
	
	bool AttributePanel::isCategoryOpen (const int categoryIndex) const
	{
		int index = 0;
		
		for (int i = 0; i < m_attribute_holder_comp->getNumCategories(); ++i)
		{
			shared_ptr<CategoryComponent> const category = m_attribute_holder_comp->getCategory (i);
			
			if (category->getName().isNotEmpty())
			{
				if (index == categoryIndex)
					return category->isOpen();
				
				++index;
			}
		}
		
		return false;
	}
	
	void AttributePanel::setCategoryOpen (const int categoryIndex, const bool shouldBeOpen)
	{
		int index = 0;
		
		for (int i = 0; i < m_attribute_holder_comp->getNumCategories(); ++i)
		{
			shared_ptr<CategoryComponent> const category = m_attribute_holder_comp->getCategory (i);
			
			if (category->getName().isNotEmpty())
			{
				if (index == categoryIndex)
				{
					category->setOpen (shouldBeOpen);
					break;
				}
				
				++index;
			}
		}
	}
	
	void AttributePanel::setCategoryEnabled (const int categoryIndex, const bool shouldBeEnabled)
	{
		int index = 0;
		
		for (int i = 0; i < m_attribute_holder_comp->getNumCategories(); ++i)
		{
			shared_ptr<CategoryComponent> const category = m_attribute_holder_comp->getCategory (i);
			
			if (category->getName().isNotEmpty())
			{
				if (index == categoryIndex)
				{
					category->setEnabled (shouldBeEnabled);
					break;
				}
				
				++index;
			}
		}
	}
}