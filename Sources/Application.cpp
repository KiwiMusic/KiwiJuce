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

#include "Application.h"

namespace Kiwi
{
    class Application::MainMenuModel  : public MenuBarModel
    {
    public:
        MainMenuModel()
        {
            //setApplicationCommandManagerToWatch (&getCommandManager());
        }
        
        StringArray getMenuBarNames()
        {
            return getApp().getMenuNames();
        }
        
        PopupMenu getMenuForIndex (int topLevelMenuIndex, const String& menuName)
        {
            PopupMenu menu;
            getApp().createMenu (menu, menuName);
            return menu;
        }
        
        void menuItemSelected (int menuItemID, int topLevelMenuIndex)
        {
            getApp().handleMainMenuCommand (menuItemID);
        }
    };
    
    //==============================================================================
    class Application::AsyncQuitRetrier  : private Timer
    {
    public:
        AsyncQuitRetrier()   { startTimer (500); }
        
        void timerCallback()
        {
            stopTimer();
            delete this;
            
            if (JUCEApplicationBase* app = JUCEApplicationBase::getInstance())
                app->systemRequestedQuit();
        }
        
        JUCE_DECLARE_NON_COPYABLE (AsyncQuitRetrier)
    };
    
    //==============================================================================
    Application::Application() : m_is_running_command_line (false)
    {
        ;
    }
    
    void Application::initialise(const String& commandLine)
    {
		juce::Process::setPriority(juce::Process::RealtimePriority);
		initCommandManager();
		m_dsp_device_manager = make_shared<KiwiJuceDspDeviceManager>();
        m_gui_device_manager = make_shared<KiwiJuceGuiDeviceManager>();
        m_instance = jInstance::create(m_gui_device_manager, m_dsp_device_manager, "main");
        m_menu_model = new MainMenuModel();
        
        /*
#if JUCE_MAC
        PopupMenu macMainMenuPopup;
        macMainMenuPopup.addCommandItem (&getCommandManager(), CommandIDs::showAboutAppWindow);
        macMainMenuPopup.addSeparator();
        macMainMenuPopup.addCommandItem (&getCommandManager(), CommandIDs::showAppSettingsWindow);
        MenuBarModel::setMacMainMenu (m_menu_model, &macMainMenuPopup, TRANS("Open Recent"));
#endif
        */
        
    }
    
    void Application::shutdown()
    {
#if JUCE_MAC
        MenuBarModel::setMacMainMenu(nullptr);
#endif
        m_instance.reset();
    }
    
    void Application::suspended()
    {
        ;
    }
    
    void Application::resumed()
    {
        ;
    }
    
    //==============================================================================
    void Application::systemRequestedQuit()
    {
        m_instance.reset();
        if(ModalComponentManager::getInstance()->cancelAllModalComponents())
        {
            new AsyncQuitRetrier();
        }
        else
        {
            //if(m_instance->closeAllMainWindows())
            {
                quit();
            }
        }
    }
    
    //==============================================================================
    void Application::anotherInstanceStarted(const String& commandLine)
    {
		DBG("another instance of Kiwi started !!");
		
		if (m_instance)
		{
			DBG("Try to open file !");
			m_instance->openFile(File(commandLine.unquoted()));
		}
	}
    
    //==============================================================================
    
    Application& Application::getApp()
    {
        Application* const app = dynamic_cast<Application*> (JUCEApplication::getInstance());
        jassert (app != nullptr);
        return *app;
    }
	
	sjInstance Application::getKiwiInstance()
	{
		sjInstance instance = getApp().m_instance;
		jassert (instance);
		return instance;
	}
	
    //==============================================================================
    StringArray Application::getMenuNames()
    {
        const char* const names[] = {"File", "Edit", "View", "Object", "Arrange", "Options", "Window", "Extra", "Help", nullptr};
        
        return StringArray(names);
    }
    
    void Application::createMenu (PopupMenu& menu, const String& menuName)
    {
        if		(menuName == "File")        createFileMenu		(menu);
        else if (menuName == "Edit")        createEditMenu		(menu);
        else if (menuName == "View")        createViewMenu		(menu);
        else if (menuName == "Object")      createObjectMenu	(menu);
        else if (menuName == "Arrange")     createArrangeMenu	(menu);
        else if (menuName == "Options")     createOptionsMenu	(menu);
        else if (menuName == "Window")      createWindowMenu	(menu);
        else if (menuName == "Extra")       createExtraMenu		(menu);
        else if (menuName == "Help")		createHelpMenu		(menu);
        
        else                                jassertfalse; // names have changed?
    }
    
    void Application::createOpenRecentPatcherMenu (PopupMenu& menu)
    {
        
    }
    
    void Application::createFileMenu (PopupMenu& menu)
    {
        /*
        menu.addCommandItem (m_command_manager, CommandIDs::newPatcher);
        menu.addCommandItem (m_command_manager, CommandIDs::newTabPatcher);
        menu.addSeparator();
        
        menu.addCommandItem (m_command_manager, CommandIDs::openFile);
        createOpenRecentPatcherMenu (menu);
        menu.addCommandItem (m_command_manager, CommandIDs::closeWindow);
        menu.addSeparator();
        
        menu.addCommandItem (m_command_manager, CommandIDs::save);
        menu.addCommandItem (m_command_manager, CommandIDs::saveAs);
        menu.addSeparator();
        
#if ! JUCE_MAC
        menu.addSeparator();
        menu.addCommandItem (m_command_manager, StandardApplicationCommandIDs::quit);
#endif
         */
    }
    
    void Application::createEditMenu (PopupMenu& menu)
    {
        /*
        menu.addCommandItem (m_command_manager, StandardApplicationCommandIDs::undo);
        menu.addCommandItem (m_command_manager, StandardApplicationCommandIDs::redo);
        menu.addSeparator();
        menu.addCommandItem (m_command_manager, StandardApplicationCommandIDs::cut);
        menu.addCommandItem (m_command_manager, StandardApplicationCommandIDs::copy);
        menu.addCommandItem (m_command_manager, StandardApplicationCommandIDs::paste);
        menu.addCommandItem (m_command_manager, StandardApplicationCommandIDs::del);
        menu.addSeparator();
        menu.addCommandItem (m_command_manager, CommandIDs::pasteReplace);
        menu.addCommandItem (m_command_manager, CommandIDs::duplicate);
        menu.addCommandItem (m_command_manager, StandardApplicationCommandIDs::selectAll);
        menu.addCommandItem (m_command_manager, StandardApplicationCommandIDs::deselectAll);
        menu.addSeparator();
         */
    }
    
    void Application::createViewMenu (PopupMenu& menu)
    {
        /*
        menu.addCommandItem (m_command_manager, CommandIDs::editModeSwitch);
        menu.addCommandItem (m_command_manager, CommandIDs::presentationModeSwitch);
		menu.addSeparator();
		menu.addCommandItem (m_command_manager, CommandIDs::showPatcherInspector);
        menu.addSeparator();
        menu.addCommandItem (m_command_manager, CommandIDs::gridModeSwitch);
        menu.addSeparator();
        menu.addCommandItem (m_command_manager, CommandIDs::zoomIn);
        menu.addCommandItem (m_command_manager, CommandIDs::zoomOut);
        menu.addCommandItem (m_command_manager, CommandIDs::zoomNormal);
        menu.addSeparator();
         */
    }
    
    void Application::createObjectMenu (PopupMenu& menu)
    {
        /*
		vector<sTag> objectNames = Factory::names();
		
		if (objectNames.size() > 0)
		{
			PopupMenu names;
			
			sort(objectNames.begin(), objectNames.end());
			
			for (int i = 0; i < objectNames.size(); ++i)
			{
				//names.addItem(objectPrototypeNamesBaseID + i, objectNames[i]);
				names.addItem(1 + i, objectNames[i]->getName());
			}
			
			menu.addSubMenu ("object thesaurus", names);
		}
		
        menu.addCommandItem (m_command_manager, CommandIDs::showObjectInspector);
        menu.addSeparator();
        menu.addCommandItem (m_command_manager, CommandIDs::addToPresentation);
        menu.addCommandItem (m_command_manager, CommandIDs::removeFromPresentation);
         */
    }
    
    void Application::createArrangeMenu (PopupMenu& menu)
    {
        /*
        menu.addCommandItem (m_command_manager, CommandIDs::enableSnapToGrid);
        menu.addSeparator();
        menu.addCommandItem (m_command_manager, CommandIDs::toFront);
        menu.addCommandItem (m_command_manager, CommandIDs::toBack);
         */
    }
    
    void Application::createOptionsMenu (PopupMenu& menu)
    {
        //menu.addCommandItem (m_command_manager, CommandIDs::showAudioStatusWindow);
    }
    
    void Application::createWindowMenu (PopupMenu& menu)
    {
        /*
        menu.addCommandItem (m_command_manager, CommandIDs::minimizeWindow);
        menu.addCommandItem (m_command_manager, CommandIDs::maximizeWindow);
        menu.addSeparator();
        
        menu.addCommandItem (m_command_manager, CommandIDs::showConsoleWindow);
        
        menu.addSeparator();
        menu.addCommandItem (m_command_manager, CommandIDs::closeAllPatchers);
         */
    }
    
    void Application::createExtraMenu (PopupMenu& menu)
    {
        ;
    }
    
    void Application::createHelpMenu (PopupMenu& menu)
    {
        //menu.addCommandItem (m_command_manager, CommandIDs::openObjectHelp);
    }
    
    void Application::handleMainMenuCommand (int menuItemID)
    {
        ;
    }
    
    //==============================================================================
    void Application::getAllCommands (Array <CommandID>& commands)
    {
        JUCEApplication::getAllCommands (commands); // get the standard quit command
        
        // this returns the set of all commands that this target can perform..
        const CommandID ids[] =
        {
            CommandIDs::newPatcher,
            CommandIDs::newTabPatcher,
            CommandIDs::openFile,
            CommandIDs::showConsoleWindow,
            CommandIDs::showAudioStatusWindow,
            CommandIDs::showAppSettingsWindow,
            CommandIDs::showAboutAppWindow
        };
        
        commands.addArray (ids, numElementsInArray (ids));
    }
    
    void Application::getCommandInfo (CommandID commandID, ApplicationCommandInfo& result)
    {
        switch (commandID)
        {
            case CommandIDs::newPatcher:
                result.setInfo (TRANS("New Patcher Window..."), TRANS("Creates a new Patcher Window"), CommandCategories::general, 0);
                result.defaultKeypresses.add (KeyPress ('n', ModifierKeys::commandModifier, 0));
                break;
                
            case CommandIDs::newTabPatcher:
                result.setInfo (TRANS("New Tab Patcher"), TRANS("Create a New Tab Patcher"), CommandCategories::general, 0);
                result.defaultKeypresses.add (KeyPress ('t', ModifierKeys::commandModifier, 0));
                result.setActive(false);
                break;
                
            case CommandIDs::openFile:
                result.setInfo (TRANS("Open..."), TRANS("Opens a File"), CommandCategories::general, 0);
                result.defaultKeypresses.add (KeyPress ('o', ModifierKeys::commandModifier, 0));
                break;
                
            case CommandIDs::closeAllPatchers:
                result.setInfo (TRANS("Close All Patchers"), TRANS("Close All Patchers"), CommandCategories::windows, 0);
                //result.setActive (m_instance->getNumOpenMainWindows() > 0);
                break;
                
            case CommandIDs::showConsoleWindow:
                result.setInfo (TRANS("Console"), TRANS("Show Kiwi Console"), CommandCategories::windows, 0);
                result.addDefaultKeypress ('k', ModifierKeys::commandModifier);
                break;
                
            case CommandIDs::showAudioStatusWindow:
                result.setInfo (TRANS("Audio settings"), TRANS("Show Audio Settings"), CommandCategories::windows, 0);
                break;
                
            case CommandIDs::showAboutAppWindow:
                result.setInfo (TRANS("About Kiwi..."), TRANS("Show App informations"), CommandCategories::windows, 0);
                break;
                
            case CommandIDs::showAppSettingsWindow:
                result.setInfo (TRANS("Preferences..."), TRANS("Show App Preferences"), CommandCategories::windows, 0);
                result.addDefaultKeypress (',', ModifierKeys::commandModifier);
                break;
                
            default:
                JUCEApplication::getCommandInfo (commandID, result);
                break;
        }
    }
    
    bool Application::perform (const InvocationInfo& info)
    {
        switch (info.commandID)
        {
            //case CommandIDs::newPatcher:						m_instance->createNewMainWindow();	break;
            case CommandIDs::openFile:						m_instance->askUserToOpenFile();		break;
            //case CommandIDs::closeAllPatchers:					m_instance->closeAllMainWindows(); 	break;
            //case CommandIDs::showConsoleWindow:				m_instance->showConsoleWindow();		break;
            //case CommandIDs::showAudioStatusWindow:			m_instance->showAudioStatusWindow();	break;
				
			case CommandIDs::showAppSettingsWindow:			m_instance->showAppSettingsWindow();	break;

            default:										return JUCEApplication::perform (info);
        }
        
        return true;
    }
    
    void Application::initCommandManager()
    {
        /*
		m_command_manager = new ApplicationCommandManager();
        m_command_manager->registerAllCommandsForTarget (this);
		
        {
            BaseWindow window;
            m_command_manager->registerAllCommandsForTarget(&window);
        }
         */
    }
}

