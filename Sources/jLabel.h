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

#ifndef __DEF_KIWI_JLABEL__
#define __DEF_KIWI_JLABEL__

#include "jTextEditor.h"

namespace Kiwi
{
	//==============================================================================
	/**
	 The jLabel is a modified version of the juce::Label class :
	 A component that displays a text string, and can optionally become a text
	 editor when clicked.
	 */
	class jLabel :	public Component,
					public Kiwi::Writer::TextField::View,
					public jTextEditor::Listener,
					private ValueListener,
					public enable_shared_from_this<jLabel>
	{
	public:
		//==============================================================================
		/** Creates a jLabel.
		 @param textfield		The textfield
		 @param componentName   The name to give the component
		 @param labelText       The text to show in the label
		 */
		jLabel(Kiwi::Writer::sTextField textfield, const String& componentName = String::empty, const String& labelText = String::empty);
		
		/** Destructor. */
		~jLabel();
		
		//==============================================================================
		/** Changes the label text.
		 
		 The NotificationType parameter indicates whether to send a change message to
		 any jLabel::Listener objects if the new text is different.
		 */
		void setText(const String& newText,
					  NotificationType notification);
		
		/** Returns the label's current text.
		 
		 @param returnActiveEditorContents   if this is true and the label is currently
		 being edited, then this method will return the
		 text as it's being shown in the editor. If false,
		 then the value returned here won't be updated until
		 the user has finished typing and pressed the return
		 key.
		 */
		String getText(bool returnActiveEditorContents = false) const;
		
		/** Returns the text content as a Value object.
		 You can call Value::referTo() on this object to make the label read and control
		 a Value object that you supply.
		 */
		Value& getTextValue() noexcept                          { return textValue; }
		
		//! Receives the notification that the textfield's text changed.
		/** The function receives the notification that the textfield's text changed.
		 */
		void textfieldTextChanged() override;
		
		//! Receives the notification that the textfield's text has been edited.
		/** The function receives the notification that the textfield's text has been edited.
		 */
		void textfieldTextEdited() override;
		
		//==============================================================================
		/** Changes the font to use to draw the text.
		 @see getFont
		 */
		void setFont(const juce::Font& newFont);
		
		/** Returns the font currently being used.
		 This may be the one set by setFont(), unless it has been overridden by the current LookAndFeel
		 @see setFont
		 */
		juce::Font getFont() const noexcept;
		
		//==============================================================================
		/** A set of colour IDs to use to change the colour of various aspects of the label.
		 
		 These constants can be used either via the Component::setColour(), or LookAndFeel::setColour()
		 methods.
		 
		 Note that you can also use the constants from jTextEditor::ColourIds to change the
		 colour of the text editor that is opened when a label is editable.
		 
		 @see Component::setColour, Component::findColour, LookAndFeel::setColour, LookAndFeel::findColour
		 */
		enum ColourIds
		{
			backgroundColourId             = 0x1000280, /**< The background colour to fill the label with. */
			textColourId                   = 0x1000281, /**< The colour for the text. */
			outlineColourId                = 0x1000282, /**< An optional colour to use to draw a border around the label.
														 Leave this transparent to not have an outline. */
			backgroundWhenEditingColourId  = 0x1000283, /**< The background colour when the label is being edited. */
			textWhenEditingColourId        = 0x1000284, /**< The colour for the text when the label is being edited. */
			outlineWhenEditingColourId     = 0x1000285  /**< An optional border colour when the label is being edited. */
		};
		
		//==============================================================================
		/** Sets the style of justification to be used for positioning the text.
		(The default is Justification::centredLeft)
		 */
		void setJustificationType(Justification justification);
		
		/** Returns the type of justification, as set in setJustificationType(). */
		Justification getJustificationType() const noexcept                         { return justification; }
		
		/** Changes the border that is left between the edge of the component and the text.
		 By default there's a small gap left at the sides of the component to allow for
		 the drawing of the border, but you can change this if necessary.
		 */
		void setBorderSize(BorderSize<int> newBorderSize);
		
		/** Returns the size of the border to be left around the text. */
		BorderSize<int> getBorderSize() const noexcept                              { return border; }
		
		/** Set a keyboard type for use when the text editor is shown. */
		void setKeyboardType(TextInputTarget::VirtualKeyboardType type) noexcept   { keyboardType = type; }
		
		//==============================================================================
		/**
		 A class for receiving events from a jLabel.
		 
		 You can register a jLabel::Listener with a jLabel using the jLabel::addListener()
		 method, and it will be called when the text of the label changes, either because
		 of a call to jLabel::setText() or by the user editing the text(if the label is
		 editable).
		 
		 @see jLabel::addListener, jLabel::removeListener
		 */
		class JUCE_API  Listener
		{
		public:
			/** Destructor. */
			virtual ~Listener() {}
			
			/** Called when a jLabel's text has changed. */
			virtual void labelTextChanged(jLabel* labelThatHasChanged) = 0;
			
			/** Called when a jLabel goes into editing mode and displays a jTextEditor. */
			virtual void editorShown(jLabel* label, sjTextEditor editor) {}
		};
		
		/** Registers a listener that will be called when the label's text changes. */
		void addListener(Listener* listener);
		
		/** Deregisters a previously-registered listener. */
		void removeListener(Listener* listener);
		
		//==============================================================================
		/** Makes the label turn into a jTextEditor when clicked.
		 
		 By default this is turned off.
		 
		 If turned on, then single- or double-clicking will turn the label into
		 an editor. If the user then changes the text, then the ChangeBroadcaster
		 base class will be used to send change messages to any listeners that
		 have registered.
		 
		 If the user changes the text, the textWasEdited() method will be called
		 afterwards, and subclasses can override this if they need to do anything
		 special.
		 
		 @param editOnSingleClick            if true, just clicking once on the label will start editing the text
		 @param editOnDoubleClick            if true, a double-click is needed to start editing
		 @param lossOfFocusDiscardsChanges   if true, clicking somewhere else while the text is being
		 edited will discard any changes; if false, then this will
		 commit the changes.
		 @see showEditor, setEditorColours, jTextEditor
		 */
		void setEditable(bool editOnSingleClick,
						 bool editOnDoubleClick = false,
						 bool lossOfFocusDiscardsChanges = false);
		
		/** Returns true if this option was set using setEditable(). */
		bool isEditableOnSingleClick() const noexcept                       { return editSingleClick; }
		
		/** Returns true if this option was set using setEditable(). */
		bool isEditableOnDoubleClick() const noexcept                       { return editDoubleClick; }
		
		/** Returns true if this option has been set in a call to setEditable(). */
		bool doesLossOfFocusDiscardChanges() const noexcept                 { return lossOfFocusDiscardsChanges; }
		
		/** Returns true if the user can edit this label's text. */
		bool isEditable() const noexcept                                    { return editSingleClick || editDoubleClick; }
		
		/** Makes the editor appear as if the label had been clicked by the user.
		 @see textWasEdited, setEditable
		 */
		void showEditor();
		
		/** Hides the editor if it was being shown.
		 
		 @param discardCurrentEditorContents     if true, the label's text will be
		 reset to whatever it was before the editor
		 was shown; if false, the current contents of the
		 editor will be used to set the label's text
		 before it is hidden.
		 */
		void hideEditor(bool discardCurrentEditorContents);
		
		/** Returns true if the editor is currently focused and active. */
		bool isBeingEdited() const noexcept;
		
		/** Returns the currently-visible text editor, or nullptr if none is open. */
		sjTextEditor getCurrentTextEditor() const noexcept;
		
	protected:
		//==============================================================================
		/** Creates the jTextEditor component that will be used when the user has clicked on the label.
		 Subclasses can override this if they need to customise this component in some way.
		 */
		virtual sjTextEditor createEditorComponent();
		
		/** Called after the user changes the text. */
		virtual void textWasEdited();
		
		/** Called when the text has been altered. */
		virtual void textWasChanged();
		
		/** Called when the text editor has just appeared, due to a user click or other focus change. */
		virtual void editorShown(sjTextEditor editor);
		
		/** Called when the text editor is going to be deleted, after editing has finished. */
		virtual void editorAboutToBeHidden(sjTextEditor editor);
		
		//==============================================================================
		/** @internal */
		void paint(Graphics&) override;
		/** @internal */
		void resized() override;
		/** @internal */
		void mouseUp(const MouseEvent&) override;
		/** @internal */
		void mouseDoubleClick(const MouseEvent&) override;
		/** @internal */
		void inputAttemptWhenModal() override;
		/** @internal */
		void focusGained(FocusChangeType) override;
		/** @internal */
		void enablementChanged() override;
		/** @internal */
		KeyboardFocusTraverser* createFocusTraverser() override;
		/** @internal */
		void textEditorTextChanged(sjTextEditor editor) override;
		/** @internal */
		void textEditorReturnKeyPressed(sjTextEditor editor) override;
		/** @internal */
		void textEditorEscapeKeyPressed(sjTextEditor editor) override;
		/** @internal */
		void textEditorFocusLost(sjTextEditor editor) override;
		/** @internal */
		void colourChanged() override;
		/** @internal */
		void valueChanged(Value&) override;
		/** @internal */
		void callChangeListeners();
		
	private:
		//==============================================================================
		Value textValue;
		String lastTextValue;
		juce::Font font;
		Justification justification;
		sjTextEditor m_editor;
		ListenerList<Listener> listeners;
		BorderSize<int> border;
		TextInputTarget::VirtualKeyboardType keyboardType;
		bool editSingleClick;
		bool editDoubleClick;
		bool lossOfFocusDiscardsChanges;
		
		bool updateFromTextEditorContents(sjTextEditor editor);
		
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(jLabel)
	};
	
	typedef shared_ptr<jLabel>		 sjLabel;
	typedef weak_ptr<jLabel>         wjLabel;
	typedef shared_ptr<const jLabel> scjLabel;
	typedef weak_ptr<const jLabel>   wcjLabel;
}
#endif
