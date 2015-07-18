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

#ifdef __KIWI_JUCE_WRAPPER__

#include "KiwiGuiJuceView.h"
#include "KiwiGuiJuceDevice.h"

namespace Kiwi
{	
	// ================================================================================ //
	//										JVIEW										//
	// ================================================================================ //

    jView::jView(sjGuiDeviceManager device, sGuiController ctrl) noexcept : GuiView(ctrl),
    m_device(device)
    {
        boundsChanged();
        setWantKeyboard(wantKeyboard());
        setWantActions(wantActions());
    }
    
    jView::~jView()
    {
        sjGuiDeviceManager mng = m_device.lock();
        if(mng && wantActions())
        {
            this->removeKeyListener(mng->getKeyMappings());
        }
    }
    
    void jView::redraw()
    {
        const MessageManagerLock thread(Thread::getCurrentThread());
        if(thread.lockWasGained())
        {
            repaint();
        }
    }
    
    void jView::boundsChanged()
    {
        const auto bounds = GuiView::getBounds();
        const MessageManagerLock thread(Thread::getCurrentThread());
        if(thread.lockWasGained())
        {
            Component::setBounds(int(bounds.x()), int(bounds.y()), int(bounds.width()), int(bounds.height()));
        }
    }
    
    void jView::positionChanged()
    {
        const auto bounds = GuiView::getBounds();
        const MessageManagerLock thread(Thread::getCurrentThread());
        if(thread.lockWasGained())
        {
            Component::setBounds(int(bounds.x()), int(bounds.y()), int(bounds.width()), int(bounds.height()));
        }
    }
    
    void jView::sizeChanged()
    {
        const auto bounds = GuiView::getBounds();
        const MessageManagerLock thread(Thread::getCurrentThread());
        if(thread.lockWasGained())
        {
            Component::setBounds(int(bounds.x()), int(bounds.y()), int(bounds.width()), int(bounds.height()));
        }
    }
    
    void jView::setWantKeyboard(const bool wanted)
    {
        Component::setWantsKeyboardFocus(wanted);
    }
    
    void jView::setWantActions(const bool wanted)
    {
        sjGuiDeviceManager mng = m_device.lock();
        if(mng && wantActions())
        {
            mng->registerAllCommandsForTarget(this);
            this->addKeyListener(mng->getKeyMappings());
        }
    }
    
    void jView::setMouseCursor(Kiwi::MouseCursor const& cursor)
    {
        juce::MouseCursor mc;
        switch(cursor.type())
        {
            case Kiwi::MouseCursor::Type::Default                 : mc = juce::MouseCursor::ParentCursor; break;
            case Kiwi::MouseCursor::Type::None                    : mc = juce::MouseCursor::NoCursor; break;
            case Kiwi::MouseCursor::Type::Arrow                   : mc = juce::MouseCursor::NormalCursor; break;
            case Kiwi::MouseCursor::Type::Wait                    : mc = juce::MouseCursor::WaitCursor; break;
            case Kiwi::MouseCursor::Type::Ibeam                   : mc = juce::MouseCursor::IBeamCursor; break;
            case Kiwi::MouseCursor::Type::Crosshair               : mc = juce::MouseCursor::CrosshairCursor; break;
            case Kiwi::MouseCursor::Type::Copying                 : mc = juce::MouseCursor::CopyingCursor; break;
            case Kiwi::MouseCursor::Type::HandPointing            : mc = juce::MouseCursor::PointingHandCursor; break;
            case Kiwi::MouseCursor::Type::HandDragging            : mc = juce::MouseCursor::DraggingHandCursor; break;
            case Kiwi::MouseCursor::Type::ResizingLeftRight       : mc = juce::MouseCursor::LeftRightResizeCursor; break;
            case Kiwi::MouseCursor::Type::ResizingUpDown          : mc = juce::MouseCursor::UpDownResizeCursor; break;
            case Kiwi::MouseCursor::Type::ResizingFourWay         : mc = juce::MouseCursor::UpDownLeftRightResizeCursor; break;
            case Kiwi::MouseCursor::Type::ResizingTop             : mc = juce::MouseCursor::TopEdgeResizeCursor; break;
            case Kiwi::MouseCursor::Type::ResizingBottom          : mc = juce::MouseCursor::BottomEdgeResizeCursor; break;
            case Kiwi::MouseCursor::Type::ResizingLeft            : mc = juce::MouseCursor::LeftEdgeResizeCursor; break;
            case Kiwi::MouseCursor::Type::ResizingRight           : mc = juce::MouseCursor::RightEdgeResizeCursor; break;
            case Kiwi::MouseCursor::Type::ResizingTopLeft         : mc = juce::MouseCursor::TopLeftCornerResizeCursor; break;
            case Kiwi::MouseCursor::Type::ResizingTopRight        : mc = juce::MouseCursor::TopRightCornerResizeCursor; break;
            case Kiwi::MouseCursor::Type::ResizingBottomLeft      : mc = juce::MouseCursor::BottomLeftCornerResizeCursor; break;
            case Kiwi::MouseCursor::Type::ResizingBottomRight     : mc = juce::MouseCursor::BottomRightCornerResizeCursor; break;
            default: break;
        }
        
        Component::setMouseCursor(mc);
    }
    
    void jView::grabFocus()
    {
        const MessageManagerLock thread(Thread::getCurrentThread());
        if(thread.lockWasGained())
        {
            grabKeyboardFocus();
        }
    }
    
    void jView::toBack()
    {
        const MessageManagerLock thread(Thread::getCurrentThread());
        if(thread.lockWasGained())
        {
            Component::toBack();
        }
    }
    
    void jView::toFront()
    {
        const MessageManagerLock thread(Thread::getCurrentThread());
        if(thread.lockWasGained())
        {
            Component::toFront(false);
        }
    }
    
    void jView::alwaysOnTop(const bool alwaysOnTop)
    {
        const MessageManagerLock thread(Thread::getCurrentThread());
        if(thread.lockWasGained())
        {
            Component::setAlwaysOnTop(alwaysOnTop);
        }
    }
    
    void jView::addChildView(sGuiView child)
    {
        if(child)
        {
            sjView jchild = dynamic_pointer_cast<jView>(child);
            if(jchild)
            {
                const MessageManagerLock thread(Thread::getCurrentThread());
                if(thread.lockWasGained())
                {
                    addAndMakeVisible(jchild.get());
                    jchild->setBounds(toJuce<int>(jchild->GuiView::getBounds()));
                    sGuiController ctrl = child->getController();
                    if(ctrl->wantKeyboard())
                    {
                        jchild->grabKeyboardFocus();
                    }
                }
            }
        }
    }
    
    void jView::removeChildView(sGuiView child)
    {
        if(child)
        {
            sjView jchild = dynamic_pointer_cast<jView>(child);
            if(jchild)
            {
                removeChildComponent(jchild.get());
            }
        }
    }
    
    void jView::addToDesktop()
    {
        const MessageManagerLock thread(Thread::getCurrentThread());
        if(thread.lockWasGained())
        {
            Component::setVisible(true);
            Component::addToDesktop(ComponentPeer::windowHasDropShadow);
            Component::toFront(true);
        }
    }
    
    void jView::removeFromDesktop()
    {
        const MessageManagerLock thread(Thread::getCurrentThread());
        if(thread.lockWasGained())
        {
            Component::setVisible(false);
            Component::removeFromDesktop();
        }
    }
    
    void jView::setMinimize(const bool state)
    {
        ComponentPeer* const peer = getPeer();
        if(peer)
        {
            if(peer->isMinimised() != state)
            {
                peer->setMinimised(state);
            }
        }
    }
    
    void jView::paint(Graphics& g)
    {
        jSketch d(g);
        draw(d);
    }
    
    void jView::paintOverChildren(Graphics& g)
    {
        jSketch d(g);
        drawOver(d);
    }
    
    void jView::mouseDown(const juce::MouseEvent& e)
    {
        receive(jEventMouse(MouseEvent::Type::Down, e));
    }
    
    void jView::mouseDrag(const juce::MouseEvent& e)
    {
        receive(jEventMouse(MouseEvent::Type::Drag, e));
    }
    
    void jView::mouseUp(const juce::MouseEvent& e)
    {
        receive(jEventMouse(MouseEvent::Type::Up, e));
    }
    
    void jView::mouseMove(const juce::MouseEvent& e)
    {
        receive(jEventMouse(MouseEvent::Type::Move, e));
    }
    
    void jView::mouseEnter(const juce::MouseEvent& e)
    {
        receive(jEventMouse(MouseEvent::Type::Enter, e));
    }
    
    void jView::mouseExit(const juce::MouseEvent& e)
    {
        receive(jEventMouse(MouseEvent::Type::Leave, e));
    }
    
    void jView::mouseDoubleClick(const juce::MouseEvent& e)
    {
        receive(jEventMouse(MouseEvent::Type::DoubleClick, e));
    }
    
    void jView::mouseWheelMove(const juce::MouseEvent& event, const MouseWheelDetails& wheel)
    {
        if(!receive(jEventMouse(event, wheel)))
        {
            sjView parent = static_pointer_cast<jView>(getParent());
            if(parent)
            {
                parent->receive(jEventMouse(event.getEventRelativeTo(parent.get()), wheel));
            }
        }
    }
    
    void jView::focusGained(FocusChangeType cause)
    {
        receive(KeyboardFocusIn);
    }
    
    void jView::focusLost(FocusChangeType cause)
    {
        receive(KeyboardFocusOut);
    }
    
    bool jView::keyPressed(const KeyPress& key)
    {
        return receive(KeyboardEvent(key.getKeyCode(), (long)key.getModifiers().getRawFlags(), key.getTextCharacter()));
    }
    
    bool jView::hitTest(int x, int y)
    {
        //return GuiView::contains(Point(double(x + getX()), double(y + getY())));
        return GuiView::hitTest(Point(x, y));
    }

    ApplicationCommandTarget* jView::getNextCommandTarget()
    {
        return findFirstTargetParentComponent();
    }
    
    void jView::getAllCommands(Array <CommandID>& commands)
    {
        vector<ulong> codes = getActionCodes();
        for(vector<ulong>::size_type i = 0; i < codes.size(); i++)
        {
            commands.add(CommandID(codes[i]));
        }
    }
    
    void jView::getCommandInfo(const CommandID commandID, ApplicationCommandInfo& result)
    {
        Action action = getAction(ulong(commandID));
        result.setInfo(translate(action.name), translate(action.description), action.category, 0);
        result.addDefaultKeypress(action.event.getCharacter(), action.event.getModifiers());
        //result.setActive(getDesktopWindowStyleFlags() & DocumentWindow::closeButton);
    }
    
    bool jView::perform(const InvocationInfo& info)
    {
        return performAction(ulong(info.commandID));
    }
    
    bool jViewTextInput::isTextInputActive() const
    {
        return true;
    }
    
    Range<int> jViewTextInput::getHighlightedRegion() const {return Range<int>();}
    
    void jViewTextInput::setHighlightedRegion (const Range<int>& newRange) {}
    
    void jViewTextInput::setTemporaryUnderlining (const Array <Range<int> >& underlinedRegions) {}
    
    String jViewTextInput::getTextInRange (const Range<int>& range) const {return String();}
    
    void jViewTextInput::insertTextAtCaret (const String& textToInsert) {cout << textToInsert.toStdString() << endl;}
    
    juce::Rectangle<int> jViewTextInput::getCaretRectangle() {return juce::Rectangle<int>();};
}

#endif
