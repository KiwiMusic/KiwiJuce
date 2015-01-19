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

#ifndef __DEF_KIWI_NUMBERBOXCOMPONENT__
#define __DEF_KIWI_NUMBERBOXCOMPONENT__

#include "../JuceLibraryCode/JuceHeader.h"

namespace Kiwi
{
	class NumberBoxComponent : public Label, public ChangeBroadcaster
	{
	private:
		double m_current_value;
		bool m_has_min, m_has_max;
		double m_minimum, m_maximum;
		double m_interval;
		unsigned int m_decimals;
		
		double m_last_dragged_value;
		juce::Point<int> m_mouse_last_dragged;
		
		bool m_editable;
		bool m_floating_point_style;
		
		double m_drag_increment;
		
	public:
		
		/** Creates a new number box component
		 @param name The name of the number box component
		 */
		NumberBoxComponent(const String& name = String::empty);
		
		/** The number box component destructor
		 */
		~NumberBoxComponent();
		
		//==============================================================================
		
		double snapValue(double attemptedValue, const bool) const;
		
		/** Sets the number box value
		 @param value The new number box value
		 @param notification The notification type
		 */
		void setValue(double value, NotificationType notification = sendNotificationAsync);
		
		/** Retrieves the number box value
		 @return The number box value
		 */
		double getValue() const noexcept
		{
			return m_current_value;
		}
		
		/** Retrieves the number box value as a long
		 @return The number box value as a long
		 */
		long getLongValue() const noexcept
		{
			return getValue();
		}
		
		/** Tells the number box to use the minimum and maximum values to clip the value internally.
		 @param useMin If true the number box value will be clipped to the minimum value.
		 @param useMax If true the number box value will be clipped to the maximum value.
		 @see setMinMax
		 */
		void useMinMax(const bool useMin, const bool useMax) noexcept;
		
		/** Tells the number box to use the minimum and maximum values to clip the value internally.
		 @param min The minimum value.
		 @param max The maximum value.
		 @see useMinMax
		 */
		void setMinMax(const double min, const double max, const double newInt);
		
		/** Retrieves if the number is currently displayed in a floating point style
		 @return True if the number is currently displayed in a floating point style, false if it represents a long number
		 */
		bool isInFloatingPointStyle() const
		{
			return m_floating_point_style;
		}
		
		/** Sets the displayed style of the number
		 @param isFloatingPointStyle If true, The number will be displayed in a floating point style, if false, it will represent a long number
		 */
		void setFloatingPointStyle(bool isFloatingPointStyle)
		{
			m_floating_point_style = isFloatingPointStyle;
			updateText();
		}
		
		//==============================================================================
		
		const String getTextFromValue(double v) const;
		
		double getValueFromText(const String& text) const;
		
		double proportionOfLengthToValue(double proportion) const;
		
		double valueToProportionOfLength(double value) const;
		
		//==============================================================================
		
		void updateText();
		
		void textChange(const bool retPressed, const bool escPressed);
		
		void textEditorTextChanged(TextEditor& editor);
		
		void textEditorReturnKeyPressed(TextEditor& editor);
		
		void textEditorEscapeKeyPressed(TextEditor& editor);
		
		void textEditorFocusLost(TextEditor& editor);
		
		//==============================================================================
		/** @internal */
		void mouseUp(const MouseEvent& e) override;
		/** @internal */
		void mouseDown (const MouseEvent& e) override;
		/** @internal */
		void mouseDrag (const MouseEvent& e) override;
		/** @internal */
		void mouseDoubleClick(const MouseEvent& e) override;
		
	protected:
		
		/** Called when the number starts dragging
		 */
		virtual void startDragging() {};
		
		/** Called when the number finish dragging
		 */
		virtual void endDragging() {};
	};
}

#endif