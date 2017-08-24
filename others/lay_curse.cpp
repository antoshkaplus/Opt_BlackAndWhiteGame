#include<vector>
#include<string>
#include<algorithm>
#include<cmath>
#include<utility>
#include<set>
#include<queue>
#include<map>
#include<cstdlib>
#include<cstdio>
#include<cstring>
#include<iostream>
#include<sstream>
#include<climits>
#include<sys/time.h>
using namespace std;

#define REP(i,a,b) for(i=a;i<b;i++)
#define rep(i,n) REP(i,0,n)

#define INF 1000000000
#define EPS 1e-10
#define RAND (myrand()/4294967296.0)
#define IRAND(a,b) ((int)(RAND*((b)-(a)) + (a)))

#define ll long long
#define VI vector<int>
#define VS vector<string>

#define time_scale 1.0


unsigned myrand(){
    static unsigned x=123456789, y=362436069, z=521288629, w=88675121;
    unsigned t;
    t = (x^(x<<11));
    x=y; y=z; z=w;
    w = (w^(w>>19))^(t^(t>>8));
    return w;
}

double time_limit = 9.5;
ll start_time;
ll get_time(){ timeval t; gettimeofday(&t, 0); return t.tv_sec*1000000LL + t.tv_usec; }
double erapsed_time(){ return (get_time()-start_time)/1000000.0/time_scale; }




int n;
VS res_string;
char buff[1000];
int res_size, res[100000][2], newfg;
int now_size, now[100000][2];

int distance(int x1, int y1, int x2, int y2){
    int dx = x1 - x2, dy = y1 - y2;
    if(dx < 0) dx = -dx;
    if(dy < 0) dy = -dy;
    if(dx > n - dx) dx = n - dx;
    if(dy > n - dy) dy = n - dy;
    return dx*dx*dx + dy*dy*dy;
}

int is_ok(VS board){
    int i, j, k, sx, sy, st_size;
    static int st[10000];
    static int dx[4] = {-1, 1, 0, 0}, dy[4] = {0, 0, -1, 1};

    if(board[n][0]=='X') return 0;

    rep(i,n) rep(j,n) if(board[i][j]=='X'){
                st_size = 0;
                st[st_size++] = i*n+j;
                board[i][j] = 'Y';
                while(st_size){
                    i = st[st_size-1] / n;
                    j = st[st_size-1] % n;
                    st_size--;

                    rep(k,4){
                        sx = i + dx[k];
                        sy = j + dy[k];
                        if(sx < 0 || sy < 0 || sx >= n || sy >= n || board[sx][sy]!='X') continue;
                        board[sx][sy] = 'Y';
                        st[st_size++] = sx*n + sy;
                    }
                }

                rep(i,n) rep(j,n) if(board[i][j] == 'X') return 0;
                return 1;
            }

    return 1;
}


int is_ok_limited(VS board, int r1, int r2, int r3){
    int i, j, k, sx, sy, st_size;
    static int st[10000];
    static int dx[4] = {-1, 1, 0, 0}, dy[4] = {0, 0, -1, 1};

    REP(i,r1,r2) rep(j,n) if(board[i][j]=='X'){
                st_size = 0;
                st[st_size++] = i*n+j;
                board[i][j] = 'Y';
                while(st_size){
                    i = st[st_size-1] / n;
                    j = st[st_size-1] % n;
                    st_size--;

                    rep(k,4){
                        sx = i + dx[k];
                        sy = j + dy[k];
                        if(sx < r1 || sy < 0 || sx >= r3 || sy >= n || board[sx][sy]!='X') continue;
                        board[sx][sy] = 'Y';
                        st[st_size++] = sx*n + sy;
                    }
                }

                REP(i,r1,r2) rep(j,n) if(board[i][j] == 'X') return 0;
                return 1;
            }

    return 1;
}


int calc_center_sub(VS &board, int cx, int cy){
    int i, j, m = 0;
    rep(i,n) rep(j,n) if(board[i][j]=='X') m += distance(cx, cy, i, j);
    return m;
}

void calc_center(VS &board, int *cx, int *cy, int *sum){
    int i, j, k, l, m;

    k = INF;
    rep(i,n){
        m = calc_center_sub(board, i, 0);
        if(k > m) k = m, *cx = i;
    }

    k = INF;
    rep(i,n){
        m = calc_center_sub(board, 0, i);
        if(k > m) k = m, *cy = i;
    }

    *sum = calc_center_sub(board, *cx, *cy);
}

VS do_shift(VS &board, int row, int col){
    int i,j,k;
    VS res = board;
    char now;
    now = res[n][0];

    if(row == -1){
        res[n][0] = res[n-1][col];
        for(i=n-1;i;i--) res[i][col] = res[i-1][col];
        res[0][col] = now;
        return res;
    }
    if(row == n){
        res[n][0] = res[0][col];
        for(i=0;i<n-1;i++) res[i][col] = res[i+1][col];
        res[n-1][col] = now;
        return res;
    }
    if(col == -1){
        res[n][0] = res[row][n-1];
        for(i=n-1;i;i--) res[row][i] = res[row][i-1];
        res[row][0] = now;
        return res;
    }
    if(col == n){
        res[n][0] = res[row][0];
        for(i=0;i<n-1;i++) res[row][i] = res[row][i+1];
        res[row][n-1] = now;
        return res;
    }

    fprintf(stderr, "ERROR in do_shift\n");
}


void do_shift_A(VS &res, int row, int col){
    int i,j,k;
    char now;
    now = res[n][0];

    if(row == -1){
        res[n][0] = res[n-1][col];
        for(i=n-1;i;i--) res[i][col] = res[i-1][col];
        res[0][col] = now;
        return;
    }
    if(row == n){
        res[n][0] = res[0][col];
        for(i=0;i<n-1;i++) res[i][col] = res[i+1][col];
        res[n-1][col] = now;
        return;
    }
    if(col == -1){
        res[n][0] = res[row][n-1];
        for(i=n-1;i;i--) res[row][i] = res[row][i-1];
        res[row][0] = now;
        return;
    }
    if(col == n){
        res[n][0] = res[row][0];
        for(i=0;i<n-1;i++) res[row][i] = res[row][i+1];
        res[row][n-1] = now;
        return;;
    }

    fprintf(stderr, "ERROR in do_shift_A : %d %d\n", row, col);
}






void res_renew(void){
    int i;
    if(now_size >= res_size) return;
    res_size = now_size; newfg = 1;
    rep(i,res_size) res[i][0] = now[i][0], res[i][1] = now[i][1];
}

void res_reverse(void){
    int i;
    rep(i,res_size) swap(res[i][0], res[i][1]);
}

void solve1(VS board, int start_row){
    int i, ii, j, k, mn, opt1, opt2, tmp1, tmp2;
    int st = start_row, ed, cnt, st_fg = 0;
    int c[100], cs;
    int loop = 0;

    cnt = 0;
    rep(i,n) rep(j,n) if(board[i][j]=='X') cnt++;
    ed = (cnt+n-1) / n;
    if(n - ed * 2 < start_row-2) return;

    for(i=n+start_row-1;i>=st;i--){
        mn = INF;
        ii = i%n;

        cs = 0;
        rep(j,n) if(board[ii][j]=='X') c[cs++] = j;
        if(cs == 0) continue;

        rep(j, cs+1){
            if(j==0){
                tmp1 = n - c[0];
                if(mn > tmp1) mn = tmp1, opt1 = tmp1, opt2 = 0;
            } else if(j==cs){
                tmp1 = c[cs-1] + 1;
                if(mn > tmp1) mn = tmp1, opt1 = -tmp1, opt2 = 0;
            } else {
                tmp1 = 2*(c[j-1]+1) + n-c[j];
                tmp2 = c[j-1]+1 + 2*(n-c[j]);
                if(mn > tmp1) mn = tmp1, opt1 = -(c[j-1]+1), opt2 = (c[j-1]+1) + n-c[j];
                if(mn > tmp2) mn = tmp2, opt1 = n-c[j], opt2 = -(n-c[j] + c[j-1]+1);
            }
        }

        while(opt1 != 0 || opt2 != 0){
            int fg;
            if(opt1 > 0) fg =  1, opt1--;
            else if(opt1 < 0) fg = -1, opt1++;
            else if(opt2 > 0) fg =  1, opt2--;
            else if(opt2 < 0) fg = -1, opt2++;

            if((loop++)%5==0 || st_fg) if(is_ok(board)) break;

            if(fg==1) now[now_size][0] = ii, now[now_size][1] = -1;
            else      now[now_size][0] = ii, now[now_size][1] =  n;
            do_shift_A(board, now[now_size][0], now[now_size][1]);
            now_size++; if(now_size >= res_size) return;

            if(i-st < 2) st_fg = 1, st = start_row;
            while(board[n][0]=='X'){
                do{
                    if(st>=n) return;
                    if(st_fg || st % 2 == 1){
                        rep(j,n) if(board[st][j]=='.') break;
                        if(j==n){ st++; continue; }
                    } else {
                        rep(j,n) if(board[st][j]=='X') break;
                        if(j!=n){ st++; continue; }
                    }
                    break;
                }while(1);
                now[now_size][0] = st, now[now_size][1] = -1;
                do_shift_A(board, now[now_size][0], now[now_size][1]);
                now_size++; if(now_size >= res_size) return;
            }
        }
        if(is_ok(board)) break;
    }

    if(is_ok(board)) res_renew();
}


void solve2(VS board, int start_row){
    int i, ii, j, k, mn, opt1, opt2, tmp1, tmp2;
    int st = start_row, ed, cnt, st_fg = 0;
    int c[100], cs;
    int loop = 0;
    int dir_fg = IRAND(0,2);
    int add_row = (int)(IRAND(0,8) * RAND);

    for(i=n+start_row-1;i>=st;i--){
        mn = INF;
        ii = i%n;
        if(start_row && i==n+start_row-1) continue;

        cs = 0;
        rep(j,n) if(board[ii][j]=='X') c[cs++] = j;
        if(cs == 0) continue;

        rep(j, cs+1){
            if(j==0){
                tmp1 = n - c[0];
                if(mn > tmp1) mn = tmp1, opt1 = tmp1, opt2 = 0;
            } else if(j==cs){
                tmp1 = c[cs-1] + 1;
                if(mn > tmp1) mn = tmp1, opt1 = -tmp1, opt2 = 0;
            } else {
                tmp1 = 2*(c[j-1]+1) + n-c[j];
                tmp2 = c[j-1]+1 + 2*(n-c[j]);
                if(mn > tmp1) mn = tmp1, opt1 = -(c[j-1]+1), opt2 = (c[j-1]+1) + n-c[j];
                if(mn > tmp2) mn = tmp2, opt1 = n-c[j], opt2 = -(n-c[j] + c[j-1]+1);
            }
        }

        while(opt1 != 0 || opt2 != 0){
            int fg;
            if(opt1 > 0) fg =  1, opt1--;
            else if(opt1 < 0) fg = -1, opt1++;
            else if(opt2 > 0) fg =  1, opt2--;
            else if(opt2 < 0) fg = -1, opt2++;

            if((loop++)%5==0 || st_fg) if(is_ok(board)) break;

            if(fg==1) now[now_size][0] = ii, now[now_size][1] = -1;
            else      now[now_size][0] = ii, now[now_size][1] =  n;
            do_shift_A(board, now[now_size][0], now[now_size][1]);
            now_size++; if(now_size >= res_size) return;

            if(i-st < 2) st_fg = 1, st = start_row;
            while(board[n][0]=='X'){
                do{
                    if(st==n) return;
                    if(st_fg){
                        rep(j,n) if(board[st][j]=='.') break;
                        if(j==n){
                            st++;
                            if(st<n){
                                double cen = 0, num = 0;
                                rep(j,n) if(board[st][j]=='X') cen+=j+0.5, num++;
                                if(num==0) cen += 0.5*n, num++;
                                cen /= num;
                                if(n*RAND < cen) dir_fg = 0; else dir_fg = 1;
                            }
                            continue;
                        }
                    } else {
                        rep(j,n) if(board[st][j]=='X') break;
                        if(j==n) break;
                        if(is_ok_limited(board, start_row, st+1, min(st+1+add_row,n))){
                            st++;
                            if(st<n){
                                double cen = 0, num = 0;
                                rep(j,n) if(board[st][j]=='X') cen+=j+0.5, num++;
                                if(num==0) cen += 0.5*n, num++;
                                cen /= num;
                                if(n*RAND < cen) dir_fg = 0; else dir_fg = 1;
                            }
                            continue;
                        }
                    }
                    break;
                }while(1);
                now[now_size][0] = st;
                if(dir_fg) now[now_size][1] = -1; else now[now_size][1] = n;
                do_shift_A(board, now[now_size][0], now[now_size][1]);
                now_size++; if(now_size >= res_size) return;
            }
        }
        if(is_ok(board)) break;
    }

    if(is_ok(board)) res_renew();
}



int count_mawari(VS &board, int i, int j, int r){
    int ii, jj, k = 0;
    REP(ii,-r,r+1) REP(jj,-r,r+1){
            if(0 <= i+ii && i+ii < n && 0 <= j+jj && j+jj < n && board[i+ii][j+jj]=='X') k++;
        }
    return k;
}

void solve3(VS board){
    int i,j,k;
    int cost, tmp;
    int a, b, c;
    int puta, putb;
    int cnt_r[120], cnt_c[120];
    int loop;
    int mmloop, mmloop_max;
    int pr_aaa, pr_bbb;

    if(n < 30) mmloop_max = 10;
    else if(n < 50) mmloop_max =  5;
    else if(n < 70) mmloop_max =  3;
    else if(n < 80) mmloop_max =  2;
    else            mmloop_max =  1;

    pr_aaa = (int)((1+RAND) * n / 10);
    pr_bbb = (int)((1+RAND) * n / 18 + 2);

    rep(mmloop, mmloop_max){
        loop = IRAND(1, 50/mmloop_max);
        if(mmloop==0) loop = IRAND(1, loop+1);
        for(;;){
            int ok = 0, fff;

            if(loop-- < 0) break;
            fff = IRAND(0,3);

            rep(i,n) cnt_r[i] = cnt_c[i] = 0;
            rep(i,n) rep(j,n) if(board[i][j]=='X') cnt_r[i]++, cnt_c[i]++;

            rep(i,n) if(fff) rep(j,n) if(!ok) if(board[i][j] == 'X'){
                                if(i < pr_bbb || n-1-i < pr_bbb || j < pr_bbb || n-1-j < pr_bbb); else continue;

                                if( count_mawari(board, i, j, 5) > 7 ) continue;
                                if( RAND < 0.3 ) continue;

                                cost = -1;
                                rep(k,n){
                                    if(board[n-1][k] != 'X'){
                                        tmp = count_mawari(board, -1, k, 5);
                                        if(cost < tmp) cost = tmp, puta = -1, putb = k;
                                    }
                                    if(board[0][k] != 'X'){
                                        tmp = count_mawari(board, n, k, 5);
                                        if(cost < tmp) cost = tmp, puta = n, putb = k;
                                    }
                                    if(board[k][n-1] != 'X'){
                                        tmp = count_mawari(board, k, -1, 5);
                                        if(cost < tmp) cost = tmp, puta = k, putb = -1;
                                    }
                                    if(board[k][0] != 'X'){
                                        tmp = count_mawari(board, k, n, 5);
                                        if(cost < tmp) cost = tmp, puta = k, putb = n;
                                    }
                                }

                                c = INF;
                                if(i < c) c = i, a = n, b = j;
                                if(j < c) c = j, a = i, b = n;
                                if(n-1-i < c) c = n-1-i, a = -1, b = j;
                                if(n-1-j < c) c = n-1-j, a = i, b = -1;
                                c++;

                                while(c--){
                                    now[now_size][0] = a, now[now_size][1] = b;
                                    do_shift_A(board, now[now_size][0], now[now_size][1]);
                                    now_size++; if(now_size >= res_size) return;

                                    int chk = 0;
                                    while(board[n][0] == 'X'){
                                        if(chk++ > 5) break;
                                        now[now_size][0] = puta, now[now_size][1] = putb;
                                        do_shift_A(board, now[now_size][0], now[now_size][1]);
                                        now_size++; if(now_size >= res_size) return;
                                    }
                                }

                                ok = 1; break;
                            }
            if(ok) continue;

            rep(i,n) if(!ok) if(cnt_r[i] && cnt_r[i] <= pr_aaa){
                        int fg = 1;
                        if(RAND < 0.3) continue;
                        if(i==n-1 || (i==0 || cnt_r[i-1] > cnt_r[i+1])) fg = -1;
                        if(cnt_r[i+fg]==0) continue;

                        rep(j,n) if(board[i][j]=='X'){
                                if(fg== 1 && board[n-1][j]=='X') break;
                                if(fg==-1 && board[  0][j]=='X') break;
                            }
                        if(j < n) continue;

                        ok = 1;
                        rep(j,n) if(board[i][j]=='X'){
                                if(fg== 1) now[now_size][0] = -1;
                                else       now[now_size][0] = n;
                                now[now_size][1] = j;
                                do_shift_A(board, now[now_size][0], now[now_size][1]);
                                now_size++; if(now_size >= res_size) return;
                            }
                        break;
                    }

            if(!ok) break;
        }

//    fprintf(stderr, "now_size = %d\n",now_size);
//    rep(i,n) fprintf(stderr,"%s\n",board[i].c_str());

        int i_cen, i_w; double i_mx = -1, i_up, i_down, i_cnt = 0;
        i_w = n/10;
        if(n>50) i_w = n/12;
        if(n>70) i_w = n/15;
        if(n>85) i_w = n/18;

        rep(i,n) rep(j,n) if(board[i][j] == 'X') i_cnt += 1;
        rep(i,n){
            i_up = i_down = 0;
            REP(k,1,i_w+1) if(i-k >= 0) rep(j,n) if(board[i-k][j] == 'X') i_up++;
            REP(k,1,i_w+1) if(i+k <  n) rep(j,n) if(board[i+k][j] == 'X') i_down++;
            i_up   += n * i_w / 2.0;
            i_down += n * i_w / 2.0;
            if(i_mx < i_down / i_up) i_mx = i_down / i_up, i_cen = i;
        }

//    fprintf(stderr, "icen %d\n", i_cen);
        int bef = now_size;
        int i_st = i_cen-i_w, i_ed = i_cen+i_w;

        if(n * (n - i_st) < i_cnt) i_st = n/2;
        REP(i, i_st, i_ed+1){
            if(i < 0 || i >= n) continue;
            if(erapsed_time() > 9) return;
            solve2(board, i);
            now_size = bef;
        }
    }
}



void solve4(VS board){
    int i,j,k,mx,opt;
    int per;
    int r1, r2, go;
    int block = 0, cnt;
    double cen[120];
    int inds;
    double sum, sums;

    rep(i,n) rep(j,n) if(board[i][j] == 'X') block++;

    per = n/10;
    if(per > 8) per = 8;
    if(per < 3) per = 3;

    sum = sums = 0;
    rep(i,n){
        inds = 0; cen[i] = -1;
        if(inds < 8) continue;
        cen[i] = 0;
        REP(j,2,inds-2) cen[i] += j;
        cen[i] /= inds-4;
        sum += cen[i]; sums += 1;
    }
    if(sums>=0.5) sum /= sums;

    rep(i,n){
        if(cen[i] < 0) continue;
        go = 0;
        if(cen[i] >= sum+1) go = (int)((cen[i] - sum+0.5)/1.5);
        if(cen[i] <= sum-1) go = -1 * (int)((sum-0.5 - cen[i])/1.5);

        while(go){
            if(go>0) now[now_size][0] =  n, go--;
            else     now[now_size][0] = -1, go++;
            now[now_size][1] = i;
            do_shift_A(board, now[now_size][0], now[now_size][1]);
            now_size++; if(now_size >= res_size) return;
        }
    }

    r1 = 0;
    while(r1 < n){
        r2 = r1 + per;
        if(r2 > n) r2 = n;

        cnt = 0;
        REP(i,r1,r2) rep(j,n) if(board[i][j]=='X') cnt++;
        if(cnt && cnt < 8*n/10 && (double)cnt/(r2-r1)/n < (double)block/n/n*0.5){
            mx = 0;
            REP(i,r1,r2){
                k = 0;
                rep(j,n) if(board[i][j]=='X') k++;
                if(k > mx) mx = k, opt = i;
            }
            REP(i,r1,r2) if(i!=opt) rep(j,n) if(board[i][j] == 'X'){
                            while(board[opt][j]=='X'){
                                now[now_size][0] = opt; now[now_size][1] = -1;
                                do_shift_A(board, now[now_size][0], now[now_size][1]);
                                now_size++; if(now_size >= res_size) return;
                            }
                            go = i - opt;
                            while(go){
                                if(go>0) now[now_size][0] =  n, go--;
                                else     now[now_size][0] = -1, go++;
                                now[now_size][1] = j;
                                do_shift_A(board, now[now_size][0], now[now_size][1]);
                                now_size++; if(now_size >= res_size) return;
                            }
                            while(board[opt][j]=='X'){
                                now[now_size][0] = opt; now[now_size][1] = -1;
                                do_shift_A(board, now[now_size][0], now[now_size][1]);
                                now_size++; if(now_size >= res_size) return;
                            }
                            go = opt - i;
                            while(go){
                                if(go>0) now[now_size][0] =  n, go--;
                                else     now[now_size][0] = -1, go++;
                                now[now_size][1] = j;
                                do_shift_A(board, now[now_size][0], now[now_size][1]);
                                now_size++; if(now_size >= res_size) return;
                            }
                        }
        }

        r1 = r2;
    }

    int bef = now_size;
    rep(i,n){
        if(erapsed_time() > 9) return;
        solve2(board, i);
        now_size = bef;
    }
}


int ab(int a){if(a<0) return -a; return a;}
int ab_cost(int a){if(a==0) return 2; if(a<0) return -a; return a;}
void solve5(VS board){
    int i,j,k,m;
    int x1, x2, y1, y2, area, narea, sx, sy;
    int a, b, c, d, loop, go;
    int all, ok;
    double ratio = 0.6 + RAND*0.4;
    static int sum[120][120];
    static int lisx[12000], lisy[12000], liss;

    rep(i,n+2) sum[0][i] = sum[i][0] = 0;
    rep(i,n) rep(j,n){
            sum[i+1][j+1] = sum[i][j+1] + sum[i+1][j] - sum[i][j];
            if(board[i][j] == 'X') sum[i+1][j+1]++;
        }

    area = 0;
    REP(a,1,n) REP(b,a,n-1) REP(c,1,n) REP(d,c,n-1){
                    narea = (b-a+1)*(d-c+1);
                    if(area >= narea) continue;
                    if(sum[b+1][d+1] - sum[a][d+1] - sum[b+1][c] + sum[a][c] < narea * ratio) continue;
                    area = narea;
                    x1 = a; x2 = b; y1 = c; y2 = d;
                }
    if(area==0) return;

    loop = (int)(n*n*0.2);
    while(loop--){
        if(is_ok(board)) break;

        all = ok = 0;
        REP(i,x1,x2+1){ all++; if(board[i][y1]=='X') ok++; }
        if(y1-1 > 0 && ok > all*ratio) y1--;

        all = ok = 0;
        REP(i,x1,x2+1){ all++; if(board[i][y2]=='X') ok++; }
        if(y2+1 < n-1 && ok > all*ratio) y2++;

        all = ok = 0;
        REP(i,y1,y2+1){ all++; if(board[x1][i]=='X') ok++; }
        if(x1-1 > 0 && ok > all*ratio) x1--;

        all = ok = 0;
        REP(i,y1,y2+1){ all++; if(board[x2][i]=='X') ok++; }
        if(x2+1 < n-1 && ok > all*ratio) x2++;

//    fprintf(stderr, "[%d %d %d %d]\n",x1,y1,x2,y2);

        liss = 0;
        rep(i,n) rep(j,n) if(board[i][j] == 'X'){
                    if(x1<=i && i<=x2 && y1<=j && j<=y2) continue;
                    if(i==  0 && x1==  1 && board[x1][j] == 'X') continue;
                    if(i==n-1 && x2==n-2 && board[x2][j] == 'X') continue;
                    if(j==  0 && y1==  1 && board[i][y1] == 'X') continue;
                    if(j==n-1 && y2==n-2 && board[i][y2] == 'X') continue;
                    lisx[liss] = i; lisy[liss] = j; liss++;
                }

        if(liss==0){
            double ra[4], mn;

            all = ok = 0;
            REP(i,x1,x2+1){ all++; if(board[i][y1]=='X') ok++; }
            ra[0] = ok / (double)all;

            all = ok = 0;
            REP(i,x1,x2+1){ all++; if(board[i][y2]=='X') ok++; }
            ra[1] = ok / (double)all;

            all = ok = 0;
            REP(i,y1,y2+1){ all++; if(board[x1][i]=='X') ok++; }
            ra[2] = ok / (double)all;

            all = ok = 0;
            REP(i,y1,y2+1){ all++; if(board[x2][i]=='X') ok++; }
            ra[3] = ok / (double)all;

            mn = min(min(ra[0],ra[1]),min(ra[2],ra[3]));
            rep(i,4) if(mn+EPS > ra[i]) break;
            if(i==0) y1++;
            if(i==1) y2--;
            if(i==2) x1++;
            if(i==3) x2--;
            continue;

            break;
        }

        k = IRAND(0, liss);
        sx = lisx[k]; sy = lisy[k];
        if(sx < x1){
            k = -1; m = INF;
            REP(j,y1,y2+1) if(board[x1][j]!='X'){
                    if(m > ab_cost(j-sy)) m = ab_cost(j-sy), k = j;
                }
//      fprintf(stderr, "%d %d -> %d %d\n",sx,sy,x1,k);
            if(k==-1) continue;
            if(k==sy){
                now[now_size][0] = sx;
                if(sy!=n-1) now[now_size][1] = -1, sy++; else now[now_size][1] = n, sy--;
                board = do_shift(board, now[now_size][0], now[now_size][1]);
                now_size++; if(now_size >= res_size || now_size >= n*n*0.4) return;
            }

            go = x1 - sx;
            rep(i,(x2-x1+2)/2) if(board[x1+i][k]=='X') break;
            go += i-1;

            rep(i,go){
                now[now_size][0] = n; now[now_size][1] = k;
                do_shift_A(board, now[now_size][0], now[now_size][1]);
                now_size++; if(now_size >= res_size) return;
            }

            while(sy != k){
                now[now_size][0] = sx;
                if(sy < k) now[now_size][1] = -1, sy++;
                if(sy > k) now[now_size][1] =  n, sy--;
                do_shift_A(board, now[now_size][0], now[now_size][1]);
                now_size++; if(now_size >= res_size || now_size >= n*n*0.4) return;
            }

            rep(i,go){
                now[now_size][0] = -1; now[now_size][1] = k;
                do_shift_A(board, now[now_size][0], now[now_size][1]);
                now_size++; if(now_size >= res_size || now_size >= n*n*0.4) return;

                while(i<go-3){
                    for(j=sx+i;j>=0;j--) if(board[j][k]=='.') break;
                    if(0 <= j && j < n); else break;

                    d = INF;
                    rep(a,n) if(board[j][a]=='X'){
                            if(d > ab(a-k)) d = ab(a-k), c = a;
                        }
                    if(d==INF) break;
                    if(d > RAND*RAND*n) break;

                    while(c != k){
                        now[now_size][0] = j;
                        if(c > k) now[now_size][1] =  n, c--;
                        else      now[now_size][1] = -1, c++;
                        do_shift_A(board, now[now_size][0], now[now_size][1]);
                        now_size++; if(now_size >= res_size || now_size >= n*n*0.4) return;
                    }
                }
            }
            continue;
        }


        if(sx > x2){
            k = -1; m = INF;
            REP(j,y1,y2+1) if(board[x2][j]!='X'){
                    if(m > ab_cost(j-sy)) m = ab_cost(j-sy), k = j;
                }
            if(k==-1) continue;
//      fprintf(stderr, "%d %d -> %d %d\n",sx,sy,x2,k);
            if(k==sy){
                now[now_size][0] = sx;
                if(sy!=n-1) now[now_size][1] = -1, sy++; else now[now_size][1] = n, sy--;
                do_shift_A(board, now[now_size][0], now[now_size][1]);
                now_size++; if(now_size >= res_size || now_size >= n*n*0.4) return;
            }

            go = sx - x2;
            rep(i,(x2-x1+2)/2) if(board[x2-i][k]=='X') break;
            go += i-1;

            rep(i,go){
                now[now_size][0] = -1; now[now_size][1] = k;
                do_shift_A(board, now[now_size][0], now[now_size][1]);
                now_size++; if(now_size >= res_size || now_size >= n*n*0.4) return;
            }

            while(sy != k){
                now[now_size][0] = sx;
                if(sy < k) now[now_size][1] = -1, sy++;
                if(sy > k) now[now_size][1] =  n, sy--;
                do_shift_A(board, now[now_size][0], now[now_size][1]);
                now_size++; if(now_size >= res_size || now_size >= n*n*0.4) return;
            }

            rep(i,go){
                now[now_size][0] = n; now[now_size][1] = k;
                board = do_shift(board, now[now_size][0], now[now_size][1]);
                now_size++; if(now_size >= res_size || now_size >= n*n*0.4) return;

                while(i<go-3){
                    for(j=sx-i;j<n;j++) if(board[j][k]=='.') break;
                    if(0 <= j && j < n); else break;

                    d = INF;
                    rep(a,n) if(board[j][a]=='X'){
                            if(d > ab(a-k)) d = ab(a-k), c = a;
                        }
                    if(d==INF) break;
                    if(d > RAND*RAND*n) break;

                    while(c != k){
                        now[now_size][0] = j;
                        if(c > k) now[now_size][1] =  n, c--;
                        else      now[now_size][1] = -1, c++;
                        do_shift_A(board, now[now_size][0], now[now_size][1]);
                        now_size++; if(now_size >= res_size || now_size >= n*n*0.4) return;
                    }
                }
            }

            continue;
        }


        if(sy < y1){
            k = -1; m = INF;
            REP(i,x1,x2+1) if(board[i][y1]!='X'){
                    if(m > ab_cost(i-sx)) m = ab_cost(i-sx), k = i;
                }
            if(k==-1) continue;
//      fprintf(stderr, "%d %d -> %d %d\n",sx,sy,k,y1);
            if(k==sx){
                if(sx!=n-1) now[now_size][0] = -1, sx++; else now[now_size][0] = n, sx--;
                now[now_size][1] = sy;
                do_shift_A(board, now[now_size][0], now[now_size][1]);
                now_size++; if(now_size >= res_size || now_size >= n*n*0.4) return;
            }

            go = y1 - sy;
            rep(i,(y2-y1+2)/2) if(board[k][y1+i]=='X') break;
            go += i-1;

            rep(i,go){
                now[now_size][0] = k; now[now_size][1] = n;
                do_shift_A(board, now[now_size][0], now[now_size][1]);
                now_size++; if(now_size >= res_size || now_size >= n*n*0.4) return;
            }

            while(sx != k){
                if(sx < k) now[now_size][0] = -1, sx++;
                if(sx > k) now[now_size][0] =  n, sx--;
                now[now_size][1] = sy;
                do_shift_A(board, now[now_size][0], now[now_size][1]);
                now_size++; if(now_size >= res_size || now_size >= n*n*0.4) return;
            }

            rep(i,go){
                now[now_size][0] = k; now[now_size][1] = -1;
                do_shift_A(board, now[now_size][0], now[now_size][1]);
                now_size++; if(now_size >= res_size || now_size >= n*n*0.4) return;

                while(i<go-3){
                    for(j=sy+i;j>=0;j--) if(board[k][j]=='.') break;
                    if(0 <= j && j < n); else break;

                    d = INF;
                    rep(a,n) if(board[a][j]=='X'){
                            if(d > ab(a-k)) d = ab(a-k), c = a;
                        }
                    if(d==INF) break;
                    if(d > RAND*RAND*n) break;

                    while(c != k){
                        if(c > k) now[now_size][0] =  n, c--;
                        else      now[now_size][0] = -1, c++;
                        now[now_size][1] = j;
                        board = do_shift(board, now[now_size][0], now[now_size][1]);
                        now_size++; if(now_size >= res_size || now_size >= n*n*0.4) return;
                    }
                }
            }
            continue;
        }


        if(sy > y2){
            k = -1; m = INF;
            REP(i,x1,x2+1) if(board[i][y2]!='X'){
                    if(m > ab_cost(i-sx)) m = ab_cost(i-sx), k = i;
                }
            if(k==-1) continue;
//      fprintf(stderr, "%d %d -> %d %d\n",sx,sy,k,y2);
            if(k==sx){
                if(sx!=n-1) now[now_size][0] = -1, sx++; else now[now_size][0] = n, sx--;
                now[now_size][1] = sy;
                board = do_shift(board, now[now_size][0], now[now_size][1]);
                now_size++; if(now_size >= res_size || now_size >= n*n*0.4) return;
            }

            go = sy - y2;
            rep(i,(y2-y1+2)/2) if(board[k][y2-i]=='X') break;
            go += i-1;

            rep(i,go){
                now[now_size][0] = k; now[now_size][1] = -1;
                board = do_shift(board, now[now_size][0], now[now_size][1]);
                now_size++; if(now_size >= res_size || now_size >= n*n*0.4) return;
            }

            while(sx != k){
                if(sx < k) now[now_size][0] = -1, sx++;
                if(sx > k) now[now_size][0] =  n, sx--;
                now[now_size][1] = sy;
                board = do_shift(board, now[now_size][0], now[now_size][1]);
                now_size++; if(now_size >= res_size || now_size >= n*n*0.4) return;
            }

            rep(i,go){
                now[now_size][0] = k; now[now_size][1] = n;
                board = do_shift(board, now[now_size][0], now[now_size][1]);
                now_size++; if(now_size >= res_size || now_size >= n*n*0.4) return;

                while(i<go-3){
                    for(j=sy-i;j<n;j++) if(board[k][j]=='.') break;
                    if(0 <= j && j < n); else break;

                    d = INF;
                    rep(a,n) if(board[a][j]=='X'){
                            if(d > ab(a-k)) d = ab(a-k), c = a;
                        }
                    if(d==INF) break;
                    if(d > RAND*RAND*n) break;

                    while(c != k){
                        if(c > k) now[now_size][0] =  n, c--;
                        else      now[now_size][0] = -1, c++;
                        now[now_size][1] = j;
                        board = do_shift(board, now[now_size][0], now[now_size][1]);
                        now_size++; if(now_size >= res_size || now_size >= n*n*0.4) return;
                    }
                }
            }
            continue;
        }


    }

    if(is_ok(board)) res_renew();
}








class BlackAndWhiteGame
{
public:
    VS makeConnected(VS board);
};

VS BlackAndWhiteGame::makeConnected(VS board){
    int i, j, k, l, m, fst_score;
    int tate, yoko;
    VS rboard;

    start_time = get_time();
    n = board.size();
    board.push_back((string)".");

    rboard = board;
    rep(i,n) rep(j,n) rboard[i][j] = board[j][i];

    res_size = 10000;
    newfg = 0;

    tate = yoko = 0;
    rep(i,n){
        k = 0;
        rep(j,n) if(board[i][j]=='X') k++;
        yoko += k*k*k;

        k = 0;
        rep(j,n) if(board[j][i]=='X') k++;
        tate += k*k*k;
    }
    k = min(tate, yoko) / 2;
    if(tate > 3*yoko || yoko > 3*tate) k = 2 * min(tate, yoko) / 3;
    yoko -= k; tate -= k;

    rep(i,5){
        now_size = 0;
        solve5(board);
        if(i==2 ) break;
    }
    fst_score = res_size;
    fprintf(stderr, "fst_score = %d, %f\n",res_size,erapsed_time());

    rep(i,n){ break;
        if(erapsed_time() > 3) break;

        now_size = 0;
        solve1(board, i);

        newfg = 0;
        now_size = 0;
        solve1(rboard, i);
        if(newfg) res_reverse(), newfg = 0;
    }

    rep(i,n){ break;
        if(erapsed_time() > 7) break;

        now_size = 0;
        solve2(board, i);

        newfg = 0;
        now_size = 0;
        solve2(rboard, i);
        if(newfg) res_reverse(), newfg = 0;
    }

//  fprintf(stderr, "score (solve2) = %d, %f\n",res_size,erapsed_time());

    if(yoko > tate){
        now_size = 0;
        solve3(board);
    } else {
        newfg = 0;
        now_size = 0;
        solve3(rboard);
        if(newfg) res_reverse(), newfg = 0;
    }

    fprintf(stderr, "score (solve3) = %d, %f\n",res_size,erapsed_time());

/*  now_size = 0;
solve4(board);

newfg = 0;
now_size = 0;
solve4(rboard);
if(newfg) res_reverse(), newfg = 0;*/

//  fprintf(stderr, "score (solve4) = %d, %f\n",res_size,erapsed_time());

    fprintf(stderr, "yoko = %d, tate = %d\n",yoko,tate);
    for(;;){
        if(erapsed_time() > 9) break;
        if(fst_score <= res_size) break;
        if(fst_score +  5*n <= res_size && erapsed_time() > 7) break;
        if(fst_score + 10*n <= res_size && erapsed_time() > 8) break;
        if(RAND * (tate+yoko) < yoko){
            now_size = 0;
            solve3(board);
        } else {
            newfg = 0;
            now_size = 0;
            solve3(rboard);
            if(newfg) res_reverse(), newfg = 0;
        }
    }

    while(erapsed_time() < 9){
        now_size = 0;
        solve5(board);
    }

    fprintf(stderr, "score (loop) = %d, %f\n",res_size,erapsed_time());

    res_string.clear();
    rep(i,res_size) if(res_size < 10000){
            sprintf(buff,"%d %d",res[i][0],res[i][1]);
            res_string.push_back((string)buff);
        }
    return res_string;
}


