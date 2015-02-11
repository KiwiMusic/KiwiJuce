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

#include "AttributeComponent.h"

namespace Kiwi
{
	AttributeComponent::AttributeComponent(sAttr attribute, Attr::sManager attrsManager, int preferredHeight)
		: m_preferred_height(preferredHeight),
		  m_attr(attribute),
		  m_manager(attrsManager)
	{
		setEnabled(!m_attr->isDisabled());
	}
	
	AttributeComponent::~AttributeComponent()
	{
		if (m_manager && m_attr)
		{
			;//m_manager->unbind(shared_from_this());
		}
	}
	
	void AttributeComponent::getAttributeValue(ElemVector& elements)
	{
		/*
		if (m_attr)
		{
			m_attr->get(elements);
		}
		*/
	}
	
	void AttributeComponent::setAttributeValue(ElemVector const& elements)
	{
		/*
		if (m_manager && m_attr)
		{
			m_manager->setAttributeValue(m_attr->getName(), elements);
		}
		*/
	}
	
	const String AttributeComponent::getAttrName() const noexcept
	{
		/*
		if (m_attr)
		{
			return m_attr->getName()->getName();
		}
		*/
		return String::empty;
	}
	
	const String AttributeComponent::getAttrLabel() const noexcept
	{
		/*
		if (m_attr)
		{
			return m_attr->getLabel()->getName();
		}
		*/
		return String::empty;
	}
	
	void AttributeComponent::paint(Graphics& g)
	{
		//@see juce::drawPropertyComponentBackground
		g.setColour (findColour (backgroundColourId));
		g.fillRect (0, 0, getWidth(), getHeight() - 1);
		
		//@see juce::drawPropertyComponentLabel
		g.setColour (findColour (textColourId).withMultipliedAlpha (isEnabled() ? 1.0f : 0.6f));
		
		g.setFont (jmin (getHeight(), 24) * 0.65f);
		
		//@see juce::getPropertyComponentContentPosition
		const int textW = jmin (200, getWidth() / 3);
		const juce::Rectangle<int> r (textW, 1, getWidth() - textW - 1, getHeight() - 3);
		
		String name = getAttrLabel();
		if (name.isNotEmpty())
		{
			g.drawFittedText (name,
							  3, r.getY(), r.getX() - 5, r.getHeight(),
							  Justification::centredLeft, 2);
		}
	}
	
	void AttributeComponent::paintOverChildren(Graphics& g)
	{
		if (!isEnabled())
		{
			g.setColour(Colours::black.withAlpha(0.1f));
			g.fillRect(getLocalBounds());
		}
	}
	
	void AttributeComponent::notify(sAttr attr)
	{
		if(attr == m_attr)
		{
			refresh();
			/*
			if (type == Attr::Notification::ValueChanged)
			{
				refresh();
			}
			else if(type == Attr::Notification::BehaviorChanged)
			{
				setEnabled(!m_attr->isDisabled());
			}
			*/
		}
	}
	
	void AttributeComponent::resized()
	{
		if (Component* const c = getChildComponent(0))
		{
			c->setBounds(getAttributeContentComponentArea());
		}
	}
	
	juce::Rectangle<int> AttributeComponent::getAttributeContentComponentArea()
	{
		//@see juce::getPropertyComponentContentPosition
		const int textW = jmin (200, getWidth() / 3);
		return juce::Rectangle<int>(textW, 1, getWidth() - textW - 1, getHeight() - 3);
	}
	
	void AttributeComponent::enablementChanged()
	{
		repaint();
	}
}