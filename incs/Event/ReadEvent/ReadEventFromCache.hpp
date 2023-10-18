#ifndef READFILECACHEHANDLER_HPP
# define READFILECACHEHANDLER_HPP

# include "ReadEvent.hpp"
# include <Channel/SingleStreamable.hpp>
# include <Channel/FileStream.hpp>
# include <Event/Exception/KqueueError.hpp>
# include <FileManager/Cache/LruCache.hpp>
# include <Event/ReadEvent/ReadEventFromCacheHandler.hpp>

class ReadEventFromCache : public ReadEvent, public SingleStreamable{
    public:
        ReadEventFromCache(std::vector<char> &content, 
        const std::string &path, std::string mode);
        virtual	~ReadEventFromCache(void);

    public:
        virtual void	callEventHandler(void);
        virtual void	onboardQueue(void);
        virtual void	offboardQueue(void);
    private:
        ft::shared_ptr<SyncroReadWithCache> _syncroReadWithCache;
    public:
        void _syncWithCache(ft::shared_ptr<SyncroReadWithCache> syncroReadWithCache);
};
#endif