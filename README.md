https://www.youtube.com/watch?v=A-lh1-bTzTw&pp=ygUid2hhdCBpcyB0aGUgc2ltcGxlc3QgZ2FtZSBwb3NzaWJsZQ%3D%3D
# The Simplest Possible Game

This project is licensed because I wrote it.
As far as I am aware, nothing concerning me has been issued to prevent the writing of this code.
This project is based on the game I at least have been made known to by @webgoatguy on YouTube™.
The person who published the video detailing the game is the same person I am aware of be the creator of it.
I have not heard a name for the game.

To play, two players take turns giving their input to eventually reach an outcome that can result in a win to either player or draw.
You have two options to pick between per turn, an X or an O. Once you make your decision, the input is tiled further on a line (geometric ray) and the turn is flipped over.
Each time you give over your turn, the tile you have placed can no longer be picked back up.
When the game ends, all settings--including who goes first--and the line are reset.

In order for the player who has their first turn before the other to win, they would need to get two identical patterns of length five on the line.
These two patterns would need to be the indifferent in order.
For the player who has their first turn after the other to win, they would need four patterns on the line in the same way.

If you need direction because you do not know how to run/execute the code (to play ofcourse), search for GNU, Clang, or Visual Studio.
You might have luck running this alongside the correct dependencies "mkdir cmake && cmake -B cmake && cmake --build cmake --target witspg -j 10".