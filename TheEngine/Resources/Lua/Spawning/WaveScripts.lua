--This table houses all the data for the various enemies in the scripts
ScriptTable = {}

--Width of the screen : 200 units

--[[
********************************************************************************************************
Enemy types are defined as follows:
Striker:	11
Assaulter:	12
Helios:		13
Crate:		34
********************************************************************************************************
--]]

--[[
********************************************************************************************************
Enemy spawn methods are defined as follows:
Default:	0 - Normal walk-in behavior
Jump In:	1 - Jump from the foreground to the normal plane
Drop Down:	2 - Drop down from the sky
Tree Drop:	3 - Drop down from a tree in the jungle
Boss:		4 - Spawn from the boss door and walk to the plane
Jetpack:	5 - Drops from the top of the screen and jetpacks in.  ASSAULTER ONLY.

Some additional notes:

When using methods 1 and 2 (jump in and drop down), Make sure the X and Y position are where you want
the enemies to be on screen.  They will jump to this point, or drop down to this point.  So you don't 
need to worry about setting their position say "at the camera".  Just set the enemy position to where
you want it to be on the actual plane to kill the player.  Everything else is handled.  

For the boss, just make sure the enemy position is that of the boss door.  The Z is handled elsewhere.

Jetpack enemies, ignore the Y value, I handle it in the arrival state.
********************************************************************************************************
--]]

--Boss crate script
ScriptTable.BossCrate = {}
ScriptTable.BossCrate.Number = 1
ScriptTable.BossCrate.Enemies = {}
ScriptTable.BossCrate.Enemies[1] = {}
ScriptTable.BossCrate.Enemies[1].Type = 34
ScriptTable.BossCrate.Enemies[1].XPos = 100.0
ScriptTable.BossCrate.Enemies[1].YPos = 200.0
ScriptTable.BossCrate.Enemies[1].Speed = false
ScriptTable.BossCrate.Enemies[1].SpawnMethod = 0

ScriptTable.BossScript = {}
ScriptTable.BossScript.Number = 1
ScriptTable.BossScript.Enemies = {}
ScriptTable.BossScript.Enemies[1] = {}
ScriptTable.BossScript.Enemies[1].Type = 11
ScriptTable.BossScript.Enemies[1].XPos = 120.0
ScriptTable.BossScript.Enemies[1].YPos = 8.0
ScriptTable.BossScript.Enemies[1].Speed = 40.0
ScriptTable.BossScript.Enemies[1].SpawnMethod = 4

--Tutorial wave
ScriptTable.Tutorial = {}
ScriptTable.Tutorial.Number = 3
ScriptTable.Tutorial.Enemies = {}
ScriptTable.Tutorial.Enemies[1] = {}
ScriptTable.Tutorial.Enemies[1].Type = 11
ScriptTable.Tutorial.Enemies[1].XPos = 200.0
ScriptTable.Tutorial.Enemies[1].YPos = 100.0
ScriptTable.Tutorial.Enemies[1].Speed = 40.0
ScriptTable.Tutorial.Enemies[1].SpawnMethod = 0
ScriptTable.Tutorial.Enemies[2] = {}
ScriptTable.Tutorial.Enemies[2].Type = 11
ScriptTable.Tutorial.Enemies[2].XPos = 230.0
ScriptTable.Tutorial.Enemies[2].YPos = 100.0
ScriptTable.Tutorial.Enemies[2].Speed = 40.0
ScriptTable.Tutorial.Enemies[2].SpawnMethod = 0
ScriptTable.Tutorial.Enemies[3] = {}
ScriptTable.Tutorial.Enemies[3].Type = 11
ScriptTable.Tutorial.Enemies[3].XPos = 260.0
ScriptTable.Tutorial.Enemies[3].YPos = 100.0
ScriptTable.Tutorial.Enemies[3].Speed = 40.0
ScriptTable.Tutorial.Enemies[3].SpawnMethod = 0

--Basic light wave
ScriptTable.BasicLightWave = {}
ScriptTable.BasicLightWave.Number = 5
ScriptTable.BasicLightWave.Enemies = {}
ScriptTable.BasicLightWave.Enemies[1] = {}
ScriptTable.BasicLightWave.Enemies[1].Type = 11
ScriptTable.BasicLightWave.Enemies[1].XPos = 100.0
ScriptTable.BasicLightWave.Enemies[1].YPos = 25.0
ScriptTable.BasicLightWave.Enemies[1].Speed = 50.0
ScriptTable.BasicLightWave.Enemies[1].SpawnMethod = 1
ScriptTable.BasicLightWave.Enemies[2] = {}
ScriptTable.BasicLightWave.Enemies[2].Type = 11
ScriptTable.BasicLightWave.Enemies[2].XPos = -200.0
ScriptTable.BasicLightWave.Enemies[2].YPos = 100.0
ScriptTable.BasicLightWave.Enemies[2].Speed = 50.0
ScriptTable.BasicLightWave.Enemies[2].SpawnMethod = 0
ScriptTable.BasicLightWave.Enemies[3] = {}
ScriptTable.BasicLightWave.Enemies[3].Type = 11
ScriptTable.BasicLightWave.Enemies[3].XPos = -220.0
ScriptTable.BasicLightWave.Enemies[3].YPos = 100.0
ScriptTable.BasicLightWave.Enemies[3].Speed = 50.0
ScriptTable.BasicLightWave.Enemies[3].SpawnMethod = 0
ScriptTable.BasicLightWave.Enemies[4] = {}
ScriptTable.BasicLightWave.Enemies[4].Type = 34
ScriptTable.BasicLightWave.Enemies[4].XPos = 100.0
ScriptTable.BasicLightWave.Enemies[4].YPos = 200.0
ScriptTable.BasicLightWave.Enemies[4].Speed = false
ScriptTable.BasicLightWave.Enemies[4].SpawnMethod = 0
ScriptTable.BasicLightWave.Enemies[5] = {}
ScriptTable.BasicLightWave.Enemies[5].Type = 13
ScriptTable.BasicLightWave.Enemies[5].XPos = 200.0
ScriptTable.BasicLightWave.Enemies[5].YPos = 0.0
ScriptTable.BasicLightWave.Enemies[5].Speed = 130.0
ScriptTable.BasicLightWave.Enemies[5].SpawnMethod = 0

--Basic medium wave
ScriptTable.BasicMediumWave = {}
ScriptTable.BasicMediumWave.Number = 8
ScriptTable.BasicMediumWave.Enemies = {}
ScriptTable.BasicMediumWave.Enemies[1] = {}
ScriptTable.BasicMediumWave.Enemies[1].Type = 11
ScriptTable.BasicMediumWave.Enemies[1].XPos = 200.0
ScriptTable.BasicMediumWave.Enemies[1].YPos = 100.0
ScriptTable.BasicMediumWave.Enemies[1].Speed = 50.0
ScriptTable.BasicMediumWave.Enemies[1].SpawnMethod = 0
ScriptTable.BasicMediumWave.Enemies[2] = {}
ScriptTable.BasicMediumWave.Enemies[2].Type = 12
ScriptTable.BasicMediumWave.Enemies[2].XPos = 150.0
ScriptTable.BasicMediumWave.Enemies[2].YPos = 0.0
ScriptTable.BasicMediumWave.Enemies[2].Speed = 40.0
ScriptTable.BasicMediumWave.Enemies[2].SpawnMethod = 5
ScriptTable.BasicMediumWave.Enemies[3] = {}
ScriptTable.BasicMediumWave.Enemies[3].Type = 11
ScriptTable.BasicMediumWave.Enemies[3].XPos = 100.0
ScriptTable.BasicMediumWave.Enemies[3].YPos = 25.0
ScriptTable.BasicMediumWave.Enemies[3].Speed = 50.0
ScriptTable.BasicMediumWave.Enemies[3].SpawnMethod = 1
ScriptTable.BasicMediumWave.Enemies[4] = {}
ScriptTable.BasicMediumWave.Enemies[4].Type = 13
ScriptTable.BasicMediumWave.Enemies[4].XPos = -200.0
ScriptTable.BasicMediumWave.Enemies[4].YPos = 0.0
ScriptTable.BasicMediumWave.Enemies[4].Speed = 130.0
ScriptTable.BasicMediumWave.Enemies[4].SpawnMethod = 0
ScriptTable.BasicMediumWave.Enemies[5] = {}
ScriptTable.BasicMediumWave.Enemies[5].Type = 11
ScriptTable.BasicMediumWave.Enemies[5].XPos = -230.0
ScriptTable.BasicMediumWave.Enemies[5].YPos = 40.0
ScriptTable.BasicMediumWave.Enemies[5].Speed = 50.0
ScriptTable.BasicMediumWave.Enemies[5].SpawnMethod = 0
ScriptTable.BasicMediumWave.Enemies[6] = {}
ScriptTable.BasicMediumWave.Enemies[6].Type = 12
ScriptTable.BasicMediumWave.Enemies[6].XPos = -250.0
ScriptTable.BasicMediumWave.Enemies[6].YPos = 100.0
ScriptTable.BasicMediumWave.Enemies[6].Speed = 40.0
ScriptTable.BasicMediumWave.Enemies[6].SpawnMethod = 0
ScriptTable.BasicMediumWave.Enemies[7] = {}
ScriptTable.BasicMediumWave.Enemies[7].Type = 34
ScriptTable.BasicMediumWave.Enemies[7].XPos = 100.0
ScriptTable.BasicMediumWave.Enemies[7].YPos = 200.0
ScriptTable.BasicMediumWave.Enemies[7].Speed = false
ScriptTable.BasicMediumWave.Enemies[7].SpawnMethod = 0
ScriptTable.BasicMediumWave.Enemies[8] = {}
ScriptTable.BasicMediumWave.Enemies[8].Type = 34
ScriptTable.BasicMediumWave.Enemies[8].XPos = -100.0
ScriptTable.BasicMediumWave.Enemies[8].YPos = 200.0
ScriptTable.BasicMediumWave.Enemies[8].Speed = false
ScriptTable.BasicMediumWave.Enemies[8].SpawnMethod = 0

--Basic heavy wave
ScriptTable.BasicHeavyWave = {}
ScriptTable.BasicHeavyWave.Number = 14
ScriptTable.BasicHeavyWave.Enemies = {}
ScriptTable.BasicHeavyWave.Enemies[1] = {}
ScriptTable.BasicHeavyWave.Enemies[1].Type = 11
ScriptTable.BasicHeavyWave.Enemies[1].XPos = 100.0
ScriptTable.BasicHeavyWave.Enemies[1].YPos = 25.0
ScriptTable.BasicHeavyWave.Enemies[1].Speed = 50.0
ScriptTable.BasicHeavyWave.Enemies[1].SpawnMethod = 1
ScriptTable.BasicHeavyWave.Enemies[2] = {}
ScriptTable.BasicHeavyWave.Enemies[2].Type = 12
ScriptTable.BasicHeavyWave.Enemies[2].XPos = 220.0
ScriptTable.BasicHeavyWave.Enemies[2].YPos = 0.0
ScriptTable.BasicHeavyWave.Enemies[2].Speed = 40.0
ScriptTable.BasicHeavyWave.Enemies[2].SpawnMethod = 5
ScriptTable.BasicHeavyWave.Enemies[3] = {}
ScriptTable.BasicHeavyWave.Enemies[3].Type = 11
ScriptTable.BasicHeavyWave.Enemies[3].XPos = 130.0
ScriptTable.BasicHeavyWave.Enemies[3].YPos = 25.0
ScriptTable.BasicHeavyWave.Enemies[3].Speed = 50.0
ScriptTable.BasicHeavyWave.Enemies[3].SpawnMethod = 1
ScriptTable.BasicHeavyWave.Enemies[4] = {}
ScriptTable.BasicHeavyWave.Enemies[4].Type = 11
ScriptTable.BasicHeavyWave.Enemies[4].XPos = 270.0
ScriptTable.BasicHeavyWave.Enemies[4].YPos = 100.0
ScriptTable.BasicHeavyWave.Enemies[4].Speed = 50.0
ScriptTable.BasicHeavyWave.Enemies[4].SpawnMethod = 0
ScriptTable.BasicHeavyWave.Enemies[5] = {}
ScriptTable.BasicHeavyWave.Enemies[5].Type = 13
ScriptTable.BasicHeavyWave.Enemies[5].XPos = -220.0
ScriptTable.BasicHeavyWave.Enemies[5].YPos = 0.0
ScriptTable.BasicHeavyWave.Enemies[5].Speed = 130.0
ScriptTable.BasicHeavyWave.Enemies[5].SpawnMethod = 0
ScriptTable.BasicHeavyWave.Enemies[6] = {}
ScriptTable.BasicHeavyWave.Enemies[6].Type = 13
ScriptTable.BasicHeavyWave.Enemies[6].XPos = 200.0
ScriptTable.BasicHeavyWave.Enemies[6].YPos = 0.0
ScriptTable.BasicHeavyWave.Enemies[6].Speed = 130.0
ScriptTable.BasicHeavyWave.Enemies[6].SpawnMethod = 0
ScriptTable.BasicHeavyWave.Enemies[7] = {}
ScriptTable.BasicHeavyWave.Enemies[7].Type = 13
ScriptTable.BasicHeavyWave.Enemies[7].XPos = -200.0
ScriptTable.BasicHeavyWave.Enemies[7].YPos = 0.0
ScriptTable.BasicHeavyWave.Enemies[7].Speed = 130.0
ScriptTable.BasicHeavyWave.Enemies[7].SpawnMethod = 0
ScriptTable.BasicHeavyWave.Enemies[8] = {}
ScriptTable.BasicHeavyWave.Enemies[8].Type = 11
ScriptTable.BasicHeavyWave.Enemies[8].XPos = -220.0
ScriptTable.BasicHeavyWave.Enemies[8].YPos = 100.0
ScriptTable.BasicHeavyWave.Enemies[8].Speed = 50.0
ScriptTable.BasicHeavyWave.Enemies[8].SpawnMethod = 0
ScriptTable.BasicHeavyWave.Enemies[9] = {}
ScriptTable.BasicHeavyWave.Enemies[9].Type = 12
ScriptTable.BasicHeavyWave.Enemies[9].XPos = 50.0
ScriptTable.BasicHeavyWave.Enemies[9].YPos = 0.0
ScriptTable.BasicHeavyWave.Enemies[9].Speed = 40.0
ScriptTable.BasicHeavyWave.Enemies[9].SpawnMethod = 5
ScriptTable.BasicHeavyWave.Enemies[10] = {}
ScriptTable.BasicHeavyWave.Enemies[10].Type = 12
ScriptTable.BasicHeavyWave.Enemies[10].XPos = -70.0
ScriptTable.BasicHeavyWave.Enemies[10].YPos = 0.0
ScriptTable.BasicHeavyWave.Enemies[10].Speed = 40.0
ScriptTable.BasicHeavyWave.Enemies[10].SpawnMethod = 5
ScriptTable.BasicHeavyWave.Enemies[11] = {}
ScriptTable.BasicHeavyWave.Enemies[11].Type = 11
ScriptTable.BasicHeavyWave.Enemies[11].XPos = -300.0
ScriptTable.BasicHeavyWave.Enemies[11].YPos = 100.0
ScriptTable.BasicHeavyWave.Enemies[11].Speed = 50.0
ScriptTable.BasicHeavyWave.Enemies[11].SpawnMethod = 0
ScriptTable.BasicHeavyWave.Enemies[12] = {}
ScriptTable.BasicHeavyWave.Enemies[12].Type = 11
ScriptTable.BasicHeavyWave.Enemies[12].XPos = -320.0
ScriptTable.BasicHeavyWave.Enemies[12].YPos = 100.0
ScriptTable.BasicHeavyWave.Enemies[12].Speed = 50.0
ScriptTable.BasicHeavyWave.Enemies[12].SpawnMethod = 0
ScriptTable.BasicHeavyWave.Enemies[13] = {}
ScriptTable.BasicHeavyWave.Enemies[13].Type = 34
ScriptTable.BasicHeavyWave.Enemies[13].XPos = 100.0
ScriptTable.BasicHeavyWave.Enemies[13].YPos = 200.0
ScriptTable.BasicHeavyWave.Enemies[13].Speed = false
ScriptTable.BasicHeavyWave.Enemies[13].SpawnMethod = 0
ScriptTable.BasicHeavyWave.Enemies[14] = {}
ScriptTable.BasicHeavyWave.Enemies[14].Type = 34
ScriptTable.BasicHeavyWave.Enemies[14].XPos = -100.0
ScriptTable.BasicHeavyWave.Enemies[14].YPos = 200.0
ScriptTable.BasicHeavyWave.Enemies[14].Speed = false
ScriptTable.BasicHeavyWave.Enemies[14].SpawnMethod = 0

--The small enemies that trickle forward
ScriptTable.LightFlow = {}
ScriptTable.LightFlow.Number = 2
ScriptTable.LightFlow.Enemies = {}
ScriptTable.LightFlow.Enemies[1] = {}
ScriptTable.LightFlow.Enemies[1].Type = 11
ScriptTable.LightFlow.Enemies[1].XPos = 250.0
ScriptTable.LightFlow.Enemies[1].YPos = 100.0
ScriptTable.LightFlow.Enemies[1].Speed = 60.0
ScriptTable.LightFlow.Enemies[1].SpawnMethod = 0
ScriptTable.LightFlow.Enemies[2] = {}
ScriptTable.LightFlow.Enemies[2].Type = 12
ScriptTable.LightFlow.Enemies[2].XPos = 250.0
ScriptTable.LightFlow.Enemies[2].YPos = 100.0
ScriptTable.LightFlow.Enemies[2].Speed = 40.0
ScriptTable.LightFlow.Enemies[2].SpawnMethod = 0

ScriptTable.ShootingGal1 = {}
ScriptTable.ShootingGal1.Number = 8
ScriptTable.ShootingGal1.Enemies = {}
ScriptTable.ShootingGal1.Enemies[1] = {}
ScriptTable.ShootingGal1.Enemies[1].Type = 11
ScriptTable.ShootingGal1.Enemies[1].XPos = 200.0
ScriptTable.ShootingGal1.Enemies[1].YPos = 100.0
ScriptTable.ShootingGal1.Enemies[1].Speed = 50.0
ScriptTable.ShootingGal1.Enemies[1].SpawnMethod = 0
ScriptTable.ShootingGal1.Enemies[2] = {}
ScriptTable.ShootingGal1.Enemies[2].Type = 11
ScriptTable.ShootingGal1.Enemies[2].XPos = 230.0
ScriptTable.ShootingGal1.Enemies[2].YPos = 100.0
ScriptTable.ShootingGal1.Enemies[2].Speed = 50.0
ScriptTable.ShootingGal1.Enemies[2].SpawnMethod = 0
ScriptTable.ShootingGal1.Enemies[3] = {}
ScriptTable.ShootingGal1.Enemies[3].Type = 11
ScriptTable.ShootingGal1.Enemies[3].XPos = 260.0
ScriptTable.ShootingGal1.Enemies[3].YPos = 100.0
ScriptTable.ShootingGal1.Enemies[3].Speed = 50.0
ScriptTable.ShootingGal1.Enemies[3].SpawnMethod = 0
ScriptTable.ShootingGal1.Enemies[4] = {}
ScriptTable.ShootingGal1.Enemies[4].Type = 11
ScriptTable.ShootingGal1.Enemies[4].XPos = 290.0
ScriptTable.ShootingGal1.Enemies[4].YPos = 100.0
ScriptTable.ShootingGal1.Enemies[4].Speed = 50.0
ScriptTable.ShootingGal1.Enemies[4].SpawnMethod = 0
ScriptTable.ShootingGal1.Enemies[5] = {}
ScriptTable.ShootingGal1.Enemies[5].Type = 11
ScriptTable.ShootingGal1.Enemies[5].XPos = 340.0
ScriptTable.ShootingGal1.Enemies[5].YPos = 100.0
ScriptTable.ShootingGal1.Enemies[5].Speed = 50.0
ScriptTable.ShootingGal1.Enemies[5].SpawnMethod = 0
ScriptTable.ShootingGal1.Enemies[6] = {}
ScriptTable.ShootingGal1.Enemies[6].Type = 13
ScriptTable.ShootingGal1.Enemies[6].XPos = 200.0
ScriptTable.ShootingGal1.Enemies[6].YPos = 100.0
ScriptTable.ShootingGal1.Enemies[6].Speed = 60.0
ScriptTable.ShootingGal1.Enemies[6].SpawnMethod = 0
ScriptTable.ShootingGal1.Enemies[7] = {}
ScriptTable.ShootingGal1.Enemies[7].Type = 13
ScriptTable.ShootingGal1.Enemies[7].XPos = 250.0
ScriptTable.ShootingGal1.Enemies[7].YPos = 0.0
ScriptTable.ShootingGal1.Enemies[7].Speed = 60.0
ScriptTable.ShootingGal1.Enemies[7].SpawnMethod = 0
ScriptTable.ShootingGal1.Enemies[8] = {}
ScriptTable.ShootingGal1.Enemies[8].Type = 13
ScriptTable.ShootingGal1.Enemies[8].XPos = -200.0
ScriptTable.ShootingGal1.Enemies[8].YPos = 0.0
ScriptTable.ShootingGal1.Enemies[8].Speed = 70.0
ScriptTable.ShootingGal1.Enemies[8].SpawnMethod = 0

ScriptTable.ShootingGal1Augment = {}
ScriptTable.ShootingGal1Augment.Number = 4
ScriptTable.ShootingGal1Augment.Enemies = {}
ScriptTable.ShootingGal1Augment.Enemies[1] = {}
ScriptTable.ShootingGal1Augment.Enemies[1].Type = 12
ScriptTable.ShootingGal1Augment.Enemies[1].XPos = 50.0
ScriptTable.ShootingGal1Augment.Enemies[1].YPos = 10.0
ScriptTable.ShootingGal1Augment.Enemies[1].Speed = 50.0
ScriptTable.ShootingGal1Augment.Enemies[1].SpawnMethod = 5
ScriptTable.ShootingGal1Augment.Enemies[2] = {}
ScriptTable.ShootingGal1Augment.Enemies[2].Type = 12
ScriptTable.ShootingGal1Augment.Enemies[2].XPos = 100.0
ScriptTable.ShootingGal1Augment.Enemies[2].YPos = 10.0
ScriptTable.ShootingGal1Augment.Enemies[2].Speed = 50.0
ScriptTable.ShootingGal1Augment.Enemies[2].SpawnMethod = 5
ScriptTable.ShootingGal1Augment.Enemies[3] = {}
ScriptTable.ShootingGal1Augment.Enemies[3].Type = 12
ScriptTable.ShootingGal1Augment.Enemies[3].XPos = -50.0
ScriptTable.ShootingGal1Augment.Enemies[3].YPos = 10.0
ScriptTable.ShootingGal1Augment.Enemies[3].Speed = 50.0
ScriptTable.ShootingGal1Augment.Enemies[3].SpawnMethod = 5
ScriptTable.ShootingGal1Augment.Enemies[4] = {}
ScriptTable.ShootingGal1Augment.Enemies[4].Type = 12
ScriptTable.ShootingGal1Augment.Enemies[4].XPos = -100.0
ScriptTable.ShootingGal1Augment.Enemies[4].YPos = 10.0
ScriptTable.ShootingGal1Augment.Enemies[4].Speed = 50.0
ScriptTable.ShootingGal1Augment.Enemies[4].SpawnMethod = 5


ScriptTable.ShootingGal2 = {}
ScriptTable.ShootingGal2.Number = 8
ScriptTable.ShootingGal2.Enemies = {}
ScriptTable.ShootingGal2.Enemies[1] = {}
ScriptTable.ShootingGal2.Enemies[1].Type = 11
ScriptTable.ShootingGal2.Enemies[1].XPos = 200.0
ScriptTable.ShootingGal2.Enemies[1].YPos = 100.0
ScriptTable.ShootingGal2.Enemies[1].Speed = 50.0
ScriptTable.ShootingGal2.Enemies[1].SpawnMethod = 0
ScriptTable.ShootingGal2.Enemies[2] = {}
ScriptTable.ShootingGal2.Enemies[2].Type = 11
ScriptTable.ShootingGal2.Enemies[2].XPos = 230.0
ScriptTable.ShootingGal2.Enemies[2].YPos = 100.0
ScriptTable.ShootingGal2.Enemies[2].Speed = 50.0
ScriptTable.ShootingGal2.Enemies[2].SpawnMethod = 0
ScriptTable.ShootingGal2.Enemies[3] = {}
ScriptTable.ShootingGal2.Enemies[3].Type = 11
ScriptTable.ShootingGal2.Enemies[3].XPos = 260.0
ScriptTable.ShootingGal2.Enemies[3].YPos = 100.0
ScriptTable.ShootingGal2.Enemies[3].Speed = 50.0
ScriptTable.ShootingGal2.Enemies[3].SpawnMethod = 0
ScriptTable.ShootingGal2.Enemies[4] = {}
ScriptTable.ShootingGal2.Enemies[4].Type = 11
ScriptTable.ShootingGal2.Enemies[4].XPos = 290.0
ScriptTable.ShootingGal2.Enemies[4].YPos = 100.0
ScriptTable.ShootingGal2.Enemies[4].Speed = 50.0
ScriptTable.ShootingGal2.Enemies[4].SpawnMethod = 0
ScriptTable.ShootingGal2.Enemies[5] = {}
ScriptTable.ShootingGal2.Enemies[5].Type = 13
ScriptTable.ShootingGal2.Enemies[5].XPos = 350.0
ScriptTable.ShootingGal2.Enemies[5].YPos = 100.0
ScriptTable.ShootingGal2.Enemies[5].Speed = 60.0
ScriptTable.ShootingGal2.Enemies[5].SpawnMethod = 0
ScriptTable.ShootingGal2.Enemies[6] = {}
ScriptTable.ShootingGal2.Enemies[6].Type = 13
ScriptTable.ShootingGal2.Enemies[6].XPos = 200.0
ScriptTable.ShootingGal2.Enemies[6].YPos = 100.0
ScriptTable.ShootingGal2.Enemies[6].Speed = 60.0
ScriptTable.ShootingGal2.Enemies[6].SpawnMethod = 0
ScriptTable.ShootingGal2.Enemies[7] = {}
ScriptTable.ShootingGal2.Enemies[7].Type = 13
ScriptTable.ShootingGal2.Enemies[7].XPos = 250.0
ScriptTable.ShootingGal2.Enemies[7].YPos = 0.0
ScriptTable.ShootingGal2.Enemies[7].Speed = 60.0
ScriptTable.ShootingGal2.Enemies[7].SpawnMethod = 0
ScriptTable.ShootingGal2.Enemies[8] = {}
ScriptTable.ShootingGal2.Enemies[8].Type = 13
ScriptTable.ShootingGal2.Enemies[8].XPos = 300.0
ScriptTable.ShootingGal2.Enemies[8].YPos = 0.0
ScriptTable.ShootingGal2.Enemies[8].Speed = 60.0
ScriptTable.ShootingGal2.Enemies[8].SpawnMethod = 0

ScriptTable.ShootingGal2Augment = {}
ScriptTable.ShootingGal2Augment.Number = 8
ScriptTable.ShootingGal2Augment.Enemies = {}
ScriptTable.ShootingGal2Augment.Enemies[1] = {}
ScriptTable.ShootingGal2Augment.Enemies[1].Type = 12
ScriptTable.ShootingGal2Augment.Enemies[1].XPos = 50.0
ScriptTable.ShootingGal2Augment.Enemies[1].YPos = 0.0
ScriptTable.ShootingGal2Augment.Enemies[1].Speed = 50.0
ScriptTable.ShootingGal2Augment.Enemies[1].SpawnMethod = 5
ScriptTable.ShootingGal2Augment.Enemies[2] = {}
ScriptTable.ShootingGal2Augment.Enemies[2].Type = 12
ScriptTable.ShootingGal2Augment.Enemies[2].XPos = 100.0
ScriptTable.ShootingGal2Augment.Enemies[2].YPos = 0.0
ScriptTable.ShootingGal2Augment.Enemies[2].Speed = 50.0
ScriptTable.ShootingGal2Augment.Enemies[2].SpawnMethod = 5
ScriptTable.ShootingGal2Augment.Enemies[3] = {}
ScriptTable.ShootingGal2Augment.Enemies[3].Type = 12
ScriptTable.ShootingGal2Augment.Enemies[3].XPos = -50.0
ScriptTable.ShootingGal2Augment.Enemies[3].YPos = 0.0
ScriptTable.ShootingGal2Augment.Enemies[3].Speed = 50.0
ScriptTable.ShootingGal2Augment.Enemies[3].SpawnMethod = 5
ScriptTable.ShootingGal2Augment.Enemies[4] = {}
ScriptTable.ShootingGal2Augment.Enemies[4].Type = 12
ScriptTable.ShootingGal2Augment.Enemies[4].XPos = -100.0
ScriptTable.ShootingGal2Augment.Enemies[4].YPos = 0.0
ScriptTable.ShootingGal2Augment.Enemies[4].Speed = 50.0
ScriptTable.ShootingGal2Augment.Enemies[4].SpawnMethod = 5
ScriptTable.ShootingGal2Augment.Enemies[5] = {}
ScriptTable.ShootingGal2Augment.Enemies[5].Type = 11
ScriptTable.ShootingGal2Augment.Enemies[5].XPos = 250.0
ScriptTable.ShootingGal2Augment.Enemies[5].YPos = 150.0
ScriptTable.ShootingGal2Augment.Enemies[5].Speed = 50.0
ScriptTable.ShootingGal2Augment.Enemies[5].SpawnMethod = 0
ScriptTable.ShootingGal2Augment.Enemies[6] = {}
ScriptTable.ShootingGal2Augment.Enemies[6].Type = 11
ScriptTable.ShootingGal2Augment.Enemies[6].XPos = 280.0
ScriptTable.ShootingGal2Augment.Enemies[6].YPos = 150.0
ScriptTable.ShootingGal2Augment.Enemies[6].Speed = 50.0
ScriptTable.ShootingGal2Augment.Enemies[6].SpawnMethod = 0
ScriptTable.ShootingGal2Augment.Enemies[7] = {}
ScriptTable.ShootingGal2Augment.Enemies[7].Type = 11
ScriptTable.ShootingGal2Augment.Enemies[7].XPos = 220.0
ScriptTable.ShootingGal2Augment.Enemies[7].YPos = 150.0
ScriptTable.ShootingGal2Augment.Enemies[7].Speed = 50.0
ScriptTable.ShootingGal2Augment.Enemies[7].SpawnMethod = 0
ScriptTable.ShootingGal2Augment.Enemies[8] = {}
ScriptTable.ShootingGal2Augment.Enemies[8].Type = 11
ScriptTable.ShootingGal2Augment.Enemies[8].XPos = 300.0
ScriptTable.ShootingGal2Augment.Enemies[8].YPos = 150.0
ScriptTable.ShootingGal2Augment.Enemies[8].Speed = 50.0
ScriptTable.ShootingGal2Augment.Enemies[8].SpawnMethod = 0


--Below are the waves for if you're playing the game single player
ScriptTable.SoloGal1 = {}
ScriptTable.SoloGal1.Number = 8
ScriptTable.SoloGal1.Enemies = {}
ScriptTable.SoloGal1.Enemies[1] = {}
ScriptTable.SoloGal1.Enemies[1].Type = 12
ScriptTable.SoloGal1.Enemies[1].XPos = 50.0
ScriptTable.SoloGal1.Enemies[1].YPos = 0.0
ScriptTable.SoloGal1.Enemies[1].Speed = 50.0
ScriptTable.SoloGal1.Enemies[1].SpawnMethod = 5
ScriptTable.SoloGal1.Enemies[2] = {}
ScriptTable.SoloGal1.Enemies[2].Type = 12
ScriptTable.SoloGal1.Enemies[2].XPos = 100.0
ScriptTable.SoloGal1.Enemies[2].YPos = 0.0
ScriptTable.SoloGal1.Enemies[2].Speed = 50.0
ScriptTable.SoloGal1.Enemies[2].SpawnMethod = 5
ScriptTable.SoloGal1.Enemies[3] = {}
ScriptTable.SoloGal1.Enemies[3].Type = 12
ScriptTable.SoloGal1.Enemies[3].XPos = -50.0
ScriptTable.SoloGal1.Enemies[3].YPos = 0.0
ScriptTable.SoloGal1.Enemies[3].Speed = 50.0
ScriptTable.SoloGal1.Enemies[3].SpawnMethod = 5
ScriptTable.SoloGal1.Enemies[4] = {}
ScriptTable.SoloGal1.Enemies[4].Type = 12
ScriptTable.SoloGal1.Enemies[4].XPos = -100.0
ScriptTable.SoloGal1.Enemies[4].YPos = 0.0
ScriptTable.SoloGal1.Enemies[4].Speed = 50.0
ScriptTable.SoloGal1.Enemies[4].SpawnMethod = 5
ScriptTable.SoloGal1.Enemies[5] = {}
ScriptTable.SoloGal1.Enemies[5].Type = 11
ScriptTable.SoloGal1.Enemies[5].XPos = 250.0
ScriptTable.SoloGal1.Enemies[5].YPos = 100.0
ScriptTable.SoloGal1.Enemies[5].Speed = 50.0
ScriptTable.SoloGal1.Enemies[5].SpawnMethod = 0
ScriptTable.SoloGal1.Enemies[6] = {}
ScriptTable.SoloGal1.Enemies[6].Type = 11
ScriptTable.SoloGal1.Enemies[6].XPos = 280.0
ScriptTable.SoloGal1.Enemies[6].YPos = 100.0
ScriptTable.SoloGal1.Enemies[6].Speed = 50.0
ScriptTable.SoloGal1.Enemies[6].SpawnMethod = 0
ScriptTable.SoloGal1.Enemies[7] = {}
ScriptTable.SoloGal1.Enemies[7].Type = 11
ScriptTable.SoloGal1.Enemies[7].XPos = 220.0
ScriptTable.SoloGal1.Enemies[7].YPos = 100.0
ScriptTable.SoloGal1.Enemies[7].Speed = 50.0
ScriptTable.SoloGal1.Enemies[7].SpawnMethod = 0
ScriptTable.SoloGal1.Enemies[8] = {}
ScriptTable.SoloGal1.Enemies[8].Type = 11
ScriptTable.SoloGal1.Enemies[8].XPos = 300.0
ScriptTable.SoloGal1.Enemies[8].YPos = 100.0
ScriptTable.SoloGal1.Enemies[8].Speed = 50.0
ScriptTable.SoloGal1.Enemies[8].SpawnMethod = 0

ScriptTable.SoloGal2 = {}
ScriptTable.SoloGal2.Number = 12
ScriptTable.SoloGal2.Enemies = {}
ScriptTable.SoloGal2.Enemies[1] = {}
ScriptTable.SoloGal2.Enemies[1].Type = 12
ScriptTable.SoloGal2.Enemies[1].XPos = 50.0
ScriptTable.SoloGal2.Enemies[1].YPos = 0.0
ScriptTable.SoloGal2.Enemies[1].Speed = 50.0
ScriptTable.SoloGal2.Enemies[1].SpawnMethod = 5
ScriptTable.SoloGal2.Enemies[2] = {}
ScriptTable.SoloGal2.Enemies[2].Type = 12
ScriptTable.SoloGal2.Enemies[2].XPos = 100.0
ScriptTable.SoloGal2.Enemies[2].YPos = 0.0
ScriptTable.SoloGal2.Enemies[2].Speed = 50.0
ScriptTable.SoloGal2.Enemies[2].SpawnMethod = 5
ScriptTable.SoloGal2.Enemies[3] = {}
ScriptTable.SoloGal2.Enemies[3].Type = 12
ScriptTable.SoloGal2.Enemies[3].XPos = -50.0
ScriptTable.SoloGal2.Enemies[3].YPos = 0.0
ScriptTable.SoloGal2.Enemies[3].Speed = 50.0
ScriptTable.SoloGal2.Enemies[3].SpawnMethod = 5
ScriptTable.SoloGal2.Enemies[4] = {}
ScriptTable.SoloGal2.Enemies[4].Type = 12
ScriptTable.SoloGal2.Enemies[4].XPos = -100.0
ScriptTable.SoloGal2.Enemies[4].YPos = 0.0
ScriptTable.SoloGal2.Enemies[4].Speed = 50.0
ScriptTable.SoloGal2.Enemies[4].SpawnMethod = 5
ScriptTable.SoloGal2.Enemies[5] = {}
ScriptTable.SoloGal2.Enemies[5].Type = 11
ScriptTable.SoloGal2.Enemies[5].XPos = 250.0
ScriptTable.SoloGal2.Enemies[5].YPos = 150.0
ScriptTable.SoloGal2.Enemies[5].Speed = 50.0
ScriptTable.SoloGal2.Enemies[5].SpawnMethod = 0
ScriptTable.SoloGal2.Enemies[6] = {}
ScriptTable.SoloGal2.Enemies[6].Type = 11
ScriptTable.SoloGal2.Enemies[6].XPos = 280.0
ScriptTable.SoloGal2.Enemies[6].YPos = 150.0
ScriptTable.SoloGal2.Enemies[6].Speed = 50.0
ScriptTable.SoloGal2.Enemies[6].SpawnMethod = 0
ScriptTable.SoloGal2.Enemies[7] = {}
ScriptTable.SoloGal2.Enemies[7].Type = 11
ScriptTable.SoloGal2.Enemies[7].XPos = 220.0
ScriptTable.SoloGal2.Enemies[7].YPos = 150.0
ScriptTable.SoloGal2.Enemies[7].Speed = 50.0
ScriptTable.SoloGal2.Enemies[7].SpawnMethod = 0
ScriptTable.SoloGal2.Enemies[8] = {}
ScriptTable.SoloGal2.Enemies[8].Type = 11
ScriptTable.SoloGal2.Enemies[8].XPos = 300.0
ScriptTable.SoloGal2.Enemies[8].YPos = 150.0
ScriptTable.SoloGal2.Enemies[8].Speed = 50.0
ScriptTable.SoloGal2.Enemies[8].SpawnMethod = 0
ScriptTable.SoloGal2.Enemies[9] = {}
ScriptTable.SoloGal2.Enemies[9].Type = 11
ScriptTable.SoloGal2.Enemies[9].XPos = 300.0
ScriptTable.SoloGal2.Enemies[9].YPos = 150.0
ScriptTable.SoloGal2.Enemies[9].Speed = 50.0
ScriptTable.SoloGal2.Enemies[9].SpawnMethod = 0
ScriptTable.SoloGal2.Enemies[10] = {}
ScriptTable.SoloGal2.Enemies[10].Type = 11
ScriptTable.SoloGal2.Enemies[10].XPos = 300.0
ScriptTable.SoloGal2.Enemies[10].YPos = 150.0
ScriptTable.SoloGal2.Enemies[10].Speed = 50.0
ScriptTable.SoloGal2.Enemies[10].SpawnMethod = 0
ScriptTable.SoloGal2.Enemies[11] = {}
ScriptTable.SoloGal2.Enemies[11].Type = 11
ScriptTable.SoloGal2.Enemies[11].XPos = 300.0
ScriptTable.SoloGal2.Enemies[11].YPos = 150.0
ScriptTable.SoloGal2.Enemies[11].Speed = 50.0
ScriptTable.SoloGal2.Enemies[11].SpawnMethod = 0
ScriptTable.SoloGal2.Enemies[12] = {}
ScriptTable.SoloGal2.Enemies[12].Type = 11
ScriptTable.SoloGal2.Enemies[12].XPos = 300.0
ScriptTable.SoloGal2.Enemies[12].YPos = 150.0
ScriptTable.SoloGal2.Enemies[12].Speed = 50.0
ScriptTable.SoloGal2.Enemies[12].SpawnMethod = 0

function Script(name, camX, playerZ)
	for count = 1, ScriptTable[name].Number, 1 do
		CreateEnemy(ScriptTable[name].Enemies[count].Type, ScriptTable[name].Enemies[count].XPos + camX, ScriptTable[name].Enemies[count].YPos, playerZ, ScriptTable[name].Enemies[count].Speed, ScriptTable[name].Enemies[count].SpawnMethod)
	end
end