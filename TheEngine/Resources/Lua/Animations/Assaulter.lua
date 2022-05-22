assaulter = {}

assaulter.default = {}
assaulter.default.startTime = 0.0
assaulter.default.endTime = 4.83
assaulter.default.rate = 0.5

assaulter.idle = {}
assaulter.idle.startTime = 5.01
assaulter.idle.endTime = 5.41
assaulter.idle.rate = 1.0

assaulter.walk = {}
assaulter.walk.startTime = 0.0833
assaulter.walk.endTime = 0.875
assaulter.walk.rate = 1.0

assaulter.die = {}
assaulter.die.startTime = 0.916
assaulter.die.endTime = 1.25
assaulter.die.rate = 0.5

assaulter.jump = {}
assaulter.jump.startTime = 1.2916
assaulter.jump.endTime = 2.1666
assaulter.jump.rate = 2.0

assaulter.aim = {}
assaulter.aim.startTime = 2.2083
assaulter.aim.endTime = 2.625
assaulter.aim.rate = 1.0

assaulter.shoot = {}
assaulter.shoot.startTime = 2.6666
assaulter.shoot.endTime = 3.1666
assaulter.shoot.rate = 1.0

assaulter.unaim = {}
assaulter.unaim.startTime = 3.2083
assaulter.unaim.endTime = 4.0416
assaulter.unaim.rate = 1.0

assaulter.shootIdle = {}
assaulter.shootIdle.startTime = 4.1
assaulter.shootIdle.endTime = 4.5
assaulter.shootIdle.rate = 1.0

assaulter.jetpack = {}
assaulter.jetpack.startTime = 4.5416
assaulter.jetpack.endTime = 5.1666
assaulter.jetpack.rate = 1.0

assaulter.jetpackLand = {}
assaulter.jetpackLand.startTime = 5.2083
assaulter.jetpackLand.endTime = 6.1999
assaulter.jetpackLand.rate = 1.0

function GetAssaulterVar(animation, data)
	return assaulter[animation][data]
end