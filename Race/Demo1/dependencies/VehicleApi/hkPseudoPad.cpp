/*
 *
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Product and Trade Secret source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2014 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 *
 */

#include <Demos/demos.h>
#include <Demos/DemoCommon/DemoFramework/hkPseudoPad.h>

int hkPseudoPad::s_keyToPadMapping[][2] = 
{
	{ HKG_VKEY_SPACE, HKG_PAD_BUTTON_0 },
	{ '1', HKG_PAD_BUTTON_1 },
	{ '2', HKG_PAD_BUTTON_2 },
	{ '3', HKG_PAD_BUTTON_3 },
	{ HKG_VKEY_UP, HKG_PAD_DPAD_UP },
	{ HKG_VKEY_DOWN, HKG_PAD_DPAD_DOWN },
	{ HKG_VKEY_LEFT, HKG_PAD_DPAD_LEFT },
	{ HKG_VKEY_RIGHT, HKG_PAD_DPAD_RIGHT },
	{ HKG_VKEY_SHIFT, HKG_PAD_SELECT },
	{ HKG_VKEY_RETURN, HKG_PAD_START },
	{ HKG_VKEY_DELETE, HKG_PAD_BUTTON_L1 },// also map to left mouse button 
	{ HKG_VKEY_END, HKG_PAD_BUTTON_R1 },// also map to right mouse button
	{ HKG_VKEY_INSERT, HKG_PAD_BUTTON_L2 },
	{ HKG_VKEY_HOME, HKG_PAD_BUTTON_R2 }
};

HKG_PAD_BUTTON HK_CALL hkPseudoPad::keyToPad( HKG_KEYBOARD_VKEY k )
{
	int numMappings = (int) HK_COUNT_OF(s_keyToPadMapping);
	for (int i=0; i<numMappings; i++)
	{
		int from = s_keyToPadMapping[i][0];
		int to   = s_keyToPadMapping[i][1];

		if (from == (int)k)
		{
			return (HKG_PAD_BUTTON)(to);
		}
	}

	return 0;
}

void hkPseudoPad::updatePad( int padIndex, hkgWindow* window, hkReal deltaTime, hkBool fakePads, hkBool desiredCtrlState )
{
	const hkgKeyboard& keyboard = window->getKeyboard();
	const hkgPad& pad = window->getGamePad(padIndex);
	m_connected = pad.isConnected();

	// do we use the pads as is, or fake one
	if (m_connected && window->hasGamePads() && !fakePads)
	{
		// copy button state
		m_curButtonState = pad.getButtonState();
		m_prevButtonState = pad.getPrevButtonState();

		m_stick[0] = pad.getStickState(0);
		m_stick[1] = pad.getStickState(1);

		m_trigger[0].m_prev = m_trigger[0].m_cur;
		m_trigger[0].m_cur = pad.getTriggerPos( 0 );
		m_trigger[1].m_prev = m_trigger[1].m_cur;
		m_trigger[1].m_cur = pad.getTriggerPos( 1 );

		m_hasIndependentAnalogTriggers = pad.hasIndependentAnalogTriggers();

	}
	else // fake one
	{
		//
		// map key presses to button pad presses
		//

		bool ctrlState = keyboard.getKeyState(HKG_VKEY_CONTROL);

		if (desiredCtrlState == ctrlState)
		{
			m_curButtonState = 0;
			m_prevButtonState = 0;

			const int nmaps = sizeof(s_keyToPadMapping) / (sizeof(int)*2);
			for(int i = 0; i < nmaps; ++i)
			{
				int from = s_keyToPadMapping[i][0];
				int to   = s_keyToPadMapping[i][1];

				if (keyboard.getKeyState( (HKG_KEYBOARD_VKEY)(from) ))
				{
					m_curButtonState |= to;
				}

				if (keyboard.getPrevKeyState( (HKG_KEYBOARD_VKEY)(from) ))
				{
					m_prevButtonState |= to;
				}
			}
		}
		else
		{
			// if the Control key state does not correspond to this gamepad, we still need to detect key releases
			m_prevButtonState = m_curButtonState;

			const int nmaps = sizeof(s_keyToPadMapping) / (sizeof(int)*2);
			for(int i = 0; i < nmaps; ++i)
			{
				int from = s_keyToPadMapping[i][0];
				int to   = s_keyToPadMapping[i][1];

				if (!keyboard.getKeyState( (HKG_KEYBOARD_VKEY)(from) ))
				{
					m_curButtonState &= ~to;
				}
			}
		}
		
		//
		// map analog sticks 
		//
		{
			const float keyResponse = 2.0f * float(deltaTime);
			const float maxVal = 1.0f;
			const float minVal = -1.0f;

			// Stick1 == main stick == dpad on PC (arrow keys)
			int directions[2][4]; // [stick][4 directions]
			directions[1][0] = m_curButtonState & HKG_PAD_DPAD_RIGHT;
			directions[1][1] = m_curButtonState & HKG_PAD_DPAD_LEFT;
			directions[1][2] = m_curButtonState & HKG_PAD_DPAD_UP;
			directions[1][3] = m_curButtonState & HKG_PAD_DPAD_DOWN;

			// Stick0 is the extra stick really, as it is the right stick by 
			// our convention, and doesn't exist on all platforms,
			// PSP(R) (PlayStation(R)Portable) for instance.
			directions[0][0] = keyboard.getKeyState('L');
			directions[0][1] = keyboard.getKeyState('J');
			directions[0][2] = keyboard.getKeyState('I');
			directions[0][3] = keyboard.getKeyState('K');

			for( int s = 0; s < 2; ++s ) // each stick
			{
				Stick& stick = m_stick[s];

				for( int i = 0; i < 2; ++i ) // two axes
				{
					stick.m_prev[i] = stick.m_cur[i];	// copy the old values

					if( directions[s][i*2] )
					{
						stick.m_cur[i] += keyResponse;
						 
						// check boundaries
						if (stick.m_cur[i] > maxVal)
						{
							stick.m_cur[i] = maxVal;
						}
					}
					else if ( directions[s][i*2+1] )
					{
						stick.m_cur[i] -= keyResponse;
						 
						// check boundaries
						if (stick.m_cur[i] < minVal)
						{
							stick.m_cur[i] = minVal;
						}
					}
					else 
					{
						// center
						stick.m_cur[i] = 0.0f;
					
					}
				}
			}
		}

		m_hasIndependentAnalogTriggers = true;
	}
}

/*
 * Havok SDK - NO SOURCE PC DOWNLOAD, BUILD(#20140907)
 * 
 * Confidential Information of Havok.  (C) Copyright 1999-2014
 * Telekinesys Research Limited t/a Havok. All Rights Reserved. The Havok
 * Logo, and the Havok buzzsaw logo are trademarks of Havok.  Title, ownership
 * rights, and intellectual property rights in the Havok software remain in
 * Havok and/or its suppliers.
 * 
 * Use of this software for evaluation purposes is subject to and indicates
 * acceptance of the End User licence Agreement for this product. A copy of
 * the license is included with this software and is also available at www.havok.com/tryhavok.
 * 
 */
