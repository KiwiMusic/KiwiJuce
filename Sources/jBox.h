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

#ifndef __DEF_KIWI_JBOX_CONTROLLER__
#define __DEF_KIWI_JBOX_CONTROLLER__

#include "jDoodle.h"
#include "jEvent.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                  JBOX CONTROLER                                  //
    // ================================================================================ //
    
    //! The jBox is the juce implementation of the box::controller.
    /**
     The jBox overrides the redraw and paint method and wraps the mouse and keyboard events.
     */
    class jBox  : public BoxView, public Component
    {
	private:
		void checkVisibilityAndInteractionMode();
    public:
        
        //! The constructor.
        /** You should never have to use this function.
         */
        jBox(sBox box, sPageView pageview);
        
        //! Destructor.
        /** You should never have to use this function.
         */
        ~jBox();
        
        //! The redraw function that should be override.
        /** The function is called by the box when it should be repainted.
         @param box    The box.
         */
        void redraw() override;
        
        //! The grab focus function that should be override.
        /** The function is called by the box when it want to grab keyboard focus.
         */
        void grabKeyboardFocus() override;
    
        //! The position notification function that should be override.
        /** The function is called by the box when its position changed.
         */
        void positionChanged() override;
        
        //! The size notification function that should be override.
        /** The function is called by the box when its size changed.
         */
        void sizeChanged() override;
		
		//! Called by the box when the box selection status changed.
		/** The function is called by the box selection status changed.
		 */
		void selectionStatusChanged() override;
		
		//! Called by the box when the presentation status changed.
		/** The function is called by the box when the presentation status changed.
		 */
		void presentationStatusChanged() override;
		
		//! Called by the page when the edition status has changed.
		/** The function is called by the page when the edition status has changed.
		 */
		void pageEditionStatusChanged() override;
		
		//! Called by the page when the edition status has changed.
		/** The function is called by the page when the edition status has changed.
		 */
		void pagePresentationStatusChanged() override;
	
        void paint(Graphics& g) override;
        void mouseEnter(const MouseEvent& e) override;
        void mouseExit(const MouseEvent& e) override;
        void mouseDown(const MouseEvent& e) override;
        void mouseUp(const MouseEvent& e) override;
        void mouseDrag(const MouseEvent& e) override;
        void mouseMove(const MouseEvent& e) override;
        void mouseDoubleClick(const MouseEvent& e) override;
        void mouseWheelMove(const MouseEvent& e, const MouseWheelDetails& wheel) override;
        void focusGained(FocusChangeType cause) override;
        void focusLost(FocusChangeType cause) override;
        bool keyPressed(const KeyPress& key) override;
    };
    
    typedef shared_ptr<jBox>		sjBox;
	typedef shared_ptr<const jBox> 	scjBox;
    typedef weak_ptr<jBox>			wjBox;
}



#endif
