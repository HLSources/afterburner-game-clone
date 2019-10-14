This folder is used to hold bits of code that are used by multiple projects (game/engine/menus).
The ideal way to do this would be to have some kind of shared subrepo or library that's used be
each of the projects, but that's going to take a lot of time and may require converting the game
from CMake to Waf, which I'm not up for doing just yet.

Point the projects to these folders using relative paths and you should be able to use the code
within.
