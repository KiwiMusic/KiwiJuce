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
    jInstance::jInstance(sGuiDeviceManager guiDevice, sDspDeviceManager dspDevice, string const& name) :
    m_instance(Instance::create(guiDevice, dspDevice, name))
    {
		LookAndFeel::setDefaultLookAndFeel(&m_lookandfeel);
    }
	
    jInstance::~jInstance()
    {
        ;
    }
    
    shared_ptr<jInstance> jInstance::create(sGuiDeviceManager guiDevice, sDspDeviceManager dspDevice, string const& name)
    {
        shared_ptr<jInstance> that = make_shared<jInstance>(guiDevice, dspDevice, name);
        if(that)
        {
            that->m_instance->addListener(that);
            that->m_instance->createPatcher();
            that->m_instance->createWindow();
        }
        return that;
    }
    
    void jInstance::patcherCreated(sInstance instance, sPatcher patcher)
    {
        if(patcher && instance == m_instance)
        {
            cout << "Patcher created" << endl;
            /*
            sGuiWindow window = patcher->createWindow();
            if(window)
            {
                cout << "Window created" << endl;
            }
            
            window = patcher->createWindow();
            if(window)
            {
                cout << "Window created" << endl;
            }
             */
        }
    }
	
    void jInstance::patcherRemoved(sInstance instance, sPatcher patcher)
    {
        Console::post("Patcher removed.");
    }
    
    void jInstance::dspStarted(shared_ptr<Instance> instance)
    {
        ;
    }
    
    void jInstance::dspStopped(shared_ptr<Instance> instance)
    {
        ;
    }
    
    void jInstance::newPatcher()
    {
        m_instance->createPatcher();
    }
    
    void jInstance::openPatcher(const File& file)
    {
        ;
    }
	
	void jInstance::showAppSettingsWindow()
	{
		/*
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
		*/
	}
	
	void jInstance::showInspector(sPatcher patcher)
	{
		/*
		if (m_patcher_inspector_window)
		{
			m_patcher_inspector_window->setPatcher(patcher);
			m_patcher_inspector_window->setVisible(true);
			m_patcher_inspector_window->toFront(true);
		}
		else
		{
			if (patcher)
			{
				m_patcher_inspector_window = InspectorWindow::create();
				showInspector(patcher);
			}
		}
		*/
	}
	
	void jInstance::showInspector(sObject object)
	{
		/*
		if (m_object_inspector_window)
		{
			m_object_inspector_window->setObject(object);
			m_object_inspector_window->setVisible(true);
			m_object_inspector_window->toFront(true);
		}
		else
		{
			m_object_inspector_window = InspectorWindow::create();
			showInspector(object);
		}
		*/
	}
	
	void jInstance::setInspectorContent(sObject object)
	{
		/*
		if (m_object_inspector_window)
		{
			m_object_inspector_window->setObject(object);
		}
		*/
	}
	
	void jInstance::savePatcher(sPatcher patcher)
	{
        /*
		if (patcher)
		{
			sDico dico = Dico::create();
			patcher->write(dico);
			const string extension = ".kiwipatcher";
			const string filename = "testeuuuur";
			const string directory = File::getSpecialLocation(File::userDesktopDirectory).getFullPathName().toStdString();
			
			dico->write(filename + extension, directory);
		}
        */
	}
	
	void jInstance::askUserToOpenFile()
	{
		FileChooser fc("Open a File");
		
		if (fc.browseForFileToOpen())
			openFile(fc.getResult());
	}
	
	bool jInstance::openFile(File file)
	{
        /*
		if (file.exists())
		{
			const string extension = file.getFileExtension().toStdString();
			const string filename = file.getFileNameWithoutExtension().toStdString();
			const string directory = file.getParentDirectory().getFullPathName().toStdString();
			
			DBG("extension : " + extension);
			DBG("filename : " + filename);
			DBG("directory : " + directory);
			
			if(extension == ".kiwipatcher" && !filename.empty() && !directory.empty())
			{
				sDico dico = Dico::create();
                if(dico)
                {
                    dico->read(filename + extension, directory);
                    sPatcher patcher = m_instance->createPatcher(dico);
                }
			}
			else
			{
				Console::error("file failed to load : " + file.getFullPathName().toStdString());
			}
		}
		*/
        
		return false;
	}
}



