/*
VRATI WRITE NA PRETHODNO STANJE!!!

NADJI BAG U TRIED TO MERGE TWO INCOMPATIBLE NODES!

KORISTI DEBUGGER

RIP
*/


#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<list>
#include<stack>
#include<cstdlib>
#include<stdlib.h>
#include <vector>

using namespace std;


class Node;

typedef pair<Node*, int> pni;

class Heap{
public:
    bool isMaxHeap;
    int size;
    int maxdepth;
    int pop();
    void push(int k);
    bool up(int a, int b){
        return isMaxHeap ^ (b>a);
    }
    void write();
    void merge(Heap* other);
    bool empty(){
        return top().size()==0;
    }
    Node* merge(Node* a, Node* b);
    void updateMaxDepth();
    Heap(bool imh);
    ~Heap();
    Node* phantom;
    list<Node*>& top();
    list<Node*>::iterator min;

};

class Node{
    public:
    Node(int k, Heap* hh, bool creatingPhantom);
    Heap* h;
    bool isMax(){
        return h->isMaxHeap;
    }
    int i;
    int depth;
    list<Node*> sons;
    Node* par;
    
};
Node::Node(int k, Heap* hh, bool creatingPhantom = false){
    i=k;
    depth = 0;
    h = hh;
    if(creatingPhantom) par=nullptr;
    else par = h->phantom;
}
Heap::Heap(bool imh) : isMaxHeap(imh){
    int x = 0;
    phantom = nullptr;
    phantom = new Node(0,this, true);
    min = top().end();
    maxdepth = 0;
}
Heap::~Heap(){
    while(!empty()) pop();
    delete phantom;
}
//Heap::Heap() {;}
list<Node*>& Heap::top()
{
    return phantom->sons;
}

void Heap::updateMaxDepth(){
    maxdepth = 0;
    for(auto node : top()){
        if(node->depth > maxdepth) maxdepth = node->depth;
    }
}

void Heap::merge(Heap* other){
    cout << "Spajam dva heap-a: " << (isMaxHeap ? "MAX" : "MIN") << " sa " << (other->isMaxHeap ? "MAX" : "MIN") << endl;
    while(!other->empty()){
        push(other->pop());
    }
    updateMaxDepth();
    cout << "Zavrsio spajanje heap-ova" << endl;
}


bool upper(Node* a, Node* b){
    return a->h->isMaxHeap ^ b->i > a->i;
}
void Heap::write(){
    cout << "Ispisujem " << (isMaxHeap?"MAX HEAP":"MIN HEAP") << endl;
    stack<pni> s;
    int d = 0;
    for(auto it = top().rbegin();it!=top().rend();it++){
        s.emplace(*it,0);
    }
    while(!s.empty()){
        Node* n = s.top().first;
        d = s.top().second;
        int tabs = maxdepth - n->depth;
        for(int i = 0; i < tabs; i++) cout << '\t';
        cout<<n->i << endl;
        s.pop();
        auto t = n->sons;
        for(auto it = t.rbegin();it!=t.rend();it++) s.emplace(*it,d+1);
    }
}

Node* Heap::merge(Node* a, Node* b){
    cout << "Spajam dva cvora: " << a->i << " i " << b->i << endl;
    if(a->depth != b->depth) throw invalid_argument("tried to merge two incompatible nodes");
    if(upper(b,a)) swap(b,a);
    //a up b
    b->par = a;
    a->sons.push_front(b);
    a->depth++;
    cout << "Spojio cvorove, nova dubina: " << a->depth << endl;
    return a;
}

int Heap::pop(){
    cout << "Pocinjem pop operaciju na " << (isMaxHeap ? "MAX" : "MIN") << " heap-u" << endl;
    if(top().size()==0){
        cout<<"ne moze pop, heap je prazan"<<endl;
        return -12345;
    }
    auto oldmin = min; //pos
    bool flag = true;
    int sol = (*min)->i;
    cout << "Uklanjam element: " << sol << endl;
    for(auto it : (*min)->sons){
        top().insert(min,it);
        it->par = phantom;
    }
    delete *min;
    top().erase(min);
    
    cout << "Pravim strukturu stabla sada, broj root cvorova " << top().size() << endl;
    Node* nmin = nullptr;
    int n = top().size();
    vector<Node*> v(n+3, nullptr);
    for(auto it = top().begin(); it!=top().end(); it++){
        Node* i = *it;
        if(!nmin || up(i->i , (nmin)->i)){
            nmin = i;
        }
        int newdepth = i->depth;
        Node* newnode = i;
        while(v[newdepth]){
            auto oldnode = v[newdepth];
            v[newdepth] = nullptr;
            cout<<"Ispisujem stablo pre merge"<<endl;
            write();
            newnode = merge(newnode, oldnode);
            cout<<"Ispisujem stablo posle merge"<<endl;
            write();
            newdepth = newnode->depth;
        }
        v[newdepth] = newnode;
    }
    cout << "Napravio strukturu stabla, rebuildujem listu na vrhu" << endl;
    top().clear();
    for(auto i : v){
        if(i){
            top().push_front(i);
            if(i == nmin) {
                min = --(top().end());
                flag = false;
            }
        }
    }
    updateMaxDepth();
    if(nmin==nullptr || empty() || flag){
        if(top().size()==0){
            cout<<"heap prazan\n";
        }
        else{
            throw invalid_argument("no min/max element");
        }
    }
    cout << "Pop zavrsen, novi min/max: " << (nmin ? nmin->i : -1) << endl;
    cout<<"!!!!!!dobar write sad:\n";
    write();
    return sol;
    
    //nadji min!
}

void Heap::push(int k){
    cout << "Ubacujem element " << k << " u " << (isMaxHeap ? "MAX" : "MIN") << " heap" << endl;
    Node* nn = new Node(k, this);
    if(empty()){
        this->top().push_back(nn);
        min=this->top().begin();
        cout << "Heap je bio prazan, novi element je min/max" << endl;
    }
    else{
        this->top().insert(min,nn);
        cout << "Dodao element u root listu" << endl;
    }
    updateMaxDepth();
}

Heap * red = new Heap(false);
Heap * blue = new Heap(true);

void merge(Heap* blue, Heap* red){
    if(blue->size >= red->size){
        blue->merge(red);

    }
    else red->merge(blue);
}

void loadAndExecuteCommand(istream& is){
    // try{
    // cout<<endl<<"AAA";
    char c,_; int k;
    is >> c;
    switch(c){
        case 'X': cout << "Izlazim iz programa" << endl; exit(0);
        case 'V' : 
        case 'B' : 
            cout << "Kreiram nove heap-ove" << endl;
            delete red; delete blue; red = new Heap(false); blue = new Heap(true);break;
        case 'T' : cout << "Pop na blue heap" << endl; blue->pop();break;
        case 'P' : cout << "Pop na red heap" << endl; red->pop();break;
        case 'G' : cout << "Ispisujem oba heap-a" << endl; blue->write();red->write();break;
        case 'L' : cout << "Spajam heap-ove" << endl; merge(blue, red);break;
        //ostalo
        default:
            is>>_>>k>>_;
            switch(c){
                case 'R' : 
                    cout << "Uklanjam " << k << " elemenata iz blue heap-a" << endl;
                    for (int i = 0; i < k; i++)
                    {
                        blue->pop();
                    }
                    break;
                case 'D' : blue->push(k);break;
                case 'M' : red->push(k);break;
                default: throw invalid_argument("los karakter\n");
            }
    }
    
    // if(is.eof()) return;
    // is>>_;
    // if(!_) return;
    // if(_!='\n'){
    //     is>>_;

    //     if(_!='\n'){
    //         cout<<"moguca greska\n";
    //         throw invalid_argument("losa linija\n");
    //     }
    // }
    
    
    // }
    // catch(exception e){
    //     cout<<e.what();
    // }
}

void loadFromTextFile(string filename){
    ifstream fs = ifstream(filename);
    while(!fs.eof()){
        loadAndExecuteCommand(fs);
    }
}

string options[] = {"X", "V","T", "P", "G", "L", "D", "M", "R"};

void execute(string x){
    auto ss = stringstream(x+"\n");
    loadAndExecuteCommand(ss);
}

void writeGameMenu(){
    cout<<"0. Zavrsite program"<<endl;
    cout<<"1. Kupite dve vrece"<<endl;
    cout<<"2. Naidjite na putujuceg trgovca oruzjima"<<endl;
    cout<<"3. Naidjite na putujuceg pekara"<<endl;
    cout<<"4. Naidjite na granicni prelaz"<<endl;
    cout<<"5. Naidjite na lopova"<<endl;
    cout<<"6. Pronadjite dijamant "<<endl;
    cout<<"7. Pronadjite mojsanit"<<endl;
    cout<<"8. Naidjite na retkog trgovca"<<endl;
    int n; cin>>n; string x = options[n];
    if(n>=6){
        cout<<"Upisite broj"<<endl;
        int nn; cin>>nn; x+= "[" + to_string(nn) + "]";
    }
    execute(x);
}

void writeInitialMenu(){
    
}

int main(){
    cout<<"0. Biranje radnje preko konzole"<<endl;
    cout<<"1. Ucitavanje preko tekstualnog fajla"<<endl;
    int n; cin>>n; 
    if(!n) while(1){
        writeGameMenu();
    }
    else{
        string x; cout<<"Napisite adresu fajla"<<endl; cin>>x; loadFromTextFile(x);
    }
    return 0;
}