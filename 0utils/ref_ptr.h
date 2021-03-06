﻿#ifndef Ref_Ptr
#define Ref_Ptr

#include <string>

template <typename TT, typename DelFunT>
class RefPtr;

// deleter函数
template <typename TT>
struct DelPtr{    void operator() (TT*) { }    };

// 应用计数类    记录引用个数
template <typename TT, typename DelFunT = DelPtr<TT> >
class Ref {
private:
    Ref(TT* p)      :_ptr(p), _cnt(1)  { }
    ~Ref()          { if(_ptr) { _fun(_ptr); delete _ptr; } };
    void incr()     { _cnt++; };                //增加引用计数
    void decr()     { _cnt--; };                //减少引用计数
    int getCnt()    { return _cnt; };           //返回引用计数
    friend class RefPtr<TT, DelFunT>;           //设 指针管理类为友元，使能修改本类
private:
    int     _cnt;                //引用计数
    TT*     _ptr;                //对象指针
    DelFunT  _fun;
};

//
template <typename TT, typename DelFunT = DelPtr<TT> >
class RefPtr {
public:
    // 默认构造  NULL        不推荐使用，因为用再去拷贝对象，浪费性能
    //RefPtr()
    //    : _ref(new Ref<TT>(NULL)), _tag("") {
    //};
    
    RefPtr(TT *ptr) 
        : _ref(new Ref<TT, DelFunT>(ptr)), _tag("") {
    };
    
    RefPtr(const RefPtr<TT, DelFunT>& refptr) {
        _ref = refptr._ref;
        _ref->incr();
        _tag = "";
    }
    
    ~RefPtr() {
        _ref->decr();
        if (_ref->getCnt() <= 0) {
            delete _ref;
        }
    };
    
    RefPtr& operator=(TT* r) {
        _ref->decr();                                   //右值内 原计数器加一
        if (_ref->getCnt() <= 0) {              
            delete _ref;                                //原计数小于零 可以析构了
        }       
        _ref = new Ref<TT>(r);                          //赋值为 新计数器
        return *this;
    };
    
    //重载操作符＝        //内部引用指向改变
    RefPtr& operator=(const RefPtr<TT, DelFunT>& rhs) {
        rhs._ref->incr();                               //左值内 原计数器减一
        _ref->decr();                                   //右值内 原计数器加一
                
        if (_ref->getCnt() <= 0) {                
            delete _ref;                                //原计数小于零 可以析构了
        }                
        _ref = rhs._ref;                                //赋值为 新计数器
        return *this;
    };
    
    void setTag(const std::string value) { _tag = value; };
    inline std::string getTag()     { return _tag; };
    inline int getRefCnt()          { return _ref->getCnt(); }
    TT& operator*()                 { return *(_ref->_ptr); };  // 指针为空时奔溃  推荐用get()
    TT* operator->()                { return _ref->_ptr; };
    TT* get()                       { return _ref->_ptr; };
    
private:
    Ref<TT, DelFunT>*       _ref;        //计数类
    std::string             _tag;        //标签名
};

/*
#include <iostream>
#include <string.h>
using namespace std;

struct stt_test{    
    int    num;
    char*  ptr;
};
template <>
struct DelPtr<stt_test>{
    void operator() (stt_test* p) {
        cout<<" DelPtr "<<p->num<<" "<<p->ptr;
        cout<<"\n"<<__FUNCTION__<<endl;
    }
};
struct DelPtr2{
    void operator() (stt_test* p) {
        cout<<" DelPtr2 "<<p->num<<" "<<p->ptr;
        cout<<"\n"<<__FUNCTION__<<endl;
    }
};

int test_2() {
    
    RefPtr<stt_test>  p=new stt_test;
    p.get()->num = 11;
    p.get()->ptr = new char[11];
    memset(p.get()->ptr,'1',10);
{
    RefPtr<stt_test,DelPtr2>  p=new stt_test;
    p.get()->num = 22;
    p.get()->ptr = new char[22];
    memset(p.get()->ptr,'2',20);
}

    int *num = new int(11);
    {
        RefPtr<int> ptr1(num);
        ptr1.setTag("ptr1");
        {
            RefPtr<int> ptr2(ptr1);
            ptr2.setTag("ptr2");
            {
                int *num7 = new int(77);
                RefPtr<int> ptr7(num7);
                ptr2 = ptr7;
                ptr2.setTag("ptr2 变");
                {
                    *ptr1 = 22;
                    std::cout << "----------------获取引用计数 " << std::endl;
                    std::cout << "ptr1 have " << ptr1.getRefCnt() << "次 " << *ptr1.get() << ends << ptr1.get() << std::endl;
                    std::cout << "ptr2 have " << ptr2.getRefCnt() << "次 " << *ptr2.get() << ends << ptr2.get() << std::endl;
                    std::cout << "ptr2 have " << ptr7.getRefCnt() << "次 " << *ptr7.get() << ends << ptr7.get() << std::endl;
                }
            }
        }
    }
    return 0;
}

int main()
{
    return test_2();
}

*/

#endif /* Ref_Ptr */
