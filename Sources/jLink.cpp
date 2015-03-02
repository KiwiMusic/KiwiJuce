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
    
	jLink::jLink(sGuiLink link) : LinkView(link)
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
        Kiwi::Rectangle bounds = LinkView::getBounds().expanded(20.);
        setBounds(bounds.x(), bounds.y(), bounds.width(), bounds.height());
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

