--[[
@title Spacebits flightplan (IXUS100)
--]]

capmode=require("capmode")

function printf(...)
	print(string.format(...))
end

function log_printf(...)
        print(string.format(...))
	log:write(os.date(s,tm),": ",string.format(...),"\n")
end

function spacebits()
        while 1 do
          take_movie(60)
          take_photo(20)
          end
	-- need this to see display if we switched back to play
	sleep(1000)
end

function take_photo(n)
        log_printf("switch photo")
	set_backlight(0)
	blink_led()
        capmode.set(capmode.name_to_mode["LANDSCAPE"])
	set_backlight(0)
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
	   set_backlight(0)
           sleep(4000)
	   set_backlight(0)
           release("shoot_full")
	end
end

function take_movie(secs)
        log_printf("switch video")
	set_backlight(0)
	blink_led()
	capmode.set(capmode.name_to_mode["VIDEO_STD"])
        log_printf("set_record true")
	set_record(true)
	set_backlight(0)
        sleep(1000)
	set_backlight(0)
        sleep(4000)

	press("shoot_half")
 	press("shoot_full")

	set_backlight(0)
        log_printf("wait secs")
        sleep(secs*1000)

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
	set_backlight(0)
        while vid==true and i<10 do
          rec,vid,mode=get_mode()
          log_printf("busy, wait %3d", i)
          sleep(1000)
          i=i+1
          end
	set_record(true)
	set_backlight(0)
        sleep(3000)
end

function blink_led()
	for i = 1,5 do
		 set_led (1,1)
		 sleep (10)
		 set_led (1,0)
		 set_led (2,1)
		 sleep (10)
		 set_led (2,0)
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
