## Programming Chess from scratch for the original Gameboy

![example_pic](images/example_pic.png)

### done:
- rom compiles -> [last compiled rom](/compiled_roms) (not tested on original hardware! Use at own risk!)
- game playable -> regular moves working
- all special rules/moves implemented:
    first Pawn move, Castling, Promotion, En passaint, Draw and Checkmate, moves leaving King under attack not possible
- replay mode
- chess clock
- end game screen
- menu to adjust chess clock
- at start of turn, move cursor to last moved piece
- title screen

### to do:
- add labels for rank and file
- add indicator whose turn it is
- add indicator for replay mode
- calculate and display material advantage
- write a short Manual in Markdown
- optimize code (especially checking for mate)

### maybe with a lot of enthusiasm:
 - rudimentary chess bot to play against
 - playable through link cable

 Also a lot of playtesting to find bugs should be done!
<br>
<br>
 ## Attributions
- dev kit [GBDK-2020](https://github.com/gbdk-2020/gbdk-2020/) used to develop the game in C
- tools by H. Mulder: [TileDesigner](http://www.devrs.com/gb/hmgd/gbtd.html) and [TileMapBuilder](http://www.devrs.com/gb/hmgd/gbmb.html)
- Chess pieces (ingame) Pixel Art created by BerryArray ([ChessPieces](https://berryarray.itch.io/chess-pieces-16x16-one-bit))
- the logo for the title screen was put together with artwork from [vecteezy](https://www.vecteezy.com/) ([Pawn](https://www.vecteezy.com/vector-art/10954466-chess-pawn-icon-vector)) and [favpng](https://favpng.com/) ([Crown](https://favpng.com/png_view/chess-chess-piece-queen-pawn-clip-art-png/ktL4W54B))
- fonts used: rank and file labels from [fontstruct](https://fontstruct.com/) ([font](https://fontstruct.com/fontstructions/show/1334143/tic-computer-6x6-font)), everything else from [Daniel Hepper](https://github.com/dhepper) ([font](https://github.com/dhepper/font8x8/blob/master/font8x8_basic.h))