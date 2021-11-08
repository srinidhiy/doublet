#include <iostream>
#include <stdexcept>
#include <vector>

template<class T>
class MinHeap {
public:
    MinHeap(int d);
    /* Constructor that builds a d-ary Min Heap
       This should work for any d >= 2,
       but doesn't have to do anything for smaller d.*/

    ~MinHeap();

    void add(T item, int priority);
    /* adds the item to the heap, with the given priority. */

    const T& peek() const;
    /* returns the element with smallest priority.
       Break ties however you wish.
       Throws an exception if the heap is empty. */

    void remove();
    /* removes the element with smallest priority.
       Break ties however you wish.
       Throws an exception if the heap is empty. */

    bool isEmpty();
    /* returns true iff there are no elements on the heap. */

private:
    // whatever you need to naturally store things.
    // You may also add helper functions here.
    int m_d;
    std::vector<std::pair<T, int> > values;
    void bubbleUp(int pos);
    void trickleDown(int pos);
    void swap(std::pair<T, int>& item1, std::pair<T, int>& item2);
};

template<class T>
MinHeap<T>::MinHeap(int d) {
    m_d = d;
}

template<class T>
MinHeap<T>::~MinHeap() {}

template<class T>
void MinHeap<T>::swap(std::pair<T, int>& item1, std::pair<T, int>& item2) {
    std::pair<T, int> temp = item1;
    item1 = item2;
    item2 = temp;
    // std::cout << "item getting switched is: " << item1.first << std::endl;
    // std::cout << "item switching is: " << item2.first << std::endl;
}

template<class T>
const T& MinHeap<T>::peek() const {
    if (values.size() == 0) {
        throw std::exception();
    } else if (values.size() == 1) {
        return values[0].first;
    } else {
        return values[0].first;
    }
}

template<class T>
void MinHeap<T>::bubbleUp(int pos) {
    // formula for parent is floor((pos-1)/d)
    if (pos > 0 && values[pos].second < values[(pos - 1) / m_d].second) {
        // swap
        swap(values[pos], values[(pos - 1) / m_d]);
        // recursion
        bubbleUp((pos - 1) / m_d);
    } else if (pos > 0 && values[pos].second == values[(pos - 1) / m_d].second) {
        if (values[pos].first < values[(pos - 1) / m_d].first) {
            swap(values[pos], values[(pos - 1) / m_d]);
            // recursion
            bubbleUp((pos - 1) / m_d);
        }
    }
}

template<class T>
void MinHeap<T>::add(T item, int priority) {
    values.push_back(std::pair<T, int>(item, priority));
    bubbleUp(values.size() - 1);
}

template<class T>
bool MinHeap<T>::isEmpty() {
    return (values.size() == 0);
}

template<class T>
void MinHeap<T>::trickleDown(int pos) {
    int child = m_d * pos + 1;
    // base case for when child is the last node
    if (child < (int)values.size()) {
        for (int i = 0; i < m_d; i++) {
            int child2 = m_d * pos + i + 1;
            if (child < (int)values.size()) {
                if (child2 < (int)values.size() && values[child].second > values[child2].second) {
                    child = child2;
                } else if (child2 < (int)values.size() && values[child].second == values[child2].second) {
                    if (values[child].first > values[child2].first) {
                        child = child2;
                    }
                }
            }
        }
        // std::cout << "child index: " << child << " child priority: " << values[child].second << std::endl;
        if (values[child].second < values[pos].second) {
            swap(values[pos], values[child]);
            trickleDown(child);
        } else if (values[child].second == values[pos].second) {
            if (values[child].first < values[pos].first) {
                swap(values[pos], values[child]);
                trickleDown(child);
            }
        }
    }
}

template<class T>
void MinHeap<T>::remove() {
    if (values.empty()) {
        throw std::exception();
    }
    // case for when it's on the last element in the vector - have to clear; cannot do pop back
    else if (values.size() == 1) {
        values.clear();
    } else {
        int vec_size = values.size();
        swap(values[0], values[vec_size - 1]);
        // erase last item
        values.pop_back();
        trickleDown(0);
    }
}
