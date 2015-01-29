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
    
		m_editor         = new juce::TextEditor();
        m_editor->addListener(this);
        addChildComponent(m_editor);
        m_iolighter      = new jIolighter();
		addChildComponent(m_iolighter);
        m_lasso          = new jLasso();
        addChildComponent(m_lasso);
    }
    
    jPage::~jPage()
    {
        removeChildComponent(m_editor);
        removeChildComponent(m_iolighter);
        removeChildComponent(m_lasso);
        
        
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
	
	sBoxView jPage::createBoxView(sBox box)
	{
		return BoxView::create<jBox>(box, static_pointer_cast<PageView>(shared_from_this()));
	}
	
	void jPage::boxViewCreated(sBoxView boxview)
    {
		if(boxview)
		{
			sjBox jbox = dynamic_pointer_cast<jBox>(boxview);
			if (jbox)
			{
				sBox box = jbox->getBox();
				if (box)
				{
					if(getPresentationStatus())
					{
						box->setAttributeValue(AttrBox::Tag_presentation, {true});
                        Gui::Point pos = box->getPosition(false);
                        box->setAttributeValue(AttrBox::Tag_presentation_position, {pos.x(), pos.y()});
                        pos = box->getSize(false);
						box->setAttributeValue(AttrBox::Tag_presentation_size, {pos.x(), pos.y()});
						jbox->pageViewLockStatusChanged();
						jbox->pageViewPresentationStatusChanged();
					}
					
					addAndMakeVisible(jbox.get());
					select(jbox);
					updateBoxesAndLinksLayers();
				}
			}
		}
    }
	
    void jPage::boxViewWillBeRemoved(sBoxView boxview)
    {
		if(boxview)
		{
			sjBox jbox = dynamic_pointer_cast<jBox>(boxview);
			if (jbox)
			{
				removeChildComponent(jbox.get());
			}
		}
    }
	
	sLinkView jPage::createLinkView(sLink link)
	{
		return LinkView::create<jLink>(link);
	}
	
	void jPage::linkViewCreated(sLinkView linkview)
	{
		if(linkview)
		{
			if (sjLink jlink = dynamic_pointer_cast<jLink>(linkview))
			{
				addAndMakeVisible(jlink.get());
				select(jlink);
				updateBoxesAndLinksLayers();
			}
		}
	}
	
	
	void jPage::linkViewWillBeRemoved(sLinkView linkview)
	{
		if(linkview)
		{
			if (sjLink jlink = dynamic_pointer_cast<jLink>(linkview))
			{
				removeChildComponent(jlink.get());
			}
		}
	}
	
    sjBox jPage::getjBox(int x, int y) noexcept
    {
        int zaza;
        /*
        knockBoxes(Gui::Point(x, y), getPresentationStatus());
        if(m_knock.hasHitBox())
        {
            sBox box = m_knock.getBox();
            if(box)
            {
                
                
                sBoxView boxview = box->getView();
                if(boxview)
                {
                    return static_pointer_cast<jBox>(boxview);
                }
                
            }
        }*/
		
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
		if (getLockStatus())
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
                const bool visible = !presentation || (presentation && jbox->isIncludeInPresentation());
                const Gui::Rectangle boxBounds = jbox->getDisplayBounds();
				const juce::Rectangle<int> finalBounds = toJuce<int>(boxBounds);
				
                animator.animateComponent(jbox.get(), finalBounds, visible, 200., false, 1., 1.);
                jbox->setVisible(visible);
			}
		}
		
		vector<sLinkView> links;
		getLinks(links);
		for (vector<sLinkView>::size_type i = 0; i < links.size(); i++)
		{
			sjLink jlink = static_pointer_cast<jLink>(links[i]);
			if (jlink)
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
		sPage page = getPage();
		if (page)
		{
			const bool locked = getLockStatus();
			const juce::Colour bgcolor = toJuce((locked ? page->getLockedBgColor() : page->getEditingBgColor()));
			
			g.setColour(bgcolor);
			g.fillAll();
			
			if (!locked)
			{
				const int grid_size = page->getGridSize();
				const juce::Rectangle<int> bounds(g.getClipBounds());
				
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
    }
	
    void jPage::mouseDown(const MouseEvent& e)
    {
		m_box_received_downevent = false;
		m_copy_on_drag = false;
		m_box_dragstatus = false;
		m_link_dragstatus = false;
		m_mouse_wasclicked = true;

		if(!getLockStatus())
		{
            m_knock = knockAll(Gui::Point(e.x, e.y));
			if(m_knock.hasHitBox())
			{
                sBoxView box = m_knock.getBox();
                if(box)
                {
					if(m_knock.getPart() == Knock::Border)
					{
						m_last_border_downstatus = m_knock.getBorder();
					}
                    else if(m_knock.getPart() == Knock::Inlet)
                    {
                        int zaza;
                        m_templinks.clear();
                        sTempLink link = make_shared<TempLink>(box, m_knock.getIndex(), false);
                        m_templinks.push_back(link);
                        addAndMakeVisible(link.get());
                        link->beginLink(e);
                        m_iolighter->toFront(false);
                        
                        unselectAll();
                        Console::post("- templink from inlet created");
                    }
                    else if(m_knock.getPart() == Knock::Outlet)
                    {
                        int zaza;
                        m_templinks.clear();
                        sTempLink link = make_shared<TempLink>(box, m_knock.getIndex(), true);
                        m_templinks.push_back(link);
                        addAndMakeVisible(link.get());
                        link->beginLink(e);
                        m_iolighter->toFront(false);
                        
                        unselectAll();
                        Console::post("- templink from outlet created");
                    }
                    else if(m_knock.getPart() == Knock::Inside)
                    {
						Console::post("Hit Box inside");
                        if(e.mods.isAltDown())
                        {
                            m_copy_on_drag = true;
                            m_box_downstatus = selectOnMouseDown(box, true);
                        }
                        else if (e.mods.isCommandDown())
						{
							Gui::sMouser mouser = dynamic_pointer_cast<Gui::Mouser>(box->getBox());
							if(mouser)
							{
								mouser->receive(jEventMouse(Gui::Event::Mouse::Down, e));
								m_box_received_downevent = true;
							}
                        }
                        else
                        {
							if(e.mods.isPopupMenu())
							{
								if (!isSelected(box))
								{
									m_box_downstatus = selectOnMouseDown(box, true);
								}

								showBoxPopupMenu(box->getBox());
							}
							else
							{
								m_box_downstatus = selectOnMouseDown(box, !e.mods.isShiftDown());
								Console::post("selectOnMouseDown");
							}
                        }
                    }
                }
			}
			else if(m_knock.hasHitLink())
			{
				if(m_knock.getPart() == Knock::Inside)
				{
                    sLinkView link = m_knock.getLink();
                    if(link)
                    {
                        m_link_downstatus = selectOnMouseDown(link, !e.mods.isShiftDown());
                    }
                }
			}
			else if(m_knock.hasHitPage())
			{
				lassoBegin(m_lasso, Gui::Point(e.x, e.y), e.mods.isShiftDown());
			}
		}

		m_last_drag = e.getPosition();
    }
	
	void jPage::mouseDrag(const MouseEvent& e)
	{
		MouseCursor::StandardCursorType mc = MouseCursor::NormalCursor;
		m_box_dragstatus = ! m_mouse_wasclicked;
		
		if(!getLockStatus())
		{
			if(m_lasso->dragging)
			{
				m_lasso->setVisible(true);
                m_lasso->toFront(false);
                lassoPerform(m_lasso, Gui::Point(e.x, e.y), true, e.mods.isAltDown(), e.mods.isShiftDown());
                m_lasso->setBounds(toJuce<int>(m_lasso->Lasso::bounds));
			}
			else if(hasTempLinks())
			{
                m_knock = knockAll(Gui::Point(e.x, e.y));
				if(m_knock.hasHitBox() && m_iolighter)
				{
					if(m_knock.getPart() == Knock::Inlet)
                    {
                        if(m_iolighter)
                        {
                            sBoxView box  = m_knock.getBox();
                            if(box)
                            {
                                m_iolighter->setInlet(box, m_knock.getIndex());
                                m_iolighter->setVisible(true);
                                m_iolighter->toFront(false);
                            }
                        }
                    }
					else if(m_knock.getPart() == Knock::Outlet)
                    {
                        if(m_iolighter)
                        {
                            sBoxView box  = m_knock.getBox();
                            if(box)
                            {
                                m_iolighter->setOutlet(box, m_knock.getIndex());
                                m_iolighter->setVisible(true);
                                m_iolighter->toFront(false);
                            }
                        }
                    }
				}
				
				sTempLink link = getTempLink();
				if(link)
				{
					link->dragLink(e);
                    if(link->isAttachedToOutlet())
                    {
                        int todo;
                        //m_magnet = magnetFindInlet(link->getAttachedBox(), link->getEndCoord());
                        m_iolighter->setInlet(m_magnet.getBox(), m_magnet.getIndex());
                        m_iolighter->setVisible(true);
                        m_iolighter->toFront(false);
                    }
                    else
                    {
                        int todo;
                        //m_magnet = magnetFindOutlet(link->getAttachedBox(), link->getEndCoord());
                        m_iolighter->setOutlet(m_magnet.getBox(), m_magnet.getIndex());
                        m_iolighter->setVisible(true);
                        m_iolighter->toFront(false);
                    }
				}
                else if(m_iolighter->isVisible())
                {
                    m_iolighter->setVisible(false);
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
			else if(m_knock.hasHitBox())
			{
                sBoxView box = m_knock.getBox();
				if (box)
				{
					if(m_box_received_downevent && m_knock.getPart() == Knock::Inside)
					{
						if(m_box_received_downevent)
						{
							Gui::sMouser mouser = dynamic_pointer_cast<Gui::Mouser>(box->getBox());
							if(mouser)
							{
								mouser->receive(jEventMouse(Gui::Event::Mouse::Drag, e));
							}
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
						Gui::Point delta = toKiwi(pos) - toKiwi(m_last_drag);
						moveSelectedBoxes(delta);
						m_last_drag = pos;
					}
				}
			}
		}
		
		setMouseCursor(mc);
		m_mouse_wasclicked = false;
	}
	
    void jPage::mouseUp(const MouseEvent& e)
    {
		m_last_border_downstatus = Knock::None;
		
		if(m_box_received_downevent)
		{
			sBoxView box = m_knock.getBox();
			if(box)
			{
				Gui::sMouser mouser = dynamic_pointer_cast<Gui::Mouser>(box->getBox());
				if(mouser)
				{
					mouser->receive(jEventMouse(Gui::Event::Mouse::Up, e));
					return;
				}
			}
		}
		
		if(!getLockStatus())
		{
            int zaza;
            m_knock = knockAll(Gui::Point(e.x, e.y));
            sBoxView box = m_knock.getBox();
            sBoxView magnet_box = m_magnet.getBox();
            sTempLink templink  = getTempLink();
			if(magnet_box && templink)
			{
				if(templink)
				{
                    wBoxView from, to;
                    long in, out;
                    
                    if(templink->isAttachedToOutlet())
                    {
                        from = templink->getAttachedBox();
                        to = magnet_box;
                        in = m_magnet.getIndex();
                        out = templink->getAttachedIOIndex();
                    }
                    else
                    {
                        from= magnet_box;
                        to  = templink->getAttachedBox();
                        in  = templink->getAttachedIOIndex();
                        out = m_magnet.getIndex();
                    }
                    
                    //sLink link = Link::create(getPage(), from, out, to, in);
                    int TODO_add_link_whith_dico;
                    //getPage()->createLink(link);
				}
			}
            else if(box)
            {
                selectOnMouseUp(box, !e.mods.isShiftDown(), m_box_dragstatus, m_box_downstatus);
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
			
			if(m_lasso->isVisible())
			{
                lassoEnd(m_lasso);
                m_lasso->setVisible(false);
			}
			
			if(m_iolighter && m_iolighter->isVisible())
			{
				m_iolighter->setVisible(false);
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
        
        m_knock = knockAll(Gui::Point(e.x, e.y));
		
		if(m_knock.hasHitBox())
		{
			if(m_knock.getPart() == Knock::Border)
			{
				switch (m_knock.getBorder())
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
			else if(m_knock.getPart() == Knock::Inlet)
			{
				if(m_iolighter)
				{
                    sBoxView box  = m_knock.getBox();
                    if(box)
                    {
                        m_iolighter->setInlet(box, m_knock.getIndex());
                        m_iolighter->setVisible(true);
                        m_iolighter->toFront(false);
                    }
				}
			}
			else if(m_knock.getPart() == Knock::Outlet)
			{
				if(m_iolighter)
				{
                    sBoxView box  = m_knock.getBox();
                    if(box)
                    {
                        m_iolighter->setOutlet(box, m_knock.getIndex());
                        m_iolighter->setVisible(true);
                        m_iolighter->toFront(false);
                    }
				}
			}
			else if(m_iolighter->isVisible())
			{
				m_iolighter->setVisible(false);
			}
		}
		else if(m_iolighter && m_iolighter->isVisible())
		{
			m_iolighter->setVisible(false);
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
		if (!getLockStatus() && !e.mods.isAnyModifierKeyDown())
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
				
				if (isAnyBoxSelected())
				{
					vector<sBoxView> boxes;
					getSelection(boxes);
					if(boxes.size() == 1)
					{
						sjBox jbox = dynamic_pointer_cast<jBox>(boxes[0]);
						if (jbox)
						{
							jbox->grabKeyboardFocus();
						}
					}
				}
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
				result.setActive(!getLockStatus() && SystemClipboard::getTextFromClipboard().isNotEmpty());
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
				result.setActive(!getLockStatus());
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
				result.setActive(!getLockStatus());
                break;
            
            case CommandIDs::newBang:
                result.setInfo(TRANS("New Bang"), TRANS("Add a bang in the page"), CommandCategories::editing, 0);
                result.addDefaultKeypress('b', ModifierKeys::noModifiers);
				result.setActive(!getLockStatus());
                break;
                
            case CommandIDs::newNumber:
                result.setInfo(TRANS("New Number"), TRANS("Add a number in the page"), CommandCategories::editing, 0);
                result.addDefaultKeypress('f', ModifierKeys::noModifiers);
				result.setActive(!getLockStatus());
                break;
                
            case CommandIDs::newToggle:
                result.setInfo(TRANS("New Toggle"), TRANS("Add a toggle in the page"), CommandCategories::editing, 0);
                result.addDefaultKeypress('t', ModifierKeys::noModifiers);
				result.setActive(!getLockStatus());
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
                result.setTicked(!getLockStatus());
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
				sPage page = getPage();
				if (page)
				{
					sDico dico = createBoxDicoAtPosition("bang", getMouseXYRelative());
					page->add(dico);
				}
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
                setLockStatus(!getLockStatus());
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
    
    // ================================================================================ //
    //                                 JIO HIGHLIGHTER									//
    // ================================================================================ //
    
    jPage::jIolighter::jIolighter()
    {
        setInterceptsMouseClicks(false, false);
        setWantsKeyboardFocus(false);
    }
    
    jPage::jIolighter::~jIolighter()
    {
        ;
    }
    
    void jPage::jIolighter::paint(Graphics& g)
    {
        const juce::Rectangle<float> size(4.5, 4.5, getWidth() - 9., getHeight() - 9.);
        g.setColour(m_colour.brighter(0.3));
        g.fillEllipse(size);
        g.setColour(m_colour);
        g.fillEllipse(size);
    }
    
    void jPage::jIolighter::setInlet(sBoxView box, ulong index)
    {
        if(box)
        {
            Box::sInlet inlet = box->getBox()->getInlet(index);
            if(inlet)
            {
                if(inlet->getPolarity() == Box::Io::Cold)
                {
                    m_colour = juce::Colour::fromFloatRGBA(0.28, 0.28, 0.88, 1);
                }
                else
                {
                    m_colour = juce::Colour::fromFloatRGBA(0.88, 0.28, 0.88, 1);
                }
                const Gui::Point pos = box->getInletPosition(index);
                setBounds(pos.x() - 8., pos.x() - 8., 16., 16.);
            }
        }
    }
    
    void jPage::jIolighter::setOutlet(sBoxView box, ulong index)
    {
        if(box)
        {
            Box::sOutlet outlet = box->getBox()->getOutlet(index);
            if(outlet)
            {
                m_colour = juce::Colour::fromFloatRGBA(0.88, 0.28, 0.88, 1);
                const Gui::Point pos = box->getOutletPosition(index);
                setBounds(pos.x() - 8., pos.x() - 8., 16., 16.);
            }
        }
    }
}

