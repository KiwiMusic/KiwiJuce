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

#ifndef __DEF_KIWI_APPLICATION__
#define __DEF_KIWI_APPLICATION__

#include "jInstance.h"
//#include "DspJuce.h"

//! The Kiwi Application, where all the magic starts !

namespace Kiwi
{
    class Application : public JUCEApplication
    {
    private:
        
        //==============================================================================
        //! A simple class to send a systemRequestedQuit message to the app after a certain amount of time (500ms)
        class AsyncQuitRetrier;
        
        //! The Kiwi Application menu model class
        class MainMenuModel;
        
        //==============================================================================
        //! Initialise the command manager
        void initCommandManager();
        
        //! Called by MainMenuModel to get the menu names
        StringArray getMenuNames();
        
        //! Called by MainMenuModel to create menus
        void createMenu (PopupMenu& menu, const String& menuName);
        
        //! Called by createMenu to create each menu
        void createOpenRecentPatcherMenu	(PopupMenu& menu);
        void createFileMenu				(PopupMenu& menu);
        void createEditMenu				(PopupMenu& menu);
        void createViewMenu				(PopupMenu& menu);
        void createObjectMenu			(PopupMenu& menu);
        void createArrangeMenu			(PopupMenu& menu);
        void createOptionsMenu			(PopupMenu& menu);
        void createWindowMenu			(PopupMenu& menu);
        void createExtraMenu			(PopupMenu& menu);
        void createHelpMenu				(PopupMenu& menu);
        
        //! Called by MainMenuModel to handle the main menu command
        void handleMainMenuCommand (int menuItemID);
        
        //==============================================================================
		shared_ptr<KiwiJuceDspDeviceManager>		m_dsp_device_manager;
        shared_ptr<KiwiJuceGuiDeviceManager>        m_gui_device_manager;
        sjInstance									m_instance;
        ScopedPointer<MainMenuModel>				m_menu_model;
        bool										m_is_running_command_line;
    public:
        
        //! Kiwi Application Constructor
        Application();
        
        /** Called when the application starts. */
        void initialise (const String& commandLine) override;
        
        /** Called to allow the application to clear up before exiting. */
        void shutdown() override;
        
        /** This method is called when the application is being put into background mode
         by the operating system.
         */
        void suspended() override;
        
        /** This method is called when the application is being woken from background mode
         by the operating system.
         */
        void resumed() override;
        
        //==============================================================================
        /** Called when the operating system is trying to close the application. */
        void systemRequestedQuit() override;
        
        //==============================================================================
        /** Returns the application's name. */
        const String getApplicationName() override       { return ProjectInfo::projectName; }
        
        /** Returns the application's version number. */
        const String getApplicationVersion() override    { return ProjectInfo::versionString; }
        
        /** Checks whether multiple instances of the app are allowed. */
        bool moreThanOneInstanceAllowed() override       { return true; }
        
        /** Indicates that the user has tried to start up another instance of the app.
         This will get called even if moreThanOneInstanceAllowed() is false.
         */
        void anotherInstanceStarted (const String& commandLine) override;
        
        //==============================================================================
        
        //! Retrieve the current running Application instance.
        /** The function retrieves the current running application.
         */
        static Application& getApp();
		
		//! Retrieve the current running kiwi instance.
		/** The function retrieves the current running kiwi instance.
		 */
		static sjInstance getKiwiInstance();
        
        // ================================================================================ //
        //                              APPLICATION COMMAND TARGET                          //
        // ================================================================================ //
        
        /** This must return a complete list of commands that this target can handle. */
        void getAllCommands(Array <CommandID>& commands) override;
        
        /** This must provide details about one of the commands that this target can perform. */
        void getCommandInfo(CommandID commandID, ApplicationCommandInfo& result) override;
        
        /** This must actually perform the specified command. */
        bool perform(const InvocationInfo& info) override;
        
        // ================================================================================ //
        //                                      MESSAGES                                    //
        // ================================================================================ //
        
        /** Post a standard message type into the Console.
         @param message  The message in the Juce::String format.
         */
        inline static void post(const String& message)
        {
            Console::post(message.toStdString());
        }
        
        /** Post a warning message type into the Console.
         @param message  The message in the Juce::String format.
         */
        inline static void warning(const String& message)
        {
            Console::warning(message.toStdString());
        }
        
        /** Post an error message type into the Console.
         @param message  The message in the Juce::String format.
         */
        inline static void error(const String& message)
        {
            Console::error(message.toStdString());
        }
    };
}

#endif
