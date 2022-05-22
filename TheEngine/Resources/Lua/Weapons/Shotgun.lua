Shotgun = {}

--Firing rates
Shotgun.NormalFiringRate = 0.3
Shotgun.AlternateFiringRate = 0.8

--Firing speeds
Shotgun.NormalFiringSpeed = 250.0
Shotgun.AlternateFiringSpeed = 225.0
Shotgun.UltimateFiringSpeed = 225.0

--Bullet life
Shotgun.NormalBulletLife = 20.0
Shotgun.AlternateBulletLife = 20.0

--Ammo count
Shotgun.MaxAmmo = 50

--Alternate cost
Shotgun.AltCost = 10

--Damage
Shotgun.NormalDamage = 1.0
Shotgun.AltDamage = 15.0
Shotgun.UltDamage = 25.0

function GetShotgunData(data)
	return Shotgun[data]
end

function GetBulletInfo(dir, isLeft, type)
	--[[The directions are associated with the following values:
	
	up							-	0
	angled up					-	1
	forward						-	2
	angled down					-	3
	down						-	4
	forward slight-up			-	5
	forward slight-down			-	6
	up slight-back				-	7
	up slight-forward			-	8
	down slight-back			-	9
	down slight-forward			-	10
	up-middle slight-up			-	11
	up-middle slight-down		-	12
	down-middle slight-up		-	13
	down-middle slight-down		-	14
	
	The type is associated with the following values:
	
	normal						-	0
	alternate					-	1
	ultimate					-	2
	
	--]]
	
	if type == 0 then
		if isLeft == true then
			midRotationY = math.rad(270)
			leftRotationY = math.rad(270)
			rightRotationY = math.rad(270)
		
			if dir == 0 then
				midRotationZ = math.rad(90)
				midX = math.cos(midRotationZ)
				midY = math.sin(midRotationZ)
				
				leftRotationZ = math.rad(80)
				leftX = math.cos(leftRotationZ)
				leftY = math.sin(leftRotationZ)
				
				rightRotationZ = math.rad(100)
				rightX = math.cos(rightRotationZ)
				rightY = math.sin(rightRotationZ)
			elseif dir == 1 then
				midRotationZ = math.rad(135)
				midX = math.cos(midRotationZ)
				midY = math.sin(midRotationZ)
				
				leftRotationZ = math.rad(145)
				leftX = math.cos(leftRotationZ)
				leftY = math.sin(leftRotationZ)
				
				rightRotationZ = math.rad(125)
				rightX = math.cos(rightRotationZ)
				rightY = math.sin(rightRotationZ)
			elseif dir == 2 then
				midRotationZ = math.rad(180)
				midX = math.cos(midRotationZ)
				midY = math.sin(midRotationZ)
				
				rightRotationZ = math.rad(170)
				rightX = math.cos(rightRotationZ)
				rightY = math.sin(rightRotationZ)
				
				leftRotationZ = math.rad(190)
				leftX = math.cos(leftRotationZ)
				leftY = math.sin(leftRotationZ)
			elseif dir == 3 then
				midRotationZ = math.rad(225)
				midX = math.cos(midRotationZ)
				midY = math.sin(midRotationZ)
				
				leftRotationZ = math.rad(215)
				leftX = math.cos(leftRotationZ)
				leftY = math.sin(leftRotationZ)
				
				rightRotationZ = math.rad(230)
				rightX = math.cos(rightRotationZ)
				rightY = math.sin(rightRotationZ)
			else
				midRotationZ = math.rad(270)
				midX = math.cos(midRotationZ)
				midY = math.sin(midRotationZ)
				
				leftRotationZ = math.rad(260)
				leftX = math.cos(leftRotationZ)
				leftY = math.sin(leftRotationZ)
				
				rightRotationZ = math.rad(280)
				rightX = math.cos(rightRotationZ)
				rightY = math.sin(rightRotationZ)
			end
		else
		
			midRotationY = math.rad(90)
			leftRotationY = math.rad(90)
			rightRotationY = math.rad(90)
		
			if dir == 0 then
				midRotationZ = math.rad(90)
				midX = math.cos(midRotationZ)
				midY = math.sin(midRotationZ)
				
				leftRotationZ = math.rad(100)
				leftX = math.cos(leftRotationZ)
				leftY = math.sin(leftRotationZ)
				
				rightRotationZ = math.rad(80)
				rightX = math.cos(rightRotationZ)
				rightY = math.sin(rightRotationZ)
			elseif dir == 1 then
				midRotationZ = math.rad(45)
				midX = math.cos(midRotationZ)
				midY = math.sin(midRotationZ)
				
				leftRotationZ = math.rad(55)
				leftX = math.cos(leftRotationZ)
				leftY = math.sin(leftRotationZ)
				
				rightRotationZ = math.rad(35)
				rightX = math.cos(rightRotationZ)
				rightY = math.sin(rightRotationZ)
			elseif dir == 2 then
				midRotationZ = math.rad(0)
				midX = math.cos(midRotationZ)
				midY = math.sin(midRotationZ)
				
				leftRotationZ = math.rad(10)
				leftX = math.cos(leftRotationZ)
				leftY = math.sin(leftRotationZ)
				
				rightRotationZ = math.rad(350)
				rightX = math.cos(rightRotationZ)
				rightY = math.sin(rightRotationZ)
			elseif dir == 3 then
				midRotationZ = math.rad(315)
				midX = math.cos(midRotationZ)
				midY = math.sin(midRotationZ)
				
				leftRotationZ = math.rad(305)
				leftX = math.cos(leftRotationZ)
				leftY = math.sin(leftRotationZ)
				
				rightRotationZ = math.rad(325)
				rightX = math.cos(rightRotationZ)
				rightY = math.sin(rightRotationZ)
			else
				midRotationZ = math.rad(270)
				midX = math.cos(midRotationZ)
				midY = math.sin(midRotationZ)
				
				leftRotationZ = math.rad(260)
				leftX = math.cos(leftRotationZ)
				leftY = math.sin(leftRotationZ)
				
				rightRotationZ = math.rad(280)
				rightX = math.cos(rightRotationZ)
				rightY = math.sin(rightRotationZ)
			end
		end
		
		midX = midX * Shotgun["NormalFiringSpeed"]
		midY = midY * Shotgun["NormalFiringSpeed"]
		
		rightX = rightX * Shotgun["NormalFiringSpeed"]
		rightY = rightY * Shotgun["NormalFiringSpeed"]
		
		leftX = leftX * Shotgun["NormalFiringSpeed"]
		leftY = leftY * Shotgun["NormalFiringSpeed"]
		
		return Shotgun["NormalBulletLife"], Shotgun["NormalDamage"], midX, midY, midRotationY, midRotationZ, leftX, leftY, leftRotationY, leftRotationZ, rightX, rightY, rightRotationY, rightRotationZ
	
	elseif type == 1 then
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
			x = -x * Shotgun["AlternateFiringSpeed"]
			y = -y * Shotgun["AlternateFiringSpeed"]
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
			x = x * Shotgun["AlternateFiringSpeed"]
			y = y * Shotgun["AlternateFiringSpeed"]
		end
		
		
		return Shotgun["AlternateBulletLife"], Shotgun["AltDamage"], x, y, rotationY, rotationZ
	else
		--[[
		The bullets for the ultimate start with the bottom left and travel counter-clockwise.  
		So bottom left, left, up left, up, up right, right, bottom right.
		--]]
		
		blRotY = math.rad(270)
		blRotZ = math.rad(225)
		blX = math.cos(blRotZ) * Shotgun["UltimateFiringSpeed"]
		blY = math.sin(blRotZ) * Shotgun["UltimateFiringSpeed"]
		
		lRotY = math.rad(270)
		lRotZ = math.rad(180)
		lX = math.cos(lRotZ) * Shotgun["UltimateFiringSpeed"]
		lY = math.sin(lRotZ) * Shotgun["UltimateFiringSpeed"]
		
		ulRotY = math.rad(270)
		ulRotZ = math.rad(135)
		ulX = math.cos(ulRotZ) * Shotgun["UltimateFiringSpeed"]
		ulY = math.sin(ulRotZ) * Shotgun["UltimateFiringSpeed"]
		
		uRotY = math.rad(270)
		uRotZ = math.rad(90)
		uX = math.cos(uRotZ) * Shotgun["UltimateFiringSpeed"]
		uY = math.sin(uRotZ) * Shotgun["UltimateFiringSpeed"]
		
		urRotY = math.rad(90)
		urRotZ = math.rad(45)
		urX = math.cos(urRotZ) * Shotgun["UltimateFiringSpeed"]
		urY = math.sin(urRotZ) * Shotgun["UltimateFiringSpeed"]
		
		rRotY = math.rad(90)
		rRotZ = math.rad(0)
		rX = math.cos(rRotZ) * Shotgun["UltimateFiringSpeed"]
		rY = math.sin(rRotZ) * Shotgun["UltimateFiringSpeed"]
		
		brRotY = math.rad(90)
		brRotZ = math.rad(315)
		brX = math.cos(brRotZ) * Shotgun["UltimateFiringSpeed"]
		brY = math.sin(brRotZ) * Shotgun["UltimateFiringSpeed"]
		
		return Shotgun["UltDamage"], blX, blY, blRotY, blRotZ,  lX, lY, lRotY, lRotZ,  ulX, ulY, ulRotY, ulRotZ,  uX, uY, uRotY, uRotZ,  urX, urY, urRotY, urRotZ,  rX, rY, rRotY, rRotZ,  brX, brY, brRotY, brRotZ
	end
end