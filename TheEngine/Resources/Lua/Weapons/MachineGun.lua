MachineGun = {}

--Firing rates
MachineGun.NormalFiringRate = 0.12

--Firing speeds
MachineGun.NormalFiringSpeed = 250.0

--Bullet life
MachineGun.NormalBulletLife = 15.0

--Ammo count
MachineGun.MaxAmmo = -100

--Damage
MachineGun.NormalDamage = 0.50

function GetMachineGunData(data)
	return MachineGun[data]
end

function GetBulletInfo(dir, isLeft)
	--[[The directions are associated with the following values:
	
	up			-	0
	angled up	-	1
	forward		-	2
	angled down -	3
	down		-	4
	--]]
	
	spread = (math.random(14) - 7 ) / 100.0
	x = 0				--X velocity
	y = 0				--Y velocity
	rotationY = 0		--Rotation for the bullet on the Y
	rotationZ = 0		--Rotation for the bullet on the Z
	
	if isLeft == true then
		rotationY = math.rad(270)
		if dir == 0 then
			rotationZ = math.rad(270)
			x = math.cos(rotationZ) + spread
			y = math.sin(rotationZ)
		elseif dir == 1 then
			rotationZ = math.rad(315)
			x = math.cos(rotationZ)
			y = math.sin(rotationZ) + spread
		elseif dir == 2 then
			rotationZ = math.rad(0)
			x = math.cos(rotationZ)
			y = math.sin(rotationZ) + spread
		elseif dir == 3 then
			rotationZ = math.rad(45)
			x = math.cos(rotationZ)
			y = math.sin(rotationZ) + spread
		else
			rotationZ = math.rad(90)
			x = math.cos(rotationZ) + spread
			y = math.sin(rotationZ)
		end
		
		x = -x * MachineGun["NormalFiringSpeed"]
		y = -y * MachineGun["NormalFiringSpeed"]
		
	else
		rotationY = math.rad(90)
		if dir == 0 then
			rotationZ = math.rad(90)
			x = math.cos(rotationZ) + spread
			y = math.sin(rotationZ)
		elseif dir == 1 then
			rotationZ = math.rad(45)
			x = math.cos(rotationZ)
			y = math.sin(rotationZ) + spread
		elseif dir == 2 then
			rotationZ = math.rad(0)
			x = math.cos(rotationZ)
			y = math.sin(rotationZ) + spread
		elseif dir == 3 then
			rotationZ = math.rad(315)
			x = math.cos(rotationZ)
			y = math.sin(rotationZ) + spread
		else
			rotationZ = math.rad(270)
			x = math.cos(rotationZ) + spread
			y = math.sin(rotationZ)
		end

		x = x * MachineGun["NormalFiringSpeed"]
		y = y * MachineGun["NormalFiringSpeed"]
	end
	
	
	return MachineGun["NormalBulletLife"], MachineGun["NormalDamage"], x, y, rotationZ, rotationY
end