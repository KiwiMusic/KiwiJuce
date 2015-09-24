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

#include "KiwiJucePatcherView.h"

namespace Kiwi
{
    // ================================================================================ //
    //                              PATCHER CONTROLLER                                  //    
    // ================================================================================ //
    
    jPatcherView::jPatcherView(sPatcher patcher) noexcept :
    GuiController(patcher), m_patcher(patcher)
    {
        shouldReceiveMouse(true);
        shouldReceiveKeyboard(true);
        shouldReceiveActions(true);
    }
    
    sjPatcherView jPatcherView::create(sPatcher patcher) noexcept
    {
        sjPatcherView ctrl = make_shared<jPatcherView>(patcher);
        if(ctrl)
        {
            patcher->addListener(ctrl);
            ctrl->m_selection = make_shared<Selection>(patcher, ctrl);
            ctrl->m_lasso = make_shared<Lasso>(patcher, ctrl, ctrl->m_selection);
        }
        return ctrl;
    }
    
    jPatcherView::~jPatcherView() noexcept
    {
        m_object_handlers.clear();
        m_link_handlers.clear();
    }
    
    void jPatcherView::objectCreated(sPatcher patcher, sObject object)
    {
        if (patcher && patcher == m_patcher)
        {
            lock_guard<mutex> guard(m_mutex);
            jPatcherView::sObjectHandler handler = make_shared<jPatcherView::ObjectHandler>(m_patcher, object);
            m_object_handlers.push_back(handler);
            //m_patcher->addChildForView(getView(), handler);
        }
    }
    
    void jPatcherView::objectRemoved(sPatcher patcher, sObject object)
    {
        if (patcher && object && patcher == m_patcher)
        {
            lock_guard<mutex> guard(m_mutex);
            for(auto it = m_object_handlers.begin(); it != m_object_handlers.end(); it++)
            {
                sObjectHandler handler = (*it);
                if(handler && handler->getObject() == object)
                {
                    //patcher->removeChildForView(getView(), handler);
                    m_object_handlers.erase(it);
                }
            }
        }
    }
    
    void jPatcherView::linkCreated(sPatcher patcher, sLink link)
    {
        /*
        if (patcher && link && patcher == m_patcher)
        {
            jPatcherView::sLinkHandler handler = make_shared<jPatcherView::LinkHandler>(patcher, link);
            m_link_handlers.push_back(handler);
            patcher->GuiModel::addChild(handler);
        }
        */
    }
    
    void jPatcherView::linkRemoved(sPatcher patcher, sLink link)
    {
        /*
        if (patcher && link && patcher == m_patcher)
        {
            for(auto it = m_link_handlers.begin(); it != m_link_handlers.end(); it++)
            {
                sLinkHandler handler = (*it);
                if(handler && handler->getLink() == link)
                {
                    patcher->removeChild(handler);
                    m_link_handlers.erase(it);
                }
            }
        }
        */
    }
    
    void jPatcherView::selectionChanged() noexcept
    {
        m_listeners.call(&Listener::selectionChanged, getShared(), m_selection);
    }
    
    // ================================================================================ //
    //									PRESENTATION                                    //
    // ================================================================================ //
    
    void jPatcherView::setZoom(ulong zoom)
    {
        m_zoom = clip(zoom, 1ul, 1000ul);
    }
    
    void jPatcherView::setLockStatus(bool locked)
    {
        if(m_locked != locked)
        {
            m_locked = locked;
            
            if(m_lasso->isDragging())
            {
                m_lasso->end();
            }
            
            m_selection->removeAll();
            redraw();
            
            /*
            m_listeners_mutex.lock();
            auto it = m_listeners.begin();
            while(it != m_listeners.end())
            {
                if((*it).expired())
                {
                    it = m_listeners.erase(it);
                }
                else
                {
                    sListener listener = (*it).lock();
                    listener->patcherViewLockStatusChanged();
                    ++it;
                }
            }
            m_listeners_mutex.unlock();
            */
            //unselectAll();
            //lockStatusChanged();
        }
    }
    
    void jPatcherView::draw(sGuiView view, Sketch& sketch)
    {
        const bool locked = getLockStatus();
        const Color bgcolor = locked ? m_patcher->getLockedBackgroundColor() : m_patcher->getUnlockedBackgroundColor();
        sketch.fillAll(bgcolor);
        if(!locked)
        {
            const int grid_size = m_patcher->getGridSize();
            const Rectangle bounds = sketch.getBounds();
            sketch.setColor((bgcolor.contrasted(0.5)).withAlpha(0.7));
            for(int x = bounds.x() - (int(bounds.x()) % grid_size); x < bounds.right(); x += grid_size)
            {
                for(int y = bounds.y() - (int(bounds.y()) % grid_size) ; y < bounds.bottom(); y += grid_size)
                {
                    sketch.drawPoint(x, y);
                }
            }
        }
    }
    
    // ================================================================================ //
    //                                      MOUSE                                       //
    // ================================================================================ //
    
    bool jPatcherView::receive(sGuiView view, MouseEvent const& event)
    {
        switch (event.getType())
        {
            case MouseEvent::Enter:         return mouseEnter(event);
            case MouseEvent::Leave:         return mouseLeave(event);
            case MouseEvent::Move:          return mouseMove(event);
            case MouseEvent::Drag:          return mouseDrag(event);
            case MouseEvent::Down:          return mouseDown(event);
            case MouseEvent::Up:            return mouseUp(event);
            case MouseEvent::DoubleClick:   return mouseDoubleClick(event);
            case MouseEvent::Wheel:         return mouseWeel(event);
                
            default: break;
        }
        
        return false;
    }
    
    bool jPatcherView::mouseEnter(MouseEvent const& event)
    {
        return true;
    }
    
    bool jPatcherView::mouseLeave(MouseEvent const& event)
    {
        return true;
    }
    
    bool jPatcherView::mouseDrag(MouseEvent const& event)
    {
        if(!m_locked)
        {
            m_lasso->drag(event.getPosition(), true, event.hasAlt(), event.hasShift());
            return true;
        }
        return false;
    }

    bool jPatcherView::mouseMove(MouseEvent const& event)
    {
        return true;
    }
    
    bool jPatcherView::mouseDown(MouseEvent const& event)
    {
        if(!m_locked)
        {
            m_lasso->start(event.getPosition(), false);
            return true;
        }
        return false;
    }
    
    bool jPatcherView::mouseUp(MouseEvent const& event)
    {
        if(m_lasso->isDragging())
        {
            m_lasso->end();
            return true;
        }
        
        return false;
    }
    
    bool jPatcherView::mouseDoubleClick(MouseEvent const& event)
    {
        performAction(newObject);
        return true;
    }
    
    bool jPatcherView::mouseWeel(MouseEvent const& event)
    {
        return false;
    }
    
    bool jPatcherView::receive(sGuiView view, KeyboardEvent const& event)
    {
        return true;
    }
    
    bool jPatcherView::receive(sGuiView view, KeyboardFocus const focus)
    {
        return true;
    }
    
    // ================================================================================ //
    //                                      ACTIONS                                     //
    // ================================================================================ //
    
    vector<Action::Code> jPatcherView::getActionCodes()
    {
        return vector<Action::Code>({newBang, newObject, editModeSwitch});
    }
    
    Action jPatcherView::getAction(const ulong code)
    {
        switch(code)
        {
            case editModeSwitch:
                return Action(KeyboardEvent(KeyboardEvent::Cmd, L'e'), "Edit", "Switch between edit and play mode", ActionCategories::editing);
            case newBang:
                return Action(KeyboardEvent(KeyboardEvent::Nothing, L'b'), "New Bang", "Add a new bang in the patcher", ActionCategories::editing);
            case newObject:
                return Action(KeyboardEvent(KeyboardEvent::Nothing, L'n'), "New Object", "Add a new object in the patcher", ActionCategories::editing);
                
            default: return Action();
        }
    }
    
    bool jPatcherView::performAction(const ulong code)
    {
        switch(code)
        {
            case editModeSwitch:
                setLockStatus(!getLockStatus());
                return true;
            case newBang:
                createObject("bang", getMouseRelativePosition());
                return true;
            case newObject:
                createObject("newobject", getMouseRelativePosition());
                return true;
                
            default: return false;
        }
    }
    
    void jPatcherView::createObject(string const& name, Point const& pos)
    {
        const Dico dico({pair<sTag, Atom>(Tags::objects, Vector({Dico({pair<sTag, Atom>(Tags::name, Tag::create(name)), pair<sTag, Atom>(Tags::text, Tag::create(name)), pair<sTag, Atom>(Tags::position, {pos.x(), pos.y()})})}))});
        m_patcher->add(dico);
    }
    
    // ================================================================================ //
    //                                PATCHER SELECTION                                 //
    // ================================================================================ //
    
    bool jPatcherView::Selection::has(sObject object)
    {
        return object && (m_objects.find(object) != m_objects.end());
    }
    
    bool jPatcherView::Selection::has(sLink link)
    {
        return link && (m_links.find(link) != m_links.end());
    }
    
    bool jPatcherView::Selection::addAllObjects()
    {
        sPatcher patcher = getPatcher();
        if (patcher)
        {
            bool changed = false;
            lock_guard<mutex> guard(m_mutex);
            for(auto object : patcher->getObjects())
            {
                if(object && m_objects.insert(object).second)
                {
                    //int todo_setSelectedStatus;
                    //object->setSelectedStatus(true);
                    changed = true;
                }
            }
            
            if(changed)
            {
                selectionChanged();
            }
            
            return true;
        }
        
        return false;
    }
    
    bool jPatcherView::Selection::addAllLinks()
    {
        sPatcher patcher = getPatcher();
        if (patcher)
        {
            bool changed = false;
            lock_guard<mutex> guard(m_mutex);
            
            for(auto link : patcher->getLinks())
            {
                if(link && m_links.insert(link).second)
                {
                    //int todo_setSelectedStatus;
                    //link->setSelectedStatus(true);
                    changed = true;
                }
            }
            
            if(changed)
            {
                selectionChanged();
            }
            
            return true;
        }
        
        return false;
    }
    
    void jPatcherView::Selection::removeAll(const bool notify)
    {
        if(isAnythingSelected())
        {
            removeAllObjects(false);
            removeAllLinks(false);
            
            if(notify)
            {
                selectionChanged();
            }
        }
    }
    
    bool jPatcherView::Selection::removeAllObjects(const bool notify)
    {
        bool sendChange = false;
        if(!m_objects.empty())
        {
            for(auto elem : m_objects)
            {
                sObject object = elem.lock();
                if(object)
                {
                    //int todo_setSelectedStatus;
                    //object->setSelectedStatus(false);
                    sendChange = true;
                }
            }
            
            {
                lock_guard<mutex> guard(m_mutex);
                m_objects.clear();
            }
            
            if(notify && sendChange)
            {
                selectionChanged();
            }
            
            return true;
        }
        return false;
    }
    
    bool jPatcherView::Selection::removeAllLinks(const bool notify)
    {
        bool sendChange = false;
        if(!m_links.empty())
        {
            for(auto elem : m_links)
            {
                sLink link = elem.lock();
                if(link)
                {
                    //int todo_setSelectedStatus;
                    //link->setSelectedStatus(false);
                    sendChange = true;
                }
            }
            
            {
                lock_guard<mutex> guard(m_mutex);
                m_links.clear();
            }
            
            if(notify && sendChange)
            {
                selectionChanged();
            }
            
            return true;
        }
        return false;
    }
    
    void jPatcherView::Selection::add(vector<sObject>& objects)
    {
        bool notify = false;
        if(!objects.empty())
        {
            for(auto object : objects)
            {
                if(object && add(object, false))
                {
                    notify = true;
                }
            }
            
            if(notify)
            {
                selectionChanged();
            }
        }
    }
    
    void jPatcherView::Selection::add(vector<sLink>& links)
    {
        bool notify = false;
        if(!links.empty())
        {
            for(auto link : links)
            {
                if(link && add(link, false))
                {
                    notify = true;
                }
            }
            
            if(notify)
            {
                selectionChanged();
            }
        }
    }
    
    bool jPatcherView::Selection::add(sObject object, const bool notify)
    {
        if(object)
        {
            {
                lock_guard<mutex> guard(m_mutex);
                if(m_objects.insert(object).second)
                {
                    //int todo_setSelectedStatus;
                    //object->setSelectedStatus(true);
                }
            }
            
            if(notify)
            {
                selectionChanged();
            }
            return true;
        }
        return false;
    }
    
    bool jPatcherView::Selection::add(sLink link, const bool notify)
    {
        if(link)
        {
            {
                lock_guard<mutex> guard(m_mutex);
                if(m_links.insert(link).second)
                {
                    //int todo_setSelectedStatus;
                    //link->setSelectedStatus(true);
                }
            }
            
            if(notify)
            {
                selectionChanged();
            }
            return true;
        }
        return false;
    }
    
    bool jPatcherView::Selection::set(sObject object)
    {
        removeAll();
        return add(object, true);
    }
    
    bool jPatcherView::Selection::set(sLink link)
    {
        removeAll();
        return add(link, true);
    }
    
    void jPatcherView::Selection::remove(vector<sObject>& objects)
    {
        bool notify = false;
        if(isAnyObjectSelected() && !objects.empty())
        {
            for(auto object : objects)
            {
                if(object && remove(object, false))
                {
                    notify = true;
                }
            }
            
            if(notify)
            {
                selectionChanged();
            }
        }
    }
    
    void jPatcherView::Selection::remove(vector<sLink>& links)
    {
        bool notify = false;
        if(isAnyLinkSelected() && !links.empty())
        {
            for(auto link : links)
            {
                if(link && remove(link, false))
                {
                    notify = true;
                }
            }
            
            if(notify)
            {
                selectionChanged();
            }
        }
    }
    
    bool jPatcherView::Selection::remove(sObject object, const bool notify)
    {
        if(object)
        {
            {
                lock_guard<mutex> guard(m_mutex);
                if(m_objects.erase(object))
                {
                    //int todo_setSelectedStatus;
                    //object->setSelectedStatus(false);
                }
            }

            if(notify)
            {
                selectionChanged();
            }
            return true;
        }
        return false;
    }
    
    bool jPatcherView::Selection::remove(sLink link, const bool notify)
    {
        if(link)
        {
            {
                lock_guard<mutex> guard(m_mutex);
                if(m_links.erase(link))
                {
                    //int todo_setSelectedStatus;
                    //link->setSelectedStatus(false);
                }
            }
            
            if(notify)
            {
                selectionChanged();
            }
            return true;
        }
        return false;
    }
    
    // ================================================================================ //
    //                                  PATCHER LASSO                                   //
    // ================================================================================ //
    
    jPatcherView::Lasso::Lasso(sPatcher patcher, sjPatcherView pctrl, sSelection selection) noexcept :
    GuiModel(patcher->GuiModel::getContext()), m_patcher(patcher), m_owner_ctrl(pctrl), m_selection(selection)
    {
        ;
    }
    
    void jPatcherView::Lasso::addToPatcher() noexcept
    {
        if(!m_active)
        {
            sPatcher p = m_patcher.lock();
            if(p)
            {
                //p->addChild(static_pointer_cast<GuiModel>(shared_from_this()));
                m_active = true;
            }
        }
    }
    
    void jPatcherView::Lasso::removeFromPatcher() noexcept
    {
        if(m_active)
        {
            sPatcher p = m_patcher.lock();
            if(p)
            {
                //p->removeChild(static_pointer_cast<GuiModel>(shared_from_this()));
                m_active = false;
            }
        }
    }
    
    void jPatcherView::Lasso::start(Point const& point, const bool preserve) noexcept
    {
        // You need to call end() before to call start again to avoid this assertion.
        assert(!m_dragging);

        const sSelection selection = m_selection.lock();
        if(selection)
        {
            m_startpos = point;
            //int todo;
            //setBounds(Rectangle(point, Size()));
            addToPatcher();
            
            if(!preserve)
            {
                selection->removeAll();
            }
            else
            {
                lock_guard<mutex> guard(m_mutex);
                
                for(auto elem : selection->getObjects())
                {
                    sObject object = elem.lock();
                    if(object)
                    {
                        m_objects.insert(object);
                    }
                }
                
                for(auto elem : selection->getLinks())
                {
                    sLink link = elem.lock();
                    if(link)
                    {
                        m_links.insert(link);
                    }
                }
            }
        }
        
        m_dragging = true;
    }
    
    void jPatcherView::Lasso::drag(Point const& point, const bool includeObjects, const bool includeLinks, const bool preserve) noexcept
    {
        const scPatcher patcher = m_patcher.lock();
        const sSelection selection = m_selection.lock();
        if(patcher && selection)
        {
            //int todo;
            //setBounds(Rectangle::withCorners(m_startpos, point));
            bool changed = false;
            
            if(preserve)
            {
                if(includeObjects)
                {
                    vector<sObject> lassoObjects;
                    //int todo_lassoHitTest;
                    //knockObjects(getBounds(), lassoObjects);
                    
                    lock_guard<mutex> guard(m_mutex);
                    for(auto object : patcher->getObjects())
                    {
                        if(object)
                        {
                            const bool isSelected = selection->has(object);
                            const bool wasSelected = m_objects.find(object) != m_objects.end();
                            const bool inLasso = find(lassoObjects.begin(), lassoObjects.end(), object) != lassoObjects.end();
                            
                            if(!isSelected && (wasSelected != inLasso))
                            {
                                selection->add(object, false);
                                changed = true;
                            }
                            else if(isSelected && (wasSelected == inLasso))
                            {
                                selection->remove(object, false);
                                changed = true;
                            }
                        }
                    }
                }
                if(includeLinks)
                {
                    vector<sLink> lassoLinks;
                    //int todo_lassoHitTest;
                    //knockObjects(getBounds(), lassoLinks);
                    
                    lock_guard<mutex> guard(m_mutex);
                    for(auto link : patcher->getLinks())
                    {
                        if(link)
                        {
                            const bool isSelected = selection->has(link);
                            const bool wasSelected = m_links.find(link) != m_links.end();
                            const bool inLasso = find(lassoLinks.begin(), lassoLinks.end(), link) != lassoLinks.end();
                            
                            if(!isSelected && (wasSelected != inLasso))
                            {
                                selection->add(link, false);
                                changed = true;
                            }
                            else if(isSelected && (wasSelected == inLasso))
                            {
                                selection->remove(link, false);
                                changed = true;
                            }
                        }
                    }
                }
                
                if(changed)
                {
                    //int todo_notify_changes;
                    //selectionChanged();
                }
            }
            else
            {
                selection->removeAll(false);
                if(includeObjects)
                {
                    vector<sObject> lassoObjects;
                    //int todo_lassoHitTest;
                    //knockObjects(getBounds(), lassoObjects);
                    selection->add(lassoObjects);
                }
                
                if(includeLinks)
                {
                    vector<sLink> lassoLinks;
                    //int todo_lassoHitTest;
                    //knockLinks(getBounds(), lassoLinks);
                    selection->add(lassoLinks);
                }
            }
        }
    }
    
    void jPatcherView::Lasso::end() noexcept
    {
        m_objects.clear();
        m_links.clear();
        m_dragging = false;
        removeFromPatcher();
    }
    
    void jPatcherView::Lasso::draw(scGuiView view, Sketch& sketch) const
    {
        scGuiView parent = view->getParent();
        if(parent)
        {
            scjPatcherView pctrl = dynamic_pointer_cast<jPatcherView>(parent->getController());
            if (pctrl && !pctrl->getLockStatus())
            {
                Color color = Color(0.96, 0.96, 0.96);
                
                if (pctrl != m_owner_ctrl.lock())
                {
                    color = Color(0.96, 0.4, 0.96);
                    //color = m_patcher->getUIColorFor(m_owner_ctrl);
                }
                
                sketch.fillAll(color.withAlpha(0.4));
                sketch.setColor(color);
                //sketch.drawRectangle(getBounds().withZeroOrigin(), 1.);
            }
        }
    }
    
    
    
    jPatcherView::ObjectHandler::ObjectHandler(sPatcher patcher, sObject object) noexcept :
    GuiModel(patcher->GuiModel::getContext()), m_patcher(patcher), m_object(object)
    {
        //setBounds(object->getBounds().expanded(2));
        object->setPosition(Point(2, 2));
        addChild(object);
    }
    
    jPatcherView::ObjectHandler::~ObjectHandler() noexcept
    {
        
    }
    
    void jPatcherView::ObjectHandler::draw(scGuiView view, Sketch& sketch) const
    {
        sketch.fillAll(Colors::blue.withAlpha(0.5));
    }
}




