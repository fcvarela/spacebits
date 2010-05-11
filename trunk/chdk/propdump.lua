--[[ 
@title propdump
@param f From propcase
@default f 0 
@param t To propcase
@default t 279
@param d Do dump?
@default d 1
@param h As hex?
@default h 1
@param i Compare with dump#
@default i 0
@param o Output to dump#
@default o 0
]] 


-- setup
_from = f
_to = t
_dodump = d
_ashex = h
if i >= 0 then
	_compare = true
end
if i == 0 then
	i = "log"
end
_infilename = "A/propdump." .. i
if o == 0 then
	o = "log"
end
_outfilename = "A/propdump." .. o


function logwrite(...)
	if _dodump and not outfile then
		outfile=io.open(_outfilename, "w+b")
	end
	if outfile then
		outfile:write(...)
	end
end


function waitshutter()
	print("press half-shutter")
	repeat
		wait_click(100)
	until not is_pressed("shoot_half")
	repeat
		wait_click(100)
	until is_pressed("shoot_half")
end


-- load previous data
if _compare then
	infile = io.open(_infilename, "rb")
	if infile then
		fn = loadstring("data=" .. infile:read("*a"))
		if fn then
			fn()
		end
		infile:close()
	end
	if not data then
		print("no existing data")
	end
end

-- loop through propcases
diffs = {}
logwrite("{\n")
for i = _from,_to do
	v = get_prop(i)
	logwrite("[", i, "]=\t", v, ",\n")
	if data and data.ver == 1 and data.from <= i and i <= data.to then
		-- do comparison
		if data[i] ~= v then
			if _ashex then
				num1 = string.sub(string.format('%04X', data[i]), -4)
				num2 = string.sub(string.format('%04X', v), -4)
			else
				num1 = data[i]
				num2 = v
			end
			table.insert(diffs, "[" .. i .. "] " .. num1 .. " -> " .. num2)
		end
	end
end
logwrite("ver=1,\n")
logwrite("from=", f, ",\n")
logwrite("to=", t, ",\n")
diffs_were = unpack(diffs)
if _compare and diffs_were then 
	logwrite("diffs_were=", string.format("%q", diffs_were), ",\n")
end
logwrite("}\n")
if outfile then
	outfile:close()
end

-- tell user the differences
if _compare then
	for i,diff in ipairs(diffs) do
		print(diff)
		if i % 4 == 0 and diffs[i+1] then
			waitshutter()
		end
	end
	if get_mode() == 1 then
		-- in play mode, screen cleared without this
		waitshutter()
	end
end
