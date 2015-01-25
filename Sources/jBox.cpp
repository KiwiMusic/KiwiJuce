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

#include "jBox.h"
#include "Application.h"

namespace Kiwi
{	
	// ================================================================================ //
	//                                  JBOX CONTROLER                                  //
	// ================================================================================ //
	
	jBox::jBox(sBox box, sPageView pageview) : BoxView(box, pageview)
    {
		const Gui::Rectangle bounds = BoxView::getBounds();
		setBounds(bounds.x(), bounds.y(), bounds.width(), bounds.height());
        setInterceptsMouseClicks(false, false);
        setWantsKeyboardFocus(false);
		setMouseClickGrabsKeyboardFocus(false);
    }
    
    jBox::~jBox()
    {
        ;
    }
	
	void jBox::checkVisibilityAndInteractionMode()
	{
        sBox box = getBox();
		if(box)
		{
            if(Component::isVisible() && !BoxView::isVisible())
			{
				setVisible(false);
			}
			else if(!Component::isVisible() && !!BoxView::isVisible())
			{
				setVisible(true);
			}
			else
			{
				redraw();
			}
			
            const bool acceptClick = BoxView::isVisible() && getPageLockStatus() && !box->getIgnoreClick();
			setInterceptsMouseClicks(acceptClick, acceptClick);
			
            Gui::sKeyboarder keyboarder = dynamic_pointer_cast<Gui::Keyboarder>(box);
			if(keyboarder)
			{
				setWantsKeyboardFocus(acceptClick);
				setMouseClickGrabsKeyboardFocus(acceptClick);
			}
		}
	}
    
    void jBox::redraw()
    {
        const MessageManagerLock thread(Thread::getCurrentThread());
        if(thread.lockWasGained())
        {
			repaint();
        }
	}
    
    void jBox::grabKeyboardFocus()
    {
        juce::Component::grabKeyboardFocus();
    }
	
    void jBox::positionChanged()
    {
		const Gui::Point pt = BoxView::getPosition();
        setTopLeftPosition(round(pt.x()), round(pt.y()));
    }
    
    void jBox::sizeChanged()
    {
		const Gui::Point pt = BoxView::getSize();
        setSize(round(pt.x()), round(pt.y()));
    }
	
	void jBox::selectionStatusChanged()
	{
		redraw();
	}
	
	void jBox::presentationStatusChanged()
	{
		checkVisibilityAndInteractionMode();
	}
	
	void jBox::pageLockStatusChanged()
	{
		checkVisibilityAndInteractionMode();
	}
	
	void jBox::pagePresentationStatusChanged()
	{
		checkVisibilityAndInteractionMode();
	}
	
    void jBox::paint(Graphics& g)
    {
        if(Component::isVisible())
        {
			g.beginTransparencyLayer(1);
            /*
			//double frameSize = getFrameSize() * 0.5;
			juce::Rectangle<int> boxFrame = getLocalBounds().reduced(frameSize).withPosition(0, 0);
			g.setOrigin(frameSize, frameSize);
			g.reduceClipRegion(boxFrame);
			JDoodle d(g, boxFrame);
			//BoxView::paintBox(getBox(), d);
			g.endTransparencyLayer();
             */
			JDoodle d2(g, getLocalBounds());
			//BoxView::paintBoxFrame(getBox(), d2, isSelected(), getPageEditionStatus(), getPagePresentationStatus());
        }
    }

    void jBox::mouseDown(const MouseEvent& e)
    {
        Gui::sMouser box = dynamic_pointer_cast<Gui::Mouser>(getBox());
        if(box)
        {
            box->receive(jEventMouse(Gui::Event::Mouse::Type::Down, e));
        }
    }
    
    void jBox::mouseDrag(const MouseEvent& e)
    {
        Gui::sMouser box = dynamic_pointer_cast<Gui::Mouser>(getBox());
        if(box)
        {
            box->receive(jEventMouse(Gui::Event::Mouse::Type::Drag, e));
        }
    }
    
    void jBox::mouseUp(const MouseEvent& e)
    {
        Gui::sMouser box = dynamic_pointer_cast<Gui::Mouser>(getBox());
        if(box)
        {
            box->receive(jEventMouse(Gui::Event::Mouse::Type::Up, e));
        }
    }
    
    void jBox::mouseMove(const MouseEvent& e)
    {
        Gui::sMouser box = dynamic_pointer_cast<Gui::Mouser>(getBox());
        if(box)
        {
			box->receive(jEventMouse(Gui::Event::Mouse::Type::Move, e));
        }
    }
    
    void jBox::mouseEnter(const MouseEvent& e)
    {
        Gui::sMouser box = dynamic_pointer_cast<Gui::Mouser>(getBox());
        if(box)
        {
            box->receive(jEventMouse(Gui::Event::Mouse::Type::Enter, e));
        }
    }
    
    void jBox::mouseExit(const MouseEvent& e)
    {
        Gui::sMouser box = dynamic_pointer_cast<Gui::Mouser>(getBox());
        if(box)
        {
            box->receive(jEventMouse(Gui::Event::Mouse::Type::Leave, e));
        }
    }
    
    void jBox::mouseDoubleClick(const MouseEvent& e)
    {
        Gui::sMouser box = dynamic_pointer_cast<Gui::Mouser>(getBox());
        if(box)
        {
            box->receive(jEventMouse(Gui::Event::Mouse::Type::Move, e));
        }
    }
    
    void jBox::mouseWheelMove(const MouseEvent& event, const MouseWheelDetails& wheel)
    {
        Gui::sMouser box = dynamic_pointer_cast<Gui::Mouser>(getBox());
        if(box)
        {
            box->receive(jEventMouse(event, wheel));
        }
    }
    
    void jBox::focusGained(FocusChangeType cause)
    {
        Gui::sKeyboarder box = dynamic_pointer_cast<Gui::Keyboarder>(getBox());
        box->receive(Gui::Event::In);
    }
    
    void jBox::focusLost(FocusChangeType cause)
    {
        Gui::sKeyboarder box = dynamic_pointer_cast<Gui::Keyboarder>(getBox());
        box->receive(Gui::Event::Out);
    }
    
    bool jBox::keyPressed(const KeyPress& key)
    {
        Gui::sKeyboarder box = dynamic_pointer_cast<Gui::Keyboarder>(getBox());
        return box->receive(jEventKeyboard(key));
    }
}

