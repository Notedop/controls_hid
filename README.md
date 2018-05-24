Arduino project for HID gamecontroller used for RaBoy

Code is written for usage with arduino leonardo which has USB interface. After x seconds (30 a.t.m.) of silence the controller is set to SLEEP_MODE_PWR_DOWN. When user pushes button attached to pinout # 2 or 3, the controller will wake-up and should be recognized again by it's host computer.

TODO: Comment out Serial to save more power. 
TODO: Add variable for setting time_till_powerdown 
TODO: Remove unneeded bits of code/comments
