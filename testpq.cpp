#include <iostream>
#include <queue>
#include <vector>

using namespace std;



int main() {
    priority_queue<int, vector<int>, greater<int> > pq;
    pq.push(10);
    pq.push(30);
    pq.push(20);
    pq.push(5);
    pq.push(1);

    while (pq.size() > 0) {
        cout << pq.top() << " ";
        pq.pop();
    }
    cout << "\n";
    return 0;
}
