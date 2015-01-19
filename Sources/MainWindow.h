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

#ifndef __DEF_KIWI_MAIN_WINDOW__
#define __DEF_KIWI_MAIN_WINDOW__

#include "Wrapper.h"
#include "jPage.h"

namespace Kiwi
{
    // ================================================================================ //
    //                                  MAIN WINDOW                                  //
    // ================================================================================ //
    
    //! The main window.
    /**
     The main window.
     */
    class BaseWindow  : public DocumentWindow, public ApplicationCommandTarget
    {
    public:
        
        //! Constructor.
        /** Create a new base window.
         @param name  The title of the window.
         @param color The background colour.
         @param buttons The buttons to show on the title bar.
         @param addToDesktop If true, the window will be automatically added to the desktop.
         */
        BaseWindow(String const& name = "untitled", Colour color = Colour(0xffdddddd), int buttons = allButtons, bool addToDesktop = true);
        
        //! Destructor.
        /** Delete the base window.
         */
        ~BaseWindow();
        
        //! Try to close the window.
        /** This function is called when the user tries to close the window.
         */
        virtual void closeButtonPressed() override;
        
        // ================================================================================ //
        //                              APPLICATION COMMAND TARGET                          //
        // ================================================================================ //
        
        virtual ApplicationCommandTarget* getNextCommandTarget() override;
        virtual void getAllCommands (Array <CommandID>& commands) override;
        virtual void getCommandInfo (const CommandID commandID, ApplicationCommandInfo& result) override;
        virtual bool perform (const InvocationInfo& info) override;
    };
    
    // ================================================================================ //
    //                                  PAGE WINDOW                                     //
    // ================================================================================ //
    
    //! The page window.
    /*
     The page window
     */
    //class MainWindow : public ResizableWindow
	class MainWindow : public BaseWindow
    {
    private:
        
    public:
        
        //! Constructor.
        /** You should never have to use this function.
         */
        MainWindow();
        
        //! Destructor.
        /** You should never have to use this function.
         */
        ~MainWindow();
        
        //! The instance component creation method.
        /** The function allocates an instance component.
         @return The instance component.
         */
        static shared_ptr<MainWindow> create();
        
        //! The instance component creation method.
        /** The function allocates an instance component.
         @return The instance component.
         */
        void addPage(sjPage page);
    };
}



#endif
