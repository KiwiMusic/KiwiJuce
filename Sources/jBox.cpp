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
	
	jBox::jBox(sBox box) : Box::Controller(box)
    {
		const Gui::Rectangle bounds = Box::Controller::getBounds(getPagePresentationStatus());
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
		if (sBox box = getBox())
		{
			const bool locked					= !getPageEditionStatus();
			const bool pagePresentationStatus	= getPagePresentationStatus();
			const bool boxPresentationStatus	= box->isInPresentation();
			const bool invisible				= (locked && box->isHiddenOnLock()) || (pagePresentationStatus && !boxPresentationStatus);
			const bool acceptClick				= !invisible && locked && !box->getIgnoreClick();
			
			if(isVisible() && invisible)
			{
				setVisible(false);
			}
			else if(!isVisible() && !invisible)
			{
				setVisible(true);
			}
			else
			{
				redraw();
			}
			
			setInterceptsMouseClicks(acceptClick, acceptClick);
			
			if(isKeyboardListener())
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
		const Gui::Point pt = Box::Controller::getPosition(getPagePresentationStatus());
        setTopLeftPosition(round(pt.x()), round(pt.y()));
    }
    
    void jBox::sizeChanged()
    {
		const Gui::Point pt = Box::Controller::getSize(getPagePresentationStatus());
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
	
	void jBox::pageEditionStatusChanged()
	{
		checkVisibilityAndInteractionMode();
	}
	
	void jBox::pagePresentationStatusChanged()
	{
		checkVisibilityAndInteractionMode();
	}
	
    void jBox::paint(Graphics& g)
    {
        if(isVisible())
        {
			g.beginTransparencyLayer(1);
			double frameSize = getFrameSize() * 0.5;
			juce::Rectangle<int> boxFrame = getLocalBounds().reduced(frameSize).withPosition(0, 0);
			g.setOrigin(frameSize, frameSize);
			g.reduceClipRegion(boxFrame);
			JDoodle d(g, boxFrame);
			Box::Controller::paintBox(getBox(), d);
			g.endTransparencyLayer();

			JDoodle d2(g, getLocalBounds());
			Box::Controller::paintBoxFrame(getBox(), d2, isSelected(), getPageEditionStatus(), getPagePresentationStatus());
        }
    }

    void jBox::mouseDown(const MouseEvent& e)
    {
		if(isMouseListener())
			getBox()->receive(jEventMouse(Gui::Event::Mouse::Type::Down, e));
    }
    
    void jBox::mouseDrag(const MouseEvent& e)
    {
		if(isMouseListener())
			getBox()->receive(jEventMouse(Gui::Event::Mouse::Type::Drag, e));
    }
    
    void jBox::mouseUp(const MouseEvent& e)
    {
		if(isMouseListener())
			getBox()->receive(jEventMouse(Gui::Event::Mouse::Type::Up, e));
    }
    
    void jBox::mouseMove(const MouseEvent& e)
    {
		if(isMouseListener())
			getBox()->receive(jEventMouse(Gui::Event::Mouse::Type::Move, e));
    }
    
    void jBox::mouseEnter(const MouseEvent& e)
    {
		if(isMouseListener())
			getBox()->receive(jEventMouse(Gui::Event::Mouse::Type::Enter, e));
    }
    
    void jBox::mouseExit(const MouseEvent& e)
    {
		if(isMouseListener())
			getBox()->receive(jEventMouse(Gui::Event::Mouse::Type::Leave, e));
    }
    
    void jBox::mouseDoubleClick(const MouseEvent& e)
    {
		if(isMouseListener())
			getBox()->receive(jEventMouse(Gui::Event::Mouse::Type::DoubleClick, e));
    }
    
    void jBox::mouseWheelMove(const MouseEvent& event, const MouseWheelDetails& wheel)
    {
		if(isMouseListener())
			getBox()->receive(jEventMouse(event, wheel));
    }
    
    void jBox::focusGained(FocusChangeType cause)
    {
        getBox()->receive(Gui::Event::Focus::In);
    }
    
    void jBox::focusLost(FocusChangeType cause)
    {
        getBox()->receive(Gui::Event::Focus::Out);
    }
    
    bool jBox::keyPressed(const KeyPress& key)
    {
        return getBox()->receive(jEventKeyboard(key));
    }
}

