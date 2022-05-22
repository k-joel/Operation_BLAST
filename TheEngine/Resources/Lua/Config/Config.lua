dataTable = {}


--Level camera defaults

dataTable.camStartX = -73.5
dataTable.camStartY = 128.0
dataTable.camStartZ = -265.0

dataTable.camStartRot = 0.2

--Player start defaults
dataTable.playerStartX = -214.0
dataTable.playerStartY = 44.9
dataTable.playerStartZ = 0.0

--Machine Gun Bullet Velocity
dataTable.normalMachineGunVel = 250

--Shotgun Bullet Velocity
dataTable.normalShotgunVel = 175.0
dataTable.alternateShotgunVel = 225.0
dataTable.ultimateShotgunVel = 200.0

--Console start defaults
dataTable.consoleStartX = 0.0
dataTable.consoleStartY = 0.0
dataTable.consoleStartZ = 0.0

--Console end position
dataTable.consoleEndX = 0.0
dataTable.consoleEndY = 0.0
dataTable.consoleEndZ = 0.0

--Console Movement Velocity
dataTable.consoleVelocityX = 0.0
dataTable.consoleVelocityY = 0.0
dataTable.consoleVelocityZ = 0.0

--Console Movement Timer
dataTable.movementTimer = 2.0

--Console Camera Angle
dataTable.consoleAngle = 0.0

--Striker Variables
dataTable.strikerHealth = 3

--MainMenu Camera Postion
dataTable.mainConsoleX = -0.2
dataTable.mainConsoleY = 10.0
dataTable.mainConsoleZ = 20.1

--MainMenu Camera Pitch
dataTable.mainRotation = -0.06

--Computer Font Variables
dataTable.fontScale = 1.0

--Computer Font Variables
dataTable.cameraRate = 2.0

function GetVar(lookupName)
	 return dataTable[lookupName]
end
