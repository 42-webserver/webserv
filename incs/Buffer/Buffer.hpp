#ifndef BUFFER_HPP
# define BUFFER_HPP
#include <vector>
#include <iterator>
#include <memory>
#include <list>
# include "shared_ptr.hpp"
class BaseNode {

private:
    size_t partialPoint;
protected:
    size_t& getPartialPoint() {
        return partialPoint;
    }
public:
    virtual ~BaseNode() {}
    virtual char* data() = 0;
    virtual size_t size() const = 0;
    virtual size_t& current() = 0;
    virtual void erase() = 0;
};

class NormalNode : public BaseNode {
    std::vector<char> buffer;
    size_t currentPos;

public:
    NormalNode() : buffer(4 * 1024), currentPos(0) {}

    // char* data() override {
    //     return buffer.data();
    // }

    size_t size() const override {
        return buffer.size();
    }

    size_t& current() override {
        return currentPos + getPartialPoint();
    }

    void erase() override {
		//To do
    }
};

class LargeNode : public BaseNode {
    std::vector<char> buffer;
    size_t currentPos;

public:
    LargeNode() : buffer(64 * 1024), currentPos(0) {}

    // char* data() override {
    //     return buffer.data();
    // }

    size_t size() const override {
        return buffer.size() - getPartialPoint();
    }

    size_t& current() override {
        return currentPos + getPartialPoint();
    }

    void erase() override {
		//Todo
    }
};

class Buffer {
    typedef ft::shared_ptr<BaseNode> NodePtr;
    typedef std::list<NodePtr> NodeList;

public:
    class Iterator : public std::iterator<std::input_iterator_tag, char> {
    private:
	    NodeList::iterator nodeListIter;
        size_t posInNode;

    public:
        Iterator(NodeList::iterator nodeListIter, size_t posInNode = 0)
            : nodeListIter(nodeListIter), posInNode(posInNode) {}
        Iterator& operator++() {
            if (++posInNode >= (*nodeListIter)->size()) {
                ++nodeListIter;
                posInNode = 0;
            }
            return *this;
        }
        bool operator==(const Iterator& other) {
            return nodeListIter == other.nodeListIter &&
                (nodeListIter != other.nodeListIter || posInNode == other.posInNode);
        }
		char& operator*() {
            return (*nodeListIter)->data()[posInNode];
        }

        bool operator!=(const Iterator& other) {
            return nodeListIter != other.nodeListIter ||
                (nodeListIter != other.nodeListIter && posInNode != other.posInNode);
        }

    };

    void erase(Iterator first, Iterator last) {
        if (first.nodeListIter != nodes.begin() || first.posInNode != (*first.nodeListIter)->getPartialPoint()) {
            throw std::runtime_error("Erase must begin from the start of the buffer or the partial point.");
        }

        if (first.nodeListIter == last.nodeListIter) {
            (*first.nodeListIter)->getPartialPoint() = last.posInNode;
            return;
        }

        Iterator it = first;
        ++it;

        while (it != last && it.nodeListIter != nodes.end()) {
            (*it.nodeListIter)->getPartialPoint() = 0;
            it = erase(it, it);
        }

        if (it.nodeListIter != nodes.end()) {
            (*it.nodeListIter)->getPartialPoint() = it.posInNode;
        }

        nodes.erase(first.nodeListIter, it.nodeListIter);
    }
    Iterator begin() {
        return Iterator(nodes.begin());
    }

    Iterator end() {
        return Iterator(nodes.end());
    }


private:
    NodeList nodes;
    NodePtr baseNode;
};

#endif