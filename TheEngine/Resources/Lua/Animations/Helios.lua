helios = {}

helios.default = {}
helios.default.startTime = 0.0833
helios.default.endTime = 1.0
helios.default.rate = 0.75

helios.idle = {}--2-24
helios.idle.startTime = 0.0833
helios.idle.endTime = 1.0
helios.idle.rate = 1.0

helios.die = {}
helios.die.startTime = 0.0833
helios.die.endTime = 6.9
helios.die.rate = 25.0

--move---------------------------------
helios.startMove = {}--25-40
helios.startMove.startTime = 1.0416
helios.startMove.endTime = 1.6666
helios.startMove.rate = 1.0

helios.move = {}--41-51
helios.move.startTime = 1.7083
helios.move.endTime = 2.125
helios.move.rate = 1.0

helios.stopMove = {}--52-67
helios.stopMove.startTime = 2.1666
helios.stopMove.endTime = 2.7916
helios.stopMove.rate = 1.0

--left----------------------------------
helios.turnLeft = {}--68-80
helios.turnLeft.startTime = 2.8333
helios.turnLeft.endTime = 3.3333
helios.turnLeft.rate = 1.0

helios.leftReadyShoot = {}--81-91
helios.leftReadyShoot.startTime = 3.375
helios.leftReadyShoot.endTime = 3.7916
helios.leftReadyShoot.rate = 1.0

helios.leftShoot = {}--92-102
helios.leftShoot.startTime = 3.8333
helios.leftShoot.endTime = 4.25
helios.leftShoot.rate = 1.0

helios.unturnLeft = {}--103-115
helios.unturnLeft.startTime = 4.2916
helios.unturnLeft.endTime = 4.7916
helios.unturnLeft.rate = 1.0

--right---------------------------------
helios.turnRight = {}--116-133
helios.turnRight.startTime = 4.8333
helios.turnRight.endTime = 5.5416
helios.turnRight.rate = 1.0

helios.rightReadyShoot = {}--134-144
helios.rightReadyShoot.startTime = 5.5416
helios.rightReadyShoot.endTime = 6.0
helios.rightReadyShoot.rate = 1.0

helios.rightShoot = {}--145-155
helios.rightShoot.startTime = 6.0416
helios.rightShoot.endTime = 6.4583
helios.rightShoot.rate = 1.0

helios.unturnRight = {}--156-168
helios.unturnRight.startTime = 6.5
helios.unturnRight.endTime = 6.93
helios.unturnRight.rate = 1.0

function GetHeliosVar(animation, data)
	return helios[animation][data]
end