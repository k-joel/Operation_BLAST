claire= {}

--basic------------------------------------

claire.default = {}
claire.default.startTime = 0.0
claire.default.endTime = 24.6
claire.default.rate = 0.5

claire.idle = {}
claire.idle.startTime = 0.125
claire.idle.endTime = 0.3333
claire.idle.rate = 0.25

claire.shootForward = {}
claire.shootForward.startTime = 0.0667
claire.shootForward.endTime = 0.3333
claire.shootForward.rate = 1.0

claire.run = {}
claire.run.startTime = 0.4583
claire.run.endTime = 1.0
claire.run.rate = 1.0

claire.walk = {}
claire.walk.startTime = 0.4583
claire.walk.endTime = 1.0
claire.walk.rate = 1.0

--standing---------------------------------

claire.aimUp = {}
claire.aimUp.startTime = 1.0416
claire.aimUp.endTime = 1.0833
claire.aimUp.rate = 0.1

claire.aimDiagonalUp = {}
claire.aimDiagonalUp.startTime = 1.125
claire.aimDiagonalUp.endTime = 1.1666
claire.aimDiagonalUp.rate = 0.1

claire.aimForward = {}
claire.aimForward.startTime = 1.2083
claire.aimForward.endTime = 1.25
claire.aimForward.rate = 0.1

claire.aimDiagonalDown = {}
claire.aimDiagonalDown.startTime = 1.2916
claire.aimDiagonalDown.endTime = 1.3333
claire.aimDiagonalDown.rate = 0.1

claire.aimDown = {}
claire.aimDown.startTime = 1.375
claire.aimDown.endTime = 1.4166
claire.aimDown.rate = 0.1

claire.shootUp = {}
claire.shootUp.startTime = 1.4583
claire.shootUp.endTime = 1.7916
claire.shootUp.rate = 1.0

claire.shootDiagonalUp = {}
claire.shootDiagonalUp.startTime = 1.8333
claire.shootDiagonalUp.endTime = 2.1666
claire.shootDiagonalUp.rate = 1.0

claire.shootDiagonalDown = {}
claire.shootDiagonalDown.startTime = 2.2083
claire.shootDiagonalDown.endTime = 2.5416
claire.shootDiagonalDown.rate = 1.0

claire.shootDown = {}
claire.shootDown.startTime = 2.5833
claire.shootDown.endTime = 2.9166
claire.shootDown.rate = 1.0

claire.prone = {}
claire.prone.startTime = 3.0833
claire.prone.endTime = 3.4
claire.prone.rate = 0.1

claire.shootProne = {}
claire.shootProne.startTime = 3.0833
claire.shootProne.endTime = 3.4166
claire.shootProne.rate = 1.0

--jumps & falls-------------------------------

claire.jumpAimUp = {}
claire.jumpAimUp.startTime = 3.4583
claire.jumpAimUp.endTime = 3.9583
claire.jumpAimUp.rate = 0.5

claire.fallAimUp = {}
claire.fallAimUp.startTime = 3.9583
claire.fallAimUp.endTime = 4.3333
claire.fallAimUp.rate = 0.5

claire.jumpAimDiagonalUp = {}
claire.jumpAimDiagonalUp.startTime = 4.375
claire.jumpAimDiagonalUp.endTime = 4.875
claire.jumpAimDiagonalUp.rate = 0.5

claire.fallAimDiagonalUp = {}
claire.fallAimDiagonalUp.startTime = 4.875
claire.fallAimDiagonalUp.endTime = 5.25
claire.fallAimDiagonalUp.rate = 0.5

claire.jumpAimForward = {}
claire.jumpAimForward.startTime = 5.2916
claire.jumpAimForward.endTime = 5.7916
claire.jumpAimForward.rate = 0.5

claire.fallAimForward = {}
claire.fallAimForward.startTime = 5.7916
claire.fallAimForward.endTime = 6.1666
claire.fallAimForward.rate = 0.5

claire.jumpAimDiagonalDown = {}
claire.jumpAimDiagonalDown.startTime = 6.2083
claire.jumpAimDiagonalDown.endTime = 6.7083
claire.jumpAimDiagonalDown.rate = 0.5

claire.fallAimDiagonalDown = {}
claire.fallAimDiagonalDown.startTime = 6.7083
claire.fallAimDiagonalDown.endTime = 7.08333
claire.fallAimDiagonalDown.rate = 0.5

claire.jumpAimDown = {}
claire.jumpAimDown.startTime = 7.125
claire.jumpAimDown.endTime = 7.625
claire.jumpAimDown.rate = 0.5

claire.fallAimDown = {}
claire.fallAimDown.startTime = 7.625
claire.fallAimDown.endTime = 8
claire.fallAimDown.rate = 0.5

claire.jumpShootUp = {}
claire.jumpShootUp.startTime = 8.0416
claire.jumpShootUp.endTime = 8.5416
claire.jumpShootUp.rate = 0.5

claire.fallShootUp = {}
claire.fallShootUp.startTime = 8.5416
claire.fallShootUp.endTime = 8.9166
claire.fallShootUp.rate = 0.5

claire.jumpShootDiagonalUp = {}
claire.jumpShootDiagonalUp.startTime = 8.9583
claire.jumpShootDiagonalUp.endTime = 9.4583
claire.jumpShootDiagonalUp.rate = 0.5

claire.fallShootDiagonalUp = {}
claire.fallShootDiagonalUp.startTime = 9.4583
claire.fallShootDiagonalUp.endTime = 9.8333
claire.fallShootDiagonalUp.rate = 0.5

claire.jumpShootForward = {}
claire.jumpShootForward.startTime = 9.875
claire.jumpShootForward.endTime = 10.375
claire.jumpShootForward.rate = 0.5

claire.fallShootForward = {}
claire.fallShootForward.startTime = 10.375
claire.fallShootForward.endTime = 10.75
claire.fallShootForward.rate = 0.5

claire.jumpShootDiagonalDown = {}
claire.jumpShootDiagonalDown.startTime = 10.7916
claire.jumpShootDiagonalDown.endTime = 11.2916
claire.jumpShootDiagonalDown.rate = 0.5

claire.fallShootDiagonalDown = {}
claire.fallShootDiagonalDown.startTime = 11.2916
claire.fallShootDiagonalDown.endTime = 11.6666
claire.fallShootDiagonalDown.rate = 0.5

claire.jumpShootDown = {}
claire.jumpShootDown.startTime = 11.7083
claire.jumpShootDown.endTime = 12.2083
claire.jumpShootDown.rate = 0.5

claire.fallShootDown = {}
claire.fallShootDown.startTime = 12.2083
claire.fallShootDown.endTime = 12.5833
claire.fallShootDown.rate = 0.5

--special-----------------------------------------

claire.damage = {}
claire.damage.startTime = 12.625
claire.damage.endTime = 13.125
claire.damage.rate = 1.0

claire.victory = {}
claire.victory.startTime = 13.1666
claire.victory.endTime = 15.6666
claire.victory.rate = 1.0

claire.victoryIdle = {}
claire.victoryIdle.startTime = 15.6666
claire.victoryIdle.endTime = 15.6667
claire.victoryIdle.rate = 1.0

claire.lose = {}
claire.lose.startTime = 15.7083
claire.lose.endTime = 16.9583
claire.lose.rate = 1.0

claire.loseIdle = {}
claire.loseIdle.startTime = 16.9583
claire.loseIdle.endTime = 16.9584
claire.loseIdle.rate = 1.0

claire.death = {}
claire.death.startTime = 17
claire.death.endTime = 18.25
claire.death.rate = 1.0

claire.special = {}
claire.special.startTime = 18.2916
claire.special.endTime = 19.5416
claire.special.rate = 1.0

--runing-------------------------------------

claire.runAimUp = {}
claire.runAimUp.startTime = 19.5833
claire.runAimUp.endTime = 20.125
claire.runAimUp.rate = 1.0

claire.runAimDiagonalUp = {}
claire.runAimDiagonalUp.startTime = 20.1666
claire.runAimDiagonalUp.endTime = 20.7083
claire.runAimDiagonalUp.rate = 1.0

claire.runAimForward = {}
claire.runAimForward.startTime = 20.79
claire.runAimForward.endTime = 21.2916
claire.runAimForward.rate = 1.0

claire.runAimDiagonalDown = {}
claire.runAimDiagonalDown.startTime = 21.3333
claire.runAimDiagonalDown.endTime = 21.875
claire.runAimDiagonalDown.rate = 1.0

claire.runAimDown = {}
claire.runAimDown.startTime = 21.9166
claire.runAimDown.endTime = 22.4583
claire.runAimDown.rate = 1.0

claire.runShootUp = {}
claire.runShootUp.startTime = 22.5
claire.runShootUp.endTime = 23.0416
claire.runShootUp.rate = 1.0

claire.runShootDiagonalUp = {}
claire.runShootDiagonalUp.startTime = 23.2916
claire.runShootDiagonalUp.endTime = 23.625
claire.runShootDiagonalUp.rate = 1.0

claire.runShootForward = {}
claire.runShootForward.startTime = 0.4583
claire.runShootForward.endTime = 1.0
claire.runShootForward.rate = 0.6

claire.runShootDiagonalDown = {}
claire.runShootDiagonalDown.startTime = 23.6666
claire.runShootDiagonalDown.endTime = 24.2083
claire.runShootDiagonalDown.rate = 1.0

claire.runShootDown = {}
claire.runShootDown.startTime = 24.25
claire.runShootDown.endTime = 24.6
claire.runShootDown.rate = 1.0

--dash----------------------------------------------------------------

claire.dashForward = {}
claire.dashForward.startTime = 25.416
claire.dashForward.endTime = 26.0
claire.dashForward.rate = 1.0

claire.dashBackward = {}
claire.dashBackward.startTime = 26.0416
claire.dashBackward.endTime = 26.583
claire.dashBackward.rate = 1.0

--down jump/burrow----------------------------------------------------

claire.burrow = {}
claire.burrow.startTime = 26.583
claire.burrow.endTime = 27.0
claire.burrow.rate = 1.0

function GetClaireVar(animation, data)
	return claire[animation][data]
end