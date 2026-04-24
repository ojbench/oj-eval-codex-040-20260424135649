#include <bits/stdc++.h>
#include <memory>
#include <optional>
using namespace std;

template <typename T>
class DoublyLinkedList {
private:
    struct Node {
        T value;
        std::shared_ptr<Node> next;
        std::weak_ptr<Node> prev;
        Node(const T& val) : value(val), next(nullptr) {}
    };
    std::shared_ptr<Node> head;
    std::weak_ptr<Node> tail;
    size_t length;
public:
    class iterator {
        std::shared_ptr<Node> current;
    public:
        iterator(std::shared_ptr<Node> node) : current(std::move(node)) {}
        T& operator*() { return current->value; }
        iterator operator++(int) { iterator tmp=*this; if(current) current=current->next; return tmp; }
        bool operator==(const iterator& other) const { return current==other.current; }
        bool operator!=(const iterator& other) const { return !(*this==other); }
    };
    DoublyLinkedList(): head(nullptr), length(0) {}
    bool empty() const { return length==0; }
    size_t size() const { return length; }
    void push_front(const T& value){ auto n=std::make_shared<Node>(value); n->next=head; if(head){ head->prev = n; } else { tail=n; } head=std::move(n); ++length; }
    void push_back(const T& value){ auto n=std::make_shared<Node>(value); auto t=tail.lock(); if(t){ t->next=n; n->prev=t; tail=n; } else { head=n; tail=n; } ++length; }
    std::optional<T> pop_front(){ if(!head) return std::nullopt; T ret=head->value; auto nh=head->next; if(nh){ nh->prev.reset(); } else { tail.reset(); } head=std::move(nh); --length; return ret; }
    std::optional<T> pop_back(){ if(empty()) return std::nullopt; auto t=tail.lock(); T ret=t->value; auto p=t->prev.lock(); if(p){ p->next.reset(); tail=p; } else { head.reset(); tail.reset(); } --length; return ret; }
    iterator begin(){ return iterator(head);} iterator end(){ return iterator(nullptr);} };

int main(){
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int q; if(!(cin>>q)) return 0; 
    DoublyLinkedList<long long> dl;
    while(q--){
        string op; cin>>op;
        if(op==PF){ long long x; cin>>x; dl.push_front(x); }
        else if(op==PB){ long long x; cin>>x; dl.push_back(x); }
        else if(op==POPF){ auto v=dl.pop_front(); if(v) cout<<*v<<n; else cout<<NULLn; }
        else if(op==POPB){ auto v=dl.pop_back(); if(v) cout<<*v<<n; else cout<<NULLn; }
        else if(op==SIZE){ cout<<dl.size()<<n; }
        else if(op==ITER){ bool first=true; for(auto it=dl.begin(); it!=dl.end(); it++){ if(!first) cout<<' '; first=false; cout<<*it; } cout<<n; }
    }
    return 0; }
