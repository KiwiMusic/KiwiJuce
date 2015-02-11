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

#include "AttributeView.h"
#include "AttributeBoolComponent.h"
#include "AttributeNumberComponent.h"
#include "AttributeTextComponent.h"
#include "AttributeMenuComponent.h"
#include "AttributeColorComponent.h"

namespace Kiwi
{
	// ================================================================================ //
	//								ATTRIBUTE PANEL COMPONENT                           //
	// ================================================================================ //
	
	AttributeViewer::AttributeViewer()
	{
		//rebuildProperties();
	}
	
	AttributeViewer::~AttributeViewer()
	{
		cout << "Panel delete" << endl;
		//setAttributeManagerToWatch(nullptr); // is it safe to note unbind the listener
	}
	
	void AttributeViewer::setAttributeManagerToWatch(Attr::sManager attrsManager)
	{
		if (m_attr_manager != attrsManager)
		{
			if (m_attr_manager)
            {
                m_attr_manager->unbind(shared_from_this());
            }
			
			m_attr_manager = attrsManager;
			
			if (m_attr_manager)
			{
				m_attr_manager->bind(shared_from_this());
				rebuildProperties();
			}
		}
	}
	
	void AttributeViewer::notify(shared_ptr<Attr::Manager> manager, sAttr attr, Attr::Notification type)
	{
		if (manager == m_attr_manager)
		{
			if (type == Attr::Notification::ValueChanged)
			{
				cout << "attrViewer : attr changed" << endl;
				refreshAttr(attr);
			}
		}
	}
	
	void AttributeViewer::resized()
	{
		AttributePanel::resized();
	}
	
	void AttributeViewer::rebuildProperties()
	{
		vector<sTag>  categories;
		vector<sAttr> attrs;
		sTag  categoryName;
		sAttr attr;
		Attr::Style style;
		vector<shared_ptr<AttributeComponent>> attrsComps;
		
		clear();
		
		m_attr_manager->getCategoriesNames(categories, true);
		
		for (int i = 0; i < categories.size(); i++)
		{
			attrsComps.clear();
			
			categoryName = categories[i];
			m_attr_manager->getAttributesInCategory(categoryName, attrs, true);
			
			if (!attrs.empty())
			{
				for (int j = 0; j < attrs.size(); j++)
				{
					attr = attrs[j];
					style = attr->getStyle();
					
					switch (style)
					{
						case Attr::Style::Toggle:
						{
							attrsComps.push_back(createBoolComponent(m_attr_manager, attr));
							break;
						}
						case Attr::Style::NumberLong:
						case Attr::Style::NumberDouble:
						{
							attrsComps.push_back(createNumberComponent(m_attr_manager, attr));
							break;
						}
						case Attr::Style::Text:
						{
							attrsComps.push_back(createTextComponent(m_attr_manager, attr));
							break;
						}
						case Attr::Style::List:
						{
							attrsComps.push_back(createTextComponent(m_attr_manager, attr));
							break;
						}
						case Attr::Style::Enum:
						{
							attrsComps.push_back(createMenuComponent(m_attr_manager, attr));
							break;
						}
						case Attr::Style::Color:
						{
							attrsComps.push_back(createColorComponent(m_attr_manager, attr));
							break;
						}
						default:
							break;
					}
				}
				
				addCategory(categoryName->getName(), attrsComps, true);
			}
		}
		
		attrsComps.clear();
	}
	
	shared_ptr<AttributeComponent> AttributeViewer::createBoolComponent(Attr::sManager manager, sAttr attr)
	{
		return AttributeComponent::create<AttributeBoolComponent>(attr, m_attr_manager);
	}
	
	shared_ptr<AttributeComponent> AttributeViewer::createNumberComponent(Attr::sManager manager, sAttr attr)
	{
		return AttributeComponent::create<AttributeNumberComponent>(attr, m_attr_manager, 0., 100., 0.1);
	}
	
	shared_ptr<AttributeComponent> AttributeViewer::createTextComponent(Attr::sManager manager, sAttr attr)
	{
		return AttributeComponent::create<AttributeTextComponent>(attr, m_attr_manager, 3000, false);
	}
	
	shared_ptr<AttributeComponent> AttributeViewer::createMenuComponent(Attr::sManager manager, sAttr attr)
	{
		return AttributeComponent::create<AttributeMenuComponent>(attr, m_attr_manager);
	}
	
	shared_ptr<AttributeComponent> AttributeViewer::createColorComponent(Attr::sManager manager, sAttr attr)
	{
		return AttributeComponent::create<AttributeColorComponent>(attr, m_attr_manager);
	}
	
	// ================================================================================ //
	//                                  INSPECTOR WINDOW                                //
	// ================================================================================ //
	
    InspectorWindow::InspectorWindow(sAttributeViewer attrsViewer) :
    BaseWindow("Inspector", Colours::lightgrey, minimiseButton | closeButton, true),
	m_attr_viewer(attrsViewer)
    {
        setResizable(true, false);
        setResizeLimits(50, 50, 32000, 32000);
        setTopLeftPosition(0, 0);
        setSize(300, 440);
        setVisible(true);
		
		if (m_attr_viewer)
		{
			m_attr_viewer->setBounds(getLocalBounds());
			setContentNonOwned(m_attr_viewer.get(), true);
		}
    }
    
    InspectorWindow::~InspectorWindow()
    {
		m_attr_viewer.reset();
    }
	
	void InspectorWindow::setPage(sPage page)
	{
		if (page)
		{
			if (!m_attr_viewer || (m_attr_viewer->getViewedManager() != page))
			{
				shared_ptr<AttributeViewer> viewer = AttributeViewer::create(page);
				
				if (viewer)
				{
					m_attr_viewer = viewer;
					m_attr_viewer->setBounds(getLocalBounds());
					setContentNonOwned(m_attr_viewer.get(), true);
					setName("Page inspector");
				}
			}
		}
	}
	
	void InspectorWindow::setObject(sObject object)
	{
		if (object)
		{
			if (!m_attr_viewer || (m_attr_viewer->getViewedManager() != object))
			{
				shared_ptr<AttributeViewer> viewer = AttributeViewer::create(object);
				
				if (viewer)
				{
					m_attr_viewer = viewer;
					m_attr_viewer->setBounds(getLocalBounds());
					setContentNonOwned(m_attr_viewer.get(), true);
					string name = toString(object->getName());
					if (name.empty())
						name = "Object";
					
					setName(name + " inspector");
				}
			}
		}
		else
		{
			m_attr_viewer = sAttributeViewer();
			setContentNonOwned(nullptr, true);
			setName("Object inspector");
		}
	}
    
    void InspectorWindow::closeButtonPressed()
    {
        setVisible(false);
    }
}