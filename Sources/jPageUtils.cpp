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

#include "jPageUtils.h"

namespace Kiwi
{
    // ================================================================================ //
    //										JLASSO                                      //
    // ================================================================================ //
    jLasso::jLasso(sPage page) : Lasso(page)
    {
        setInterceptsMouseClicks(false, false);
        setWantsKeyboardFocus(false);
        setVisible(false);
    }
    
    jLasso::~jLasso()
    {
        
    }
    
    void jLasso::boundsHasChanged()
    {
        const Gui::Rectangle bounds = Lasso::getBounds();
        setBounds(bounds.x(), bounds.y(), bounds.width(), bounds.height());
    }
    
    void jLasso::paint(Graphics& g)
    {
        JDoodle d(g, Component::getBounds());
        draw(d);
    }
    
    // ================================================================================ //
	//                                 JIO HIGHLIGHTER									//
	// ================================================================================ //
	
	jIoletHighlighter::jIoletHighlighter()
	{
		setInterceptsMouseClicks(false, false);
		setWantsKeyboardFocus(false);
	}
    
    jIoletHighlighter::~jIoletHighlighter()
	{
		;
	}
    
    void jIoletHighlighter::boundsHasChanged()
    {
        const Gui::Rectangle bounds = IoletHighlighter::getBounds();
        setBounds(bounds.x(), bounds.y(), bounds.width(), bounds.height());
    }
	
	void jIoletHighlighter::paint(Graphics& g)
	{
		JDoodle d(g, Component::getBounds());
        draw(d);
	}
}

