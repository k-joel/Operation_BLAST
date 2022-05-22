Flamethrower = {}

--Firing rates
Flamethrower.NormalFiringRate = 0.1
Flamethrower.AlternateFiringRate = 0.2
Flamethrower.NapalmRate = 0.1

--Firing speeds
Flamethrower.NormalFiringSpeed = 10.0
Flamethrower.AlternateFiringSpeed = 225.0
Flamethrower.UltimateFiringSpeed = 300.0

--Bullet life
Flamethrower.NormalBulletLife = 2.0
Flamethrower.AlternateBulletLife = 15.0
Flamethrower.UltimateBulletLife = 15.0
Flamethrower.NapalmLife = 4.0

--Ammo count
Flamethrower.MaxAmmo = 300

--Alternate cost
Flamethrower.AltCost = 50

--Damage
Flamethrower.NormalDamage = 0.3
Flamethrower.AltDamage = 10.0
Flamethrower.UltDamage = 25.0
Flamethrower.NapalmDamage = 0.2

function GetFlamethrowerData(data)
	return Flamethrower[data]
end

--[[
***********************************************
Type meaning:	0 - normal
				1 - alternate
				2 - ultimate
				
Dir meaning:	0 - up
				1 - angled up
				2 - forward
				3 - angled down
				4 - down
***********************************************
--]]

function GetBulletInfo(dir, isLeft, type)

	if type == 0 then
		if isLeft == false then
			rotationY = math.rad(90)
			
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
			x = -x * Flamethrower["NormalFiringSpeed"]
			y = -y * Flamethrower["NormalFiringSpeed"]
		else		
			rotationY = math.rad(270)		
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
			x = x * Flamethrower["NormalFiringSpeed"]
			y = y * Flamethrower["NormalFiringSpeed"]
		end
		
		return Flamethrower["NormalBulletLife"], Flamethrower["NormalDamage"], x, y
		
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
			
			x = x * Flamethrower["AlternateFiringSpeed"]
			y = y * Flamethrower["AlternateFiringSpeed"]

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

			x = -x * Flamethrower["AlternateFiringSpeed"]
			y = -y * Flamethrower["AlternateFiringSpeed"]
		end
		
		return Flamethrower["NapalmLife"], Flamethrower["NapalmDamage"], Flamethrower["NapalmRate"], Flamethrower["AlternateBulletLife"], Flamethrower["AltDamage"], x, y, rotationY, rotationZ
		
	elseif type == 2 then
	
		lRotZ = math.rad(0)
		rRotZ = math.rad(0)
		lRotY = math.rad(90)
		rRotY = math.rad(270)
		
		leftX = math.cos(lRotZ)

		rightX = math.cos(rRotZ)
		
		leftX = -leftX * Flamethrower["UltimateFiringSpeed"]
		
		rightX = rightX * Flamethrower["UltimateFiringSpeed"]
		
		return Flamethrower["UltDamage"], leftX, 0, lRotZ, lRotY, rightX, 0, rRotZ, rRotY
	end
end