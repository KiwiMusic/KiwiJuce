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

#include "jPatcher.h"
#include "Application.h"

namespace Kiwi
{
    sDico createObjectDicoAtPosition(string const& name, juce::Point<int> const& pt)
    {
        sDico dico = Dico::evaluateForObject(name);
        if(dico)
        {
            sDico sub = dico->get(Tag::List::objects);
            if(sub)
            {
                sub->set(Tag::create("position"), {pt.x, pt.y});
            }
        }
        return dico;
    }
	// ================================================================================ //
	//                                 JPAGE CONTROLER                                  //
	// ================================================================================ //
	
	jPatcher::jPatcher(sPatcher patcher) : PatcherView(patcher)
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
    
    jPatcher::~jPatcher()
    {
        removeChildComponent(m_editor);
        removeChildComponent(m_iolighter);
        removeChildComponent(m_lasso);
		
        m_editor->removeListener(this);
		m_editor = nullptr;
    }
	
	void jPatcher::redraw()
	{
		repaint();
	}
    
    void jPatcher::newObject(int x, int y, bool dblClick)
    {
        sjObject object;
        object = m_object_edited.lock();
        if(object)
        {
            object->setVisible(true);
            m_object_edited.reset();
        }
        if(dblClick)
        {
            object = getjObject(x, y);
            if(object)
            {
                m_object_edited = object;
				m_editor->setBounds(object->Component::getBounds());
                m_editor->setText(object->getText());
                object->setVisible(false);
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
	
	void jPatcher::addToSelectionBasedOnModifiers(sObjectView object, bool selOnly)
	{
		if(selOnly)
		{
			selectOnly(object);
		}
		else if(isSelected(object))
		{
			unselect(object);
		}
		else
		{
			select(object);
		}
	}
	
	void jPatcher::addToSelectionBasedOnModifiers(sLinkView link, bool selOnly)
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
	
	bool jPatcher::selectOnMouseDown(sObjectView object, bool selOnly)
	{
		if(isSelected(object))
        {
			return true;
        }
		
		addToSelectionBasedOnModifiers(object, selOnly);
		return false;
	}
	
	bool jPatcher::selectOnMouseDown(sLinkView link, bool selOnly)
	{
		if(isSelected(link))
        {
			return true;
        }
		
		addToSelectionBasedOnModifiers(link, selOnly);
		return false;
	}
	
	void jPatcher::selectOnMouseUp(sObjectView object, bool selOnly, const bool objectWasDragged, const bool resultOfMouseDownSelectMethod)
	{
		if(resultOfMouseDownSelectMethod && ! objectWasDragged)
        {
			addToSelectionBasedOnModifiers(object, selOnly);
        }
	}
	
	
	void jPatcher::selectOnMouseUp(sLinkView link, bool selOnly, const bool objectWasDragged, const bool resultOfMouseDownSelectMethod)
	{
		if(resultOfMouseDownSelectMethod && ! objectWasDragged)
        {
			addToSelectionBasedOnModifiers(link, selOnly);
        }
	}
	
	void jPatcher::copySelectionToClipboard()
	{
		scDico dico = getSelectedObjectsDico();
		String text = toString(dico);
		
		if (text.isNotEmpty())
		{
			SystemClipboard::copyTextToClipboard(text);
			Application::commandStatusChanged();
		}
	}
	
	void jPatcher::pasteFromClipboard(Kiwi::Point const& offset)
	{
		const string text = SystemClipboard::getTextFromClipboard().toStdString();
		if(!text.empty())
		{
            sDico dico = Dico::evaluateForJson(text);
			unselectAll();
			addObjectsFromDico(dico, offset);
		}
	}
	
	void jPatcher::bringsLinksToFront()
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
	
	void jPatcher::bringsObjectsToFront()
	{
		vector<sObjectView> objects;
		getObjects(objects);
		for (vector<sObjectView>::size_type i = 0; i < objects.size(); i++)
		{
			sjObject object = dynamic_pointer_cast<jObject>(objects[i]);
			if (object)
			{
				object->toFront(false);
			}
		}
	}
	
	bool jPatcher::notify(sAttr attr)
	{
		redraw();
		return true;
	}
	
	sObjectView jPatcher::createObjectView(sObject object)
	{
		return ObjectView::create<jObject>(object, static_pointer_cast<PatcherView>(shared_from_this()));
	}
	
	void jPatcher::objectViewCreated(sObjectView objectview)
    {
		if(objectview)
		{
			sjObject jobject = dynamic_pointer_cast<jObject>(objectview);
			if (jobject)
			{
				sObject object = jobject->getObject();
				if (object)
				{
					if(getPresentationStatus())
					{
						int todo;
						object->setAttributeValue(AttrObject::Tag_presentation, {true});
						Kiwi::Point pos = object->getPosition(false);
						object->setAttributeValue(AttrObject::Tag_presentation_position, {pos.x(), pos.y()});
						pos = object->getSize(false);
						object->setAttributeValue(AttrObject::Tag_presentation_size, {pos.x(), pos.y()});
						jobject->patcherViewLockStatusChanged();
						jobject->patcherViewPresentationStatusChanged();
						
						/*
						object->setAttributeValue(AttrObject::Tag_presentation, {true});
                        Kiwi::Point pos = object->getPosition(false);
                        object->setAttributeValue(AttrObject::Tag_presentation_position, {pos.x(), pos.y()});
                        pos = object->getSize(false);
						object->setAttributeValue(AttrObject::Tag_presentation_size, {pos.x(), pos.y()});
						jobject->patcherViewLockStatusChanged();
						jobject->patcherViewPresentationStatusChanged();
						*/
					}
					
					addAndMakeVisible(jobject.get());
					select(jobject);
					updateObjectsAndLinksLayers();
				}
			}
		}
    }
	
    void jPatcher::objectViewWillBeRemoved(sObjectView objectview)
    {
		if(objectview)
		{
			sjObject jobject = dynamic_pointer_cast<jObject>(objectview);
			if (jobject)
			{
				removeChildComponent(jobject.get());
			}
		}
    }
	
	sLinkView jPatcher::createLinkView(sLink link)
	{
		return LinkView::create<jLink>(link);
	}
	
	void jPatcher::linkViewCreated(sLinkView linkview)
	{
		if(linkview)
		{
			if (sjLink jlink = dynamic_pointer_cast<jLink>(linkview))
			{
				addAndMakeVisible(jlink.get());
				select(jlink);
				updateObjectsAndLinksLayers();
			}
		}
	}
	
	
	void jPatcher::linkViewWillBeRemoved(sLinkView linkview)
	{
		if(linkview)
		{
			if (sjLink jlink = dynamic_pointer_cast<jLink>(linkview))
			{
				removeChildComponent(jlink.get());
			}
		}
	}
	
    sjObject jPatcher::getjObject(int x, int y) noexcept
    {
        int zaza;
        /*
        knockObjects(Kiwi::Point(x, y), getPresentationStatus());
        if(m_knock.hasHitObject())
        {
            sObject object = m_knock.getObject();
            if(object)
            {
                
                
                sObjectView objectview = object->getView();
                if(objectview)
                {
                    return static_pointer_cast<jObject>(objectview);
                }
                
            }
        }*/
		
        return nullptr;
    }
	
	void jPatcher::selectionChanged()
	{
		vector<sObjectView> objects, selobjects;
		getObjects(objects);
		getSelection(selobjects);
		
		// update inspector
		if(selobjects.size() == 1)
		{
			Application::getKiwiInstance()->setInspectorContent(selobjects[0]->getObject());
		}
		else if(selobjects.empty())
		{
			Application::getKiwiInstance()->setInspectorContent(sObject());
		}
		
		Application::commandStatusChanged();
	}
	
	void jPatcher::updateObjectsAndLinksLayers()
	{
		if (getLockStatus())
			bringsObjectsToFront();
		else
			bringsLinksToFront();
	}
	
	void jPatcher::lockStatusChanged()
	{
		updateObjectsAndLinksLayers();
		repaint();
	}
	
	void jPatcher::presentationStatusChanged()
	{
		const bool presentation = getPresentationStatus();
		
		ComponentAnimator& animator = Desktop::getInstance().getAnimator();
		vector<sObjectView> objects;
		getObjects(objects);
		for(vector<sObjectView>::size_type i = 0; i < objects.size(); i++)
		{
            sjObject jobject = static_pointer_cast<jObject>(objects[i]);
			if(jobject)
			{
                const bool visible = !presentation || (presentation && jobject->isIncludeInPresentation());
                const Kiwi::Rectangle objectBounds = jobject->getDisplayBounds();
				const juce::Rectangle<int> finalBounds = toJuce<int>(objectBounds);
				
                animator.animateComponent(jobject.get(), finalBounds, visible, 200., false, 1., 1.);
                jobject->setVisible(visible);
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
    
    void jPatcher::paint(Graphics& g)
    {
		sPatcher patcher = getPatcher();
		if (patcher)
		{
			const bool locked = getLockStatus();
			const juce::Colour bgcolor = toJuce((locked ? patcher->getLockedBackgroundColor() : patcher->getUnlockedBackgroundColor()));
			
			g.setColour(bgcolor);
			g.fillAll();
			
			if (!locked)
			{
				const int grid_size = patcher->getGridSize();
				const juce::Rectangle<int> bounds(g.getClipBounds());
				
				g.setColour(bgcolor.contrasting(0.5).withAlpha(0.7f));
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
	
    void jPatcher::mouseDown(const MouseEvent& e)
    {
		m_object_received_downevent = false;
		m_copy_on_drag = false;
		m_object_dragstatus = false;
		m_link_dragstatus = false;
		m_mouse_wasclicked = true;

		if(!getLockStatus())
		{
            m_knock = knockAll(Kiwi::Point(e.x, e.y));
			if(m_knock.hasHitObject())
			{
                sObjectView object = m_knock.getObject();
                if(object)
                {
					if(m_knock.getPart() == Knock::Border)
					{
						m_last_border_downstatus = m_knock.getBorder();
					}
                    else if(m_knock.getPart() == Knock::Inlet)
                    {
                        int zaza;
                        m_templinks.clear();
                        sTempLink link = make_shared<TempLink>(object, m_knock.getIndex(), false);
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
                        sTempLink link = make_shared<TempLink>(object, m_knock.getIndex(), true);
                        m_templinks.push_back(link);
                        addAndMakeVisible(link.get());
                        link->beginLink(e);
                        m_iolighter->toFront(false);
                        
                        unselectAll();
                        Console::post("- templink from outlet created");
                    }
                    else if(m_knock.getPart() == Knock::Inside)
                    {
						Console::post("Hit Object inside");
                        if(e.mods.isAltDown())
                        {
                            m_copy_on_drag = true;
                            m_object_downstatus = selectOnMouseDown(object, true);
                        }
                        else if (e.mods.isCommandDown())
						{
							Gui::sMouser mouser = dynamic_pointer_cast<Gui::Mouser>(object->getObject());
							if(mouser)
							{
								mouser->receive(jEventMouse(Gui::Mouser::Event::Down, e));
								m_object_received_downevent = true;
							}
                        }
                        else
                        {
							if(e.mods.isPopupMenu())
							{
								if (!isSelected(object))
								{
									m_object_downstatus = selectOnMouseDown(object, true);
								}

								showObjectPopupMenu(object->getObject());
							}
							else
							{
								m_object_downstatus = selectOnMouseDown(object, !e.mods.isShiftDown());
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
			else if(m_knock.hasHitPatcher())
			{
				lassoBegin(m_lasso, Kiwi::Point(e.x, e.y), e.mods.isShiftDown());
			}
		}

		m_last_drag = e.getPosition();
    }
	
	void jPatcher::mouseDrag(const MouseEvent& e)
	{
		MouseCursor::StandardCursorType mc = MouseCursor::NormalCursor;
		m_object_dragstatus = ! m_mouse_wasclicked;
		
		if(!getLockStatus())
		{
			if(m_lasso->dragging)
			{
				m_lasso->setVisible(true);
                m_lasso->toFront(false);
                lassoPerform(m_lasso, Kiwi::Point(e.x, e.y), true, e.mods.isAltDown(), e.mods.isShiftDown());
                m_lasso->setBounds(toJuce<int>(m_lasso->Lasso::bounds));
			}
			else if(hasTempLinks())
			{
                m_knock = knockAll(Kiwi::Point(e.x, e.y));
				if(m_knock.hasHitObject() && m_iolighter)
				{
					if(m_knock.getPart() == Knock::Inlet)
                    {
                        if(m_iolighter)
                        {
                            sObjectView object  = m_knock.getObject();
                            if(object)
                            {
                                m_iolighter->setInlet(object, m_knock.getIndex());
                                m_iolighter->setVisible(true);
                                m_iolighter->toFront(false);
                            }
                        }
                    }
					else if(m_knock.getPart() == Knock::Outlet)
                    {
                        if(m_iolighter)
                        {
                            sObjectView object  = m_knock.getObject();
                            if(object)
                            {
                                m_iolighter->setOutlet(object, m_knock.getIndex());
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
                        //m_magnet = magnetFindInlet(link->getAttachedObject(), link->getEndCoord());
                        m_iolighter->setInlet(m_magnet.getObject(), m_magnet.getIndex());
                        m_iolighter->setVisible(true);
                        m_iolighter->toFront(false);
                    }
                    else
                    {
                        int todo;
                        //m_magnet = magnetFindOutlet(link->getAttachedObject(), link->getEndCoord());
                        m_iolighter->setOutlet(m_magnet.getObject(), m_magnet.getIndex());
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
				sDico dico = getSelectedObjectsDico();
				unselectAll();
				addObjectsFromDico(dico);
				unselectAllLinks();
				m_copy_on_drag = false;
			}
			else if(m_knock.hasHitObject())
			{
                sObjectView object = m_knock.getObject();
				if (object)
				{
					if(m_object_received_downevent && m_knock.getPart() == Knock::Inside)
					{
						if(m_object_received_downevent)
						{
							Gui::sMouser mouser = dynamic_pointer_cast<Gui::Mouser>(object->getObject());
							if(mouser)
							{
								mouser->receive(jEventMouse(Gui::Mouser::Event::Drag, e));
							}
						}
					}
					else if (m_last_border_downstatus != Knock::None)
					{
						if (m_mouse_wasclicked)
						{
							startMoveOrResizeObjects();
						}
						Kiwi::Point delta = Kiwi::Point(e.getDistanceFromDragStartX(), e.getDistanceFromDragStartY());
						resizeSelectedObjects(delta, m_last_border_downstatus, e.mods.isShiftDown());
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
					else if(isAnyObjectSelected() && m_object_dragstatus && !e.mods.isPopupMenu())
					{
						if(m_mouse_wasclicked)
						{
							startMoveOrResizeObjects();
						}
						const juce::Point<int> pos = e.getPosition();
						Kiwi::Point delta = toKiwi(pos) - toKiwi(m_last_drag);
						moveSelectedObjects(delta);
						m_last_drag = pos;
					}
				}
			}
		}
		
		setMouseCursor(mc);
		m_mouse_wasclicked = false;
	}
	
    void jPatcher::mouseUp(const MouseEvent& e)
    {
		m_last_border_downstatus = Knock::None;
		
		if(m_lasso->dragging)
		{
			lassoEnd(m_lasso);
			m_lasso->setVisible(false);
		}
		
		if(m_object_received_downevent)
		{
			sObjectView object = m_knock.getObject();
			if(object)
			{
				Gui::sMouser mouser = dynamic_pointer_cast<Gui::Mouser>(object->getObject());
				if(mouser)
				{
					mouser->receive(jEventMouse(Gui::Mouser::Event::Up, e));
					return;
				}
			}
		}
		
		if(!getLockStatus())
		{
            int zaza;
            m_knock = knockAll(Kiwi::Point(e.x, e.y));
            sObjectView object = m_knock.getObject();
            sObjectView magnet_object = m_magnet.getObject();
            sTempLink templink  = getTempLink();
			if(magnet_object && templink)
			{
				if(templink)
				{
                    wObjectView from, to;
                    long in, out;
                    
                    if(templink->isAttachedToOutlet())
                    {
                        from = templink->getAttachedObject();
                        to = magnet_object;
                        in = m_magnet.getIndex();
                        out = templink->getAttachedIOIndex();
                    }
                    else
                    {
                        from= magnet_object;
                        to  = templink->getAttachedObject();
                        in  = templink->getAttachedIOIndex();
                        out = m_magnet.getIndex();
                    }
                    
                    //sLink link = Link::create(getPatcher(), from, out, to, in);
                    int TODO_add_link_whith_dico;
                    //getPatcher()->createLink(link);
				}
			}
            else if(object)
            {
                selectOnMouseUp(object, !e.mods.isShiftDown(), m_object_dragstatus, m_object_downstatus);
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
	
    void jPatcher::mouseMove(const MouseEvent& e)
    {
		MouseCursor::StandardCursorType mc = MouseCursor::NormalCursor;
        
        m_knock = knockAll(Kiwi::Point(e.x, e.y));
		
		if(m_knock.hasHitObject())
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
                    sObjectView object  = m_knock.getObject();
                    if(object)
                    {
                        m_iolighter->setInlet(object, m_knock.getIndex());
                        m_iolighter->setVisible(true);
                        m_iolighter->toFront(false);
                    }
				}
			}
			else if(m_knock.getPart() == Knock::Outlet)
			{
				if(m_iolighter)
				{
                    sObjectView object  = m_knock.getObject();
                    if(object)
                    {
                        m_iolighter->setOutlet(object, m_knock.getIndex());
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
	
    void jPatcher::mouseEnter(const MouseEvent& e)
    {
        ;
    }
    
    void jPatcher::mouseExit(const MouseEvent& e)
    {
        ;
    }
    
    void jPatcher::mouseDoubleClick(const MouseEvent& e)
    {
		if (!getLockStatus() && !e.mods.isAnyModifierKeyDown())
		{
			newObject(e.x, e.y, true);
		}
    }
    
    void jPatcher::mouseWheelMove(const MouseEvent& event, const MouseWheelDetails& wheel)
    {
        ;
    }
    
    bool jPatcher::keyPressed(const KeyPress& key)
    {
        if(key.isKeyCode(KeyPress::spaceKey))
        {
            sPatcher patcher = getPatcher();
            if(patcher)
            {
                patcher->dspStart(44100., 64);
                return true;
            }
        }
        if(key.getTextCharacter() == L'v')
        {
            sPatcher patcher = getPatcher();
            if(patcher)
            {
                patcher->dspTick();
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
				//if an object is selected (only one) anf this object is a textobject container => give it textediting focus
				
				if (isAnyObjectSelected())
				{
					vector<sObjectView> objects;
					getSelection(objects);
					if(objects.size() == 1)
					{
						sjObject jobject = dynamic_pointer_cast<jObject>(objects[0]);
						if (jobject)
						{
							jobject->grabKeyboardFocus();
						}
					}
				}
			}
			else
			{
				const bool snap = key.getModifiers().isShiftDown();
				int todo;
				//const long gridsize = getPatcher()->getGridSize();
				const long gridsize = 10;
				const int amount = snap ? gridsize : 1;

				if(key.isKeyCode(KeyPress::rightKey))
				{
					moveSelectedObjects(Kiwi::Point(amount, 0));
					return true;
				}
				else if(key.isKeyCode(KeyPress::downKey))
				{
					moveSelectedObjects(Kiwi::Point(0, amount));
					return true;
				}
				else if(key.isKeyCode(KeyPress::leftKey))
				{
					moveSelectedObjects(Kiwi::Point(-amount, 0));
					return true;
				}
				else if(key.isKeyCode(KeyPress::upKey))
				{
					moveSelectedObjects(Kiwi::Point(0, -amount));
					return true;
				}
			}
		}
		
		return false;
    }
    
    // ================================================================================ //
    //                                  LABEL LISTENER                                  //
    // ================================================================================ //
    
	void jPatcher::textEditorTextChanged(juce::TextEditor& e)
    {
        Console::post("textEditorTextChanged");
    }
    
    void jPatcher::textEditorReturnKeyPressed(juce::TextEditor& e)
    {
        sDico dico = createObjectDicoAtPosition(e.getText().toStdString(), e.getPosition());
        e.clear();
        e.setVisible(false);
        getPatcher()->add(dico);
    }
    
    void jPatcher::textEditorEscapeKeyPressed(juce::TextEditor& e)
    {
        sjObject object = m_object_edited.lock();
        if(object)
        {
            object->setVisible(true);
            m_object_edited.reset();
        }
        Console::post("textEditorEscapeKeyPressed");
        e.setVisible(false);
    }
    
    void jPatcher::textEditorFocusLost(juce::TextEditor& e)
    {
        ;
    }
    
    // ================================================================================ //
    //                              APPLICATION COMMAND TARGET                          //
    // ================================================================================ //
	
	void jPatcher::showObjectPopupMenu(sObject object)
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
    
    ApplicationCommandTarget* jPatcher::getNextCommandTarget()
    {
        return findFirstTargetParentComponent();
    }
    
    void jPatcher::getAllCommands(Array<CommandID>& commands)
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
		
        commands.add(CommandIDs::newObject);
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
		
		commands.add(CommandIDs::showPatcherInspector);
		commands.add(CommandIDs::showObjectInspector);
		
		//CommandIDs::openObjectHelp
    }
    
    void jPatcher::getCommandInfo(const CommandID commandID, ApplicationCommandInfo& result)
    {
        switch(commandID)
        {
			case CommandIDs::save:
				result.setInfo (TRANS("Save"), TRANS("Save document"), CommandCategories::general, 0);
				result.addDefaultKeypress ('s',  ModifierKeys::commandModifier);
				break;
				
			case CommandIDs::showPatcherInspector:
				result.setInfo (TRANS("Patcher inspector"), TRANS("Open patcher inspector"), CommandCategories::view, 0);
				result.addDefaultKeypress ('i',  ModifierKeys::commandModifier | ModifierKeys::shiftModifier);
				break;
				
			case CommandIDs::showObjectInspector:
				result.setInfo (TRANS("Object inspector"), TRANS("Open object inspector"), CommandCategories::view, 0);
				result.addDefaultKeypress ('i',  ModifierKeys::commandModifier);
				result.setActive(isAnyObjectSelected());
				break;
				
			case StandardApplicationCommandIDs::cut:
				result.setInfo(TRANS("Cut"), TRANS("Cut"), CommandCategories::editing, 0);
				result.addDefaultKeypress('x', ModifierKeys::commandModifier);
				result.setActive(isAnyObjectSelected());
				break;
				
			case StandardApplicationCommandIDs::copy:
				result.setInfo(TRANS("Copy"), TRANS("Copy"), CommandCategories::editing, 0);
				result.addDefaultKeypress('c', ModifierKeys::commandModifier);
				result.setActive(isAnyObjectSelected());
				break;
				
			case StandardApplicationCommandIDs::paste:
				result.setInfo(TRANS("Paste"), TRANS("Paste"), CommandCategories::editing, 0);
				result.addDefaultKeypress('v', ModifierKeys::commandModifier);
				result.setActive(!getLockStatus() && SystemClipboard::getTextFromClipboard().isNotEmpty());
				break;
				
			case CommandIDs::pasteReplace:
				result.setInfo(TRANS("Paste replace"), TRANS("Replace selected objects with the object on the clipboard"), CommandCategories::editing, 0);
				result.addDefaultKeypress('v', ModifierKeys::commandModifier | ModifierKeys::shiftModifier);
				result.setActive(isAnyObjectSelected() && SystemClipboard::getTextFromClipboard().isNotEmpty());
				break;
				
			case CommandIDs::duplicate:
				result.setInfo(TRANS("Duplicate"), TRANS("Duplicate the selection"), CommandCategories::editing, 0);
				result.addDefaultKeypress('d', ModifierKeys::commandModifier);
				result.setActive(isAnyObjectSelected());
				break;
				
            case StandardApplicationCommandIDs::del:
                result.setInfo(TRANS("Delete"), TRANS("Delete all selected objects and links"), CommandCategories::editing, 0);
                result.addDefaultKeypress(KeyPress::backspaceKey, ModifierKeys::noModifiers);
                result.setActive(isAnythingSelected());
                break;
            
            case StandardApplicationCommandIDs::selectAll:
                result.setInfo(TRANS("Select All"), TRANS("Select all objects and links"), CommandCategories::editing, 0);
                result.addDefaultKeypress('a', ModifierKeys::commandModifier);
				result.setActive(!getLockStatus());
                break;
				
			case CommandIDs::toFront:
				result.setInfo(TRANS("Bring to Front"), TRANS("Bring selected objects to front"), CommandCategories::editing, 0);
				result.addDefaultKeypress('f', ModifierKeys::commandModifier | ModifierKeys::shiftModifier);
				result.setActive(isAnyObjectSelected());
				break;
				
			case CommandIDs::toBack:
				result.setInfo(TRANS("Send to Back"), TRANS("Send selected objects to back"), CommandCategories::editing, 0);
				result.addDefaultKeypress('b', ModifierKeys::commandModifier | ModifierKeys::shiftModifier);
				result.setActive(isAnyObjectSelected());
				break;
				
			case CommandIDs::addToPresentation:
				result.setInfo(TRANS("Add to Presentation"), TRANS("Add selected objects to presentation"), CommandCategories::editing, 0);
				result.addDefaultKeypress('p', ModifierKeys::commandModifier | ModifierKeys::shiftModifier);
				result.setActive(isAnyObjectSelected());
				break;
				
			case CommandIDs::removeFromPresentation:
				result.setInfo(TRANS("Remove from Presentation"), TRANS("Remove selected objects from presentation"), CommandCategories::editing, 0);
				result.addDefaultKeypress('p', ModifierKeys::commandModifier | ModifierKeys::altModifier);
				result.setActive(isAnyObjectSelected());
				break;
                
            case CommandIDs::newObject:
                result.setInfo(TRANS("New Object"), TRANS("Add a new object in the patcher"), CommandCategories::editing, 0);
                result.addDefaultKeypress('n', ModifierKeys::noModifiers);
				result.setActive(!getLockStatus());
                break;
            
            case CommandIDs::newBang:
                result.setInfo(TRANS("New Bang"), TRANS("Add a bang in the patcher"), CommandCategories::editing, 0);
                result.addDefaultKeypress('b', ModifierKeys::noModifiers);
				result.setActive(!getLockStatus());
                break;
                
            case CommandIDs::newNumber:
                result.setInfo(TRANS("New Number"), TRANS("Add a number in the patcher"), CommandCategories::editing, 0);
                result.addDefaultKeypress('f', ModifierKeys::noModifiers);
				result.setActive(!getLockStatus());
                break;
                
            case CommandIDs::newToggle:
                result.setInfo(TRANS("New Toggle"), TRANS("Add a toggle in the patcher"), CommandCategories::editing, 0);
                result.addDefaultKeypress('t', ModifierKeys::noModifiers);
				result.setActive(!getLockStatus());
                break;
                
            case CommandIDs::newMessage:
                result.setInfo(TRANS("New Message"), TRANS("Add a message in the patcher"), CommandCategories::editing, 0);
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
    
    bool jPatcher::perform(const InvocationInfo& info)
    {
        Console::post("perform command");
        switch (info.commandID)
        {
			case CommandIDs::save:
			{
				DBG("|- try to save patcher");
				Application::getKiwiInstance()->savePatcher(getPatcher());
				break;
			}
			case CommandIDs::showPatcherInspector:
			{
				DBG("|- patcher inspector");
				Application::getKiwiInstance()->showInspector(getPatcher());
				break;
			}
			case CommandIDs::showObjectInspector:
			{
				DBG("|- object inspector");
				
				vector<sObjectView> objects;
				getSelection(objects);
				if(objects.size() == 1)
				{
					Application::getKiwiInstance()->showInspector(objects[0]->getObject());
				}
				break;
			}
			case StandardApplicationCommandIDs::cut:
			{
				DBG("|- cut object");
				copySelectionToClipboard();
				deleteSelection();
				break;
			}
			case StandardApplicationCommandIDs::copy:
			{
				DBG("|- copy object");
				copySelectionToClipboard();
				break;
			}
			case StandardApplicationCommandIDs::paste:
			{
				DBG("|- paste objects");
				const long gridsize = getPatcher()->getGridSize();
				pasteFromClipboard(Kiwi::Point(gridsize, gridsize));
				break;
			}
			case CommandIDs::pasteReplace:
			{
				DBG("|- paste replace objects");
				//replaceObjectsFromClipboard();
				break;
			}
			case CommandIDs::duplicate:
			{
				DBG("|- duplicate objects");
				copySelectionToClipboard();
				const long gridsize = getPatcher()->getGridSize();
				pasteFromClipboard(Kiwi::Point(gridsize, gridsize));
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
				selectAllObjects();
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
				vector<sObjectView> objects;
				getSelection(objects);
				setObjectsPresentationStatus(objects, true);
				break;
			}
			case CommandIDs::removeFromPresentation:
			{
				vector<sObjectView> objects;
				getSelection(objects);
				setObjectsPresentationStatus(objects, false);
				break;
			}
            case CommandIDs::newObject:
            {
				if (isMouseButtonDownAnywhere()) return false;
				unselectAll();
                getPatcher()->add(createObjectDicoAtPosition("newobject", getMouseXYRelative()));
                break;
            }
            case CommandIDs::newBang:
            {
				if (isMouseButtonDownAnywhere()) return false;
				unselectAll();
				sPatcher patcher = getPatcher();
				if (patcher)
				{
					sDico dico = createObjectDicoAtPosition("bang", getMouseXYRelative());
					patcher->add(dico);
				}
                break;
            }
            case CommandIDs::newToggle:
            {
				if (isMouseButtonDownAnywhere()) return false;
				unselectAll();
                getPatcher()->add(createObjectDicoAtPosition("toggle", getMouseXYRelative()));
                break;
            }
            case CommandIDs::newNumber:
            {
				if (isMouseButtonDownAnywhere()) return false;
				unselectAll();
                getPatcher()->add(createObjectDicoAtPosition("number", getMouseXYRelative()));
                break;
            }
            case CommandIDs::newMessage:
            {
				if (isMouseButtonDownAnywhere()) return false;
                unselectAll();
                getPatcher()->add(createObjectDicoAtPosition("message", getMouseXYRelative()));
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
    
    jPatcher::jIolighter::jIolighter()
    {
        setInterceptsMouseClicks(false, false);
        setWantsKeyboardFocus(false);
    }
    
    jPatcher::jIolighter::~jIolighter()
    {
        ;
    }
    
    void jPatcher::jIolighter::paint(Graphics& g)
    {
        const juce::Rectangle<float> size(4.5, 4.5, getWidth() - 9., getHeight() - 9.);
        g.setColour(m_colour.brighter(0.3));
        g.fillEllipse(size);
        g.setColour(m_colour);
        g.fillEllipse(size);
    }
    
    void jPatcher::jIolighter::setInlet(sObjectView object, ulong index)
    {
        if(object)
        {
            Object::sInlet inlet = object->getObject()->getInlet(index);
            if(inlet)
            {
                if(inlet->getPolarity() == Object::Io::Cold)
                {
                    m_colour = juce::Colour::fromFloatRGBA(0.28, 0.28, 0.88, 1);
                }
                else
                {
                    m_colour = juce::Colour::fromFloatRGBA(0.88, 0.28, 0.88, 1);
                }
                const Kiwi::Point pos = object->getInletPosition(index);
                setBounds(pos.x() - 8., pos.x() - 8., 16., 16.);
            }
        }
    }
    
    void jPatcher::jIolighter::setOutlet(sObjectView object, ulong index)
    {
        if(object)
        {
            Object::sOutlet outlet = object->getObject()->getOutlet(index);
            if(outlet)
            {
                m_colour = juce::Colour::fromFloatRGBA(0.88, 0.28, 0.88, 1);
                const Kiwi::Point pos = object->getOutletPosition(index);
                setBounds(pos.x() - 8., pos.x() - 8., 16., 16.);
            }
        }
    }
}

