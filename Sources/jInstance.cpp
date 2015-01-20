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

#include "jInstance.h"

namespace Kiwi
{
    jInstance::jInstance() :
    m_instance(Instance::create()),
    m_window(MainWindow::create())
    {
		LookAndFeel::setDefaultLookAndFeel(&m_lookandfeel);
    }
	
    jInstance::~jInstance()
    {
        m_pages.clear();
    }
    
    shared_ptr<jInstance> jInstance::create()
    {
        shared_ptr<jInstance> that = make_shared<jInstance>();
        if(that)
        {
            that->m_instance->bind(that);
        }
        return that;
    }
    
    void jInstance::pageCreated(sInstance instance, sPage page)
    {
        if(instance == m_instance)
        {
            if(page)
            {
				sjPage jpc = PageView::create<jPage>(page);
                if(jpc)
                {
                    m_pages.push_back(jpc);
                    m_window->setContentNonOwned(jpc.get(), false);
					//m_window->setName(jpc->getN)
                    Console::post("Page has been added to the application");
                }
                else
                {
                    m_instance->removePage(page);
                }
            }
        }
    }
    
    void jInstance::pageRemoved(sInstance instance, sPage page)
    {
        Console::post("Page has been removed from the application");
    }
    
    void jInstance::dspStarted(shared_ptr<Instance> instance)
    {
        ;
    }
    
    void jInstance::dspStopped(shared_ptr<Instance> instance)
    {
        ;
    }
    
    void jInstance::newPage()
    {
        m_instance->createPage(Dico::create());
    }
    
    void jInstance::openPage(const File& file)
    {
        ;
    }
	
	void jInstance::showAppSettingsWindow()
	{
		if (m_app_settings_window)
		{
			m_app_settings_window->setVisible(true);
			m_app_settings_window->toFront(true);
		}
		else
		{
			m_app_settings_window = InspectorWindow::create();
			showAppSettingsWindow();
		}
	}
	
	void jInstance::showInspector(sPage page)
	{
		if (m_page_inspector_window)
		{
			m_page_inspector_window->setPage(page);
			m_page_inspector_window->setVisible(true);
			m_page_inspector_window->toFront(true);
		}
		else
		{
			if (page)
			{
				m_page_inspector_window = InspectorWindow::create();
				showInspector(page);
			}
		}
	}
	
	void jInstance::showInspector(sBox box)
	{
		if (m_object_inspector_window)
		{
			m_object_inspector_window->setBox(box);
			m_object_inspector_window->setVisible(true);
			m_object_inspector_window->toFront(true);
		}
		else
		{
			m_object_inspector_window = InspectorWindow::create();
			showInspector(box);
		}
	}
	
	void jInstance::setInspectorContent(sBox box)
	{
		if (m_object_inspector_window)
		{
			m_object_inspector_window->setBox(box);
		}
	}
	
	void jInstance::savePage(sPage page)
	{
		if (page)
		{
			sDico dico = Dico::create();
			page->write(dico);
			const string extension = ".kiwipage";
			const string filename = "testeuuuur";
			const string directory = File::getSpecialLocation(File::userDesktopDirectory).getFullPathName().toStdString();
			
			dico->write(filename + extension, directory);
		}
	}
	
	void jInstance::askUserToOpenFile()
	{
		FileChooser fc("Open a File");
		
		if (fc.browseForFileToOpen())
			openFile(fc.getResult());
	}
	
	bool jInstance::openFile(File file)
	{
		if (file.exists())
		{
			const string extension = file.getFileExtension().toStdString();
			const string filename = file.getFileNameWithoutExtension().toStdString();
			const string directory = file.getParentDirectory().getFullPathName().toStdString();
			
			DBG("extension : " + extension);
			DBG("filename : " + filename);
			DBG("directory : " + directory);
			
			if (extension == ".kiwipage" && !filename.empty() && !directory.empty())
			{
				sDico dico = Dico::create();
                if(dico)
                {
                    dico->read(filename + extension, directory);
                    sPage page = m_instance->createPage(dico);
                    if(page)
                    {
                        sPageView ctrl = dynamic_pointer_cast<PageView>(page->getController());
                        if(ctrl)
                        {
                            ctrl->setLockStatus(true);
                            DBG("page loaded");
                            return true;
                        }
                    }
                }
			}
			else
			{
				Console::error("file failed to load : " + file.getFullPathName().toStdString());
			}
		}
		
		return false;
	}
}



