wordParts = {}

wordParts.default = {}
wordParts.default.startTime = 0.0416
wordParts.default.endTime = 4.1666
wordParts.default.rate = 1.0

wordParts.victory = {}
wordParts.victory.startTime = 0.0416
wordParts.victory.endTime = 1.5
wordParts.victory.rate = 1.0

wordParts.victoryIdle = {}
wordParts.victoryIdle.startTime = 1.5
wordParts.victoryIdle.endTime = 6.0
wordParts.victoryIdle.rate = 1.0

wordParts.gameOver = {}
wordParts.gameOver.startTime = 0.0416
wordParts.gameOver.endTime = 2.625
wordParts.gameOver.rate = 1.0

wordParts.gameOverIdle = {}
wordParts.gameOverIdle.startTime = 3.9
wordParts.gameOverIdle.endTime = 4.0
wordParts.gameOverIdle.rate = 1.0

wordParts.player = {}
wordParts.player.startTime = 0.0416
wordParts.player.endTime = 1.5
wordParts.player.rate = 1.0

wordParts.playerIdle = {}
wordParts.playerIdle.startTime = 1.5
wordParts.playerIdle.endTime = 1.75
wordParts.playerIdle.rate = 1.0

function GetWordsVar(animation, data)
	return wordParts[animation][data]
end