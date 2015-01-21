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

#include "jLink.h"
#include "Application.h"

namespace Kiwi
{
    
	jLink::jLink(sLink link) : LinkView(link)
    {
        setInterceptsMouseClicks(false, false);
        boundsChanged();
    }
    
    jLink::~jLink()
    {
    }
    
    void jLink::redraw()
    {
        repaint();
    }
    
    void jLink::boundsChanged()
    {
        Gui::Rectangle bounds = getLink()->getBounds();
        setBounds(bounds.x() - 10., bounds.y() - 10., bounds.width() + 20., bounds.height() + 20.);
    }

    void jLink::paint(Graphics& g)
    {
        if(isVisible())
        {
            JDoodle d(g, getLocalBounds());
            LinkView::paint(getLink(), d, isSelected());
        }
    }
}

