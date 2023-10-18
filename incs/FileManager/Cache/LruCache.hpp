#ifndef LRUCACHE_HPP
# define LRUCACHE_HPP

# include <iostream>
# include <list>
# include <map>
# include <vector>
# include <Buffer/Buffer/IoReadAndWriteBuffer.hpp>
# include "../../../libs/shared_ptr/shared_ptr.hpp"
# include <sys/stat.h>
# include <FileManager/Exception/FileNotExistException.hpp>


typedef enum{
	e_reading,
	e_writing,
	e_done
}	e_cache_node_status;


class LruCacheNode;
class SyncroWriteWithCache{
public:
	SyncroWriteWithCache(LruCacheNode &lruCache);
	~SyncroWriteWithCache(void);
private:
	LruCacheNode &lruCache;
	int _thisWriterNum;
public:
	bool isFinalWriter(void);
};

class SyncroReadWithCache{
public:
	SyncroReadWithCache(LruCacheNode &lruCache);
	~SyncroReadWithCache(void);
private:
	LruCacheNode &lruCache;

};
class LruCacheNode {
	private:
		std::vector<char>	_content;
		e_cache_node_status	_status;
		int					_finalWriterNum;
	public:
		LruCacheNode(void);
		LruCacheNode(ft::shared_ptr<IoReadAndWriteBuffer> buffer);
		~LruCacheNode(void);
		bool isUpdatedContent(void);
		e_cache_node_status getStatus(void);
		void updateFinsish(void);
		void setWriting(void);
		void setReading(void);
		bool getFinalWriterNum(void);
		bool isFinalWriter(int writerNum);
		std::vector<char>	&getContent(void);
		size_t				getContentSize(void);
		ft::shared_ptr<SyncroReadWithCache> buildSyncroReadWithCache(void);
		ft::shared_ptr<SyncroWriteWithCache> buildSyncroWriteWithCache(void);
};




class LruCache {
	public:
	class FailToGetException: public std::exception {
		public:
			virtual const char *what() const throw();
	};
	public:
		typedef std::list< std::pair<std::string, LruCacheNode> > lru_list_t;
		typedef std::map< std::string, lru_list_t::iterator > cache_map_t;
	public:
		static const int _BlockSize = 4 * 1024;
		static const int _capacity = 4 * 1024;
	private:
		lru_list_t	_lru_list; 
		std::vector<char>	empty;
		cache_map_t	_cache; 
	public:
		LruCache(void);
		~LruCache(void);

	private:
		void _readToCache(const std::string &uri);
		void _writeToCache(const std::string &uri, ft::shared_ptr<IoReadAndWriteBuffer> buffer);
	public:
		bool hit(const std::string &uri);
		const std::vector<char>	&get(const std::string &uri);
		// std::vector<char>::iterator	getIter(const std::string &uri);
		 size_t				getCacheContentSize(const std::string &uri);
		// void				put(std::string uri, const std::vector<char> &content);
		void				put(const std::string& uri);
		void 				put(const std::string& uri, ft::shared_ptr<IoReadAndWriteBuffer> buffer);
		void				deleteContent(const std::string &uri);
};

#endif
