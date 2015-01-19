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

#ifndef __DEF_KIWI_JPAGE_UTILS__
#define __DEF_KIWI_JPAGE_UTILS__

#include "jBox.h"
#include "jLink.h"

namespace Kiwi
{
    // ================================================================================ //
    //										JLASSO                                      //
    // ================================================================================ //
    
    class jLasso : public Lasso, public Component
    {
    public:
        //! Contructor.
        /** You should never have to use this method.
         */
        jLasso(sPage page);
        
        //! Destrcutor.
        /** You should never have to use this method.
         */
        ~jLasso();
        
        //! Retrieve the notification that the bounds has changed.
        /** The function retrieves the notification that the bounds has changed.
         */
        void boundsHasChanged() override;
    
        //! The juce paint method.
        /** The function paints in a graphics.
         */
        void paint(Graphics& g) override;
    };
    
    typedef shared_ptr<jLasso>          sjLasso;
    typedef weak_ptr<jLasso>            wjLasso;
    typedef shared_ptr<const jLasso>    scjLasso;
    typedef weak_ptr<const jLasso>		wcjLasso;
    
    
    // ================================================================================ //
    //                                 JIOLET HIGHLIGHTER                               //
    // ================================================================================ //
    
    class jIoletHighlighter : public IoletHighlighter, public Component
    {
    public:
        //! Contructor.
        /** You should never have to use this method.
         */
        jIoletHighlighter();
        
        //! Destrcutor.
        /** You should never have to use this method.
         */
        ~jIoletHighlighter();
        
        //! Retrieve the notification that the bounds has changed.
        /** The function retrieves the notification that the bounds has changed.
         */
        void boundsHasChanged() override;
        
        //! The juce paint method.
        /** The function paints in a graphics.
         */
        void paint(Graphics& g) override;
        
        //! The juce hit test method.
        /** The function test if the component has been hitted.
         */
        bool hitTest(int x, int y) override
        {
            return false;
        }
    };
    
    typedef shared_ptr<jIoletHighlighter>        sjIoletHighlighter;
    typedef weak_ptr<jIoletHighlighter>          wjIoletHighlighter;
    typedef shared_ptr<const jIoletHighlighter>  scjIoletHighlighter;
    typedef weak_ptr<const jIoletHighlighter>    wcjIoletHighlighter;
}



#endif
