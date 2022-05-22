RocketLauncher = {}

--Firing rates
RocketLauncher.NormalFiringRate = 0.5
RocketLauncher.AlternateFiringRate = 0.5

--Firing speeds
RocketLauncher.NormalFiringSpeed = 250.0
RocketLauncher.AlternateFiringSpeed = 250.0
RocketLauncher.UltimateFiringSpeed = 0.0

--Bullet life
RocketLauncher.NormalBulletLife = 20.0
RocketLauncher.AlternateBulletLife = 30.0

--Ammo count
RocketLauncher.MaxAmmo = 10

--Alternate cost
RocketLauncher.AltCost = 1

--Damage
RocketLauncher.NormalDamage = 50.0
RocketLauncher.AltDamage = 50.0
RocketLauncher.UltDamage = 100.0

function GetRocketLauncherData(data)
	return RocketLauncher[data]
end

function GetBulletInfo(dir, isLeft, type)
	if type == 0 then
		if isLeft == true then
			rotationY = math.rad(90)
			
			if dir == 0 then
				rotationZ = math.rad(90)
				x = math.cos(rotationZ)
				y = math.sin(rotationZ)
			elseif dir == 1 then
				rotationZ = math.rad(135)
				x = math.cos(rotationZ)
				y = math.sin(rotationZ)
			elseif dir == 2 then
				rotationZ = math.rad(180)
				x = math.cos(rotationZ)
				y = math.sin(rotationZ)
			elseif dir == 3 then
				rotationZ = math.rad(225)
				x = math.cos(rotationZ)
				y = math.sin(rotationZ)
			else
				rotationZ = math.rad(270)
				x = math.cos(rotationZ)
				y = math.sin(rotationZ)
			end
			x = x * RocketLauncher["NormalFiringSpeed"]
			y = y * RocketLauncher["NormalFiringSpeed"]
		else		
			rotationY = math.rad(270)		
			if dir == 0 then
				rotationZ = math.rad(270)
				x = math.cos(rotationZ)
				y = math.sin(rotationZ)
			elseif dir == 1 then
				rotationZ = math.rad(225)
				x = math.cos(rotationZ)
				y = math.sin(rotationZ)
			elseif dir == 2 then
				rotationZ = math.rad(180)
				x = math.cos(rotationZ)
				y = math.sin(rotationZ)
			elseif dir == 3 then
				rotationZ = math.rad(135)
				x = math.cos(rotationZ)
				y = math.sin(rotationZ)
			else
				rotationZ = math.rad(90)
				x = math.cos(rotationZ)
				y = math.sin(rotationZ)
			end
			x = -x * RocketLauncher["NormalFiringSpeed"]
			y = -y * RocketLauncher["NormalFiringSpeed"]
		end
		
		
		return RocketLauncher["NormalBulletLife"], RocketLauncher["NormalDamage"], x, y, rotationY, rotationZ
	elseif type == 1 then
		if isLeft == true then
			rotationY = math.rad(90)
		
			if dir == 0 then
				rotationZ = math.rad(90)
				x = math.cos(rotationZ)
				y = math.sin(rotationZ)
			elseif dir == 1 then
				rotationZ = math.rad(135)
				x = math.cos(rotationZ)
				y = math.sin(rotationZ)
			elseif dir == 2 then
				rotationZ = math.rad(180)
				x = math.cos(rotationZ)
				y = math.sin(rotationZ)
			elseif dir == 3 then
				rotationZ = math.rad(225)
				x = math.cos(rotationZ)
				y = math.sin(rotationZ)
			else
				rotationZ = math.rad(270)
				x = math.cos(rotationZ)
				y = math.sin(rotationZ)
			end
			x = x * RocketLauncher["AlternateFiringSpeed"]
			y = y * RocketLauncher["AlternateFiringSpeed"]
		else		
			rotationY = math.rad(270)		
			
			if dir == 0 then
				rotationZ = math.rad(270)
				x = math.cos(rotationZ)
				y = math.sin(rotationZ)
			elseif dir == 1 then
				rotationZ = math.rad(225)
				x = math.cos(rotationZ)
				y = math.sin(rotationZ)
			elseif dir == 2 then
				rotationZ = math.rad(180)
				x = math.cos(rotationZ)
				y = math.sin(rotationZ)
			elseif dir == 3 then
				rotationZ = math.rad(135)
				x = math.cos(rotationZ)
				y = math.sin(rotationZ)
			else
				rotationZ = math.rad(90)
				x = math.cos(rotationZ)
				y = math.sin(rotationZ)
			end
			x = -x * RocketLauncher["AlternateFiringSpeed"]
			y = -y * RocketLauncher["AlternateFiringSpeed"]
		end
	
			
		return RocketLauncher["AlternateBulletLife"], RocketLauncher["AltDamage"], x, y, rotationY, rotationZ
	else
	
	
	end
end