#include <map>
#include <set>
#include <cmath>
#include <stack>
#include <queue>
#include <string>
#include <vector>
#include <bitset>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <iostream>
#include <algorithm>
#include <sys/time.h>
using namespace std;
#define li        long long int
#define rep(i,to) for(int i=0;i<((int)(to));++i)
#define pb        push_back
#define sz(v)     ((int)(v).size())
#define bit(n)    (1<<(int)(n))
#define all(vec)  (vec).begin(),(vec).end()
#define each(i,c) for(__typeof((c).begin()) i=(c).begin();i!=(c).end();i++)
#define MP        make_pair
#define F         first
#define X         first
#define S         second
#define Y         second


//#define LOCAL

#ifdef LOCAL
#define GENERATE_LIMIT 3.0
	#define CLIMB_LIMIT 4.0
	#define TIME_LIMIT 4.75
#else
#define GENERATE_LIMIT 6.0
#define CLIMB_LIMIT 8.0
#define TIME_LIMIT 9.6
#endif


///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////

#ifdef LOCAL
#define assert(flag) { if(flag) cerr << "error : " << __LINE__ << endl; }
#else
#define assert(flag) ;
#endif


///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////




class Timer{
    static double getTimeOfDay();
    static unsigned long long int rdtsc();
    static unsigned long long int RDTSC_PER_SEC;
    static bool RDTSC_PER_SEC_INIT_FLAG;

    double start_time;
    unsigned long long int start_clock;

    inline double getElapsedTime();
public:

    Timer();
    void reset();
    void print(string label);
    bool inTime(double limit);
    double getRemain(double limit);
};
bool Timer::RDTSC_PER_SEC_INIT_FLAG=false;
unsigned long long int Timer::RDTSC_PER_SEC=1;

inline double Timer::getRemain(double limit)
{
    return limit-getElapsedTime()/RDTSC_PER_SEC;
}

inline bool Timer::inTime(double limit)
{
    return getElapsedTime()<limit*RDTSC_PER_SEC;
}

inline void Timer::print(string label)
{
    fprintf(stderr,"%s : %0.20lf\n",label.c_str(),getElapsedTime()/RDTSC_PER_SEC);
}

inline double Timer::getElapsedTime()
{
    if(RDTSC_PER_SEC_INIT_FLAG){
        return (double)(rdtsc()-start_clock);
    }else{
        double res=getTimeOfDay()-start_time;
        if(res<0.1) return res;
        RDTSC_PER_SEC=(rdtsc()-start_clock)/res;
        RDTSC_PER_SEC_INIT_FLAG=true;
        return (double)(rdtsc() - start_clock);
    }
}

inline void Timer::reset()
{
    start_time=getTimeOfDay();
    start_clock=rdtsc();
}

inline Timer::Timer()
{
    reset();
}

inline double Timer::getTimeOfDay()
{
    timeval tv;
    gettimeofday(&tv,0);
    return tv.tv_sec+tv.tv_usec*0.000001;
}

inline unsigned long long int Timer::rdtsc()
{
    unsigned int low,high;
    __asm__ volatile ("rdtsc" : "=a" (low), "=d" (high));
    return ((unsigned long long int)low)|((unsigned long long int)high<<32);
}



///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////




class XorShift{
    static unsigned int x;
    static unsigned int y;
    static unsigned int z;
    static unsigned int w;
    static unsigned int t;
public:
    static int rand();
};
unsigned int XorShift::x=123456789;
unsigned int XorShift::y=362436069;
unsigned int XorShift::z=521288629;
unsigned int XorShift::w=88675123;
unsigned int XorShift::t=1;

int XorShift::rand()
{
    t=x^(x<<11);
    x=y;
    y=z;
    z=w;
    w=(w^(w>>19))^(t^(t>>8));
    return w&0x7fffffff;
}





///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////

Timer timer;
typedef pair<int,int> P;
const int MAX = 105;
const int MAX_P = 5050;
const int MAX_MAP = 10;
const int MAX_ANS = 10;
int n;
int m;
int dx[] = {1, 0, -1, 0};
int dy[] = {0, 1, 0, -1};
vector<int> walk_indexes;
vector<int> path_index_part[6];
vector<string> board;


inline P Rot(P p, int rot)
{
    if(rot == 0) return P(p.X, p.Y);
    else if(rot == 1) return P(n - 1 - p.Y, p.X);
    else if(rot == 2) return P(n - 1 - p.X, n - 1 - p.Y);
    return P(p.Y, n - 1 - p.X);
}

inline P Derot(P p, int rot)
{

    if(rot == 0) return P(p.X, p.Y);
    else if(rot == 1) return P(p.Y, n - 1 - p.X);
    else if(rot == 2) return P(n - 1 - p.X, n - 1 - p.Y);
    return P(n - 1 - p.Y, p.X);
}


vector<int> getPathIndexCandidates(P from, P dest, int dist = 1 << 25)
{
    vector<int> res;
    int can_dist = dist + 30000;
    if(from == dest){
        res.pb(0);
    }else{
        if(from.X == dest.X){
            if(abs(from.Y - dest.Y) * 2 < dist + can_dist){
                rep(i, sz(path_index_part[1])){
                    if(i % 2 == 0) continue;
                    res.pb(path_index_part[1][i] );
                }
            }
        }else if(from.Y == dest.Y){
            if(abs(from.X - dest.X) * 2 < dist + can_dist){
                rep(i, sz(path_index_part[1])){
                    if(i % 2 == 1) continue;
                    res.pb(path_index_part[1][i]);
                }
            }
        }else{
            if((abs(from.Y - dest.Y) + abs(dest.X - from.X)) * 2 < dist + can_dist){

                rep(i, sz(path_index_part[2])){
                    res.pb(path_index_part[2][i]);
                }

                rep(i, sz(path_index_part[4])){
                    res.pb(path_index_part[4][i]);
                }
            }
        }
/*
		int d0 = min(min(from.X + 1, n - from.X), min(from.Y + 1, n - from.Y));
		int d1 = min(min(dest.X + 1, n - dest.X), min(dest.Y + 1, n - dest.Y));
		if((d0 + d1) * 2 < dist + can_dist){
			vector<pair<int,int> > best_directions;
			best_directions.pb(min(MP(n - from.X, 0), MP(from.X + 1, 2)));
			best_directions.pb(min(MP(n - from.Y, 1), MP(from.Y + 1, 3)));
			rep(i, sz(path_index_part[3]) / 4){
				rep(j, sz(best_directions)){
					int index = i * 4 + best_directions[j].S;
					res.pb(path_index_part[3][index]);
				}
			}
		}
*/
        rep(i, sz(path_index_part[5])) res.pb(path_index_part[5][i]);
    }
    return res;
}

void init(vector<string> _board)
{
    board = _board;
    n = sz(board);
    m = 0;
    rep(i, n) m += count(all(board[i]), 'X');
    path_index_part[0].pb(0);
    rep(i,  8) path_index_part[1].pb((1 << 10) + i);
    rep(i,  8) path_index_part[2].pb((2 << 10) + i);
    rep(i, 32) path_index_part[3].pb((3 << 10) + i);
    rep(i,  4) path_index_part[4].pb((4 << 10) + i);
    rep(i,  8) path_index_part[5].pb((5 << 10) + i);



    int b0, b1, b2, b3, b4, b5;
#define GET_BIT_INDEX \
	((b0 == 1) * bit(0) + (b1 == 1) * bit(1) + (b2 == 1) * bit(2) + \
	 (b3 == 1) * bit(3) + (b4 == 1) * bit(4) + (b5 == 1) * bit(5))
    for(b0 = 0; b0 < 2; b0++){
        for(b1 = 0; b1 < 2; b1++){
            if(b1){
                for(b2 = 0; b2 < 2; b2++){
                    if(b2){
                        for(b3 = 0; b3 < 2; b3++){
                            for(b4 = 0; b4 < 2; b4++){
                                walk_indexes.pb(GET_BIT_INDEX);
                            }
                        }
                    }else{
                        b3 = 0;
                        for(b4 = 0; b4 < 2; b4++){
                            walk_indexes.pb(GET_BIT_INDEX);
                        }
                    }
                }
            }else{
                for(b2 = 0; b2 < 2; b2++){
                    if(b2){
                        for(b3 = 0; b3 < 2; b3++){
                            for(b4 = 0; b4 < 2; b4++){
                                if(b4){
                                    walk_indexes.pb(GET_BIT_INDEX);
                                }else{
                                    for(b5 = 0; b5 < 2; b5++){
                                        walk_indexes.pb(GET_BIT_INDEX);
                                    }
                                }
                            }
                        }
                    }else{
                        for(b3 = 0; b3 < 2; b3++){
                            if(b3){
                                for(b4 = 0; b4 < 2; b4++){
                                    walk_indexes.pb(GET_BIT_INDEX);
                                }
                            }else{
                                for(b4 = 0; b4 < 2; b4++){
                                    if(b4){
                                        walk_indexes.pb(GET_BIT_INDEX);
                                    }else{
                                        for(b5 = 0; b5 < 2; b5++){
                                            walk_indexes.pb(GET_BIT_INDEX);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

//	walk_indexes.clear();
//	rep(i, bit(6)) walk_indexes.pb(i);

}

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////


class MAP
{
public:
    MAP();
    int have;
    int mp[MAX][MAX];
    int& getMap(int x, int y, int rot);
    int rotMap(int xy, int dir);
    void rotMapDist(int xy,int dir, int dist);
    void rotMapXY(P p);
    void cloneBoard();

    void union_init();
    int union_pr[MAX_P];
    int union_cnt[MAX_P];
    int union_find(int num);
    int max_group_element_num;
    P union_pos[MAX_P];
    P center;
    int center_pr;


//	int estimate();
};

MAP M[MAX_MAP];
bool M_used[MAX_MAP];

/*
int estimate_dist[MAX][MAX]; // false true

int MAP::estimate()
{
	int res = 0;
	union_init();
	queue<pair<int,P> > q;
	rep(i, n)rep(j, n){
		estimate_dist[i][j] = false;
		if(union_find(mp[i][j]) == center_pr){
			q.push(MP(0, MP(i, j)));
			estimate_dist[i][j] = true;
		}
	}
	bool edge = false;

	while(sz(q)){
		int dist = q.front().F;
		int x = q.front().S.F;
		int y = q.front().S.S;
		q.pop();
		rep(dir, 4){
			int xx = x + dx[dir];
			int yy = y + dy[dir];
			if(xx < 0 || n <= xx) continue;
			if(yy < 0 || n <= yy) continue;
			if(estimate_dist[xx][yy]) continue;
			estimate_dist[xx][yy] = true;
			if(mp[xx][yy] != -1) res += dist + 1;
			q.push(MP(dist + 1, MP(xx, yy)));
		}

		if(!edge){
			if(x == 0 || x + 1 == n || y == 0 || y + 1 == n){
				edge = true;
				rep(i, n){
					q.push(MP(dist + 1, MP(-1,  i)));
					q.push(MP(dist + 1, MP( n,  i)));
					q.push(MP(dist + 1, MP( i,  n)));
					q.push(MP(dist + 1, MP( i, -1)));
				}
			}
		}
	}

	return res * 2;
}
*/

inline int searchUnusedMap()
{
    rep(i, MAX_MAP)if(!M_used[i]){
            M_used[i] = true;
            return i;
        }
    assert(true);
}

inline int MAP::union_find(int num){
    if(union_pr[num] == num) return num;
    return union_pr[num] = union_find(union_pr[num]);
}

bool union_init_rand_mode = false;

void MAP::union_init()
{

    max_group_element_num = 0;
    center = MP(-1, -1);

    rep(i, n)rep(j, n){
            int index = mp[i][j];
            if(index == -1) continue;
            union_cnt[index] = 1;
            union_pos[index] = MP(i, j);
            union_pr[index] = index;
            for(int dir = 2; dir < 4; dir++){
                int x = i + dx[dir];
                int y = j + dy[dir];
                assert(n <= x || n <= y);
                if(x < 0 || y < 0) continue;
                if(mp[x][y] == -1) continue;
                int pr0 = union_find(index);
                int pr1 = union_find(mp[x][y]);
                if(pr0 == pr1) continue;
                union_pr[pr0] = pr1;
                union_cnt[pr1] += union_cnt[pr0];
                int point = union_cnt[pr1] + (union_init_rand_mode ? (XorShift::rand() % 5) : 0);
                if(max_group_element_num < point){
                    max_group_element_num = union_cnt[pr1];
                    center = union_pos[pr1];
                }
            }
        }
    if(center.F == -1){
        bool found = false;
        for(int x = 0; x < n && !found; x++){
            for(int y = 0; y < n && !found; y++){
                if(mp[x][y] != -1){
                    max_group_element_num = 1;
                    center = MP(x, y);
                    found = true;
                }
            }
        }
    }
    assert(center.F == -1);
    center_pr = union_find(mp[center.F][center.S]);
}



MAP::MAP()
{
}

inline int& MAP::getMap(int x, int y, int rot)
{
    P p = Derot(MP(x, y), rot);
    return mp[p.X][p.Y];
}

void MAP::cloneBoard()
{
    rep(i, n) memset(mp[i], -1, sizeof(int) * n);
    int cnt = 0;
    rep(i, n)rep(j, n)if(board[i][j] == 'X') mp[i][j] = cnt++;
    have = -1;
}

inline void MAP::rotMapXY(P p)
{
    if(p.X == -1){
        int t = mp[n - 1][p.Y];
        for(int i = n - 1; 0 < i; i--) mp[i][p.Y] = mp[i - 1][p.Y];
        mp[0][p.Y] = have;
        have = t;
    }else if(p.Y == -1){
        int t = mp[p.X][n - 1];
        for(int i = n - 1; 0 < i; i--) mp[p.X][i] = mp[p.X][i - 1];
        mp[p.X][0] = have;
        have = t;
    }else if(p.X == n){
        int t = mp[0][p.Y];
        for(int i = 1; i < n; i++) mp[i - 1][p.Y] = mp[i][p.Y];
        mp[n - 1][p.Y] = have;
        have = t;
    }else{
        int t = mp[p.X][0];
        for(int i = 1; i < n; i++) mp[p.X][i - 1] = mp[p.X][i];
        mp[p.X][n - 1] = have;
        have = t;
    }
}

inline int  MAP::rotMap(int xy, int dir)
{
    if(dir == 0){
        int t = mp[n - 1][xy];
        for(int i = 1; i < n; i++) mp[n - i][xy] = mp[n - 1 - i][xy];
        mp[0][xy] = have;
        have = t;
        if(have != -1) return 0;
        rep(i, n)if(mp[i][xy] != -1) return 1;
        return -1;
    }else if(dir == 1){
        int t = mp[xy][n - 1];
        for(int i = 1; i < n; i++) mp[xy][n - i] = mp[xy][n - 1 - i];
        mp[xy][0] = have;
        have = t;
        if(have != -1) return 0;
        rep(i, n)if(mp[xy][i] != -1) return 1;
        return -1;
    }else if(dir == 2){
        int t  = mp[0][xy];
        rep(i, n - 1) mp[i][xy] = mp[i + 1][xy];
        mp[n - 1][xy] = have;
        have = t;
        if(have != -1) return 0;
        rep(i, n)if(mp[i][xy] != -1) return 1;
        return -1;
    }else{
        int t = mp[xy][0];
        rep(i, n - 1) mp[xy][i] = mp[xy][i + 1];
        mp[xy][n - 1] = have;
        have = t;
        if(have != -1) return 0;
        rep(i, n)if(mp[xy][i] != -1) return 1;
        return -1;
    }
    return -1;
}

inline void MAP::rotMapDist(int xy, int dir, int dist)
{
    if(dir == 0){
        rep(i, dist) mp[n - 1 - i][n] = mp[n - 1 - i][xy];
        rep(i, n - dist) mp[n - 1 - i][xy] = mp[n - 1 - i - dist][xy];
        rep(i, dist) mp[i][xy] = mp[n - dist + i][n];
    }else if(dir == 1){
        rep(i, dist) mp[n][n - 1 - i] = mp[xy][n - 1 - i];
        rep(i, n - dist) mp[xy][n - 1 - i] = mp[xy][n - 1 - i - dist];
        rep(i, dist) mp[xy][i] = mp[n][n - dist + i];
    }else if(dir == 2){
        rep(i, dist) mp[i][n] = mp[i][xy];
        rep(i, n - dist) mp[i][xy] = mp[i + dist][xy];
        rep(i, dist) mp[n - 1 - i + dist][xy] = mp[i][n];
    }else if(dir == 3){
        rep(i, dist) mp[n][i] = mp[xy][i];
        rep(i, n - dist) mp[xy][i] = mp[xy][i + dist];
        rep(i, dist) mp[xy][i] = mp[n][n - 1 - i + dist];
    }
}





///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////



class ANSWER
{
public:
    P answer_from[MAX_P];
    P answer_dest[MAX_P];
    int answer_type[MAX_P];
    int answer_stack_m;
    vector<P> answer_stack;
    vector<pair<pair<P,P>,int> > answer_dat;

    ANSWER();

    void setAnswer(P from, P dest, int type, int index);
    void addAnswer(P from, P dest, int rot);
    void save();
};

ANSWER A[MAX_ANS];
bool A_used[MAX_ANS];

int searchUnusedAns()
{
    rep(i, MAX_ANS)if(!A_used[i]){
            A_used[i] = true;
            A[i].answer_stack_m = 0;
            A[i].answer_stack.clear();
            return i;
        }
    assert(true);
}


void ANSWER::save()
{
    answer_stack_m = sz(answer_stack);
}

ANSWER::ANSWER()
{
    answer_stack_m = 0;
}

void ANSWER::setAnswer(P from, P dest, int type, int index)
{
    answer_from[index] = from;
    answer_dest[index] = dest;
    answer_type[index] = type;
}

void ANSWER::addAnswer(P from, P dest, int rot)
{
    from = Derot(from, rot);
    dest = Derot(dest, rot);
    assert(from.X != dest.X && from.Y != dest.Y);

    int ASM = answer_stack_m;
    for(;from.X < dest.X; from.X++){
        if(ASM < sz(answer_stack) && answer_stack.back().X == n && answer_stack.back().Y == from.Y){
            answer_stack.pop_back();
        }else{
            answer_stack.pb(P(-1, from.Y));
        }
    }
    for(;dest.X < from.X; from.X--){
        if(ASM < sz(answer_stack) && answer_stack.back().X == -1 && answer_stack.back().Y == from.Y){
            answer_stack.pop_back();
        }else{
            answer_stack.pb(P(n, from.Y));
        }
    }
    for(;from.Y < dest.Y; from.Y++){
        if(ASM < sz(answer_stack) && answer_stack.back().X == from.X && answer_stack.back().Y == n){
            answer_stack.pop_back();
        }else{
            answer_stack.pb(P(from.X, -1));
        }
    }
    for(;dest.Y < from.Y; from.Y--){
        if(ASM < sz(answer_stack) && answer_stack.back().X == from.X && answer_stack.back().Y == -1){
            answer_stack.pop_back();
        }else{
            answer_stack.pb(P(from.X, n));
        }
    }
}

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////


class element
{
public:
    int ai;
    int mi;

    element();
    ~element();

    void restore();
    void save();
    void clone(element &e);

    int rotMapWithPushAns(int xy, int dir,int rot);
    int rotMapWithPushAns(P p, int dir, int rot);
    int rotMapWithPushAns(int xy, int dir);

    int checkPath(P from, P dest, int type, bool output, bool change);
    bool walkPath(P from, P dest, int type, bool only_this, bool is_first_x);
    void walkAlong(P form, P dest, int rot);
};

void element::walkAlong(P from, P dest, int rot)
{
    from = Derot(from, rot);
    dest = Derot(dest, rot);
    if(from.X == dest.X){
        for(;from.Y < dest.Y; from.Y++) rotMapWithPushAns(from.X, 1);
        for(;dest.Y < from.Y; dest.Y++) rotMapWithPushAns(from.X, 3);
    }else if(from.Y == dest.Y){
        for(;from.X < dest.X; from.X++) rotMapWithPushAns(from.Y, 0);
        for(;dest.X < from.X; dest.X++) rotMapWithPushAns(from.Y, 2);
    }else{
        assert(true);
    }
}

inline int element::rotMapWithPushAns(P p, int dir, int rot)
{
    p = Derot(p, rot);
    dir = (dir - rot + 4) & 3;
    if(dir == 0 || dir == 2) return rotMapWithPushAns(p.S, dir, 0);
    return rotMapWithPushAns(p.F, dir);
}


inline int element::rotMapWithPushAns(int xy, int dir, int rot)
{
    P p = Derot(MP(xy, xy), rot);
    dir = (dir - rot + 4 ) & 3;
    if(dir & 1) xy = p.F;
    else xy = p.S;
    return rotMapWithPushAns(xy, dir);
}

inline int element::rotMapWithPushAns(int xy, int dir)
{

    int ASM = A[ai].answer_stack_m;

    if(dir == 0){
        if(ASM < sz(A[ai].answer_stack) && A[ai].answer_stack.back().X == n && A[ai].answer_stack.back().Y == xy){
            A[ai].answer_stack.pop_back();
        }else{
            A[ai].answer_stack.pb(P(-1, xy));
        }
    }else if(dir == 1){
        if(ASM < sz(A[ai].answer_stack) && A[ai].answer_stack.back().X == xy && A[ai].answer_stack.back().Y == n){
            A[ai].answer_stack.pop_back();
        }else{
            A[ai].answer_stack.pb(P(xy, -1));
        }
    }else if(dir == 2){
        if(ASM < sz(A[ai].answer_stack) && A[ai].answer_stack.back().X == -1 && A[ai].answer_stack.back().Y == xy){
            A[ai].answer_stack.pop_back();
        }else{
            A[ai].answer_stack.pb(P(n, xy));
        }
    }else if(dir == 3){
        if(ASM < sz(A[ai].answer_stack) && A[ai].answer_stack.back().X == xy && A[ai].answer_stack.back().Y == -1){
            A[ai].answer_stack.pop_back();
        }else{
            A[ai].answer_stack.pb(P(xy, n));
        }
    }else{
        assert(true);
    }
    return M[mi].rotMap(xy, dir);
}

bool element::walkPath(P from, P dest, int type, bool only_this, bool is_first_x)
{
    bool rot = type & bit(0);
    if(!only_this && !rot && !is_first_x) return false;
    if(!only_this &&  rot &&  is_first_x) return false;
    from = Rot(from, rot);
    dest = Rot(dest, rot);
    bool adj = type & bit(1);
    bool around = type & bit(2);
    if(adj){
        if(around){
            P first_dest = MP(-1, -1);
            if(from.X + 1 < n - from.X) first_dest = MP(-1, from.Y);
            else first_dest = MP(n, from.Y);
            walkAlong(from, first_dest, rot);
            bool second_xy = type & bit(3);
            bool gap = type & bit(4); // left
            P second_from = MP(-1, -1);
            P second_dest = MP(-1, -1);
            int avoid_dir;
            if(second_xy){ // x
                if(gap){
                    avoid_dir = 3; // same direction
                    if(dest.Y - 1 <  0) return false;
                    second_dest = MP(dest.X, dest.Y - 1);
                    if(dest.X + 1 < n - dest.X) second_from = MP(-1, dest.Y - 1);
                    else second_from = MP(n, dest.Y - 1);
                }else{
                    avoid_dir = 1;
                    if(dest.Y + 1 == n) return false;
                    second_dest = MP(dest.X, dest.Y + 1);
                    if(dest.X + 1 < n - dest.X) second_from = MP(-1, dest.Y + 1);
                    else second_from = MP(n, dest.Y + 1);
                }
                walkAlong(second_from, second_dest, rot);
                rotMapWithPushAns(dest.X, avoid_dir ^ 0, rot);
                walkAlong(second_dest, second_from, rot);
                rotMapWithPushAns(dest.X, avoid_dir ^ 2, rot);
            }else{
                if(gap){
                    avoid_dir = 2; // same_direction
                    if(dest.X - 1 <  0) return false;
                    second_dest = MP(dest.X - 1, dest.Y);
                    if(dest.Y + 1 < n - dest.Y) second_from = MP(dest.X - 1, -1);
                    else second_from = MP(dest.X - 1, n);
                }else{
                    avoid_dir = 0;
                    if(dest.X + 1 == n) return false;
                    second_dest = MP(dest.X + 1, dest.Y);
                    if(dest.Y + 1 < n - dest.Y) second_from = MP(dest.X + 1, -1);
                    else second_from = MP(dest.X + 1, n);
                }
                walkAlong(second_from, second_dest, rot);
                rotMapWithPushAns(dest.Y, avoid_dir ^ 0, rot);
                walkAlong(second_dest, second_from, rot);
                rotMapWithPushAns(dest.Y, avoid_dir ^ 2, rot);
            }
            int cnt = 0;
            rep(i, n)if(M[mi].getMap(i, from.Y, rot) != -1) cnt++;
            if(0 < cnt) walkAlong(first_dest, from, rot);
            return true;
        }else{
            bool axis = type & bit(3);
            if(axis){ // x == x || y == y
//				assert(true);
                return false;
            }else{ // x != x && y != y

                if(from.X == dest.X || from.Y == dest.Y) return false;
                bool gap = type & bit(4);
                P first_dest = MP(-1, -1);
                P second_dest = MP(-1, -1);
                int avoid_dir = -1;
                if( gap && dest.X == 0) return false;
                if(!gap && dest.X + 1 == n) return false;
                if(gap){
                    first_dest = MP(dest.X - 1, from.Y);
                    second_dest = MP(dest.X - 1, dest.Y);
                    avoid_dir = 2;
                }else{
                    first_dest = MP(dest.X + 1, from.Y);
                    second_dest = MP(dest.X + 1, dest.Y);
                    avoid_dir = 0;
                }
                walkAlong(from, first_dest, rot);
                walkAlong(first_dest, second_dest, rot);
                rotMapWithPushAns(second_dest, avoid_dir ^ 0, rot);
                walkAlong(second_dest, first_dest, rot);
                rotMapWithPushAns(second_dest, avoid_dir ^ 2, rot);
                int cnt = 0;
                rep(i, n)if(M[mi].getMap(i, from.Y, rot) != -1) cnt++;
                if(0 < cnt) walkAlong(first_dest, from, rot);
                return true;
            }
        }
    }else{
        if(around){
            P first_dest = MP(-1, -1);
            if(from.X + 1 < n - from.X) first_dest = MP(-1, from.Y);
            else first_dest = MP(n, from.Y);
            P second_from = MP(-1, -1);
            bool second_xy = type & bit(3);
            bool avoid_else = type & bit(4);
            walkAlong(from, first_dest, rot);
            if(avoid_else){
                vector<pair<int,int> > avoided;
                rep(i, n){
                    if(second_xy){
                        if(M[mi].getMap(i, dest.Y, rot) == -1) continue;
                        if(dest.Y != 0 && M[mi].getMap(i, dest.Y - 1, rot) != -1) avoided.pb(MP(i, 3));
                        else avoided.pb(MP(i, 1));
                    }else{
                        if(M[mi].getMap(dest.X, i, rot) == -1) continue;
                        if(dest.X != 0 && M[mi].getMap(dest.X - 1, i, rot) != -1) avoided.pb(MP(i, 2));
                        else avoided.pb(MP(i, 0));
                    }
                }
                rep(i, sz(avoided)) rotMapWithPushAns(avoided[i].F, avoided[i].S ^ 0, rot);
                walkAlong(MP(dest.X, from.Y), dest, rot);
                rep(i, sz(avoided)) rotMapWithPushAns(avoided[i].F, avoided[i].S ^ 2, rot);
            }else{
                bool avoid = type & bit(5);
                int avoid_dir = -1;
                if(second_xy){ // x
                    if(dest.X + 1 < n - dest.X) second_from = MP(-1, dest.Y);
                    else second_from = MP(n, dest.Y);
                    if(avoid) avoid_dir = 1;
                    else avoid_dir = 3;
                }else{
                    if(dest.Y + 1 < n - dest.Y) second_from = MP(dest.X, -1);
                    else second_from = MP(dest.X, n);
                    if(avoid) avoid_dir = 0;
                    else avoid_dir = 2;
                }
                walkAlong(second_from, dest, rot);
                int cnt = 0;
                if(second_xy){
                    rep(i, n)if(M[mi].getMap(i, dest.Y, rot) != -1) cnt++;
                }else{ // y
                    rep(i, n)if(M[mi].getMap(dest.X, i, rot) != -1) cnt++;
                }
                if(1 < cnt){
                    rotMapWithPushAns(dest, avoid_dir ^ 0, rot);
                    walkAlong(dest, second_from, rot);
                    rotMapWithPushAns(dest, avoid_dir ^ 2, rot);
                }
            }
            int cnt = 0;
            rep(i, n)if(M[mi].getMap(i, from.Y, rot) != -1) cnt++;
            if(0 < cnt) walkAlong(first_dest, from, rot);
        }else{
            bool axis = type & bit(3);
            if(axis){ // x == x || y == y
                if(from.X != dest.X) return false;
                bool avoid = type & bit(4);
                walkAlong(from, dest, rot);
                int cnt = 0;
                rep(i, n)if(M[mi].getMap(from.F, i, rot) != -1) cnt++;
                if(1 < cnt){
                    int avoid_dir = -1;
                    if(avoid) avoid_dir = 1;
                    else avoid_dir = 3;
                    rotMapWithPushAns(dest.Y, avoid_dir ^ 0, rot);
                    walkAlong(dest, from, rot);
                    rotMapWithPushAns(dest.Y, avoid_dir ^ 2, rot);
                }
                return true;
            }else{ // x != x && y != y
                if(from.X == dest.X || from.Y == dest.Y) return false;
                bool avoid_else = type & bit(4);
                walkAlong(from, MP(dest.X, from.Y), rot);
                if(avoid_else){
                    vector<pair<int,int> > avoided;
                    rep(i, n){
                        if(M[mi].getMap(dest.X, i, rot) == -1) continue;
                        if(i == from.Y) continue;
                        if(dest.Y != 0 && M[mi].getMap(dest.X - 1, i, rot) != -1) avoided.pb(MP(i, 2));
                        else avoided.pb(MP(i, 0));
                    }
                    rep(i, sz(avoided)) rotMapWithPushAns(avoided[i].F, avoided[i].S ^ 0, rot);
                    walkAlong(MP(dest.X, from.Y), dest, rot);
                    rep(i, sz(avoided)) rotMapWithPushAns(avoided[i].F, avoided[i].S ^ 2, rot);
                }else{
                    walkAlong(MP(dest.X, from.Y), dest, rot);
                    bool avoid = type & bit(5);
                    int cnt = 0;
                    rep(i, n)if(M[mi].getMap(dest.X, i, rot) != -1) cnt++;
                    if(1 < cnt){
                        int avoid_dir = -1;
                        if(avoid) avoid_dir = 2;
                        else avoid_dir = 0;
                        rotMapWithPushAns(dest, avoid_dir ^ 0, rot);
                        walkAlong(dest, MP(dest.X, from.Y), rot);
                        rotMapWithPushAns(dest, avoid_dir ^ 2, rot);
                    }
                }
                int cnt = 0;
                rep(i, n)if(M[mi].getMap(i, from.Y, rot) != -1) cnt++;
                if(0 < cnt) walkAlong(MP(dest.X, from.Y), from, rot);
                return true;
            }
        }
    }
    return true;
}


void element::clone(element &e)
{
    rep(i, n) memcpy(M[mi].mp[i], M[e.mi].mp[i], sizeof(int) * n);
    memcpy(A[ai].answer_from, A[e.ai].answer_from, sizeof(P) * m);
    memcpy(A[ai].answer_dest, A[e.ai].answer_dest, sizeof(P) * m);
    memcpy(A[ai].answer_type, A[e.ai].answer_type, sizeof(int) * m);
    A[ai].answer_stack = A[e.ai].answer_stack;
    A[ai].answer_stack_m = A[e.ai].answer_stack_m;
    M[mi].have = M[e.mi].have;
}

void element::save()
{
    A[ai].answer_stack_m = sz(A[ai].answer_stack);
}

void element::restore()
{
    while(A[ai].answer_stack_m < sz(A[ai].answer_stack)) A[ai].answer_stack.pop_back();
    M[mi].cloneBoard();
    rep(i, A[ai].answer_stack_m) M[mi].rotMapXY(A[ai].answer_stack[i]);
}

element::element()
{
    mi = searchUnusedMap();
    ai = searchUnusedAns();
}

element::~element()
{
    M_used[mi] = false;
    A_used[ai] = false;
}




// return path length, or -1 if it is invalid
int element::checkPath(P from, P dest, int type, bool output, bool change)
{
    if(M[mi].have != -1) return -1;

    int sub_type = type & 1023;
    int rot = sub_type & 3;
    sub_type = sub_type >> 2;
    type = type >> 10;

    if(type == 0){ // no move
        if(from != dest) return -1;
        return 0;
    }else if(type == 1){ // straight
        from = Rot(from, rot);
        dest = Rot(dest, rot);
        if(dest.S - 1 < 0) return -1;
        if(n <= dest.S + 1) return -1;
        if(dest.F < from.F) return -1;
        if(from.S != dest.S) return -1;
        if(M[mi].getMap(dest.F, dest.S, rot) != -1) return -1;
        if(M[mi].getMap(n - (dest.F - from.F), dest.S, rot) != -1) return -1;
        int cnt = 0;
        if(output){
            rep(i, n)if(M[mi].getMap(i, dest.S, rot) != -1) cnt++;
        }
        if(sub_type == 0){ // left avoid, none around
            if(M[mi].getMap(dest.F, dest.S - 1, rot) != -1) return -1;
            if(M[mi].getMap(dest.F, n - 1, rot) != -1) return -1;
            if(output){
                A[ai].addAnswer(from, dest, rot);
                if(2 <= cnt){
                    A[ai].addAnswer(dest, MP(dest.F, dest.S + 1), rot);
                    A[ai].addAnswer(dest, from, rot);
                    A[ai].answer_dat.pb(MP(MP(MP(dest.F, dest.S + 1), dest), rot));
                }
            }
        }else if(sub_type == 1){ // right avoid, none around
            if(M[mi].getMap(dest.F, dest.S + 1, rot) != -1) return -1;
            if(M[mi].getMap(dest.F, 0, rot) != -1) return -1;
            if(output){
                A[ai].addAnswer(from, dest, rot);
                if(2 <= cnt){
                    A[ai].addAnswer(dest, MP(dest.F, dest.S - 1), rot);
                    A[ai].addAnswer(dest, from, rot);
                    A[ai].answer_dat.pb(MP(MP(MP(dest.F, dest.S - 1), dest), rot));
                }
            }
        }
        if(change){
            M[mi].getMap(dest.F, dest.S, rot) = M[mi].getMap(from.F, from.S, rot);
            M[mi].getMap(from.F, from.S, rot) = -1;
        }
        return (abs(dest.F - from.F) + 1) * 2;
    }else if(type == 2){ // L
        from = Rot(from, rot);
        dest = Rot(dest, rot);
        if(dest.S == from.S) return -1;
        if(dest.F <= from.F) return -1;
        if(M[mi].getMap(dest.F, dest.S, rot) != -1) return -1;
        int tx = (n - (dest.F - from.F)) % (n + 1);
        int ty = (n - (dest.S - from.S)) % (n + 1);
        if(M[mi].getMap(tx, from.S, rot) != -1) return -1;
        if(M[mi].getMap(dest.F, ty, rot) != -1) return -1;
        P first_dest = MP(dest.F, from.S);
        bool avoid = sub_type & 1;
        if(dest.F == 0) return -1;
        if(dest.F + 1 == n) return -1;
        if(avoid){
            if(M[mi].getMap(0, dest.S, rot) != -1) return -1;
            if(M[mi].getMap(dest.F + 1, dest.S, rot) != -1) return -1;
        }else{
            if(M[mi].getMap(n - 1, dest.S, rot) != -1) return -1;
            if(M[mi].getMap(dest.F - 1, dest.S, rot) != -1) return -1;
        }
        if(output){
            int cnt0 = 0, cnt1 = 0;
            rep(i, n)if(M[mi].getMap(dest.F, i, rot) != -1) cnt0++;
            rep(i, n)if(M[mi].getMap(i, from.S, rot) != -1) cnt1++;
            A[ai].addAnswer(from, first_dest, rot);
            A[ai].addAnswer(first_dest, dest, rot);
            if(0 < cnt0){
                if(avoid){
                    A[ai].addAnswer(dest, MP(dest.F - 1, dest.S), rot);
                    A[ai].addAnswer(dest, first_dest, rot);
                    A[ai].addAnswer(MP(dest.F - 1, dest.S), dest, rot);
                }else{
                    A[ai].addAnswer(dest, MP(dest.F + 1, dest.S), rot);
                    A[ai].addAnswer(dest, first_dest, rot);
                    A[ai].addAnswer(MP(dest.F + 1, dest.S), dest, rot);
                }
            }
            if(1 < cnt1){
                A[ai].addAnswer(first_dest, from, rot);
            }
        }
        if(change){
            M[mi].getMap(dest.F, dest.S, rot) = M[mi].getMap(from.F, from.S, rot);
            M[mi].getMap(from.F, from.S, rot) = -1;
        }
        int dist0 = abs(dest.F - from.F);
        int dist1 = abs(dest.S - from.S);
        return (dist0 + dist1 + 1) * 2;
    }else if(type == 3){
        assert(true);
/*
		int first_dir = rot;
		int dist = -1;
		rot = sub_type & 3;
		sub_type = sub_type >> 2;
		first_dir = (first_dir + rot) & 3;

		from = Rot(from, rot);
		dest = Rot(dest, rot);
		if(from.F == dest.F) return -1;
		if(from.S == dest.S) return -1;
		if(M[mi].getMap(dest.F, dest.S, rot) != -1) return -1;
		if(M[mi].getMap(n - 1 - dest.F, dest.S, rot) != -1) return -1;
		if(n <= dest.S + 1) return -1;
		if(dest.S - 1 < 0) return -1;
		if(sub_type & 1){ // left avoid

			if(M[mi].getMap(dest.F, n - 1, rot) != -1) return -1;
			if(M[mi].getMap(dest.F, dest.S - 1, rot) != -1) return -1;

			if(first_dir == 0 || first_dir == 2){
				int ok = 0;
				if(dest.S - 1 != from.S) ok |= 1;
				if(from.S + 1 != n) ok |= 2;
				int x = (from.F + (dest.F + 1)) % (n + 1);
				if(ok != 3 && x != n && M[mi].getMap(x, from.S, rot) != -1) return -1;
				if(first_dir == 2) dist = from.F + 1;
				else dist = n - from.F;
			}else{
				if(from.F == n - 1 - dest.F){
					int y = (dest.S + (from.S + 1) + (n + 1)) % (n + 1);
					if(y != n && M[mi].getMap(from.F, y, rot) != -1) return -1;
				}
				if(first_dir == 3)	dist = from.S + 1;
				else dist = n - from.S;
			}

			assert(dist <= 0);


			if(output){
				if(first_dir == 0) A[ai].addAnswer(from, MP(n, from.S), rot);
				else if(first_dir == 1) A[ai].addAnswer(from, MP(from.F, n), rot);
				else if(first_dir == 2) A[ai].addAnswer(from, MP(-1, from.S), rot);
				else A[ai].addAnswer(from, MP(from.F, -1), rot);

				A[ai].addAnswer(MP(-1, dest.S), dest, rot);

				int cnt = 0; // first rotation moves white dest the row of "dest".
				rep(i, n)if(M[mi].getMap(i, dest.S, rot) != -1) cnt++;
				if(first_dir == 1 || first_dir == 3){
					int y = (dest.S + (from.S + 1)) % (n + 1);
					if(y != n && M[mi].getMap(from.F, y, rot) != -1) cnt++;
				}
				if(1 <= cnt){
					A[ai].addAnswer(dest, MP(dest.F, dest.S + 1), rot);
					A[ai].addAnswer(dest, MP(-1, dest.S), rot);
					A[ai].addAnswer(MP(dest.F, dest.S + 1), dest, rot);
				}

				int cnt1 = 0;
				if(first_dir == 0 || first_dir == 2){
					rep(i, n)if(M[mi].getMap(i, from.S, rot) != -1) cnt1++;
					if(first_dir == 0 && 2 <= cnt1) A[ai].addAnswer(MP( n, from.S), from, rot);
					if(first_dir == 2 && 2 <= cnt1) A[ai].addAnswer(MP(-1, from.S), from, rot);
				}else{
					rep(i, n)if(M[mi].getMap(from.F, i, rot) != -1) cnt1++;
					if(first_dir == 1 && 2 <= cnt1) A[ai].addAnswer(MP(from.F,  n), from, rot);
					if(first_dir == 3 && 2 <= cnt1) A[ai].addAnswer(MP(from.F, -1), from, rot);
				}
			}
		}else{ // right avoid
			if(M[mi].getMap(dest.F, 0, rot) != -1) return -1;
			if(M[mi].getMap(dest.F, dest.S + 1, rot) != -1) return -1;

			if(first_dir == 0 || first_dir == 2){
				int ok = 0;
				if(dest.S + 1 != from.S) ok |= 1;
				if(from.S != 0) ok |= 2;
				int x = (from.F + (dest.F + 1)) % (n + 1);
				if(ok != 3 && x != n && M[mi].getMap(x, from.S, rot) != -1) return -1;
				if(first_dir == 2)	dist = from.F + 1;
				else dist = n - from.F;
			}else{
				if(from.F == n - 1 - dest.F){
					int y = (dest.S + (from.S + 1) + (n + 1)) % (n + 1);
					if(y != n && M[mi].getMap(from.F, y, rot) != -1) return -1;
				}
				if(first_dir == 3) dist = from.S + 1;
				else dist = n - from.S;
			}

			assert(dist <= 0);

			if(output){
				if(first_dir == 0) A[ai].addAnswer(from, MP(n, from.S), rot);
				else if(first_dir == 1) A[ai].addAnswer(from, MP(from.F, n), rot);
				else if(first_dir == 2) A[ai].addAnswer(from, MP(-1, from.S), rot);
				else A[ai].addAnswer(from, MP(from.F, -1), rot);

				A[ai].addAnswer(MP(-1, dest.S), dest, rot);
				int cnt = 0; // first rotation moves white dest the row of "dest".
				rep(i, n)if(M[mi].getMap(i, dest.S, rot) != -1) cnt++;
				if(first_dir == 1 || first_dir == 3){
					int y = (dest.S + (from.S + 1)) % (n + 1);
					if(y != n && M[mi].getMap(from.F, y, rot) != -1) cnt++;
				}

				if(1 <= cnt){
					A[ai].addAnswer(dest, MP(dest.F, dest.S - 1), rot);
					A[ai].addAnswer(dest, MP(-1, dest.S), rot);
					A[ai].addAnswer(MP(dest.F, dest.S - 1), dest, rot);
				}

				int cnt1 = 0;
				if(first_dir == 0 || first_dir == 2){
					rep(i, n)if(M[mi].getMap(i, from.S, rot) != -1) cnt1++;
					if(first_dir == 0 && 2 <= cnt1) A[ai].addAnswer(MP( n, from.S), from, rot);
					if(first_dir == 2 && 2 <= cnt1) A[ai].addAnswer(MP(-1, from.S), from, rot);
				}else{
					rep(i, n)if(M[mi].getMap(from.F, i, rot) != -1) cnt1++;
					if(first_dir == 1 && 2 <= cnt1) A[ai].addAnswer(MP(from.F,  n), from, rot);
					if(first_dir == 3 && 2 <= cnt1) A[ai].addAnswer(MP(from.F, -1), from, rot);
				}

			}
		}
		if(change){
			M[mi].getMap(dest.F, dest.S, rot) = M[mi].getMap(from.F, from.S, rot);
			M[mi].getMap(from.F, from.S, rot) = -1;
		}
		return (dest.F + dist + 1) * 2 + 1;
*/
    }else if(type == 4){ // L coming
        from = Rot(from, rot);
        dest = Rot(dest, rot);
        if(from.F == dest.F) return -1;
        if(from.S == dest.S) return -1;
        if(dest.F < from.F) return -1;
        if(M[mi].getMap(dest.F, dest.S, rot) != -1) return -1;
        if(M[mi].getMap(dest.F, from.S, rot) != -1) return -1;
        int tx = (n - (dest.F - from.F)) % (n + 1);
        int ty = (n + (dest.S - from.S)) % (n + 1);
        assert(tx == n);
        assert(ty == n);
        if(M[mi].getMap(tx, from.S, rot) != -1) return -1;
        if(M[mi].getMap(dest.F, ty, rot) != -1) return -1;
        if(output){
            int cnt0 = 0;
            int cnt1 = 0;
            rep(i, n)if(M[mi].getMap(dest.F, i, rot) != -1) cnt0++;
            rep(i, n)if(M[mi].getMap(i, from.S, rot) != -1) cnt1++;
            if(0 < cnt0) A[ai].addAnswer(dest, MP(dest.F, from.S), rot);
            A[ai].addAnswer(from, MP(dest.F, from.S), rot);
            A[ai].addAnswer(MP(dest.F, from.S), dest, rot);
            if(1 < cnt1) A[ai].addAnswer(MP(dest.F, from.S), from, rot);
        }
        if(change){
            M[mi].getMap(dest.F, dest.S, rot) = M[mi].getMap(from.F, from.S, rot);
            M[mi].getMap(from.F, from.S, rot) = -1;
        }
        return (abs(dest.F - from.F) + abs(dest.S - from.S)) * 2;
    }else if(type == 5){ // around coming
        from = Rot(from, rot);
        dest = Rot(dest, rot);
        if(from.F + 1 < n - from.F) return -1;
        P first_dest = MP(n, from.S);
        P second_from;
        int cnt0 = 0, cnt1 = 0;
        if(M[mi].getMap(dest.F, dest.S, rot) != -1) return -1;
        if(sub_type & 1){ // x
            if(from.S == dest.S) return -1;
            if(dest.F + 1 < n - dest.F) second_from = MP(-1, dest.S);
            else second_from = MP(n, dest.S);
            if(output){
                rep(i, n)if(M[mi].getMap(i, dest.S, rot) != -1) cnt0++;
                rep(i, n)if(M[mi].getMap(i, from.S, rot) != -1) cnt1++;
            }
        }else{ // y
            if(M[mi].getMap(dest.X, from.Y, rot) != -1) return -1;
            if(from.S == dest.S) return -1;
            if(from.F == dest.F) return -1;
            int ty = (dest.S + 1 + from.S) % (n + 1);
            int tx = (dest.F + 1 + from.F) % (n + 1);
            if(ty != n && M[mi].getMap(dest.F, ty, rot) != -1) return -1;
            if(tx != n && M[mi].getMap(tx, from.S, rot) != -1) return -1;
            if(dest.S + 1 < n - dest.S) second_from = MP(dest.F, -1);
            else second_from = MP(dest.F, n);
            if(output){
                rep(i, n)if(M[mi].getMap(dest.F, i, rot) != -1) cnt0++;
                rep(i, n)if(M[mi].getMap(i, from.S, rot) != -1) cnt1++;
            }
        }
        if(output){
            if(0 < cnt0) A[ai].addAnswer(dest, second_from, rot);
            A[ai].addAnswer(from, first_dest, rot);
            A[ai].addAnswer(second_from, dest, rot);
            if(1 < cnt1) A[ai].addAnswer(first_dest, from, rot);
        }
        if(change){
            M[mi].getMap(dest.F, dest.S, rot) = M[mi].getMap(from.F, from.S, rot);
            M[mi].getMap(from.F, from.S, rot) = -1;
        }
        return ((n - from.F) + abs(second_from.F - dest.F) + abs(second_from.S - dest.S)) * 2;
    }
    assert(true);
}



///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////



class Processor
{
    static vector<int> follower[MAX_P];
    static vector<int> following_mp[MAX][MAX];
    static int seal[MAX_P];
    void analyze(int index, P from, P dest, int type);
    void marking(int index, int x, int y, int rot);

public:
    element e;
    Processor();



    int getScore();
    bool collapsed;
    void greedy();
    bool greedyClimbing();
    vector<string> getAnswer(bool final_answer = true);
};
vector<int> Processor::follower[MAX_P];
vector<int> Processor::following_mp[MAX][MAX];
int Processor::seal[MAX_P];





























int climb_mp[MAX][MAX];
int climb_pr[MAX_P];
int climb_candidated[MAX][MAX];
int climb_used_mp[MAX][MAX];
inline int climb_union_find(int num){
    if(climb_pr[num] == num) return num;
    return climb_pr[num] = climb_union_find(climb_pr[num]);
}










inline void climb_marking(int index, int x, int y, int rot)
{
    P p = Derot(MP(x, y), rot);
/*
	if(M[e.mi].mp[p.F][p.S] != -1){
		follower[M[e.mi].mp[p.F][p.S]].pb(index);
		seal[index]++;
	}
	following_mp[p.F][p.S].pb(index);
*/
    climb_candidated[p.F][p.S] = true;
}



void climb_analyze(int index, P from, P dest, int type)
{
/*
	rep(i, sz(following_mp[dest.F][dest.S])){
		follower[following_mp[dest.F][dest.S][i]].pb(index);
		seal[index]++;
	}
*/
    int sub_type = type & 1023;
    int rot = sub_type & 3;
    sub_type = sub_type >> 2;
    type = type >> 10;


    if(type == 0) return;
    if(type == 1){ // straight
        from = Rot(from, rot);
        dest = Rot(dest, rot);
        climb_marking(index, dest.F, dest.S, rot);
        climb_marking(index, n - (dest.F - from.F), dest.S, rot);
        if(sub_type == 0){ // left avoid, none around
            climb_marking(index, dest.F, dest.S - 1, rot);
            climb_marking(index, dest.F, n - 1, rot);
            return;
        }else if(sub_type == 1){ // right avoid, none around
            climb_marking(index, dest.F, dest.S + 1, rot);
            climb_marking(index, dest.F, 0, rot);
            return;
        }
        assert(true);


    }else if(type == 2){ // L
        from = Rot(from, rot);
        dest = Rot(dest, rot);
        climb_marking(index, dest.F, dest.S, rot);
        int tx = (n - (dest.F - from.F)) % (n + 1);
        int ty = (n - (dest.S - from.S)) % (n + 1);
        climb_marking(index, tx, from.S, rot);
        climb_marking(index, dest.F, ty, rot);
        bool avoid = sub_type & 1;
        if(avoid){
            climb_marking(index, 0, dest.S, rot);
            climb_marking(index, dest.F + 1, dest.S, rot);
        }else{
            climb_marking(index, n - 1, dest.S, rot);
            climb_marking(index, dest.F - 1, dest.S, rot);
        }
        return;
    }else if(type == 3){
        int first_dir = rot;
        int dist = -1;
        rot = sub_type & 3;
        sub_type = sub_type >> 2;
        from = Rot(from, rot);
        dest = Rot(dest, rot);
        first_dir = (first_dir + rot) & 3;

        climb_marking(index, dest.F, dest.S, rot);
        climb_marking(index, n - 1 - dest.F, dest.S, rot);
        if(sub_type & 1){ // left avoid
            climb_marking(index, dest.F, n - 1, rot);
            climb_marking(index, dest.F, dest.S - 1, rot);
            if(first_dir == 0 || first_dir == 2){
                int ok = 0;
                if(dest.S - 1 != from.S) ok |= 1;
                if(from.S + 1 != n) ok |= 2;
                int x = (from.F + (dest.F + 1)) % (n + 1);
                if(ok != 3 && x != n) climb_marking(index, x, from.S, rot);
            }else{
                if(from.F == n - 1 - dest.F){
                    int y = (dest.S + (from.S + 1) + (n + 1)) % (n + 1);
                    if(y != n) climb_marking(index, from.F, y, rot);
                }
            }
            return;
        }else{ // right avoid
            climb_marking(index, dest.F, 0, rot);
            climb_marking(index, dest.F, dest.S + 1, rot);
            if(first_dir == 0 || first_dir == 2){
                int ok = 0;
                if(dest.S + 1 != from.S) ok |= 1;
                if(from.S != 0) ok |= 2;
                int x = (from.F + (dest.F + 1)) % (n + 1);
                if(ok != 3 && x != n) climb_marking(index, x, from.S, rot);
            }else{
                if(from.F == n - 1 - dest.F){
                    int y = (dest.S + (from.S + 1) + (n + 1)) % (n + 1);
                    if(y != n) climb_marking(index, from.F, y, rot);
                }
            }
            return;
        }
    }else if(type == 4){
        from = Rot(from, rot);
        dest = Rot(dest, rot);
        climb_marking(index, dest.F, dest.S, rot);
        climb_marking(index, dest.F, from.S, rot);
        climb_marking(index, (n - (dest.F - from.F)) % (n + 1), from.S, rot);
        climb_marking(index, dest.F, (n - (from.S - dest.S)) % (n + 1), rot);
        return;
    }else if(type == 5){
        from = Rot(from, rot);
        dest = Rot(dest, rot);
        climb_marking(index, dest.F, dest.S, rot);
        if(sub_type & 1){
        }else{
            climb_marking(index, dest.X, from.Y, rot);
            int ty = (dest.S + 1 + from.S) % (n + 1);
            int tx = (dest.F + 1 + from.F) % (n + 1);
            if(ty != n) climb_marking(index, dest.F, ty, rot);
            if(tx != n) climb_marking(index, tx, from.Y, rot);
        }
        return;
    }
    assert(true);
}













bool Processor::greedyClimbing()
{
#define CHECK_CLIMB_LIMIT  { if(!timer.inTime(CLIMB_LIMIT)){ collapsed = true; return false; } }
    MAP &mp = M[e.mi];
    ANSWER &ans = A[e.ai];

    assert(collapsed);
    e.restore();
    int none_move_cnt = 0;
    CHECK_CLIMB_LIMIT;
    while(none_move_cnt < m && ans.answer_type[none_move_cnt] == 0) none_move_cnt++;
    if(none_move_cnt == m) return false;

    int start = XorShift::rand() % (m - none_move_cnt) + none_move_cnt;
    int end   = XorShift::rand() % (m - none_move_cnt) + none_move_cnt;
    if(end < start) swap(start, end);
    if(XorShift::rand() % 5 == 0) end = m - 1;
    end = min(end, start + (none_move_cnt));
    set<P> need;
    set<P> rem;
    vector<int> rem_vec;
    vector<P> used_vec;

    rep(i, n) memset(climb_candidated[i], false, sizeof(int) * n);
    rep(i, n) memset(climb_used_mp[i], false, sizeof(int) * n);
    for(int i = end + 1; i < m && timer.inTime(CLIMB_LIMIT); i++){
        climb_analyze(i, ans.answer_from[i], ans.answer_dest[i], ans.answer_type[i]);
    }
    rep(i, start){
        e.checkPath(ans.answer_from[i], ans.answer_dest[i], ans.answer_type[i], false, true);
    }
    CHECK_CLIMB_LIMIT;
    {
        rep(i, n) memset(climb_mp[i], -1, sizeof(int) * n);
        for(int i = 0; i < m; i++){
            if(i < start || end < i){
                climb_mp[ans.answer_dest[i].F][ans.answer_dest[i].S] = i;
                climb_used_mp[ans.answer_dest[i].F][ans.answer_dest[i].S] = true;
                used_vec.pb(ans.answer_dest[i]);
                climb_candidated[ans.answer_dest[i].F][ans.answer_dest[i].S] = true;
            }else{
                rem.insert(ans.answer_from[i]);
                rem_vec.pb(i);
            }
        }
        CHECK_CLIMB_LIMIT;
        rep(i, sz(rem_vec)) swap(rem_vec[i], rem_vec[XorShift::rand() % (i + 1)]);
        int group_num = m - (end - start + 1);
        rep(i, m) climb_pr[i] = i;
        rep(i, n)rep(j, n)if(climb_mp[i][j] != -1){
                    rep(k, 4){
                        int x = i + dx[k];
                        int y = j + dy[k];
                        if(x < 0 || n <= x) continue;
                        if(y < 0 || n <= y) continue;
                        if(climb_mp[x][y] == -1) continue;
                        int pr0 = climb_union_find(climb_mp[i][j]);
                        int pr1 = climb_union_find(climb_mp[x][y]);
                        if(pr0 == pr1) continue;
                        group_num--;
                        climb_pr[pr0] = pr1;
                    }
                }
        CHECK_CLIMB_LIMIT;
        rep(i, sz(rem_vec)){
            if(group_num == 1) break;
            group_num++;
            int x = ans.answer_dest[rem_vec[i]].F;
            int y = ans.answer_dest[rem_vec[i]].S;
            climb_mp[x][y] = rem_vec[i];
            climb_candidated[x][y] = true;
            need.insert(MP(x, y));
            rep(j, 4){
                int xx = x + dx[j];
                int yy = y + dy[j];
                if(xx < 0 || n <= xx) continue;
                if(yy < 0 || n <= yy) continue;
                if(climb_mp[xx][yy] == -1) continue;
                int pr0 = climb_union_find(climb_mp[x][y]);
                int pr1 = climb_union_find(climb_mp[xx][yy]);
                if(pr0 == pr1) continue;
                group_num--;
                climb_pr[pr0] = pr1;
            }
        }
    }


    CHECK_CLIMB_LIMIT;

    typedef pair<int,pair<P,pair<P,int> > > ANS_P;
    priority_queue<ANS_P,vector<ANS_P>,greater<ANS_P> > q0;
    priority_queue<ANS_P,vector<ANS_P>,greater<ANS_P> > q1;
    each(it0, rem){
        CHECK_CLIMB_LIMIT;
        each(it1, need){
            vector<int> indexes = getPathIndexCandidates(*it0, *it1);
            rep(i, sz(indexes)){
                int t = e.checkPath(*it0, *it1, indexes[i], false, false);
                if(t == -1) continue;
                q0.push(MP(t + XorShift::rand() % 10, MP(*it0, MP(*it1, indexes[i]))));
            }
        }
    }


    int answer_index = start;
    while(sz(q0) && sz(need)){
        CHECK_CLIMB_LIMIT;
        P p = q0.top().S.S.F;
        int dist = q0.top().F;
        P from = q0.top().S.F;
        int path_index = q0.top().S.S.S;
        q0.pop();
        if(climb_used_mp[p.F][p.S]) continue;
        if(rem.find(from) == rem.end()) continue;
        int t = e.checkPath(from, p, path_index, false, false);
        if(t == -1) continue;
        e.checkPath(from, p, path_index, false, true);
        ans.setAnswer(from, p, path_index, answer_index++);
        rem.erase(from);
        climb_used_mp[p.F][p.S] = true;
        used_vec.pb(p);
        need.erase(p);
    }

    each(it0, rem){
        CHECK_CLIMB_LIMIT;
        rep(used_index, sz(used_vec)){
            rep(dir, 4){
                int x = used_vec[used_index].F + dx[dir];
                int y = used_vec[used_index].S + dy[dir];
                if(x < 0 || n<=x) continue;
                if(y < 0 || n<=y) continue;
                if(climb_candidated[x][y]) continue;
                vector<int> indexes = getPathIndexCandidates(*it0, MP(x, y));
                rep(i, sz(indexes)){
                    int t = e.checkPath(*it0, MP(x, y), indexes[i], false, false);
                    if(t == -1) continue;
                    q1.push(MP(t + XorShift::rand() % 10, MP(*it0, MP(MP(x, y), indexes[i]))));
                }
            }
        }
    }

    if(sz(need)){
        collapsed = true;
        return false;
    }


    while(sz(q1) && sz(rem)){
        CHECK_CLIMB_LIMIT;
        P p = q1.top().S.S.F;
        P from = q1.top().S.F;
        int dist = q1.top().F;
        int path_index = q1.top().S.S.S;
        q1.pop();
        if(climb_used_mp[p.F][p.S]) continue;
        if(rem.find(from) == rem.end()) continue;
        int t = e.checkPath(from, p, path_index, false, false);
        if(t == -1) continue;
        e.checkPath(from, p, path_index, false, true);
        ans.setAnswer(from, p, path_index, answer_index++);
        rem.erase(from);
        climb_used_mp[p.F][p.S] = true;

        rep(dir, 4){
            int x = p.F + dx[dir];
            int y = p.S + dy[dir];
            if(x < 0 || n <= x) continue;
            if(y < 0 || n <= y) continue;
            if(climb_candidated[x][y]) continue;
            climb_candidated[x][y] = true;
            each(it, rem){
                vector<int> indexes = getPathIndexCandidates(*it, MP(x, y));
                rep(i, sz(indexes)){
                    int t = e.checkPath(*it, MP(x, y), indexes[i], false, false);
                    if(t == -1) continue;
                    q1.push(MP(t + XorShift::rand() % 10, MP(*it, MP(MP(x, y), indexes[i]))));
                }
            }
        }
    }

    if(sz(rem)){
        collapsed = true;
        return false;
    }
    return true;
}

















































int Processor::getScore()
{
    if(collapsed) return 1 << 25;
    return sz(getAnswer(false));
}



bool isSame(element &e0, element &e1)
{
    rep(i, n)rep(j, n)if(M[e0.mi].mp[i][j] != M[e1.mi].mp[i][j]) return false;
    return true;
}

vector<int> dat[MAX][4][6];
int candidates_sum[MAX][4];
vector<int> answer_memo[4][MAX];

vector<string> Processor::getAnswer(bool final_answer)
{
    Timer T;
    e.restore();

    memset(candidates_sum, 0, sizeof(int[4]) * n);
    memset(seal, 0, sizeof(int) * m);

    rep(i, n)rep(j, 4)rep(k, 6) dat[i][j][k].clear();
    rep(i, n)rep(j, n) following_mp[i][j].clear();
    rep(i, m) follower[i].clear();

    rep(i, m) M[e.mi].mp[A[e.ai].answer_from[i].F][A[e.ai].answer_from[i].S] = i;
    rep(i, m) analyze(i, A[e.ai].answer_from[i], A[e.ai].answer_dest[i], A[e.ai].answer_type[i]);

    e.restore();
    set<pair<int,int> > candidates;


    rep(i, m){
        int type = A[e.ai].answer_type[i];
        int rot = type & 3;
        type = type >> 10;
        if(type == 0) continue;
        int xy;
        if(rot == 0 || rot == 2) xy = A[e.ai].answer_from[i].S;
        else xy = A[e.ai].answer_from[i].F;
        candidates_sum[xy][rot]++;
        if(seal[i] == 0){
            dat[xy][rot][type].pb(i);
            candidates.insert(MP(xy, rot));
        }
    }

    if(final_answer && false){
/*
		while(sz(candidates)){

			element e_dat;
			e_dat.clone(e);

			vector<int> used;
			pair<int,pair<int,int> > best = MP(-1, MP(-1, -1));
			each(it, candidates){
				int sum = 0;
				int cnt = 0;
				rep(i, 6){
					rep(j, sz(dat[it->F][it->S][i])){
						cnt++;
						sum += sz(follower[dat[it->F][it->S][i][j]]);
					}
				}
				if(cnt == candidates_sum[it->F][it->S]){
					best = MP(sum, *it);
					break;
				}
				best = max(best, MP(sum, *it));
			}

			vector<int> next;
			int xy = best.S.F;
			int cur_dir = best.S.S;
			while(true){
				int t = -1;
				if(sz(dat[xy][cur_dir][3])){
					candidates_sum[xy][cur_dir]--;
					t = dat[xy][cur_dir][3].back();
					dat[xy][cur_dir][3].pop_back();
				}else if(sz(dat[xy][cur_dir][2])){
					pair<int,int> best = MP(-1, -1);
					rep(i, sz(dat[xy][cur_dir][2])){
						int dist = -1;
						t = dat[xy][cur_dir][2][i];
						if(cur_dir == 0 || cur_dir == 2) dist = abs(A[e.ai].answer_from[t].F - A[e.ai].answer_dest[t].F);
						else dist = abs(A[e.ai].answer_from[t].S - A[e.ai].answer_dest[t].F);
						best = max(best, MP(dist, t));
					}
					candidates_sum[xy][cur_dir]--;
					t = best.S;
					dat[xy][cur_dir][2].erase(find(all(dat[xy][cur_dir][2]), t));
				}
				if(t == -1) break;
				used.pb(t);
				int d = e.checkPath(A[e.ai].answer_from[t], A[e.ai].answer_dest[t], A[e.ai].answer_type[t], true, true);
				assert(d == -1);
				rep(j, sz(follower[t])){
					int target = follower[t][j];
					if(--seal[target] == 0){
						int type = A[e.ai].answer_type[target];
						int rot = type & 3;
						type = type >> 10;
						int next_xy = -1;
						if(rot == 0 || rot == 2) next_xy = A[e.ai].answer_from[target].S;
						else next_xy = A[e.ai].answer_from[target].F;
						dat[next_xy][rot][type].pb(target);
						candidates.insert(MP(next_xy, rot));
					}
				}
			}

			while(sz(dat[xy][cur_dir][1])){
				pair<int,int> best = MP(-1, -1);
				int t = -1;
				rep(i, sz(dat[xy][cur_dir][1])){
					int dist = -1;
					t = dat[xy][cur_dir][1][i];
					if(cur_dir == 0 || cur_dir == 2) dist = abs(A[e.ai].answer_from[t].F - A[e.ai].answer_dest[t].F);
					else dist = abs(A[e.ai].answer_from[t].F - A[e.ai].answer_dest[t].F);
					best = max(best, MP(dist, t));
				}
				t = best.S;
				used.pb(t);
				candidates_sum[xy][cur_dir]--;
				dat[xy][cur_dir][1].erase(find(all(dat[xy][cur_dir][1]), t));
				int d = e.checkPath(A[e.ai].answer_from[t], A[e.ai].answer_dest[t], A[e.ai].answer_type[t], true, true);
				assert(d == -1);
				rep(j, sz(follower[t])){
					int target = follower[t][j];
					if(--seal[target] == 0){
						int type = A[e.ai].answer_type[target];
						int rot = type & 3;
						type = type >> 10;
						int next_xy = -1;
						if(rot == 0 || rot == 2) next_xy = A[e.ai].answer_from[target].S;
						else next_xy = A[e.ai].answer_from[target].F;
						dat[next_xy][rot][type].pb(target);
						candidates.insert(MP(next_xy, rot));
					}
				}
			}

			while(sz(A[e.ai].answer_dat)){
				A[e.ai].addAnswer(A[e.ai].answer_dat.back().F.F, A[e.ai].answer_dat.back().F.S, A[e.ai].answer_dat.back().S);
				A[e.ai].answer_dat.pop_back();
			}
			candidates.erase(MP(xy, cur_dir));
			rep(i, 6)if(sz(dat[xy][cur_dir][i])) candidates.insert(MP(xy, cur_dir));



			vector<int> straight;



			rep(turn, 2){
				if(turn == 1 && sz(straight)) continue;
				element now;
				now.clone(e_dat);



				set<P> will_be_used;
				vector<P> rotated;
				if(turn == 1){
					rep(i, sz(used)) will_be_used.insert(A[now.ai].answer_from[used[i]]);
					each(it, will_be_used) assert(M[now.mi].mp[it->F][it->S] == -1);
					if(cur_dir == 0 || cur_dir == 2){
						rep(i, n){
							if(M[now.mi].mp[i][xy] == -1) continue;
							if(will_be_used.find(MP(i, xy)) != will_be_used.end()) continue;
							if(0 < i && M[now.mi].mp[i][xy - 1] != -1){
								now.rotMapWithPushAns(i, 3);
								rotated.pb(MP(i, 3));
							}else{
								now.rotMapWithPushAns(i, 1);
								rotated.pb(MP(i, 1));
							}
							}
					}else{
						rep(i, n){
							if(M[now.mi].mp[xy][i] == -1) continue;
							if(will_be_used.find(MP(xy, i)) != will_be_used.end()) continue;
							if(0 < i && M[now.mi].mp[xy][i - 1] != -1){
								now.rotMapWithPushAns(i, 2);
								rotated.pb(MP(i, 2));
							}else{
								now.rotMapWithPushAns(i, 0);
								rotated.pb(MP(i, 0));
							}
						}
					}
				}



				bool ok = true;
				rep(i, sz(used)){
					element best;
					best.clone(now);
					int t = used[i];
					int type = A[best.ai].answer_type[t];
					if(type == 0) continue;
					if((type >> 10) == 1){
						straight.pb(t);
						continue;
					}
					assert(sz(straight));
					int d = best.checkPath(A[best.ai].answer_from[t], A[best.ai].answer_dest[t], type, true, true);
					assert(sz(A[best.ai].answer_dat));
					assert(d == -1 && turn == 0);
					if(d == -1){
						ok = false;
						break;
					}
					rep(j, sz(walk_indexes)){
						element cur;
						cur.clone(now);
						if(!cur.walkPath(A[cur.ai].answer_from[t], A[cur.ai].answer_dest[t], walk_indexes[j], sz(used) == 1, 1 - (type & 1))){
							continue;
						}
						if(M[cur.mi].mp[A[cur.ai].answer_dest[t].F][A[cur.ai].answer_dest[t].S] == -1) continue;
						if(sz(A[best.ai].answer_stack) <= sz(A[cur.ai].answer_stack)) continue;
						if(!isSame(best, cur)) continue;
						best.clone(cur);
					}
					now.clone(best);
				}

				if(1){
					rep(i,sz(straight)){
						assert(turn == 1);
						int t = straight[i];
						int d = now.checkPath(A[now.ai].answer_from[t], A[now.ai].answer_dest[t], A[now.ai].answer_type[t], true, true);
						assert(d == -1);
					}
					while(sz(A[now.ai].answer_dat)){
						assert(turn == 1);
						A[now.ai].addAnswer(A[now.ai].answer_dat.back().F.F, A[now.ai].answer_dat.back().F.S, A[now.ai].answer_dat.back().S);
						A[now.ai].answer_dat.pop_back();
					}
				}
				if(!ok) continue;



				rep(i, sz(rotated)) now.rotMapWithPushAns(rotated[i].F, rotated[i].S ^ 2);



				assert(!isSame(now, e) && turn == 0);

				if(sz(A[now.ai].answer_stack) < sz(A[e.ai].answer_stack) && isSame(now, e)){
					cerr<<sz(A[e.ai].answer_stack)<<" -> "<<sz(A[now.ai].answer_stack)<<endl;
					assert(sz(A[e.ai].answer_stack) < sz(A[now.ai].answer_stack));
					cerr<<"OKOKOKOKOKOKOKOKOKOKOKOKO "<<turn<<endl;
					e.clone(now);
				}
			}
		}
*/
    }else{
//		if(final_answer) cerr<<"final"<<endl;
        while(sz(candidates)){
            pair<int,pair<int,int> > best = MP(-1, MP(-1, -1));
            each(it, candidates){
                int sum = 0;
                int cnt = 0;
                rep(i, 6){
                    rep(j, sz(dat[it->F][it->S][i])){
                        cnt++;
                        sum += sz(follower[dat[it->F][it->S][i][j]]);
                    }
                }
                if(cnt == candidates_sum[it->F][it->S]){
                    best = MP(sum, *it);
                    break;
                }
                best = max(best, MP(sum, *it));
            }

            vector<int> next;
            int xy = best.S.F;
            int cur_dir = best.S.S;
            while(true){
                int t = -1;
                if(sz(dat[xy][cur_dir][3])){
                    candidates_sum[xy][cur_dir]--;
                    t = dat[xy][cur_dir][3].back();
                    dat[xy][cur_dir][3].pop_back();
                }else if(sz(dat[xy][cur_dir][5])){
                    pair<int,int> best = MP(-1, -1);
                    rep(i, sz(dat[xy][cur_dir][5])){
                        int dist = -1;
                        t = dat[xy][cur_dir][5][i];
                        if(cur_dir == 0 || cur_dir == 2) dist = min(A[e.ai].answer_from[t].F + 1, n - A[e.ai].answer_from[t].F);
                        else dist = min(A[e.ai].answer_from[t].S + 1, n - A[e.ai].answer_from[t].S);
                        best = max(best, MP(dist, t));
                    }
                    candidates_sum[xy][cur_dir]--;
                    t = best.S;
                    dat[xy][cur_dir][5].erase(find(all(dat[xy][cur_dir][5]), t));
                }else if(sz(dat[xy][cur_dir][2])){
                    pair<int,int> best = MP(-1, -1);
                    rep(i, sz(dat[xy][cur_dir][2])){
                        int dist = -1;
                        t = dat[xy][cur_dir][2][i];
                        if(cur_dir == 0 || cur_dir == 2) dist = abs(A[e.ai].answer_from[t].F - A[e.ai].answer_dest[t].F);
                        else dist = abs(A[e.ai].answer_from[t].S - A[e.ai].answer_dest[t].F);
                        best = max(best, MP(dist, t));
                    }
                    candidates_sum[xy][cur_dir]--;
                    t = best.S;
                    dat[xy][cur_dir][2].erase(find(all(dat[xy][cur_dir][2]), t));
                }else if(sz(dat[xy][cur_dir][4])){
                    candidates_sum[xy][cur_dir]--;
                    t = dat[xy][cur_dir][4].back();
                    dat[xy][cur_dir][4].pop_back();
                }
                if(t == -1) break;
                int d = e.checkPath(A[e.ai].answer_from[t], A[e.ai].answer_dest[t], A[e.ai].answer_type[t], true, true);
                assert(d == -1);
//				if(d == -1) cerr<<A[e.ai].answer_type[t]<<endl;
                rep(j, sz(follower[t])){
                    int target = follower[t][j];
                    if(--seal[target] == 0){
                        int type = A[e.ai].answer_type[target];
                        int rot = type & 3;
                        type = type >> 10;
                        int next_xy = -1;
                        if(rot == 0 || rot == 2) next_xy = A[e.ai].answer_from[target].S;
                        else next_xy = A[e.ai].answer_from[target].F;
                        dat[next_xy][rot][type].pb(target);
                        candidates.insert(MP(next_xy, rot));
                    }
                }
            }

            while(sz(dat[xy][cur_dir][1])){
                pair<int,int> best = MP(-1, -1);
                int t = -1;
                rep(i, sz(dat[xy][cur_dir][1])){
                    int dist = -1;
                    t = dat[xy][cur_dir][1][i];
                    if(cur_dir == 0 || cur_dir == 2) dist = abs(A[e.ai].answer_from[t].F - A[e.ai].answer_dest[t].F);
                    else dist = abs(A[e.ai].answer_from[t].F - A[e.ai].answer_dest[t].F);
                    best = max(best, MP(dist, t));
                }
                t = best.S;
                candidates_sum[xy][cur_dir]--;
                dat[xy][cur_dir][1].erase(find(all(dat[xy][cur_dir][1]), t));
                int d = e.checkPath(A[e.ai].answer_from[t], A[e.ai].answer_dest[t], A[e.ai].answer_type[t], true, true);
                assert(d == -1);
                rep(j, sz(follower[t])){
                    int target = follower[t][j];
                    if(--seal[target] == 0){
                        int type = A[e.ai].answer_type[target];
                        int rot = type & 3;
                        type = type >> 10;
                        int next_xy = -1;
                        if(rot == 0 || rot == 2) next_xy = A[e.ai].answer_from[target].S;
                        else next_xy = A[e.ai].answer_from[target].F;
                        dat[next_xy][rot][type].pb(target);
                        candidates.insert(MP(next_xy, rot));
                    }
                }
            }

            while(sz(A[e.ai].answer_dat)){
                A[e.ai].addAnswer(A[e.ai].answer_dat.back().F.F, A[e.ai].answer_dat.back().F.S, A[e.ai].answer_dat.back().S);
                A[e.ai].answer_dat.pop_back();
            }
            candidates.erase(MP(xy, cur_dir));
            rep(i, 6)if(sz(dat[xy][cur_dir][i])) candidates.insert(MP(xy, cur_dir));
        }
    }

    vector<string> res;
    vector<P> vec;
    rep(i, sz(A[e.ai].answer_stack)){
        P p0 = A[e.ai].answer_stack[i];
        bool cancel = false;
        if(sz(vec)){
            P p1 = vec.back();
            if(min(p0.X, p1.X) == -1 && max(p0.X, p1.X) == n && p0.Y == p1.Y) cancel = true;
            if(min(p0.Y, p1.Y) == -1 && max(p0.Y, p1.Y) == n && p0.X == p1.X) cancel = true;
        }
        if(cancel) vec.pop_back();
        else vec.pb(p0);
    }


    if(final_answer){
//		rep(i, m)if(A[e.ai].answer_type[i])  cerr<<"type : "<<(A[e.ai].answer_type[i] >> 10)<<endl;
//		T.print("get answer0");
        int dat = sz(vec);

        bool updated = true;

        int loop_cnt = 0;
        while(timer.inTime(TIME_LIMIT)){
            loop_cnt++;
            updated = false;
            vector<P> next;
            rep(i, sz(vec)){
                bool cancel = false;
                if(sz(next)){
                    P p0 = next.back();
                    P p1 = vec[i];
                    if(min(p0.X, p1.X) == -1 && max(p0.X, p1.X) == n && p0.Y == p1.Y) cancel = true;
                    if(min(p0.Y, p1.Y) == -1 && max(p0.Y, p1.Y) == n && p0.X == p1.X) cancel = true;
                }
                if(cancel){
//					cerr<<"cacel2"<<endl;
                    next.pop_back();
                    updated = true;
                }else{
                    next.pb(vec[i]);
                }
            }
            vec = next;


            for(int target = sz(vec) - 1; 0 <= target && timer.inTime(TIME_LIMIT); target--){
                element cur;
                cur.clone(e);
                M[cur.mi].cloneBoard();
                next.clear();
                rep(i, sz(vec))if(i != target) next.pb(vec[i]);
                rep(i, sz(next)) M[cur.mi].rotMapXY(next[i]);
                M[cur.mi].union_init();
                if(M[cur.mi].max_group_element_num == m){
//					cerr<<"remove1"<<endl;
                    vec = next;
                    updated = true;
                }
            }


            if(!updated){

                for(int i = 0; i < sz(vec) && timer.inTime(TIME_LIMIT); i++){
                    int t = i;
                    for(int j = i + 1; j < sz(vec); j++){
                        if(vec[t] == vec[j]){
                            t = j;
                            continue;
                        }
                        bool cancel = false;
                        P p0 = vec[j];
                        P p1 = vec[t];
                        if(min(p0.X, p1.X) == -1 && max(p0.X, p1.X) == n && p0.Y == p1.Y) cancel = true;
                        if(min(p0.Y, p1.Y) == -1 && max(p0.Y, p1.Y) == n && p0.X == p1.X) cancel = true;
                        if(!cancel) continue;
                        next.clear();
                        rep(k, sz(vec))if(k != j && k != t) next.pb(vec[k]);
                        element cur;
                        cur.clone(e);
                        M[cur.mi].cloneBoard();
                        rep(j, sz(next)) M[cur.mi].rotMapXY(next[j]);
                        M[cur.mi].union_init();
                        if(M[cur.mi].max_group_element_num == m){
//							cerr<<"cancel separates"<<endl;
                            vec = next;
                            updated = true;
                            i--;
                        }
                    }
                }
            }

            if(!updated && timer.inTime(TIME_LIMIT)){
                element cur;
                cur.clone(e);
                M[cur.mi].cloneBoard();
                next.clear();
                for(int i = 0; i + 1 < sz(vec) && timer.inTime(TIME_LIMIT); i++){
                    next.pb(vec[i]);
                    M[cur.mi].rotMapXY(next[i]);
                    M[cur.mi].union_init();
                    if(M[cur.mi].max_group_element_num == m){
//						cerr<<"remove tail"<<endl;
                        vec = next;
                        updated = true;
                        break;
                    }
                }
            }
        }
//		cerr<<"last remove : "<<dat<<" -> "<<sz(vec)<<endl;
//		cerr<<"remove loop : "<<loop_cnt<<endl;


        rep(i, sz(vec)){
            stringstream ss;
            ss << vec[i].X << " " << vec[i].Y;
            res.pb(ss.str());
        }
        return res;
    }
    return vector<string>(sz(vec));
}




Processor::Processor()
{
    collapsed = false;
}







int greedy_candidated_mp[MAX][MAX];
int greedy_used[MAX][MAX];
int greedy_dist[MAX][MAX];
typedef pair<P,pair<P,int> > ANS_P;
vector<ANS_P> greedy_candidate[MAX*4];

void Processor::greedy()
{
#define GREEDY_CHECK_LIMIT { if(!timer.inTime(GENERATE_LIMIT)){ collapsed = true; return; } }
    MAP &mp = M[e.mi];
    ANSWER &ans = A[e.ai];

    collapsed = false;
    e.restore();


    mp.union_init();
    set<P> rem;
    vector<P> used_vec;
    int answer_index = 0;
    rep(i, n) memset(greedy_candidated_mp[i], false, sizeof(int) * n);
    rep(i, n) memset(greedy_used[i], false, sizeof(int) * n);

    rep(i, n)rep(j, n){
            if(mp.mp[i][j] == -1) continue;
            if(mp.union_find(mp.mp[i][j]) != mp.center_pr){
                rem.insert(MP(i, j));
                greedy_dist[i][j] = bit(25);
            }else{
                used_vec.pb(MP(i, j));
                greedy_used[i][j] = true;
                greedy_candidated_mp[i][j] = true;
                ans.setAnswer(MP(i, j), MP(i, j), 0, answer_index++);
            }
        }

    GREEDY_CHECK_LIMIT;

    set<int> candidate_dist;
    rep(used_index, sz(used_vec)){
        GREEDY_CHECK_LIMIT;
        rep(dir, 4){
            int x = used_vec[used_index].F + dx[dir];
            int y = used_vec[used_index].S + dy[dir];
            if(x < 0 || n <= x) continue;
            if(y < 0 || n <= y) continue;
            if(greedy_candidated_mp[x][y]) continue;
            greedy_candidated_mp[x][y] = true;
            each(it,rem){
                vector<int> indexes = getPathIndexCandidates(*it, MP(x, y), greedy_dist[it->F][it->S]);
                rep(i, sz(indexes)){
                    int t = e.checkPath(*it, MP(x, y), indexes[i], false, false);
                    if(t == -1) continue;
                    greedy_dist[it->F][it->S] = min(greedy_dist[it->F][it->S], t);
                    if(sz(greedy_candidate[t]) == 0) candidate_dist.insert(t);
                    greedy_candidate[t].pb(MP(*it, MP(MP(x, y), indexes[i])));
                }
            }
        }
    }

//	cerr<<"rem : "<<sz(rem)<<endl;

    while(sz(candidate_dist) && sz(rem)){
        GREEDY_CHECK_LIMIT;
        int dist = *(candidate_dist.begin());
        P p = greedy_candidate[dist].back().S.F;
        P pos = greedy_candidate[dist].back().F;
        int path_index = greedy_candidate[dist].back().S.S;
        greedy_candidate[dist].pop_back();
        if(sz(greedy_candidate[dist]) == 0) candidate_dist.erase(dist);
        if(greedy_used[p.F][p.S]) continue;
        if(rem.find(pos) == rem.end()) continue;
        int t = e.checkPath(pos, p, path_index, false, false);
        assert(dist != t && t != -1);
        if(t == -1) continue;
        e.checkPath(pos, p, path_index, false, true);
        ans.setAnswer(pos, p, path_index, answer_index++);
        rem.erase(pos);
        greedy_used[p.F][p.S] = true;

        rep(dir, 4){
            int x = p.F + dx[dir];
            int y = p.S + dy[dir];
            if(x < 0 || n <= x) continue;
            if(y < 0 || n <= y) continue;
            if(greedy_candidated_mp[x][y]) continue;
            greedy_candidated_mp[x][y] = true;

            each(it,rem){

                greedy_dist[it->F][it->S] = max(greedy_dist[it->F][it->S], dist);
                vector<int> indexes = getPathIndexCandidates(*it, MP(x, y), greedy_dist[it->F][it->S]);
                rep(i, sz(indexes)){
                    int t = e.checkPath(*it, MP(x, y), indexes[i], false, false);
                    if(t == -1) continue;
                    greedy_dist[it->F][it->S] = min(greedy_dist[it->F][it->S], t);
                    if(sz(greedy_candidate[t]) == 0) candidate_dist.insert(t);
                    greedy_candidate[t].pb(MP(*it, MP(MP(x, y), indexes[i])));
                }

            }
        }
    }

    each(it, candidate_dist) greedy_candidate[*it].clear();


    if(sz(rem)){
        collapsed = true;
        return;
//		assert(true);
    }
}



inline void Processor::marking(int index, int x, int y, int rot)
{
    P p = Derot(MP(x, y), rot);
    if(M[e.mi].mp[p.F][p.S] != -1){
        follower[M[e.mi].mp[p.F][p.S]].pb(index);
        seal[index]++;
    }
    following_mp[p.F][p.S].pb(index);
}



void Processor::analyze(int index, P from, P dest, int type)
{
    rep(i, sz(following_mp[dest.F][dest.S])){
        follower[following_mp[dest.F][dest.S][i]].pb(index);
        seal[index]++;
    }

    int sub_type = type & 1023;
    int rot = sub_type & 3;
    sub_type = sub_type >> 2;
    type = type >> 10;


    if(type == 0) return;
    if(type == 1){ // straight
        from = Rot(from, rot);
        dest = Rot(dest, rot);
        marking(index, dest.F, dest.S, rot);
        marking(index, n - (dest.F - from.F), dest.S, rot);
        if(sub_type == 0){ // left avoid, none around
            marking(index, dest.F, dest.S - 1, rot);
            marking(index, dest.F, n - 1, rot);
            return;
        }else if(sub_type == 1){ // right avoid, none around
            marking(index, dest.F, dest.S + 1, rot);
            marking(index, dest.F, 0, rot);
            return;
        }
        assert(true);
    }else if(type == 2){ // L
        from = Rot(from, rot);
        dest = Rot(dest, rot);
        marking(index, dest.F, dest.S, rot);
        int tx = (n - (dest.F - from.F)) % (n + 1);
        int ty = (n - (dest.S - from.S)) % (n + 1);
        marking(index, tx, from.S, rot);
        marking(index, dest.F, ty, rot);
        bool avoid = sub_type & 1;
        if(avoid){
            marking(index, 0, dest.S, rot);
            marking(index, dest.F + 1, dest.S, rot);
        }else{
            marking(index, n - 1, dest.S, rot);
            marking(index, dest.F - 1, dest.S, rot);
        }
        return;
        /*
            from = Rot(from, rot);
            dest = Rot(dest, rot);
            marking(index, dest.F, dest.S, rot);
            marking(index, n - (dest.F - from.F), from.S, rot);
            if(sub_type & 1){ // left turn
                marking(index, dest.F, n - (dest.S - from.S), rot);
                if(sub_type & 2){ // left avoid, none around
                    marking(index, 0, dest.S, rot);
                    marking(index, dest.F + 1, dest.S, rot);
                    return;
                }else{ // right avoid, none around
                    marking(index, n - 1, dest.S, rot);
                    marking(index, dest.F - 1, dest.S, rot);
                    return;
                }
            }else{ // right turn
                marking(index, dest.F, (from.S - dest.S - 1), rot);
                if(sub_type & 2){ // left avoid, none around
                    marking(index, 0, dest.S, rot);
                    marking(index, dest.F + 1, dest.S, rot);
                    return;
                }else{ // left avoid, none around
                    marking(index, n - 1, dest.S, rot);
                    marking(index, dest.F - 1, dest.S, rot);
                    return;
                }
            }
            */



    }else if(type == 3){
        int first_dir = rot;
        int dist = -1;
        rot = sub_type & 3;
        sub_type = sub_type >> 2;
        from = Rot(from, rot);
        dest = Rot(dest, rot);
        first_dir = (first_dir + rot) & 3;

        marking(index, dest.F, dest.S, rot);
        marking(index, n - 1 - dest.F, dest.S, rot);
        if(sub_type & 1){ // left avoid
            marking(index, dest.F, n - 1, rot);
            marking(index, dest.F, dest.S - 1, rot);
            if(first_dir == 0 || first_dir == 2){
                int ok = 0;
                if(dest.S - 1 != from.S) ok |= 1;
                if(from.S + 1 != n) ok |= 2;
                int x = (from.F + (dest.F + 1)) % (n + 1);
                if(ok != 3 && x != n) marking(index, x, from.S, rot);
            }else{
                if(from.F == n - 1 - dest.F){
                    int y = (dest.S + (from.S + 1) + (n + 1)) % (n + 1);
                    if(y != n) marking(index, from.F, y, rot);
                }
            }
            return;
        }else{ // right avoid
            marking(index, dest.F, 0, rot);
            marking(index, dest.F, dest.S + 1, rot);
            if(first_dir == 0 || first_dir == 2){
                int ok = 0;
                if(dest.S + 1 != from.S) ok |= 1;
                if(from.S != 0) ok |= 2;
                int x = (from.F + (dest.F + 1)) % (n + 1);
                if(ok != 3 && x != n) marking(index, x, from.S, rot);
            }else{
                if(from.F == n - 1 - dest.F){
                    int y = (dest.S + (from.S + 1) + (n + 1)) % (n + 1);
                    if(y != n) marking(index, from.F, y, rot);
                }
            }
            return;
        }
    }else if(type == 4){
        from = Rot(from, rot);
        dest = Rot(dest, rot);
        marking(index, dest.F, dest.S, rot);
        marking(index, dest.F, from.S, rot);
        marking(index, (n - (dest.F - from.F)) % (n + 1), from.S, rot);
        marking(index, dest.F, (n - (from.S - dest.S)) % (n + 1), rot);
        return;
    }else if(type == 5){
        from = Rot(from, rot);
        dest = Rot(dest, rot);
        marking(index, dest.F, dest.S, rot);
        if(sub_type & 1){
        }else{
            marking(index, dest.X, from.Y, rot);
            int ty = (dest.S + 1 + from.S) % (n + 1);
            int tx = (dest.F + 1 + from.F) % (n + 1);
            if(ty != n) marking(index, dest.F, ty, rot);
            if(tx != n) marking(index, tx, from.Y, rot);
        }
        return;
    }
    assert(true);
}


///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////


class BlackAndWhiteGame
{
    void init(vector<string> &board);
public:
    vector<string> makeConnected(vector<string> board);
};

void BlackAndWhiteGame::init(vector<string> &board)
{
    ::init(board);
}

vector<string> BlackAndWhiteGame::makeConnected(vector<string> board)
{
    timer.reset();
    init(board);
    Processor process;
    int best_score = 0;
    vector<vector<pair<int,int> > > best_rots;
    int lengths[] ={1, 2, 3, 4, 1, 5, 6, 1};
    int mp_dist[4][MAX][MAX];

    union_init_rand_mode = true;

    double remain_time = timer.getRemain(GENERATE_LIMIT);
    for(int turn = 0; timer.inTime(GENERATE_LIMIT); turn++){
        bool is_child = (timer.getRemain(GENERATE_LIMIT) < remain_time * 0.15) && (15 < turn);
        double ratio = 0.7 + 0.3 * (XorShift::rand() % bit(25)) / (double) bit(25);
        Timer T;
        Processor now;
        M[now.e.mi].cloneBoard();
        M[now.e.mi].union_init();
        vector<vector<pair<int,int> > > cur_rots;
//		#define ESTIMATE(process) ((turn < 10) ? (M[process.e.mi].estimate() + sz(A[process.e.ai].answer_stack)) : 0)
//		int now_est = ESTIMATE(now);
        int lengths_length = 5;
        if(n < 40) lengths_length += XorShift::rand() % 4;
        else if(n < 70) lengths_length += XorShift::rand() % 3;
        else lengths_length += XorShift::rand() % 2;
        for(int _len = 0; _len < lengths_length && timer.inTime(GENERATE_LIMIT); _len++){
            int len = lengths[_len];
            vector<vector<pair<int,int> > > vec;
/*
			if(150 < best_score){
				rep(i, 4)rep(j, n){
					if(i == 0){
						for(int k = n - 1; 0 <= k; k--){
							mp_dist[i][k][j] = mp_dist[i][k + 1][j] + 1;
							if(k + 1 == n) mp_dist[i][k][j] = 0;
							if(M[now.e.mi].union_find(M[now.e.mi].mp[k][j]) == M[now.e.mi].center_pr) mp_dist[i][k][j] = 0;
						}
					}else if(i == 1){
						for(int k = n - 1; 0 <= k; k--){
							mp_dist[i][j][k] = mp_dist[i][j][k + 1] + 1;
							if(k + 1 == n) mp_dist[i][j][k] = 0;
							if(M[now.e.mi].union_find(M[now.e.mi].mp[j][k]) == M[now.e.mi].center_pr) mp_dist[i][j][k] = 0;
						}
					}else if(i == 2){
						for(int k = 0; k < n; k++){
							if(k == 0) mp_dist[i][k][j] = 0;
							else mp_dist[i][k][j] = mp_dist[i][k - 1][j] + 1;
							if(M[now.e.mi].union_find(M[now.e.mi].mp[k][j]) == M[now.e.mi].center_pr) mp_dist[i][k][j] = 0;
						}
					}else if(i == 3){
						for(int k = 0; k < n; k++){
							if(k == 0) mp_dist[i][j][k] = 0;
							else mp_dist[i][j][k] = mp_dist[i][j][k - 1] + 1;
							if(M[now.e.mi].union_find(M[now.e.mi].mp[j][k]) == M[now.e.mi].center_pr) mp_dist[i][j][k] = 0;
						}
					}
				}
				rep(i, 4)rep(j, n){
					bool ok = true;
					bool exist = false;
					rep(k, n){
						if(i == 0 || i == 2){
							if(M[now.e.mi].mp[j][k] == -1) continue;
							exist = true;
							if(mp_dist[i ^ 0][j][k] < mp_dist[i ^ 2][j][k]){
								ok = false;
								break;
							}
						}else{
							if(M[now.e.mi].mp[k][j] == -1) continue;
							exist = true;
							if(mp_dist[i ^ 0][k][j] < mp_dist[i ^ 2][k][j]){
								ok = false;
								break;
							}
						}
					}
					if(ok && exist) vec.pb(vector<pair<int,int> >(len, MP(i, j)));
				}
				rep(i, sz(vec)) swap(vec[i], vec[XorShift::rand() % (i + 1)]);
			}
*/
            if(_len <= 1 && is_child){
                rep(i, sz(best_rots)){
                    if(ratio < (XorShift::rand() % bit(25)) / (double) bit(25)) continue;
                    vec.pb(best_rots[i]);
                }
            }
            rep(i, 4)rep(j, n){
                    vector<pair<int,int> > v;
                    rep(k, len) v.pb(MP((i + turn / 2) % 4, j));
                    vec.pb(v);
                }
            rep(i, sz(vec))  swap(vec[i], vec[XorShift::rand() % (i + 1)]);
            int cnt[] ={0, 0, 0, 0};
            for(int i = 0; i < sz(vec) && timer.inTime(GENERATE_LIMIT); i++){
                Processor cur;
                cur.e.clone(now.e);
                int len = sz(vec[i]);
                rep(j, len){
                    int dir  =vec[i][j].F;
                    int xy = vec[i][j].S;
                    int r = cur.e.rotMapWithPushAns(xy, dir);
                    if(r == -1) break;

                    if(r == 0){
                        if(n < 50){
                            int pr = M[now.e.mi].center_pr;
                            rep(k, n)if(XorShift::rand() % 3 == 0){
                                    int dir = -1;
#define CHECK_CAN_ROT(x0, y0, x1, y1) \
									( M[cur.e.mi].mp[x0][y0] != -1 && \
									  M[now.e.mi].union_find(M[cur.e.mi].mp[x0][y0]) == pr && \
									  M[cur.e.mi].mp[x1][y1] == -1)
                                    if(k     != 0 && CHECK_CAN_ROT(0, k - 1, n - 1, k)) dir = 0;
                                    if(k + 1 != n && CHECK_CAN_ROT(0, k + 1, n - 1, k)) dir = 0;
                                    if(k     != 0 && CHECK_CAN_ROT(k - 1, 0, k, n - 1)) dir = 1;
                                    if(k + 1 != n && CHECK_CAN_ROT(k + 1, 0, k, n - 1)) dir = 1;
                                    if(k     != 0 && CHECK_CAN_ROT(n - 1, k - 1, 0, k)) dir = 2;
                                    if(k + 1 != n && CHECK_CAN_ROT(n - 1, k + 1, 0, k)) dir = 2;
                                    if(k     != 0 && CHECK_CAN_ROT(k - 1, n - 1, k, 0)) dir = 3;
                                    if(k + 1 != n && CHECK_CAN_ROT(k + 1, n - 1, k, 0)) dir = 3;
                                    if(dir != -1){
                                        vec[i].pb(MP(dir, k));
                                        swap(vec[i].back(), vec[i][j + 1]);
                                        break;
                                    }
                                }
                        }
                        continue;
                    }





                    bool adj = false;
                    {
                        int pr = M[now.e.mi].center_pr;
                        if(dir == 0 || dir == 2){
                            if(0 < xy){
                                for(int x = 0; x < n && !adj; x++){
                                    int mp0 = M[cur.e.mi].mp[x][xy];
                                    int mp1 = M[cur.e.mi].mp[x][xy - 1];
                                    if(mp0 == -1) continue;
                                    if(mp1 == -1) continue;
                                    if(M[now.e.mi].union_find(mp0) == pr){
                                        if(M[now.e.mi].union_find(mp1) != pr) adj = true;
                                    }else{
                                        if(M[now.e.mi].union_find(mp1) == pr) adj = true;
                                    }
                                }
                            }
                            if(xy + 1 < n){
                                for(int x = 0; x < n && !adj; x++){
                                    int mp0 = M[cur.e.mi].mp[x][xy];
                                    int mp1 = M[cur.e.mi].mp[x][xy + 1];
                                    if(mp0 == -1) continue;
                                    if(mp1 == -1) continue;
                                    if(M[now.e.mi].union_find(mp0) == pr){
                                        if(M[now.e.mi].union_find(mp1) != pr) adj = true;
                                    }else{
                                        if(M[now.e.mi].union_find(mp1) == pr) adj = true;
                                    }
                                }
                            }
                        }else{
                            if(0 < xy){
                                for(int y = 0; y < n && !adj; y++){
                                    int mp0 = M[cur.e.mi].mp[xy][y];
                                    int mp1 = M[cur.e.mi].mp[xy - 1][y];
                                    if(mp0 == -1) continue;
                                    if(mp1 == -1) continue;
                                    if(M[now.e.mi].union_find(mp0) == pr){
                                        if(M[now.e.mi].union_find(mp1) != pr) adj = true;
                                    }else{
                                        if(M[now.e.mi].union_find(mp1) == pr) adj = true;
                                    }
                                }
                            }
                            if(xy + 1 < n){
                                for(int y = 0; y < n && !adj; y++){
                                    int mp0 = M[cur.e.mi].mp[xy][y];
                                    int mp1 = M[cur.e.mi].mp[xy + 1][y];
                                    if(mp0 == -1) continue;
                                    if(mp1 == -1) continue;
                                    if(M[now.e.mi].union_find(mp0) == pr){
                                        if(M[now.e.mi].union_find(mp1) != pr) adj = true;
                                    }else{
                                        if(M[now.e.mi].union_find(mp1) == pr) adj = true;
                                    }
                                }
                            }
                        }
                        if(!adj) continue;
                    }


                    M[cur.e.mi].union_init();
                    if(M[now.e.mi].max_group_element_num < M[cur.e.mi].max_group_element_num){
/*
						if(0){
							Processor optional;
							optional.e.clone(now.e);
							set<int> good;
							#define CHECK_FILLED(x, y) \
								(0 <= x && x < n && 0 <= y && y < n && M[cur.e.mi].mp[x][y] != -1 && \
								 M[cur.e.mi].union_find(M[cur.e.mi].mp[x][y]) == M[cur.e.mi].center_pr)
							#define CHECK_FILLED4(x, y) \
								(CHECK_FILLED(x - 1, y) || CHECK_FILLED(x, y - 1) || \
								 CHECK_FILLED(x + 1, y) || CHECK_FILLED(x, y + 1))
							#define CHECK_GOOD(x, y) (M[cur.e.mi].mp[x][y] == -1 && CHECK_FILLED4(x, y))
							if(dir == 0){
								rep(x, n)if(CHECK_GOOD(x, xy)) good.insert(x - j - 1);
								for(int x = n - 1; 0 <= x; x--){
									if(good.find(x) == good.end()) continue;
									if(xy != 0 && M[optional.e.mi].mp[x][xy - 1] != -1){
										optional.e.rotMapWithPushAns(x, 1, 0);
										if(x != 0 && M[optional.e.mi].mp[x - 1][xy] == -1) good.insert(x - 1);
									}else if(xy + 1 != n && M[optional.e.mi].mp[x][xy + 1] != -1){
										optional.e.rotMapWithPushAns(x, 3, 0);
										if(x != 0 && M[optional.e.mi].mp[x - 1][xy] == -1) good.insert(x - 1);
									}
								}
							}else if(dir == 1){
								rep(y, n)if(CHECK_GOOD(xy, y)) good.insert(y - j - 1);
								for(int y = n - 1; 0 <= y; y--){
									if(good.find(y) == good.end()) continue;
									if(xy != 0 && M[optional.e.mi].mp[xy - 1][y] != -1){
										optional.e.rotMapWithPushAns(y, 0, 0);
										if(y != 0 && M[optional.e.mi].mp[xy][y - 1] == -1) good.insert(y - 1);
									}else if(xy + 1 != n && M[optional.e.mi].mp[xy + 1][y] != -1){
										optional.e.rotMapWithPushAns(y, 2, 0);
										if(y != 0 && M[optional.e.mi].mp[xy][y - 1] == -1) good.insert(y - 1);
									}
								}
							}else if(dir == 2){
								rep(x, n)if(CHECK_GOOD(x, xy)) good.insert(x + j + 1);
								rep(x, n){
									if(good.find(x) == good.end()) continue;
									if(xy != 0 && M[optional.e.mi].mp[x][xy - 1] != -1){
										optional.e.rotMapWithPushAns(x, 1, 0);
										if(x + 1 != n && M[optional.e.mi].mp[x + 1][xy] == -1) good.insert(x + 1);
									}else if(xy + 1 != n && M[optional.e.mi].mp[x][xy + 1] != -1){
										optional.e.rotMapWithPushAns(x, 3, 0);
										if(x + 1 != n && M[optional.e.mi].mp[x + 1][xy] == -1) good.insert(x + 1);
									}
								}
							}else if(dir == 3){
								rep(y, n)if(CHECK_GOOD(xy, y)) good.insert(y - j - 1);
								rep(y, n){
									if(good.find(y) == good.end()) continue;
									if(xy + 1 != n && M[optional.e.mi].mp[xy + 1][y] != -1){
										optional.e.rotMapWithPushAns(y, 0, 0);
										if(y + 1 != n && M[optional.e.mi].mp[xy][y + 1] == -1) good.insert(y + 1);
									}else if(xy + 1 != n && M[optional.e.mi].mp[xy + 1][y] != -1){
										optional.e.rotMapWithPushAns(y, 2, 0);
										if(y + 1 != n && M[optional.e.mi].mp[xy][y + 1] == -1) good.insert(y + 1);
									}
								}
							}
							rep(k, j + 1) optional.e.rotMapWithPushAns(vec[i][k].S, vec[i][k].F, 0);

							M[optional.e.mi].union_init();
							if(M[now.e.mi].max_group_element_num < M[optional.e.mi].max_group_element_num && M[optional.e.mi].have == -1){
cerr<<M[now.e.mi].max_group_element_num<<" * "<<M[cur.e.mi].max_group_element_num<<" * "<<M[optional.e.mi].max_group_element_num<<endl;
								now.e.clone(optional.e);
								M[now.e.mi].union_init();
							}
						}
TCO12_1 >./test.sh 100 1 -novis solve.cpp > debug.txt
cnt: 100
95.7049111278

*/
                        if(M[now.e.mi].max_group_element_num < M[cur.e.mi].max_group_element_num){
//							int est = ESTIMATE(cur);
//							if(est <= now_est + 5 + XorShift::rand() % (1 + turn)){
                            now.e.clone(cur.e);
                            M[now.e.mi].union_init();
                            cur_rots.pb(vec[i]);
//							}
                        }

                    }
                }
            }
        }

//		T.print("rot     ");
//		cerr<<"group : "<<M[now.e.mi].max_group_element_num<<endl;
//		cerr<<"size :  "<<sz(A[now.e.ai].answer_stack)<<endl;
        now.e.save();
        T.reset();
        M[now.e.mi].union_init();
//		cerr<<M[now.e.mi].max_group_element_num<<" A"<<endl;
        now.greedy();
//		T.print("greedy  ");
//		cerr<<"score : "<<now.getScore()<<endl;
//		cerr<<endl;
        T.reset();

        int score = now.getScore();

//		T.print("getScore");
        if(turn == 0 || score < best_score){
/*
			cerr<<"improve turn : "<<turn<<endl;
			cerr<<"score : "<<score<<endl;
			cerr<<"is child : "<<is_child<<endl;
			cerr<<"ratio    : "<<ratio<<endl;
			cerr<<"union    : "<<M[now.e.mi].max_group_element_num<<endl;
			cerr<<endl;
*/
            process.e.clone(now.e);
            best_score = score;
            best_rots = cur_rots;
        }
    }

//	cerr<<"turn debug : "<<turn_debug<<endl;

    int loop_cnt = 0;

    while(timer.inTime(CLIMB_LIMIT)){
        loop_cnt++;
        Processor cur;
        cur.e.clone(process.e);
        if(!cur.greedyClimbing()) continue;
        int score = cur.getScore();
        if(score <= best_score){
//			if(score < best_score) cerr<<best_score<<" -> "<<score<<endl;
            process.e.clone(cur.e);
            best_score = score;
        }
    }

//	process.complete();

//	cerr<<"loop : "<<loop_cnt<<endl;
    return process.getAnswer();
}







/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////


#ifdef LOCAL

int main(){
	int n;
	cin >> n;
	vector<string> board(n);
	vector<string> ans;
	rep(i, n) cin >> board[i];
	double start_time = clock();
	ans = BlackAndWhiteGame().makeConnected(board);
	cerr << "n : " << n << endl;
	int cnt = 0;
	rep(i, n) cnt += count(all(board[i]), 'X');
	cerr << "m : " << cnt << endl;
	double time = (clock() - start_time) / CLOCKS_PER_SEC;
	if(5.0 < time){
		assert(true);
		cerr<<"connect : "<<__LINE__<<endl;
	}
	fprintf(stderr, "time : %0.20lf\n", (clock() - start_time) / CLOCKS_PER_SEC);
	cerr << "answer : " << sz(ans) << endl;
	cout << sz(ans) << endl;
	rep(i, sz(ans)) cout << ans[i] << endl;
}

#endif





