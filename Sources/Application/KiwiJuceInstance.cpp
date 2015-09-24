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

#include "KiwiJuceInstance.h"

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
        }
        return that;
    }
    
    void jInstance::patcherCreated(sInstance instance, sPatcher patcher)
    {
        if(patcher && instance == m_instance)
        {
            cout << "Patcher created" << endl;
            sjWindow window = createWindow();
            
            sjPatcherView pv = jPatcherView::create(patcher);
            
            //window->setContentNonOwned(<#juce::Component *newContentComponent#>, <#bool resizeToFitWhenContentChangesSize#>)
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
        ;
	}
	
	void jInstance::showInspector(sPatcher patcher)
	{
        ;
	}
	
	void jInstance::showInspector(sObject object)
	{
        ;
	}
	
	void jInstance::setInspectorContent(sObject object)
	{
        ;
	}
	
	void jInstance::savePatcher(sPatcher patcher)
	{
        ;
	}
	
	void jInstance::askUserToOpenFile()
	{
		FileChooser fc("Open a File");
		
		if (fc.browseForFileToOpen())
			openFile(fc.getResult());
	}
	
	bool jInstance::openFile(File file)
	{
		return false;
	}
}



