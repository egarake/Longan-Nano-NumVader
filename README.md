Longan-Nano-NumVader

Longan Nano Number Invader Game

[Farmwear]

numvader.zip

    numvader064KB.bin

    numvader128KB.bin

[Sauce]

main.c

sipeed/Longan_GD32VF_examples/gd32v_lcd/src/

Replace main.c and build

[Hardware]

VR Left GND A0 3.3V

(VR Right GND A1 3.3V)

SW BOOT0 A8 3.3V

[How to play]

Game screen
+-------------------+
|      Score      0 |
| @:_________6391n0 |
|      Fail  -    0 |
+-------------------+

"@(0-9,n)" Cannon, ":" Final defense line, "0-9" Invaders +1, "n" UFO +10

Invaders attack from the right edge of the screen.

Defeat the invaders with the left cannon.

Match the numbers of the cannon with the left volume and shoot with the BOOT0SW.

If the total of the two numbers deleted is 10, the UFO will be displayed.

In addition to the score, 1 point for intruders and 10 points for UFOs are added.

If the attack fails, the failed score will be added.

Invaders will gradually get faster.

You lose when the invaders reach the final defense line.

(Like a CASIO Invader Game Calculator Dentaku LCD Game Watch SL-880-N F/S)
