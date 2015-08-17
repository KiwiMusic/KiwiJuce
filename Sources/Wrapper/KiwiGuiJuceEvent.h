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

#ifdef __KIWI_JUCE_WRAPPER__

#ifndef __DEF_KIWI_GUI_JUCE_EVENT__
#define __DEF_KIWI_GUI_JUCE_EVENT__

#include <JuceHeader.h>
#include "../KiwiModules/KiwiModules.h"
//#include "../Application/KiwiApplication.h"

namespace Kiwi
{
    class KiwiJuceGuiDeviceManager;
    typedef shared_ptr<KiwiJuceGuiDeviceManager>        sjGuiDeviceManager;
    typedef shared_ptr<const KiwiJuceGuiDeviceManager>  scjGuiDeviceManager;
    typedef weak_ptr<KiwiJuceGuiDeviceManager>          wjGuiDeviceManager;
    typedef weak_ptr<const KiwiJuceGuiDeviceManager>    wcjGuiDeviceManager;
    
    template<typename type> static inline juce::Point<type> toJuce(Kiwi::Point const& pt) noexcept {
        return juce::Point<type>((type)pt.x(), (type)pt.y());
    }
    
    template<typename type> static inline Kiwi::Point toKiwi(juce::Point<type> const& rect) noexcept {
        return Kiwi::Point(rect.getX(), rect.getY());
    }
    
    template<typename type> static inline juce::Rectangle<type> toJuce(Kiwi::Rectangle const& rect) noexcept {
        return juce::Rectangle<type>((type)rect.x(), (type)rect.y(), (type)rect.width(), (type)rect.height());
    }
    
    template<typename type> static inline Kiwi::Rectangle toKiwi(juce::Rectangle<type> const& rect) noexcept {
        return Kiwi::Rectangle(rect.getX(), rect.getY(), rect.getWidth(), rect.getHeight());
    }
    
    static inline juce::Colour toJuce(Kiwi::Color const& color) noexcept {
        return Colour::fromFloatRGBA(color.red(), color.green(), color.blue(), color.alpha());
    }
    
    static inline juce::Font toJuce(Kiwi::Font const& font) noexcept {
        return juce::Font(font.getName(), (float)font.getHeight(), font.getStyle());
    }
    
    static inline Kiwi::Color toKiwi(juce::Colour const& color) noexcept {
        return Kiwi::Color(color.getFloatRed(), color.getFloatGreen(), color.getFloatBlue(), color.getFloatAlpha());
    }
    
    class jSketch : public Sketch
    {
    private:
        Graphics &g;
        juce::Path createJucePath(Kiwi::Path const& path) const noexcept;
        
    public:
        inline jSketch(Graphics& graphics) noexcept : Sketch(toKiwi(graphics.getClipBounds())), g(graphics) {}
        
        inline ~jSketch() noexcept {}
        
        void internalFillPath(Path const& path, Color const& color) const noexcept override;
        
        void internalDrawPath(Path const& path, double const thickness,
                              const Path::Joint joint, const Path::LineCap linecap,
                              Color const& color) const noexcept override;
        
        void internalDrawText(string const& text, double x, double y, double w, double h, Font const& font,
                              Font::Justification j, bool truncated = false) const noexcept override;
        
        void internalDrawText(wstring const& text, double x, double y, double w, double h, Font const& font,
                                      Font::Justification j, bool truncated = false) const noexcept override;
        
        void internalDrawTextLine(string const& text, double x, double y, double w, double h, Font const& font,
                              Font::Justification j, bool ellipses = false) const noexcept override;
        
        void internalDrawTextLine(wstring const& text, double x, double y, double w, double h, Font const& font,
                              Font::Justification j, bool ellipses = false) const noexcept override;
    };
    
    class jEventMouse : public Kiwi::MouseEvent
    {
    private:
        const MouseInputSource m_source;
    public:
        
        jEventMouse(Type const& type, juce::MouseEvent const& event) noexcept;
        
        jEventMouse(juce::MouseEvent const& event, MouseWheelDetails const& wheel) noexcept;
        
        ~jEventMouse() noexcept;
        
        void setMouseUnlimited(bool isLimited, bool visibleUntilLimits = false) const override;
    };
}

#endif

#endif


