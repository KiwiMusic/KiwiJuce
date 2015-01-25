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

#include "AttributeColorComponent.h"

namespace Kiwi
{
	AttributeColorComponent::AttributeColorComponent (sAttr attribute, Attr::sManager attrsManager)
	: AttributeComponent (attribute, attrsManager)
	{
		m_color.addListener(this);
	}
	
	AttributeColorComponent::~AttributeColorComponent()
	{
		m_color.removeListener(this);
	}
	
	void AttributeColorComponent::paint (Graphics& g)
	{
		AttributeComponent::paint (g);
		
		const juce::Rectangle<int> rect = getAttributeContentComponentArea();
		
		const Colour colour (getColour());
		
		g.setColour(Colours::lightgrey.contrasting(0.3f).withAlpha(0.8f));
		g.fillRect(rect);
		g.fillCheckerBoard (rect.reduced(2),
							10, 10,
							Colour(0xffcdcdcd).overlaidWith(colour),
							Colour(0xffdedede).overlaidWith(colour));
	}
	
	Colour AttributeColorComponent::getColour() const
	{
		if (m_color.toString().isEmpty())
			return Colour();
		
		return Colour::fromString(m_color.toString());
	}
	
	void AttributeColorComponent::mouseDown(const MouseEvent&)
	{
		;
	}
	
	void AttributeColorComponent::mouseUp(const MouseEvent&)
	{
		if (isEnabled())
		{
			PopupColourSelector* popup = new PopupColourSelector(m_color);
			CallOutBox::launchAsynchronously(popup, getScreenBounds(), nullptr);
		}
	}
	
	void AttributeColorComponent::refresh()
	{
		ElemVector attrval;
		getAttributeValue(attrval);
		
		if (!attrval.empty())
		{
			float red, green, blue, alpha;
			const ElemVector::size_type size = attrval.size();
			
			if(size && attrval[0].isNumber())
			{
				red = clip((double)attrval[0], 0., 1.);
			}
			if(size > 1 && attrval[1].isNumber())
			{
				green = clip((double)attrval[1], 0., 1.);
			}
			if(size > 2 && attrval[2].isNumber())
			{
				blue = clip((double)attrval[2], 0., 1.);
			}
			if(size > 3 && attrval[3].isNumber())
			{
				alpha = clip((double)attrval[3], 0., 1.);
			}
			
			m_color = Colour::fromFloatRGBA(red, green, blue, alpha).toDisplayString(true);
			
			repaint();
		}
	}
	
	void AttributeColorComponent::valueChanged(Value& val)
	{
		if (val == m_color)
		{
			const Colour col = getColour();

			if (col != m_lastcolor)
				setAttributeValue({(double)col.getFloatRed(), (double)col.getFloatGreen(), (double)col.getFloatBlue(), (double)col.getFloatAlpha()});

			m_lastcolor = col;
		}
	}
}