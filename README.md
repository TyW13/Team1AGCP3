# En-Gauge

Created using C++ and DirectX12 graphics library

En-gauge is a fast paced 2D platformer prototype developed by students of different disciplines for their university 3rd year group project. This game currently consists of multiple levels in which the player will have to utilise the in-built physics system and items to generate momentum to navigate their way through the environment to reach the goal at the end. Using their magically imbued shotgun to propel themselves and various supportive items found in the map, the player must proceed through the hazardous environment to make it through the area and make it to the opposite end in order to move onto the next level.

# Build Instructions
1. Clone the Repository into an empty folder
```
git clone https://github.com/TyreseWelsh/Team1AGCP3.git
cd Team1AGCP3
```

2. Switch to the DX12-Release branch
```
git switch DX12-Release
```

3. Build project
```
msbuild AGCP3.sln /p:Configuration=Release /p:Platform=x64
```

4. Navigate to the newly created executable folder
```
cd AGCP3Game/bin/x64/Release
```

5. Run the executable
```
AGCP3.exe
````