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

#ifndef __DEF_KIWI_JINSTANCECONTROLLER__
#define __DEF_KIWI_JINSTANCECONTROLLER__

#include "jLookAndFeel.h"
#include "MainWindow.h"
#include "ConsoleComponent.h"
#include "jPage.h"
#include "AttributeView.h"

namespace Kiwi
{
    namespace CommandCategories
    {
        static const char* const general       = "General";
        static const char* const editing       = "Editing";
        static const char* const view          = "View";
        static const char* const windows       = "Windows";
    }
    
    // ================================================================================ //
    //                                  EDITOR APPLICATION                              //
    // ================================================================================ //
    
    //! The editor application is the main class of the well-known graphical object application.
    /**
     Patati patata.
     */
    class jInstance : public Instance::Listener, public enable_shared_from_this<jInstance>
    {
    private:
        sInstance                               m_instance;
        shared_ptr<MenuBarModel>                m_menu;
        shared_ptr<MainWindow>                  m_window;
		shared_ptr<InspectorWindow>             m_app_settings_window;
		shared_ptr<InspectorWindow>             m_page_inspector_window;
		shared_ptr<InspectorWindow>             m_object_inspector_window;
        vector<sjPage>							m_pages;
		vector<shared_ptr<MainWindow>>          m_page_windows;
		KiwiLookAndFeel							m_lookandfeel;
    public:
        
        //! The constructor.
        /** You should never have to use this function.
         */
        jInstance();
        
        //! The destrcutor.
        /** You should never have to use this function.
         */
        ~jInstance();
        
        //! The instance component creation method.
        /** The function allocates an instance component.
         @return The instance component.
         */
        static shared_ptr<jInstance> create();
        
        //! Receive the notification that a page has been created.
        /** The function is called by the instance when a page has been created.
         @param instance    The instance.
         @param page        The page.
         */
        void pageCreated(sInstance instance, sPage page) override;
        
        //! Receive the notification that a page has been closed.
        /** The function is called by the instance when a page has been closed.
         @param instance    The instance.
         @param page        The page.
         */
        void pageRemoved(sInstance instance, sPage page) override;
        
        //! Receive the notification that the dsp has been started.
        /** The function is called by the instance when the dsp has been started.
         @param instance    The instance.
         */
        void dspStarted(shared_ptr<Instance> instance) override;
        
        //! Receive the notification that the dsp has been stopped.
        /** The function is called by the instance when the dsp has been stopped.
         @param instance    The instance.
         */
        void dspStopped(shared_ptr<Instance> instance) override;
        
        //! Create a new empty page.
        /** The function creates a new empty page.
         */
        void newPage();
        
        //! Open a file
        /** The function opens a page
         @param file The file to open.
         */
        void openPage(const File& file);
		
		//! Brings the global application settings window to front
		/** The function brings the global application settings window to front
		 */
		void showAppSettingsWindow();
		
		//! Brings the page inspector window to front
		/** The function brings the page inspector window to front
		 */
		void showInspector(sPage jpage);
		
		//! Brings the object inspector window to front
		/** The function brings the object inspector window to front
		 */
		void showInspector(sBox boxes);
		
		//! Changes the object inspector content if visible.
		/** Changes the object inspector content if visible.
		 */
		void setInspectorContent(sBox boxes);
		
		//! Try to save a page to disk.
		/** The function attempts to save a page to disk.
		 */
		void savePage(sPage jpage);
		
		//! Asks user to open a file.
		/** The function asks user to open a file and attempts to load it.
		 */
		void askUserToOpenFile();
		
		//! Try to open a file
		/** The function attempts to load a page.
		 */
		bool openFile(File file);
        
        /*
        closePage
        save
        saveAs
        
        minimizeWindow
        maximizeWindow
        closeWindow	
        closeAllPages
         */
    };
	
	typedef shared_ptr<jInstance>	sjInstance;
}


#endif








