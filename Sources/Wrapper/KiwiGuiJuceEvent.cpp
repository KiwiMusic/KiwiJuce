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

#include "KiwiGuiJuceEvent.h"

namespace Kiwi
{
    void jSketch::internalDrawText(string const& text, double x, double y, double w, double h, Font const& font,
                                   Font::Justification j, bool truncated) const noexcept
    {
        g.setColour(toJuce(getColor()));
        const juce::Font jfont = toJuce(font);
        g.setFont(jfont);
        g.drawMultiLineText(String(text), x, jfont.getAscent(), w);
    }
    
    void jSketch::internalDrawText(wstring const& text, double x, double y, double w, double h, Font const& font,
                          Font::Justification j, bool truncated) const noexcept
    {
        g.setColour(toJuce(getColor()));
        const juce::Font jfont = toJuce(font);
        g.setFont(jfont);
        g.drawMultiLineText(String(text.c_str()), x, jfont.getAscent(), w);
    }
    
    void jSketch::internalDrawTextLine(string const& text, double x, double y, double w, double h, Font const& font,
                                   Font::Justification j, bool ellipses) const noexcept
    {
        g.setColour(toJuce(getColor()));
        const juce::Font jfont = toJuce(font);
        g.setFont(jfont);
        g.drawText(String(text), juce::Rectangle<float>(x, y, w, h), j, ellipses);
    }
    
    void jSketch::internalDrawTextLine(wstring const& text, double x, double y, double w, double h, Font const& font,
                                   Font::Justification j, bool ellipses) const noexcept
    {
        g.setColour(toJuce(getColor()));
        const juce::Font jfont = toJuce(font);
        g.setFont(jfont);
        g.drawText(String(text.c_str()), juce::Rectangle<float>(x, y, w, h), j, ellipses);
    }
    
    juce::Path jSketch::createJucePath(Kiwi::Path const& path) const noexcept
    {
        juce::Path jpath;
        if(!path.empty())
        {
            vector<Node> const& nodes = getNodes(path);
            
            jpath.startNewSubPath(toJuce<float>(nodes[0].point()));
            
            for(ulong i = 1; i < nodes.size(); i++)
            {
                const Node node = nodes[i];
                switch(node.mode())
                {
                    case Sketch::Mode::Close:   jpath.closeSubPath(); break;
                    case Sketch::Mode::Move:    jpath.startNewSubPath(node.point().x(), node.point().y()); break;
                    case Sketch::Mode::Linear:  jpath.lineTo(node.point().x(), node.point().y()); break;
                        
                    case Sketch::Mode::Quadratic:
                    {
                        if(++i < path.size())
                        {
                            jpath.quadraticTo(toJuce<float>(node.point()), toJuce<float>(nodes[i].point()));
                        }
                        break;
                    }
                    case Sketch::Mode::Cubic:
                    {
                        i += 2;
                        if(i < path.size())
                        {
                            jpath.cubicTo(toJuce<float>(node.point()), toJuce<float>(nodes[i-1].point()), toJuce<float>(nodes[i].point()));
                        }
                        break;
                    }
                        
                    default:
                        break;
                }
            }
        }
        return jpath;
    }
    
    void jSketch::internalFillPath(Path const& path, Color const& color) const noexcept
    {
        g.setColour(toJuce(color));
        g.fillPath(createJucePath(path));
    }
    
    void jSketch::internalDrawPath(Path const& path,
                                   double const thickness,
                                   const Path::Joint joint,
                                   const Path::LineCap linecap,
                                   Color const& color) const noexcept
    {
        g.setColour(toJuce(color));
        g.strokePath(createJucePath(path), juce::PathStrokeType(thickness,
                                                                static_cast<juce::PathStrokeType::JointStyle>(joint),
                                                                static_cast<juce::PathStrokeType::EndCapStyle>(linecap)));
    }
    
    
    jEventMouse::jEventMouse(Type const& type, juce::MouseEvent const& event) noexcept :
    Kiwi::MouseEvent(type, event.x, event.y, event.mods.getRawFlags(), 0., 0., event.mouseWasClicked(), event.getMouseDownPosition().x, event.getMouseDownPosition().y, event.getNumberOfClicks()),
    m_source(event.source)
    {
        ;
    }
    
    jEventMouse::jEventMouse(juce::MouseEvent const& event, MouseWheelDetails const& wheel) noexcept :
    Kiwi::MouseEvent(Kiwi::MouseEvent::Type::Wheel, event.x, event.y, event.mods.getRawFlags(), wheel.deltaX, wheel.deltaY, event.mouseWasClicked(), event.getMouseDownPosition().x, event.getMouseDownPosition().y, event.getNumberOfClicks()),
    m_source(event.source)
    {
        ;
    }
    
    jEventMouse::~jEventMouse() noexcept
    {
        ;
    }
    
    void jEventMouse::setMouseUnlimited(bool isLimited, bool visibleUntilLimits) const
    {
        m_source.enableUnboundedMouseMovement(isLimited, visibleUntilLimits);
    }
}

#endif

