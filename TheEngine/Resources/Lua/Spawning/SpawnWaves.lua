WaveInfo = {}

--Number o' enemies
WaveInfo.LightNumber = 3
WaveInfo.MediumNumber = 6
WaveInfo.HeavyNumber = 12

--Position
WaveInfo.SpaceBetween = 4.0
WaveInfo.DistanceFromCam = 20.0


--The below funcZ:\Three Way Tie Studios\N Squared\Engine\TheEngine\Resources\Lua\Spawning\WaveScripts.luation returns a table that has the position of all the enemies for the wave
--The type of wave corrosponds to:
--		0 = light
--		1 = medium
--		2 = heavy
function CreateEnemyLocations(waveType, cameraPosX, cameraPosY, cameraPosZ)

	EnemiesToMake = {}
	
	if waveType == 0 then
		for index = 0, WaveInfo.LightNumber, 1 do
			EnemiesToMake.index = {}
		end
	elseif waveType == 1 then
	else
	end
	
	if waveType == 0 then
		return WaveInfo.LightNumber
	elseif waveType == 1 then
	
	else
	
	end
	
end