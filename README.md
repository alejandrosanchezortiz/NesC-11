# NesC-11
I try to implement a Nintendo NES emulator using C++11 or higher and SDL library.
The purpose of this project is not to implement the perfect emulator as there are many available.
I'll try to use the most advance C++ techniques when possible and learn at the same time :)

1. Start from CPU first. And make sure it’s really solid.
Duh. You can’t do anything without CPU. Just go implement the instructions and the main loop - stick to the official instructions first. There are very few games use unofficial/undocumented instructions.

2. Add NES rom support (and mapper 0)
Why add ROM support before you can actually render anything to screen? You want to use test roms to make sure your CPU simulation is really good and squash bugs as early as possible. This will save you a ton of time.

Trust me - you don’t want to find your CPU bugs in real games. And you’ll als catch your own regressions. Many ROMs would automatically write a success/fail code at a well known location - this can be your check/asserts, depending on what test framework you use.

And eventually you’d want to load some games, right? :)

3. Then go implement your PPU.
This is probably going to be fairly involved if not challenging. Even for experienced programmers, the PPU rendering pipeline takes quite a bit of time to wrap one’s head around. Due to memory constraints, the rendering tiles/sprites is completely 8x8 tile (block) based, and the way those tiles are represented in memory takes a bit getting used too (bitplanes, etc), and implementing the precise rendering pipeline has a lot of details.

Don’t worry about rendering to screen yet. Just observe your VRAM and see with your inner eye to imagine.

Don’t add scroll. That should be the next step.

Now you can add the rendering and the main game loop. Make sure your game loop process as the exact cycle as the real hardware by doing some math over elapsed tick count and CPU MHZ.

4. Go try some simple games Permalink
Try some games that don’t scroll and use mapper 0. Donkey Kong/Popeye/balloon fight are solid choices. You want to focus on those simple ones before you attempt your favorite games.

Useful Information:

https://medium.com/@fogleman/i-made-an-nes-emulator-here-s-what-i-learned-about-the-original-nintendo-2e078c9b28fe

https://bisqwit.iki.fi/jutut/kuvat/programming_examples/nesemu1/nesemu1.cc

https://www.youtube.com/watch?v=y71lli8MS8s

https://en.cppreference.com/w/cpp/types/integer

http://stannum.co.il/blog/5/sized-integer-types-and-signedness

https://stackoverflow.com/questions/20109780/int-ptr-in-64-bit-conversion

https://github.com/neveraway/nes-emulator/blob/master/src-vs2012/emulator/emulator/nes/cpu.cpp

http://nesdev.com/NESDoc.pdf


