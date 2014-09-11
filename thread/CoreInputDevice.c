src/input/idirectfbinputbuffer.c
	typedef struct {
		 DirectLink       link;

		 CoreInputDevice *device;       /* pointer to input core device struct */
		 Reaction         reaction;

		 DFBInputDeviceDescription desc;
	} AttachedDevice;
	
	
src/input/idirectfbinputdevive.c
{
	typedef struct {
		 int                         ref;               /* reference counter */
		 CoreInputDevice            *device;            /* pointer to input core
														   device struct*/

		 int                         axis[DIAI_LAST+1]; /* position of all axes */
		 DFBInputDeviceKeyState      keystates[DIKI_NUMBER_OF_KEYS];
														/* state of all keys */
		 DFBInputDeviceModifierMask  modifiers;         /* bitmask reflecting the
														   state of the modifier
														   keys */
		 DFBInputDeviceLockState     locks;             /* bitmask reflecting the
								   state of the key locks */
		 DFBInputDeviceButtonMask    buttonmask;        /* bitmask reflecting the
														   state of the buttons */

		 DFBInputDeviceDescription   desc;              /* device description */

		 Reaction                    reaction;
	} IDirectFBInputDevice_data;
}


src/core/system.c
	dfb_system_input_filter( CoreInputDevice *device,
							 DFBInputEvent   *event )
						 
						 