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
    }
    
    jWindow::~jWindow()
    {
        
    }
}



