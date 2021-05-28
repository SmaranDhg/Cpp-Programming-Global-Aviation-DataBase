#include "headers.h"
/*---------------------------Static Contents---------------------------*/
//its just the directory where data of ailine project is held
static string dd_ = "./al01";

struct IndexBound
{
    string msg;
};
struct ValueNotFound
{
    string msg;
};
/*---------------------------Linked list---------------------------*/
template <typename T>
class linkedlist_
{
private:
    struct _node
    {
        T data;
        _node *right;
    };
    struct _ll_iterator /* linked list iterator */
    {
        _node *_data;
        _ll_iterator(_node *data) : _data(data) {}
        auto operator*() -> T & { return _data->data; } /* derefrenceing */
        auto operator!=(_ll_iterator rhs) -> bool { return rhs._data != _data; }
        auto operator++() -> void { _data = _data->right; };
    };

public:
    /*---------------------------Insertions---------------------------*/
    auto einsert(T val) -> bool /* insert at end of the node */
    {
        _node *right_ = new _node;
        _head->data = val;
        _head->right = right_;
        _head = _head->right;
        _size++;
        return 1;
    }
    auto binsert(T val) -> bool /* insert at begining of list */
    {
        _node *node_ = new _node;
        node_->right = _root;
        node_->data = val;
        _root = node_;
        _size++;
        return 1;
    }
    auto ainsert(T val, T afterval) -> bool /* insert after a node */
    {
        auto after = _bfind(afterval);
        _node *node_ = new _node;
        node_->right = after->right;
        node_->data = val;
        after->right = node_;
        _size++;
        return 1;
    }
    auto binsert(T val, T beforeval) -> bool /* insert before a node */
    {
        auto before = _bfind(beforeval);
        _node *node_ = new _node;
        auto parent = _parent(before);
        return ainsert(val, parent->data);
    }
    auto find(T val, bool fromBegining = 1) -> _node * /* Finding the node matching the values */
    {
        return fromBegining ? _bfind(val) : _efind(val);
    }

    /*---------------------------Deletion---------------------------*/
    auto bdel() -> bool /* delete from beging */
    {
        auto temp = _root->right;
        delete _root;
        _root = temp;
        _size--;

        return 1;
    }
    auto edel() -> bool /* delete from end */
    {

        if (_size <= 1)
        {
            delete _root;
            delete _head;
            _root = new _node();
            _head = _root;
        }
        else
        {
            auto parent_h = _parent(_head);     /* parent of head */
            auto parent_ph = _parent(parent_h); /* parent of parent of head */
            parent_ph->right = parent_h->right;
            delete parent_h;
        }
        _size--;

        return 1;
    }
    auto adel(T val, T after) -> bool
    {
        auto start_node = find(after);
        auto val_node = _bfind(val, start_node);
        auto parent = _parent(val_node);
        parent->right = val_node->right;
        delete val_node;
        _size--;
        return 1;
    }

    /*---------------------------Accessors---------------------------*/
    /** ________________ \b Values _______________*/
    auto operator[](int index) -> T &
    {
        auto node = _root;
        while (index)
        {
            node = node->right;
            index--;
        }
        return node->data;
    }
    auto at(int index) -> T
    {
        return this->operator[](index);
    }
    auto size() -> int
    {
        return _size;
    }
    auto index(T val) -> int
    {

        int index = 0;
        auto node = _root;
        if (node->data == val)
            return index;
        while (node->right)
        {
            if (node->data == val)
                return index;
            index++;
            node = node->right;
        }
        throw ValueNotFound{"The value (" + val + ") provided doesn't exists in this linkelist."};
    }

    /** ________________ \b Iterators _______________*/
    auto begin() -> _ll_iterator { return _ll_iterator(_root); }
    auto end() -> _ll_iterator { return _ll_iterator(_head); }

    /*________________Printing_______________*/
    auto show() -> bool /* for printing stuff */
    {
        auto start = _root;
        print("[", false);
        while (start->right)
        {
            print(start->data, false);
            start = start->right;
            start->right and print(",", false);
        }
        print("]");
        return 1;
    }

    /*---------------------------Constructors---------------------------*/

    linkedlist_()
    {
        _root = new _node();
        _head = _root;
        _size = 0;
    }
    template <typename... a>
    linkedlist_(a... val)
    {
        _root = new _node();
        _head = _root;
        _size = 0;
        ((einsert(val)), ...);
    }

private:
    auto _parent(_node *node) -> _node * /* returns the parent of the node */
    {

        auto start = _root;
        while (start->right)
        {
            if (start->right equals node)
                return start;
            start = start->right;
        }
        return 0;
    }
    auto _bfind(T val, _node *start = NULL) -> _node * /* Find node from begining */
    {
        start = start ? start : _root; /* if start is null take _root as starting point else take argument itself */
        if (start->data equals val)
            return start;
        else
            while (start->right)
            {
                if (start->data equals val)
                    return start;
                start = start->right;
            }
        return 0;
    }
    auto _efind(T val) -> _node * /* Find from end */
    {
        auto start = _head;
        auto parent = _parent(start); /* here we search for parent instead */
        if (start->data equals val)
            return start;
        else
            while (parent)
            {
                if (start->data equals val)
                    return start;
                parent = _parent(start);
                start = parent;
            }
        return 0;
    }
    _node *_head;
    _node *_root;
    int _size;
};

/*---------------------------Implmenataion of queue as list---------------------------*/
template <typename T>
class queue_
{
public:
    auto add(T a) -> bool
    {
        return _data.binsert(a);
    }
    auto remove()
    {
        return _data.edel();
    }
    auto front() -> T /* start of the queue */
    {
        return _data[0];
    }
    auto back() -> T /* at back of the queue */
    {
        return _data[size() - 1];
    }
    auto size() -> int
    {
        return _data.size();
    }
    auto empty() -> bool
    {
        return size() == 0;
    }
    auto show() -> bool
    {
        return _data.show();
    }

private:
    int _max;
    linkedlist_<T> _data;
};

/*---------------------------Stack---------------------------*/
template <typename T>
class stack_
{
public:
    auto push(T val) -> bool /* just insert from begining */
    {
        return _dlist.binsert(val);
    }
    auto pop() -> T /* for pop just delete from begingng */
    {
        if (_dlist.size() == 0)
        {
            print("Stack underflow");
        }
        else
        {
            T val = _dlist[0];
            _dlist.bdel();
            return val;
        }
    }
    auto top() -> T
    {
        return _dlist[0];
    }
    auto bottom() -> T
    {
        return _dlist[size() - 1];
    }
    auto size() -> T
    {
        return _dlist.size();
    }
    auto show() -> bool
    {
        return _dlist.show();
    }

private:
    linkedlist_<T> _dlist;
};

/*---------------------------Binary search tree---------------------------*/
template <typename Key, typename Value>
class BinaryTree_
{

private:
    typedef bool Fn(Key, Key);
    struct _node
    {
        Key key;
        Value *val;
        _node *right;
        _node *left;
    };
    auto _gt_node(Key key, Value *val) -> _node *
    {
        // "node creation"_p;
        auto node = new _node;
        node->key = key;
        node->val = val;
        node->right = NULL;
        node->left = NULL;

        // "node created"_p;

        return node;
    }

    static auto _dominates(Key p, Key c) -> bool
    {
        return p > c;
    }
    _node *_root;
    int empty_flg = 1;
    int _size = 0;

public:
    auto insert(const Key &key, const Value &val, Fn *fn = 0)
    {
        if (!fn)
            fn = &_dominates;

        Value *val1 = new Value;
        *(val1) = val;
        if (_root == NULL)
        {
            _root = _gt_node(key, val1);
            // "here"_p;
        }
        else
        {
            queue_<_node *> q;
            q.add(_root);

            while (!q.empty())
            {
                auto temp = q.front();
                // "ok"_p;
                q.remove();
                if (!(fn(key, temp->key)))
                {
                    // print(white, "left");
                    if (temp->left == NULL)
                        temp->left = _gt_node(key, val1);
                    else
                        q.add(temp->left);
                }
                else
                {
                    // print(green, "right");

                    if (temp->right == NULL)
                        temp->right = _gt_node(key, val1);
                    else
                        q.add(temp->right);
                }
            }
        }
        empty_flg = 0;
        _size++;
        return 1;
    }
    BinaryTree_()
    {
        _root = NULL;
    }
    BinaryTree_(initializer_list<pair<Key, Value>> data)
    {
        _root = NULL;
        for (auto i = begin(data); i != end(data); i++)
            insert(i->first, i->second);
    }
    auto size() -> int
    {
        return _size;
    }
    auto gtroot() -> _node *
    {
        return _root;
    }
    /* return the value of correspond key, which it get from binary search tree */
    auto operator[](Key key) -> Value *
    {
        struct
        {
            auto operator()(Key key, _node *root) -> Value *
            {
                // print(root->right->key);
                if (root == NULL)
                    return 0;
                if (root->key == key)
                    return root->val;
                else if (key < root->key)
                    return this->operator()(key, root->left);
                else
                    return this->operator()(key, root->right);
            };
        } bin_search;

        return bin_search(key, _root);
    }
    auto getDefault(pair<Key, Value> key_default) -> Value &
    {
        auto ret = this->operator[](key_default.first);
        if (ret)
            return *ret;
        else
        {
            insert(key_default.first, key_default.second);
            return getDefault(key_default);
        }
    }

    auto empty() -> bool
    {
        return empty_flg;
    }
    auto Inorder(_node *root = 0, bool first = 1) -> bool
    {
        if (first)
            root = _root;
        if (root != NULL)
        {
            Inorder(root->left, 0);
            print("at:", root->key);
            Inorder(root->right, 0);
        }
        return 1;
    }

    /* apply operation inorder traversal of key value pair */
    template <typename Operate>
    auto traverse(Operate op)
    {
        struct
        {

            auto operator()(_node *root, Operate op) -> bool
            {
                if (root != NULL)
                {
                    this->operator()(root->left, op);
                    op(root->key, *(root->val));
                    this->operator()(root->right, op);
                }
                return 1;
            }
        } _in_order;
        return _in_order(_root, op);
    }

    auto Keys() -> linkedlist_<Key>
    {
        linkedlist_<string> ret;
        struct
        {
            auto operator()(_node *root, linkedlist_<string> &ret) -> bool
            {
                if (root != NULL)
                {
                    this->operator()(root->right, ret);
                    ret.einsert(root->key);
                    this->operator()(root->left, ret);
                }
                return 1;
            }
        } _in_order;
        _in_order(_root, ret);
        return ret;
    }
};

/*---------------------------Sparse Linkedlist---------------------------*/
// basically here we store duplicated data, as there input is given as (data,count)

template <typename T>
class SparseLinkedList_
{
private:
    linkedlist_<T> _data;
    linkedlist_<int> _count;
    int size = 0;

public:
    SparseLinkedList_(){

    };
    auto push(T val, int count) -> bool
    {
        if (count)
        {
            _data.einsert(val);
            _count.einsert(count);
            size += count;
            return 1;
        }
        return 0;
    }
    auto push(pair<T, int> data) -> bool { return push(data.first, data.second); }
    auto operator[](int index) -> T
    {

        if (index >= size)
            throw IndexBound{"Index Bound Sparse Matrix"};

        int i = 0;
        while (index >= 0)
        {
            if (index < _count[i])
                return _data[i];
            index -= _count[i];
            i++;
        }
        return 0;
    }

    SparseLinkedList_(initializer_list<pair<T, int>> args)
    {
        for (auto a = args.begin(); a != args.end(); a++)
            push(*a);
    }
};
