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

#include "KiwiMainMenu.h"

namespace Kiwi
{
    KiwiMainMenuModel::KiwiMainMenuModel(shared_ptr<ApplicationCommandManager> commandManagerToWatch)
    {
        m_cmd_manager = commandManagerToWatch;
        ApplicationCommandManager* manager = commandManagerToWatch.get();
        setApplicationCommandManagerToWatch(manager);
        
        #if JUCE_MAC
        PopupMenu macMainMenuPopup;
        macMainMenuPopup.addCommandItem(manager, ActionCodes::showAboutAppWindow);
        macMainMenuPopup.addSeparator();
        macMainMenuPopup.addCommandItem(manager, ActionCodes::showAppSettingsWindow);
        MenuBarModel::setMacMainMenu(this, &macMainMenuPopup, TRANS("Open Recent"));
        #endif
    }
    
    KiwiMainMenuModel::~KiwiMainMenuModel()
    {
        #if JUCE_MAC
        MenuBarModel::setMacMainMenu(nullptr);
        #endif
    }
    
    PopupMenu KiwiMainMenuModel::getMenuForIndex(int topLevelMenuIndex, const String& menuName)
    {
        PopupMenu menu;
        createMenu(menu, menuName);
        return menu;
    }
    
    void KiwiMainMenuModel::menuItemSelected(int menuItemID, int topLevelMenuIndex)
    {
        handleMainMenuCommand(menuItemID);
    }
	
    StringArray KiwiMainMenuModel::getMenuBarNames()
    {
        const char* const names[] = {"File", "Edit", "View", "Object", "Arrange", "Options", "Window", "Extra", "Help", nullptr};
        
        return StringArray(names);
    }
    
    void KiwiMainMenuModel::createMenu(PopupMenu& menu, const String& menuName)
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
    
    void KiwiMainMenuModel::createOpenRecentPatcherMenu(PopupMenu& menu)
    {
        
    }
    
    void KiwiMainMenuModel::createFileMenu(PopupMenu& menu)
    {
        ApplicationCommandManager* manager = getCommandManager();
        
        menu.addCommandItem(manager, ActionCodes::newPatcher);
        menu.addCommandItem(manager, ActionCodes::newTabPatcher);
        menu.addSeparator();
        
        menu.addCommandItem(manager, ActionCodes::openFile);
        createOpenRecentPatcherMenu(menu);
        menu.addCommandItem(manager, ActionCodes::closeWindow);
        menu.addSeparator();
        
        menu.addCommandItem(manager, ActionCodes::save);
        menu.addCommandItem(manager, ActionCodes::saveAs);
        menu.addSeparator();
        
        #if !JUCE_MAC
        menu.addSeparator();
        menu.addCommandItem(m_command_manager, StandardApplicationCommandIDs::quit);
        #endif
    }
    
    void KiwiMainMenuModel::createEditMenu(PopupMenu& menu)
    {
        ApplicationCommandManager* manager = getCommandManager();
        
        menu.addCommandItem(manager, StandardApplicationCommandIDs::undo);
        menu.addCommandItem(manager, StandardApplicationCommandIDs::redo);
        menu.addSeparator();
        menu.addCommandItem(manager, StandardApplicationCommandIDs::cut);
        menu.addCommandItem(manager, StandardApplicationCommandIDs::copy);
        menu.addCommandItem(manager, StandardApplicationCommandIDs::paste);
        menu.addCommandItem(manager, StandardApplicationCommandIDs::del);
        menu.addSeparator();
        menu.addCommandItem(manager, ActionCodes::pasteReplace);
        menu.addCommandItem(manager, ActionCodes::duplicate);
        menu.addCommandItem(manager, StandardApplicationCommandIDs::selectAll);
        menu.addCommandItem(manager, StandardApplicationCommandIDs::deselectAll);
        menu.addSeparator();
    }
    
    void KiwiMainMenuModel::createViewMenu(PopupMenu& menu)
    {
        ApplicationCommandManager* manager = getCommandManager();
        
        menu.addCommandItem(manager, ActionCodes::editModeSwitch);
        menu.addCommandItem(manager, ActionCodes::presentationModeSwitch);
		menu.addSeparator();
		menu.addCommandItem(manager, ActionCodes::showPatcherInspector);
        menu.addSeparator();
        menu.addCommandItem(manager, ActionCodes::gridModeSwitch);
        menu.addSeparator();
        menu.addCommandItem(manager, ActionCodes::zoomIn);
        menu.addCommandItem(manager, ActionCodes::zoomOut);
        menu.addCommandItem(manager, ActionCodes::zoomNormal);
        menu.addSeparator();
    }
    
    void KiwiMainMenuModel::createObjectMenu(PopupMenu& menu)
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
			
			menu.addSubMenu("object thesaurus", names);
		}
		
        menu.addCommandItem(m_command_manager, ActionCodes::showObjectInspector);
        menu.addSeparator();
        menu.addCommandItem(m_command_manager, ActionCodes::addToPresentation);
        menu.addCommandItem(m_command_manager, ActionCodes::removeFromPresentation);
         */
    }
    
    void KiwiMainMenuModel::createArrangeMenu(PopupMenu& menu)
    {
        /*
        menu.addCommandItem(m_command_manager, ActionCodes::enableSnapToGrid);
        menu.addSeparator();
        menu.addCommandItem(m_command_manager, ActionCodes::toFront);
        menu.addCommandItem(m_command_manager, ActionCodes::toBack);
         */
    }
    
    void KiwiMainMenuModel::createOptionsMenu(PopupMenu& menu)
    {
        //menu.addCommandItem(m_command_manager, ActionCodes::showAudioStatusWindow);
    }
    
    void KiwiMainMenuModel::createWindowMenu(PopupMenu& menu)
    {
        ApplicationCommandManager* manager = getCommandManager();

        menu.addCommandItem(manager, ActionCodes::minimizeWindow);
        menu.addCommandItem(manager, ActionCodes::maximizeWindow);
        menu.addSeparator();
        
        menu.addCommandItem(manager, ActionCodes::showConsoleWindow);
        
        menu.addSeparator();
        menu.addCommandItem(manager, ActionCodes::closeAllPatchers);
    }
    
    void KiwiMainMenuModel::createExtraMenu(PopupMenu& menu)
    {
        ;
    }
    
    void KiwiMainMenuModel::createHelpMenu(PopupMenu& menu)
    {
        //menu.addCommandItem(m_command_manager, ActionCodes::openObjectHelp);
    }
    
    void KiwiMainMenuModel::handleMainMenuCommand(int menuItemID)
    {
        ;
    }
}

