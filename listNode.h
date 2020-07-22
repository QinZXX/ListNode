//
// Created by laoqin on 2020/7/17.
//

#ifndef LISTNODE_NODE_H
#define LISTNODE_NODE_H

typedef int Rank; //秩
#define ListNodePosi(T) ListNode<T>* //列表节点位置

template <typename T> struct ListNode { //列表节点模板类（以双向链表形式实现）
    // 成员
    T data;
    ListNodePosi(T) pred;
    ListNodePosi(T) succ; //数值、前驱、后继

    // 构造函数
    ListNode() {} //针对header和trailer的构造
    ListNode( T e, ListNodePosi(T) p = NULL, ListNodePosi(T) s = NULL)
        : data(e), pred(p), succ(s) {} //默认构造器

    // 操作接口
    ListNodePosi(T) insertAsPred(T const& e){ //紧靠当前节点之前插入新节点
        ListNodePosi(T) x= new ListNode(e,pred,this);
        pred->succ=x;
        pred=x;
        return x; //返回新节点位置
    }
    ListNodePosi(T) insertAsSucc(T const& e) { //紧随当前节点之后插入新节点
        ListNodePosi(T) x=new ListNode (e,this,succ);// 创建新节点
        succ ->pred=x;
        succ=x;
        return x;//返回新节点的位置
    }
};

// #include "listNode_implentation.h"
#endif //LISTNODE_NODE_H
