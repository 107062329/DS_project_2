#include<iostream>
#include<fstream>

using namespace std;

ifstream infile("floor.data",ios::in);
ofstream outfile("floor.path",ios::out);


class Pair{
    public:
        Pair(){
            x = 0;
            y = 0;
        }

        void set_pair(int a,int b){
            x = a;
            y = b;
        }

        int get_x(){
            return x;
        }
        int get_y(){
            return y;
        }

        void print(){
            cout<<"("<<x<<","<<y<<")"<<endl;
        }

        void print_ans(){
            outfile<<x<<" "<<y<<endl;
        }
    private:
        int x,y;
};

class mystack{
    public:
        mystack(){
            top = -1;
        }

        void set_stack(int a){
            if(top == -1){
                Stack = new Pair[a];
            }
        }

        void push(int a,int b){
            top++;
            Pair temp;
            temp.set_pair(a,b);
            Stack[top] = temp;
        }

        Pair pop(){
            Pair temp = Stack[top];
            top--;
            return temp;
        }

        bool isempty(){ return top == -1; }

        void print(){
            cout<<"the stack: "<<endl;
            cout<<"top = "<<top<<endl;
            for(int i=0;i<top+1;i++){
                Stack[i].print();
            }
        }


    private:
        Pair* Stack;
        int top;
};

class robot{
    public:
        robot(int a,int b,int c){
            m = a;
            n = b;
            battery = c;
            count_step = 0;
            ans_path = new Pair[m*n*10];
            dist = 0;
            room = new char*[m];
            for(int i=0;i<m;i++){
                room[i] = new char[n];
            }

            search_map = new int*[m];
            for(int i=0;i<m;i++){
                search_map[i] = new int[n];
            }
            for(int i=0;i<m;i++){
                for(int j=0;j<n;j++){
                    search_map[i][j] = 0;
                }
            }
        }

        void set_room(){
            for(int i=0;i<m;i++){
                for(int j=0;j<n;j++){
                    infile>>room[i][j];
                    if(room[i][j] == 'R'){
                        R.set_pair(i,j);
                    }
                }
            }
        }

        void check_data(){
            cout<<"----------------------"<<endl;
            cout<<endl;
            cout<<"m ="<<m<<",n ="<<n<<",battery ="<<battery<<endl;
            cout<<"R = ";
            R.print();
            for(int i=0;i<m;i++){
                for(int j=0;j<n;j++){
                    cout<<room[i][j];
                }
                cout<<endl;
            }
            cout<<endl;
            cout<<"----------------------"<<endl;
        }

        void DFS_search(int a,int b,int step){
            if(room[a][b] == 'R'){
                search_map[a][b] = 0;
            }
            else if(room[a][b] == '0'){
                if(search_map[a][b] > step || search_map[a][b] == 0){
                    search_map[a][b] = step;
                }
                else return ;
            }
            else{
                search_map[a][b] = 0;
                return;
            }

            if(b-1 > -1)DFS_search(a,b-1,step+1);
            if(a+1 < m)DFS_search(a+1,b,step+1);
            if(b+1 < n)DFS_search(a,b+1,step+1);
            if(a-1 > -1)DFS_search(a-1,b,step+1);
        }

        void find_rear(){
            dist = 0;
            for(int i=0;i<m;i++){
                for(int j=0;j<n;j++){
                    if( search_map[i][j] >dist && room[i][j] == '0' ){
                        dist = search_map[i][j];
                        rear.set_pair(i,j);
                    }
                }
            }
        }

        void search_path(){
            search_path(rear.get_x(),rear.get_y(),dist);
        }

        void search_path(int r,int c,int d){
            if(path.isempty() == 1){
                path.set_stack(d+1);
            }
            path.push(r,c);

            if(d > 1){
                if( c-1 >-1 && room[r][c-1] == '0' && search_map[r][c-1] == d-1 )search_path(r,c-1,d-1);
                else if( r+1 <m && room[r+1][c] == '0' && search_map[r+1][c] == d-1 )search_path(r+1,c,d-1);
                else if( c+1 <n && room[r][c+1] == '0' && search_map[r][c+1] == d-1 )search_path(r,c+1,d-1);
                else if( r-1 >-1 && room[r-1][c] == '0' && search_map[r-1][c] == d-1 )search_path(r-1,c,d-1);

                else if( c-1 >-1 && room[r][c-1] != '1' && search_map[r][c-1] == d-1 )search_path(r,c-1,d-1);
                else if( r+1 <m && room[r+1][c] != '1' && search_map[r+1][c] == d-1 )search_path(r+1,c,d-1);
                else if( c+1 <n && room[r][c+1] != '1' && search_map[r][c+1] == d-1 )search_path(r,c+1,d-1);
                else if( r-1 >-1 && room[r-1][c] != '1' && search_map[r-1][c] == d-1 )search_path(r-1,c,d-1);
            }
            else if(d == 1){
                if( c-1 >-1 && room[r][c-1] == 'R' )search_path(r,c-1,d-1);
                else if( r+1 <m && room[r+1][c] == 'R' )search_path(r+1,c,d-1);
                else if( c+1 <n && room[r][c+1] == 'R' )search_path(r,c+1,d-1);
                else if( r-1 >-1 && room[r-1][c] == 'R' )search_path(r-1,c,d-1);
            }
        }

        void go_through(){
            while(path.isempty() != 1){
                Pair temp;
                temp = path.pop();
                //temp.print_ans();
                ans_path[count_step].set_pair(temp.get_x(),temp.get_y());
                count_step++;
                if(room[temp.get_x()][temp.get_y()] == '0')room[temp.get_x()][temp.get_y()] = 'C';
            }
        }

        void go_around(){
            go_around(rear.get_x(),rear.get_y(),dist);
        }

        void go_around(int r,int c,int step){
            if(step != dist){
                //cout<<r<<" "<<c<<endl;
                //cout<<"step = "<<step<<endl;
                ans_path[count_step].set_pair(r,c);
                count_step++;
                if(room[r][c] == '0')room[r][c] = 'C';
            }
            if( c-1 >-1 && room[r][c-1] == '0' && search_map[r][c-1] + step < battery )go_around(r,c-1,step+1);
            else if( r+1 <m && room[r+1][c] == '0' && search_map[r+1][c] + step < battery )go_around(r+1,c,step+1);
            else if( c+1 <n && room[r][c+1] == '0' && search_map[r][c+1] + step < battery )go_around(r,c+1,step+1);
            else if( r-1 >-1 && room[r-1][c] == '0' && search_map[r-1][c] + step < battery )go_around(r-1,c,step+1);
            else{
                //cout<<"around is clean!"<<endl;
                go_back(r,c,1);
            }
        }

        void go_back(int r,int c,bool flag){
            if(flag == 0 && room[r][c] !='R'){
                //cout<<r<<" "<<c<<endl;
                if(room[r][c] == '0')room[r][c] = 'C';
                ans_path[count_step].set_pair(r,c);
                count_step++;
            }
            if(room[r][c] != 'R'){
                if( c-1 >-1 && room[r][c-1] == '0' && search_map[r][c-1] < search_map[r][c] )go_back(r,c-1,0);
                else if( r+1 <m && room[r+1][c] == '0' && search_map[r+1][c] < search_map[r][c] )go_back(r+1,c,0);
                else if( c+1 <n && room[r][c+1] == '0' && search_map[r][c+1] < search_map[r][c] )go_back(r,c+1,0);
                else if( r-1 >-1 && room[r-1][c] == '0' && search_map[r-1][c]< search_map[r][c] )go_back(r-1,c,0);

                else if( c-1 >-1 && room[r][c-1] != '1' && search_map[r][c-1] < search_map[r][c] )go_back(r,c-1,0);
                else if( r+1 <m && room[r+1][c] != '1' && search_map[r+1][c] < search_map[r][c] )go_back(r+1,c,0);
                else if( c+1 <n && room[r][c+1] != '1' && search_map[r][c+1] < search_map[r][c] )go_back(r,c+1,0);
                else if( r-1 >-1 && room[r-1][c] != '1' && search_map[r-1][c] < search_map[r][c] )go_back(r-1,c,0);
            }
        }

        void check_path(){ path.print(); }

        void print_search_map(){
            cout<<"----------------------"<<endl;
            cout<<endl;
            for(int i=0;i<m;i++){
                for(int j=0;j<n;j++){
                    cout<<search_map[i][j];
                }
                cout<<endl;
            }
            cout<<endl;
            rear.print();
            cout<<dist<<endl;
            cout<<endl;
            cout<<"----------------------"<<endl;
        }

        Pair return_R(){return this->R;}

        int return_dist(){ return this->dist; }

        void print_R(){ R.print_ans(); }

        char** room;
        Pair* ans_path;
        int count_step;
    private:
        int m,n;
        int battery;
        Pair R;
        Pair rear;
        int dist;
        mystack path;
        int**  search_map;
};

int main()
{
    int m,n;
    int battery;
    infile>>m>>n>>battery;
    robot Rob(m,n,battery);
    Rob.set_room();

    //Rob.check_data();
    Rob.DFS_search(Rob.return_R().get_x(),Rob.return_R().get_y(),0);

    Rob.find_rear();
    //Rob.print_search_map();

    while(Rob.return_dist() != 0){
        Rob.search_path();
        //Rob.check_path();
        Rob.go_through();
        Rob.go_around();
        //Rob.check_data();
        Rob.find_rear();
    }
    Rob.ans_path[Rob.count_step].set_pair(Rob.return_R().get_x(),Rob.return_R().get_y());
    Rob.count_step++;

    //Rob.check_data();

    outfile<<Rob.count_step<<endl;
    for(int i=0;i<Rob.count_step;i++){
        Rob.ans_path[i].print_ans();
    }

    return 0;
}
