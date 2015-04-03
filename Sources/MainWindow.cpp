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

#include "MainWindow.h"
#include "jInstance.h"
#include "Application.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                  BASE WINDOW                                     //
    // ================================================================================ //
    
    BaseWindow::BaseWindow(String const& name, Colour color, int requiredButtons, bool addToDesktop) :
    DocumentWindow(name, color, requiredButtons, addToDesktop)
    {
#if ! JUCE_MAC
        setMenuBar(Application::getApp().m_menu_model);
#endif
        setUsingNativeTitleBar(true);
        //Application::bindToCommandManager(this);
        //Application::bindToKeyMapping(this);
    }
    
    BaseWindow::~BaseWindow()
    {
        int keymapping;
        //removeKeyListener(Application::getKeyMappings());
    }
    
    void BaseWindow::closeButtonPressed()
    {
        //Application::getController()->closeMainWindow(this);
    }
    
    // ================================================================================ //
    //                              APPLICATION COMMAND TARGET                          //
    // ================================================================================ //
    
    ApplicationCommandTarget* BaseWindow::getNextCommandTarget()
    {
        return findFirstTargetParentComponent();
    }
    
    void BaseWindow::getAllCommands(Array <CommandID>& commands)
    {
        commands.add(CommandIDs::closeWindow);
        commands.add(CommandIDs::minimizeWindow);
        commands.add(CommandIDs::maximizeWindow);
    }
    
    void BaseWindow::getCommandInfo(const CommandID commandID, ApplicationCommandInfo& result)
    {
        switch (commandID)
        {
            case CommandIDs::closeWindow:
                result.setInfo (TRANS("Close"), TRANS("Close Window"), CommandCategories::windows, 0);
                result.addDefaultKeypress ('w', ModifierKeys::commandModifier);
                result.setActive(getDesktopWindowStyleFlags() & allButtons || getDesktopWindowStyleFlags() & closeButton);
                break;
                
            case CommandIDs::minimizeWindow:
                result.setInfo (TRANS("Minimize"), TRANS("Minimize Window"), CommandCategories::windows, 0);
                result.setActive(getDesktopWindowStyleFlags() & allButtons || getDesktopWindowStyleFlags() & minimiseButton);
                break;
                
            case CommandIDs::maximizeWindow:
                result.setInfo (TRANS("Maximize"), TRANS("Maximize Window"), CommandCategories::windows, 0);
                result.setActive(getDesktopWindowStyleFlags() & allButtons || getDesktopWindowStyleFlags() & maximiseButton);
                break;
                
            default:
                break;
        }
    }
    
    bool BaseWindow::perform (const InvocationInfo& info)
    {
        switch (info.commandID)
        {
            case CommandIDs::minimizeWindow: minimiseButtonPressed(); break;
            case CommandIDs::maximizeWindow: maximiseButtonPressed(); break;
            case CommandIDs::closeWindow:    closeButtonPressed();    break;
            default: return false;
        }
        return true;
    }
    
    
    // ================================================================================ //
    //                                  PAGE WINDOW                                     //
    // ================================================================================ //
    
    MainWindow::MainWindow() : BaseWindow("Untitled")
    {
        setBackgroundColour(Colours::lightgrey);
        setResizable(true, true);
        setSize(800, 600);
        setResizeLimits (20, 20, 320000, 320000);
        setVisible(true);
    }
    
    MainWindow::~MainWindow()
    {
        ;
    }
    
    shared_ptr<MainWindow> MainWindow::create()
    {
        return make_shared<MainWindow>();
    }
}

