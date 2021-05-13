# Arduino_Mastermind-Game
I created this little game as a hobby for my children during the Covid-19 quarantine. I had already seen some versions for two players on the Internet. But I wanted a version in which the player would challenge Arduino itself, because then I could turn the game into a puzzle for Escape Room.


The game works like this: there are seven colors available and a code with four colors, without any of them being repeated. The player's goal is to find out what is the sequence of four colors in a maximum of sixteen attempts. When the player hits one of the colors, but misses the position, the computer lights up a red LED. When the color and position are correct, the computer lights up a green led. With these tips and some strategy, the player must discover the password.

In my research, I found some versions of this game, but I decided to create one using the fewest possible Arduino pins, in order to create other puzzles on the same board.
