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

#ifndef __DEF_KIWI_JUCE_PATCHER_VIEW__
#define __DEF_KIWI_JUCE_PATCHER_VIEW__

#include "../KiwiJuce.h"

namespace Kiwi
{
    // ================================================================================ //
    //                              PATCHER CONTROLLER                                  //
    // ================================================================================ //

    class jPatcherView : public GuiController, public Patcher::Listener
    {
    public:
        class Listener;
        typedef shared_ptr<Listener>            sListener;
        typedef weak_ptr<Listener>              wListener;
        typedef shared_ptr<const Listener>      scListener;
        typedef weak_ptr<const Listener>        wcListener;
        
        class Selection;
        typedef shared_ptr<Selection>           sSelection;
        typedef weak_ptr<Selection>             wSelection;
        typedef shared_ptr<const Selection>     scSelection;
        typedef weak_ptr<const Selection>       wcSelection;
        
        class Lasso;
        typedef shared_ptr<Lasso>               sLasso;
        typedef weak_ptr<Lasso>                 wLasso;
        typedef shared_ptr<const Lasso>         scLasso;
        typedef weak_ptr<const Lasso>           wcLasso;
        
        class ObjectHandler;
        typedef shared_ptr<ObjectHandler>       sObjectHandler;
        typedef weak_ptr<ObjectHandler>         wObjectHandler;
        typedef shared_ptr<const ObjectHandler> scObjectHandler;
        typedef weak_ptr<const ObjectHandler>   wcObjectHandler;
        
        class LinkHandler;
        typedef shared_ptr<LinkHandler>         sLinkHandler;
        typedef weak_ptr<LinkHandler>           wLinkHandler;
        typedef shared_ptr<const LinkHandler>   scLinkHandler;
        typedef weak_ptr<const LinkHandler>     wcLinkHandler;
        
    private:
        const sPatcher          m_patcher;
        vector<sObjectHandler>  m_object_handlers;
        vector<sLinkHandler>    m_link_handlers;
        mutable mutex           m_mutex;
        sSelection              m_selection;
        sLasso                  m_lasso;
        ulong                   m_zoom;
        bool                    m_locked;
        bool                    m_presentation;
        bool                    m_display_grid;
        bool                    m_snap_to_grid;
        ListenerSet<Listener>   m_listeners;
        
        void selectionChanged() noexcept;

    public:
        
        //! The patcher controller constructor.
        /** The function does nothing.
         @param patcher The patcher.
         */
        jPatcherView(sPatcher patcher) noexcept;
        
        //! The controller destructor.
        /** The function does nothing.
         */
        ~jPatcherView() noexcept;
        
        //! The controller creation method.
        /** The function allocates a controller.
         @return The controller.
         */
        static sjPatcherView create(sPatcher patcher) noexcept;
        
        //! Retrieve the shared pointer of the controller.
        /** The function retrieves the shared pointer of the controller.
         @return The shared pointer of the controller.
         */
        inline scjPatcherView getShared() const noexcept
        {
            return static_pointer_cast<const jPatcherView>(shared_from_this());
        }
        
        //! Retrieve the shared pointer of the controller.
        /** The function retrieves the shared pointer of the controller.
         @return The shared pointer of the controller.
         */
        inline sjPatcherView getShared() noexcept
        {
            return static_pointer_cast<jPatcherView>(shared_from_this());
        }
        
        //! Add a patcher controller listener.
        /** The function adds a patcher controller listener.
         If the listener was already listening the patcher controller, the function has no effect.
         @param listener The listener to add.
         */
        void addListener(sListener listener) noexcept
        {
            m_listeners.add(listener);
        }
        
        //! Remove a patcher controller listener.
        /** The function removes a patcher controller listener.
         If the listener wasn't listening the patcher controller, the function has no effect.
         @param listener The listener to add.
         */
        void removeListener(sListener listener) noexcept
        {
            m_listeners.remove(listener);
        }
        
        // ================================================================================ //
        //										PRESENTATION                                //
        // ================================================================================ //
        
        //! Retrieve the zoom of the patcher.
        /** The function retrieves the zoom of the patcher.
         @return the zoom of the patcher in percent.
         @see setZoom
         */
        inline ulong getZoom() const noexcept
        {
            return m_zoom;
        }
        
        //! Set the zoom of the patcher.
        /** The function sets the zoom of the patcher.
         @param zoom The zoom of the patcher in percent.
         @see getZoom
         */
        void setZoom(const ulong zoom);
        
        //! Retrieve if the patcher is locked or unlocked.
        /** The function retrieves if the patcher is locked or unlocked.
         @return True if the patcher is locked, false if it is unlocked.
         @see setLockStatus
         */
        inline bool getLockStatus() const noexcept
        {
            return m_locked;
        }
        
        //! Lock or unlock the patcher.
        /** The function locks or unlocks the patcher.
         @param locked True to lock the patcher, false to unlock it.
         @see getLockStatus
         */
        void setLockStatus(const bool locked);
        
    protected:
        
        //! The draw method that should be override.
        /** The function shoulds draw some stuff.
         @param view    The view that owns the controller.
         @param sketch  The sketch to draw.
         */
        void draw(sGuiView view, Sketch& sketch) override;
        
        //! The mouse receive method.
        /** The function pass the mouse event to the model if it inherits from mouser.
         @param event    A mouser event.
         @return true if the class has done something with the event otherwise false
         */
        bool receive(sGuiView view, MouseEvent const& event) override;
        
        //! The keyboard receive method.
        /** The function pass the keyboard event to the model if it inherits from keyboarder.
         @param event    A keyboard event.
         @return true if the class has done something with the event otherwise false
         */
        bool receive(sGuiView view, KeyboardEvent const& event) override;
        
        //! The keyboard focus receive method.
        /** The function pass the keyboard event to the model if it inherits from keyboarder.
         @param event    A focus event.
         @return true if the class has done something with the event otherwise false
         */
        bool receive(sGuiView view, KeyboardFocus const event) override;
        
        //! Retrieves the action codes.
        /** The function retreives the action codes from the the manager.
         @return The action codes.
         */
        vector<Action::Code> getActionCodes() override;
        
        //! Retrieves an action from the manager.
        /** The function retreives an action from the the manager.
         @param code The code of the action.
         @return The action.
         */
        Action getAction(const ulong code) override;
        
        //! Performs an action.
        /** The function performs an action.
         @param code The code of the action.
         @return true if the action has been performed, otherwise false.
         */
        bool performAction(const ulong code) override;
        
        //! Receive the notification that an object has been created.
        /** The function is called by the patcher when an object has been created.
         @param object     The object.
         */
        void objectCreated(sPatcher patcher, sObject object) override;
        
        //! Receive the notification that an object has been removed.
        /** The function is called by the patcher when an object has been removed.
         @param object     The object.
         */
        void objectRemoved(sPatcher patcher, sObject object) override;
        
        //! Receive the notification that a link has been created.
        /** The function is called by the patcher when a link has been created.
         @param link     The link.
         */
        void linkCreated(sPatcher patcher, sLink link) override;
        
        //! Receive the notification that a link has been removed.
        /** The function is called by the patcher when a link has been removed.
         @param link    The link.
         */
        void linkRemoved(sPatcher patcher, sLink link) override;
        
    private:
        
        //@internal
        bool mouseEnter(MouseEvent const& event);
        //@internal
        bool mouseLeave(MouseEvent const& event);
        //@internal
        bool mouseDrag(MouseEvent const& event);
        //@internal
        bool mouseMove(MouseEvent const& event);
        //@internal
        bool mouseDown(MouseEvent const& event);
        //@internal
        bool mouseUp(MouseEvent const& event);
        //@internal
        bool mouseDoubleClick(MouseEvent const& event);
        //@internal
        bool mouseWeel(MouseEvent const& event);
        
        //@internal
        void createObject(string const& name, Point const& position);
    };
    
    // ================================================================================ //
    //                           PATCHER CONTROLLER LISTENER                            //
    // ================================================================================ //
    
    //! The patcher controller listener is a pure virtual class that subclasses should inherit from to receive notifications.
    /**
     The patcher controller listener is a pure virtual class that subclasses should inherit from to receive notifications related to :
      - object and link selection
     */
    class jPatcherView::Listener
    {
    public:
        //! The destructor.
        virtual ~Listener() {}
        
        //! Receive the notification that a selection has changed.
        /** The function is called by the controller when its selection has changed.
         @param controller The controller.
         @param selection The selection.
         */
        virtual void selectionChanged(sjPatcherView controller, sSelection selection) = 0;
    };
    
    // ================================================================================ //
    //                                PATCHER SELECTION                                 //
    // ================================================================================ //
    
    class jPatcherView::Selection
    {
    private:
        const wPatcher          m_patcher;
        const wjPatcherView     m_owner_ctrl;
        set<wObject,
        owner_less<wObject>>    m_objects;
        set<wLink,
        owner_less<wLink>>      m_links;
        mutable mutex           m_mutex;
        
        void selectionChanged() noexcept
        {
            m_owner_ctrl.lock()->selectionChanged();
        }
        
    public:
        
        //! The patcher selection constructor.
        /** The function allocates a patcher selection and initializes memory.
         */
        Selection(sPatcher patcher, sjPatcherView controller) noexcept : m_patcher(patcher), m_owner_ctrl(controller)
        {
            ;
        }
        
        //! Destructor.
        /** The destructor frees the selection memory.
         */
        ~Selection() noexcept
        {
            m_objects.clear();
            m_links.clear();
        }
        
        //! Retrieve the patcher.
        /** The function retrieves the patcher.
         @return The patcher.
         */
        inline sPatcher getPatcher() const noexcept
        {
            return m_patcher.lock();
        }
        
        //! Retrieves if some objects or links are currently selected.
        /** The function retrieves if some objects or links are currently selected.
         @return True if some objects or links are currently selected, false if nothing is selected.
         */
        inline bool isAnythingSelected() const noexcept
        {
            return isAnyObjectSelected() || isAnyLinkSelected();
        }
        
        //! Retrieves if some objects are currently selected.
        /** The function retrieves if some objects are currently selected.
         @return True if some objects are currently selected, false if no object is selected.
         */
        inline bool isAnyObjectSelected() const noexcept
        {
            lock_guard<mutex> guard(m_mutex);
            return !m_objects.empty();
        }
        
        //! Retrieves the number of objects currently selected.
        /** The function retrieves the number of objects currently selected.
         @return The number of objects currently selected.
         */
        inline long getNumberOfSelectedObjects() const noexcept
        {
            lock_guard<mutex> guard(m_mutex);
            return !m_objects.size();
        }
        
        //! Retrieves if some links are currently selected.
        /** The function retrieves if some links are currently selected.
         @return True if some links are currently selected, false if no object is selected.
         */
        inline bool isAnyLinkSelected() const noexcept
        {
            lock_guard<mutex> guard(m_mutex);
            return !m_links.empty();
        }
        
        //! Retrieves the selected objects.
        /** The function retrieves the selected objects.
         */
        set<wObject, owner_less<wObject>> getObjects() const noexcept
        {
            return m_objects;
        }
        
        //! Retrieves the selected links.
        /** The function retrieves the selected links.
         */
        set<wLink, owner_less<wLink>> getLinks() const noexcept
        {
            return m_links;
        }
        
        //! Retrieves if an object is selected.
        /** The function retrieve if an object is selected.
         */
        bool has(sObject object);
        
        //! Retrieves if a link is selected.
        /** The function retrieve if a link is selected.
         */
        bool has(sLink link);
        
        //! Adds all objects to selection.
        /** The function adds all objects to selection.
         */
        bool addAllObjects();
        
        //! Adds all links to selection.
        /** The function adds all links to selection.
         */
        bool addAllLinks();
        
        //! Selects a set of objects.
        /** The function selects a set of objects.
         */
        void add(vector<sObject>& objects);
        
        //! Selects a set of links.
        /** The function selects a set of links.
         */
        void add(vector<sLink>& links);
        
        //! Adds an object to the selection.
        /** The function adds an object to the selection.
         */
        bool add(sObject object, const bool notify = true);
        
        //! Adds a link to the selection.
        /** The function adds a link to the selection.
         */
        bool add(sLink link, const bool notify = true);
        
        //! Clears the selection then the selects an object.
        /** The function clears the selection then the selects an object.
         */
        bool set(sObject object);
        
        //! Clears the selection then the selects an object.
        /** The function clears the selection then the selects an object.
         */
        bool set(sLink link);
        
        //! Unselects all objects and links.
        /** The function unselects all objects and links.
         */
        void removeAll(const bool notify = true);
        
        //! Unselects all objects.
        /** The function unselects all objects.
         */
        bool removeAllObjects(const bool notify = true);
        
        //! Unselects all links.
        /** The function unselects all links.
         */
        bool removeAllLinks(const bool notify = true);
        
        //! Unselects a set of objects.
        /** The function unselects a set of objects.
         */
        void remove(vector<sObject>& objects);
        
        //! Unselects a set of links.
        /** The function unselects a set of links.
         */
        void remove(vector<sLink>& links);
        
        //! Removes an object from the selection.
        /** The function unselects object.
         */
        bool remove(sObject object, const bool notify = true);
        
        //! Removes a link from the selection.
        /** The function unselects link.
         */
        bool remove(sLink link, const bool notify = true);
    };
    
    // ================================================================================ //
    //                                  PATCHER LASSO                                   //
    // ================================================================================ //
    
    class jPatcherView::Lasso : public GuiModel
    {
    private:
        const wPatcher          m_patcher;
        const wcjPatcherView    m_owner_ctrl;
        const wSelection        m_selection;
        bool                    m_dragging;
        bool                    m_active;
        Point                   m_startpos;
        set<wObject,
        owner_less<wObject>>    m_objects;
        set<wLink,
        owner_less<wLink>>      m_links;
        mutable mutex           m_mutex;
        
        //! @internal
        void addToPatcher() noexcept;
        
        //! @internal
        void removeFromPatcher() noexcept;
        
    public:
        
        //! Constructor.
        /** The function initialises a lasso.
         @param patcher     The patcher that will hold it.
         @param selection   The patcher's selection.
         */
        Lasso(sPatcher patcher, sjPatcherView pv, jPatcherView::sSelection selection) noexcept;
        
        //! Destructor.
        inline ~Lasso() noexcept {}
        
        //! Return true if the lasso is being dragged.
        /** The function returns true if the lasso is being dragged.
         @return True if the lasso is being dragged, false otherwise.
         */
        inline bool isDragging() const noexcept {return m_dragging;}
        
        //! Initialize the selection of the links and objects.
        /** The function initialize the selection of the links and objects.
         @param point       The starting point.
         @param preserve    Pass true if you want that the lasso preserves the current selection.
         */
        void start(Point const& point, const bool preserve) noexcept;
        
        //! Perform the selection of the links and the objects.
        /** The function performs the selection of the links and the objects.
         @param point       The dragging point.
         @param objects     The lasso should add objects to the selection.
         @param links       The lasso should add links to the selection.
         @param preserve    The lasso should preserve the last selection.
         */
        void drag(Point const& point, const bool objects, const bool links, const bool preserve) noexcept;
        
        //! Finish the selection of the links and objects.
        /** The function finishes the selection of the links and objects.
         */
        void end() noexcept;
        
        //! The lasso drawing method.
        /** The function draws the lasso.
         @param view    The view that ask to draw.
         @param sketch  A sketch to draw.
         */
        void draw(scGuiView view, Sketch& sketch) const;
        
        //! Create the controller.
        /** The function creates a controller depending on the inheritance.
         @return The controller.
         */
        sGuiController createController() override {return nullptr;}
    };
    
    // ================================================================================ //
    //                                  OBJECT HANDLER                                  //
    // ================================================================================ //
    
    class jPatcherView::ObjectHandler : public GuiModel
    {
    private:
        const wPatcher  m_patcher;
        const wObject   m_object;
        
    public:
        
        //! The object's holder constructor.
        /** The function does nothing.
         @param object The object.
         */
        ObjectHandler(sPatcher patcher, sObject object) noexcept;
        
        //! The object's holder destructor.
        /** The function does nothing.
         */
        ~ObjectHandler() noexcept;
        
        //! Retrieve the object that this handler handles.
        /** The function retrieves the object that this handler handles.
         @return The object.
         */
        inline sObject getObject() const noexcept { return m_object.lock(); }
        
        //! The overrided drawing method.
        /** The function draws the object's borders and inlets.
         @param view    The view that ask to draw.
         @param sketch  A sketch to draw.
         */
        void draw(scGuiView view, Sketch& sketch) const;
        
        //! Create the controller.
        /** The function creates a controller depending on the inheritance.
         @return The controller.
         */
        sGuiController createController() override {return nullptr;}
    };
}


#endif


