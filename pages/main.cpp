// fifo.cpp: First in first out page replacement
// Compile: g++ -o fifo fifo.cpp
// Execute: ./fifo
#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<deque>
 
using namespace std;
class Cframe {
public:
    int frameNo; // frame number
    int pageNo; // page number
    int r; // reference bit
    Cframe ( int n, int p ) // constructor
    {
        frameNo = n;
        pageNo = p; // no page loaded at beginning
        r = 0;
    }
};
 
deque <Cframe> Q;
int nFaults = 0;
int page, frame;
int displayMsg()
{ 
    cout <<"\npage " << page <<" is allocated to frame " << frame;
    return 1;
}
void fault ()
{
    nFaults++;
    cout<<"\nTotal page faults = " << nFaults;
}
int search ( const deque<Cframe> &q, int p )
{
    int n = q.size();
    for ( int i = 0; i < n; i++ ){
        if ( q[i].pageNo == p )
            return q[i].frameNo;
    }
    return -1;
}
int main()
{
    int maxFrames;
    cout << "\nEnter max. number of frames allowed in main memory: ";
    cin >> maxFrames;
    int n;
    cout << "\nEnter sequence of page requests (-99 to terminate). ";
    while ( true ) {
        cout << "\nNew page : ";
        cin >> page;
        if ( page == -99 ) 
            break;
        if (( frame = search ( Q, page )) != -1 ) { 
            cout << "\npage "<< page << " already in frame " << frame;
        } else {
            n = Q.size();
            if ( n < maxFrames ) { 
                Cframe aFrame ( n, page ) ;
                Q.push_back ( aFrame );
                frame = aFrame.frameNo;
                displayMsg ();
            } else {
                Cframe aFrame = Q.front();
                Q.pop_front();
                aFrame.pageNo = page;
                Q.push_back ( aFrame );
                frame = aFrame.frameNo;
                displayMsg ();
            } 
            fault();
        }
    } // while
    cout << "\nTotal number of faults: " << nFaults << endl;
    return 0;
}
