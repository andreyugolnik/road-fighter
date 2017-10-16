#include <cstdlib>

#pragma once

/* 
Funciones para LISTAS: 

	void Delete()
	void Instance(List<T> &l)
	void Rewind(void)
	void Forward(void)
	void Next(void)
	void Prev(void)
	T *GetObj(void)
	void SetObj(T *o)
	LLink<T> *GetPos(void)
	bool EmptyP()
	bool EndP()
	bool LastP()
	bool BeginP()
	void Insert(T *o)
	void Add(T *o)
	void AddAfter(LLink<T> *pos,T *o)
	void AddBefore(LLink<T> *pos,T *o)
	bool Iterate(T *&o)
	T *ExtractIni(void)
	T *Extract(void)
	bool MemberP(T *o)
	T *MemberGet(T *o)
	bool MemberRefP(T *o)
	int Length()
	void Copy(List l)
	void Synchronize(List *l);
	void Append(List l) 
	bool DeleteElement(T *o)
	T *GetRandom(void)
	int SearchObjRef(T *o)
	int SearchObj(T *o)
	void SetNoOriginal(void)
	void SetOriginal(void)
	void Sort(bool *p(T *o1,T *o2))


	bool operator==(List<T> &l)
	T *operator[](int index)
*/

template <class T>
class LLink final
{
public:
    LLink(T* o, LLink<T>* n = 0)
    {
        obj = o;
        next = n;
    }

    ~LLink()
    {
        delete obj;
        if (next != 0)
            delete next;
    }

    inline LLink<T>* Getnext()
    {
        return next;
    }

    inline void Setnext(LLink<T>* n)
    {
        next = n;
    }

    inline T* GetObj()
    {
        return obj;
    }

    inline void SetObj(T* o)
    {
        obj = o;
    }

    void Anade(T* o)
    {
        if (next == 0)
        {
            LLink<T>* node = new LLink<T>(o);
            next = node;
        }
        else
        {
            next->Anade(o);
        }
    }

private:
    T* obj;
    LLink<T>* next;
};

template <class T>
class List final
{
public:
    List()
    {
        list = 0;
        act = 0;
        top = 0;
        original = true;
    }

    List(List<T>& l)
    {
        list = l.list;
        act = list;
        top = l.top;
        original = false;
    }

    ~List()
    {
        Delete();
    }

    void Delete()
    {
        if (original)
        {
            while (!EmptyP())
            {
                delete ExtractIni();
            }
            delete list;
        }

        list = 0;
        act = 0;
        top = 0;
    }

    void Instance(const List<T>& l)
    {
        list = l.list;
        act = list;
        top = l.top;
        original = false;
    }

    void Rewind()
    {
        act = list;
    }

    void Forward()
    {
        act = top;
    }

    void Next()
    {
        if (act != nullptr)
            act = act->Getnext();
    }

    void Prev()
    {
        LLink<T>* tmp;

        if (act != list)
        {
            tmp = list;
            while (tmp->Getnext() != act)
                tmp = tmp->Getnext();
            act = tmp;
        }
    }

    T* GetObj()
    {
        return act->GetObj();
    }

    void SetObj(T* o)
    {
        act->SetObj(o);
    }

    LLink<T>* GetPos()
    {
        return act;
    }

    bool EmptyP()
    {
        return list == 0;
    }

    bool EndP()
    {
        return act == 0;
    }

    bool LastP()
    {
        return act == top;
    }

    bool BeginP()
    {
        return act == list;
    }

    void Insert(T* o)
    {
        if (list == 0)
        {
            list = new LLink<T>(o);
            top = list;
        }
        else
        {
            list = new LLink<T>(o, list);
        }
    }

    void Add(T* o)
    {
        if (list == 0)
        {
            list = new LLink<T>(o);
            top = list;
        }
        else
        {
            top->Anade(o);
            top = top->Getnext();
        }
    }

    void AddAfter(LLink<T>* pos, T* o)
    {
        if (pos == 0)
        {
            if (list == 0)
            {
                list = new LLink<T>(o);
                top = list;
            }
            else
            {
                list = new LLink<T>(o, list);
            }
        }
        else
        {
            LLink<T>* nl = new LLink<T>(o);

            nl->Setnext(pos->Getnext());
            pos->Setnext(nl);
            if (nl->Getnext() == 0)
                top = nl;
        }
    }

    void AddBefore(LLink<T>* pos, T* o)
    {
        if (pos == list)
        {
            if (list == 0)
            {
                list = new LLink<T>(o);
                top = list;
            }
            else
            {
                list = new LLink<T>(o, list);
            }
        }
        else
        {
            LLink<T>*l, *nl = new LLink<T>(o);

            l = list;
            while (l->Getnext() != pos)
                l = l->Getnext();
            l->Setnext(nl);
            nl->Setnext(pos);

            if (pos == 0)
                top = nl;
        }
    }

    T* operator[](int index)
    {
        LLink<T>* tmp = list;
        while (tmp != 0 && index > 0)
        {
            tmp = tmp->Getnext();
            index--;
        } /* while */
        if (tmp == 0)
            throw;
        return tmp->GetObj();
    };

    bool Iterate(T*& o)
    {
        if (EndP())
            return false;
        o = act->GetObj();
        act = act->Getnext();
        return true;
    }

    T* ExtractIni()
    {
        LLink<T>* tmp;
        T* o;

        if (list == 0)
            return 0;
        o = list->GetObj();
        tmp = list;
        list = list->Getnext();
        tmp->Setnext(0);
        if (act == tmp)
            act = list;
        if (top == act)
            top = 0;
        tmp->SetObj(0);
        delete tmp;
        return o;
    }

    T* Extract()
    {
        LLink<T>*tmp, *tmp2 = 0;
        T* o;

        if (list == 0)
            return 0;
        tmp = list;
        while (tmp->Getnext() != 0)
        {
            tmp2 = tmp;
            tmp = tmp->Getnext();
        }
        o = tmp->GetObj();
        if (tmp2 == 0)
        {
            list = 0;
            top = 0;
            act = 0;
        }
        else
        {
            tmp2->Setnext(0);
            top = tmp2;
        }

        if (act == tmp)
            act = top;
        tmp->SetObj(0);
        delete tmp;
        return o;
    }

    bool MemberP(T* o)
    {
        LLink<T>* tmp;
        tmp = list;
        while (tmp != 0)
        {
            if (*(tmp->GetObj()) == *o)
                return true;
            tmp = tmp->Getnext();
        }
        return false;
    }

    T* MemberGet(T* o)
    {
        LLink<T>* tmp;
        tmp = list;
        while (tmp != 0)
        {
            if (*(tmp->GetObj()) == *o)
                return tmp->GetObj();
            tmp = tmp->Getnext();
        }
        return 0;
    }

    bool MemberRefP(T* o)
    {
        LLink<T>* tmp;
        tmp = list;
        while (tmp != 0)
        {
            if (tmp->GetObj() == o)
                return true;
            tmp = tmp->Getnext();
        }
        return false;
    }

    int Length()
    {
        LLink<T>* tmp;
        int count = 0;

        tmp = list;
        while (tmp != 0)
        {
            tmp = tmp->Getnext();
            count++;
        }
        return count;
    }

    void Copy(List l)
    {
        List<T> ltmp;
        T* o;
        Delete();
        original = true;

        ltmp.Instance(l);
        ltmp.Rewind();
        while (ltmp.Iterate(o))
        {
            o = new T(*o);
            Add(o);
        } /* while */
        Synchronize(&l);
    }

    void Synchronize(List* l)
    {
        LLink<T>* ll;

        ll = l->list;
        act = list;
        while (ll != 0 && ll != l->act)
        {
            ll = ll->Getnext();
            if (act != 0)
                act = act->Getnext();
        }
    }

    void Append(List l)
    {
        T* o;

        l.Rewind();
        while (l.Iterate(o))
        {
            o = new T(*o);
            Add(o);
        }
    }

    bool DeleteElement(T* o)
    {
        LLink<T>*tmp1, *tmp2;

        tmp1 = list;
        tmp2 = 0;
        while (tmp1 != 0 && tmp1->GetObj() != o)
        {
            tmp2 = tmp1;
            tmp1 = tmp1->Getnext();
        }

        if (tmp1 != 0)
        {
            if (tmp2 == 0)
            {
                /* Eliminar el primer elemento de la lista: */
                list = list->Getnext();
                tmp1->Setnext(0);
                if (act == tmp1)
                    act = list;
                tmp1->SetObj(0);
                delete tmp1;
            }
            else
            {
                /* Eliminar un elemento intermedio: */
                tmp2->Setnext(tmp1->Getnext());
                if (act == tmp1)
                    act = tmp1->Getnext();
                if (top == tmp1)
                    top = tmp2;
                tmp1->Setnext(0);
                tmp1->SetObj(0);
                delete tmp1;
            }
            return true;
        }
        else
        {
            return false;
        }
    }

    T* GetRandom()
    {
        int i, l = Length();
        i = ((rand() * l) / RAND_MAX);
        if (i == l)
            i = l - 1;

        return operator[](i);
    }

    bool operator==(List<T>& l)
    {
        LLink<T>*tmp1, *tmp2;

        tmp1 = list;
        tmp2 = l.list;
        while (tmp1 != 0 && tmp2 != 0)
        {
            if (!((*(tmp1->GetObj())) == (*(tmp2->GetObj()))))
                return false;
            tmp1 = tmp1->Getnext();
            tmp2 = tmp2->Getnext();
        }
        return tmp1 == tmp2;
    }

    int SearchObjRef(T* o)
    {
        LLink<T>* tmp;
        int pos = 0;

        tmp = list;
        while (tmp != 0)
        {
            if ((tmp->GetObj()) == o)
                return pos;
            tmp = tmp->Getnext();
            pos++;
        }
        return -1;
    }

    int SearchObj(T* o)
    {
        LLink<T>* tmp;
        int pos = 0;

        tmp = list;
        while (tmp != 0)
        {
            if (*(tmp->GetObj()) == *o)
                return pos;
            tmp = tmp->Getnext();
            pos++;
        }
        return -1;
    }

    void Sort(bool (*p)(T* o1, T* o2))
    {
        LLink<T>*l1, *l2;
        T* tmp;

        l1 = 0;
        l2 = list;
        while (l2 != 0)
        {
            if (l1 != 0 && l2 != 0)
            {
                if (!(*p)(l1->GetObj(), l2->GetObj()))
                {
                    tmp = l1->GetObj();
                    l1->SetObj(l2->GetObj());
                    l2->SetObj(tmp);
                }
            }
            l1 = l2;
            l2 = l2->Getnext();
        }
    }

    void SetNoOriginal()
    {
        original = false;
    }
    void SetOriginal()
    {
        original = true;
    }

private:
    bool original;
    LLink<T>* list;
    LLink<T>* top;
    LLink<T>* act;
};
