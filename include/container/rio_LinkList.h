#ifndef RIO_CNT_LINK_LIST_H
#define RIO_CNT_LINK_LIST_H

#include <misc/rio_Types.h>

namespace rio {

class LinkListNode
{
public:
    LinkListNode()
        : mPrev(nullptr)
        , mNext(nullptr)
    {
    }

private:
    // Disallow copying
    LinkListNode(const LinkListNode&);
    LinkListNode& operator=(const LinkListNode&);

public:
    // Get next node
    LinkListNode* next() const { return mNext; }
    // Get previous node
    LinkListNode* prev() const { return mPrev; }
    // Check if connected to any other node
    bool isLinked() const { return mNext || mPrev; }

private:
    // Initialize the node
    void init()
    {
        mNext = nullptr;
        mPrev = nullptr;
    }

    // Insert n after self
    void insertBack(LinkListNode* n);
    // Insert n before self
    void insertFront(LinkListNode* n);
    // Detach self from list
    void erase();

private:
    LinkListNode*   mPrev;  // Previous node
    LinkListNode*   mNext;  // Next node

    friend class LinkList;
};

class LinkList
{
    // Extensible circular, doubly-linked list

public:
    LinkList()
        : mStartEnd()
        , mCount(0)
    {
        mStartEnd.mNext = &mStartEnd;
        mStartEnd.mPrev = &mStartEnd;
    }

protected:
    // Disallow copying
    LinkList(const LinkList&);
    LinkList& operator=(const LinkList&);

public:
    // Is list empty
    bool isEmpty() const { return mCount == 0; }
    // Get list size (node count)
    s32 size() const { return mCount; }

    // Reverse order of nodes
    void reverse();

protected:
    // Push n to back
    void pushBack(LinkListNode* n) { insertBefore(&mStartEnd, n); }
    // Push n to front
    void pushFront(LinkListNode* n) { insertAfter(&mStartEnd, n); }
    // Pop node from back
    LinkListNode* popBack();
    // Pop node from front
    LinkListNode* popFront();

    // Insert n before basis
    void insertBefore(LinkListNode* basis, LinkListNode* n)
    {
        basis->insertFront(n);
        mCount += 1;
    }

    // Insert n after basis
    void insertAfter(LinkListNode* basis, LinkListNode* n)
    {
        basis->insertBack(n);
        mCount += 1;
    }

    // Detach n from list
    void erase(LinkListNode* n)
    {
        n->erase();
        mCount -= 1;
    }

    // Get list front
    LinkListNode* front() const { return mCount > 0 ? mStartEnd.mNext : nullptr; }
    // Get list back
    LinkListNode* back() const { return mCount > 0 ? mStartEnd.mPrev : nullptr; }

    // Get node at index
    LinkListNode* nth(s32 index) const;
    // Get index of n (-1 if not in list)
    s32 indexOf(const LinkListNode* n) const;

    // Swap n1 and n2
    static void swap(LinkListNode* n1, LinkListNode* n2);
    // Move n to after basis
    static void moveAfter(LinkListNode* basis, LinkListNode* n);
    // Move n to before basis
    static void moveBefore(LinkListNode* basis, LinkListNode* n);

    // Clear list by removing all nodes
    void clear();

    // Clear list (Does not clear node links)
    void unsafeClear()
    {
        mCount = 0;
        mStartEnd.mNext = &mStartEnd;
        mStartEnd.mPrev = &mStartEnd;
    }

protected:
    LinkListNode    mStartEnd;  // Root node
    s32             mCount;     // Node count (excluding root node)
};

}

#endif // RIO_CNT_LINK_LIST_H
