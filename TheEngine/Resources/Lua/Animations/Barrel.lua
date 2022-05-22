barrel = {}

barrel.default = {}
barrel.default.startTime = 0.0416
barrel.default.endTime = 0.43
barrel.default.rate = 1.0

-------barrelExplosion----------------------

barrel.explode = {}
barrel.explode.startTime = 0.0416
barrel.explode.endTime = 0.43
barrel.explode.rate = 1.0

function GetBarrelVar(animation, data)
	return barrel[animation][data]
end