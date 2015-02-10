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

#ifndef __DEF_KIWI_JPAGECONTROLLER__
#define __DEF_KIWI_JPAGECONTROLLER__

#include "jLink.h"

namespace Kiwi
{
	// ================================================================================ //
	//                                 JPAGE CONTROLER                                  //
	// ================================================================================ //
	
	//! The jPage is the juce implementation of the Page::View.
	/**
	 The jPage ...
	 */
	class jPage : public PageView, public Component, public ApplicationCommandTarget, public juce::TextEditor::Listener
	{
    private:
        class jLasso;
        class jIolighter;
        
        wjBox								m_box_edited;
		ScopedPointer<juce::TextEditor>		m_editor;
        vector<sTempLink>					m_templinks;
        
        Knock                               m_knock;
        Magnet                              m_magnet;
        ScopedPointer<jIolighter>           m_iolighter;
        ScopedPointer<jLasso>               m_lasso;
        
        juce::Point<int>	m_last_drag;
		bool				m_copy_on_drag;
		bool				m_box_received_downevent;
        bool m_box_dragstatus, m_link_dragstatus;
		bool m_box_downstatus, m_link_downstatus;
		
		bool m_mouse_wasclicked;
		long m_last_border_downstatus;
		
		sjBox getjBox(int x, int y) noexcept;
		void newBox(int x, int y, bool dblClick = 0);
		
		
		//! Retrieves if there are one or more temporary links.
		inline bool hasTempLinks()
		{
			return !m_templinks.empty();
		}
		
		//! Retrieves a temporary link.
		inline sTempLink getTempLink(const ulong index = 0) const
		{
			if (index < m_templinks.size())
            {
				return m_templinks[index];
            }
			
			return nullptr;
		}
		
		//! Removes any temporary links.
		inline void removeAllTempLink()
		{
			for (int i = 0; i < m_templinks.size(); i++)
			{
				removeChildComponent(m_templinks[i].get());
			}
			
			m_templinks.clear();
		}
		
		void addToSelectionBasedOnModifiers(sBoxView box, bool selOnly);
		void addToSelectionBasedOnModifiers(sLinkView link, bool selOnly);
		bool selectOnMouseDown(sBoxView box, bool selOnly);
		bool selectOnMouseDown(sLinkView link, bool selOnly);
		void selectOnMouseUp(sBoxView box, bool selOnly, const bool boxWasDragged, const bool resultOfMouseDownSelectMethod);
		void selectOnMouseUp(sLinkView link, bool selOnly, const bool boxWasDragged, const bool resultOfMouseDownSelectMethod);
		
		//! Copy selected boxes to clipboard
		/** The function copy boxes to clipboard
		 */
		void copySelectionToClipboard();
		
		//! Attempts to add boxes to the page from clipboard.
		/** The function attempts to add boxes to the page from clipboard.
		 */
		void pasteFromClipboard(Gui::Point const& offset = Gui::Point());
		
		//! Bring all the links in front of boxes.
		/** The function brings all the links in front of boxes.
		 */
		void bringsLinksToFront();
		
		//! Brings all the boxes in front of links.
		/** The function brings all the boxes in front of links.
		 */
		void bringsBoxesToFront();
		
		//! Brings boxes or links to front depending on lock status.
		/** Brings boxes or links to front depending on lock status.
		 */
		void updateBoxesAndLinksLayers();
		
    public:
		
        //! The constructor.
        /** You should never have to use this function.
		 Please use the Box::View
         */
        jPage(sPage page);
        
        //! Destructor.
        /** You should never have to use this function.
         */
        ~jPage();
		
        // ================================================================================ //
        //                                      COMPONENT                                   //
        // ================================================================================ //
        
        void paint(Graphics& g) override;
        void mouseEnter(const MouseEvent& e) override;
		void mouseMove(const MouseEvent& e) override;
        void mouseDown(const MouseEvent& e) override;
		void mouseDrag(const MouseEvent& e) override;
        void mouseUp(const MouseEvent& e) override;
		void mouseExit(const MouseEvent& e) override;
        void mouseDoubleClick(const MouseEvent& e) override;
        void mouseWheelMove(const MouseEvent& e, const MouseWheelDetails& wheel) override;
        bool keyPressed(const KeyPress& key) override;
		
        // ================================================================================ //
        //                              TEXT EDITOR LISTENER                                //
        // ================================================================================ //
        
		void textEditorTextChanged(juce::TextEditor&) override;
        void textEditorReturnKeyPressed(juce::TextEditor&) override;
        void textEditorEscapeKeyPressed(juce::TextEditor&) override;
        void textEditorFocusLost(juce::TextEditor&) override;
        
        // ================================================================================ //
        //                              APPLICATION COMMAND TARGET                          //
        // ================================================================================ //
        
        ApplicationCommandTarget* getNextCommandTarget() override;
        void getAllCommands(Array <CommandID>& commands) override;
        void getCommandInfo(CommandID commandID, ApplicationCommandInfo& result) override;
        bool perform(const InvocationInfo& info) override;
		
		//! Shows box contextual popup menu.
		/** The function shows the box contextual popup menu.
		 @param box The box.
		 */
		void showBoxPopupMenu(sBox box);
        
        // ================================================================================ //
        //                                  PAGE CONTROLLER                                 //
        // ================================================================================ //
		
		//! Receives notification when an attribute value of the page has changed.
		/** The function receives notification when an attribute value of the page has changed.
		 @param attr The attribute.
		 @return pass true to notify changes to listeners, false if you don't want them to be notified
		 */
		void attributeChanged(sPage page, sAttr attr) override;
		
		//! Create a box controller.
		/** Page controller's subclasses must implement this method to create custom box controller.
		 @param box     The box.
		 @return The newly created box controller.
		 */
		virtual sBoxView createBoxView(sBox box) override;
		
		//! Receive the notification that a box controller has been created.
		/** The function is called by the page when a box controller has been created.
		 @param boxctrl The box controller.
		 */
		virtual void boxViewCreated(sBoxView boxctrl) override;
        
		//! Receive the notification that a box controller before a box has been removed.
		/** The function is called by the page controller before a box has been removed.
		 @param boxctrl The box controller.
		 */
        virtual void boxViewWillBeRemoved(sBoxView boxctrl) override;
        
		//! Create a link controller.
		/** Page controller's subclasses must implement this method to create custom link controller.
		 @param link     The link.
		 @return The newly created link controller.
		 */
		virtual sLinkView createLinkView(sLink link) override;
		
		//! Receive the notification that a link controller has been created.
		/** The function is called by the page when a link controller has been created.
		 @param linkctrl The link controller.
		 */
		virtual void linkViewCreated(sLinkView linkctrl) override;
		
		//! Receive the notification that a link controller before a box has been removed.
		/** The function is called by the page controller before a link has been removed.
		 @param linkctrl The link controller.
		 */
		virtual void linkViewWillBeRemoved(sLinkView linkctrl) override;
		
		//! The overriden redraw function.
		/** The function is called when the page needs to be redrawn.
		 */
		void redraw() override;
		
		//! Called when the selection has changed.
		/** The function is called when the selection has changed.
		 */
		void selectionChanged() override;
		
		//! Called when the page has been locked/unlocked.
		/** The function is called when the page has been locked/unlocked.
		 */
		void lockStatusChanged() override;
		
		//! Called when the page presentation mode has been activated/deactivated.
		/** The function is called when the page presentation mode has been activated/deactivated.
		 */
		void presentationStatusChanged() override;
    };

	typedef shared_ptr<jPage>		sjPage;
	typedef shared_ptr<jPage>		scjPage;
	typedef weak_ptr<jPage>		wjPage;
    
    class jPage::jLasso : public PageView::Lasso, public Component
    {
    public:
        jLasso()
        {
            setInterceptsMouseClicks(false, false);
            setWantsKeyboardFocus(false);
            setVisible(false);
        }
        
        ~jLasso()
        {
            ;
        }
        
        void paint(Graphics& g) override
        {
			const Colour color = Colour::fromFloatRGBA(0.96, 0.96, 0.96, 1.);
            g.fillAll(color.withAlpha(0.4f));
            g.setColour(color);
            g.drawRect(getLocalBounds(), 1.);
        }
    };

    class jPage::jIolighter : public Component
    {
    private:
        juce::Colour    m_colour;
    public:
        //! Contructor.
        /** You should never have to use this method.
         */
        jIolighter();
        
        //! Destrcutor.
        /** You should never have to use this method.
         */
        ~jIolighter();
        
        //! The juce paint method.
        /** The function paints in a graphics.
         */
        void paint(Graphics& g) override;
        
        //! The juce hit test method.
        /** The function test if the component has been hitted.
         */
        bool hitTest(int x, int y) override
        {
            return false;
        }
        
        //! Defines an inlet to be highlighted.
        /** The function defines an inlet to be highlighted.
         @param box The box that owns the inlet.
         @param index The index of the inlet.
         */
        void setInlet(sBoxView box, ulong index);
        
        //! Defines an outlet to be highlighted.
        /** The function defines an outlet to be highlighted.
         @param box The box that owns the outlet.
         @param index The index of the outlet.
         */
        void setOutlet(sBoxView box, ulong index);
    };
}



#endif
