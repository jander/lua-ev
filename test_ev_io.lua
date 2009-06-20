print '1..??'

local tap   = require("tap")
local ev = require("ev")
local help  = require("test_help")
local ok    = tap.ok
local dump  = require("dumper").dump

local noleaks = help.collect_and_assert_no_watchers
local loop    = ev.Loop.default
--print(dump("loop fenv", debug.getfenv(loop)))


-- Simply see if we can do a simple one second timer:
function test_basic() 
   local io1 = ev.IO.new(
      function(loop, io, revents)
         ok(true, 'STDIN is writable')
         io:stop(loop)
      end, 1, ev.WRITE)
--   print(dump("io fenv", debug.getfenv(io1)))
   io1:start(loop)
--   print "start io worked"
   loop:loop()
end

-- TODO: Actually test all the other functionality.
noleaks(test_basic, "test_basic");