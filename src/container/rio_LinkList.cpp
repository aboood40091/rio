#include <container/rio_LinkList.h>

namespace rio {

void LinkListNode::insertBack(LinkListNode* n)
{
    RIO_ASSERT(!n->isLinked());

    LinkListNode* next = mNext;

    this->mNext = n;
    n->mPrev = this;
    n->mNext = next;

    if (next)
        next->mPrev = n;
}

void LinkListNode::insertFront(LinkListNode* n)
{
    RIO_ASSERT(!n->isLinked());

    LinkListNode* prev = mPrev;

    this->mPrev = n;
    n->mPrev = prev;
    n->mNext = this;

    if (prev)
        prev->mNext = n;
}

void LinkListNode::erase()
{
    RIO_ASSERT(isLinked());

    if (mPrev)
        mPrev->mNext = mNext;

    if (mNext)
        mNext->mPrev = mPrev;

    init();
}

LinkListNode* LinkList::popBack()
{
    LinkListNode* n;

    if (mCount > 0)
    {
        n = mStartEnd.mPrev;
        erase(n);
    }
    else
    {
        n = nullptr;
    }

    return n;
}

LinkListNode* LinkList::popFront()
{
    LinkListNode* n;

    if (mCount > 0)
    {
        n = mStartEnd.mNext;
        erase(n);
    }
    else
    {
        n = nullptr;
    }

    return n;
}

LinkListNode* LinkList::nth(s32 index) const
{
    LinkListNode* n;

    if ((u32)index < (u32)mCount)
    {
        n = mStartEnd.mNext;
        for (s32 i = 0; i < index; i++)
            n = n->mNext;
    }
    else
    {
        RIO_ASSERT(false);
        n = nullptr;
    }

    return n;
}

s32 LinkList::indexOf(const LinkListNode* n) const
{
    LinkListNode* node = mStartEnd.mNext;
    s32 i = 0;

    while (node != &mStartEnd)
    {
        if (node == n)
            return i;

        node = node->mNext;
        i++;
    }

    return -1;
}

void LinkList::clear()
{
    LinkListNode* node = mStartEnd.mNext;
    while (node != &mStartEnd)
    {
        LinkListNode* n = node;
        node = node->mNext;
        n->init();
    }

    unsafeClear();
}

void LinkList::swap(LinkListNode* n1, LinkListNode* n2)
{
    RIO_ASSERT(n1->mPrev && n1->mNext && n2->mPrev && n2->mNext);

    if (n1 != n2)
    {
        LinkListNode* n1_prev = n1->mPrev;
        LinkListNode* n2_prev = n2->mPrev;

        if (n2_prev != n1)
        {
            n1->erase();
            n2_prev->insertBack(n1);
        }

        if (n1_prev != n2)
        {
            n2->erase();
            n1_prev->insertBack(n2);
        }
    }
}

void LinkList::moveAfter(LinkListNode* basis, LinkListNode* n)
{
    RIO_ASSERT(basis->mPrev && basis->mNext && n->mPrev && n->mNext);

    if (basis != n)
    {
        n->erase();
        basis->insertBack(n);
    }
}

void LinkList::moveBefore(LinkListNode* basis, LinkListNode* n)
{
    RIO_ASSERT(basis->mPrev && basis->mNext && n->mPrev && n->mNext);

    if (basis != n)
    {
        n->erase();
        basis->insertFront(n);
    }
}

void LinkList::reverse()
{
    if (mCount > 0)
    {
        LinkListNode* front = mStartEnd.mNext;
        LinkListNode* back = mStartEnd.mPrev;

        do
        {
            LinkListNode* n = back->mPrev;
            back->erase();
            front->insertFront(back);
            back = n;
        }
        while (back != front);
    }
}

}
