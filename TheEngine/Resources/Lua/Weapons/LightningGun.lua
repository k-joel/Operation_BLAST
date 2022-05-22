LightningGun = {}

--Firing rates
LightningGun.NormalFiringRate = 0.75
LightningGun.AlternateFiringRate = 1.0

--Firing speeds
LightningGun.NormalFiringSpeed = 400.0
LightningGun.AlternateFiringSpeed = 350.0
LightningGun.UltimateFiringSpeed = 700.0

--Bullet life
LightningGun.NormalBulletLife = 5.0
LightningGun.AlternateBulletLife = 5.0

--Ammo count
LightningGun.MaxAmmo = 50

--Alternate cost
LightningGun.AltCost = 10

--Arc mechanics
LightningGun.ArcRadius = 2.0
LightningGun.MaxJumps = 3.0

--Damage
LightningGun.NormalDamage = 1.5
LightningGun.AltDamage = 6.0
LightningGun.UltDamage = 40.0

--Ultimate mechanics
LightningGun.UltBaseDelay = 0.75
LightningGun.UltDelayDecayRate = 0.5
LightningGun.UltMaxBolts = 500

function GetLightningGunData(data)
	return LightningGun[data]
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
		else		
			rotationY = math.rad(270)		
			if dir == 0 then
				rotationZ = math.rad(90)
				x = math.cos(rotationZ)
				y = math.sin(rotationZ)
			elseif dir == 1 then
				rotationZ = math.rad(45)
				x = math.cos(rotationZ)
				y = math.sin(rotationZ)
			elseif dir == 2 then
				rotationZ = math.rad(0)
				x = math.cos(rotationZ)
				y = math.sin(rotationZ)
			elseif dir == 3 then
				rotationZ = math.rad(315)
				x = math.cos(rotationZ)
				y = math.sin(rotationZ)
			else
				rotationZ = math.rad(270)
				x = math.cos(rotationZ)
				y = math.sin(rotationZ)
			end
		end
		
		x = x * LightningGun["NormalFiringSpeed"]
		y = y * LightningGun["NormalFiringSpeed"]
		
		return LightningGun["NormalBulletLife"], LightningGun["NormalDamage"], x, y, rotationY, rotationZ	
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
		else		
			rotationY = math.rad(270)		
			if dir == 0 then
				rotationZ = math.rad(90)
				x = math.cos(rotationZ)
				y = math.sin(rotationZ)
			elseif dir == 1 then
				rotationZ = math.rad(45)
				x = math.cos(rotationZ)
				y = math.sin(rotationZ)
			elseif dir == 2 then
				rotationZ = math.rad(0)
				x = math.cos(rotationZ)
				y = math.sin(rotationZ)
			elseif dir == 3 then
				rotationZ = math.rad(315)
				x = math.cos(rotationZ)
				y = math.sin(rotationZ)
			else
				rotationZ = math.rad(270)
				x = math.cos(rotationZ)
				y = math.sin(rotationZ)
			end
		end
		
		x = x * LightningGun["AlternateFiringSpeed"]
		y = y * LightningGun["AlternateFiringSpeed"]
		
		return LightningGun["AlternateBulletLife"], LightningGun["AltDamage"], x, y, rotationY, rotationZ	
	else
		return LightningGun["UltimateFiringSpeed"], LightningGun["UltDamage"], LightningGun["UltBaseDelay"], LightningGun["UltDelayDecayRate"], LightningGun["UltMaxBolts"]
	end
end