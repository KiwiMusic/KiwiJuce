/*
 ==============================================================================
 
 This file is part of the KIWI library.
 Copyright(c) 2014 Pierre Guillot & Eliott Paris.
 
 Permission is granted to use this software under the terms of either:
 a) the GPL v2(or any later version)
 b) the Affero GPL v3
 
 Details of these licenses can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 To release a closed-source product which uses KIWI, contact : guillotpierre6@gmail.com
 
 ==============================================================================
 */

#include "ConsoleComponent.h"
#include "Application.h"

namespace Kiwi
{
    shared_ptr<Console::History> ConsoleComponent::m_history = Console::History::create();
    
    // ================================================================================ //
    //                                  CONSOLE COMPONENT                               //
    // ================================================================================ //
    
    ConsoleComponent::ConsoleComponent() :
    m_font(13.f)
    {
        TableHeaderComponent* header = new TableHeaderComponent();
        header->addColumn(String("ID"),        Column::Id, 30, 20, 50, TableHeaderComponent::defaultFlags, -1);
        header->addColumn(String("Type"),      Column::Type, 30, 20, 50, TableHeaderComponent::defaultFlags, -1);
        header->addColumn(String("Object"),    Column::Object, 60, 20, 10000, TableHeaderComponent::defaultFlags, -1);
        header->addColumn(String("Message"),   Column::Message, 100, 40, 10000, TableHeaderComponent::defaultFlags, -1);
        header->setStretchToFitActive(false);
        header->setColumnVisible(1, false);
        header->setColumnVisible(2, false);
        header->addListener(this);
        
        m_table.setWantsKeyboardFocus(true);
        m_table.setMultipleSelectionEnabled(true);
        m_table.setMouseMoveSelectsRows(false);
        m_table.setHeaderHeight(m_font.getHeight() + 15);
        m_table.setRowHeight(m_font.getHeight() + 5);
        m_table.setColour(ListBox::ColourIds::backgroundColourId, Colours::transparentWhite);
        m_table.getViewport()->setScrollBarsShown(true, false, true, false);
        m_table.getViewport()->setScrollBarThickness(10);
        m_table.setModel(this);
        m_table.setHeader(header);
        addAndMakeVisible(m_table);
        
        addKeyListener(Application::getCommandManager().getKeyMappings());

    }
    
    ConsoleComponent::~ConsoleComponent()
    {
        ;
    }
    
    shared_ptr<ConsoleComponent> ConsoleComponent::create()
    {
        shared_ptr<ConsoleComponent> component = make_shared<ConsoleComponent>();
        m_history->bind(component);
        return component;
    }

    // ================================================================================ //
    //                                      COMMAND                                     //
    // ================================================================================ //
    
    void ConsoleComponent::copy()
    {
        String text;
        SparseSet<int> selection = m_table.getSelectedRows();
        for(size_t i = 0; i < selection.size(); i++)
        {
            sConsoleMessage mess = m_history->get(selection[i]);
            if(mess && !mess->content.empty())
            {
                shared_ptr<const Box> box = mess->box.lock();
                if(box)
                {
                    text += toString(box) + " : ";
                }
                text += mess->content + "\n";
            }
        }
        SystemClipboard::copyTextToClipboard(text);
    }
    
    void ConsoleComponent::erase()
    {
        SparseSet<int> selection = m_table.getSelectedRows();
        vector<size_t> select;
        for(size_t i = 0; i < selection.size(); i++)
        {
            select.push_back(selection[i]);
        }
        m_history->erase(select);
        m_table.setVerticalPosition(0);
        Application::commandStatusChanged();
    }
    
    // ================================================================================ //
    //                                  HISTORY LISTENER                                //
    // ================================================================================ //
    
    void ConsoleComponent::historyHasChanged(shared_ptr<Console::History> history)
    {
        m_table.updateContent();
    }
    
    // ================================================================================ //
    //                                      COMPONENT                                   //
    // ================================================================================ //
    
    void ConsoleComponent::resized()
    {
        m_table.setBounds(getLocalBounds());
        updateRighmostColumnWidth(&m_table.getHeader());
    }
    
    void ConsoleComponent::paint(Graphics& g)
    {
        int width   = m_table.getVisibleContentWidth();
        int rowheight = m_table.getRowHeight();
        g.setColour(Colours::black.withAlpha(0.15f));
        for(int i = m_table.getHeaderHeight() - 1; i < getHeight(); i+= rowheight)
        {
             g.drawHorizontalLine(i, 0, width);
        }
    }
    
    // ================================================================================ //
    //                        APPLICATION COMMAND TARGET                                //
    // ================================================================================ //
    
    ApplicationCommandTarget* ConsoleComponent::getNextCommandTarget()
    {
        return findFirstTargetParentComponent();
    }
    
    void ConsoleComponent::getAllCommands(Array<CommandID>& commands)
    {
        /*
        commands.add(StandardApplicationCommandIDs::cut);
        commands.add(StandardApplicationCommandIDs::copy);
        commands.add(StandardApplicationCommandIDs::del);
        commands.add(StandardApplicationCommandIDs::selectAll);
         */
        // Todo save
    }
    
    void ConsoleComponent::getCommandInfo(const CommandID commandID, ApplicationCommandInfo& result)
    {
        const bool rowSelected = m_table.getNumSelectedRows();
        switch(commandID)
        {
                /*
            case StandardApplicationCommandIDs::cut:
                result.setInfo(TRANS("Cut"), String::empty, CommandCategories::editing, 0);
                result.addDefaultKeypress('x', ModifierKeys::commandModifier);
                result.setActive(rowSelected);
                break;
                
            case StandardApplicationCommandIDs::copy:
                result.setInfo(TRANS("Copy"), String::empty, CommandCategories::editing, 0);
                result.addDefaultKeypress('c', ModifierKeys::commandModifier);
                result.setActive(rowSelected);
                break;
                
            case StandardApplicationCommandIDs::del:
                result.setInfo(TRANS("Delete"), String::empty, CommandCategories::editing, 0);
                result.setActive(rowSelected);
                break;
                
            case StandardApplicationCommandIDs::selectAll:
                result.setInfo(TRANS("Select All"), String::empty, CommandCategories::editing, 0);
                result.addDefaultKeypress('a', ModifierKeys::commandModifier);
                break;
            
            case StandardApplicationCommandIDs::selectAll:
                result.setInfo(TRANS("Save"), String::empty, CommandCategories::editing, 0);
                result.addDefaultKeypress('s', ModifierKeys::commandModifier);
                break;
            */
            default:
                break;
        }
    }
    
    bool ConsoleComponent::perform(const InvocationInfo& info)
    {
        switch(info.commandID)
        {
            case StandardApplicationCommandIDs::cut:
            {
                copy();
                erase();
                break;
            }
            case StandardApplicationCommandIDs::copy:
            {
                copy();
                break;
            }
            case StandardApplicationCommandIDs::del:
            {
                erase();
                break;
            }
            case StandardApplicationCommandIDs::selectAll:
            {
                SparseSet<int> rows;
                rows.addRange(Range<int>(0, m_history->size()));
                m_table.setSelectedRows(rows);
                break;
            }
            // Todo save
            default:
                return false;
        }
        
        return true;
    }
    
    // ================================================================================ //
    //                              TABLE LIST BOX MODEL                                //
    // ================================================================================ //
    
    void ConsoleComponent::selectedRowsChanged(int row)
    {
        Application::commandStatusChanged();
    }
    
    void ConsoleComponent::deleteKeyPressed(int lastRowSelected)
    {
        erase();
    }
    
    void ConsoleComponent::backgroundClicked(const MouseEvent& mouse)
    {
        m_table.deselectAllRows();
    }
    
    int ConsoleComponent::getNumRows()
    {
        return m_history->size();
    }
    
    void ConsoleComponent::paintRowBackground(Graphics& g, int rowNumber, int width, int height, bool selected)
    {
        sConsoleMessage mess = m_history->get(rowNumber);
        if(mess)
        {
            if(selected)
            {
                g.fillAll(Colours::lightsteelblue);
            }
            else if(mess->kind == Console::Message::Error)
            {
                g.fillAll(Colours::lightpink);
            }
            else if(mess->kind == Console::Message::Warning)
            {
                g.fillAll(Colours::lightgoldenrodyellow);
            }
            else
            {
                g.fillAll(Colour(0xfffefefe));
            }
        }
        else
        {
            g.fillAll(Colour(0xfffefefe));
        }
    
        g.setColour(Colours::black.withAlpha(0.15f));
        g.drawLine(0, height, width, height);
    }
    
    void ConsoleComponent::paintOverChildren(Graphics &g)
    {
        int numColumns = m_table.getHeader().getNumColumns(true);
        float left = 0, width = 0;
        
        for(int i = 0; i < numColumns; i++)
        {
            width = m_table.getHeader().getColumnWidth(m_table.getHeader().getColumnIdOfIndex(i, true));
            
            if(m_table.getVisibleContentWidth() >= width + left)
            {
                g.setColour(Colours::black.withAlpha(0.15f));
                g.drawLine(left + width, m_table.getHeaderHeight(), left + width, getHeight());
            }
            
            left += width;
        }
    }
    
    void ConsoleComponent::paintCell(Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected)
    {
        g.setColour(Colours::black.brighter(0.4));
        g.setFont(m_font);
        sConsoleMessage mess = m_history->get(rowNumber);
        if(mess)
        {
            switch (columnId)
            {
                case Column::Object:
                {
                    shared_ptr<const Box> box = mess->box.lock();
                    if(box)
                    {
                        g.drawText(toString(box), 2, 0, width - 4, height, Justification::centredLeft, true);
                    }
                }
                    break;
                case Column::Message:
                    g.drawText(mess->content, 2, 0, width - 4, height, Justification::centredLeft, true);
                    break;
                    
                default:
                    break;
            }
        }
    }
    
    void ConsoleComponent::sortOrderChanged(int newSortColumnId, bool isForwards)
    {
        switch(newSortColumnId)
        {
            case 1 :
                m_history->sort(Console::History::Index);
                break;
            case 2 :
                m_history->sort(Console::History::Kind);
                break;
            case 3 :
                m_history->sort(Console::History::Name);
                break;
            case 4 :
                m_history->sort(Console::History::Content);
                break;
            default:
                break;
        }
        m_table.updateContent();
    }
    
    void ConsoleComponent::cellDoubleClicked(int rowNumber, int columnId, const MouseEvent& mouse)
    {
        // TODO : hilight(if possible) object corresponding to the dblclicked row
    }
    
    // This is overloaded from TableListBoxModel, and must update any custom components that we're using
    Component* ConsoleComponent::refreshComponentForCell(int rowNumber, int columnId, bool /*isRowSelected*/,
                                                          Component* existingComponentToUpdate)
    {
        // Just return 0, as we'll be painting these columns directly.
        jassert(existingComponentToUpdate == 0);
        return 0;
    }
    
    // This is overloaded from TableListBoxModel, and should choose the best width for the specified
    // column.
    int ConsoleComponent::getColumnAutoSizeWidth(int columnId)
    {
        if(columnId == Column::Id)
            return 30;
        
        int widest = 32;
        // find the widest bit of text in this column..
        for(int i = getNumRows(); --i >= 0;)
        {
            sConsoleMessage mess = m_history->get(i);
            if(mess)
            {
                widest = max(widest, m_font.getStringWidth(mess->content));
            }
        }
        return widest + 8;
    }
    
    void ConsoleComponent::tableColumnsResized(TableHeaderComponent* tableHeader)
    {
        if(tableHeader == &m_table.getHeader())
        {
            updateRighmostColumnWidth(&m_table.getHeader());
        }
    }
    
    void ConsoleComponent::updateRighmostColumnWidth(TableHeaderComponent* header)
    {
        int rightmostColumnId   = 0;
        int rightmostColumnX    = 0;
        
        for(int i = 0; i <= header->getNumColumns(false) - 1; i++)
        {
            if(header->isColumnVisible(i) && rightmostColumnId <= header->getIndexOfColumnId(i, true))
            {
                rightmostColumnId = i;
            }
        }
        rightmostColumnId   = header->getColumnIdOfIndex(header->getNumColumns(true)-1, true);
        rightmostColumnX    = header->getTotalWidth() - header->getColumnWidth(rightmostColumnId);
        
        if(rightmostColumnX <= getWidth())
        {
            m_table.getHeader().setColumnWidth(rightmostColumnId, getWidth() - rightmostColumnX);
        }
    }
    
    // ================================================================================ //
    //                                  CONSOLE WINDOW                                  //
    // ================================================================================ //
    /*
    ConsoleWindow::ConsoleWindow(PropertiesFile* file) :
    BaseWindow("Console", Colours::white, minimiseButton | closeButton, true),
    m_component(ConsoleComponent::create()),
    m_property_file(file)
    {
        setContentNonOwned(m_component.get(), false);
        setResizable(true, false);
        setResizeLimits(50, 50, 32000, 32000);
        setTopLeftPosition(0, 0);
        setSize(300, 440);
        setVisible(true);
        //restoreWindowStateFromString(m_property_file->getValue("consoleWindowPos", ""));
        //setVisible(m_property_file->getBoolValue("consoleIsVisible", true));
        //m_component.restoreState(m_property_file);
    }
    
    ConsoleWindow::~ConsoleWindow()
    {
        //m_property_file->setValue("consoleWindowPos", getWindowStateAsString());
        //m_property_file->setValue("consoleIsVisible", isVisible());
        //m_component.saveState(m_property_file);
    }
    
    void ConsoleWindow::closeButtonPressed()
    {
        setVisible(false);
    }*/
}