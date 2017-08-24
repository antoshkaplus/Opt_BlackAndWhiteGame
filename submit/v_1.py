
# tile values
BLACK = '.'
WHITE = 'X'

# orient
HORZ = 1
VERT = 2

# directions
UP = 1
DOWN = 2
LEFT = 3
RIGHT = 4

class Board:
    def __init__(self,board):
        self.sz = sz = len(board)
        self.board = [[board[i][j] for j in range(sz)] for i in range(sz)]

    def __getitem__(self,(i,j)):
        return self.board[i][j]
    def __setitem__(self,(i,j),elem):
        self.board[i][j] = elem

    def any(self,orient,k,val=WHITE):
        sz = self.sz
        res = False
        if orient == HORZ:
            for j in range(sz):
                if self[k,j] == val:
                    res = True
                    break
        if orient == VERT:
            for i in range(sz):
                if self[i,k] == val:
                    res = True
                    break
        return res

    def min(self,orient,k,val=WHITE):
        sz = self.sz
        res = None
        if orient == HORZ:
            for j in range(sz):
                if self[k,j] == val:
                    res = j
                    break
        if orient == VERT:
            for i in range(sz):
                if self[i,k] == val:
                    res = i
                    break
        return res

    def max(self,orient,k,val=WHITE):
        sz = self.sz
        res = None
        if orient == HORZ:
            for j in range(sz-1,-1,-1):
                if self[k,j] == val:
                    res = j
                    break
        if orient == VERT:
            for i in range(sz-1,-1,-1):
                if self[i,k] == val:
                    res = i
                    break
        return res

    def all(self,orient,k,val=WHITE):
        sz = self.sz
        res = True
        if orient == HORZ:
            for j in range(sz):
                if self[k,j] != val:
                    res = False
                    break
        if orient == VERT:
            for i in range(sz):
                if self[i,k] != val:
                    res = False
                    break
        return res




class BlackAndWhiteGame:

    def makeConnected(self,board):
        self.sz = sz = len(board)
        self.board = Board(board)
        self.cur_tile = BLACK

        self.algorithm2()
        return tuple(self.history)

    def shift(self,dir,k):
        """
        dir - direction
        k - position
        """
        board = self.board
        sz = self.sz

        move = ""

        if dir == RIGHT:
            m = board[k,sz-1]
            for j in range(sz-1,0,-1):
                board[k,j] = board[k,j-1]
            board[k,0] = self.cur_tile
            self.cur_tile = m
            move = "%d %d" % (k,0)

        elif dir == LEFT:
            m = board[k,0]
            for j in range(sz-1):
                board[k,j] = board[k,j+1]
            board[k,sz-1] = self.cur_tile
            self.cur_tile = m
            move = "%d %d" % (k,sz)

        elif dir == UP:
            m = board[0,k]
            for i in range(sz-1):
                board[i,k] = board[i+1,k]
            board[sz-1,k] = self.cur_tile
            self.cur_tile = m
            move = "%d %d" % (sz,k)

        elif dir == DOWN:
            m = board[sz-1,k]
            for i in range(sz-1,0,-1):
                board[i,k] = board[i-1,k]
            board[0,k] = self.cur_tile
            self.cur_tile = m
            move = "%d %d" % (0,k)

        return move

    def algorithm(self):
        """
        go by columns
        """
        self.history = []
        shift = self.shift
        add = self.history.append

        sz = self.sz
        ib,iw = 0,0
        jb,jw = 0,sz-1

        while jb != jw:
            if self.cur_tile == BLACK:
                add(shift(UP,jb))
                ib+=1
                if ib == sz:
                    jb+=1; ib=0
            else:
                add(shift(UP,jw))
                iw+=1
                if iw == sz:
                    jw-=1; iw=0

    def algorithm2(self):
        """
        go throw one
        """
        self.history = []
        shift = self.shift
        add = self.history.append
        board = self.board

        sz = self.sz
        ib,iw = 0,0
        jb,jw = 0,sz-1

        while True:
            if self.cur_tile == BLACK:
                add(shift(UP,jb))
                ib+=1
                if ib == sz:
                    jb+=1; ib=0
            else:
                add(shift(UP,jw))
                iw+=1
                if iw == sz:
                    if board.any(VERT,jw-1):
                        jw-=2
                    else: jw-=1
                    iw=0

            if jb >= jw:
                break
                if not board.all(VERT,jw+1):
                    jw+=1
                    iw=0
                else: break
