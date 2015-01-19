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

#include "NumberBoxComponent.h"
namespace Kiwi
{
	NumberBoxComponent::NumberBoxComponent(const String &name)
		:	Label( name, "0" ),
			m_current_value(0),
			m_has_min(false),
			m_has_max(false),
			m_minimum(0),
			m_maximum(127),
			m_interval(1),
			m_decimals(0),
			m_editable(true),
			m_floating_point_style(true)
	{
		setRepaintsOnMouseActivity(true);

		setEditable(m_editable);
	}
	
	NumberBoxComponent::~NumberBoxComponent()
	{
		;
	}

	//==============================================================================

	double NumberBoxComponent::snapValue(double attemptedValue, const bool) const
	{
	    return attemptedValue;
	}

    void NumberBoxComponent::setValue(double newValue, NotificationType notification)
	{
		if (newValue <= m_minimum || m_maximum <= m_minimum)
		{
			newValue = m_minimum;
		}
		else if (newValue >= m_maximum)
		{
			newValue = m_maximum;
		}
		else if (m_interval > 0)
		{
			newValue = m_minimum + m_interval * floor ((newValue - m_minimum) / m_interval + 0.5);
		}

		if (m_current_value != newValue)
		{
			hideEditor (true);

			m_current_value = newValue;
			updateText();
			repaint();
			
            if (notification != dontSendNotification)
            {
                if (notification == sendNotificationSync)
                    sendSynchronousChangeMessage();
                else
                    sendChangeMessage();
            }
		}
	}

	void NumberBoxComponent::setMinMax(const double newMin, const double newMax, const double newInt)
	{
		if (m_minimum != newMin || m_maximum != newMax || m_interval != newInt)
		{
			m_minimum = newMin;
			m_maximum = newMax;
			m_interval = newInt;

			// figure out the number of decimal points needed to display all values at this interval setting.
			m_decimals = 7;

			if (m_interval != 0)
			{
				int v = abs ((int) (m_interval * 10000000));

				while ((v % 10) == 0)
				{
					--m_decimals;
					v /= 10;
				}
			}

            setValue(m_current_value, dontSendNotification);
			updateText();
		}
	}

	//==============================================================================

	const String NumberBoxComponent::getTextFromValue(double v) const
	{
		if (m_decimals > 0 && m_floating_point_style)
			return String(v, m_decimals);
		else
			return String(roundDoubleToInt(v));
	}

	double NumberBoxComponent::getValueFromText(const String& text) const
	{
		return text.trimStart().initialSectionContainingOnly("0123456789.-").getDoubleValue();
	}

	double NumberBoxComponent::proportionOfLengthToValue(double proportion) const
	{
		return (m_minimum + (m_maximum - m_minimum) * proportion);
	}

	double NumberBoxComponent::valueToProportionOfLength(double value) const
	{
		return (double)((value - m_minimum) / (m_maximum - m_minimum));
	}

	//==============================================================================

	void NumberBoxComponent::updateText()
	{
		setText (getTextFromValue (m_current_value), NotificationType::sendNotification);
	}

	void NumberBoxComponent::textChange(const bool retPressed, const bool escPressed)
	{
		if (retPressed || ! hasKeyboardFocus (true))
		{
			startDragging();
			setValue (snapValue (getValueFromText (getText()), false), sendNotificationSync);
			endDragging();
		}
		
		if ((retPressed || escPressed) && hasKeyboardFocus (true))
		{
			grabKeyboardFocus();
			updateText();
		}
	}

	void NumberBoxComponent::textEditorTextChanged(TextEditor& editor)
	{
		textChange (false, false);
	}

	void NumberBoxComponent::textEditorReturnKeyPressed(TextEditor& editor)
	{
		hideEditor(false);
		textChange (true, false);

	}

	void NumberBoxComponent::textEditorEscapeKeyPressed(TextEditor& editor)
	{
		hideEditor(true);
		textChange (false, true);
	}

	void NumberBoxComponent::textEditorFocusLost(TextEditor& editor)
	{
		hideEditor(false);
		textChange (false, true);
	}
	
	//==============================================================================
	void NumberBoxComponent::mouseUp(const MouseEvent&)
	{
		if (isEnabled() && (m_maximum > m_minimum))
		{
			endDragging();
		}
	}

	void NumberBoxComponent::mouseDown(const MouseEvent& e)
	{
		if (isEnabled() && (m_maximum > m_minimum))
		{
			e.source.enableUnboundedMouseMovement(true, true);
			hideEditor (true);

			m_mouse_last_dragged.setXY(e.x, e.y);

			m_last_dragged_value = m_current_value;
			startDragging();
		}
	}

	void NumberBoxComponent::mouseDrag(const MouseEvent& e)
	{
		if (isEnabled()	&& (m_maximum > m_minimum))
		{
			int mouseDiff = e.y - m_mouse_last_dragged.y;

			const double maxSpeed = 100;
			double speed = jlimit (-maxSpeed, maxSpeed, (double) mouseDiff);

			if (speed != 0)
			{
				speed = jmax (0.0, speed * speed - 2.0) / 2;

				double delta = 0.1;

				//double delta = (0.5 * m_interval) + (speed * (m_maximum - m_minimum) / (maxSpeed * maxSpeed));

				if (mouseDiff < 0)
					delta = -delta;

				// set sign for vertical action (y++ moves DOWN the screen)
				delta = -delta;

				m_last_dragged_value += delta;
			}
			
			if (m_has_min)
				m_last_dragged_value = jmax(m_last_dragged_value, m_minimum);
			
			if (m_has_max)
				m_last_dragged_value = jmin(m_last_dragged_value, m_maximum);

            setValue (snapValue (m_last_dragged_value, true), sendNotificationAsync);
			m_mouse_last_dragged.setXY(e.x, e.y);
		}
	}
	
	void NumberBoxComponent::mouseDoubleClick(const MouseEvent& e)
	{
		if (isEnabled())
			showEditor();
	}
}