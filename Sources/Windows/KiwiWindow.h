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

#ifndef __DEF_KIWI_WINDOW__
#define __DEF_KIWI_WINDOW__

#include "../KiwiModules/KiwiModules.h"
#include "../Wrapper/KiwiGuiJuceDefine.h"

namespace Kiwi
{
    
    // ================================================================================ //
    //                                      WINDOW                                      //
    // ================================================================================ //
    
    class jWindow : public DocumentWindow
    {
    private:
        
    protected:
        //int getDesktopWindowStyleFlags() const override;
        
    public:
        jWindow(string const& name,
                Kiwi::Color bgcolor = Kiwi::Colors::grey,
                int buttons = DocumentWindow::allButtons,
                const bool visible = true);
        
        ~jWindow();
    };
    
    typedef shared_ptr<jWindow>	sjWindow;
}


#endif

