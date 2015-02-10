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

namespace Kiwi
{	
	// ================================================================================ //
	//										JBOX										//
	// ================================================================================ //
	
	jBox::jBox(sBox box, sPageView pageview) : BoxView(box, pageview),
	m_framesize(2)
    {
		setBounds(toJuce<int>(getDisplayBounds()));
        setInterceptsMouseClicks(false, true);
        setWantsKeyboardFocus(false);
		setMouseClickGrabsKeyboardFocus(false);
    }
	
    jBox::~jBox()
    {
		;
    }
	
	void jBox::init()
	{
		Gui::sWriter writer = dynamic_pointer_cast<Gui::Writer>(getBox());
		if (writer)
		{
			Gui::Writer::sTextField textfield = writer->getTextField();
			if (textfield)
			{
				m_label = make_shared<jLabel>(textfield);
				
				if (m_label)
				{
					m_label->setBounds(toJuce<int>(BoxView::getBounds()).withPosition(m_framesize, m_framesize));
					addAndMakeVisible(m_label.get());
					textfield->addView(m_label);
				}
			}
		}
	}
	
	Gui::Rectangle jBox::getDisplayBounds() const noexcept
	{
		return BoxView::getBounds().expanded(m_framesize);
	}

	Gui::Point jBox::getDisplayPosition() const noexcept
	{
		return BoxView::getPosition() - m_framesize;
	}

	Gui::Point jBox::getDisplaySize() const noexcept
	{
		return BoxView::getSize() + m_framesize * 2;
	}
	
	ulong jBox::resizerKnock(Gui::Point const& pt) const noexcept
	{
		ulong borderFlag = Knock::BorderZone::None;
		const bool growy = (m_label != nullptr);
		
		if(isSelected())
		{
			const Gui::Point localPoint = pt - getDisplayPosition();
			if(localPoint.y() <= m_framesize*2)
			{
				borderFlag |= Knock::BorderZone::Top;
			}
			if(localPoint.x() >= getDisplaySize().x() - m_framesize*2)
			{
				borderFlag |= Knock::BorderZone::Right;
			}
			if(localPoint.y() >= getDisplaySize().y() - m_framesize*2)
			{
				borderFlag |= Knock::BorderZone::Bottom;
			}
			if(localPoint.x() <= m_framesize*2)
			{
				borderFlag |= Knock::BorderZone::Left;
			}
			
			if(growy)
			{
				if((borderFlag & Knock::BorderZone::Top || borderFlag & Knock::BorderZone::Bottom))
				{
					if(localPoint.x() <= m_framesize*4)
					{
						borderFlag |= Knock::BorderZone::Left;
					}
					else if(localPoint.x() >= getDisplaySize().x() - m_framesize*4)
					{
						borderFlag |= Knock::BorderZone::Right;
					}
					
					borderFlag &= ~Knock::BorderZone::Top;
					borderFlag &= ~Knock::BorderZone::Bottom;
				}
			}
		}
		
		return borderFlag;
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
	
	void jBox::textFieldEditorShown()
	{
		;
	}
	
	void jBox::textfieldTextChanged()
	{
		if (m_label)
		{
			if (m_label->isBeingEdited())
			{
				String text = m_label->getText(true);
				Gui::Font font = getBox()->getFont();
				juce::Font jfont(font.getName(), (float)font.getSize(), font.getStyle());
				double textwidth = jfont.getStringWidthFloat(text);
				getBox()->setAttributeValue(AttrBox::Tag_size, {textwidth + 10, getSize().y()});
			}
		}
	}
	
    void jBox::grabKeyboardFocus()
    {
		if (m_label)
		{
			m_label->showEditor();
		}
		else
		{
			juce::Component::grabKeyboardFocus();
		}
    }
	
    void jBox::positionChanged()
    {
        setTopLeftPosition(toJuce<int>(getDisplayPosition()));
		
		if (m_label)
		{
			m_label->setBounds(toJuce<int>(BoxView::getBounds()).withPosition(m_framesize, m_framesize));
		}
    }
    
    void jBox::sizeChanged()
    {
		const Gui::Point pt = getDisplaySize();
        setSize(round(pt.x()), round(pt.y()));
		
		if (m_label)
		{
			m_label->setBounds(toJuce<int>(BoxView::getBounds()).withPosition(m_framesize, m_framesize));
		}
    }
	
	void jBox::pageViewSelectionStatusChanged()
	{
		redraw();
	}
	
	void jBox::presentationStatusChanged()
	{
		checkVisibilityAndInteractionMode();
	}
	
	void jBox::pageViewLockStatusChanged()
	{
		checkVisibilityAndInteractionMode();
	}
	
	void jBox::pageViewPresentationStatusChanged()
	{
		checkVisibilityAndInteractionMode();
	}
	
    void jBox::paint(Graphics& g)
    {
        if(Component::isVisible())
        {
			sBox box = getBox();
			if (box)
			{
				const bool edit = !getPageLockStatus();
				const bool presentation = getPagePresentationStatus();
				
				const Gui::Rectangle localBoxFrame = getDisplayBounds().withZeroOrigin();
				const Gui::Rectangle localBoxBounds = BoxView::getBounds().withPosition(Gui::Point(m_framesize, m_framesize));
				
				JDoodle d(g, localBoxFrame);

				Gui::sSketcher sketcher = dynamic_pointer_cast<Gui::Sketcher>(box);
				if (sketcher)
				{
					const Rectangle<int> jlocalBoxBounds = toJuce<int>(localBoxBounds.withZeroOrigin());
					g.beginTransparencyLayer(1);
					
					JDoodle d(g, jlocalBoxBounds);
					g.setOrigin(m_framesize, m_framesize);
					g.reduceClipRegion(jlocalBoxBounds);
					
					sketcher->draw(d);
					
					g.endTransparencyLayer();
				}
				else
				{
					const double borderSize = 1.;
					d.setColor(box->getBackgroundColor());
					d.fillRectangle(localBoxBounds);
					
					d.setColor(box->getBorderColor());
					d.drawRectangle(localBoxBounds.reduced(borderSize*0.5), borderSize);
					
					d.setColor(box->getTextColor());
					d.drawText(toString(box->getText()), 3 + m_framesize, m_framesize, localBoxBounds.width(), localBoxBounds.height(), box->getFontJustification());
				}
			 
				//Paint Box frame :
				if(edit)
				{
					const bool growy = (m_label != nullptr);
					const Gui::Color ioColor = Gui::Color(0.3, 0.3, 0.3);
					const Gui::Color presentationColor = Gui::Color(0., 0.8, 0.);
					const Gui::Color selectionColor = presentation ? presentationColor : Gui::Color(0., 0.6, 0.9);
					
					if(isSelected())
					{
						if (growy)
						{
							d.setColor(selectionColor);
							// left
							d.fillRectangle(0, 0, m_framesize*4, m_framesize);
							d.fillRectangle(0, m_framesize, m_framesize, localBoxFrame.height() - m_framesize*2);
							d.fillRectangle(0, localBoxFrame.height() - m_framesize, m_framesize*4, m_framesize);
							// right
							d.fillRectangle(localBoxFrame.width() - m_framesize*4, 0, m_framesize*4, m_framesize);
							d.fillRectangle(localBoxFrame.width() - m_framesize, m_framesize, m_framesize, localBoxFrame.height() - m_framesize*2);
							d.fillRectangle(localBoxFrame.width() - m_framesize*4, localBoxFrame.height() - m_framesize, m_framesize*4, m_framesize);
						}
						else
						{
							d.setColor(selectionColor);
							d.drawRectangle(localBoxFrame.reduced(m_framesize*0.5), m_framesize);
							
							d.setColor(selectionColor.darker(0.1));
							d.drawRectangle(localBoxFrame.reduced(0.5), 1);
						}
					}
					else if(!presentation)
					{
						const ulong ninlets = box->getNumberOfInlets();
						const ulong noutlets= box->getNumberOfOutlets();
						
						d.setColor(ioColor);
						for(ulong i = 1; i <= ninlets; i++)
						{
							d.fillRectangle(getInletBounds(i) - getDisplayPosition());
						}
						
						for(ulong i = 1; i <= noutlets; i++)
						{
							d.fillRectangle(getOutletBounds(i) - getDisplayPosition());
						}
						
						if (isIncludeInPresentation())
						{
							d.setColor(presentationColor.withAlpha(0.2));
							d.drawRectangle(localBoxBounds, 3);
						}
					}
				}
			}
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

