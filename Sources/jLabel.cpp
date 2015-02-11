/*
 ==============================================================================
 
 This file is part of the KIWI library.
 Copyright(c) 2014 Pierre Guillot & Eliott Paris.
 
 Permission is granted to use this software under the terms of either:
 a) the GPL v2(or any later version)
 b) the Affero GPL v3
 
 Details of these licenses can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 To release a closed-source product which uses KIWI, contact : guillotpierre6@gmail.com
 
 ==============================================================================
 */

#include "jLabel.h"

namespace Kiwi
{
	jLabel::jLabel(Gui::Writer::sTextField textfield, const String& name, const String& labelText)
	: Component(name),
	Gui::Writer::TextField::View(textfield),
	textValue(labelText),
	lastTextValue(labelText),
	font(15.0f),
	justification(Justification::centredLeft),
	border(1, 5, 1, 5),
	keyboardType(jTextEditor::textKeyboard),
	editSingleClick(false),
	editDoubleClick(false),
	lossOfFocusDiscardsChanges(false)
	{
		setColour(jTextEditor::textColourId, Colours::black);
		setColour(jTextEditor::backgroundColourId, Colours::transparentBlack);
		setColour(jTextEditor::outlineColourId, Colours::transparentBlack);
		setColour(Label::ColourIds::backgroundWhenEditingColourId, Colours::transparentWhite);
		
		setText(textfield->getText().c_str(), dontSendNotification);
		
		juce::Font font(13);
		font.setTypefaceName("Menelo");
		setFont(font);

		setJustificationType(Justification::topLeft);
		setBorderSize(BorderSize<int>(3, 4, 3, 4));
		setEditable(false, true);
		
		textValue.addListener(this);
	}
	
	jLabel::~jLabel()
	{
		textValue.removeListener(this);
	}
	
	//==============================================================================
	void jLabel::setText(const String& newText,
						  const NotificationType notification)
	{
		hideEditor(true);
		
		if(lastTextValue != newText)
		{
			lastTextValue = newText;
			textValue = newText;
			repaint();
			
			textWasChanged();
			
			if(notification != dontSendNotification)
				callChangeListeners();
		}
	}
	
	String jLabel::getText(const bool returnActiveEditorContents) const
	{
		return(returnActiveEditorContents && isBeingEdited())
		? m_editor->getText()
		: textValue.toString();
	}
	
	void jLabel::valueChanged(Value&)
	{
		if(lastTextValue != textValue.toString())
			setText(textValue.toString(), sendNotification);
	}
	
	void jLabel::textfieldTextChanged()
	{
		Gui::Writer::sTextField textfield = getTextField();
		
		if(textfield)
		{
			setText(textfield->getText().c_str(), dontSendNotification);
		}
	}
	
	void jLabel::textfieldTextEdited()
	{
		if (!isBeingEdited())
		{
			Gui::Writer::sTextField textfield = getTextField();
			
			if(textfield)
			{
				setText(textfield->getEditedText().c_str(), dontSendNotification);
			}
		}
	}
	
	//==============================================================================
	void jLabel::setFont(const juce::Font& newFont)
	{
		if(font != newFont)
		{
			font = newFont;
			repaint();
		}
	}
	
	juce::Font jLabel::getFont() const noexcept
	{
		return font;
	}
	
	void jLabel::setEditable(const bool editOnSingleClick,
							  const bool editOnDoubleClick,
							  const bool lossOfFocusDiscardsChanges_)
	{
		editSingleClick = editOnSingleClick;
		editDoubleClick = editOnDoubleClick;
		lossOfFocusDiscardsChanges = lossOfFocusDiscardsChanges_;
		
		setWantsKeyboardFocus(editOnSingleClick || editOnDoubleClick);
		setFocusContainer(editOnSingleClick || editOnDoubleClick);
	}
	
	void jLabel::setJustificationType(Justification newJustification)
	{
		if(justification != newJustification)
		{
			justification = newJustification;
			repaint();
		}
	}
	
	void jLabel::setBorderSize(BorderSize<int> newBorder)
	{
		if(border != newBorder)
		{
			border = newBorder;
			repaint();
		}
	}
	
	//==============================================================================
	void jLabel::textWasEdited() {}
	void jLabel::textWasChanged() {}
	
	void jLabel::editorShown(sjTextEditor editor)
	{
		Component::BailOutChecker checker(this);
		listeners.callChecked(checker, &jLabel::Listener::editorShown, this, editor);
	}
	
	void jLabel::editorAboutToBeHidden(sjTextEditor editor)
	{
		if(ComponentPeer* const peer = getPeer())
			peer->dismissPendingTextInput();
	}
	
	void jLabel::showEditor()
	{
		if(m_editor == nullptr)
		{
			m_editor = createEditorComponent();
		}
			
		if(m_editor)
		{
			addAndMakeVisible(m_editor.get());
			m_editor->setText(getText(), false);
			m_editor->setKeyboardType(keyboardType);
			m_editor->addListener(this);
			m_editor->grabKeyboardFocus();
			
			if(m_editor.get() == nullptr) // may be deleted by a callback
				return;
			
			// if selectall when focus :
			m_editor->setHighlightedRegion(Range<int>(0, textValue.toString().length()));
			
			resized();
			repaint();
			
			editorShown(m_editor);
			
			enterModalState(false);
			m_editor->grabKeyboardFocus();
			
			Gui::Writer::sTextField textfield = getTextField();
			if (textfield)
			{
				textfield->startEditing();
			}
		}
	}
	
	bool jLabel::updateFromTextEditorContents(sjTextEditor editor)
	{
		if (editor)
		{
			const String newText(editor->getText());
			
			if(textValue.toString() != newText)
			{
				lastTextValue = newText;
				textValue = newText;
				repaint();
				
				textWasChanged();
				
				return true;
			}
		}
		
		return false;
	}
	
	void jLabel::hideEditor(const bool discardCurrentEditorContents)
	{
		if(m_editor != nullptr)
		{
			WeakReference<Component> deletionChecker(this);
			
			editorAboutToBeHidden(m_editor);
			
			const bool changed = (!discardCurrentEditorContents) && updateFromTextEditorContents(m_editor);
			
			m_editor.reset();
			
			repaint();
			
			if(changed)
				textWasEdited();
			
			if(deletionChecker != nullptr)
				exitModalState(0);
			
			if(changed && deletionChecker != nullptr)
				callChangeListeners();
			
			Gui::Writer::sTextField textfield = getTextField();
			if (textfield)
			{
				textfield->endEditing();
			}
		}
	}
	
	void jLabel::inputAttemptWhenModal()
	{
		if(m_editor)
		{
			if(lossOfFocusDiscardsChanges)
				textEditorEscapeKeyPressed(m_editor);
			else
				textEditorReturnKeyPressed(m_editor);
		}
	}
	
	bool jLabel::isBeingEdited() const noexcept
	{
		return m_editor != nullptr;
	}
	
	static void copyColourIfSpecified(jLabel& l, jTextEditor& ed, int colourID, int targetColourID)
	{
		if(l.isColourSpecified(colourID) || l.getLookAndFeel().isColourSpecified(colourID))
			ed.setColour(targetColourID, l.findColour(colourID));
	}
	
	sjTextEditor jLabel::createEditorComponent()
	{
		Gui::sWriter writer = getWriter();
		
		if (writer)
		{
			sjTextEditor ed = make_shared<jTextEditor>(writer, getName());
			
			ed->applyFontToAllText(getFont());
			copyAllExplicitColoursTo(*ed);
			
			copyColourIfSpecified(*this, *ed, textWhenEditingColourId, jTextEditor::textColourId);
			copyColourIfSpecified(*this, *ed, backgroundWhenEditingColourId, jTextEditor::backgroundColourId);
			copyColourIfSpecified(*this, *ed, outlineWhenEditingColourId, jTextEditor::outlineColourId);
			
			ed->setBorder(BorderSize<int>(0, 0, 2, 0));
			
			double isMultiline = false;
			ed->setMultiLine(true, true);
			ed->setReturnKeyStartsNewLine(isMultiline);
			
			return ed;
		}
		
		return sjTextEditor();
	}
	
	sjTextEditor jLabel::getCurrentTextEditor() const noexcept
	{
		return m_editor;
	}
	
	//==============================================================================
	void jLabel::paint(Graphics& g)
	{
		g.fillAll(findColour(Label::backgroundColourId));
		
		if(!isBeingEdited())
		{
			const float alpha = isEnabled() ? 1.0f : 0.5f;
			const juce::Font font(getFont());
			
			g.setColour(findColour(Label::textColourId).withMultipliedAlpha(alpha));
			g.setFont(font);
			
			juce::Rectangle<int> textArea(getBorderSize().subtractedFrom(getLocalBounds()));
			
			const bool isMultiline = true;
			
			if (isMultiline)
			{
				g.drawMultiLineText(getText(), textArea.getX(), font.getAscent() * 1.5, textArea.getWidth());
			}
			else
			{
				const bool useEllipsis = true;
				g.drawText(getText(), textArea, juce::Justification::left, useEllipsis);
				
				/*
				g.drawFittedText(getText(), textArea, getJustificationType(),
								 jmax(1,(int)(textArea.getHeight() / font.getHeight())), 1);
				*/
				
			}
			
			g.setColour(findColour(Label::outlineColourId).withMultipliedAlpha(alpha));
		}
		else if(isEnabled())
		{
			g.setColour(findColour(Label::outlineColourId));
		}
		
		g.drawRect(getLocalBounds());
	}
	
	void jLabel::mouseUp(const MouseEvent& e)
	{
		if(editSingleClick
			&& isEnabled()
			&& e.mouseWasClicked()
			&& contains(e.getPosition())
			&& ! e.mods.isPopupMenu())
		{
			showEditor();
		}
	}
	
	void jLabel::mouseDoubleClick(const MouseEvent& e)
	{
		if(editDoubleClick
		   && isEnabled()
		   && ! e.mods.isPopupMenu())
			showEditor();
	}
	
	void jLabel::resized()
	{
		if(m_editor != nullptr)
			m_editor->setBounds(getLocalBounds());
	}
	
	void jLabel::focusGained(FocusChangeType cause)
	{
		if(editSingleClick
		   && isEnabled()
		   && cause == focusChangedByTabKey)
			showEditor();
	}
	
	void jLabel::enablementChanged()
	{
		repaint();
	}
	
	void jLabel::colourChanged()
	{
		repaint();
	}
	
	//==============================================================================
	// We'll use a custom focus traverser here to make sure focus goes from the
	// text editor to another component rather than back to the label itself.
	class jLabelKeyboardFocusTraverser   : public KeyboardFocusTraverser
	{
	public:
		jLabelKeyboardFocusTraverser() {}
		
		Component* getNextComponent(Component* c)     { return KeyboardFocusTraverser::getNextComponent(getComp(c)); }
		Component* getPreviousComponent(Component* c) { return KeyboardFocusTraverser::getPreviousComponent(getComp(c)); }
		
		static Component* getComp(Component* current)
		{
			return dynamic_cast <jTextEditor*>(current) != nullptr
			? current->getParentComponent() : current;
		}
	};
	
	KeyboardFocusTraverser* jLabel::createFocusTraverser()
	{
		return new jLabelKeyboardFocusTraverser();
	}
	
	//==============================================================================
	void jLabel::addListener(jLabel::Listener* const listener)
	{
		listeners.add(listener);
	}
	
	void jLabel::removeListener(jLabel::Listener* const listener)
	{
		listeners.remove(listener);
	}
	
	void jLabel::callChangeListeners()
	{
		Component::BailOutChecker checker(this);
		listeners.callChecked(checker, &jLabel::Listener::labelTextChanged, this);  //(can't use jLabel::Listener due to idiotic VC2005 bug)
	}
	
	//==============================================================================
	void jLabel::textEditorTextChanged(sjTextEditor editor)
	{
		Console::post("textEditorTextChanged  ...");
		if(m_editor)
		{
			if(!(hasKeyboardFocus(true) || isCurrentlyBlockedByAnotherModalComponent()))
			{
				if(lossOfFocusDiscardsChanges)
					textEditorEscapeKeyPressed(editor);
				else
					textEditorReturnKeyPressed(editor);
			}
			else
			{
				Gui::Writer::sTextField textfield = getTextField();
				if (textfield)
				{
					textfield->setEditedText(getText(true).toWideCharPointer());
				}
			}
		}
	}
	
	void jLabel::textEditorReturnKeyPressed(sjTextEditor editor)
	{
		Console::post("textEditorReturnKeyPressed");
		if(m_editor)
		{
			const bool changed = updateFromTextEditorContents(editor);
			hideEditor(true);
			
			if(changed)
			{
				WeakReference<Component> deletionChecker(this);
				textWasEdited();
				
				if(deletionChecker != nullptr)
					callChangeListeners();
			}
		}
	}
	
	void jLabel::textEditorEscapeKeyPressed(sjTextEditor editor)
	{
		Console::post("textEditorEscapeKeyPressed");
		if(m_editor)
		{
			m_editor->setText(textValue.toString(), false);
			hideEditor(true);
		}
	}
	
	void jLabel::textEditorFocusLost(sjTextEditor editor)
	{
		Console::post("textEditorFocusLost");
		textEditorTextChanged(editor);
	}
}