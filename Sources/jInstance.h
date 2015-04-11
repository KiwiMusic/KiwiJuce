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
#include "../JuceLibraryCode/JuceHeader.h"
#include "KiwiModules.h"

namespace Kiwi
{
    
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
		KiwiLookAndFeel							m_lookandfeel;
    public:
        
        //! The constructor.
        /** You should never have to use this function.
         */
        jInstance(sGuiDeviceManager guiDevice, sDspDeviceManager dspDevice, string const& name);
        
        //! The destrcutor.
        /** You should never have to use this function.
         */
        ~jInstance();
        
        //! The instance component creation method.
        /** The function allocates an instance component.
         @return The instance component.
         */
        static shared_ptr<jInstance> create(sGuiDeviceManager guiDevice, sDspDeviceManager dspDevice, string const& name);
        
        //! Receive the notification that a patcher has been created.
        /** The function is called by the instance when a patcher has been created.
         @param instance    The instance.
         @param patcher        The patcher.
         */
        void patcherCreated(sInstance instance, sPatcher patcher) override;
        
        //! Receive the notification that a patcher has been closed.
        /** The function is called by the instance when a patcher has been closed.
         @param instance    The instance.
         @param patcher        The patcher.
         */
        void patcherRemoved(sInstance instance, sPatcher patcher) override;
        
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
        
        //! Create a new empty patcher.
        /** The function creates a new empty patcher.
         */
        void newPatcher();
        
        //! Open a file
        /** The function opens a patcher
         @param file The file to open.
         */
        void openPatcher(const File& file);
		
		//! Brings the global application settings window to front
		/** The function brings the global application settings window to front
		 */
		void showAppSettingsWindow();
		
		//! Brings the patcher inspector window to front
		/** The function brings the patcher inspector window to front
		 */
		void showInspector(sPatcher jpatcher);
		
		//! Brings the object inspector window to front
		/** The function brings the object inspector window to front
		 */
		void showInspector(sObject objects);
		
		//! Changes the object inspector content if visible.
		/** Changes the object inspector content if visible.
		 */
		void setInspectorContent(sObject objects);
		
		//! Try to save a patcher to disk.
		/** The function attempts to save a patcher to disk.
		 */
		void savePatcher(sPatcher jpatcher);
		
		//! Asks user to open a file.
		/** The function asks user to open a file and attempts to load it.
		 */
		void askUserToOpenFile();
		
		//! Try to open a file
		/** The function attempts to load a patcher.
		 */
		bool openFile(File file);
    };
	
	typedef shared_ptr<jInstance>	sjInstance;
}


#endif








