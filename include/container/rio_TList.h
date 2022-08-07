#ifndef RIO_CNT_TLIST_H
#define RIO_CNT_TLIST_H

#include <container/rio_LinkList.h>

namespace rio {

template <typename T>
class TList;

template <typename T>
class TListNode : public LinkListNode
{
public:
    TListNode(T data)
        : LinkListNode()
        , mData(data)
        , mList(nullptr)
    {
    }

    // Insert n after self
    void insertBack(TListNode<T>* n)
    {
        TList<T>* list = mList;
        if (list)
            list->insertAfter(this, n);
    }

    // Insert n before self
    void insertFront(TListNode<T>* n)
    {
        TList<T>* list = mList;
        if (list)
            list->insertBefore(this, n);
    }

    // Detach self from list
    void erase()
    {
        TList<T>* list = mList;
        if (list)
            list->erase(this);
    }

protected:
    T           mData;  // Pointer to data
    TList<T>*   mList;  // Parent list

    friend class TList<T>;
};

template <typename T>
class TList : public LinkList
{
    // Circular, doubly-linked list of objects of type T

public:
    TList()
        : LinkList()
    {
    }

    // Push obj to back
    void pushBack(TListNode<T>* obj)
    {
        obj->erase();
        obj->mList = this;
        LinkList::pushBack(obj);
    }

    // Push obj to front
    void pushFront(TListNode<T>* obj)
    {
        obj->erase();
        obj->mList = this;
        LinkList::pushFront(obj);
    }

    // Pop object node from back
    TListNode<T>* popBack() { return static_cast<TListNode<T>*>(LinkList::popBack()); }
    // Pop object node from front
    TListNode<T>* popFront() { return static_cast<TListNode<T>*>(LinkList::popFront()); }

    // Insert obj before basis
    void insertBefore(TListNode<T>* basis, TListNode<T>* obj)
    {
        LinkList::insertBefore(basis, obj);
    }

    // Insert obj after basis
    void insertAfter(TListNode<T>* basis, TListNode<T>* obj)
    {
        LinkList::insertAfter(basis, obj);
    }

    // Detach obj from list
    void erase(TListNode<T>* obj)
    {
        obj->mList = nullptr;
        LinkList::erase(obj);
    }

    // Get list front
    TListNode<T>* front() const { return static_cast<TListNode<T>*>(LinkList::front()); }
    // Get list back
    TListNode<T>* back() const { return static_cast<TListNode<T>*>(LinkList::back()); }

    // Get object node previous to obj
    TListNode<T>* prev(const TListNode<T>* obj) const
    {
        return obj->prev() == &mStartEnd ? nullptr
                                         : static_cast<TListNode<T>*>(obj->prev());
    }

    // Get object node next to obj
    TListNode<T>* next(const TListNode<T>* obj) const
    {
        return obj->next() == &mStartEnd ? nullptr
                                         : static_cast<TListNode<T>*>(obj->next());
    }

    // Get object node at index
    TListNode<T>* nth(s32 index) const
    {
        return static_cast<TListNode<T>*>(LinkList::nth(index));
    }

    // Get index of obj (-1 if not in list)
    s32 indexOf(const TListNode<T>* obj) const
    {
        return LinkList::indexOf(obj);
    }

    // Check obj in list
    bool isNodeLinked(const TListNode<T>* obj) const
    {
        return indexOf(obj) != -1;
    }

    // Swap obj1 and obj2
    static void swap(TListNode<T>* obj1, TListNode<T>* obj2)
    {
        LinkList::swap(obj1, obj2);
    }

    // Move obj to after basis
    static void moveAfter(TListNode<T>* basis, TListNode<T>* obj)
    {
        LinkList::moveAfter(basis, obj);
    }

    // Move obj to before basis
    static void moveBefore(TListNode<T>* basis, TListNode<T>* obj)
    {
        LinkList::moveBefore(basis, obj);
    }

    // Clear list by removing all nodes
    void clear() { LinkList::clear(); }

    // Clear list (Does not clear node links)
    void unsafeClear() { LinkList::unsafeClear(); }

public:
    class iterator
    {
    public:
        iterator(TListNode<T>* ptr)
            : mPtr(ptr)
        {
        }

    public:
        iterator& operator++()
        {
            mPtr = static_cast<TListNode<T>*>(mPtr->next());
            return *this;
        }

        iterator& operator--()
        {
            mPtr = static_cast<TListNode<T>*>(mPtr->prev());
            return *this;
        }

        T& operator*() const
        {
            return mPtr->mData;
        }

        T* operator->() const
        {
            return &mPtr->mData;
        }

        friend bool operator==(const iterator& it1, const iterator& it2)
        {
            return it1.mPtr == it2.mPtr;
        }

        friend bool operator!=(const iterator& it1, const iterator& it2)
        {
            return !(it1 == it2);
        }

    protected:
        TListNode<T>* mPtr;
    };

public:
    // Create iterator of list front
    iterator begin() const
    {
        return iterator(static_cast<TListNode<T>*>(mStartEnd.next()));
    }

    // Create iterator of list end
    iterator end() const
    {
        return iterator(static_cast<TListNode<T>*>(const_cast<LinkListNode*>(&mStartEnd)));
    }
};

}

#endif // RIO_CNT_TLIST_H
