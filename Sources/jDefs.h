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
#ifndef __Kiwi__EventsWrapper__
#define __Kiwi__EventsWrapper__

#include "../JuceLibraryCode/JuceHeader.h"
#include "Kiwi.h"

enum CommandIDs
{
    newPage						= 0x200010,		/**< Create a new blank page window. */
    newTabPage					= 0x200020,		/**< Create a new blank tab window. */
    openFile					= 0x200030,		/**< Open a file in a new window. */
    closePage					= 0x200051,		/**< Close the current page. */
    save						= 0x200060,		/**< Save the current page or document. */
    saveAs						= 0x200061,		/**< Save the current page or document as. */
    
    minimizeWindow				= 0x201010,		/**< Reduce the current window. */
    maximizeWindow				= 0x201020,		/**< Maximise the  current window. */
    closeWindow					= 0x201031,		/**< Close the current window. */
    closeAllPages				= 0x201040,		/**< Close all page window. */
    
    showConsoleWindow			= 0x202000,		/**< Make visible the "console" window. */
    showAudioStatusWindow		= 0x202010,		/**< Make visible the "audio status" window. */
    showAboutAppWindow			= 0x202020,		/**< Make visible the "about app" window. */
    showAppSettingsWindow		= 0x202030,		/**< Make visible the "application settings" window. */
    
    duplicate					= 0xf1000a,		/**< Duplicate selected objects of the page and paste them on it. */
    pasteReplace				= 0xf1000b,		/**< Duplicate selected objects of the page and paste them on it. */
    
    toFront						= 0xf2000a,		/**< Move selected object ahead of all other objects. */
    toBack						= 0xf2000b,		/**< Move selected object behind all other objects. */
    addToPresentation			= 0xf2000c,		/**< Add selected objects to presentation. */
    removeFromPresentation		= 0xf2000d,		/**< Remove selected objects from presentation. */
    
    zoomIn						= 0xf20013,		/**< Magnify the page view of almost 10%. */
    zoomOut						= 0xf20014,		/**< Reduce the page view of almost 10%. */
    zoomNormal					= 0xf20015,		/**< Restore the page view zoom to 100%. */
    
    editModeSwitch				= 0xf20100,		/**< Toggle Lock/Unlock page view. */
    presentationModeSwitch		= 0xf20101,		/**< Toggle Presentation page view mode. */
    
    gridModeSwitch				= 0xf20200,		/**< Toggle grid page mode. */
    enableSnapToGrid			= 0xf20201,		/**< Toggle snap to grid page mode. */
    
    newObject						= 0xf30300,		/**< Add a new "object" to the page. */
    newMessage					= 0xf30301,		/**< Add a new "message" object object to the page. */
    newFlonum					= 0xf30302,		/**< Add a new "flonum" object object to the page. */
    newNumber					= 0xf30303,		/**< Add a new "number" object object to the page. */
    newComment					= 0xf30304,		/**< Add a new "comment" object object to the page. */
    newBang						= 0xf30305,		/**< Add a new "button" object object to the page. */
    newToggle					= 0xf30306,		/**< Add a new "toggle" object object to the page. */
    
    showPageInspector			= 0xf20400,		/**< Shows the page properties inspector. */
    
    showObjectInspector			= 0xf20500,		/**< Shows the selected objects properties inspector. */
    openObjectHelp				= 0xf20510		/**< Open selected object help page. */
};

#endif /* defined(__Kiwi__EventsWrapper__) */
