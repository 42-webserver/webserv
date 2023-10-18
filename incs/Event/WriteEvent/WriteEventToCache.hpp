#ifndef WRITEEVENTTOCACHE_HPP
# define WRITEEVENTTOCACHE_HPP

# include <Event/WriteEvent/WriteEvent.hpp>
# include <Channel/SingleStreamable.hpp>
# include <vector>
# include <Channel/FileStream.hpp>
# include <Event/Exception/KqueueError.hpp>
# include <Event/WriteEvent/WriteEventToCacheHandler.hpp>
# include "../../../libs/shared_ptr/shared_ptr.hpp"
# include <FileManager/Cache/LruCache.hpp>
# include <Buffer/Buffer/IoReadAndWriteBuffer.hpp>
class WriteEventToCache: public WriteEvent, public SingleStreamable{
    public:
        WriteEventToCache(
            ft::shared_ptr<IoReadAndWriteBuffer> buffer,
            const std::string &path, std::string mode);
        virtual	~WriteEventToCache(void);

    public:
        virtual void	callEventHandler(void);
        virtual void	onboardQueue(void);
        virtual void	offboardQueue(void);
    private:
        ft::shared_ptr<SyncroWriteWithCache> _syncroWriteWithCache;
    public:   
        void _syncWithCache(ft::shared_ptr<SyncroWriteWithCache> syncroWriteWithCache);
        bool isFinalWriter(void);
};
#endif