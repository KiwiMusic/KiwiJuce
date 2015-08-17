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

#include "KiwiApplication.h"
#include "../Wrapper/KiwiGuiJuceDefine.h"

namespace Kiwi
{
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
    Application::Application()
    {
        ;
    }
    
    void Application::initialise(const String& commandLine)
    {
		juce::Process::setPriority(juce::Process::RealtimePriority);
		initCommandManager();
        m_menubar = make_shared<KiwiMainMenuModel>(m_command_manager);
        
		m_dsp_device_manager = make_shared<KiwiJuceDspDeviceManager>();
        m_gui_device_manager = make_shared<KiwiJuceGuiDeviceManager>();
        m_gui_device_manager->initialize();
        m_instance = jInstance::create(m_gui_device_manager, m_dsp_device_manager, "main");
    }
    
    void Application::shutdown()
    {
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
        Application* const app = dynamic_cast<Application*>(JUCEApplication::getInstance());
        jassert(app != nullptr);
        return *app;
    }
	
	sjInstance Application::getKiwiInstance()
	{
		sjInstance instance = getApp().m_instance;
		jassert(instance);
		return instance;
	}
    
    MenuBarModel* Application::getMenuBar()
    {
        sjMenuBar menubar = getApp().m_menubar;
        jassert(menubar);
        return menubar.get();
    }
    
    //==============================================================================
    void Application::getAllCommands(Array <CommandID>& commands)
    {
        JUCEApplication::getAllCommands(commands); // get the standard quit command
        
        // this returns the set of all commands that this target can perform..
        const CommandID ids[] =
        {
            ActionCodes::newPatcher,
            ActionCodes::newTabPatcher,
            ActionCodes::openFile,
            ActionCodes::showConsoleWindow,
            ActionCodes::showAudioStatusWindow,
            ActionCodes::showAppSettingsWindow,
            ActionCodes::showAboutAppWindow
        };
        
        commands.addArray(ids, numElementsInArray (ids));
    }
    
    void Application::getCommandInfo(CommandID commandID, ApplicationCommandInfo& cmd)
    {
        switch (commandID)
        {
            case ActionCodes::newPatcher:
                cmd.setInfo (TRANS("New Patcher Window..."), TRANS("Creates a new Patcher Window"), ActionCategories::general, 0);
                cmd.defaultKeypresses.add (KeyPress ('n', ModifierKeys::commandModifier, 0));
                break;
                
            case ActionCodes::newTabPatcher:
                cmd.setInfo (TRANS("New Tab Patcher"), TRANS("Create a New Tab Patcher"), ActionCategories::general, 0);
                cmd.defaultKeypresses.add (KeyPress ('t', ModifierKeys::commandModifier, 0));
                cmd.setActive(false);
                break;
                
            case ActionCodes::openFile:
                cmd.setInfo (TRANS("Open..."), TRANS("Open a File"), ActionCategories::general, 0);
                cmd.defaultKeypresses.add (KeyPress ('o', ModifierKeys::commandModifier, 0));
                break;
                
            case ActionCodes::closeAllPatchers:
                cmd.setInfo (TRANS("Close All Patchers"), TRANS("Close All Patchers"), ActionCategories::windows, 0);
                //cmd.setActive (m_instance->getNumOpenMainWindows() > 0);
                break;
                
            case ActionCodes::showConsoleWindow:
                cmd.setInfo (TRANS("Console"), TRANS("Show Kiwi Console"), ActionCategories::windows, 0);
                cmd.addDefaultKeypress ('k', ModifierKeys::commandModifier);
                break;
                
            case ActionCodes::showAudioStatusWindow:
                cmd.setInfo (TRANS("Audio settings"), TRANS("Show Audio Settings"), ActionCategories::windows, 0);
                break;
                
            case ActionCodes::showAboutAppWindow:
                cmd.setInfo (TRANS("About Kiwi..."), TRANS("Show App informations"), ActionCategories::windows, 0);
                break;
                
            case ActionCodes::showAppSettingsWindow:
                cmd.setInfo (TRANS("Preferences..."), TRANS("Show App Preferences"), ActionCategories::windows, 0);
                cmd.addDefaultKeypress (',', ModifierKeys::commandModifier);
                break;
                
            default:
                JUCEApplication::getCommandInfo(commandID, cmd);
                break;
        }
    }
    
    bool Application::perform(const InvocationInfo& info)
    {
        switch (info.commandID)
        {
            case ActionCodes::newPatcher:						m_instance->newPatcher();	break;
            //case CommandIDs::openFile:						m_instance->askUserToOpenFile();		break;
            //case CommandIDs::closeAllPatchers:					m_instance->closeAllMainWindows(); 	break;
            //case CommandIDs::showConsoleWindow:				m_instance->showConsoleWindow();		break;
            //case CommandIDs::showAudioStatusWindow:			m_instance->showAudioStatusWindow();	break;
				
			//case CommandIDs::showAppSettingsWindow:			m_instance->showAppSettingsWindow();	break;

            default:										return JUCEApplication::perform (info);
        }
        return true;
    }
    
    void Application::initCommandManager()
    {
        m_command_manager = make_shared<ApplicationCommandManager>();
        m_command_manager->registerAllCommandsForTarget(this);
        
        {
            //BaseWindow window;
            //m_command_manager->registerAllCommandsForTarget(&window);
        }
    }
}

