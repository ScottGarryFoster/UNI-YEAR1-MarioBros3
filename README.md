# Super Mario Bros. 3
This project is my university submission for a Super Mario game using SDL. This is **only** to be used for **educational** purposes and learning how to use SDL, C++ and other such tools to produce a video game. This was built in Visual Studios 2019 and worked at the time, if you'd like to see a preview see the below YouTube video. Additionally to see my portfolio see the link below.

YouTube video:

My portfolio: [https://ScottGarryFoster.com](https://ScottGarryFoster.com)

# Reflection on the Project
## Introduction
Written in SDL in C++ my goal was to recreate the first level of Super Mario Bros. 3 as part of my University course work.

The end result was the first level, with working enemies, split screen multi-player, music/sound and a fully working level editor.

I included external controller support for SNES and NES controllers, multi-player where one player can save the other if needed. Both players can pick up items, fire shells or even complete the level.

The level editor has all the blocks included, allows complete customization of the layout, standard tools such as pencil/eraser and more advanced tools which interact with the functionality of the game such as question mark blocks. Play your game instantly and save them to be included in the main game-play mode.

Completed in just over 3 months the project taught me a lot about programming in C++ and invaluable games programming skills.

## Overall Reflection
I really feel like the end results of this project speaks for itself. It is a close recreation of the first level and acts on first principles. Where I feel it falls short is planning ahead. When creating this project I scrapped everything I had twice and started again because it was becoming too messy and I understood better ways of implementing key code. So some of the classes are way bigger than they need to be, you can see in some classes I've not created new sub classes but instead broken up big methods into smaller ones. That is the key failure in this project because when it comes to debugging the compartmentalization matters. If a class is not encapsulated enough or in the correct manner it means you have to jump through hoops in order to link classes together or have them send information between each other. I'll get into the specifics below but take enemies, they need a link to the tilemap, player and other enemies. The issue I came into was not understanding a good way to link these concepts until quite far into the project.

### You can step 'into' a Koopa
**What happens**: The first Koopa you find, you can occasionally jump into it without it reacting to you

**How I sort of fixed it**: I didn't really fix it, it doesn't happen a lot and tried a lot.

This I believe goes into reactions and connections between the player and enemies. So if I were to create this in say Unreal Engine 4, the event would be on these two objects entering the same collision space, do x y z. This is sort of how the enemy class works however I attempted to ensure that only one action is activated at a single point and to keep checking every so often. It's almost like a what happens on a word processor. On a word processor you hold down say the S key and a single S will be typed but in theory that S trigger is being sent several times a second. After a few seconds though it'll then start sending S letters to the screen again. This type of behavior is the type of thing I was attempting with the trigger between these two classes. This works for the most part with the enemies however occasionally on the first Koopa only the "shell" trigger fires and nothing else does because you've not entered the space.

## Sections I'm proud of
There's a lot of good in this project, I'm really proud of the UI in the level editor. I'm really proud of the gamepad support, Mario's jump which I wrote an article describing as I researched it is really close to Super Mario Bros. 3.

### Mario's Jump
I put a lot of effort in to making Mario feel as he does in the original game. I wrote an article going in depth into this system [here](https://www.linkedin.com/pulse/marios-jump-its-complicated-scott-foster/). This system was torn down and started again several times over and I compared it to an emulator version of the game to get it really close. It really made me think about exactly how to implement game-play which feels good. If I were to do it again though I'd probably encapsulate the jump into it's own class as a motion type meaning I could apply it to more.

### Multi-player and Split screen
The split screen is not made here using an SDL feature. I found out later that there is a feature which would have made this far easier called view-ports I believe. The system as it stands instead uses maths, applies it to all sprites and tiles, as to whether the item is on a screen or not and then how much of it should be shown culling very exactly. An article describing more in-depth can be found [here](https://www.linkedin.com/pulse/culling-2d-people-hurry-scott-foster/). If I were to do this again I would do more research and see if using view-ports would have worked better.

### Tilemap system
So I actually made the file reader and tile-map system read a very condensed version of the map. So it takes a letter and a number, the letter is converted into a number A-1, B-2 and so on, and the number is then the tile. This means say for the highest row the level does not read 0 0 0 for all the top, instead it could read Z0 meaning 26 air blocks. The second system there is flags, if you add a forward slash this is what gives tiles behaviors such as animation and items.

I'm happy with the system however if I were to do it again I think I'd make several files to read in, so the item blocks would have their own file loaded after the fact as a complete entity. There already are these little file describing types of question mark blocks but I would extend it to all variants meaning the tile-map file would be easier to maintain.

### The level editor
The level editor speaks for itself, it has functionality for everything. You can rearrange the blocks for easier editing, you have a customizable top bar and a lot of tools for getting the blocks you need on the screen. You can also play the level instantly and it will not override your progress as it uses a temporary file. Additionally when saving it makes a temporary file and provided this is created successfully it just copies this to the main file. This reduces the chance that the build could override during the course of a save an corrupt the information.

If I were to do this again I'd make more classes, the UI classes became quite big. I'd encapsulate far more features and maybe even make the behaviors very separate from the actions they were attached to. This class as it stands could use some cleaning up and the method I'd use for this is encapsulation and separation of concerns.

### The Particle system
The particle system is probably my most proud moments of this project, only because by the time I created it I was doing so not for marks but because I wanted to. I really enjoyed making it and it works so well. When you create a particle you attach a behavior and then the tick of each particle is individual and it even triggers it's own death when no longer on the screen. I think the results are amazing and the code isn't too messy to understand.

If I were to make this again I'd forward declare more to link the concepts together. This was something I was still learning how to do.

## Bugs
Probably the hardest part of any reflection is talking about bugs, especially when it's for a portfolio piece. It's even harder to pick out the bugs you also have no idea why they happen or how to fix them. I'm going head on to these bug and I'm going to attempt to explain them as best as I can.

### Enemies fall through the map or get stuck
**What happens**: Enemies occasionally spawn and get stuck on ground below them.

**How I sort of fixed it**: Spawn enemies much higher than the ground and hope the character class stops them falling under the map.

What I think is happening here is the check which takes place thinks the enemy is in the ground and doesn't have a movement. So the character class is assuming they're in a block and the reaction is to send them down or not send them anywhere. This leads to enemies getting stuck or falling through the map.

I do actually know how I'd fix this, the tick between the character gravity check does not have to be done every frame. If I instead slowed down the check to something like 0.5 seconds it wouldn't be doing a very granular check and should instead remain on the surface. This would take a complete rehaul of that system and that's something I'm not sure how I'd do in order to fix this issue and not introduce other bugs.

## Overall Improvements
I learned so much from this project that my only improvements would be to use the techniques I played around with but properly.

I understood classes going into this but I don't think I understand the way to link the classes fully and know this now I could create a much better system for having things relate probably by creating an interface class for objects as a whole. This is more like Unreal Engine 4 but I'd have a system which was much more event based than tick based. This would lead to far less bugs, far less mess and overall the game would probably run smoother.