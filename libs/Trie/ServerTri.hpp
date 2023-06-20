#ifndef SERVERTRI_HPP
# define SERVERTRI_HPP

# include <iostream>
# include <vector>
# include <sstream>
# include "../shared_ptr/shared_ptr.hpp"
# include "../../incs/Server/VirtualServer.hpp"

typedef enum NodeType{
	Socket,
	Server,
	NONE
} NodeType;

class ServerTrie {
private:
    NodeType _type;
    ft::shared_ptr<VirtualServer> _data;
    std::map<bool, ServerTrie> _next;
public:
    ServerTrie(): _type(NONE) {}

    void insert(ft::shared_ptr<VirtualServer> curVirtualServer) {
        std::vector<bool> ipBits = convertIPToVector(curVirtualServer->getIP());
        _insert(ipBits, curVirtualServer);
    }
    void _insert(const std::vector<bool> &ipBits, ft::shared_ptr<VirtualServer> curVirtualServer, 
    std::vector<bool>::size_type idx = 0) {
        if (idx == ipBits.size()) {
            _type = Server;
            this->_data = curVirtualServer;
        } else {
            _next[ipBits[idx]]._insert(ipBits, curVirtualServer, idx + 1);
        }
    }

    ft::shared_ptr<VirtualServer> getData(const std::vector<bool> &ipBits, 
    std::vector<bool>::size_type idx = 0) throw (std::exception) {
        if (idx == ipBits.size()) {
            if (isHasData()) {
                return _data;
            } else {
                throw FailToFindDataInTrie();
            }
        } else {
            if (_next.count(ipBits[idx]) == 0) {
                throw FailToFindDataInTrie();
            } else {
                return _next[ipBits[idx]].getData(ipBits, idx + 1);
            }
        }
    }
	bool isHasData() {
		return (_type == Server || _type == Socket);
	}
    ft::shared_ptr<VirtualServer> FindVirtualServer(const std::string &str) {
        ft::shared_ptr<VirtualServer> result;
		std::vector<bool> ipBits = convertIPToVector(str);
        std::vector<bool> current;
        _longestPrefix(ipBits, 0, current, result);
        return result;
    }
	void checkingSocketInTrie(){
		if (_type == Server) {
			_type = Socket;
		}
		else {
			for (std::map<bool, ServerTrie>::iterator it = _next.begin(); it != _next.end(); ++it) {
				it->second.checkingSocketInTrie();
			}
		}
	}

private:
std::vector<bool> convertIPToVector(const std::string& ip) {
    std::vector<bool> result(32);
    std::stringstream ss(ip);
    std::string segment;
    
    int segNum = 0;
    while(std::getline(ss, segment, '.')) {
        int num = std::stoi(segment);
        
        for (int i = 7; i >= 0; --i) {
            result[segNum * 8 + i] = num % 2;
            num /= 2;
        }
        
        ++segNum;
    }
    
    return result;
}
    void _longestPrefix(const std::vector<bool> &ipBits, std::vector<bool>::size_type 
    idx, std::vector<bool> &current, 
	ft::shared_ptr<VirtualServer> &result) {
        if (isHasData()) {
            result = this->_data;
        }
        if (idx == ipBits.size() || _next.count(ipBits[idx]) == 0) {
            return;
        } else {
			current.push_back(ipBits[idx]);
            _next[ipBits[idx]]._longestPrefix(ipBits, idx + 1, current, result);
            current.pop_back();
        }
    }
public:
	class FailToFindDataInTrie: public std::exception {
		public:
			virtual const char* what() const throw() {
		return "Fail to find data in trie";
		}
	};
    friend std::ostream& operator<<(std::ostream& os, const ServerTrie& trie);

};

std::ostream& operator<<(std::ostream& os, const ServerTrie& trie) {
    os << "type: " << trie._type << std::endl;
    os << "data: " << trie._data.get()->getIP() << std::endl;
    os << "next: " << std::endl;
    for (std::map<bool, ServerTrie>::const_iterator it = trie._next.begin(); it != trie._next.end(); ++it) {
        os << it->first << ": " << it->second << std::endl;
    }
    return os;
}

#endif