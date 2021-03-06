
#include "utils.h"
#include "hid.h"
#include "i2c.h"

u32 InputWait() {
    static u64 delay = 0;
    u32 pad_state_old = HID_STATE;
    delay = (delay) ? 72 : 128;
    timer_start();
	
	while (true) {
		
		
		
		u32 pad_state = HID_STATE;
		u8 Special_HID = i2cReadRegister(I2C_DEV_MCU, 0x10);
		
		if (Special_HID & BIT_0){return ~pad_state + 0x00002000;}//button power 0x01
		if (Special_HID & BIT_2){return ~pad_state + 0x00003000;}//button home 0x04
		
		if (!(~pad_state & BUTTON_ANY)) { // no buttons pressed
            pad_state_old = pad_state;
            delay = 0;
			continue;
		}
		
		if ((pad_state == pad_state_old) && 
		(!(~pad_state & BUTTON_ARROW) || 
		(delay && (timer_msec() < delay)))) 
		continue;
		
		//Make sure the key is pressed
        u32 t_pressed = 0;
        for( ; (t_pressed < 0x13000) && (pad_state == HID_STATE); t_pressed++);
		
		if(t_pressed >= 0x13000)return ~pad_state + 0x00001000;
	}
}