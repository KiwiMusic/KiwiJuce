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

#ifndef __DEF_KIWI_JEVENT__
#define __DEF_KIWI_JEVENT__

#include "jDefs.h"

namespace Kiwi
{
	class jEventMouse : public Kiwi::Mouser::Event
    {
    private:
        const MouseInputSource m_source;
    public:
        
        jEventMouse(Type const& type, MouseEvent const& event) noexcept :
		Kiwi::Mouser::Event(type, event.x, event.y, event.mods.getRawFlags(), 0., 0., event.mouseWasClicked(), event.getMouseDownPosition().x, event.getMouseDownPosition().y, event.getNumberOfClicks()),
        m_source(event.source)
        {
            ;
        }
        
        jEventMouse(MouseEvent const& event, MouseWheelDetails const& wheel) noexcept :
        Kiwi::Mouser::Event(Kiwi::Mouser::Event::Type::Wheel, event.x, event.y, event.mods.getRawFlags(), wheel.deltaX, wheel.deltaY, event.mouseWasClicked(), event.getMouseDownPosition().x, event.getMouseDownPosition().y, event.getNumberOfClicks()),
        m_source(event.source)
        {
            ;
        }
        
        ~jEventMouse()
        {
            ;
        }
        
        void setMouseUnlimited(bool isLimited, bool visibleUntilLimits = false) const override;
    };

	class jEventKeyboard : public Kiwi::Keyboarder::Event
    {
    public:
        
        jEventKeyboard(KeyPress const& key) noexcept :
		Kiwi::Keyboarder::Event(key.getTextCharacter(), (long)key.getModifiers().getRawFlags())
        {
            ;
        }
    
        ~jEventKeyboard()
        {
            ;
        }
    };
}

#endif
