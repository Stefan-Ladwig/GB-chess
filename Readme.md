## Programming Chess from scratch for the original Gameboy

chess piece bitart from <link>

current status:
- rom compiles
- game playable -> regular moves working
- special rules/moves:
    -> implemented enterily: first Pawn move, Castling, En passaint, Stale- and Checkmate, Moves leaving King under attack not possible
    -> implemented partially: Promotion, checking for draw

left to implement:
- choosing piece when promoting
- ending game in draw, when material is insufficient
- chess clock
    -> font for digits
    -> menu to set/reset time
    -> keeping track of time
    -> ending, when time ran out
- after selecting a piece, only squares that a legal move leads to are selectable
- keeping track of every move, to review after game
- calculating and displaying material advantage

(with a lot of enthusiasm:
 - rudimentary chess bot to play against
 - playable through link cable)

 Also a lot of playtesting to find bugs should be done!