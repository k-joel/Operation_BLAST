baron = {}

--basic------------------------------------

baron.default = {}
baron.default.startTime = 0.0
baron.default.endTime = 24.6
baron.default.rate = 0.5

baron.idle = {}
baron.idle.startTime = 0.1
baron.idle.endTime = 0.3333
baron.idle.rate = 0.25

baron.shootForward = {}
baron.shootForward.startTime = 0.1
baron.shootForward.endTime = 0.3333
baron.shootForward.rate = 1.0

baron.run = {}
baron.run.startTime = 0.4583
baron.run.endTime = 1.0
baron.run.rate = 1.0

baron.walk = {}
baron.walk.startTime = 0.4583
baron.walk.endTime = 1.0
baron.walk.rate = 1.0

--standing---------------------------------

baron.aimUp = {}
baron.aimUp.startTime = 1.0416
baron.aimUp.endTime = 1.0833
baron.aimUp.rate = 0.1

baron.aimDiagonalUp = {}
baron.aimDiagonalUp.startTime = 1.125
baron.aimDiagonalUp.endTime = 1.1666
baron.aimDiagonalUp.rate = 0.1

baron.aimForward = {}
baron.aimForward.startTime = 1.2083
baron.aimForward.endTime = 1.25
baron.aimForward.rate = 0.1

baron.aimDiagonalDown = {}
baron.aimDiagonalDown.startTime = 1.2916
baron.aimDiagonalDown.endTime = 1.3333
baron.aimDiagonalDown.rate = 0.1

baron.aimDown = {}
baron.aimDown.startTime = 1.375
baron.aimDown.endTime = 1.4166
baron.aimDown.rate = 0.1

baron.shootUp = {}
baron.shootUp.startTime = 1.4583
baron.shootUp.endTime = 1.7916
baron.shootUp.rate = 1.0

baron.shootDiagonalUp = {}
baron.shootDiagonalUp.startTime = 1.8333
baron.shootDiagonalUp.endTime = 2.1666
baron.shootDiagonalUp.rate = 1.0

baron.shootDiagonalDown = {}
baron.shootDiagonalDown.startTime = 2.2083
baron.shootDiagonalDown.endTime = 2.5416
baron.shootDiagonalDown.rate = 1.0

baron.shootDown = {}
baron.shootDown.startTime = 2.5833
baron.shootDown.endTime = 2.7
baron.shootDown.rate = 1.0

baron.prone = {}
baron.prone.startTime = 3.0
baron.prone.endTime = 3.0833
baron.prone.rate = 01.0

baron.shootProne = {}
baron.shootProne.startTime = 3.0833
baron.shootProne.endTime = 3.4166
baron.shootProne.rate = 1.0

--jumps & falls-------------------------------

baron.jumpAimUp = {}
baron.jumpAimUp.startTime = 3.4583
baron.jumpAimUp.endTime = 3.9583
baron.jumpAimUp.rate = 0.5

baron.fallAimUp = {}
baron.fallAimUp.startTime = 3.9583
baron.fallAimUp.endTime = 4.3333
baron.fallAimUp.rate = 0.5

baron.jumpAimDiagonalUp = {}
baron.jumpAimDiagonalUp.startTime = 4.375
baron.jumpAimDiagonalUp.endTime = 4.875
baron.jumpAimDiagonalUp.rate = 0.5

baron.fallAimDiagonalUp = {}
baron.fallAimDiagonalUp.startTime = 4.875
baron.fallAimDiagonalUp.endTime = 5.25
baron.fallAimDiagonalUp.rate = 0.5

baron.jumpAimForward = {}
baron.jumpAimForward.startTime = 5.2916
baron.jumpAimForward.endTime = 5.7916
baron.jumpAimForward.rate = 0.5

baron.fallAimForward = {}
baron.fallAimForward.startTime = 5.7916
baron.fallAimForward.endTime = 6.1666
baron.fallAimForward.rate = 0.5

baron.jumpAimDiagonalDown = {}
baron.jumpAimDiagonalDown.startTime = 6.2083
baron.jumpAimDiagonalDown.endTime = 6.7083
baron.jumpAimDiagonalDown.rate = 0.5

baron.fallAimDiagonalDown = {}
baron.fallAimDiagonalDown.startTime = 6.7083
baron.fallAimDiagonalDown.endTime = 7.08333
baron.fallAimDiagonalDown.rate = 0.5

baron.jumpAimDown = {}
baron.jumpAimDown.startTime = 7.125
baron.jumpAimDown.endTime = 7.625
baron.jumpAimDown.rate = 0.5

baron.fallAimDown = {}
baron.fallAimDown.startTime = 7.625
baron.fallAimDown.endTime = 8
baron.fallAimDown.rate = 0.5

baron.jumpShootUp = {}
baron.jumpShootUp.startTime = 8.0416
baron.jumpShootUp.endTime = 8.5416
baron.jumpShootUp.rate = 0.5

baron.fallShootUp = {}
baron.fallShootUp.startTime = 8.5416
baron.fallShootUp.endTime = 8.9166
baron.fallShootUp.rate = 0.5

baron.jumpShootDiagonalUp = {}
baron.jumpShootDiagonalUp.startTime = 8.9583
baron.jumpShootDiagonalUp.endTime = 9.4583
baron.jumpShootDiagonalUp.rate = 0.5

baron.fallShootDiagonalUp = {}
baron.fallShootDiagonalUp.startTime = 9.4583
baron.fallShootDiagonalUp.endTime = 9.8333
baron.fallShootDiagonalUp.rate = 0.5

baron.jumpShootForward = {}
baron.jumpShootForward.startTime = 9.875
baron.jumpShootForward.endTime = 10.375
baron.jumpShootForward.rate = 0.5

baron.fallShootForward = {}
baron.fallShootForward.startTime = 10.375
baron.fallShootForward.endTime = 10.75
baron.fallShootForward.rate = 0.5

baron.jumpShootDiagonalDown = {}
baron.jumpShootDiagonalDown.startTime = 10.7916
baron.jumpShootDiagonalDown.endTime = 11.2916
baron.jumpShootDiagonalDown.rate = 0.5

baron.fallShootDiagonalDown = {}
baron.fallShootDiagonalDown.startTime = 11.2916
baron.fallShootDiagonalDown.endTime = 11.6666
baron.fallShootDiagonalDown.rate = 0.5

baron.jumpShootDown = {}
baron.jumpShootDown.startTime = 11.7083
baron.jumpShootDown.endTime = 12.2083
baron.jumpShootDown.rate = 0.5

baron.fallShootDown = {}
baron.fallShootDown.startTime = 12.2083
baron.fallShootDown.endTime = 12.5833
baron.fallShootDown.rate = 0.5

--special-----------------------------------------

baron.damage = {}
baron.damage.startTime = 12.625
baron.damage.endTime = 13.125
baron.damage.rate = 1.0

baron.victory = {}
baron.victory.startTime = 13.1666
baron.victory.endTime = 15.6666
baron.victory.rate = 1.0

baron.victoryIdle = {}
baron.victoryIdle.startTime = 15.6666
baron.victoryIdle.endTime = 15.6667
baron.victoryIdle.rate = 1.0

baron.lose = {}
baron.lose.startTime = 15.7083
baron.lose.endTime = 16.9583
baron.lose.rate = 1.0

baron.loseIdle = {}
baron.loseIdle.startTime = 16.9583
baron.loseIdle.endTime = 16.9584
baron.loseIdle.rate = 1.0

baron.death = {}
baron.death.startTime = 17
baron.death.endTime = 18.25
baron.death.rate = 1.0

baron.special = {}
baron.special.startTime = 18.2916
baron.special.endTime = 19.5416
baron.special.rate = 1.0

--runing-------------------------------------

baron.runAimUp = {}
baron.runAimUp.startTime = 19.5833
baron.runAimUp.endTime = 20.125
baron.runAimUp.rate = 1.0

baron.runAimDiagonalUp = {}
baron.runAimDiagonalUp.startTime = 20.1666
baron.runAimDiagonalUp.endTime = 20.7083
baron.runAimDiagonalUp.rate = 1.0

baron.runAimForward = {}
baron.runAimForward.startTime = 20.8
baron.runAimForward.endTime = 21.2916
baron.runAimForward.rate = 1.0

baron.runAimDiagonalDown = {}
baron.runAimDiagonalDown.startTime = 21.3333
baron.runAimDiagonalDown.endTime = 21.875
baron.runAimDiagonalDown.rate = 1.0

baron.runAimDown = {}
baron.runAimDown.startTime = 21.9166
baron.runAimDown.endTime = 22.4583
baron.runAimDown.rate = 1.0

baron.runShootUp = {}
baron.runShootUp.startTime = 22.5
baron.runShootUp.endTime = 23.0416
baron.runShootUp.rate = 1.0

baron.runShootDiagonalUp = {}
baron.runShootDiagonalUp.startTime = 23.2916
baron.runShootDiagonalUp.endTime = 23.625
baron.runShootDiagonalUp.rate = 1.0

baron.runShootForward = {}
baron.runShootForward.startTime = 0.4583
baron.runShootForward.endTime = 1.0
baron.runShootForward.rate = 1.0

baron.runShootDiagonalDown = {}
baron.runShootDiagonalDown.startTime = 23.6666
baron.runShootDiagonalDown.endTime = 24.2083
baron.runShootDiagonalDown.rate = 1.0

baron.runShootDown = {}
baron.runShootDown.startTime = 24.25
baron.runShootDown.endTime = 24.6
baron.runShootDown.rate = 1.0

--dash----------------------------------------------------------------

baron.dashForward = {}
baron.dashForward.startTime = 25.416
baron.dashForward.endTime = 26.0
baron.dashForward.rate = 1.0

baron.dashBackward = {}
baron.dashBackward.startTime = 26.0416
baron.dashBackward.endTime = 26.583
baron.dashBackward.rate = 1.0

--down jump/burrow----------------------------------------------------

baron.burrow = {}
baron.burrow.startTime = 26.583
baron.burrow.endTime = 27.0
baron.burrow.rate = 1.0

function GetBaronVar(animation, data)
	return baron[animation][data]
end