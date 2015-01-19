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

#include "AttributeTextComponent.h"

namespace Kiwi
{
	class AttributeTextComponent::LabelComp  : public Label
	{
	public:
		LabelComp (AttributeTextComponent& tpc, const int charLimit, const bool multiline)
		: Label (String::empty, String::empty),
		owner (tpc),
		maxChars (charLimit),
		isMultiline (multiline)
		{
			setEditable (true, true, false);
			
			setColour (backgroundColourId, owner.findColour (AttributeTextComponent::backgroundColourId));
			setColour (outlineColourId,    owner.findColour (AttributeTextComponent::outlineColourId));
			setColour (textColourId,       owner.findColour (AttributeTextComponent::textColourId));
		}
		
		juce::TextEditor* createEditorComponent() override
		{
			juce::TextEditor* const ed = Label::createEditorComponent();
			ed->setInputRestrictions (maxChars);
			
			if (isMultiline)
			{
				ed->setMultiLine (true, true);
				ed->setReturnKeyStartsNewLine (true);
			}
			
			return ed;
		}
		
		void textWasEdited() override
		{
			owner.textWasEdited();
		}
		
	private:
		AttributeTextComponent& owner;
		int maxChars;
		bool isMultiline;
	};
	
	//==============================================================================
	AttributeTextComponent::AttributeTextComponent (sAttr attribute, Attr::sManager attrsManager,
													const int maxNumChars, const bool isMultiLine)
	: AttributeComponent (attribute, attrsManager)
	{
		createEditor (maxNumChars, isMultiLine);
	}
	
	AttributeTextComponent::~AttributeTextComponent()
	{
	}
	
	void AttributeTextComponent::setText (const String& newText)
	{
		textEditor->setText (newText, sendNotificationSync);
	}
	
	String AttributeTextComponent::getText() const
	{
		return textEditor->getText();
	}
	
	void AttributeTextComponent::createEditor (const int maxNumChars, const bool isMultiLine)
	{
		addAndMakeVisible (textEditor = new LabelComp (*this, maxNumChars, isMultiLine));
		
		if (isMultiLine)
		{
			textEditor->setJustificationType (Justification::topLeft);
			m_preferred_height = 100;
		}
		
		enablementChanged();
	}
	
	void AttributeTextComponent::refresh()
	{
		ElemVector value;
		getAttributeValue(value);
		
		if (!value.empty())
		{
			textEditor->setText(toString(value), dontSendNotification);
		}
	}
	
	void AttributeTextComponent::enablementChanged()
	{
		textEditor->setEditable(false, isEnabled());
		textEditor->setEnabled(isEnabled());
		repaint();
	}
	
	void fromText(ElemVector& elements, string const& text)
	{
		string word;
		istringstream iss(text);
		while(iss >> word)
		{
			if(isdigit(word[0]))
			{
				if(word.find('.') != string::npos)
				{
					elements.push_back(atof(word.c_str()));
				}
				else
				{
					elements.push_back(atol(word.c_str()));
				}
			}
			else
			{
				elements.push_back(Tag::create(word));
			}
		}
	}
	
	void AttributeTextComponent::textWasEdited()
	{
		ElemVector elements;
		fromText(elements, textEditor->getText().toStdString());
		
		setAttributeValue(elements);
	}
}