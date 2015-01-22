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

#include "jPage.h"
#include "Application.h"

namespace Kiwi
{
    sDico createBoxDicoAtPosition(string const& name, juce::Point<int> const& pt)
    {
        sDico dico = Dico::evaluateForBox(name);
        if(dico)
        {
            sDico sub = dico->get(Tag::List::boxes);
            if(sub)
            {
                sub = sub->get(Tag::List::box);
                if(sub)
                {
                    sub->set(AttrBox::Tag_position, {pt.x, pt.y});
                }
            }
        }
        return dico;
    }
	// ================================================================================ //
	//                                 JPAGE CONTROLER                                  //
	// ================================================================================ //
	
	jPage::jPage(sPage page) : PageView(page)
    {
        setWantsKeyboardFocus(true);
		setInterceptsMouseClicks(true, true);
        setMouseClickGrabsKeyboardFocus(true);
        
        Application::bindToCommandManager(this);
        Application::bindToKeyMapping(this);
		
		m_io_highlighter = IoletHighlighter::create<jIoletHighlighter>();
        m_lasso          = Lasso::create<jLasso>(getPage());
		m_editor         = new juce::TextEditor();
        m_editor->addListener(this);
        
        addChildComponent(m_editor);
		addChildComponent(m_io_highlighter.get());
        addChildComponent(m_lasso.get());
    }
    
    jPage::~jPage()
    {
        removeChildComponent(m_editor);
        removeChildComponent(m_lasso.get());
        removeChildComponent(m_io_highlighter.get());
        
        m_editor->removeListener(this);
		m_editor = nullptr;
    }
	
	void jPage::redraw()
	{
		repaint();
	}
    
    void jPage::newBox(int x, int y, bool dblClick)
    {
        sjBox box;
        box = m_box_edited.lock();
        if(box)
        {
            box->setVisible(true);
            m_box_edited.reset();
        }
        if(dblClick)
        {
            box = getjBox(x, y);
            if(box)
            {
                m_box_edited = box;
				m_editor->setBounds(box->Component::getBounds());
                m_editor->setText(box->getText());
                box->setVisible(false);
                m_editor->setVisible(true);
            }
            else
            {
                m_editor->setBounds(x, y, 100, 20);
                m_editor->setVisible(true);
            }
        }
        else
        {
            m_editor->setBounds(x, y, 100, 20);
            m_editor->setVisible(true);
        }
    }
	
	void jPage::addToSelectionBasedOnModifiers(sBoxView box, bool selOnly)
	{
		if(selOnly)
		{
			selectOnly(box);
		}
		else if(isSelected(box))
		{
			unselect(box);
		}
		else
		{
			select(box);
		}
	}
	
	void jPage::addToSelectionBasedOnModifiers(sLinkView link, bool selOnly)
	{
		if(selOnly)
		{
			selectOnly(link);
		}
		else if(isSelected(link))
		{
			unselect(link);
		}
		else
		{
			select(link);
		}
	}
	
	bool jPage::selectOnMouseDown(sBoxView box, bool selOnly)
	{
		if(isSelected(box))
        {
			return true;
        }
		
		addToSelectionBasedOnModifiers(box, selOnly);
		return false;
	}
	
	bool jPage::selectOnMouseDown(sLinkView link, bool selOnly)
	{
		if(isSelected(link))
        {
			return true;
        }
		
		addToSelectionBasedOnModifiers(link, selOnly);
		return false;
	}
	
	void jPage::selectOnMouseUp(sBoxView box, bool selOnly, const bool boxWasDragged, const bool resultOfMouseDownSelectMethod)
	{
		if(resultOfMouseDownSelectMethod && ! boxWasDragged)
        {
			addToSelectionBasedOnModifiers(box, selOnly);
        }
	}
	
	
	void jPage::selectOnMouseUp(sLinkView link, bool selOnly, const bool boxWasDragged, const bool resultOfMouseDownSelectMethod)
	{
		if(resultOfMouseDownSelectMethod && ! boxWasDragged)
        {
			addToSelectionBasedOnModifiers(link, selOnly);
        }
	}
	
	void jPage::copySelectionToClipboard()
	{
		sDico dico = Dico::create();
		getSelectedBoxesDico(dico);
		String text = toString(dico);
		if (text.isNotEmpty())
		{
			SystemClipboard::copyTextToClipboard(text);
			Application::commandStatusChanged();
		}
	}
	
	void jPage::pasteFromClipboard(Gui::Point const& offset)
	{
		const string text = SystemClipboard::getTextFromClipboard().toStdString();
		if(!text.empty())
		{
            sDico dico = Dico::evaluateForJson(text);
			unselectAll();
			addBoxesFromDico(dico, offset);
		}
	}
	
	void jPage::bringsLinksToFront()
	{
		vector<sLinkView> links;
		getLinks(links);
		for(vector<sLinkView>::size_type i = 0; i < links.size(); i++)
		{
			sjLink link = dynamic_pointer_cast<jLink>(links[i]);
			if(link)
			{
				link->toFront(false);
			}
		}
	}
	
	void jPage::bringsBoxesToFront()
	{
		vector<sBoxView> boxes;
		getBoxes(boxes);
		for (vector<sBoxView>::size_type i = 0; i < boxes.size(); i++)
		{
			sjBox box = dynamic_pointer_cast<jBox>(boxes[i]);
			if (box)
			{
				box->toFront(false);
			}
		}
	}
	
	void jPage::attributeChanged(sPage, sAttr attr)
	{
		redraw();
	}
	
	sBoxView jPage::createBoxController(sBox box)
	{
		return BoxView::create<jBox>(box, static_pointer_cast<PageView>(shared_from_this()));
	}
	
	void jPage::boxControllerCreated(sBoxView boxctrl)
    {
		if(boxctrl)
		{
			if (sjBox jbox = dynamic_pointer_cast<jBox>(boxctrl))
			{
				if (sBox box = jbox->getBox())
				{
					if(getPresentationStatus())
					{
						box->setAttributeValue(AttrBox::Tag_presentation, {true});
                        Gui::Point pos = box->getPosition(false);
                        box->setAttributeValue(AttrBox::Tag_presentation_position, {pos.x(), pos.y()});
                        pos = box->getSize(false);
						box->setAttributeValue(AttrBox::Tag_presentation_size, {pos.x(), pos.y()});
						jbox->setPageEditionStatus(!getEditionStatus());
						jbox->setPagePresentationStatus(getPresentationStatus());
					}
					
					addAndMakeVisible(jbox.get());
					select(jbox);
					updateBoxesAndLinksLayers();
				}
			}
		}
    }
	
    void jPage::boxControllerWillBeRemoved(sBoxView boxctrl)
    {
		if(boxctrl)
		{
			if (sjBox jbox = dynamic_pointer_cast<jBox>(boxctrl))
			{
				removeChildComponent(jbox.get());
			}
		}
    }
	
	sLinkView jPage::createLinkController(sLink link)
	{
		return LinkView::create<jLink>(link);
	}
	
	void jPage::linkControllerCreated(sLinkView linkctrl)
	{
		if(linkctrl)
		{
			if (sjLink jlink = dynamic_pointer_cast<jLink>(linkctrl))
			{
				addAndMakeVisible(jlink.get());
				select(jlink);
				updateBoxesAndLinksLayers();
			}
		}
	}
	
	
	void jPage::linkControllerWillBeRemoved(sLinkView linkctrl)
	{
		if(linkctrl)
		{
			if (sjLink jlink = dynamic_pointer_cast<jLink>(linkctrl))
			{
				removeChildComponent(jlink.get());
			}
		}
	}
	
    sjBox jPage::getjBox(int x, int y) noexcept
    {
        knockBoxes(Gui::Point(x, y), getPresentationStatus());
        if(knockHasHitBox())
        {
            sBox box = knockGetBox();
            if(box)
            {
                int zaza;
                /*
                sBoxView box_ctrl = box->getController();
                if(box_ctrl)
                {
                    return static_pointer_cast<jBox>(box_ctrl);
                }
                 */
            }
        }
		
        return nullptr;
    }
	
	void jPage::selectionChanged()
	{
		vector<sBoxView> boxes, selboxes;
		getBoxes(boxes);
		getSelection(selboxes);
		
		// update inspector
		if(selboxes.size() == 1)
		{
			Application::getKiwiInstance()->setInspectorContent(selboxes[0]->getBox());
		}
		else if(selboxes.empty())
		{
			Application::getKiwiInstance()->setInspectorContent(sBox());
		}
		
		Application::commandStatusChanged();
	}
	
	void jPage::updateBoxesAndLinksLayers()
	{
		if (getEditionStatus())
			bringsBoxesToFront();
		else
			bringsLinksToFront();
	}
	
	void jPage::lockStatusChanged()
	{
		updateBoxesAndLinksLayers();
		repaint();
	}
	
	void jPage::presentationStatusChanged()
	{
		const bool presentation = getPresentationStatus();
		
		ComponentAnimator& animator = Desktop::getInstance().getAnimator();
		vector<sBoxView> boxes;
		getBoxes(boxes);
		for(vector<sBoxView>::size_type i = 0; i < boxes.size(); i++)
		{
            sjBox jbox = static_pointer_cast<jBox>(boxes[i]);
			if(jbox)
			{
                const bool usePresentationBounds = presentation && jbox->isIncludeInPresentation();
                const bool visible = !presentation || (presentation && jbox->isIncludeInPresentation());
                const Gui::Rectangle boxBounds = jbox->getDisplayBounds(usePresentationBounds);
                const juce::Rectangle<int> finalBounds = juce::Rectangle<int>(boxBounds.x(),
                                                                              boxBounds.y(),
                                                                              boxBounds.width(),
                                                                              boxBounds.height());
                
                animator.animateComponent(jbox.get(), finalBounds, visible ? 1. : 0., 200., false, 1., 1.);
                jbox->setVisible(visible);
			}
		}
		
		vector<sLinkView> links;
		getLinks(links);
		for (vector<sLinkView>::size_type i = 0; i < links.size(); i++)
		{
			if (sjLink jlink = static_pointer_cast<jLink>(links[i]))
			{
				jlink->setVisible(!presentation);
			}
		}
	}
	
    // ================================================================================ //
    //                                      COMPONENT                                   //
    // ================================================================================ //
    
    void jPage::paint(Graphics& g)
    {
		const bool locked = getEditionStatus();
		const juce::Colour bgcolor = KiwiToJuceColour((locked ? getPage()->getLockedBgColor() : getPage()->getEditingBgColor()));
		const int grid_size = getPage()->getGridSize();
        const juce::Rectangle<int> bounds(g.getClipBounds());
		
		g.setColour(bgcolor);
		g.fillAll();
		
		if (!getEditionStatus())
		{
			getPage()->getEditingBgColor();
			
			g.setColour(bgcolor.contrasting(0.5));
			for(int x = bounds.getX() - (bounds.getX() % grid_size); x < bounds.getRight(); x += grid_size)
			{
				for(int y = bounds.getY() - (bounds.getY() % grid_size) ; y < bounds.getBottom(); y += grid_size)
				{
					g.setPixel(x, y);
				}
			}
		}
    }
	
    void jPage::mouseDown(const MouseEvent& e)
    {
		m_box_received_downevent = false;
		m_copy_on_drag = false;
		m_box_dragstatus = false;
		m_link_dragstatus = false;
		m_mouse_wasclicked = true;

		if(!getEditionStatus())
		{
            knockAll(Gui::Point(e.x, e.y), getPresentationStatus());
			if(knockHasHitBox())
			{
                if(sBox box = knockGetBox())
                {
					if(knockGetPart() == Knock::Border)
					{
						m_last_border_downstatus = knockGetBorder();
					}
                    else if(knockGetPart() == Knock::Inlet)
                    {
                        int zaza;
                        m_templinks.clear();
                        sTempLink link = make_shared<TempLink>(box, knockGetIndex(), false);
                        m_templinks.push_back(link);
                        addAndMakeVisible(link.get());
                        link->beginLink(e);
                        m_io_highlighter->toFront(false);
                        
                        unselectAll();
                        Console::post("- templink from inlet created");
                    }
                    else if(knockGetPart() == Knock::Outlet)
                    {
                        int zaza;
                        m_templinks.clear();
                        sTempLink link = make_shared<TempLink>(box, knockGetIndex(), true);
                        m_templinks.push_back(link);
                        addAndMakeVisible(link.get());
                        link->beginLink(e);
                        m_io_highlighter->toFront(false);
                        
                        unselectAll();
                        Console::post("- templink from outlet created");
                    }
                    else if(knockGetPart() == Knock::Inside)
                    {
                        if(e.mods.isAltDown())
                        {
                            int zaza;
                            sBoxView box_ctrl = nullptr;//box->getController();
                            if(box_ctrl)
                            {
                                m_copy_on_drag = true;
                                m_box_downstatus = selectOnMouseDown(box_ctrl, true);
                            }
                        }
                        else if (e.mods.isCommandDown())
                        {
                            int todo;//
                            //box->receive(jEventMouse(Gui::Event::Mouse::Down, e));
                            m_box_received_downevent = true;
                        }
                        else
                        {
                            int zaza;
                            /*
                            sBoxView box_ctrl = nullptr;//box->getController();
                            {
                                if(e.mods.isPopupMenu())
                                {
									if (!boxctrl->isSelected())
									{
										m_box_downstatus = selectOnMouseDown(boxctrl, true);
									}
									
									showBoxPopupMenu(box);
                                }
								else
								{
									m_box_downstatus = selectOnMouseDown(boxctrl, !e.mods.isShiftDown());
									Console::post("selectOnMouseDown");
								}
                            }*/
                        }
                    }
                }
			}
			else if(knockHasHitLink())
			{
				if(knockGetPart() == Knock::Inside)
				{
                    sLink link = knockGetLink();
                    if(link)
                    {
                        int zaza;
                        sLinkView link_ctrl = nullptr;//link->getController();
                        if(link_ctrl)
                        {
                            m_link_downstatus = selectOnMouseDown(link_ctrl, !e.mods.isShiftDown());
                        }
                    }
                }
			}
			else if(knockHasHitPage())
			{
				m_lasso->begin(Gui::Point(e.x, e.y), e.mods.isShiftDown());
			}
		}

		m_last_drag = e.getPosition();
    }
	
	void jPage::mouseDrag(const MouseEvent& e)
	{
		MouseCursor::StandardCursorType mc = MouseCursor::NormalCursor;
		m_box_dragstatus = ! m_mouse_wasclicked;
		
		if(!getEditionStatus())
		{
			if(m_lasso->isPerforming())
			{
				m_lasso->setVisible(true);
                m_lasso->toFront(false);
                m_lasso->perform(Gui::Point(e.x, e.y), true, e.mods.isAltDown(), e.mods.isShiftDown(), getPresentationStatus());
			}
			else if(hasTempLinks())
			{
                knockAll(Gui::Point(e.x, e.y), getPresentationStatus());
				if(knockHasHitBox() && m_io_highlighter)
				{
					if(knockGetPart() == Knock::Inlet)
                    {
                        if(m_io_highlighter)
                        {
                            sBox box  = knockGetBox();
                            if(box)
                            {
                                m_io_highlighter->highlightInlet(box, knockGetIndex());
                                m_io_highlighter->setVisible(true);
                                m_io_highlighter->toFront(false);
                            }
                        }
                    }
					else if(knockGetPart() == Knock::Outlet)
                    {
                        if(m_io_highlighter)
                        {
                            sBox box  = knockGetBox();
                            if(box)
                            {
                                m_io_highlighter->highlightOutlet(box, knockGetIndex());
                                m_io_highlighter->setVisible(true);
                                m_io_highlighter->toFront(false);
                            }
                        }
                    }
				}
				
				sTempLink link = getTempLink();
				if(link)
				{
					link->dragLink(e);
					if(magnetFindIolet(link->getEndCoord(), link->getAttachedBox(), link->isAttachedToOutlet(), 50.))
                    {
                        if(link->isAttachedToOutlet())
                        {
                            m_io_highlighter->highlightInlet(magnetGetBox(), magnetGetIndex());
                        }
                        else
                        {
                            m_io_highlighter->highlightOutlet(magnetGetBox(), magnetGetIndex());
                        }
                        m_io_highlighter->setVisible(true);
                        m_io_highlighter->toFront(false);
                    }
                    else if(m_io_highlighter->isVisible())
                    {
                        m_io_highlighter->setVisible(false);
                    }
				}
			}
			else if (m_copy_on_drag && e.mods.isAltDown())
			{
				sDico dico = Dico::create();
				getSelectedBoxesDico(dico);
				unselectAll();
				addBoxesFromDico(dico);
				unselectAllLinks();
				m_copy_on_drag = false;
			}
			else if(sBox box = knockGetBox())
			{
				if(m_box_received_downevent && knockGetPart() == Knock::Inside)
				{
					if(m_box_received_downevent)
					{
                        int zaza;
						//box->receive(jEventMouse(Gui::Event::Mouse::Type::Drag, e));
					}
				}
				else if (m_last_border_downstatus != Knock::None)
				{
					if (m_mouse_wasclicked)
					{
						startMoveOrResizeBoxes();
					}
					Gui::Point delta = Gui::Point(e.getDistanceFromDragStartX(), e.getDistanceFromDragStartY());
					resizeSelectedBoxes(delta, m_last_border_downstatus, e.mods.isShiftDown());
					m_last_drag = e.getPosition();
					
					switch (m_last_border_downstatus)
					{
						case (Knock::Left | Knock::Top):		mc = MouseCursor::TopLeftCornerResizeCursor; break;
						case (Knock::Top) :						mc = MouseCursor::TopEdgeResizeCursor; break;
						case (Knock::Top | Knock::Right):		mc = MouseCursor::TopRightCornerResizeCursor; break;
						case (Knock::Left):						mc = MouseCursor::LeftEdgeResizeCursor; break;
						case (Knock::Right):					mc = MouseCursor::RightEdgeResizeCursor; break;
						case (Knock::Left | Knock::Bottom):		mc = MouseCursor::BottomLeftCornerResizeCursor; break;
						case (Knock::Bottom):					mc = MouseCursor::BottomEdgeResizeCursor; break;
						case (Knock::Right | Knock::Bottom):	mc = MouseCursor::BottomRightCornerResizeCursor; break;
						default: break;
					}
				}
				else if(isAnyBoxSelected() && m_box_dragstatus && !e.mods.isPopupMenu())
				{
					if(m_mouse_wasclicked)
					{
						startMoveOrResizeBoxes();
					}
					const juce::Point<int> pos = e.getPosition();
					Gui::Point delta = JuceToKiwiPoint(pos) - JuceToKiwiPoint(m_last_drag);
					moveSelectedBoxes(delta);
					m_last_drag = pos;
				}
			}
		}
		
		setMouseCursor(mc);
		m_mouse_wasclicked = false;
	}
	
    void jPage::mouseUp(const MouseEvent& e)
    {
		m_last_border_downstatus = Knock::None;
		
		if(!getEditionStatus())
		{
			if(knockHasHitBox() && knockGetPart() == Knock::Inside && e.mods.isCommandDown())
			{
				sBox box = knockGetBox();
				if(box)
				{
                    int zaza;
					//box->receive(jEventMouse(Gui::Event::Mouse::Type::Up, e));
					return;
				}
			}

            knockAll(Gui::Point(e.x, e.y), getPresentationStatus());
            sBox       magnet_box = magnetGetBox();
            sTempLink templink          = getTempLink();
            int zaza;
            sBox box = knockGetBox();
			if(magnet_box && templink)
			{
				if(templink)
				{
                    sBox from, to;
                    long in, out;
                    
                    if(templink->isAttachedToOutlet())
                    {
                        from = templink->getAttachedBox();
                        to = magnet_box;
                        in = magnetGetIndex();
                        out = templink->getAttachedIOIndex();
                    }
                    else
                    {
                        from= magnet_box;
                        to  = templink->getAttachedBox();
                        in  = templink->getAttachedIOIndex();
                        out = magnetGetIndex();
                    }
                    
                    //sLink link = Link::create(getPage(), from, out, to, in);
                    int TODO_add_link_whith_dico;
                    //getPage()->createLink(link);
				}
			}
            else if(box)
            {
                int zaza;
                sBoxView box_ctrl = nullptr;//box->getController();
                {
                    selectOnMouseUp(box_ctrl, !e.mods.isShiftDown(), m_box_dragstatus, m_box_downstatus);
                }
            }
			else if(e.mods.isCommandDown())
			{
				setLockStatus(true);
			}
			
			if(hasTempLinks())
			{
				sTempLink link = getTempLink();
				if (link)
				{
					link->endDrag();
					removeAllTempLink();
				}
			}
			
			if(m_lasso)
			{
				m_lasso->end();
                m_lasso->setVisible(false);
			}
			
			if(m_io_highlighter && m_io_highlighter->isVisible())
			{
				m_io_highlighter->setVisible(false);
			}
		}
		else if(e.mods.isCommandDown())
		{
			setLockStatus(false);
		}
		
		m_mouse_wasclicked = false;
    }
	
    void jPage::mouseMove(const MouseEvent& e)
    {
		MouseCursor::StandardCursorType mc = MouseCursor::NormalCursor;
        
        knockAll(Gui::Point(e.x, e.y), getPresentationStatus());
		
		if(knockHasHitBox())
		{
			if(knockGetPart() == Knock::Border)
			{
				switch (knockGetBorder())
				{
					case (Knock::Left | Knock::Top):		mc = MouseCursor::TopLeftCornerResizeCursor; break;
					case (Knock::Top) :						mc = MouseCursor::TopEdgeResizeCursor; break;
					case (Knock::Top | Knock::Right):		mc = MouseCursor::TopRightCornerResizeCursor; break;
					case (Knock::Left):						mc = MouseCursor::LeftEdgeResizeCursor; break;
					case (Knock::Right):					mc = MouseCursor::RightEdgeResizeCursor; break;
					case (Knock::Left | Knock::Bottom):		mc = MouseCursor::BottomLeftCornerResizeCursor; break;
					case (Knock::Bottom):					mc = MouseCursor::BottomEdgeResizeCursor; break;
					case (Knock::Right | Knock::Bottom):	mc = MouseCursor::BottomRightCornerResizeCursor; break;
					default: break;
				}
			}
			else if(knockGetPart() == Knock::Inlet)
			{
				if(m_io_highlighter)
				{
                    sBox box  = knockGetBox();
                    if(box)
                    {
                        m_io_highlighter->highlightInlet(box, knockGetIndex());
                        m_io_highlighter->setVisible(true);
                        m_io_highlighter->toFront(false);
                    }
				}
			}
			else if(knockGetPart() == Knock::Outlet)
			{
				if(m_io_highlighter)
				{
                    sBox box  = knockGetBox();
                    if(box)
                    {
                        m_io_highlighter->highlightOutlet(box, knockGetIndex());
                        m_io_highlighter->setVisible(true);
                        m_io_highlighter->toFront(false);
                    }
				}
			}
			else if(m_io_highlighter->isVisible())
			{
				m_io_highlighter->setVisible(false);
			}
		}
		else if(m_io_highlighter && m_io_highlighter->isVisible())
		{
			m_io_highlighter->setVisible(false);
		}
		
		setMouseCursor(mc);
    }
	
    void jPage::mouseEnter(const MouseEvent& e)
    {
        ;
    }
    
    void jPage::mouseExit(const MouseEvent& e)
    {
        ;
    }
    
    void jPage::mouseDoubleClick(const MouseEvent& e)
    {
		if (!getEditionStatus() && !e.mods.isAnyModifierKeyDown())
		{
			newBox(e.x, e.y, true);
		}
    }
    
    void jPage::mouseWheelMove(const MouseEvent& event, const MouseWheelDetails& wheel)
    {
        ;
    }
    
    bool jPage::keyPressed(const KeyPress& key)
    {
        if(key.isKeyCode(KeyPress::spaceKey))
        {
            sPage page = getPage();
            if(page)
            {
                page->dspStart(44100., 64);
                return true;
            }
        }
        if(key.getTextCharacter() == L'v')
        {
            sPage page = getPage();
            if(page)
            {
                page->dspTick();
                return true;
            }
        }
		if (isAnythingSelected())
		{
			if (key.isKeyCode(KeyPress::deleteKey) || key.isKeyCode(KeyPress::backspaceKey))
			{
				deleteSelection();
				return true;
			}
			else if(key.isKeyCode(KeyPress::returnKey))
			{
				//if a box is selected (only one) anf this box is a textbox container => give it textediting focus
				//showBoxTextEditor(nullptr);
			}
			else
			{
				const bool snap = key.getModifiers().isShiftDown();
				const long gridsize = getPage()->getGridSize();
				const int amount = snap ? gridsize : 1;

				if(key.isKeyCode(KeyPress::rightKey))
				{
					moveSelectedBoxes(Gui::Point(amount, 0));
					return true;
				}
				else if(key.isKeyCode(KeyPress::downKey))
				{
					moveSelectedBoxes(Gui::Point(0, amount));
					return true;
				}
				else if(key.isKeyCode(KeyPress::leftKey))
				{
					moveSelectedBoxes(Gui::Point(-amount, 0));
					return true;
				}
				else if(key.isKeyCode(KeyPress::upKey))
				{
					moveSelectedBoxes(Gui::Point(0, -amount));
					return true;
				}
			}
		}
		
		return false;
    }
    
    // ================================================================================ //
    //                                  LABEL LISTENER                                  //
    // ================================================================================ //
    
	void jPage::textEditorTextChanged(juce::TextEditor& e)
    {
        Console::post("textEditorTextChanged");
    }
    
    void jPage::textEditorReturnKeyPressed(juce::TextEditor& e)
    {
        sDico dico = createBoxDicoAtPosition(e.getText().toStdString(), e.getPosition());
        e.clear();
        e.setVisible(false);
        getPage()->add(dico);
    }
    
    void jPage::textEditorEscapeKeyPressed(juce::TextEditor& e)
    {
        sjBox box = m_box_edited.lock();
        if(box)
        {
            box->setVisible(true);
            m_box_edited.reset();
        }
        Console::post("textEditorEscapeKeyPressed");
        e.setVisible(false);
    }
    
    void jPage::textEditorFocusLost(juce::TextEditor& e)
    {
        ;
    }
    
    // ================================================================================ //
    //                              APPLICATION COMMAND TARGET                          //
    // ================================================================================ //
	
	void jPage::showBoxPopupMenu(sBox box)
	{
		ApplicationCommandManager* commandManager = &Application::getCommandManager();
		
		PopupMenu m;
		
		m.addCommandItem(commandManager, StandardApplicationCommandIDs::cut);
		m.addCommandItem(commandManager, StandardApplicationCommandIDs::copy);
		m.addCommandItem(commandManager, StandardApplicationCommandIDs::paste);
		m.addCommandItem(commandManager, StandardApplicationCommandIDs::del);
		m.addSeparator();
		m.addCommandItem(commandManager, CommandIDs::showObjectInspector);
		m.addCommandItem(commandManager, CommandIDs::toFront);
		m.addCommandItem(commandManager, CommandIDs::toBack);
		m.addCommandItem(commandManager, CommandIDs::addToPresentation);
		m.addCommandItem(commandManager, CommandIDs::removeFromPresentation);
		m.show();
	}
    
    ApplicationCommandTarget* jPage::getNextCommandTarget()
    {
        return findFirstTargetParentComponent();
    }
    
    void jPage::getAllCommands(Array<CommandID>& commands)
    {
		commands.add(CommandIDs::save);
        //commands.add(StandardApplicationCommandIDs::undo);
        //commands.add(StandardApplicationCommandIDs::redo);
        commands.add(StandardApplicationCommandIDs::cut);
        commands.add(StandardApplicationCommandIDs::copy);
        commands.add(StandardApplicationCommandIDs::paste);
        commands.add(CommandIDs::pasteReplace);
        commands.add(CommandIDs::duplicate);
		commands.add(StandardApplicationCommandIDs::del);
		commands.add(StandardApplicationCommandIDs::selectAll);
		
		commands.add(CommandIDs::toFront);
		commands.add(CommandIDs::toBack);
		commands.add(CommandIDs::addToPresentation);
		commands.add(CommandIDs::removeFromPresentation);
		
        commands.add(CommandIDs::newBox);
        commands.add(CommandIDs::newBang);
        commands.add(CommandIDs::newToggle);
        commands.add(CommandIDs::newNumber);
        commands.add(CommandIDs::newMessage);
        
        commands.add(CommandIDs::zoomIn);
        commands.add(CommandIDs::zoomOut);
        commands.add(CommandIDs::zoomNormal);
        commands.add(CommandIDs::editModeSwitch);
        commands.add(CommandIDs::presentationModeSwitch);
        commands.add(CommandIDs::gridModeSwitch);
        commands.add(CommandIDs::enableSnapToGrid);
		
		commands.add(CommandIDs::showPageInspector);
		commands.add(CommandIDs::showObjectInspector);
		
		//CommandIDs::openObjectHelp
    }
    
    void jPage::getCommandInfo(const CommandID commandID, ApplicationCommandInfo& result)
    {
        switch(commandID)
        {
			case CommandIDs::save:
				result.setInfo (TRANS("Save"), TRANS("Save document"), CommandCategories::general, 0);
				result.addDefaultKeypress ('s',  ModifierKeys::commandModifier);
				break;
				
			case CommandIDs::showPageInspector:
				result.setInfo (TRANS("Page inspector"), TRANS("Open page inspector"), CommandCategories::view, 0);
				result.addDefaultKeypress ('i',  ModifierKeys::commandModifier | ModifierKeys::shiftModifier);
				break;
				
			case CommandIDs::showObjectInspector:
				result.setInfo (TRANS("Object inspector"), TRANS("Open object inspector"), CommandCategories::view, 0);
				result.addDefaultKeypress ('i',  ModifierKeys::commandModifier);
				result.setActive(isAnyBoxSelected());
				break;
				
			case StandardApplicationCommandIDs::cut:
				result.setInfo(TRANS("Cut"), TRANS("Cut"), CommandCategories::editing, 0);
				result.addDefaultKeypress('x', ModifierKeys::commandModifier);
				result.setActive(isAnyBoxSelected());
				break;
				
			case StandardApplicationCommandIDs::copy:
				result.setInfo(TRANS("Copy"), TRANS("Copy"), CommandCategories::editing, 0);
				result.addDefaultKeypress('c', ModifierKeys::commandModifier);
				result.setActive(isAnyBoxSelected());
				break;
				
			case StandardApplicationCommandIDs::paste:
				result.setInfo(TRANS("Paste"), TRANS("Paste"), CommandCategories::editing, 0);
				result.addDefaultKeypress('v', ModifierKeys::commandModifier);
				result.setActive(!getEditionStatus() && SystemClipboard::getTextFromClipboard().isNotEmpty());
				break;
				
			case CommandIDs::pasteReplace:
				result.setInfo(TRANS("Paste replace"), TRANS("Replace selected objects with the object on the clipboard"), CommandCategories::editing, 0);
				result.addDefaultKeypress('v', ModifierKeys::commandModifier | ModifierKeys::shiftModifier);
				result.setActive(isAnyBoxSelected() && SystemClipboard::getTextFromClipboard().isNotEmpty());
				break;
				
			case CommandIDs::duplicate:
				result.setInfo(TRANS("Duplicate"), TRANS("Duplicate the selection"), CommandCategories::editing, 0);
				result.addDefaultKeypress('d', ModifierKeys::commandModifier);
				result.setActive(isAnyBoxSelected());
				break;
				
            case StandardApplicationCommandIDs::del:
                result.setInfo(TRANS("Delete"), TRANS("Delete all selected boxes and links"), CommandCategories::editing, 0);
                result.addDefaultKeypress(KeyPress::backspaceKey, ModifierKeys::noModifiers);
                result.setActive(isAnythingSelected());
                break;
            
            case StandardApplicationCommandIDs::selectAll:
                result.setInfo(TRANS("Select All"), TRANS("Select all boxes and links"), CommandCategories::editing, 0);
                result.addDefaultKeypress('a', ModifierKeys::commandModifier);
				result.setActive(!getEditionStatus());
                break;
				
			case CommandIDs::toFront:
				result.setInfo(TRANS("Bring to Front"), TRANS("Bring selected boxes to front"), CommandCategories::editing, 0);
				result.addDefaultKeypress('f', ModifierKeys::commandModifier | ModifierKeys::shiftModifier);
				result.setActive(isAnyBoxSelected());
				break;
				
			case CommandIDs::toBack:
				result.setInfo(TRANS("Send to Back"), TRANS("Send selected boxes to back"), CommandCategories::editing, 0);
				result.addDefaultKeypress('b', ModifierKeys::commandModifier | ModifierKeys::shiftModifier);
				result.setActive(isAnyBoxSelected());
				break;
				
			case CommandIDs::addToPresentation:
				result.setInfo(TRANS("Add to Presentation"), TRANS("Add selected boxes to presentation"), CommandCategories::editing, 0);
				result.addDefaultKeypress('p', ModifierKeys::commandModifier | ModifierKeys::shiftModifier);
				result.setActive(isAnyBoxSelected());
				break;
				
			case CommandIDs::removeFromPresentation:
				result.setInfo(TRANS("Remove from Presentation"), TRANS("Remove selected boxes from presentation"), CommandCategories::editing, 0);
				result.addDefaultKeypress('p', ModifierKeys::commandModifier | ModifierKeys::altModifier);
				result.setActive(isAnyBoxSelected());
				break;
                
            case CommandIDs::newBox:
                result.setInfo(TRANS("New Box"), TRANS("Add a new box in the page"), CommandCategories::editing, 0);
                result.addDefaultKeypress('n', ModifierKeys::noModifiers);
				result.setActive(!getEditionStatus());
                break;
            
            case CommandIDs::newBang:
                result.setInfo(TRANS("New Bang"), TRANS("Add a bang in the page"), CommandCategories::editing, 0);
                result.addDefaultKeypress('b', ModifierKeys::noModifiers);
				result.setActive(!getEditionStatus());
                break;
                
            case CommandIDs::newNumber:
                result.setInfo(TRANS("New Number"), TRANS("Add a number in the page"), CommandCategories::editing, 0);
                result.addDefaultKeypress('f', ModifierKeys::noModifiers);
				result.setActive(!getEditionStatus());
                break;
                
            case CommandIDs::newToggle:
                result.setInfo(TRANS("New Toggle"), TRANS("Add a toggle in the page"), CommandCategories::editing, 0);
                result.addDefaultKeypress('t', ModifierKeys::noModifiers);
				result.setActive(!getEditionStatus());
                break;
                
            case CommandIDs::newMessage:
                result.setInfo(TRANS("New Message"), TRANS("Add a message in the page"), CommandCategories::editing, 0);
                result.addDefaultKeypress('m', ModifierKeys::noModifiers);
                break;
                
            case CommandIDs::zoomIn:
                result.setInfo(TRANS("Zoom In"), TRANS("Zoom In"), CommandCategories::view, 0);
                result.addDefaultKeypress('+', ModifierKeys::commandModifier | ModifierKeys::shiftModifier);
                break;
                
            case CommandIDs::zoomOut:
                result.setInfo (TRANS("Zoom Out"), TRANS("Zoom Out"), CommandCategories::view, 0);
                result.addDefaultKeypress ('-',  ModifierKeys::commandModifier);
                break;
                
            case CommandIDs::zoomNormal:
                result.setInfo (TRANS("Show at 100%"), TRANS("Restore the Zoom to 100%"), CommandCategories::view, 0);
                result.addDefaultKeypress ('=',  ModifierKeys::commandModifier);
                result.setActive(getZoom() != 100);
                break;
                
            case CommandIDs::editModeSwitch:
                result.setInfo (TRANS("Edit"), TRANS("Switch between edit and play mode"), CommandCategories::view, 0);
                result.addDefaultKeypress ('e',  ModifierKeys::commandModifier);
                result.setTicked(!getEditionStatus());
                break;
                
            case CommandIDs::presentationModeSwitch:
                result.setInfo (TRANS("Presentation"), TRANS("Switch between presentation and patching mode"), CommandCategories::view, 0);
                result.addDefaultKeypress ('e',  ModifierKeys::commandModifier | ModifierKeys::altModifier);
                result.setTicked(getPresentationStatus());
                break;
                
            case CommandIDs::gridModeSwitch:
                result.setInfo (TRANS("Grid"), TRANS("Switch between grid modes"), CommandCategories::view, 0);
                result.setTicked(getGridDisplayedStatus());
                break;
                
            case CommandIDs::enableSnapToGrid:
                result.setInfo (TRANS("Snap to grid"), TRANS("Enable/disable snap to grid"), CommandCategories::view, 0);
                result.addDefaultKeypress ('g',  ModifierKeys::commandModifier | ModifierKeys::altModifier);
                result.setTicked(getSnapToGridStatus());
                break;
                
            default:
                result.setInfo (TRANS("[unknown command]"), TRANS("dada"), CommandCategories::view, 0);
                break;
        }
    }
    
    bool jPage::perform(const InvocationInfo& info)
    {
        Console::post("perform command");
        switch (info.commandID)
        {
			case CommandIDs::save:
			{
				DBG("|- try to save page");
				Application::getKiwiInstance()->savePage(getPage());
				break;
			}
			case CommandIDs::showPageInspector:
			{
				DBG("|- page inspector");
				Application::getKiwiInstance()->showInspector(getPage());
				break;
			}
			case CommandIDs::showObjectInspector:
			{
				DBG("|- object inspector");
				
				vector<sBoxView> boxes;
				getSelection(boxes);
				if(boxes.size() == 1)
				{
					Application::getKiwiInstance()->showInspector(boxes[0]->getBox());
				}
				break;
			}
			case StandardApplicationCommandIDs::cut:
			{
				DBG("|- cut box");
				copySelectionToClipboard();
				deleteSelection();
				break;
			}
			case StandardApplicationCommandIDs::copy:
			{
				DBG("|- copy box");
				copySelectionToClipboard();
				break;
			}
			case StandardApplicationCommandIDs::paste:
			{
				DBG("|- paste boxes");
				const long gridsize = getPage()->getGridSize();
				pasteFromClipboard(Gui::Point(gridsize, gridsize));
				break;
			}
			case CommandIDs::pasteReplace:
			{
				DBG("|- paste replace boxes");
				//replaceBoxesFromClipboard();
				break;
			}
			case CommandIDs::duplicate:
			{
				DBG("|- duplicate boxes");
				copySelectionToClipboard();
				const long gridsize = getPage()->getGridSize();
				pasteFromClipboard(Gui::Point(gridsize, gridsize));
				unselectAllLinks();
				break;
			}
			case StandardApplicationCommandIDs::del:
            {
                DBG("|- delete selection");
                deleteSelection();
                break;
            }
            case StandardApplicationCommandIDs::selectAll:
            {
				selectAllBoxes();
                break;
            }
			case CommandIDs::toFront:
			{
				break;
			}
			case CommandIDs::toBack:
			{
				break;
			}
			case CommandIDs::addToPresentation:
			{
				vector<sBoxView> boxes;
				getSelection(boxes);
				setBoxesPresentationStatus(boxes, true);
				break;
			}
			case CommandIDs::removeFromPresentation:
			{
				vector<sBoxView> boxes;
				getSelection(boxes);
				setBoxesPresentationStatus(boxes, false);
				break;
			}
            case CommandIDs::newBox:
            {
				if (isMouseButtonDownAnywhere()) return false;
				unselectAll();
                getPage()->add(createBoxDicoAtPosition("newbox", getMouseXYRelative()));
                break;
            }
            case CommandIDs::newBang:
            {
				if (isMouseButtonDownAnywhere()) return false;
				unselectAll();
                getPage()->add(createBoxDicoAtPosition("bang", getMouseXYRelative()));
                break;
            }
            case CommandIDs::newToggle:
            {
				if (isMouseButtonDownAnywhere()) return false;
				unselectAll();
                getPage()->add(createBoxDicoAtPosition("toggle", getMouseXYRelative()));
                break;
            }
            case CommandIDs::newNumber:
            {
				if (isMouseButtonDownAnywhere()) return false;
				unselectAll();
                getPage()->add(createBoxDicoAtPosition("number", getMouseXYRelative()));
                break;
            }
            case CommandIDs::newMessage:
            {
				if (isMouseButtonDownAnywhere()) return false;
                unselectAll();
                getPage()->add(createBoxDicoAtPosition("message", getMouseXYRelative()));
                break;
            }
            case CommandIDs::zoomIn:
            {
                setZoom(getZoom() + 10);
                break;
            }
            case CommandIDs::zoomOut:
            {
                setZoom(getZoom() - 10);
                break;
            }
            case CommandIDs::zoomNormal:
            {
                setZoom(100);
                break;
            }
            case CommandIDs::editModeSwitch:
            {
                setLockStatus(!getEditionStatus());
                break;
            }
            case CommandIDs::presentationModeSwitch:
            {
                setPresentationStatus(!getPresentationStatus());
                break;
            }
            case CommandIDs::gridModeSwitch:
            {
                setGridDisplayedStatus(!getGridDisplayedStatus());
                break;
            }
            case CommandIDs::enableSnapToGrid:
            {
                setSnapToGridStatus(!getSnapToGridStatus());
                break;
            }
            default:
                return false;
        }
        
        return true;
    }
}

