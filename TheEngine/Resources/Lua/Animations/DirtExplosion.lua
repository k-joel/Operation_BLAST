dirt = {}

dirt.default = {}
dirt.default.startTime = 0.0416
dirt.default.endTime = 0.0832
dirt.default.rate = 1.0

--------dirtExplosion------------------

dirt.explosion = {}
dirt.explosion.startTime = 0.0416
dirt.explosion.endTime = 0.5
dirt.explosion.rate = 1.0

dirt.explosionA = {}
dirt.explosionA.startTime = 0.0416
dirt.explosionA.endTime = 0.5
dirt.explosionA.rate = 1.0

dirt.explosionB = {}
dirt.explosionB.startTime = 0.0416
dirt.explosionB.endTime = 0.5
dirt.explosionB.rate = 1.0

function GetDirtVar(animation, data)
	return dirt[animation][data]
end