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

#include "KiwiWindow.h"
#include "../Application/KiwiApplication.h"

namespace Kiwi
{
    jWindow::jWindow(string const& name,
                     Kiwi::Color bgcolor,
                     int buttons,
                     const bool visible) :
        DocumentWindow(name, toJuce(bgcolor), buttons, false)
    {
#if ! JUCE_MAC
        setMenuBar(Application::getMenuBar());
#endif
        
        setUsingNativeTitleBar(true);
        
        setDraggable(false);
        setResizable(true, true);
        setSize(800, 600);
        setResizeLimits (20, 20, 320000, 320000);
        
        if(visible)
        {
            addToDesktop();
            setVisible(true);
        }
        
        Application::bindToCommandManager(this);
        Application::bindToKeyMapping(this);
    }
    
    jWindow::~jWindow()
    {
        
    }
    
    void jWindow::closeButtonPressed()
    {
        jassertfalse;
    }
    
    void jWindow::minimiseButtonPressed()
    {
        DocumentWindow::minimiseButtonPressed();
    }
    
    void jWindow::maximiseButtonPressed()
    {
        DocumentWindow::maximiseButtonPressed();
    }
    
    // ================================================================================ //
    //                              APPLICATION COMMAND TARGET                          //
    // ================================================================================ //
    
    ApplicationCommandTarget* jWindow::getNextCommandTarget()
    {
        return findFirstTargetParentComponent();
    }
    
    void jWindow::getAllCommands(Array <CommandID>& commands)
    {
        commands.add(ActionCodes::closeWindow);
        commands.add(ActionCodes::minimizeWindow);
        commands.add(ActionCodes::maximizeWindow);
    }
    
    void jWindow::getCommandInfo(const CommandID commandID, ApplicationCommandInfo& result)
    {
        switch (commandID)
        {
            case ActionCodes::closeWindow:
                result.setInfo (TRANS("Close"), TRANS("Close Window"), ActionCategories::windows, 0);
                result.addDefaultKeypress ('w', ModifierKeys::commandModifier);
                result.setActive(getDesktopWindowStyleFlags() & allButtons || getDesktopWindowStyleFlags() & closeButton);
                break;
                
            case ActionCodes::minimizeWindow:
                result.setInfo (TRANS("Minimize"), TRANS("Minimize Window"), ActionCategories::windows, 0);
                result.setActive(getDesktopWindowStyleFlags() & allButtons || getDesktopWindowStyleFlags() & minimiseButton);
                break;
                
            case ActionCodes::maximizeWindow:
                result.setInfo (TRANS("Maximize"), TRANS("Maximize Window"), ActionCategories::windows, 0);
                result.setActive(getDesktopWindowStyleFlags() & allButtons || getDesktopWindowStyleFlags() & maximiseButton);
                break;
                
            default:
                break;
        }
    }
    
    bool jWindow::perform (const InvocationInfo& info)
    {
        switch (info.commandID)
        {
            case ActionCodes::minimizeWindow: minimiseButtonPressed(); break;
            case ActionCodes::maximizeWindow: maximiseButtonPressed(); break;
            case ActionCodes::closeWindow:    closeButtonPressed();    break;
            default: return false;
        }
        return true;
    }
}



