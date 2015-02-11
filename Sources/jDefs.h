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
    newPatcher						= 0x200010,		/**< Create a new blank patcher window. */
    newTabPatcher					= 0x200020,		/**< Create a new blank tab window. */
    openFile					= 0x200030,		/**< Open a file in a new window. */
    closePatcher					= 0x200051,		/**< Close the current patcher. */
    save						= 0x200060,		/**< Save the current patcher or document. */
    saveAs						= 0x200061,		/**< Save the current patcher or document as. */
    
    minimizeWindow				= 0x201010,		/**< Reduce the current window. */
    maximizeWindow				= 0x201020,		/**< Maximise the  current window. */
    closeWindow					= 0x201031,		/**< Close the current window. */
    closeAllPatchers				= 0x201040,		/**< Close all patcher window. */
    
    showConsoleWindow			= 0x202000,		/**< Make visible the "console" window. */
    showAudioStatusWindow		= 0x202010,		/**< Make visible the "audio status" window. */
    showAboutAppWindow			= 0x202020,		/**< Make visible the "about app" window. */
    showAppSettingsWindow		= 0x202030,		/**< Make visible the "application settings" window. */
    
    duplicate					= 0xf1000a,		/**< Duplicate selected objects of the patcher and paste them on it. */
    pasteReplace				= 0xf1000b,		/**< Duplicate selected objects of the patcher and paste them on it. */
    
    toFront						= 0xf2000a,		/**< Move selected object ahead of all other objects. */
    toBack						= 0xf2000b,		/**< Move selected object behind all other objects. */
    addToPresentation			= 0xf2000c,		/**< Add selected objects to presentation. */
    removeFromPresentation		= 0xf2000d,		/**< Remove selected objects from presentation. */
    
    zoomIn						= 0xf20013,		/**< Magnify the patcher view of almost 10%. */
    zoomOut						= 0xf20014,		/**< Reduce the patcher view of almost 10%. */
    zoomNormal					= 0xf20015,		/**< Restore the patcher view zoom to 100%. */
    
    editModeSwitch				= 0xf20100,		/**< Toggle Lock/Unlock patcher view. */
    presentationModeSwitch		= 0xf20101,		/**< Toggle Presentation patcher view mode. */
    
    gridModeSwitch				= 0xf20200,		/**< Toggle grid patcher mode. */
    enableSnapToGrid			= 0xf20201,		/**< Toggle snap to grid patcher mode. */
    
    newObject						= 0xf30300,		/**< Add a new "object" to the patcher. */
    newMessage					= 0xf30301,		/**< Add a new "message" object object to the patcher. */
    newFlonum					= 0xf30302,		/**< Add a new "flonum" object object to the patcher. */
    newNumber					= 0xf30303,		/**< Add a new "number" object object to the patcher. */
    newComment					= 0xf30304,		/**< Add a new "comment" object object to the patcher. */
    newBang						= 0xf30305,		/**< Add a new "button" object object to the patcher. */
    newToggle					= 0xf30306,		/**< Add a new "toggle" object object to the patcher. */
    
    showPatcherInspector			= 0xf20400,		/**< Shows the patcher properties inspector. */
    
    showObjectInspector			= 0xf20500,		/**< Shows the selected objects properties inspector. */
    openObjectHelp				= 0xf20510		/**< Open selected object help patcher. */
};

#endif /* defined(__Kiwi__EventsWrapper__) */
