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

#ifndef __DEF_KIWI_ATTRIBUTEVIEW__
#define __DEF_KIWI_ATTRIBUTEVIEW__

#include "MainWindow.h"
#include "AttributePanel.h"

namespace Kiwi
{
    // ================================================================================ //
    //								ATTRIBUTE PANEL COMPONENT                           //
    // ================================================================================ //
	
	//! Display a set of attributes.
	/** The attribute panel is a component that display a set of attributes
	 @see \class Attribute, \class Attribute::Manager
	 */
    class AttributeViewer : public AttributePanel,
							public Attr::Listener,
							public enable_shared_from_this<AttributeViewer>
    {
    private:
		Attr::sManager					m_attr_manager;
		
		shared_ptr<AttributeComponent> createBoolComponent(Attr::sManager manager, sAttr attr);
		shared_ptr<AttributeComponent> createNumberComponent(Attr::sManager manager, sAttr attr);
		shared_ptr<AttributeComponent> createTextComponent(Attr::sManager manager, sAttr attr);
		shared_ptr<AttributeComponent> createMenuComponent(Attr::sManager manager, sAttr attr);
		shared_ptr<AttributeComponent> createColorComponent(Attr::sManager manager, sAttr attr);
        
    public:
        
        //! The constructor.
        /** You should never use this method except if you really know what you do, use the static create method instead.
		 @see create
         */
        AttributeViewer();
        
        //! The destructor.
        /** You should never use this method except if you really know what you do.
         */
        ~AttributeViewer();
		
		//! Attribute panel creator.
		/** This static method creates and returns a shared pointer of an AttributePanelComponent.
		 @param attrsManager A shared pointer of an Attribute::Manager to watch.
		 @return    A shared pointer of an AttributePanelComponent.
		 */
		static inline shared_ptr<AttributeViewer> create(Attr::sManager attrsManager)
		{
			shared_ptr<AttributeViewer> panel = make_shared<AttributeViewer>();
			panel->setAttributeManagerToWatch(attrsManager);
			return panel;
		}
		
		inline Attr::sManager getViewedManager() const noexcept
		{
			return m_attr_manager;
		}
		
		//! Returns a shared pointer of this.
		shared_ptr<AttributeViewer> getShared() {return shared_from_this();}
		
		//==============================================================================
		//! Sets a new Attribute Manager to watch
		/** This method Sets a new Attribute Manager to watch.
		 @param  attrsManager  A shared pointer of an Attribute::Manager to watch.
		 */
		void setAttributeManagerToWatch(Attr::sManager attrsManager);
		
		//! Receive the notification that an attribute has changed.
		/** Receive notifications when an attribute is added or removed, or when its value, appearance or behavior changes.
		 @param manager		The Attribute::Manager that manages the attribute.
		 @param attr		The attribute that has been modified.
		 @type type			The type of notification
		 */
		void notify(Attr::sManager manager, sAttr attr, Attr::Notification type) override;
	
		//==============================================================================
		//! Rebuild all property components.
		/** This method rebuild all property components
		 */
		void rebuildProperties();
	
		//==============================================================================
		//! Called when this component is resized.
		void resized() override;
    };
	
	//! The shared pointer of an attribute panel component.
	/**
	 The sAttributePanel is shared pointer of an attribute panel component.
	 */
	typedef shared_ptr<AttributeViewer> sAttributeViewer;

    // ================================================================================ //
    //                                  INSPECTOR WINDOW                                //
    // ================================================================================ //
    
    class InspectorWindow  :	public BaseWindow
    {
    private:
        sAttributeViewer		m_attr_viewer;
        
    public:
        InspectorWindow(sAttributeViewer panel = sAttributeViewer());
        ~InspectorWindow();
		
		//! Inspector window creator.
		/** This static method creates and returns a shared pointer of an InspectorWindow.
		 @param attrViewer A shared pointer of an AttributeViewer.
		 @return    A shared pointer of an InspectorWindow.
		 */
		static inline shared_ptr<InspectorWindow> create(sAttributeViewer attrViewer = sAttributeViewer())
		{
			shared_ptr<InspectorWindow> window = make_shared<InspectorWindow>(attrViewer);
			return window;
		}
		
		//! Replaces the current inspector panel by a new one.
		/** Replaces the current inspector panel by a new one.
		 @param page A page.
		 */
		void setPage(sPage page);
		
		//! Replaces the current inspector panel by a new one.
		/** Replaces the current inspector panel by a new one.
		 @param page A Box.
		 */
		void setBox(sBox box);
        
        void closeButtonPressed() override;
    };
}


#endif