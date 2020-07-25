//
// Created by laoqin on 2020/7/17.
//

#ifndef LISTNODE_LIST_H
#define LISTNODE_LIST_H

#include "listNode.h" //引入列表节点类
#include "QRandom.h" // 随机数（随机选择排序算法）

template <typename T> class List { //列表模板类
private:
    int _size;
    ListNodePosi(T) header;
    ListNodePosi(T) trailer; //规模、头哨兵、尾哨兵

protected:
    void init(); //列表创建初始化
    int clear(); //清除所有节点
    void copyNodes(ListNodePosi(T), int); //复制列表中自位置p起的n项
    void merge(ListNodePosi(T)&, int, ListNodePosi(T), int); //有序列表区间归并
    void mergeSort(ListNodePosi(T)&, int); //对从p开始连续的n个节点归并排序
    void selectionSort(ListNodePosi(T), int); //对从p开始连续的n个节点选择排序
//public:
    void insertionSort(ListNodePosi(T), int); //对从p开始连续的n个节点插入排序
public:
    // 构造函数
    List() { init(); } //默认
    List(List<T> const& L);  //整体复制列表L
    List(List<T> const& L, Rank r, int n);  //复制列表L中自第r项起的n项
    List(ListNodePosi(T) p, int n); //复制列表中自位置p起的n项

    // 析构函数
    ~List(); //释放（包含头、尾哨兵在内的）所有节点

    // 只读访问接口
    Rank size() const { return _size; } //大小
    bool empty() const { return _size <= 0; } //判空
    T& operator[](Rank r) const; //重载，支持循秩访问（效率低）
    ListNodePosi(T) first() const { return header->succ; } //首节点位置
    ListNodePosi(T) last() const { return trailer->pred; } //末节点位置
    bool valid(ListNodePosi(T) p) {//判断位置p是否对外合法
         return p && (trailer != p) && (header != p);
    } //将头、尾节点等同于NULL
    bool disordered() const; //判断列表是否已排序
    ListNodePosi(T) find(T const& e) const   //无序列表查找
    { return find(e, _size, trailer); }
    ListNodePosi(T) find(T const& e, int n, ListNodePosi(T) p) const;  //无序区间查找
    ListNodePosi(T) search(T const& e) const   //有序列表查找
    { return search(e, _size, trailer); }
    ListNodePosi(T) search(T const& e, int n, ListNodePosi(T) p) const;  //有序区间查找
    ListNodePosi(T) selectMax(ListNodePosi(T) p, int n); //在p及其前n-1个后继中选出最大者
    ListNodePosi(T) selectMax() { return selectMax(header->succ, _size); } //整体最大者

    // 可写访问接口
    ListNodePosi(T) insertAsFirst(T const& e);  //将e作首节点揑入
    ListNodePosi(T) insertAsLast(T const& e);  //将e作末节点揑入
    ListNodePosi(T) insertBefore(ListNodePosi(T) p, T const& e);  //将e作p的前驱揑入
    ListNodePosi(T) insertAfter(ListNodePosi(T) p, T const& e);  //将e作p的后继揑入
    T remove(ListNodePosi(T) p); //删除合法位置p处的节点,返回被删除节点
    void merge(List<T>& L) { merge(first(), _size, L, L.first(), L._size); } //全列表归并
    void sort(ListNodePosi(T) p, int n); //列表区间排序
    void sort() { sort(first(), _size); } //列表整体排序
    int deduplicate(); //无序去重
    int uniquify(); //有序去重
    void reverse(); //前后倒置

    // 遍历
    void traverse(void (*)(T&)); //遍历，依次实施visit操作（函数指针，只读或局部性修改）
    template <typename VST> //操作器
    void traverse(VST&); //遍历，依次实施visit操作（函数对象，可全局性修改）
}; //List

template <typename T>
void List<T>::init() { //列表初始化，在创建列表对象时统一调用
    header = new ListNode<T>; //创建头哨兵节点
    trailer = new ListNode<T>; //创建尾哨兵节点
    header->succ = trailer;header->pred = NULL;
    trailer->pred = header;trailer->succ = NULL;
    _size = 0; //记录规模
}
template <typename T> //重载下标操作符，以通过秩直接访问列表节点（虽简便，效率低，需慎用）
T& List<T>::operator[](Rank r) const { //assert: 0 <= r < size
    //todo:越界异常  (内存泄漏？)  or使用异常处理跳过
    //assert(r>=0 && r<=_size-1); // #include<assert.h>
    ListNodePosi(T) p = first(); //从首节点出发
    while (0 < r--) p = p->succ; //顺数第r个节点（首节点为0）
    return p->data; //目标节点，返回其中所存元素
}

template <typename T>//在无序列表内节点p（可能是trailer）的n个（真）前驱中，找出等于e的最后者
ListNodePosi(T) List<T>::find(T const& e, int n, ListNodePosi(T) p) const { //0<=n<=rank(p)<_size
    while (0 < n--) {//对于p的最近的n个前驱，从右向左
        if (e == (p = p->pred)->data)
            return p; //逐个比对，直至命中或范围越界
    }
    //return NULL; //p越出左边界意味着区间内不含e，查找失败
    return nullptr;
} //失败时，返回NULL (nullptr)

template <typename T>
ListNodePosi(T) List<T>::insertAsFirst(T const& e){ //e作首节点插入
    _size++;
    return header->insertAsSucc(e);
}

template <typename T>
ListNodePosi(T) List<T>::insertAsLast(T const& e) {
    _size++;
    return trailer->insertAsPred(e);
}

template <typename T>
ListNodePosi(T) List<T>::insertBefore(ListNodePosi(T) p, T const& e){  //e作p的前驱插入
    _size++;
    return p->insertAsPred(e);
}

template <typename T>
ListNodePosi(T) List<T>::insertAfter(ListNodePosi(T) p, T const& e){  //e作p的后继插入
    _size++;
    return p->insertAsSucc(e);
}

template <typename T> //列表内部方法：复制列表中自位置p起的n项
void List<T>::copyNodes(ListNodePosi(T) p, int n) { //p合法，且至少有n-1个真后继节点
    init(); //创建头、尾哨兵节点并做初始化
    while(n--) { //将起自p的n项依次作为末节点插入
        insertAsLast(p->data);
        p = p->succ;
    }
}
template <typename T> //整体复制构造
List<T>::List(List<T> const& L){
    copyNodes(L.first(),L._size);
}

template <typename T> //assert: p为合法位置，且至少有n-1个后继节点
List<T>::List(ListNodePosi(T) p, int n) {  //复制列表中自位置p起的n项
    copyNodes(p, n);
}

template <typename T> //assert: r+n <= L._size
List<T>::List(List<T> const& L, int r, int n) {  //复制L中自第r顷起的n项
    copyNodes(L[r], n);
}

template <typename T>
T List<T>::remove(ListNodePosi(T) p){ // 删除p节点，并返回节点的值
    T e = p->data;
    p->succ->pred = p->pred;
    p->pred->succ = p->succ;
    delete p;
    --_size;
    // do not forget size
    return e;
}

template <typename T>
List<T>::~List() {//析构函数
    clear();
    delete header;
    delete trailer;
} //清空列表，释放头、尾哨兵节点

template <typename T>
int List<T>::clear(){
    int oldSize=_size;
    while (0 < _size) {
        remove(header->succ); //反复删除首节点，直至列表为空
    }
    return oldSize;
}

template <typename T>
int List<T>::deduplicate() {  //剔除无序列表中的重复节点
    if (_size < 2)  // 0 or 1
        return 0;
    int oldSize = _size;
    ListNodePosi(T) p = header;
    Rank r = 0; //p从首节点开始
    while (trailer != (p = p->succ)) { //依次直到末节点
        ListNodePosi(T) q = find(p->data, r, p); //在p的r个（真）前驱中查找雷同者
        q ? remove(q) : r++; //若的确存在，则删除之；否则秩加一
    } //assert: 循环过程中的任意时刻，p的所有前驱互不相同
    return oldSize - _size; // 被删除元素总数
}
/* 正确性说明： 从前往后，设满足无重复性，p移动到下一个节点，两种情况：
     1.新的节点与前面的节点不重复，继续满足无重复性，此时 r++ ，p移动到下一个节点（下一次就检查前r+1个元素了）。
     2.新的节点与前面的节点有重复（当然只有一个重复），找出该元素位置并且移除，所以r不增加了，p移动到下一个节点
*/


//template <typename T>
//int List<T>::uniquify()  { // 有序列表删除重复节点
//    if (_size < 2)  // 0 or 1
//        return 0;
//    int oldSize=_size;
//    ListNodePosi(T) p=trailer->pred;
//    while((p=p->pred)!=header->succ){
//        ListNodePosi(T) q=p;
//        if (p->pred->data==p->data){
//            remove(q);
//        } // 如果当前和前一个重复 ，删除节点p，并且把节点p指向pred
//    }
//
//    return oldSize-_size;// 返回删除的节点数
//}

template <typename T>
int List<T>::uniquify() { //成批剔除重复元素，效率更高
    if ( _size < 2 ) return 0; //平凡列表自然无重复
    int oldSize = _size; //记录原规模
    ListNodePosi(T) p = header->succ;
    ListNodePosi(T) q; //p为各区段起点，q为其后继
    while ( trailer != ( q = p->succ ) ) //反复考查紧邻的节点对(p, q)
        if ( p->data != q->data ) p = q; //若互异，则转向下一区段
        else remove ( q ); //否则（雷同），删除后者
    return oldSize - _size; //列表规模变化量，即被删除元素总数
}

//遍历
template<typename T>
void List<T>::traverse(void (*visit)(T&)) { // 传递函数指针
    for( ListNodePosi(T) p=header->succ ;p!=trailer;p=p->succ){
        visit(p->data);
    }
}
template<typename T>
template<typename VST>
void List<T>::traverse(VST & visit) { // 传递的一个函数对象
    for(ListNodePosi(T) p=header->succ; p!=trailer;p=p->succ ){
        visit(p->data);
    }
}

template <typename T> // 在有序列表内节点p（可能是trailer）的n个（真）前驱中，找到不大于e的最后者
ListNodePosi(T) List<T>::search(const T &e, int n, ListNode<T> *p) const { // 返回第一个不大于e 的元素的节点指针
    ListNodePosi(T) q=p;
    while( 0<=(n--) ){
        if( (q=(q->pred) )->data<=e) {
            break;
        }
    }
    return q;// 失败时，返回n个前驱的 左边界（即p的第（n+1）个前驱）
}

template <typename T>
void List<T>::sort ( ListNodePosi(T) p, int n ) { //列表区间排序
    //insertionSort(p, n);
    switch ( getRandNum(0,2) ) { //随机选取排序算法。可根据具体问题的特点灵活选取或扩充
        // switch ( rand() % 3 ) {
        case 1:
            insertionSort(p, n);
            break; //插入排序
        case 2:
            selectionSort(p, n);
            break; //选择排序
        default:
            mergeSort(p, n);
            break; //归并排序
    }
}

template<typename T> // 对起始于位置p的n个元素  插入排序(稳定算法)
void List<T>::insertionSort(ListNodePosi(T) p, int n) {
    for(int r=0;r<n;r++){
        insertAfter(search(p->data,r,p),p->data); // 查找适当的位置并插入
        p=p->succ;
        remove(p->pred);
    }
}

template<typename T> // 列表的选择排序算法：对起始于位置p的n个元素排序
void List<T>::selectionSort(ListNodePosi(T) p,int n){
    ListNodePosi(T) head = p->pred;
    ListNodePosi(T) tail = p;
    for ( int i = 0; i < n; i++ )
        tail = tail->succ; //待排序区间为(head, tail) （左开右开）
    while ( 1 < n ) { //在至少还剩两个节点之前，在待排序区间内
        ListNodePosi(T) max = selectMax ( head->succ, n ); //找出最大者（歧义时后者优先）
        insertBefore ( tail, remove ( max ) ); //将其移至无序区间末尾（作为有序区间新的首元素）
        tail = tail->pred; // tail向前移动
        --n;
    }
}

template<typename T>  //从起始于位置p的n个元素中选出最大者
ListNodePosi(T) List<T>::selectMax(ListNodePosi(T) p, int n) {
    ListNodePosi(T) max=p;
    ListNodePosi(T) cur = p;
    for(int i=1; i < n ;i++){
        cur=cur->succ;
        if( (cur->data)>=(max->data)){
            max=cur;
        }
    }
    return max;
}

template <typename T>
void List<T>::merge(ListNodePosi(T)& p, int n, ListNodePosi(T) q, int m) { //有序列表的归并：当前列表中从q起的n个元素，和列表L中从q起的m个元素合并
    ListNodePosi(T) pp = p->pred; //借助前驱（可能是header），以便返回前 ...
    while ( 0 < m )  //在q尚未移出区间之前
        if ( ( 0 < n ) && ( p->data <= q->data ) ) {  //若p仍在区间内且v(p) <= v(q)，则
            if (q == (p = p->succ))
                break;
            n--;
        } //p归入合并的列表，并替换为其直接后继
        else {  //若p已超出右界或v(q) < v(p)，则
            insertBefore ( p, remove( ( q = q->succ )->pred ) ); m--; } //将q转移至p之前
    p = pp->succ; //确定归并后区间的（新）起点
}

template <typename T> //归并排序：对起始于位置p的n个元素排序
void List<T>::mergeSort(ListNodePosi(T) &p, int n) {  // 传递引用，是因为本指针指向的值会发生改变？
    if(n<2)
        return;
    int mid=n>>1;

    ListNodePosi(T) q=p;
    for(int i=0; i<mid;i++){ //均分列表
        q=q->succ; //列表2 的起点
    }
    mergeSort(p,mid);
    mergeSort(q,n-mid);
    merge(p, mid, q, n - mid);
}  //注意：排序后，p依然指向归并后区间的（新）起点

template <typename T> // 判断列表是否已经有序 返回值语义是：无序
bool List<T>::disordered() const{
    for(ListNodePosi(T) p=header->succ; p!=trailer->pred ;p=p->succ){ // 终止条件错了，应该是 p!= trailer->pred
        if( (p->data) > (p->succ->data) ){
            return true; // 无序，返回true
        }
    }
    return false;
}

template <typename T>
void List<T>::reverse(){
    ListNodePosi(T) p; // 这里的错误！！！ 分开写，这样定义的话 q不是节点指针型，而是节点
    ListNodePosi(T) q;
    // 原错误： ListNodePosi(T) p,q;
    p=header->succ;
    q=trailer->pred;
    T temp;
    while( (p!=q->pred) && (p!=q)){
        temp=p->data;
        p->data=q->data;
        q->data=temp;

        p=p->succ; q=q->pred;
    }
}
// #include "List_implementation.h"
#endif //LISTNODE_LIST_H
