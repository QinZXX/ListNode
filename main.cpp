#include<iostream>
#include "List.h"

template <typename T>
void visit(T & e){
    // e=2*e;
    std::cout<<e <<" ";
}

int main(){

    std::cout<<"over!\n";
    List<int> qin;
    for(int i=0;i<11;i++){
        qin.insertAsLast(i);
    }
    qin.traverse(visit);
    std::cout<<std::endl;
    std::cout<<qin.disordered()<<std::endl; // 此时有序，返回的是false 输出为0
    std::cout<<"*************************"<<std::endl;

    qin.reverse();
    qin.traverse(visit);
    std::cout<<std::endl;
    std::cout<<qin.disordered()<<std::endl; // 此时无序，返回的是true 输出为1
    std::cout<<"*************************"<<std::endl;



    std::cout<<"begin to sort:\n";

    qin.sort();
    qin.traverse(visit);
    std::cout<<std::endl;
    std::cout<<qin.disordered()<<std::endl;



    return 0;
}