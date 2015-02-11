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

#ifndef __DEF_KIWI_JBOX__
#define __DEF_KIWI_JBOX__

#include "jDoodle.h"
#include "jEvent.h"
#include "jLabel.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                  JBOX CONTROLER                                  //
    // ================================================================================ //
    
    //! The jObject is the juce implementation of the ObjectView.
    /**
     The jObject overrides the redraw and paint method and wraps the mouse and keyboard events.
     */
	class jObject : public ObjectView, public Component
    {
	private:
		typedef shared_ptr<jObject>		sjObject;
		sjLabel							m_label;
				
		const double m_framesize;
		void checkVisibilityAndInteractionMode();
    public:
        
        //! The constructor.
        /** You should never have to use this function.
         */
        jObject(sObject object, sPatcherView patcherview);
        
        //! Destructor.
        /** You should never have to use this function.
         */
        ~jObject();
		
		sjObject getShared()
		{
			return dynamic_pointer_cast<jObject>(shared_from_this());
		}
		
		/** This function is called just after the objectview has been created.
		 */
		void init() override;
		
		//! Retrieve the display bounds of the object view.
		/** The function retrieves the display bounds of the object view.
		 @return The bounds of the object view.
		 */
		Gui::Rectangle getDisplayBounds() const noexcept override;
		
		//! Retrieve the display position of the object view.
		/** The function retrieves the display position of the object view.
		 @return The position of the object view.
		 */
		Gui::Point getDisplayPosition() const noexcept override;
		
		//! Retrieve the display size of the object view.
		/** The function retrieves the display size of the object view.
		 @return The size of the object view.
		 */
		Gui::Point getDisplaySize() const noexcept override;
		
		//! Tests if a point is inside an object resizer zone.
		/** The function tests if a point is inside an object resizer zone. The point is relative to the patcher top-left's coordinates
		 @param point The point to test.
		 @return A flag describing the resizer zone as defined in the Knock::Border enum.
		 */
		ulong resizerKnock(Gui::Point const& point) const noexcept override;
		
        //! The redraw function that should be override.
        /** The function is called by the object when it should be repainted.
         @param object    The object.
         */
        void redraw() override;
		
		//! Get if the view is currently being edited.
		/** The function get if the view is currently being edited.
		 @return True if the view is currently being edited, false otherwise.
		 */
		void textFieldEditorShown() override;
		
		//! Receives the notification that the textfield's text changed.
		/** The function receives the notification that the textfield's text changed
		 */
		void textfieldTextChanged() override;
		
        //! The grab focus function that should be override.
        /** The function is called by the object when it want to grab keyboard focus.
         */
        void grabKeyboardFocus() override;
    
        //! The position notification function that should be override.
        /** The function is called by the object when its position changed.
         */
        void positionChanged() override;
        
        //! The size notification function that should be override.
        /** The function is called by the object when its size changed.
         */
        void sizeChanged() override;
		
		//! Called by the object when the object selection status changed.
		/** The function is called by the object selection status changed.
		 */
		void patcherViewSelectionStatusChanged() override;
		
		//! Called by the object when the presentation status changed.
		/** The function is called by the object when the presentation status changed.
		 */
		void presentationStatusChanged() override;
		
		//! Called by the patcher when the lock status has changed.
		/** The function is called by the patcher when the lock status has changed.
		 */
		void patcherViewLockStatusChanged() override;
		
		//! Called by the patcher when the edition status has changed.
		/** The function is called by the patcher when the edition status has changed.
		 */
		void patcherViewPresentationStatusChanged() override;
	
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
    
    typedef shared_ptr<jObject>		sjObject;
	typedef weak_ptr<jObject>			wjObject;
	typedef shared_ptr<const jObject> 	scjObject;
	typedef weak_ptr<const jObject> 	wcjObject;
}



#endif
