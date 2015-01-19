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

#include "jDoodle.h"

namespace Kiwi
{
    JDoodle::JDoodle(Graphics &_g, juce::Rectangle<int> b) :
    g(_g), bounds(b.getX(), b.getY(), b.getWidth(), b.getHeight())
    {
        ;
    }
    
    JDoodle::~JDoodle()
    {
        
    }
    
    void JDoodle::setColor(Gui::Color const& color)
    {
        g.setColour(Colour::fromFloatRGBA(color.red(), color.green(), color.blue(), color.alpha()));
    }
    
    void JDoodle::setFont(Gui::Font const& font)
    {
        juce::Font jfont(font.name, (float)font.size, font.face);
        g.setFont(jfont);
    }
    
    void JDoodle::fillAll()
    {
        g.fillAll();
    }
    
    void JDoodle::drawText(string const& text, double x, double y, double w, double h, Gui::Font::Justification j, bool wrap)
    {
        g.drawText(text, x, y, w, h, juce::Justification(j), wrap);
    }
    
    void JDoodle::drawText(string const& text, Gui::Rectangle const& rect, Gui::Font::Justification j, bool wrap )
    {
        g.drawText(text, rect.x(), rect.y(), rect.width(), rect.height(), juce::Justification(j), wrap);
    }
    
    void JDoodle::fillPath(Gui::Path const& path)
    {
        if(!path.empty())
        {
            juce::Path jpath;
            Gui::Path::Node node = path.getNode(0);
            jpath.startNewSubPath(node.point.x(), node.point.y());
            for(unsigned long i = 1; i < path.size(); i++)
            {
                node = path.getNode(i);
                switch(node.mode)
                {
                    case Gui::Path::Move:
                        jpath.startNewSubPath(node.point.x(), node.point.y());
                        break;
                    case Gui::Path::Path::Linear:
                        jpath.lineTo(node.point.x(), node.point.y());
                        break;
                    case Gui::Path::Path::Quadratic:
                    {
                        if(++i < path.size())
                        {
                            Gui::Path::Node next_node = path.getNode(i);
                            jpath.quadraticTo(node.point.x(), node.point.y(), next_node.point.x(), next_node.point.y());
                        }
                        
                    }
                        break;
                    case Gui::Path::Cubic:
                    {
                        i += 2;
                        if(i < path.size())
                        {
                            Gui::Path::Node next_node = path.getNode(i-1);
                            Gui::Path::Node next_node2 = path.getNode(i);
                            jpath.cubicTo(node.point.x(), node.point.y(), next_node.point.x(), next_node.point.y(), next_node2.point.x(), next_node2.point.y());
                        }
                    }
                        break;
                        
                    default:
                        break;
                }
            }
            g.fillPath(jpath);
        }
    }
    
    void JDoodle::drawPath(const Gui::Path& path, double const thickness)
    {
        if(!path.empty())
        {
            juce::Path jpath;
            Gui::Path::Node node = path.getNode(0);
            jpath.startNewSubPath(node.point.x(), node.point.y());
            for(unsigned long i = 1; i < path.size(); i++)
            {
                node = path.getNode(i);
                switch(node.mode)
                {
                    case Gui::Path::Move:
                        jpath.startNewSubPath(node.point.x(), node.point.y());
                        break;
                    case Gui::Path::Linear:
                        jpath.lineTo(node.point.x(), node.point.y());
                        break;
                    case Gui::Path::Quadratic:
                    {
                        if(++i < path.size())
                        {
                            Gui::Path::Node next_node = path.getNode(i);
                            jpath.quadraticTo(node.point.x(), node.point.y(), next_node.point.x(), next_node.point.y());
                        }
                        
                    }
                        break;
                    case Gui::Path::Cubic:
                    {
                        i += 2;
                        if(i < path.size())
                        {
                            Gui::Path::Node next_node = path.getNode(i-1);
                            Gui::Path::Node next_node2 = path.getNode(i);
                            jpath.cubicTo(node.point.x(), node.point.y(), next_node.point.x(), next_node.point.y(), next_node2.point.x(), next_node2.point.y());
                        }
                    }
                        break;
                        
                    default:
                        break;
                }
            }
            g.strokePath(jpath, juce::PathStrokeType(thickness));
        }
    }
}


