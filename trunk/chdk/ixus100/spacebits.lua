--[[
@title Spacebits flightplan (IXUS100)
@param b backlit 0=off
@default b 1
@param m movie duration
@default m 60
@param n number of photos
@default n 20
--]]

capmode=require("capmode")

function printf(...)
	print(string.format(...))
end

function log_printf(...)
        print(string.format(...))
	log:write(os.date(s,tm),": ",string.format(...),"\n")
end

function my_backlight()
  if b == 0 then set_backlight(0) end
end

function spacebits()

        log_printf("wait 10 secs")
	sleep_and_blink(10)
        log_printf("end wait 10 secs")
        log_printf("wait 10 secs")
	sleep_and_blink(10)
        log_printf("end wait 10 secs")

        -- takes two photos and one small video to warm up
	blink_led(3)
        take_photo(2)
	blink_led(3)
        take_movie(5)
	blink_led(7)
        -- starts main loop
        while 1 do
          take_movie(m)
          take_photo(n)
          -- sd card size sleep saver
          my_backlight()
          sleep(2000)
          my_backlight()
          sleep(2000)
          blink_led(100)
          end
	-- need this to see display if we switched back to play
	sleep(1000)
end

function take_photo(n)
        log_printf("switch photo")
        my_backlight()
	blink_led(1)
        capmode.set(capmode.name_to_mode["LANDSCAPE"])
        my_backlight()
        sleep(500)
        -- ISO 100
	set_prop(149,100)
	-- flash off
	set_prop(143,2)
	-- Picture Quality (0,1,2 = Superfine, Fine, Normal)
	set_prop(57,0)
	-- Focus Mode (0,1,3,4,5 = Normal, Macro, Infinity, Manual, Super Macro [SX10])
	set_prop(6,3)
	for i = 1,n do
           log_printf("shooting %3d",i)
           press("shoot_half")
           sleep(500)
           press("shoot_full")
           sleep(1000)
           my_backlight()
           sleep(4000)
           my_backlight()
           release("shoot_full")
	end
end

function take_movie(secs)
        log_printf("switch video")
        my_backlight()
	blink_led(1)
	capmode.set(capmode.name_to_mode["VIDEO_STD"])
	blink_led(1)
        set_led (1,1)
        log_printf("set_record true")
	set_record(true)
        my_backlight()
        sleep(1000)
        my_backlight()
        sleep(4000)

	press("shoot_half")
 	press("shoot_full")

        my_backlight()
        log_printf("wait secs" .. secs)
        sleep_and_blink(secs)
-- releasing the video is tricky. sd card may be slow, we must check the status

        release("shoot_full")
 	press("shoot_full")
        sleep(1000)
        release("shoot_full")

        log_printf("set_record false")
	set_record(false)

        log_printf("waiting for cam")
        rec,vid,mode=get_mode()
        i=0
        my_backlight()
        while vid==true and i<10 do
          rec,vid,mode=get_mode()
          log_printf("busy, wait %3d", i)
          sleep(1000)
          i=i+1
          end
	set_record(true)
        my_backlight()
        set_led (1,0)
        sleep(3000)
end

function blink_led(n)
	for j = 1,n do
	  for i = 1,5 do
		 set_led (1,1)
		 sleep (10)
		 set_led (1,0)
		 set_led (2,1)
		 sleep (10)
		 set_led (2,0)
	  end
	end
end

function sleep_and_blink(s)
   for j = 1,s do
	for i = 1,9 do
	  sleep (10)
	  set_led (1,1)
	  sleep (10)
	  set_led (1,0)
	  sleep (10)
	  set_led (2,1)
	  sleep (10)
	  set_led (2,0)
	end
   end
end

-- main program
set_prop(105,2)
log=io.open("A/spacebits.log","ab")
log_printf("Flight started")
-- init the loop
spacebits()
log_printf("Flight ended")
log:close()
