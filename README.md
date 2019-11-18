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

## Bugs which I don't know how I'd fix
Probably the hardest part of any reflection is talking about bugs, especially when it's for a portfolio piece. It's even harder to pick out the bugs you also have no idea why they happen or how to fix them. I'm going head on to these bug and I'm going to attempt to explain them as best as I can.

### Enemies fall through the map or get stuck
**What happens**: Enemies occasionally spawn and get stuck on ground below them.

**How I sort of fixed it**: Spawn enemies much higher than the ground and hope the character class stops them falling under the map.

What I think is happening here is the check which takes place thinks the enemy is in the ground and doesn't have a movement. So the character class is assuming they're in a block and the reaction is to send them down or not send them anywhere. This leads to enemies getting stuck or falling through the map.

I do actually know how I'd fix this, the tick between the character gravity check does not have to be done every frame. If I instead slowed down the check to something like 0.5 seconds it wouldn't be doing a very granular check and should instead remain on the surface. This would take a complete rehaul of that system and that's something I'm not sure how I'd do in order to fix this issue and not introduce other bugs.

